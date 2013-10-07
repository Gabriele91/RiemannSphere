#ifndef RAYS_H
#define RAYS_H

#include <Easy3D.h>

namespace RiemannSphere {

    struct Ray{
        
        Easy3D::Vec3 point;
		Easy3D::Vec3 dir;
        
		Ray(const Easy3D::Vec3& point=Easy3D::Vec3::ZERO,
            const Easy3D::Vec3& dir=Easy3D::Vec3::ZERO)
            :point(point)
            ,dir(dir.getNormalize()){
		}
        
		void draw(Easy3D::Render* render,float leng=10000);
	};
    
	struct Segment{
        
		Easy3D::Vec3 t[2];
		void draw(Easy3D::Render* render);
	};


};

#endif