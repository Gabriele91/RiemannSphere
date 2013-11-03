#ifndef GEODESICTRI_H
#define GEODESICTRI_H

#include <Easy3D.h>

namespace RiemannSphere {

	struct GeodesicTri {
        
        Easy3D::Vec3 point[3];
        void getAABox(Easy3D::AABox& box,float radius);
        void genChild(GeodesicTri& c,Easy3D::uchar i,float radius);

	};

};

#endif