#pragma once

#include "../PrecompiledHeaders.h"
#include "Core/Base.h"
#include "../SmartComponent/SmartComponent.h"


namespace ers {

typedef uint32_t entity_id_t;
typedef uint32_t entity_id;

typedef std::type_index component_id_t;

	class EntityRegistrySystem {

	public:
		EntityRegistrySystem() {
			m_id_counter = 1;
			ERS_LOG_INFO("ERS Initialized");

		}
		~EntityRegistrySystem() {
			m_map_entity_components_data.clear();
			m_map_component_common_entitites.clear();

			ERS_LOG_INFO("ERS Destructed");
		}

	public:

		_NODISCARD entity_id_t add_entity() {

			uint32_t new_id = m_id_counter;
			m_id_counter++;
			m_map_entity_components_data.emplace(new_id, std::unordered_map<component_id_t, SmartComponent>());

			ERS_LOG_INFO("Added entity. ID:", new_id);

			return new_id;

		}

		void delete_entity(entity_id_t entity_id) {

			if (!does_entity_exists(entity_id)) {
				ERS_LOG_ERROR("Trying to delete entity", entity_id, "that does not exist");
				return;
			}

			for (auto& [component_id, component_data] : m_map_entity_components_data.at(entity_id)) {

				m_map_component_common_entitites.at(component_id).erase(entity_id);

				ERS_LOG_INFO("Destructed component ", component_id.name(), "at entity ID:", entity_id);
			}

			m_map_entity_components_data.erase(entity_id);

			ERS_LOG_INFO("Deleted entity. ID:", entity_id);

		}

		template<typename A>
		inline A* add_component(entity_id_t entity_id) {

			component_id_t component_id = typeid(A);

			if (!does_entity_exists(entity_id)) {
				ERS_LOG_ERROR("Trying to add component", component_id.name(), "to entity", entity_id, "that does not exist");
				return nullptr;
			}

			if (does_entity_has_component(entity_id, component_id)) {
				ERS_LOG_WARNING("Adding component", component_id.name(), "to entity", entity_id, "that already has it");
				return ((A*)m_map_entity_components_data.at(entity_id).at(component_id).data);
			}

			m_map_entity_components_data.at(entity_id).emplace(component_id, A());

			if (!does_component_exist(component_id)) {
				m_map_component_common_entitites.emplace(component_id, std::unordered_set<entity_id_t>());
				ERS_LOG_INFO("Added component", component_id.name(), "to common components map");
			}

			m_map_component_common_entitites.at(component_id).insert(entity_id);

			ERS_LOG_INFO("Added component", component_id.name(), "to", entity_id);
			return ((A*)m_map_entity_components_data.at(entity_id).at(component_id).data);

		}

		template<typename G>
		_NODISCARD inline G* get_component(entity_id_t entity_id) {

			component_id_t component_id = typeid(G);

			if (!does_entity_exists(entity_id)) {
				ERS_LOG_ERROR("Trying to get component", component_id.name(), "from entity", entity_id, "that does not exist");
				return nullptr;
			}

			if (!does_component_exist(component_id)) {
				ERS_LOG_ERROR("Trying to get component that does not exist", component_id.name(), "from entity", entity_id);
				return nullptr;
			}

			if (!does_entity_has_component(entity_id, component_id)) {
				ERS_LOG_ERROR("Trying to get component", component_id.name(), "from entity", entity_id, "that does not have it");
				return nullptr;
			}

			return ((G*)m_map_entity_components_data.at(entity_id).at(component_id).data);

		}

		template<typename D>
		inline void delete_component(entity_id_t entity_id) {

			component_id_t component_id = typeid(D);

			if (!does_entity_exists(entity_id)) {
				ERS_LOG_ERROR("Trying to delete component", component_id.name(), "from entity", entity_id, "that does not exsist");
				return;
			}

			//if (!does_component_exist(component_id)) {
			//	ERS_LOG_ERROR("Trying to delete component that does not exsist", component_id.name(), "from entity", entity_id);
			//	return;
			//}

			if (!does_entity_has_component(entity_id, component_id)) {
				ERS_LOG_ERROR("Trying to delete component", component_id.name(), "from entity", entity_id, "that does not have it");
				return;
			}

			m_map_entity_components_data.at(entity_id).erase(component_id);
			m_map_component_common_entitites.at(component_id).erase(entity_id);

			ERS_LOG_INFO("Deleted component", component_id.name(), "from entity", entity_id);

		}

		template<typename...T>
		_NODISCARD inline const std::unordered_set<entity_id_t> get_by_common_components() {

			const size_t elements_num = sizeof...(T);
			std::unordered_set<entity_id_t> result;

			if (elements_num == 0) {
				ERS_LOG_ERROR("Trying to get common set of entities with empty component filter");
				return result;
			}

			std::array<component_id_t, elements_num> arr_component_ids = { typeid(T)... };

			if (!m_map_component_common_entitites.contains(arr_component_ids[0])) {
				ERS_LOG_ERROR("Trying to get common set of entities with component filter", arr_component_ids[0].name(), "that does not exsist");
				return result;
			}

			result = m_map_component_common_entitites.at(arr_component_ids[0]);

			std::unordered_set<entity_id_t> set_temp;

			for (component_id_t curr_component_id : arr_component_ids) {

				if (!m_map_component_common_entitites.contains(curr_component_id)) {
					ERS_LOG_ERROR("Trying to get common set of entities with component filter", curr_component_id.name(), "that does not exsist");
					return std::unordered_set<entity_id_t>();
				}

				if (m_map_component_common_entitites.at(curr_component_id) != result) {

					for (entity_id_t curr_entity_id : result) {

						if (does_entity_has_component(curr_entity_id, curr_component_id)) {
							set_temp.insert(curr_entity_id);
						}

					}

					result = set_temp;
					set_temp.clear();

				}

			}

			return result;

		}

		template<typename...T>
		_NODISCARD inline const std::unordered_set<entity_id_t> get_by_common_components(std::unordered_set<entity_id_t>&& range) {

			const size_t elements_num = sizeof...(T);
			std::unordered_set<entity_id_t> result;

			if (elements_num == 0) {
				ERS_LOG_ERROR("Trying to get common set of entities with empty component filter");
				return result;
			}

			std::array<component_id_t, elements_num> arr_component_ids = { typeid(T)... };

			if (!m_map_component_common_entitites.contains(arr_component_ids[0])) {
				ERS_LOG_ERROR("Trying to get common set of entities with component filter", arr_component_ids[0].name(), "that does not exsist");
				return result;
			}

			result = m_map_component_common_entitites.at(arr_component_ids[0]);

			std::unordered_set<entity_id_t> set_temp;

			for (component_id_t curr_component_id : arr_component_ids) {
				if (!m_map_component_common_entitites.contains(curr_component_id)) {
					ERS_LOG_ERROR("Trying to get common set of entities with component filter", curr_component_id.name(), "that does not exsist");
					return std::unordered_set<entity_id_t>();
				}

				if (m_map_component_common_entitites.at(curr_component_id) != result) {

					for (entity_id_t curr_entity_id : result) {

						if (does_entity_has_component(curr_entity_id, curr_component_id)) {

							if (range.contains(curr_entity_id)) {

								set_temp.insert(curr_entity_id);
							}

						}

					}

					result = set_temp;
					set_temp.clear();

				}

			}

			std::set_intersection(result.begin(), result.end(), range.begin(), range.end(), std::inserter(set_temp, set_temp.begin()));

			return set_temp;

		}


		template<typename Component>
		_NODISCARD inline std::unordered_set<Component*> get_every_component_data() {

			component_id_t component_id = typeid(Component);

			if (!does_component_exist(component_id)) {
				ERS_LOG_ERROR("Trying to get data of all components", component_id.name(), "that do not exsist");
				return std::unordered_set<Component*>();
			}

			if (m_map_component_common_entitites.at(component_id).empty()) {
				ERS_LOG_WARNING("Trying to get data of all components", component_id.name(), "that do not have any entities assosiated with is");
				return std::unordered_set<Component*>();
			}

			std::unordered_set<Component*> set_return;

			for (entity_id_t entity_id : m_map_component_common_entitites.at(component_id)) {
				set_return.insert((Component*)m_map_entity_components_data.at(entity_id).at(component_id).data);
			}

			return set_return;
		}

		template<typename Component>
		_NODISCARD inline std::unordered_set<Component*> get_every_component_data(std::unordered_set<entity_id_t>&& range) {

			component_id_t component_id = typeid(Component);

			if (!does_component_exist(component_id)) {
				ERS_LOG_ERROR("Trying to get data of all components", component_id.name(), "that do not exsist");
				return std::unordered_set<Component*>();
			}

			if (m_map_component_common_entitites.at(component_id).empty()) {
				ERS_LOG_WARNING("Trying to get data of all components", component_id.name(), "that do not have any entities assosiated with is");
				return std::unordered_set<Component*>();
			}

			std::unordered_set<Component*> set_return;

			for (entity_id_t entity_id : m_map_component_common_entitites.at(component_id)) {
				if (range.contains(entity_id)) {
					set_return.insert((Component*)m_map_entity_components_data.at(entity_id).at(component_id).data);
				}
			}

			return set_return;
		}


	private:
		std::unordered_map<entity_id_t, std::unordered_map<component_id_t, SmartComponent>>	m_map_entity_components_data;
		std::unordered_map<component_id_t, std::unordered_set<entity_id_t>>		m_map_component_common_entitites;

	private:
		entity_id_t m_id_counter;

	private:
		inline bool does_entity_exists(entity_id_t entity_id) {
			return m_map_entity_components_data.contains(entity_id);
		};

		inline bool does_component_exist(component_id_t component_id) {
			return m_map_component_common_entitites.contains(component_id);
		};

		inline bool does_entity_has_component(entity_id_t entity_id, component_id_t component_id) {
			return m_map_entity_components_data.at(entity_id).contains(component_id);
		};
	};

	typedef EntityRegistrySystem Context;

}