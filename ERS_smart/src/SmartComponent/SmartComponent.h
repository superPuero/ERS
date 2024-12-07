#pragma once

#include "../PrecompiledHeaders.h"

class SmartComponent {
public:
	SmartComponent() {
		data = nullptr;
		destructor_lambda_ptr = nullptr;
		std::cout << '\n' << "SmartComponent construct";
	};

	SmartComponent(SmartComponent&& other) noexcept {
		data = other.data;
		destructor_lambda_ptr = other.destructor_lambda_ptr;
		other.destructor_lambda_ptr = nullptr;
		std::cout << '\n' << "SmartComponent move";
	};

	template<typename Component>
	inline SmartComponent(Component&& component) noexcept {
		data = new Component(std::move(component));
		destructor_lambda_ptr = [](void* data_ptr) { delete static_cast<Component*>(data_ptr); };
		std::cout << '\n' << "SmartComponent templated move";
	};

	~SmartComponent() {
		if (destructor_lambda_ptr) {
			destructor_lambda_ptr(data);
		}
	};

public:
	void* data;

private:
	void (*destructor_lambda_ptr)(void*);
};
