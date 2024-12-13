#pragma once

#include "../PrecompiledHeaders.h"

class SmartComponent{
public:
	SmartComponent() {
		data = nullptr;
		destructor_lambda_ptr = nullptr;
		std::cout << "SmartComponent construct" << '\n';
	};

	SmartComponent(SmartComponent&& other) noexcept {
		data = other.data;
		destructor_lambda_ptr = other.destructor_lambda_ptr;
		other.destructor_lambda_ptr = nullptr;
		std::cout << "SmartComponent move" << '\n';
	};

	template<typename Component>
	inline SmartComponent(Component& component) noexcept {
		data = new Component(component);
		destructor_lambda_ptr = [](void* data_ptr) { delete static_cast<Component*>(data_ptr); };
		std::cout << "SmartComponent templated copy" << '\n';
	};


	template<typename Component>
	SmartComponent(Component&& component) noexcept {
		data = new Component(std::move(component));
		destructor_lambda_ptr = [](void* data_ptr) { delete static_cast<Component*>(data_ptr); };
		std::cout << "SmartComponent templated move" << '\n';
	};

	~SmartComponent() {
		if (destructor_lambda_ptr) {
			destructor_lambda_ptr(data);
		}
		std::cout << "Smart dead" << '\n';
	};

public:
	void* data;

private:
	void (*destructor_lambda_ptr)(void*);

};
