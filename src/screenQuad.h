#ifndef SCREENQUAD_H
#define SCREENQUAD_H

struct screenQuad {
	float pos[3];
} const tSceenQuad[4] = {
	{-1.0,1.0,-1.0},
	{1.0,1.0,-1.0},
	{-1.0,-1.0,-1.0},
	{1.0,-1.0,-1.0}
};

#endif