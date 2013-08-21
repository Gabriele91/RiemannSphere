#ifndef SPHEREMESH_H
#define SPHEREMESH_H

#include <Easy3D.h>
#include <Sphere.h>

namespace RiemannSphere {
    
    /////////////////
    class SpheresManager;
    
    /////////////////
	class SphereMesh {
        
        //friends
        friend class SpheresManager;
        
        //boxs
        Easy3D::AABox box;
        
        //sub part
        Sphere sphere;
        SubSphere sub;
        
        //sphere 
        //ids
        GLuint vertexBuffer;
        size_t vertexBufferSize;
        
        //build mesh
        void setMeshInfo(const Sphere& sphere,
                         const SubSphere& sub);
        void buildMesh();
        
        //draw
        void draw() const;
        
        //info
        DFORCEINLINE bool isBuild() const {
            return vertexBuffer!=0;
        }
        
	public:
        
        //spheres
		SphereMesh();
        
        DFORCEINLINE const Easy3D::AABox& getAABox() const{
            return box;
        }

	};

};

#endif