#ifndef FBOMANAGER_H
#define FBOMANAGER_H

#include "../commonIncludes.h"

class CFboManager {
public:
	CFboManager();
	~CFboManager();
	bool checkFboStatus();
	void genRenderBuffer(unsigned w, unsigned h);
	void bindRenderBuffer();
	void bindToFbo(GLenum type, GLenum texture, GLuint textureId);
	void initFbo();
	GLuint getFboId();
private:
	GLuint _fboId;
	GLuint _renderBufferId;
	bool useRenderBuffer;
	unsigned attachmentCount;
};

#endif