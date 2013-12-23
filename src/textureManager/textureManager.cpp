#include "textureManager.h"

CTextureManager::CTextureManager() {
}

CTextureManager::~CTextureManager() {
	textures.clear();
}

void CTextureManager::add(const string& texture) {
	GLuint tmpId;
	glGenTextures(1, &tmpId);
	textures[texture] = tmpId;
}

void CTextureManager::createTexture(const string& texture, const string filePath,unsigned w, unsigned h, GLuint filter, GLuint type, GLuint type_2){
	SDL_Surface *surface;
	GLuint textureid;
	int mode;
	add(texture);
	glBindTexture(GL_TEXTURE_2D, textures[texture]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//No image data - for frameBuffer
	if(filePath.empty()) {
		glTexImage2D(GL_TEXTURE_2D, 0 ,type, w, h, 0, type_2, GL_FLOAT, 0);
	} else {
		surface = IMG_Load(filePath.c_str());
		if (!surface) {
			std::cout<<"Cannot load image!"<<std::endl;
		}
		// work out what format to tell glTexImage2D to use...
		if (surface->format->BytesPerPixel == 3) { // RGB 24bit
				mode = GL_RGB;
		} else if (surface->format->BytesPerPixel == 4) { // RGBA 32bit
			mode = GL_RGBA;
		} else {
			SDL_FreeSurface(surface);
		}
		// this reads from the sdl surface and puts it into an opengl texture
		glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
		// clean up
		SDL_FreeSurface(surface);
	}
	//glBindTexture(GL_TEXTURE_2D,NULL);
}

GLuint CTextureManager::operator[] (const string& texture) {
	return textures[texture];
}