#ifndef RIEMANNSCENEGLSL_H
#define RIEMANNSCENEGLSL_H

#include <Easy3D.h>
#include <Polynomial.h>
#include <CameraManager.h>

namespace RiemannSphere {

	class RiemannSceneGLSL: public Easy3D::Scene,
                                Easy3D::Input::KeyboardHandler,
                                Easy3D::Input::MouseHandler
    {
        
        Polynomial<double> poly;
		Sphere  sphere;
        CameraManager  *cameraManager;
        Easy3D::Camera camera;
        Easy3D::Object obj;
        Easy3D::Font aharoni;
		Easy3D::Shader newtonShader;
        
        enum SceneInfo{
            ON_RESUME,
            ON_PAUSE
        };

        SceneInfo sceneInfo;

	public:
		
        RiemannSceneGLSL();
        ~RiemannSceneGLSL();
        
        virtual void onPause();
        
        virtual void onResume();
        
        virtual void onStart();
        
        virtual void onRun(float dt);
        
        virtual void onEnd();

        virtual void onKeyDown(Easy3D::Key::Keyboard key);
        virtual void onMouseDown(Easy3D::Vec2 mousePosition,
                                 Easy3D::Key::Mouse button);
	};

};

#endif