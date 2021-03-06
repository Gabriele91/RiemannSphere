#ifndef RIEMANNSCENEGEODESIC_H
#define RIEMANNSCENEGEODESIC_H

#include <Easy3D.h>
#include <CameraManager.h>
#include <GeodesicSphere.h>
#include <NewtonFractal.h>
#include <HalleyFractal.h>
#include <Halley4Fractal.h>
#include <SchroederFractal.h>
#include <Schroeder4Fractal.h>
#include <GenericFractal.h>
#include <DrawSymbols.h>
#include <GridMesh.h>
#include <RiemannInterface.h>

namespace RiemannSphere {

	class RiemannSceneGeodesic : public Easy3D::Scene,
                                 public RiemannInterface,
                                 Easy3D::Input::KeyboardHandler,
                                 Easy3D::Input::MouseHandler
    {
        
        Easy3D::Camera camera;
        Easy3D::Object obj;
        CameraManager  *cameraManager;
        Polynomial<double> *poly;
        NewtonFractal<double> newton;
        HalleyFractal<double> halley;
        Halley4Fractal<double> halley4;
        SchroederFractal<double> schroeder;
        Schroeder4Fractal<double> schroeder4;
        GenericFractal<double> genericfractal;
		DrawSymbols drawSymbols;
        GeodesicSphere *sphere;
		Sphere  camera_sphere;
        GridMesh grid;
        void drawGridHack();
        
        enum SceneInfo{
            ON_RESUME,
            ON_PAUSE
        };
        SceneInfo sceneInfo;

	public:
		
        RiemannSceneGeodesic(Polynomial<double> *poly);
        ~RiemannSceneGeodesic();
        
        virtual void onPause();
        
        virtual void onResume();
        
        virtual void onStart();
        
        virtual void onRun(float dt);
        
        virtual void onEnd();

		virtual void lock();
		virtual void unlock();
        //camera info
        virtual CameraPositionInfo getCameraPositionInfo();
        virtual void setCameraPositionInfo(const CameraPositionInfo& cpi);

        virtual void onKeyDown(Easy3D::Key::Keyboard key);
        virtual void onMouseDown(Easy3D::Vec2 mousePosition,
                                 Easy3D::Key::Mouse button);
	};

};

#endif