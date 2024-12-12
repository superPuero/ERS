#pragma once

#include "Core/Base.h"
#include "../PrecompiledHeaders.h"
#include "../SmartComponent/SmartComponent.h"

namespace ers {

typedef uint32_t entity_id_t;
typedef entity_id_t entity;

typedef std::type_index component_id_t;

	class Context {

	public:
		Context() {
			m_id_counter = 1;
			ERS_LOG_INFO("ERS Initialized");
		}
		~Context() {
			m_map_entity_components_data.clear();
			m_map_component_common_entitites.clear();
			ERS_LOG_INFO("ERS Destructed");
		}

	public:
		[[nodiscard]] entity_id_t entity_add() {
			uint32_t new_id = m_id_counter;
			m_id_counter++;
			m_map_entity_components_data.insert({ new_id, std::unordered_map<component_id_t, SmartComponent>()});

			ERS_LOG_INFO("Added entity. ID:", new_id);

			return new_id;
		}

		void entity_delete(entity_id_t entity_id) {

			ERS_ASSERT(valid(entity_id));

			for (auto& [component_id, component_data] : m_map_entity_components_data.at(entity_id)) {
				m_map_component_common_entitites.at(component_id).erase(entity_id);
			}

			m_map_entity_components_data.erase(entity_id);

			ERS_LOG_INFO("Deleted entity. ID:", entity_id);
		}

		template<typename A>
		inline A* component_add(entity_id_t entity_id) {
			component_id_t component_id = typeid(A);

			ERS_ASSERT(valid(entity_id));
			ERS_ASSERT(!valid(entity_id, component_id));

			m_map_entity_components_data.at(entity_id).emplace( component_id, A() );

			if (!valid(component_id)) {
				m_map_component_common_entitites.insert({ component_id, std::unordered_set<entity_id_t>() });
				ERS_LOG_INFO("Added component", component_id.name(), "to common components map");
			}

			m_map_component_common_entitites.at(component_id).insert(entity_id);

			ERS_LOG_INFO("Added component", component_id.name(), "to", entity_id);

			return static_cast<A*>(m_map_entity_components_data.at(entity_id).at(component_id).data);
		}

		template<typename E>
		inline E* component_emplace(entity_id_t entity_id, E&& component) {

			component_id_t component_id = typeid(E);
				
			ERS_ASSERT(valid(entity_id));
			ERS_ASSERT(!valid(entity_id, component_id));

			m_map_entity_components_data.at(entity_id).emplace( component_id, std::forward<E>(component) );
			
			if (!valid(component_id)) {
				m_map_component_common_entitites.insert({ component_id, std::unordered_set<entity_id_t>() });
				ERS_LOG_INFO("Added component", component_id.name(), "to common components map");
			}

			m_map_component_common_entitites.at(component_id).insert(entity_id);

			ERS_LOG_INFO("Emplaced component", component_id.name(), "to", entity_id);
			return static_cast<E*>(m_map_entity_components_data.at(entity_id).at(component_id).data);
		}

		template<typename...E>
		std::enable_if_t<(sizeof...(E) > 0), void>
		inline component_emplace_pack(entity_id_t entity_id, E&&...component) {

			const size_t components_count = sizeof...(E);

			(component_emplace<E>(entity_id, std::forward<E>(component)),...);

			return;
		}

		template<typename C>
		inline void component_copy(entity_id_t from_entity_id, entity_id_t to_entity_id) {

			component_id_t component_id = typeid(C);

			ERS_ASSERT(valid(from_entity_id));
			ERS_ASSERT(valid(to_entity_id));

			ERS_ASSERT(valid(from_entity_id, component_id));
			ERS_ASSERT(!valid(to_entity_id, component_id));

			m_map_entity_components_data.at(to_entity_id).emplace( component_id, *component_get<C>(from_entity_id) );
			m_map_component_common_entitites.at(component_id).insert(to_entity_id);

			ERS_LOG_INFO("Copied component", component_id.name(), "from entity", from_entity_id, "to entity", to_entity_id);

			return;
		}

		template<typename...C>
		std::enable_if_t<(sizeof...(C) > 1)>
		inline component_copy(entity_id_t from_entity_id, entity_id_t to_entity_id) {

			(component_copy<C>(from_entity_id, to_entity_id),...);

			return;
		}


		template<typename G>
		[[nodiscard]] inline G* component_get(entity_id_t entity_id) {

			component_id_t component_id = typeid(G);

			ERS_ASSERT(valid(entity_id));
			ERS_ASSERT(valid(entity_id, component_id));

			return static_cast<G*>(m_map_entity_components_data.at(entity_id).at(component_id).data);
		}

		template<typename D>
		inline void component_delete(entity_id_t entity_id) {

			component_id_t component_id = typeid(D);

			ERS_ASSERT(valid(entity_id));
			ERS_ASSERT(valid(entity_id, component_id));

			m_map_entity_components_data.at(entity_id).erase(component_id);
			m_map_component_common_entitites.at(component_id).erase(entity_id);

			ERS_LOG_INFO("Deleted component", component_id.name(), "from entity", entity_id);

		}

		template<typename...T>
		[[nodiscard]] std::enable_if_t<sizeof...(T), const std::unordered_set<entity_id_t>>
		inline entities_get_by_commmon_component() {

			const size_t elements_num = sizeof...(T);
			std::unordered_set<entity_id_t> result;
			component_id_t arr_component_ids[elements_num] = {typeid(T)...};

			ERS_ASSERT(valid(arr_component_ids[0]));

			result = m_map_component_common_entitites.at(arr_component_ids[0]);
			std::unordered_set<entity_id_t> set_temp;

			for (component_id_t curr_component_id : arr_component_ids) {

				;if (!valid(curr_component_id)) {
					return std::unordered_set<entity_id_t>();
				}

				if (m_map_component_common_entitites.at(curr_component_id) != result) {
					for (entity_id_t curr_entity_id : result) {
						if (valid(curr_entity_id, curr_component_id)) {
							set_temp.insert(curr_entity_id);
						}
					}

					result = set_temp;
					set_temp.clear();

				}

			}
			return result;

		}

		template<typename Component>
		[[nodiscard]] inline std::unordered_set<Component*> components_data_get_all() {

			component_id_t component_id = typeid(Component);

			ERS_ASSERT(valid(component_id));

			std::unordered_set<Component*> set_return;

			for (entity_id_t entity_id : m_map_component_common_entitites.at(component_id)) {
				set_return.insert(static_cast<Component*>(m_map_entity_components_data.at(entity_id).at(component_id).data));
			}

			return set_return;
		}

		template<typename Component>
		[[nodiscard]] inline std::unordered_set<Component*> components_data_get_all(std::unordered_set<entity_id_t>&& range) {

			component_id_t component_id = typeid(Component);

			ERS_ASSERT(valid(component_id));

			std::unordered_set<Component*> set_return;

			for (entity_id_t entity_id : m_map_component_common_entitites.at(component_id)) {
				if (range.contains(entity_id)) {
					set_return.insert(static_cast<Component*>(m_map_entity_components_data.at(entity_id).at(component_id).data));
				}
			}

			return set_return;
		}

	private:
		[[nodiscard]] bool valid(entity_id_t entity_id) {
			return m_map_entity_components_data.contains(entity_id);
		}
		
		[[nodiscard]] bool valid(component_id_t component_id) {
			return m_map_component_common_entitites.contains(component_id);
		}

		[[nodiscard]] bool valid(entity_id_t entity_id, component_id_t component_id) {
			return m_map_entity_components_data.at(entity_id).contains(component_id);
		}

	private:
		std::unordered_map<entity_id_t, std::unordered_map<component_id_t, SmartComponent>>	m_map_entity_components_data;
		std::unordered_map<component_id_t, std::unordered_set<entity_id_t>>					m_map_component_common_entitites;

	private:
		entity_id_t m_id_counter;

	};
}