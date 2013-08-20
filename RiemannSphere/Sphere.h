#ifndef SPHERE_H
#define SPHERE_H

#include <Easy3D.h>

namespace RiemannSphere {
    //////////////
    class SphereMesh;
    //////////////
	class Sphere {
        
        int rings;
        int sectors;
        float radius;

	public:

		Sphere(int rings,int sectors,float radius);
        
        DFORCEINLINE int getRings() const { return rings; }
        DFORCEINLINE int getSectors() const { return sectors; }
        DFORCEINLINE float getRadius() const { return radius; }
        
        SphereMesh *genMesh(float rStart,float sStart,float rEnd,float sEnd);
        Easy3D::AABox genAABox(float rStart,float sStart,float rEnd,float sEnd) const;
        Easy3D::Vec3 getPoint(int pRings,int pSectors) const;

	};

};

#endif