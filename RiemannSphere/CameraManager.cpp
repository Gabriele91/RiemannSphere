#include <stdafx.h>
#include <CameraManager.h>

///////////////////////
using namespace RiemannSphere;
using namespace Easy3D;
///////////////////////

CameraManager::CameraManager(Easy3D::Camera *camera,const Easy3D::Vec3& toPoint)
:camera(camera)
{
    addState(ON_ENABLE);
    addState(ON_DISABLE);
    setNextState(ON_ENABLE);
    //set point to point
    cameraPointer.addChild(camera,Object::ParentMode::ENABLE_PARENT,false);
    cameraPointer.setPosition(toPoint);
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
}
void CameraManager::onMouseDown(Vec2 mousePosition, Key::Mouse button){
    if(Key::list(button, Key::BUTTON_LEFT, Key::BUTTON_RIGHT)){
        sendMessage(DO_POINT);
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

void CameraManager::onStateRun(float dt){
    
    //update projection
    camera->setPerspective(angle,n, f);
    /*
    float aspect=(float)Application::instance()->getScreen()->getWidth()/
                 (float)Application::instance()->getScreen()->getHeight();
    float top=n/(cos(angle*0.5)/sin(angle*0.5));
    float bottom=-top;
    float right_=top*aspect;
    float left_=-right_;
    camera->setPerspective(left_,right_,bottom,top, n,f);*/
    camera->update();
    
    
    //update rotation
    if(getLastMessage()==DO_POINT){
        
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
