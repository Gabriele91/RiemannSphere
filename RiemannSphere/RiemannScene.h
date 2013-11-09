#ifndef RIEMANNSCENE_H
#define RIEMANNSCENE_H

#include <Easy3D.h>
#include <CameraManager.h>
#include <SpheresManager.h>
#include <NewtonFractal.h>
#include <HalleyFractal.h>
#include <Halley4Fractal.h>
#include <SchroederFractal.h>
#include <Schroeder4Fractal.h>
#include <GenericFractal.h>
#include <DrawSymbols.h>

namespace RiemannSphere {

	class RiemannScene : public Easy3D::Scene,
                                Easy3D::Input::KeyboardHandler,
                                Easy3D::Input::MouseHandler
    {
        
        Easy3D::Camera camera;
        Easy3D::Object obj;
        CameraManager  *cameraManager;
        Easy3D::Table polynomialConfig;
        Polynomial<double> poly;
        NewtonFractal<double> newton;
        HalleyFractal<double> halley;
        Halley4Fractal<double> halley4;
        SchroederFractal<double> schroeder;
        Schroeder4Fractal<double> schroeder4;
        GenericFractal<double> genericfractal;
        SpheresManager *sphere;
        Easy3D::Font aharoni;
		DrawSymbols drawSymbols;
        
        enum SceneInfo{
            ON_RESUME,
            ON_PAUSE
        };
        SceneInfo sceneInfo;

	public:
		
        RiemannScene();
        ~RiemannScene();
        
        virtual void onPause();
        
        virtual void onResume();
        
        virtual void onStart();
        
        virtual void onRun(float dt);
        
        virtual void onEnd();
        
        //camera info
        CameraPositionInfo getCameraPositionInfo();
        void setCameraPositionInfo(const CameraPositionInfo& cpi);

        virtual void onKeyDown(Easy3D::Key::Keyboard key);
        virtual void onMouseDown(Easy3D::Vec2 mousePosition,
                                 Easy3D::Key::Mouse button);

		void drawFontIn3DScene(const Easy3D::Vec3& pos,
							   const Easy3D::String& text,
							   const Easy3D::Vec2& scale);
	};

};

#endif