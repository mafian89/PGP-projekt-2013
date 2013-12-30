#include "object.h"

CObject::CObject(){
	m = glm::mat4(1.0);
	_textured = false;
}

CObject::CObject(const string path){
	bool loaded = loadFromFile(path);
	if(!loaded) {
		std::cerr<<"[ERROR] Cannot load object "<<path<<"!"<<std::endl;
	} else {
		std::cout<<"[INFO] Object "<<path<<" has been successufully loaded!"<<std::endl;
	}
	m = glm::mat4(1.0);
	_textured = false;
	genBuffers();
	fillBuffers();
}

CObject::~CObject(){
	vertices.clear();
	uvs.clear();
	normals.clear();
	indices.clear();
    
	indexed_vertices.clear();
	indexed_uvs.clear();
	indexed_normals.clear();
}

void CObject::translateModel(glm::vec3 _to) {
	m = glm::translate(m,_to);
}

void CObject::rotateModel(float _degrees, glm::vec3 _axis){
	m = glm::rotate(m,_degrees,_axis);
}

void CObject::genBuffers(){
	glGenBuffers(1,&_VBO);
	glGenBuffers(1,&_EBO);
	glGenBuffers(1,&_NBO);
	glGenBuffers(1,&_UVBO);
}

void CObject::fillBuffers(){
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _NBO);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _UVBO);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0] , GL_STATIC_DRAW);
	indexSize = indices.size();
}

void CObject::setTexture(GLuint _tex){
	_Texture = _tex;
	_textured = true;
}

bool CObject::hasTexture() {
	return _textured;
}

void CObject::setMaterial(glm::vec3 _mat){
	_material = _mat;
}

GLuint CObject::getObjectTexture(){
	return _Texture;
}

glm::vec3 CObject::getObjetMaterial(){
	return _material;
}

glm::mat4 CObject::getObjectModelMatrix(){
	return m;
}

unsigned int CObject::getIndexSize(){
	return indexSize;
}

bool CObject::loadFromFile(const string path){
	bool res = loadOBJ(path.c_str(), vertices, uvs, normals);
    indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);
    
    return res;
}