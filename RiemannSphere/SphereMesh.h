#ifndef SPHEREMESH_H
#define SPHEREMESH_H

#include <Easy3D.h>
#include <Sphere.h>
#include <NewtonFractal.h>
#include <PoolThread.h>
#include <VirtualOctree.h>
#include <VirtualVBO.h>

namespace RiemannSphere {
    
    /////////////////
    class SpheresManager;
    
    /////////////////
	class SphereMesh : public VirtualOctree<SphereMesh>::Node {
        //friends
        friend class SpheresManager;
        //boxs
        Easy3D::AABox box;
        //sub part
        Easy3D::ushort idSphere;
        SubSphere sub;
        //vertices info
		VirtualVBO::Node nvbo;
		bool isvirtual;
		bool inbuilding;
        //set info
        void setMeshInfo(Easy3D::ushort idSphere,
                         const SubSphere& sub,
                         const SpheresManager& smanager);
        //build mesh
        void buildMesh(SpheresManager& smanager, 
					   const Easy3D::Camera& camera,
					   const NewtonFractal<float>& newton);
        void buildMesh(SpheresManager& smanager,
					   const Easy3D::Camera& camera,
					   const NewtonFractal<double>& newton);
        //draw
        bool draw();
		//send to gpu
		void sendToGpu(VirtualVBO* vVBO);
		//dealloc
		void freeCpuBuffers();
		//query
		bool lockTask(){
			return nvbo.isAllocated() || inbuilding;
		}
        
	public:
        
        //spheres
		SphereMesh(bool avirtualVBO=true);
		virtual ~SphereMesh(); //virtual?
        DFORCEINLINE const Easy3D::AABox& getAABox() const{
            return box;
        }

	};

};

#endif