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

	quadShader.LoadFromFile(GL_VERTEX_SHADER, (shaderDir+"screenQuad.vp").c_str());
	//quadShader.LoadFromFile(GL_FRAGMENT_SHADER, (shaderDir+"textureRenderShader.fp").c_str());
	quadShader.LoadFromFile(GL_FRAGMENT_SHADER, (shaderDir+"multiTexShader.fp").c_str());
	quadShader.CreateAndLinkProgram();

	blurShader.LoadFromFile(GL_VERTEX_SHADER, (shaderDir+"screenQuad.vp").c_str());
	blurShader.LoadFromFile(GL_FRAGMENT_SHADER, (shaderDir+"blurShader.fp").c_str());
	blurShader.CreateAndLinkProgram();

	bloomSsaoShader.LoadFromFile(GL_VERTEX_SHADER, (shaderDir+"screenQuad.vp").c_str());
	bloomSsaoShader.LoadFromFile(GL_FRAGMENT_SHADER, (shaderDir+"extractAndSsaoShader.fp").c_str());
	bloomSsaoShader.CreateAndLinkProgram();


	////////////////////////////////////////////////////
	// LOCATION OF ATRIBUTES AND UNIFORMS
	////////////////////////////////////////////////////
	simpleShader.Use();
		simpleShader.AddAttribute("vPosition");
		simpleShader.AddAttribute("vNormal");
		simpleShader.AddAttribute("vUv");
		simpleShader.AddUniform("myTexture");
		simpleShader.AddUniform("hasTexture");
		simpleShader.AddUniform("mvp");
		simpleShader.AddUniform("mv");
		simpleShader.AddUniform("mn");
		simpleShader.AddUniform("vLightPos");
	simpleShader.UnUse();

	quadShader.Use();
		quadShader.AddAttribute("vPosition");
		quadShader.AddUniform("color");
		quadShader.AddUniform("bloom");
		quadShader.AddUniform("useHDR");
		quadShader.AddUniform("useSSAO");
		quadShader.AddUniform("onlySSAO");
		quadShader.AddUniform("scale");
		quadShader.AddUniform("exp");
		quadShader.AddUniform("bloomStrength");
		quadShader.AddUniform("bMax");
	quadShader.UnUse();

	blurShader.Use();
		blurShader.AddAttribute("vPosition");
		blurShader.AddUniform("render_tex");
		blurShader.AddUniform("res");
		blurShader.AddUniform("kernelSize");
	blurShader.UnUse();

	bloomSsaoShader.Use();
		bloomSsaoShader.AddUniform("vPosition");
		bloomSsaoShader.AddUniform("render_tex");
		bloomSsaoShader.AddUniform("normal_tex");
		bloomSsaoShader.AddUniform("random_tex");
		bloomSsaoShader.AddUniform("treshold");
		bloomSsaoShader.AddUniform("totStrength");
		bloomSsaoShader.AddUniform("strength");
		bloomSsaoShader.AddUniform("offset");
		bloomSsaoShader.AddUniform("falloff");
		bloomSsaoShader.AddUniform("rad");
	bloomSsaoShader.UnUse();

	////////////////////////////////////////////////////
	// CAMERA INIT
	////////////////////////////////////////////////////
	controlCamera->initControlCamera(glm::vec3(.0,.0,5.0),3.14,0.0,800,600,0.1,1000.0);

	////////////////////////////////////////////////////
	// TEXTURE INIT
	////////////////////////////////////////////////////
	texManager.createTexture("tex",(textureDir + "textura2.png"),width,height,GL_NEAREST,0,0);
	texManager.createTexture("susie",(textureDir + "susie.png"),width,height,GL_NEAREST,0,0);
	texManager.createTexture("floor",(textureDir + "floor.png"),width,height,GL_NEAREST,0,0);
	texManager.createTexture("noise_tex",(textureDir + "noise.png"),0,0,GL_NEAREST,0,0);
	texManager.createTexture("render_tex","",width,height,GL_NEAREST,GL_RGBA16F,GL_RGBA);
	texManager.createTexture("normal_tex","",width,height,GL_NEAREST,GL_RGBA16F,GL_RGBA);
	texManager.createTexture("blur_tex","",width,height,GL_NEAREST,GL_RGBA16F,GL_RGBA);
	texManager.createTexture("bloom_tex","",width,height,GL_NEAREST,GL_RGBA16F,GL_RGBA);
	texManager.createTexture("depth_tex","",width,height,GL_NEAREST,GL_DEPTH_COMPONENT32,GL_DEPTH_COMPONENT);
	currentTexture = texManager["render_tex"];

	////////////////////////////////////////////////////
	// FBO INIT
	////////////////////////////////////////////////////
	fboManager->initFbo();
	fboManager->genRenderBuffer(width,height);
	fboManager->bindRenderBuffer();
	fboManager->bindToFbo(GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,texManager["render_tex"]);
	fboManager->bindToFbo(GL_COLOR_ATTACHMENT1,GL_TEXTURE_2D,texManager["normal_tex"]);
	//fboManager->bindToFbo(GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,texManager["depth_tex"]);
	fboManager->setDrawBuffers();
	if(!fboManager->checkFboStatus()){
		return;
	}

	fboManagerBlur->initFbo();
	fboManagerBlur->genRenderBuffer(width,height);
	fboManagerBlur->bindRenderBuffer();
	fboManagerBlur->bindToFbo(GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,texManager["blur_tex"]);
	fboManagerBlur->setDrawBuffers();
	if(!fboManagerBlur->checkFboStatus()){
		return;
	}

	fboManagerBloomSsao->initFbo();
	fboManagerBloomSsao->genRenderBuffer(width,height);
	fboManagerBloomSsao->bindRenderBuffer();
	fboManagerBloomSsao->bindToFbo(GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,texManager["bloom_tex"]);
	fboManagerBloomSsao->setDrawBuffers();
	if(!fboManagerBloomSsao->checkFboStatus()){
		return;
	}

	////////////////////////////////////////////////////
	// LOAD OBJECTS
	////////////////////////////////////////////////////
	tmp = new CObject(objectDir + "crates.obj");
	tmp->setTexture(texManager["tex"]);
	tmp->translateModel(glm::vec3(-5.0,0.0,3.0));
	sceneManager->addObject(tmp);
	tmp = new CObject(objectDir + "crates.obj");
	tmp->setTexture(texManager["tex"]);
	tmp->translateModel(glm::vec3(5.0,0.0,-3.0));
	tmp->rotateModel(45.0,glm::vec3(0.0,1.0,0.0));
	sceneManager->addObject(tmp);
	tmp = new CObject(objectDir + "plane.obj");
	tmp->setTexture(texManager["floor"]);
	tmp->translateModel(glm::vec3(0.0,-1.0,0.0));
	sceneManager->addObject(tmp);
	tmp = new CObject(objectDir + "monkey.obj");
	tmp->setTexture(texManager["susie"]);
	tmp->translateModel(glm::vec3(0.0,-1.0,0.0));
	sceneManager->addObject(tmp);
	//obj1 = new CObject(objectDir + "crates.obj");
	//obj1->translateModel(glm::vec3(0.0,0.0,-2.0));
	//obj1->rotateModel(90.0,glm::vec3(0.0,1.0,0.0));
	
	////////////////////////////////////////////////////
	// ANT TWEAK BAR
	////////////////////////////////////////////////////
	// Initialize AntTweakBar
    TwInit(TW_OPENGL, NULL);
    // Tell the window size to AntTweakBar
    TwWindowSize(width, height);
    // Create a tweak bar
    bar = TwNewBar("Parametry");
	TwDefine(" GLOBAL help='HELP!!!' "); // Message added to the help bar.
	TwAddVarRW(bar, "Exposition for tonemapping", TW_TYPE_DOUBLE, &exposition, 
               " label='Exposition' keyIncr=1 keyDecr=CTRL+1 min=0 max=2 step=0.1 ");
	TwAddVarRW(bar, "Bright Max for tonemapping", TW_TYPE_DOUBLE, &bMax, 
               " label='Bright Max' keyIncr=2 keyDecr=CTRL+2 min=0.1 max=1.50 step=0.025 ");
	TwAddVarRW(bar, "Bloom strength for tonemapping", TW_TYPE_DOUBLE, &bloomStrength, 
               " label='Bloom strength' keyIncr=3 keyDecr=CTRL+3 min=0 max=3 step=0.1 ");
	TwAddVarRW(bar, "Treshold for blurring", TW_TYPE_DOUBLE, &treshold, 
               " label='Treshold' keyIncr=4 keyDecr=CTRL+4 min=0.1 max=1 step=0.1 ");
	/*TwAddVarRW(bar, "use HDR", TW_TYPE_BOOL32, &useHdr, 
            " label='HDR' true='YES' false='NO' key='h' help='Turns on/off hdr' ");*/


	TwAddVarRW(bar, "totStrength", TW_TYPE_DOUBLE, &totStrength, 
               " label='totStrength' min=1.0 max=20.0 step=0.1 ");
	TwAddVarRW(bar, "strength", TW_TYPE_DOUBLE, &strength, 
               " label='strength' min=0.0 max=20.0 step=0.1 ");
	TwAddVarRW(bar, "offset", TW_TYPE_DOUBLE, &offset, 
               " label='offset' min=1.0 max=40.0 step=1.0 ");
	//TwAddVarRW(bar, "falloff", TW_TYPE_DOUBLE, &falloff, 
 //              " label='falloff' min=0.00000001 max=0.000001 step=0.00000005");
	TwAddVarRW(bar, "rad", TW_TYPE_DOUBLE, &rad, 
               " label='rad' min=0.0 max=2.0 step=0.001");
	/*TwAddVarRW(bar, "use SSAO", TW_TYPE_BOOL32, &useSSAO, 
            " label='SSAO' true='YES' false='NO' help='Turns on/off SSAO' ");
	TwAddVarRW(bar, "only SSAO", TW_TYPE_BOOL32, &onlySSAO, 
            " label='only SSAO' true='YES' false='NO' help='Turns on/off SSAO component' ");*/


	////////////////////////////////////////////////////
	// OTHER STUFF BELONGS HERE
	////////////////////////////////////////////////////
	//Screen quad
	glGenBuffers(1,&screenQuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tSceenQuad), tSceenQuad, GL_STATIC_DRAW);
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
	//Clear color
	glClearColor(0.0,0.0,0.6,1.0);
	//glClearColor(0.0,0.0,0.0,1.0);
	//Enable depth testing
	glEnable( GL_DEPTH_TEST );
	//View port
	glViewport(0,0,width,height);
	//downsample
	//glViewport(0,0,width/2,height/2);

	//Camera update
	controlCamera->computeMatricesFromInputs();

	//Draw sphere
	glBindFramebuffer(GL_FRAMEBUFFER, fboManager->getFboId());
	simpleShader.Use();
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		for(int i = 0; i < sceneManager->scene.size(); i++) {
			if(sceneManager->scene[i]->hasTexture()) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, sceneManager->scene[i]->getObjectTexture());
			}

			glm::mat4 m = sceneManager->scene[i]->getObjectModelMatrix();
			glm::mat3 mn  = glm::transpose(glm::inverse(glm::mat3(controlCamera->getViewMatrix()*m)));
			glUniformMatrix4fv(simpleShader("mvp"), 1, GL_FALSE,  glm::value_ptr(controlCamera->getProjectionMatrix() * controlCamera->getViewMatrix()*m)); 
			glUniformMatrix4fv(simpleShader("mv"), 1, GL_FALSE,  glm::value_ptr(controlCamera->getViewMatrix()*m)); 
			glUniformMatrix3fv(simpleShader("mn"), 1, GL_FALSE,  glm::value_ptr(mn)); 
			glUniform3f(simpleShader("vLightPos"),lightPosition.x,lightPosition.y, lightPosition.z);
			glUniform1i(simpleShader("hasTexture"), sceneManager->scene[i]->hasTexture());

			glBindBuffer(GL_ARRAY_BUFFER, sceneManager->scene[i]->_VBO);
			glEnableVertexAttribArray(simpleShader["vPosition"]);
			glVertexAttribPointer(simpleShader["vPosition"],  3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		
			glBindBuffer(GL_ARRAY_BUFFER, sceneManager->scene[i]->_NBO);
			glEnableVertexAttribArray(simpleShader["vNormal"]);
			glVertexAttribPointer(simpleShader["vNormal"],  3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glBindBuffer(GL_ARRAY_BUFFER, sceneManager->scene[i]->_UVBO);
			glEnableVertexAttribArray(simpleShader["vUv"]);
			glVertexAttribPointer(simpleShader["vUv"],  2, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sceneManager->scene[i]->_EBO);
			glDrawElements(GL_TRIANGLES, sceneManager->scene[i]->getIndexSize(), GL_UNSIGNED_INT, NULL);
			glBindTexture(GL_TEXTURE_2D,NULL);
		}
	simpleShader.UnUse();
	glBindFramebuffer(GL_FRAMEBUFFER,0);

	//Bloom and SSAO
	glViewport(0,0,width,height);
	glBindFramebuffer(GL_FRAMEBUFFER, fboManagerBloomSsao->getFboId());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texManager["render_tex"]); 
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texManager["normal_tex"]); 
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texManager["noise_tex"]); 
	bloomSsaoShader.Use();
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glUniform1i(bloomSsaoShader("render_tex"),0);
		glUniform1i(bloomSsaoShader("normal_tex"),1);
		glUniform1i(bloomSsaoShader("random_tex"),2);
		glUniform1f(bloomSsaoShader("treshold"),treshold);
		glUniform1f(bloomSsaoShader("totStrength"),totStrength);
		glUniform1f(bloomSsaoShader("strength"),strength);
		glUniform1f(bloomSsaoShader("offset"),offset);
		glUniform1f(bloomSsaoShader("falloff"),falloff);
		glUniform1f(bloomSsaoShader("rad"),rad);
		glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBO);
		glEnableVertexAttribArray(bloomSsaoShader["vPosition"]);
		glVertexAttribPointer(bloomSsaoShader["vPosition"],  3, GL_FLOAT, GL_FALSE, sizeof(screenQuad), NULL);
		glDrawArrays(GL_TRIANGLE_STRIP,0,4);
	bloomSsaoShader.UnUse();
	glBindTexture(GL_TEXTURE_2D, NULL); 
	glBindFramebuffer(GL_FRAMEBUFFER,0);

	//Blur - undersample
	glViewport(0,0,width/2,height/2);
	glBindFramebuffer(GL_FRAMEBUFFER, fboManagerBlur->getFboId());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texManager["bloom_tex"]); 
	blurShader.Use();
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glUniform1i(blurShader("render_tex"),0);
		glUniform2f(blurShader("res"),(float)width/2.0,(float)height/2.0);
		glUniform1i(blurShader("kernelSize"),kernelSize);
		glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBO);
		glEnableVertexAttribArray(blurShader["vPosition"]);
		glVertexAttribPointer(blurShader["vPosition"],  3, GL_FLOAT, GL_FALSE, sizeof(screenQuad), NULL);
		glDrawArrays(GL_TRIANGLE_STRIP,0,4);
	blurShader.UnUse();
	glBindTexture(GL_TEXTURE_2D, NULL); 
	glBindFramebuffer(GL_FRAMEBUFFER,0);

	//Draw screen quad
	glViewport(0,0,width,height);
	//glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texManager["render_tex"]); 
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texManager["blur_tex"]); 
	quadShader.Use();
		glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBO);
		glUniform1i(quadShader("color"),0);
		glUniform1i(quadShader("bloom"),1);
		glUniform1i(quadShader("useHDR"),useHdr);
		glUniform1i(quadShader("useSSAO"),useSSAO);
		glUniform1i(quadShader("onlySSAO"),onlySSAO);
		glUniform1f(quadShader("exp"),exposition);
		glUniform1f(quadShader("bMax"),bMax);
		glUniform1f(quadShader("bloomStrength"),bloomStrength);
		glUniform1f(quadShader("scale"),0.5);
		glEnableVertexAttribArray(quadShader["vPosition"]);
		glVertexAttribPointer(quadShader["vPosition"],  3, GL_FLOAT, GL_FALSE, sizeof(screenQuad), NULL);
		glDrawArrays(GL_TRIANGLE_STRIP,0,4);
	quadShader.UnUse();
	glBindTexture(GL_TEXTURE_2D, NULL); 

	// Draw tweak bars
    TwDraw();

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
	SDL_EnableKeyRepeat(100,SDL_DEFAULT_REPEAT_INTERVAL);
	//Create a OpenGL window
	screen = init(width,height,24,24,8);
	SDL_WM_SetCaption(WINDOW_TITLE, WINDOW_TITLE);

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
		int handled;
		myTimer.start();
		//Respond to any events that occur
		while(SDL_PollEvent(&event))
		{
			// Send event to AntTweakBar
            handled = TwEventSDL(&event, SDL_MAJOR_VERSION, SDL_MINOR_VERSION);
			if(!handled) {
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
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							done=1;
							break;
						case SDLK_KP_PLUS:
							if(treshold <= 1.0) {treshold += 0.1;}
							std::cout<<treshold<<std::endl;
							break;
						case SDLK_KP_MINUS:
							if(treshold >= 0.2) {treshold -= 0.1;}
							std::cout<<treshold<<std::endl;
							break;
						case SDLK_h:
							useHdr = !useHdr;
							break;
						case SDLK_j:
							onlySSAO = !onlySSAO;
							break;
						case SDLK_k:
							useSSAO = !useSSAO;
							break;
					}
					break;
				}
			}
		}

		keys = SDL_GetKeyState(NULL);
		if( keys[SDLK_w] ) {
			controlCamera->setPosition(controlCamera->getPosition() + (controlCamera->getDirection() * movementSpeed));
		} else if ( keys[SDLK_s] ) {
			controlCamera->setPosition(controlCamera->getPosition() - (controlCamera->getDirection() * movementSpeed));
		} else if ( keys[SDLK_a] ) {
			controlCamera->setPosition(controlCamera->getPosition() - (controlCamera->getRight() * movementSpeed));
		} else if ( keys[SDLK_d] ) {
			controlCamera->setPosition(controlCamera->getPosition() + (controlCamera->getRight() * movementSpeed));
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
	delete fboManager;
	delete fboManagerBlur;
	delete fboManagerBloomSsao;
	//delete obj1;
	delete sceneManager;
	delete tmp;
	// Terminate AntTweakBar
    TwTerminate();
	// Clean up and quit
	SDL_Quit();
	return 0;
}