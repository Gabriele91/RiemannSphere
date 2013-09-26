#ifndef SPHEREMESH_H
#define SPHEREMESH_H

#include <Easy3D.h>
#include <Sphere.h>
#include <NewtonFractal.h>
#include <PoolThread.h>

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
        //cpu side
		void *cpuVertexBuffer;
		size_t cpuVertexBufferSize;
		bool build;
        //ids gpu
        GLuint vertexBuffer;
        size_t vertexBufferSize;
		bool canDraw;
        //set info
        void setMeshInfo(const Sphere& sphere,const SubSphere& sub);
        //build mesh
        void buildMesh(SpheresManager& smanager,const NewtonFractal<float>& newton);
        void buildMesh(SpheresManager& smanager,const NewtonFractal<double>& newton);
        //draw
        bool draw();
        
        //info
        DFORCEINLINE bool isBuild() const {
            return vertexBuffer!=0;
        }       
        DFORCEINLINE bool isDrawenable() const {
            return build;
        }       
		//send cpu buffer to gpu
		void cpuBufferToGpu();
		void freeCpuBuffers();

        
	public:
        
        //spheres
		SphereMesh();
        
        DFORCEINLINE const Easy3D::AABox& getAABox() const{
            return box;
        }

	};

};

#endif