#ifndef RIEMANNSCENEGLSL_H
#define RIEMANNSCENEGLSL_H

#include <Easy3D.h>
#include <Polynomial.h>
#include <CameraManager.h>
#include <DrawSymbols.h>

namespace RiemannSphere {


	class RiemannSceneGLSL: public Easy3D::Scene,
                                Easy3D::Input::KeyboardHandler,
                                Easy3D::Input::MouseHandler
    {
        
		Sphere  sphere;
        Easy3D::Table polynomialConfig;
        Polynomial<double> poly;
        CameraManager  *cameraManager;
        Easy3D::Camera camera;
        Easy3D::Object obj;
        Easy3D::Font aharoni;
		Easy3D::Shader newtonShader;
		Easy3D::Shader halleyShader;
		Easy3D::Shader schroederShader;
		DrawSymbols drawSymbols;

		struct FractalShader{
			Easy3D::Shader *sheder;
			std::vector<Easy3D::Vec2> constants;
			std::vector<Easy3D::Vec2> roots;
			std::vector<Easy3D::Vec4> colors;
			FractalShader(Polynomial<double>& poly);
			void bind();
			void unbind();
		}fractal;

        
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

		void drawFontIn3DScene(const Easy3D::Vec3& pos,
							   const Easy3D::String& text,
							   const Easy3D::Vec2& scale,
							   const Easy3D::Color& color=Easy3D::Color());
		
		
	};
};

#endif