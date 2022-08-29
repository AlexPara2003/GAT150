#pragma once
#include <string>
#include <variant>
#include <functional>

namespace neu {

	class GameObject;
	class Event {
	public:
		using functionPtr = std::function<void(const Event&)>;

		std::string name;
		GameObject* receiver = nullptr;
		std::variant<int, bool, float, std::string> data;
		
	};
}