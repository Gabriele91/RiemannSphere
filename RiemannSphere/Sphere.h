#ifndef SPHERE_H
#define SPHERE_H

#include <Easy3D.h>

namespace RiemannSphere {
    
    
    struct SubSphere {
        
        int rStart;
        int rEnd;
        int sStart;
        int sEnd;
        
        SubSphere();
        SubSphere(int rStart,
                  int rEnd,
                  int sStart,
                  int sEnd);
        
    };
    
	struct Sphere {
        
        int rings;
        int sectors;
        float radius;
        
        Sphere();
        Sphere(int rings,
               int sectors,
               float radius);
        
        Easy3D::AABox genAABox(const SubSphere& sub) const;
        Easy3D::Vec3 getPoint(int pRings,int pSectors) const;
	};
};

#endif