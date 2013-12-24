#ifndef OBJECT_H
#define OBJECT_H

#include "commonIncludes.h"

using namespace std;

class CObject {
public:
	CObject();
	CObject(const string filename);
	~CObject();
	bool loadFromFile(const string path);
	void genBuffers();
	void fillBuffers();
	//void bindBuffer(GLuint buffer);
	//void unbindBuffers();
	void setTexture(GLuint _tex);
	void setMaterial(glm::vec3 _mat);
	void translate(glm::vec3 _to);

	GLuint getObjectTexture();
	glm::vec3 getObjetMaterial();
	glm::mat4 getObjectModelMatrix();
private:
	GLuint _VBO;
	GLuint _UVBO;
	GLuint _NBO;
	GLuint _EBO;
	GLuint _Texture;
	glm::vec3 _material;

	vector<glm::vec3> vertices;
	vector<glm::vec2> uvs;
	vector<glm::vec3> normals;
	vector<unsigned int> indices;
    
	vector<glm::vec3> indexed_vertices;
	vector<glm::vec2> indexed_uvs;
	vector<glm::vec3> indexed_normals;

	glm::mat4 m;
};

#endif