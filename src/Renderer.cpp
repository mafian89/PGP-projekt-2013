#include "Renderer.h"
#include "Timer.h"

////////////////////////////////////////////////////
// INITIALIZATION FUNCTION
////////////////////////////////////////////////////
void onInit() {
	////////////////////////////////////////////////////
	// SHADER CREATION AND COMPILATION
	////////////////////////////////////////////////////
	simpleShader.LoadFromFile(GL_VERTEX_SHADER, (shaderDir+"simpleShader.vp").c_str());
	simpleShader.LoadFromFile(GL_FRAGMENT_SHADER, (shaderDir+"simpleShader.fp").c_str());
	simpleShader.CreateAndLinkProgram();

	////////////////////////////////////////////////////
	// LOCATION OF ATRIBUTES AND UNIFORMS
	////////////////////////////////////////////////////
	simpleShader.Use();
		simpleShader.AddAttribute("vPosition");
		simpleShader.AddUniform("mvp");
	simpleShader.UnUse();

	////////////////////////////////////////////////////
	// CAMERA INIT
	////////////////////////////////////////////////////
	controlCamera->initControlCamera(glm::vec3(.0,.0,5.0),3.14,0.0,800,600,1.0,100.0);

	////////////////////////////////////////////////////
	// OTHER STUFF BELONGS HERE
	////////////////////////////////////////////////////
	//Sphere
	glGenBuffers(1, &sphereVBO);
	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphereVertices), sphereVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &sphereEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere), sphere, GL_STATIC_DRAW);
}


////////////////////////////////////////////////////
// FUNCTION FOR WINDOW RESIZE EVENT
////////////////////////////////////////////////////
void onWindowResized(unsigned w, unsigned h){
	width = w; height = h;
	glViewport(0, 0, w, h);
}

////////////////////////////////////////////////////
// RENDER SCENE
////////////////////////////////////////////////////
void Render(){
	//std::cout<<"RENDER"<<std::endl;
	//Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	//Cleat color
	glClearColor(0.6,0.6,0.6,1.0);
	//Enable depth testing
	glEnable( GL_DEPTH_TEST );

	//Camera update
	controlCamera->computeMatricesFromInputs();

	//Draw sphere
	simpleShader.Use();
		glUniformMatrix4fv(simpleShader("mvp"), 1, GL_FALSE,  glm::value_ptr(controlCamera->getProjectionMatrix() * controlCamera->getViewMatrix())); 
		glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
		glEnableVertexAttribArray(simpleShader["vPosition"]);
		glVertexAttribPointer(simpleShader["vPosition"],  3, GL_FLOAT, GL_FALSE, sizeof(SphereVertex), (void*)offsetof(SphereVertex, position));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
		glDrawElements(GL_TRIANGLES, sizeof(sphere)/sizeof(*sphere)*3, sphereIndexType, NULL);
	simpleShader.UnUse();


	//Swap buffers
	SDL_GL_SwapBuffers();
}

void onMouseMove(unsigned x, unsigned y, int xrel, int yrel, Uint8 buttons)
{
	if(buttons & SDL_BUTTON_LMASK)
	{
		SDL_WarpMouse((Uint16)(width/2),(Uint16)(height/2));
		SDL_GetMouseState(&_x,&_y); 
		
		#ifndef DEBUG
			//std::cout<<_x<<" "<<_y<<std::endl;
			//std::cout<<"width/2: "<<width/2<<"\n"<<"+: "<<mouseSpeed * float(width/2 - _x)<<std::endl;
			//std::cout<<"rel: "<<xrel<<" "<<yrel<<std::endl;
			//std::cout<<"normal: "<<x<<" "<<y<<std::endl;
			//std::cout<<"float(width/2 - _x): "<<float(400 - _x)<<std::endl;
			//std::cout<<"float(width/2 - x): "<<float(300 - x)<<std::endl;
			//std::cout<<"mouseSpeed *float(width/2 - _x): "<<mouseSpeed *float(400 - _x)<<std::endl;
			//std::cout<<"mouseSpeed *float(width/2 - x): "<<mouseSpeed *float(300 - x)<<std::endl;
			//std::cout<<controlCamera->getHorizontalAngle() + mouseSpeed * float(width/2 - _x)<<std::endl;
		std::cout<<"+---"<<std::endl;
		std::cout<<"[DEFAULT] horizontal angle: "<<controlCamera->getHorizontalAngle()<<std::endl;
		std::cout<<"[DEFAULT] vertical angle: "<<controlCamera->getVerticalAngle()<<std::endl;
		//std::cout<<"[VALUE] (float(width/2) - float(_x)): "<<(float(width/2) - float(_x))<<std::endl;
		//std::cout<<"[VALUE] (float(width/2) - float(_x)): "<<(float(width/2) - float(_x))<<std::endl;
		//std::cout<<"[VALUE] mouseSpeed * float(width/2 - _x): "<<mouseSpeed * (float(width/2) - float(_x))<<std::endl;
		//std::cout<<"[VALUE] mouseSpeed * float(height/2 - _y): "<<mouseSpeed * (float(height/2) - float(_y))<<std::endl;
		std::cout<<"[NEW] horizontal angle: "<<controlCamera->getHorizontalAngle() + mouseSpeed * (float(width/2) - float(_x))<<std::endl;
		std::cout<<"[NEW] vertical angle: "<<controlCamera->getVerticalAngle() + mouseSpeed * (float(height/2) - float(_y))<<std::endl;
		//std::cout<<"[WIDTH/2]: "<<width/2<<std::endl;
		//std::cout<<"[HEIGHT/2]: "<<height/2<<std::endl;
		//std::cout<<"[_x]: "<<_x<<std::endl;
		//std::cout<<"[_Y]: "<<_y<<std::endl;
		std::cout<<"+---"<<std::endl;
		#endif
		
		//controlCam->horizontalAngle += mouseSpeed * float(width/2 - _x);
		//controlCam->verticalAngle   += mouseSpeed * float(height/2 - _y);
		controlCamera->setHorizontalAngle(controlCamera->getHorizontalAngle() + mouseSpeed * (float(width/2) - float(_x)));
		controlCamera->setVerticalAngle(controlCamera->getVerticalAngle() + mouseSpeed * (float(height/2) - float(_y)));
		if(controlCamera->getVerticalAngle() >= 3.14f/2.0f){
			controlCamera->setVerticalAngle(3.14f/2.0f);
		} else if(controlCamera->getVerticalAngle() <= (-3.14f/2.0f)){
			controlCamera->setVerticalAngle(-3.14f/2.0f);
		}
		Render();
		//std::cout<<controlCamera->getHorizontalAngle()<<std::endl;
	}
}

////////////////////////////////////////////////////
// APPLICATION MAIN LOOP
////////////////////////////////////////////////////
int main(int argc, char** argv) {
	SDL_Surface *screen;
	int done;
	Uint8 *keys;
	//Initialize
	if(SDL_Init(SDL_INIT_VIDEO)< 0) throw SDL_Exception();
	//Create a OpenGL window
	screen = init(width,height,24,24,8);
	SDL_WM_SetCaption(WINDOW_TITLE, WINDOW_TITLE);
	//##### INSERT ANY ARGUMENT (PARAMETER) PARSING CODE HERE

	const int FPS_limit = 60;
	Uint32 start;

	Timer myTimer;
	Timer fps;
	Timer update;

	// The main loop
	done = 0;
	update.start();
	fps.start();

	while(!done) 
	{
		SDL_Event event;
		myTimer.start();
		//Respond to any events that occur
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
			case SDL_MOUSEMOTION :
				if(event.motion.state & SDL_BUTTON_LMASK)
				{
					controlCamera->moved = true;
					controlCamera->computeMatricesFromInputs();
					controlCamera->moved = false;
				}
				//onMouseMove(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel, event.motion.state);
				break;
			// ##### INSERT CODE TO HANDLE ANY OTHER EVENTS HERE #####
			}
		}
		
		//Check for escape
		keys = SDL_GetKeyState(NULL);
		if( keys[SDLK_ESCAPE] ) {
			done = 1;
		} else if( keys[SDLK_w] ) {
			controlCamera->setPosition(controlCamera->getPosition() + (controlCamera->getDirection() * 1.0f));
		} else if ( keys[SDLK_s] ) {
			controlCamera->setPosition(controlCamera->getPosition() - (controlCamera->getDirection() * 1.0f));
		} else if ( keys[SDLK_a] ) {
			controlCamera->setPosition(controlCamera->getPosition() - (controlCamera->getRight() * 1.0f));
		} else if ( keys[SDLK_d] ) {
			controlCamera->setPosition(controlCamera->getPosition() + (controlCamera->getRight() * 1.0f));
		} else if ( keys[SDLK_LEFT] ) {
			controlCamera->setHorizontalAngle(controlCamera->getHorizontalAngle() + 0.01);
		} else if ( keys[SDLK_RIGHT] ) {
			controlCamera->setHorizontalAngle(controlCamera->getHorizontalAngle() - 0.01);
		} else if ( keys[SDLK_UP] ) {
			controlCamera->setVerticalAngle(controlCamera->getVerticalAngle() + 0.01);
		} else if ( keys[SDLK_DOWN] ) {
			controlCamera->setVerticalAngle(controlCamera->getVerticalAngle() - 0.01);
		}
		

		// Draw the screen
		Render();
		FPS++;
		/*
		* Pøevzato z: http://www.lazyfoo.net/SDL_tutorials/lesson15/index.php
		*/
		if( update.get_ticks() > 100 )
		{
			//std::cout << "Average Frames Per Second: " << FPS / ( fps.get_ticks() / 1000.f )<<std::endl;
			std::stringstream caption;
			caption << WINDOW_TITLE << " FPS: " << FPS / ( fps.get_ticks() / 1000.f );
			SDL_WM_SetCaption( caption.str().c_str(), NULL );
			update.start();
		}
		if((useFrameRateCap) && myTimer.get_ticks() < 1000 / FPS_limit ) {
			SDL_Delay(( 1000 / FPS_limit ) - myTimer.get_ticks() );
		}
	}
	delete controlCamera;
	// Clean up and quit
	SDL_Quit();
	return 0;
}