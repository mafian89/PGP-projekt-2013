#ifndef FBOMANAGER_H
#define FBOMANAGER_H

#include "../commonIncludes.h"

class CFboManager {
public:
	CFboManager();
	~CFboManager();
	bool checkFboStatus();
	void genRenderBuffer(unsigned w, unsigned h);
	GLuint getFboId();
private:
	GLuint _fboId;
	GLuint _renderBufferId;
	bool useRenderBuffer;
};

#endif