#ifndef CAMERAINPUT_H
#define CAMERAINPUT_H

#include <Easy3D.h>

namespace RiemannSphere {

	class CameraManager: public Easy3D::StateManager,
                                Easy3D::Input::KeyboardHandler,
                                Easy3D::Input::MouseHandler {

        Easy3D::Camera *camera;
        Easy3D::Object cameraPointer;
        Easy3D::Vec3   velocity;
        float angle,n,f;
                                  
	public:
        
        DFORCEINLINE float getAngle(){return angle;}
        
        enum EVENTS{
          ON_ENABLE=1,
          ON_DISABLE=2
        };
        enum MESSAGE{
            DO_POINT=1,
            NO_POINT=2
        };
                                  
		CameraManager(Easy3D::Camera *camera,const Easy3D::Vec3& toPoint);
        
        void setProjectionInfo(float angle,float n,float f);
        void setVelocity(const Easy3D::Vec3& vel);
                                  
        virtual void onStateStart();
        virtual void onStateRun(float dt);
        virtual void onStateEnd();
        virtual void onStateMessage(int msg);
        
        virtual void onKeyDown(Easy3D::Key::Keyboard key);
        virtual void onMouseScroll(short scroll);
        virtual void onMouseDown(Easy3D::Vec2 mousePosition,  Easy3D::Key::Mouse button);
	};

};

#endif