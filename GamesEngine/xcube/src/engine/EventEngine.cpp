#include "EventEngine.h"

EventEngine::EventEngine() : running(true) {
	for (int i = 0; i < Key::LAST; ++i) {
		keys[i] = false;
	}

	buttons[Mouse::BTN_LEFT] = false;
	buttons[Mouse::BTN_RIGHT] = false;
}

EventEngine::~EventEngine() {}

void EventEngine::pollEvents() {
	while (SDL_PollEvent(&event)) {
		if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) && event.key.repeat == 0) {
			updateKeys(event.key.keysym.sym, event.type == SDL_KEYDOWN);
		}

		if (event.type == SDL_QUIT) {
			keys[QUIT] = true;
		}

		buttons[Mouse::BTN_LEFT]  = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
		buttons[Mouse::BTN_RIGHT] = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
	}
}

void EventEngine::updateKeys(const SDL_Keycode &key, bool keyDown) {
	Key index;

	switch (key) {
		case SDLK_d:	index = Key::RIGHT; break;
		case SDLK_a:		index = Key::LEFT; break; 
		case SDLK_w:		index = Key::UP; break;
		case SDLK_s:		index = Key::DOWN; break;	
		case SDLK_ESCAPE:	index = Key::ESC; break;
		case SDLK_SPACE:	index = Key::SPACE; break;
		//case SDLK_e:	 index = Key::INTERACT1; break;		
		case SDLK_e: index = Key::INTERACT1; break;
		case SDLK_f: index = Key::INTERACT2; break;
		case SDLK_g: index = Key::INTERACT3; break;
		case SDLK_i: index = Key::QUIT; break;
		default:


			return;	// we don't care about other keys, at least now
	}

	keys[index] = keyDown;
}

void EventEngine::setPressed(Key key) {
    keys[key] = true;
}

void EventEngine::setPressed(Mouse btn) {
    buttons[btn] = true;
}

bool EventEngine::isPressed(Key key) {
	return keys[key];
}

bool EventEngine::isPressed(Mouse btn) {
	return buttons[btn];
}

void EventEngine::setMouseRelative(bool b) {
	if (SDL_SetRelativeMouseMode(b ? SDL_TRUE : SDL_FALSE) < 0) {
#ifdef __DEBUG
		debug("Warning: SDL_SetRelativeMouseMode() isn't supported");
#endif
	}
}

Point2 EventEngine::getMouseDPos() {
	Point2 mouseDPos;
	SDL_GetRelativeMouseState(&mouseDPos.x, &mouseDPos.y);
	return mouseDPos;
}

Point2 EventEngine::getMousePos() {
	Point2 pos;
	SDL_GetMouseState(&pos.x, &pos.y);
	return pos;
}
