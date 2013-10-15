#ifndef DRAWSYMBOLS_H
#define DRAWSYMBOLS_H

#include <Easy3D.h>

namespace RiemannSphere {

	class DrawSymbols {

	public:

		DrawSymbols(Easy3D::Camera *camera,
					Easy3D::Render *render,
					const Easy3D::Utility::Path& infinity,
					const Easy3D::Utility::Path& zero,
					const Easy3D::Utility::Path& point);
			
		void drawInfinity(const Easy3D::Vec3& pos,
							const Easy3D::Vec2& scale,
							float cstart=0.0f,float cend=1.0f,
							Easy3D::Color color=Easy3D::Color());

		void drawZero(const Easy3D::Vec3& pos,
						const Easy3D::Vec2& scale,
						float cstart=0.0f,float cend=1.0f,
						Easy3D::Color color=Easy3D::Color());

		void drawPoint(const Easy3D::Vec3& pos,
						const Easy3D::Vec2& scale,
						float cstart=0.0f,float cend=1.0f,
						Easy3D::Color color=Easy3D::Color());

	protected:
			
		Easy3D::Texture infinity,zero,point;
		Easy3D::Camera *camera;
		Easy3D::Render *render;

		float getColorIntensity(const Easy3D::Vec3& dir,
								float cstart,float cend);

		void drawSymbolIn3DScene(const Easy3D::Vec3& pos,
									const Easy3D::Vec2& scale,
									const Easy3D::Color& color=Easy3D::Color());
	};


};

#endif