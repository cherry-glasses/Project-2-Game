#include "Log.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "SDL/include/SDL.h"
#include "SDL/include/SDL_gamecontroller.h"

ModuleInput::ModuleInput() : Module()
{
	name = "input";

	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * NUM_MOUSE_BUTTONS);
	
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Awake(pugi::xml_node& _config)
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
	{
		LOG("SDL_GAMECONTROLLER could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	controller = nullptr;
	for (int i = 0; i < SDL_NumJoysticks(); ++i)
	{
		LOG("Init the controller (search and asign)");
		controller = nullptr;
		for (int i = 0; i < SDL_NumJoysticks(); i++)
		{
			if (SDL_IsGameController(i)) {
				joystick = SDL_JoystickOpen(0);
				controller = SDL_GameControllerOpen(i);
				if (controller) {
					break;
				}
			}
		}
	}

	return ret;
}

// Called before the first frame
bool ModuleInput::Start()
{
	SDL_StopTextInput();
	return true;
}

// Called each loop iteration
bool ModuleInput::PreUpdate()
{
	static SDL_Event event;

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (mouse_buttons[i] == KEY_DOWN)
			mouse_buttons[i] = KEY_REPEAT;

		if (mouse_buttons[i] == KEY_UP)
			mouse_buttons[i] = KEY_IDLE;
	}

	buttonForGamepad();

	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			windowEvents[EW_QUIT] = true;
			break;

		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_FOCUS_LOST:
				windowEvents[EW_HIDE] = true;
				break;

			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_RESTORED:
				windowEvents[EW_SHOW] = true;
				break;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			mouse_buttons[event.button.button - 1] = KEY_DOWN;
			break;

		case SDL_MOUSEBUTTONUP:
			mouse_buttons[event.button.button - 1] = KEY_UP;
			break;

		case SDL_MOUSEMOTION:
			int scale = App->window->GetScale();
			mouse_motion_x = event.motion.xrel / scale;
			mouse_motion_y = event.motion.yrel / scale;
			mouse_x = event.motion.x / scale;
			mouse_y = event.motion.y / scale;
			break;
		}
	}

	

	return true;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	
	return true;
}

// ---------
bool ModuleInput::GetWindowEvent(EVENT_WINDOW _ev)
{
	return windowEvents[_ev];
}

void ModuleInput::GetMousePosition(int& _x, int& _y)
{
	_x = mouse_x;
	_y = mouse_y;
}

void ModuleInput::GetMouseMotion(int& _x, int& _y)
{
	_x = mouse_motion_x;
	_y = mouse_motion_y;
}
void ModuleInput::buttonForGamepad() {
	//BUTTON A
	if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A) == 1) {
		if (gamepad.A == PAD_BUTTON_IDLE)
			gamepad.A = PAD_BUTTON_DOWN;
		else
			gamepad.A = PAD_BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.A == PAD_BUTTON_REPEAT || (gamepad.A == PAD_BUTTON_DOWN))
			gamepad.A = PAD_BUTTON_KEY_UP;
		else
			gamepad.A = PAD_BUTTON_IDLE;
	}

	//BUTTON B
	if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B) == 1) {
		if (gamepad.B == PAD_BUTTON_IDLE)
			gamepad.B = PAD_BUTTON_DOWN;
		else
			gamepad.B = PAD_BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.B == PAD_BUTTON_REPEAT || (gamepad.B == PAD_BUTTON_DOWN))
			gamepad.B = PAD_BUTTON_KEY_UP;
		else
			gamepad.B = PAD_BUTTON_IDLE;
	}

	//BUTTON X
	if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_X) == 1) {
		if (gamepad.X == PAD_BUTTON_IDLE)
			gamepad.X = PAD_BUTTON_DOWN;
		else
			gamepad.X = PAD_BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.X == PAD_BUTTON_REPEAT || (gamepad.X == PAD_BUTTON_DOWN))
			gamepad.X = PAD_BUTTON_KEY_UP;
		else
			gamepad.X = PAD_BUTTON_IDLE;
	}

	//BUTTON Y
	if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_Y) == 1) {
		if (gamepad.Y == PAD_BUTTON_IDLE)
			gamepad.Y = PAD_BUTTON_DOWN;
		else
			gamepad.Y = PAD_BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.Y == PAD_BUTTON_REPEAT || (gamepad.Y == PAD_BUTTON_DOWN))
			gamepad.Y = PAD_BUTTON_KEY_UP;
		else
			gamepad.Y = PAD_BUTTON_IDLE;
	}

	//BUTTON START
	if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_START) == 1) {
		if (gamepad.START == PAD_BUTTON_IDLE)
			gamepad.START = PAD_BUTTON_DOWN;
		else
			gamepad.START = PAD_BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.START == PAD_BUTTON_REPEAT || (gamepad.START == PAD_BUTTON_DOWN))
			gamepad.START = PAD_BUTTON_KEY_UP;
		else
			gamepad.START = PAD_BUTTON_IDLE;
	}

	//BUTTON DPAD UP
	if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP) == 1) {
		if (gamepad.CROSS_UP == PAD_BUTTON_IDLE)
			gamepad.CROSS_UP = PAD_BUTTON_DOWN;
		else
			gamepad.CROSS_UP = PAD_BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.CROSS_UP == PAD_BUTTON_REPEAT || (gamepad.CROSS_UP == PAD_BUTTON_DOWN))
			gamepad.CROSS_UP = PAD_BUTTON_KEY_UP;
		else
			gamepad.CROSS_UP = PAD_BUTTON_IDLE;
	}

	//BUTTON DPAD DOWN
	if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN) == 1) {
		if (gamepad.CROSS_DOWN == PAD_BUTTON_IDLE)
			gamepad.CROSS_DOWN = PAD_BUTTON_DOWN;
		else
			gamepad.CROSS_DOWN = PAD_BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.CROSS_DOWN == PAD_BUTTON_REPEAT || (gamepad.CROSS_DOWN == PAD_BUTTON_DOWN))
			gamepad.CROSS_DOWN = PAD_BUTTON_KEY_UP;
		else
			gamepad.CROSS_DOWN = PAD_BUTTON_IDLE;
	}

	//BUTTON DPAD LEFT
	if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT) == 1) {
		if (gamepad.CROSS_LEFT == PAD_BUTTON_IDLE)
			gamepad.CROSS_LEFT = PAD_BUTTON_DOWN;
		else
			gamepad.CROSS_LEFT = PAD_BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.CROSS_LEFT == PAD_BUTTON_REPEAT || (gamepad.CROSS_LEFT == PAD_BUTTON_DOWN))
			gamepad.CROSS_LEFT = PAD_BUTTON_KEY_UP;
		else
			gamepad.CROSS_LEFT = PAD_BUTTON_IDLE;
	}

	//BUTTON DPAD UP
	if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == 1) {
		if (gamepad.CROSS_RIGHT == PAD_BUTTON_IDLE)
			gamepad.CROSS_RIGHT = PAD_BUTTON_DOWN;
		else
			gamepad.CROSS_RIGHT = PAD_BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.CROSS_RIGHT == PAD_BUTTON_REPEAT || (gamepad.CROSS_RIGHT == PAD_BUTTON_DOWN))
			gamepad.CROSS_RIGHT = PAD_BUTTON_KEY_UP;
		else
			gamepad.CROSS_RIGHT = PAD_BUTTON_IDLE;
	}

	//JOYSTICK UP
	if (SDL_JoystickGetAxis(joystick, 1) < -12000) {
		if (gamepad.JOYSTICK_UP == PAD_BUTTON_IDLE)
			gamepad.JOYSTICK_UP = PAD_BUTTON_DOWN;
		else
			gamepad.JOYSTICK_UP = PAD_BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.JOYSTICK_UP == PAD_BUTTON_REPEAT || (gamepad.JOYSTICK_UP == PAD_BUTTON_DOWN))
			gamepad.JOYSTICK_UP = PAD_BUTTON_KEY_UP;
		else
			gamepad.JOYSTICK_UP = PAD_BUTTON_IDLE;
	}

	//JOYSTICK DOWN
	if (SDL_JoystickGetAxis(joystick, 1) > 12000) {
		if (gamepad.JOYSTICK_DOWN == PAD_BUTTON_IDLE)
			gamepad.JOYSTICK_DOWN = PAD_BUTTON_DOWN;
		else
			gamepad.JOYSTICK_DOWN = PAD_BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.JOYSTICK_DOWN == PAD_BUTTON_REPEAT || (gamepad.JOYSTICK_DOWN == PAD_BUTTON_DOWN))
			gamepad.JOYSTICK_DOWN = PAD_BUTTON_KEY_UP;
		else
			gamepad.JOYSTICK_DOWN = PAD_BUTTON_IDLE;
	}

	//JOYSTICK LEFT
	if (SDL_JoystickGetAxis(joystick, 0) < -12000) {
		if (gamepad.JOYSTICK_LEFT == PAD_BUTTON_IDLE)
			gamepad.JOYSTICK_LEFT = PAD_BUTTON_DOWN;
		else
			gamepad.JOYSTICK_LEFT = PAD_BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.JOYSTICK_LEFT == PAD_BUTTON_REPEAT || (gamepad.JOYSTICK_LEFT == PAD_BUTTON_DOWN))
			gamepad.JOYSTICK_LEFT = PAD_BUTTON_KEY_UP;
		else
			gamepad.JOYSTICK_LEFT = PAD_BUTTON_IDLE;
	}

	//JOYSTICK RIGHT
	if (SDL_JoystickGetAxis(joystick, 0) > 12000) {
		if (gamepad.JOYSTICK_RIGHT == PAD_BUTTON_IDLE)
			gamepad.JOYSTICK_RIGHT = PAD_BUTTON_DOWN;
		else
			gamepad.JOYSTICK_RIGHT = PAD_BUTTON_REPEAT;
	}
	else
	{
		if (gamepad.JOYSTICK_RIGHT == PAD_BUTTON_REPEAT || (gamepad.JOYSTICK_RIGHT == PAD_BUTTON_DOWN))
			gamepad.JOYSTICK_RIGHT = PAD_BUTTON_KEY_UP;
		else
			gamepad.JOYSTICK_RIGHT = PAD_BUTTON_IDLE;
	}

}