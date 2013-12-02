#include <stdafx.h>
#include <cmath>
#include <CameraManager.h>

///////////////////////
using namespace RiemannSphere;
using namespace Easy3D;
///////////////////////

CameraManager::CameraManager(Easy3D::Camera *camera,
							 const Easy3D::Vec3& toPoint,
							 Sphere *sphere)
:camera(camera)
,sphere(sphere)
{
    addState(ON_ENABLE);
    addState(ON_DISABLE);
    setNextState(ON_ENABLE);
    //set point to point
    cameraPointer.setPosition(toPoint,true);
    cameraPointer.addChild(camera,Object::ParentMode::ENABLE_PARENT,false);
    //default
    velocity=Vec3::ONE;
    angle=Math::torad(45.0f);
    n=0.5;
    f=100.0f;
}

//////SETTINGS
void CameraManager::setVelocity(const Easy3D::Vec3& vel){
    velocity=vel;
}
void CameraManager::setProjectionInfo(float angle,float n,float f){
    this->angle=angle;
    this->n=n;
    this->f=f;
}

CameraPositionInfo CameraManager::getCameraPositionInfo(){
    CameraPositionInfo cpiout;
    cpiout.angle=angle;
    //cpiout.n=n;
    //cpiout.f=f;
    cpiout.pos=cameraPointer.getPosition(true);
    cpiout.rot=cameraPointer.getRotation(true);
    return cpiout;
}
void CameraManager::setCameraPositionInfo(const CameraPositionInfo& cpi){
    angle=cpi.angle;
    //n=cpi.n;
    //f=cpi.f;
    cameraPointer.setPosition(cpi.pos,true);
    cameraPointer.setRotation(cpi.rot,true);
}
//////////////

void CameraManager::changeFar(float f){
    this->f=f;
    //update projection
    camera->setPerspective(angle,n, f);
    camera->update();
}
void CameraManager::changeNear(float n){
    this->n=n;
    //update projection
    camera->setPerspective(angle,n, f);
    camera->update();
}

//////INPUT
void CameraManager::onKeyDown(Key::Keyboard key){}
void CameraManager::onMouseScroll(short scroll){
    angle+=scroll*velocity.z*Application::instance()->getLastDeltaTime();
	angle= angle<velocity.z ? velocity.z : angle;
    if(angle>Math::PI) angle=Math::PI;
}
void CameraManager::onMouseDown(Vec2 mousePosition, Key::Mouse button){
    if(Key::list(button, Key::BUTTON_LEFT, Key::BUTTON_RIGHT)){
		//get ray collision
		onMove.ray=getMouseRay();
		onMove.collided=sphere->rayCast(onMove.ray,onMove.segment);

    }
}
void CameraManager::onMouseMove(Easy3D::Vec2 mousePosition){
	if(Application::instance()->getInput()->getMouseDown(Key::BUTTON_LEFT)||
	   Application::instance()->getInput()->getMouseDown(Key::BUTTON_RIGHT))
		//send message
		sendMessage(DO_POINT);
}
void CameraManager::onMouseRelease(Easy3D::Vec2 mousePosition,  Easy3D::Key::Mouse button){
	if(Key::list(button, Key::BUTTON_LEFT, Key::BUTTON_RIGHT))
        sendMessage(NO_POINT);
}
void CameraManager::onMousePress(Vec2 mousePosition, Key::Mouse button){
    if(Key::list(button, Key::BUTTON_LEFT, Key::BUTTON_RIGHT)){
		//get ray collision
		onClick.ray=getMouseRay();
		onClick.collided=sphere->rayCast(onClick.ray,onClick.segment);
		//save rotation
		startPickRotation=Quaternion::fromMatrix(cameraPointer.getGlobalMatrix()).getNormalize();

	}
}
///////////
//debug
void CameraManager::drawFrustum(Easy3D::Render* r){
	//save 
	Render::ClientState saveClientState=r->getClientState();
	Render::ColorState saveColorState=r->getColorState();
	Render::MatrixsState saveMatrixsState=r->getMatrixsState();

	//change state:
	r->setClientState(Render::ClientState::VERTEX);
	r->setColorState({0,255,0,255});
	//change matrix
	auto newMS=saveMatrixsState;
	newMS.modelview=newMS.modelview.mul(camera->getGlobalMatrix());
	r->setMatrixsState(newMS);
	//draw
	// Get near and far from the Projection matrix.
	Mat4 proj=camera->getProjectionMatrix();
    float cnear =this->n;// proj[11] / (proj[10] - 1.0);
    float cfar =this->f;// proj[11] / (1.0 + proj[10]);
	// Get the sides of the near plane.
    float nLeft = cnear * (proj[2] - 1.0) / proj[0];
    float nRight = cnear * (1.0 + proj[2]) / proj[0];
    float nTop = cnear * (1.0 + proj[6]) / proj[5];
    float nBottom = cnear * (proj[6] - 1.0) / proj[5];
	// Get the sides of the far plane.
    float fLeft = cfar * (proj[2] - 1.0) / proj[0];
    float fRight = cfar * (1.0 + proj[2]) / proj[0];
    float fTop = cfar * (1.0 + proj[6]) / proj[5];
    float fBottom = cfar * (proj[6] - 1.0) / proj[5];

	 // 8 of vertex coords
    GLfloat vertices[] = {	

		0.0f, 0.0f, 0.0f,
		fLeft, fBottom, -cfar,
		0.0f, 0.0f, 0.0f,
		fRight, fBottom, -cfar,
		0.0f, 0.0f, 0.0f,
		fRight, fTop, -cfar,
		0.0f, 0.0f, 0.0f,
		fLeft, fTop, -cfar,
		//far
		fLeft, fBottom, -cfar,
		fRight, fBottom, -cfar,
		fRight, fTop, -cfar,
		fLeft, fTop, -cfar,
		fRight, fTop, -cfar,
		fRight, fBottom, -cfar,
		fLeft, fTop, -cfar,
		fLeft, fBottom, -cfar,
		//near
		nLeft, nBottom, -cnear,
		nRight, nBottom, -cnear,
		nRight, nTop, -cnear,
		nLeft, nTop, -cnear,
		nLeft, nTop, -cnear,
		nLeft, nBottom, -cnear,
		nRight, nTop, -cnear,
		nRight, nBottom, -cnear

    };
	//unbind VBO
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
    //pointer to vertexs
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    //draw call
    glDrawArrays(GL_LINE_STRIP, 0, 24);
	//reset
	r->setClientState(saveClientState);
	r->setColorState(saveColorState);
	r->setMatrixsState(saveMatrixsState);
}

///STATE MACHINE
void CameraManager::onStateMessage(int message){}

void CameraManager::onStateStart(){
    if(getCurrentStateID()==ON_ENABLE){
        //add input
        Application::instance()->getInput()->addHandler((Easy3D::Input::KeyboardHandler*)this);
        Application::instance()->getInput()->addHandler((Easy3D::Input::MouseHandler*)this);
    }
}

Ray CameraManager::getMouseRay(){    
	Vec3 dir=camera->getNormalPointFrom2DScreen(Application::instance()->getInput()->getMouse());
    Vec3 pos=camera->getPointFrom2DScreen(Application::instance()->getInput()->getMouse());
	Mat4 rotation=cameraPointer.getRotation(true).getNormalize().getInverse().getMatrix();
	pos=rotation.mul(Vec4(pos,0)).xyz();
	dir=rotation.mul(Vec4(dir,0)).xyz();
    return Ray(pos,dir);
}

void CameraManager::onStateRun(float dt){
    
    //update projection
    camera->setPerspective(angle,n, f);
    camera->update();
    
    //update rotation
    if(getLastMessage()==DO_POINT){
		if(onClick.collided&&onMove.collided){
				//start point
				Vec3 vStart=(onClick.segment.t[0]-cameraPointer.getPosition(true)).getNormalize();
				Quaternion rotStart=Quaternion::fromLookRotation(vStart,Vec3(0,1,0)).getNormalize();
				//end point
				Vec3 vEnd=(onMove.segment.t[0]-cameraPointer.getPosition(true)).getNormalize();
				Quaternion rotEnd=Quaternion::fromLookRotation(vEnd,Vec3(0,1,0)).getNormalize();
				//turn
				cameraPointer.setRotation(
					startPickRotation.getInverse().mul(rotEnd.mul(rotStart.getInverse()))
					);
		}
		sendMessage(NO_POINT);
    }
    
}

void CameraManager::onStateEnd(){
    if(getCurrentStateID()==ON_ENABLE){
        //remove input
        Application::instance()->getInput()->removeHandler((Easy3D::Input::KeyboardHandler*)this);
        Application::instance()->getInput()->removeHandler((Easy3D::Input::MouseHandler*)this);
    }
}
////////////////
