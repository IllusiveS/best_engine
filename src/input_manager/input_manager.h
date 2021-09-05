#pragma once

#include <SDL.h>
#undef main

class InputManager
{
	bool wasProcessed[SDL_NUM_SCANCODES];

	bool isPressed[SDL_NUM_SCANCODES];
	bool wasPressed[SDL_NUM_SCANCODES];

	bool onClick[SDL_NUM_SCANCODES];
	bool onRelease[SDL_NUM_SCANCODES];

public:
	InputManager();

	void processAllKeys();

	bool isKeyPressed(SDL_Scancode code) const;
};