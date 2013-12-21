
int main(int argc, char** argv) {
	/*SDL_Surface *screen;
	int done;
	Uint8 *keys;

	// Initialize
	SDL_Init(SDL_INIT_VIDEO);

	// Create a OpenGL window
	screen = init(width,height,24,24,8);
	SDL_WM_SetCaption(WINDOW_TITLE, WINDOW_TITLE);

	// ##### INSERT ANY ARGUMENT (PARAMETER) PARSING CODE HERE


	// Initialize the time variables
	int startTime = SDL_GetTicks();
	int prevTime = startTime;

	// The main loop
	done = 0;
	while(!done) 
	{
		SDL_Event event;

		int currTime = SDL_GetTicks();
		timeElapsed = currTime - prevTime;
		if(timeElapsed < MIN_FRAMETIME_MSECS)
		{
			// Not enough time has elapsed. Let's limit the frame rate
			SDL_Delay(MIN_FRAMETIME_MSECS - timeElapsed);
			currTime = SDL_GetTicks();
			timeElapsed = currTime - prevTime;
		}
		prevTime = currTime;
		//animate(timeElapsed);

		// Respond to any events that occur
		while(SDL_PollEvent(&event))
		{
			switch(event.type) 
			{
			case SDL_VIDEORESIZE:
				onWindowResized(event.resize.w, event.resize.h);
				break;

			case SDL_QUIT:
				done = 1;
				break;

			// ##### INSERT CODE TO HANDLE ANY OTHER EVENTS HERE #####
			}
		}

		// Check for escape
		keys = SDL_GetKeyState(NULL);
		if( keys[SDLK_ESCAPE] ) {
			done = 1;
		}
	
		// Draw the screen
		Render();
	}

  // Clean up and quit
  SDL_Quit();*/
  return 0;
}