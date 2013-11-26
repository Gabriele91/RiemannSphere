#ifndef RIEMANNSCENEGLSL_H
#define RIEMANNSCENEGLSL_H

#include <Easy3D.h>
#include <Polynomial.h>
#include <CameraManager.h>
#include <GridMesh.h>
#include <GeodesicMesh.h>
#include <DrawSymbols.h>
#include <RiemannInterface.h>

namespace RiemannSphere {


	class RiemannSceneGLSL: public Easy3D::Scene,
                            public RiemannInterface,
                            Easy3D::Input::KeyboardHandler,
                            Easy3D::Input::MouseHandler
    {
        
        Polynomial<double> *poly;
        CameraManager  *cameraManager;
        Easy3D::Camera camera;
        Easy3D::Object obj;
        Easy3D::Shader shader[Iterations::FRACTMAX];
		DrawSymbols drawSymbols;
        GridMesh grid;
		Sphere  sphere;
        GeodesicMesh gfSphere;

		struct FractalShader{
            Easy3D::Shader* shader;
			std::vector<Easy3D::Vec2> constants;
			std::vector<Easy3D::Vec2> subconstants;
			std::vector<Easy3D::Vec2> roots;
			std::vector<Easy3D::Vec4> colors;
			Easy3D::Vec4 infiniteColor;
			FractalShader(Polynomial<double>& poly);
            virtual ~FractalShader(){}
		}fractal;
        
        void shaderbind();
        void shaderunbind();
        
        enum SceneInfo{
            ON_RESUME,
            ON_PAUSE
        };

        SceneInfo sceneInfo;

	public:
		
        RiemannSceneGLSL(Polynomial<double> *poly);
        ~RiemannSceneGLSL();
        
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