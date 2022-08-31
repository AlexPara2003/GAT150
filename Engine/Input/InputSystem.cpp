#include "InputSystem.h"
#include <SDL.h>
#include <iostream>

namespace neu{
	const uint32_t key_space = SDL_SCANCODE_SPACE;
	const uint32_t key_w = SDL_SCANCODE_W;
	const uint32_t key_a = SDL_SCANCODE_A;
	const uint32_t key_s = SDL_SCANCODE_S;
	const uint32_t key_d = SDL_SCANCODE_D;
	const uint32_t key_escape = SDL_SCANCODE_ESCAPE;

	void InputSystem::Initialize(){
		const uint8_t* keyboardState = SDL_GetKeyboardState(&m_numKeys);

		m_keyboardState.resize(m_numKeys);
		std::copy(keyboardState, keyboardState + m_numKeys, m_keyboardState.begin());

		m_prevKeyboardState = m_keyboardState;
	}

	void InputSystem::Shutdown(){
		
	}

	void InputSystem::Update(){
		SDL_Event event;
		SDL_PollEvent(&event);

		m_prevKeyboardState = m_keyboardState;

		const uint8_t* keyboardState = SDL_GetKeyboardState(nullptr);
		std::copy(keyboardState, keyboardState + m_numKeys, m_keyboardState.begin());

		m_prevMouseButtonState = m_mouseButtonState;
		int x, y;
		uint32_t buttons = SDL_GetMouseState(&x, &y);
		m_mousePosition = neu::Vector2{ x, y };

		m_mouseButtonState[0] = buttons & SDL_BUTTON_LMASK; 
		m_mouseButtonState[1] = buttons & SDL_BUTTON_MMASK;
		m_mouseButtonState[2] = buttons & SDL_BUTTON_RMASK; 
	}

	InputSystem::State InputSystem::GetKeyState(uint32_t key){
		State state = State::Idle;

		bool keyDown = GetKeyDown(key);
		bool prevKeyDown = GetPreviousKeyDown(key);

		if (keyDown && prevKeyDown) { state = State::Held; }
		else if (keyDown && !prevKeyDown) { state = State::Press; }
		else if (!keyDown && prevKeyDown) { state = State::Released; }
		else { state = State::Idle; }

		return state;
	}

	InputSystem::State InputSystem::GetButtonState(uint32_t button){
		State state = State::Idle;

		bool buttonDown = GetButtonDown(button);
		bool prevButtonDown = GetPreviousButtonDown(button);

		if (buttonDown && prevButtonDown) { state = State::Held; }
		else if (buttonDown && !prevButtonDown) { state = State::Press; }
		else if (!buttonDown && prevButtonDown) { state = State::Released; }
		else { state = State::Idle; }

		return state;
	}
}