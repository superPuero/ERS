#include "ERS_smart/src/ERS/EntityRegistrySystem.h"

struct Vec2 {
	float* x;

	Vec2() {
		x = nullptr;
	}

	Vec2(Vec2&& other) noexcept{
		x = other.x;
		other.x = nullptr;
	}

	~Vec2() {
		delete x;
	}
};

int main() {
	ers::Context context;

	std::vector<ers::entity_id> vec;

	std::cin.get();

	for (int i = 0; i < 1000; i++) {
		ers::entity_id entity = context.add_entity();
		context.add_component<Vec2>(entity)->x = new float(15);
	}


	std::cin.get();

	context.~EntityRegistrySystem();

	std::cin.get();

}