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
		
		sendMessage(DO_POINT);
    }
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
		//get mouse pos
		lastMousePos=mousePosition;
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
    return Ray(pos,dir);
}

float rotNormalize(float rot){
	rot=std::fmodf(rot,(float)Math::PI2);
	if(rot<0){
		rot+=Math::PI2;
	}
	return rot;
}
Vec3 dirToEuler(const Vec3& d){

	float r = sqrt(d.x*d.x + d.y*d.y + d.z*d.z);
	float t = atan2(d.y,d.x);
	float p = atan2(d.z,d.r);

	return Vec3(rotNormalize(t),rotNormalize(p),rotNormalize(r));
}

void CameraManager::onStateRun(float dt){
    
    //update projection
    camera->setPerspective(angle,n, f);
    camera->update();
    
    //update rotation
    if(getLastMessage()==DO_POINT){
		if(onClick.collided&&onMove.collided){
			/*
			Vec3 start=onClick.segment.t[0].getNormalize();
			Vec3 end=onMove.segment.t[0].getNormalize();
			//rote only Yaw
			float syaw=dirToEuler(start).y;
			float eyaw=dirToEuler(end).y;
			//rote only Pitch
			float spitch=dirToEuler(start).x;
			float epitch=dirToEuler(end).x;
			
			Debug::message()<< "start: " << (dirToEuler(start)*Math::G180OVERPI).toString() <<"\n";
			Debug::message()<< "end: " << (dirToEuler(end)*Math::G180OVERPI).toString() <<"\n";
			*/
			//
			//turn
			//up vector not work
			Vec3 vStart=(onClick.segment.t[0]-cameraPointer.getPosition(true)).getNormalize();
			vStart=cameraPointer.getRotation(true).getNormalize().getInverse().getMatrix().mul(Vec4(vStart,0)).xyz();
			Quaternion rotStart=Quaternion::fromLookRotation(vStart,Vec3(0,1,0)).getNormalize();

			Vec3 vEnd=(onMove.segment.t[0]-cameraPointer.getPosition(true)).getNormalize();
			vEnd=cameraPointer.getRotation(true).getNormalize().getInverse().getMatrix().mul(Vec4(vEnd,0)).xyz();
			Quaternion rotEnd=Quaternion::fromLookRotation(vEnd,Vec3(0,1,0)).getNormalize();

			cameraPointer.setRotation(
				startPickRotation.getInverse().mul(rotEnd.mul(rotStart.getInverse()))
				);
			
			sendMessage(NO_POINT);
		}
        /*
        //var declaretion
        Vec2  center(Application::instance()->getScreen()->getWidth()*0.5,
                     Application::instance()->getScreen()->getHeight()*0.5);
        Vec2  dir=Application::instance()->getInput()->getMouse()-center;
        float leng=dir.length();
        Vec3  euler;
        
        if(leng!=0.0f){
            //rotation
            cameraPointer.getRotation().getEulero(euler);
            if(dir.x>0) euler.y-=Math::torad(1)*leng*dt*velocity.y;
            if(dir.x<0) euler.y+=Math::torad(1)*leng*dt*velocity.y;
            
            if(dir.y>0) euler.x-=Math::torad(1)*leng*dt*velocity.x;
            if(dir.y<0) euler.x+=Math::torad(1)*leng*dt*velocity.x;
            cameraPointer.setRotation(Quaternion::fromEulero(euler));
        }
        
        //center screen
        Application::instance()->getScreen()->setPositionCursor(center);
        
        //end point
        sendMessage(NO_POINT);
		*/
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
