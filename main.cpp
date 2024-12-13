#include "ERS_smart/src/ERS/EntityRegistrySystem.h"

struct Vec2 {
	float x, y;
};

struct TransformComponent {
	Vec2 position;
};

int main() {
	ers::Context context;

	auto e1 = context.entity_add();
	auto e2 = context.entity_add();

	context.component_emplace_pack<TransformComponent, Vec2>(e1, { 56.0f, 134.0f }, { 65.0f, 4564.0f });

	context.component_add<int>(e1) = 5;

	context.component_copy<TransformComponent, Vec2>(e1, e2);
	

	for (auto [t, v, i] : context.component_get_group<TransformComponent, Vec2, int>()) {
		std::cout << t.position.x << '\n';
		std::cout << t.position.x << '\n';
		std::cout << v.x << '\n';
		std::cout << v.y << '\n';
		std::cout << i << '\n';

		v.y++;
		t.position.x++;
	}
	
	auto& t = context.component_get<TransformComponent>(e1);


}