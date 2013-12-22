#include "fboManager.h"

CFboManager::CFboManager() {
	glGenFramebuffers(1, &_fboId);
	useRenderBuffer = false;
}

CFboManager::~CFboManager() {
	glDeleteFramebuffers(1, &_fboId);
	if(useRenderBuffer) {
		glDeleteRenderbuffers(1, &_renderBufferId);
	}
}

void CFboManager::genRenderBuffer(unsigned w, unsigned h) {
	glGenRenderbuffers(1,&_renderBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, _renderBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,w,h);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
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