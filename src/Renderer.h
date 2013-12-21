#ifndef RENDERER_H
#define RENDERER_H
///////////////////////////////////////////
// INCLUDES
///////////////////////////////////////////
#include "GLSLshader/GLSLShader.h"
#include "tmp_model/sphere.h"
#include "camera/controlCamera.h"
#include "commonIncludes.h"

#ifdef _MSC_VER
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "assimp.lib")
#endif//_MSC_VER

///////////////////////////////////////////
// DEFINES
///////////////////////////////////////////
#define MIN_FRAMETIME_MSECS 15
#define WINDOW_TITLE "PGP - Projekt 2013 FPS: "

///////////////////////////////////////////
// FUNCTION PROTOTYPES
///////////////////////////////////////////
extern void onInit();
extern void onWindowResized(unsigned, unsigned);

///////////////////////////////////////////
// VARIABLES
///////////////////////////////////////////
unsigned width = 800;
unsigned height = 600;
bool useFrameRateCap = true;
int FPS = 0;
string shaderDir = "../src/shaders/";
GLSLShader simpleShader;
CControlCamera* controlCamera = new CControlCamera();
float mouseSpeed =  0.05f;
int _x,_y;
bool moved = true;

///////////////////////////////////////////
// TMP MODEL VARIABLES
///////////////////////////////////////////
GLuint sphereVBO;
GLuint sphereEBO;

///////////////////////////////////////////
// FUNCTIONS
///////////////////////////////////////////
struct SDL_Exception : public std::runtime_error
{
    SDL_Exception() throw()
        : std::runtime_error(std::string("SDL : ") + SDL_GetError()) {}
    SDL_Exception(const char * text) throw()
        : std::runtime_error(std::string("SDL : ") + text + " : " + SDL_GetError()) {}
    SDL_Exception(const std::string & text) throw()
        : std::runtime_error(std::string("SDL : ") + text + " : " + SDL_GetError()) {}
};

SDL_Surface * init(unsigned width, unsigned height, unsigned color, unsigned depth, unsigned stencil)
{
    // Set OpenGL attributes
    if(SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, color) < 0) throw SDL_Exception();
    if(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, depth) < 0) throw SDL_Exception();
    if(SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, stencil) < 0) throw SDL_Exception();
    if(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) < 0) throw SDL_Exception();

    // Create window
    SDL_Surface * screen = SDL_SetVideoMode(width, height, color, SDL_OPENGL | SDL_RESIZABLE);
    if(screen == NULL) throw SDL_Exception();
    // Init extensions
    GLenum error = glewInit();
    if(error != GLEW_OK)
        throw std::runtime_error(std::string("GLEW : Init failed : ") + (const char*)glewGetErrorString(error));
    // Call init code
    onInit();

    onWindowResized(width, height);

    return screen;
}
//error LNK2019: unresolved external symbol _main referenced in function ___tmainCRTStartup
#undef main
//or use sdlmain.lib

#endif