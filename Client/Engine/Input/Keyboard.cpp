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

bool Keyboard::GetKey(Scancode scancode)
{
	KeyState& key_state = key_states_[static_cast<uint32_t>(scancode)];
	return key_state.is_down;
}

bool Keyboard::GetKeyDown(Scancode scancode)
{
	KeyState& key_state = key_states_[static_cast<uint32_t>(scancode)];
	return key_state.is_down && !key_state.was_down;
}

bool Keyboard::GetKeyUp(Scancode scancode)
{
	KeyState& key_state = key_states_[static_cast<uint32_t>(scancode)];
	return !key_state.is_down && key_state.was_down;
}

void Keyboard::OnEvent(const Event& kEvent)
{
	const uint32_t& kType = kEvent.type;

	if (kType == static_cast<uint32_t>(EventType::kKeyPressed) && !kEvent.key.is_repeat ||
		kType == static_cast<uint32_t>(EventType::kKeyReleased))
	{
		// WORD key_code = kEvent.key.key_code;
		uint32_t scancode = kEvent.key.scancode;
			
		auto it = key_states_.find(scancode);
		if (it != key_states_.end())
		{
			KeyState& key_state = it->second;
			key_state.is_down = kType == static_cast<uint32_t>(EventType::kKeyPressed);
		}
	}

	if (kType == static_cast<uint32_t>(EventType::kKeyPressed) ||
		kType == static_cast<uint32_t>(EventType::kKeyReleased))
	{
		key_event.Execute(kEvent.key.scancode, kEvent.key.is_repeat);
	}
}
