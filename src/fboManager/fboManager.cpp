#include "fboManager.h"

CFboManager::CFboManager() {
	useRenderBuffer = false;
	attachmentCount = 0;
	for(int i=0;i<5;i++) {
		mrt[i] = GL_COLOR_ATTACHMENT0+i;
	}
}

CFboManager::~CFboManager() {
	glDeleteFramebuffers(1, &_fboId);
	if(useRenderBuffer) {
		glDeleteRenderbuffers(1, &_renderBufferId);
	}
}

void CFboManager::initFbo() {
	glGenFramebuffers(1, &_fboId);
}

void CFboManager::genRenderBuffer(unsigned w, unsigned h) {
	glGenRenderbuffers(1,&_renderBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, _renderBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,w,h);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void CFboManager::bindRenderBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER,_fboId);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,_renderBufferId);
	//glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void CFboManager::setDrawBuffers() {
	glDrawBuffers(attachmentCount, mrt);
	std::cout<<attachmentCount<<std::endl;
}

void CFboManager::bindToFbo(GLenum type, GLenum texture, GLuint textureId) {
	glBindFramebuffer(GL_FRAMEBUFFER,_fboId);
	glFramebufferTexture2D(GL_FRAMEBUFFER,type,texture,textureId,0);
	//glDrawBuffers(1, mrt);
	attachmentCount += 1;
	//glBindFramebuffer(GL_FRAMEBUFFER,0);
}

GLuint CFboManager::getFboId() {
	return _fboId;
}

bool CFboManager::checkFboStatus() {
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