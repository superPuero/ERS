#include "ERS_smart/src/ERS/EntityRegistrySystem.h"

struct Vec2 {
	float x = 0;
	float y = 0;
};

int main() {
	EntityRegistrySystem ers;

	std::vector<entity_id_t> vec_entities;

	std::cin.get();

	for (int i = 0; i < 100; i++) {
		entity_id_t e = ers.add_entity();
		ers.add_component<Vec2>(e);
		vec_entities.push_back(e);
	}
	std::cin.get();

	for (auto e : vec_entities){
		ers.delete_entity(e);
	
	}

	vec_entities.clear();

	std::cin.get();

	for (int i = 0; i < 100; i++) {
		entity_id_t e = ers.add_entity();
		ers.add_component<Vec2>(e);
		vec_entities.push_back(e);
	}
	std::cin.get();

	for (auto e : vec_entities) {
		ers.delete_entity(e);

	}

	vec_entities.clear();

	std::cin.get();
}