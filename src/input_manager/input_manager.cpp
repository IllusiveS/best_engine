#include "input_manager.h"

#include <algorithm>

#include "backends/imgui_impl_sdl.h"

#include <iostream>
#include <string>

InputManager::InputManager()
	:mouseHorizontal(0), mouseVertical(0)
{
	SDL_SetRelativeMouseMode(SDL_TRUE);

	for(auto key = 0; key < SDL_NUM_SCANCODES; key++)
	{
		onClick[key] = false;
		onRelease[key] = false;
		wasProcessed[key] = false;
		isPressed[key] = false;
		wasPressed[key] = false;
	}
}

void InputManager::processAllKeys()
{
	SDL_Event e;

	mouseHorizontal = 0;
	mouseVertical = 0;

	//Handle events on queue
	while(SDL_PollEvent(&e) != 0)
	{
		ImGui_ImplSDL2_ProcessEvent(&e);

		for(auto key = 0; key < SDL_NUM_SCANCODES; key++)
		{
			onClick[key] = false;
			onRelease[key] = false;
			wasProcessed[key] = false;
		}

		if(e.type == SDL_KEYDOWN)
		{
			const auto code = e.key.keysym.scancode;

			wasProcessed[code] = true;
			isPressed[code] = true;

			if(!wasPressed[code])
			{
				onClick[code] = true;
			}
		}
		else if(e.type == SDL_KEYUP)
		{
			const auto code = e.key.keysym.scancode;

			wasProcessed[code] = true;
			isPressed[code] = false;

			if(wasPressed[code])
			{
				onRelease[code] = true;
			}
		}

		if(e.type == SDL_MOUSEMOTION)
		{
			mouseHorizontal = -e.motion.xrel;
			mouseVertical = -e.motion.yrel;
		}

		std::cout << std::to_string(mouseHorizontal) << std::endl;

		if(e.type == SDL_MOUSEBUTTONDOWN)
		{

		}
		
		if(e.type == SDL_MOUSEBUTTONUP)
		{

		}

	}
}

bool InputManager::isKeyPressed(const SDL_Scancode code) const 
{
	return isPressed[code];
}

float InputManager::getHorizontalMouse() const
{
	return 0.001f * mouseHorizontal;
}

float InputManager::getVerticalMouse() const
{
	return 0.001f * mouseVertical;
}
