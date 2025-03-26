#include "pch.h"
#include "Keyboard.h"

#include <ranges>

#include "Event/Events.h"

Keyboard::Keyboard() :
	key_states_()
{
}

void Keyboard::UpdateKeyStates()
{
	for (auto& key_state : key_states_ | std::views::values)
	{
		key_state.was_down = key_state.is_down;
	}
}

void Keyboard::Clear()
{
	std::lock_guard<std::mutex> lock(mutex_);

	for (auto& key_state : key_states_ | std::views::values)
	{
		key_state.is_down = false;
		key_state.was_down = false;
	}
}

bool Keyboard::GetKey(WORD key_code)
{
	KeyState& key_state = key_states_[key_code];
	return key_state.is_down;
}

bool Keyboard::GetKeyDown(WORD key_code)
{
	KeyState& key_state = key_states_[key_code];
	return key_state.is_down && !key_state.was_down;
}

bool Keyboard::GetKeyUp(WORD key_code)
{
	KeyState& key_state = key_states_[key_code];
	return !key_state.is_down && key_state.was_down;
}

void Keyboard::OnEvent(const Event& kEvent)
{
	const Type::uint32& kType = kEvent.type;

	if (kType == static_cast<Type::uint32>(EventType::kKeyPressed) && !kEvent.key.is_repeat ||
		kType == static_cast<Type::uint32>(EventType::kKeyReleased))
	{
		WORD key_code = kEvent.key.key_code;
			
		auto it = key_states_.find(key_code);
		if (it != key_states_.end())
		{
			KeyState& key_state = it->second;
			key_state.is_down = kType == static_cast<Type::uint32>(EventType::kKeyPressed);
		}
	}
}
