#include "../globals/globals.h"

CControlCamera::CControlCamera() {
}

CControlCamera::~CControlCamera() {
}

glm::mat4 CControlCamera::getViewMatrix(){
        return this->ViewMatrix;
}
glm::mat4 CControlCamera::getProjectionMatrix(){
        return this->ProjectionMatrix;
}

glm::vec3 CControlCamera::getPosition(){
	return this->position;
}

glm::vec3 CControlCamera::getInitialPosition(){
	return this->initial_position;
}

glm::vec3 CControlCamera::getDirection(){
	return this->direction;
}

glm::vec3 CControlCamera::getRight(){
	return this->right;
}

glm::vec3 CControlCamera::getUp(){
	return this->up;
}

float CControlCamera::getHorizontalAngle(){
	return this->horizontalAngle;
}

float CControlCamera::getInitialHorizontalAngle(){
	return this->initial_horizontalAngle;
}

float CControlCamera::getVerticalAngle(){
	return this->verticalAngle;
}

float CControlCamera::getInitialVerticalAngle(){
	return this->initial_verticalAngle;
}

//--Settery
void CControlCamera::setHorizontalAngle(float _horAngle){
	this->horizontalAngle = _horAngle;
}

void CControlCamera::setVerticalAngle(float _vertAngle){
	this->verticalAngle = _vertAngle;
}

void CControlCamera::setPosition(glm::vec3 _pos){
	this->position = _pos;
}

void CControlCamera::setDirection(glm::vec3 _dir){
	this->direction = _dir;
}

void CControlCamera::setUp(glm::vec3 _up){
	this->up;
}

void CControlCamera::initControlCamera(glm::vec3 _pos, float _horAngle = 3.14f, float _verAngle = 0.0f) {
    this->position = _pos;
    this->initial_position = this->position;
    this->horizontalAngle = _horAngle;
    this->initial_horizontalAngle = horizontalAngle;
    this->verticalAngle = _verAngle;
    this->initial_verticalAngle = this->verticalAngle;
    this->fov = 45.0f;
    this->aspec = (float)width/(float)height;
}

void CControlCamera::computeMatricesFromInputs(void){
        // Direction : Spherical coordinates to Cartesian coordinates conversion
        //Viz http://www.lighthouse3d.com/wp-content/uploads/2011/04/vfpoints2.gif
        this->direction = glm::vec3(
                cos(verticalAngle) * sin(horizontalAngle), 
                sin(verticalAngle),
                cos(verticalAngle) * cos(horizontalAngle)
        );
        
        // Right vector
        this->right = glm::vec3(
                sin(horizontalAngle - 3.14f/2.0f), 
                0,
                cos(horizontalAngle - 3.14f/2.0f)
        );
        
        // Up vector - cross produktem dostanu kolmý vektor na tyto dva
        this->up = glm::cross( right, direction );
#ifdef DEBUG
        std::cout<<"Camera POSITION vector: ("<<position.x<<", "<<position.y<<", "<<position.z<<")"<<std::endl;
        std::cout<<"Camera UP vector: ("<<up.x<<", "<<up.y<<", "<<up.z<<")"<<std::endl;
        std::cout<<"Camera RIGHT vector: ("<<right.x<<", "<<right.y<<", "<<right.z<<")"<<std::endl;
        std::cout<<"Camera DIRECTION vector: ("<<direction.x<<", "<<direction.y<<", "<<direction.z<<")"<<std::endl;
		std::cout<<"Camera horizotnal angle: "<<horizontalAngle<<std::endl;
		std::cout<<"Camera vertical angle: "<<verticalAngle<<std::endl;
#endif
        this->aspec = (float)width/(float)height;
        this->ProjectionMatrix = glm::perspective(fov, aspec, (float)NEAR_CLIP_PLANE, (float)FAR_CLIP_PLANE);
        // Camera matrix
        this->ViewMatrix       = glm::lookAt(
												position,           
												position+direction,
												up                 
											);
//        ViewMatrix = glm::rotate(ViewMatrix, -90.0f, glm::vec3(1,0,0)); 
        
}


