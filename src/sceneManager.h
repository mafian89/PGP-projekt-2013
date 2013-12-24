#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "commonIncludes.h"
#include "object.h"

class CSceneManager {
public:
	CSceneManager();
	~CSceneManager();

	vector<CObject> scene;
};

#endif