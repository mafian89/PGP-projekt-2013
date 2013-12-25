/*
* Author: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
* Desc: Nacitani obj souboru
*/
#ifndef OBJLOADER_H
#define OBJLOADER_H

bool loadOBJ(
	const char * path, 
	std::vector<glm::vec3> & out_vertices, 
	std::vector<glm::vec2> & out_uvs, 
	std::vector<glm::vec3> & out_normals
);

#endif