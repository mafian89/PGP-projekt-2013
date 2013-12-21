#ifndef RENDERER_H
#define RENDERER_H
///////////////////////////////////////////
// INCLUDES
///////////////////////////////////////////
#include <string>
#include <exception>
#include <stdexcept>
#include <cstdlib>
#include <sstream>
#include <stdio.h>
#include <iostream>

#include <SDL/SDL.h>
#include <GL/glew.h>
#include "GLSLshader/GLSLShader.h"

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