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
	//No image data - for frameBuffer
	add(texture);
	glBindTexture(GL_TEXTURE_2D, textures[texture]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	if(filePath.empty()) {
		glTexImage2D(GL_TEXTURE_2D, 0 ,type, w, h, 0, type_2, GL_FLOAT, 0);
	} else {
		//Dodelat
	}
	glBindTexture(GL_TEXTURE_2D,NULL);
}

GLuint CTextureManager::operator[] (const string& texture) {
	return textures[texture];
}