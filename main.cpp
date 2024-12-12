#include "ERS_smart/src/ERS/EntityRegistrySystem.h"
#include <assert.h>

class Entity {
public:
	Entity(ers::entity context_entity_handle, ers::Context* context_ptr) : m_contex_entity_handle(context_entity_handle), m_context_ptr(context_ptr){}

	template<typename Component>
	Component* component_add() {
		return m_context_ptr->component_add<Component>(m_contex_entity_handle);
	}

	template<typename Component, typename...Args>
	Component* component_emplace(Args&&...args) {
		return m_context_ptr->component_emplace<Component>(m_contex_entity_handle, std::forward<Args>(args)...);
	}

	template<typename Component>
	void component_remove() {
		m_context_ptr->component_delete<Component>(m_contex_entity_handle);
	}

private:
	const ers::entity m_contex_entity_handle;
	ers::Context* m_context_ptr;
};

struct TransformComponent {
	float x, y;
};

int main() {
	ers::Context context;
	
	Entity ent(context.entity_add(), &context);
	
	ers::entity e = context.entity_add();

	context.component_emplace<TransformComponent>(e, {1,2});
	
}