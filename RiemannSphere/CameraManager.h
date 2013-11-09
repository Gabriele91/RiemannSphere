#ifndef CAMERAINPUT_H
#define CAMERAINPUT_H

#include <Easy3D.h>
#include <Rays.h>
#include <SpheresManager.h>

namespace RiemannSphere {
    
    struct CameraPositionInfo{
        float angle,n,f;
        Easy3D::Vec3 pos;
        Easy3D::Quaternion rot;
    };

	class CameraManager: public Easy3D::StateManager,
                                Easy3D::Input::KeyboardHandler,
                                Easy3D::Input::MouseHandler {

		
        Easy3D::Camera *camera;
        Easy3D::Object cameraPointer;
        Easy3D::Vec3   velocity;
        float angle,n,f;
        Sphere *sphere;
		//info rotation
		Easy3D::Quaternion startPickRotation;
		//info pick
		struct Pick{	
			Pick():collided(false){};
			Ray ray; Segment segment; bool collided;
		} onClick,onMove;
                                  
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
                                  
		CameraManager(Easy3D::Camera *camera,
					  const Easy3D::Vec3& toPoint,
					  Sphere *sphere);
                                    
        void setProjectionInfo(float angle,float n,float f);
        void changeFar(float f);
        void changeNear(float n);
        void setVelocity(const Easy3D::Vec3& vel);
        Ray getMouseRay();
        void drawFrustum(Easy3D::Render* r);
        CameraPositionInfo getCameraPositionInfo();
        void setCameraPositionInfo(const CameraPositionInfo& cpi);
                                    
                                  
        virtual void onStateStart();
        virtual void onStateRun(float dt);
        virtual void onStateEnd();
        virtual void onStateMessage(int msg);
        
        virtual void onKeyDown(Easy3D::Key::Keyboard key);
        virtual void onMouseScroll(short scroll);
        virtual void onMouseMove(Easy3D::Vec2 mousePosition);
        virtual void onMouseDown(Easy3D::Vec2 mousePosition,  Easy3D::Key::Mouse button);
		virtual void onMousePress(Easy3D::Vec2 mousePosition,  Easy3D::Key::Mouse button);
        virtual void onMouseRelease(Easy3D::Vec2 mousePosition,  Easy3D::Key::Mouse button);
	};

};

#endif