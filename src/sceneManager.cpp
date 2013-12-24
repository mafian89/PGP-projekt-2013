#include "sceneManager.h"

CSceneManager::CSceneManager() {
}

void CSceneManager::addObject(CObject * o) {
	scene.push_back(o);
}

CSceneManager::~CSceneManager() {
	scene.clear();
}