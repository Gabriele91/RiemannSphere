#include <stdafx.h>
#include <cmath>
#include <CameraManager.h>

///////////////////////
using namespace RiemannSphere;
using namespace Easy3D;
///////////////////////

CameraManager::CameraManager(Easy3D::Camera *camera,
							 const Easy3D::Vec3& toPoint,
							 SpheresManager *smanager)
:camera(camera)
,smanager(smanager)
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
//////////////

//////INPUT
void CameraManager::onKeyDown(Key::Keyboard key){}
void CameraManager::onMouseScroll(short scroll){
    angle+=scroll*velocity.z*Application::instance()->getLastDeltaTime();
	angle= angle<velocity.z ? velocity.z : angle;
}
void CameraManager::onMouseDown(Vec2 mousePosition, Key::Mouse button){
    if(Key::list(button, Key::BUTTON_LEFT, Key::BUTTON_RIGHT)){
		//get ray collision
		onMove.ray=getMouseRay();
		onMove.collided=smanager
						 ->getCurSphere()
						 .rayCast(onMove.ray,onMove.segment);

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
		onClick.collided=smanager
						 ->getCurSphere()
						 .rayCast(onClick.ray,onClick.segment);
		//save rotation
		startPickRotation=Quaternion::fromMatrix(cameraPointer.getGlobalMatrix()).getNormalize();

	}
}
///////////


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
