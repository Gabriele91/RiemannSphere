#ifndef SPHEREMESH_H
#define SPHEREMESH_H

#include <Easy3D.h>

namespace RiemannSphere {
    /////////////////
    class Sphere;
    /////////////////
	class SphereMesh {
        
        friend class Sphere;
        Easy3D::AABox box;
        GLuint vertexBuffer;
        size_t vertexBufferSize;
        
        void buildMesh(const Sphere& sphere,
                       int rStart,
                       int sStart,
                       int rEnd,
                       int sEnd);
        
		SphereMesh(const Sphere& sphere);
		SphereMesh(const Sphere& sphere,
                   int rStart,
                   int sStart,
                   int rEnd,
                   int sEnd
                   );

	public:
        
        void draw();
        
        DFORCEINLINE const Easy3D::AABox& getAABox() const{
            return box;
        }

	};

};

#endif