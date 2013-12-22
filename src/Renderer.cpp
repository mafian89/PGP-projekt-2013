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
		simpleShader.AddAttribute("vNormal");
		simpleShader.AddUniform("mvp");
		simpleShader.AddUniform("mv");
		simpleShader.AddUniform("mn");
		simpleShader.AddUniform("vLightPos");
	simpleShader.UnUse();

	////////////////////////////////////////////////////
	// CAMERA INIT
	////////////////////////////////////////////////////
	controlCamera->initControlCamera(glm::vec3(.0,.0,5.0),3.14,0.0,800,600,1.0,100.0);

	////////////////////////////////////////////////////
	// TEXTURE INIT
	////////////////////////////////////////////////////
	texManager.createTexture("tex",(textureDir + "textura.png"),width,height,GL_NEAREST,0,0);
	texManager.createTexture("tex1","",width,height,GL_NEAREST,GL_RGBA16F,GL_RGBA);

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

	//Screen quad
	glGenBuffers(1,&screenQuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tSceenQuad), tSceenQuad, GL_STATIC_DRAW);
}

////////////////////////////////////////////////////
// FBO STATUS CHECK
////////////////////////////////////////////////////
bool checkFramebufferStatus()
{
	// check FBO status
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch(status)
	{
	case GL_FRAMEBUFFER_COMPLETE_EXT:
		std::cout << "Framebuffer complete." << std::endl;
		return true;

	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
		std::cout << "[ERROR] Framebuffer incomplete: Attachment is NOT complete." << std::endl;
		return false;

	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
		std::cout << "[ERROR] Framebuffer incomplete: No image is attached to FBO." << std::endl;
		return false;

	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
		std::cout << "[ERROR] Framebuffer incomplete: Attached images have different dimensions." << std::endl;
		return false;

	case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
		std::cout << "[ERROR] Framebuffer incomplete: Color attached images have different internal formats." << std::endl;
		return false;

	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
		std::cout << "[ERROR] Framebuffer incomplete: Draw buffer." << std::endl;
		return false;

	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
		std::cout << "[ERROR] Framebuffer incomplete: Read buffer." << std::endl;
		return false;

	case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
		std::cout << "[ERROR] Unsupported by FBO implementation." << std::endl;
		return false;

	default:
		std::cout << "[ERROR] Unknow error." << std::endl;
		return false;
	}
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
		glm::mat3 mn  = glm::transpose(glm::inverse(glm::mat3(controlCamera->getViewMatrix())));
		glUniformMatrix4fv(simpleShader("mvp"), 1, GL_FALSE,  glm::value_ptr(controlCamera->getProjectionMatrix() * controlCamera->getViewMatrix())); 
		glUniformMatrix4fv(simpleShader("mv"), 1, GL_FALSE,  glm::value_ptr(controlCamera->getViewMatrix())); 
		glUniformMatrix3fv(simpleShader("mn"), 1, GL_FALSE,  glm::value_ptr(mn)); 
		glUniform3f(simpleShader("vLightPos"),lightPosition.x,lightPosition.y, lightPosition.z);
		glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
		glEnableVertexAttribArray(simpleShader["vPosition"]);
		glVertexAttribPointer(simpleShader["vPosition"],  3, GL_FLOAT, GL_FALSE, sizeof(SphereVertex), (void*)offsetof(SphereVertex, position));
		glEnableVertexAttribArray(simpleShader["vNormal"]);
		glVertexAttribPointer(simpleShader["vNormal"],  3, GL_FLOAT, GL_FALSE, sizeof(SphereVertex), (void*)offsetof(SphereVertex, normal));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
		glDrawElements(GL_TRIANGLES, sizeof(sphere)/sizeof(*sphere)*3, sphereIndexType, NULL);
	simpleShader.UnUse();
	//Draw screen quad
	//quadShader.Use();
	//	glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBO);
	//	glEnableVertexAttribArray(simpleShader["vPosition"]);
	//	glVertexAttribPointer(simpleShader["vPosition"],  3, GL_FLOAT, GL_FALSE, sizeof(screenQuad), NULL);
	//	glDrawArrays(GL_TRIANGLE_STRIP,0,4);
	//quadShader.UnUse();


	//Swap buffers
	SDL_GL_SwapBuffers();
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
				break;
			// ##### INSERT CODE TO HANDLE ANY OTHER EVENTS HERE #####
			}
		}
		
		//Check for escape
		keys = SDL_GetKeyState(NULL);
		if( keys[SDLK_ESCAPE] ) {
			done = 1;
		} else if( keys[SDLK_w] ) {
			controlCamera->setPosition(controlCamera->getPosition() + (controlCamera->getDirection() * movementSpeed));
		} else if ( keys[SDLK_s] ) {
			controlCamera->setPosition(controlCamera->getPosition() - (controlCamera->getDirection() * movementSpeed));
		} else if ( keys[SDLK_a] ) {
			controlCamera->setPosition(controlCamera->getPosition() - (controlCamera->getRight() * movementSpeed));
		} else if ( keys[SDLK_d] ) {
			controlCamera->setPosition(controlCamera->getPosition() + (controlCamera->getRight() * movementSpeed));
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