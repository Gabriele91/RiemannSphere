#ifndef GEODESICNODE_H
#define GEODESICNODE_H

#include <Easy3D.h>
#include <GeodesicTri.h>
#include <VirtualTree.h>
#include <VirtualVBO.h>
#include <Fractal.h>

namespace RiemannSphere {
    
    class GeodesicNode;
    class GeodesicTree;
    class GeodesicSphere;
    
	class GeodesicNode : public VirtualTree<GeodesicNode,4>::Node {
        
        
        friend class GeodesicNode;
        friend class GeodesicTree;
        friend class GeodesicSphere;
        //tris
        GeodesicTri tri;
        //boxs
        Easy3D::AABox box;
        //sub part
        float radius;
        int level;
        //vertices info
		VirtualVBO::Node nvbo;
		bool isvirtual;
		bool inbuilding;
        //utiliti draw
        void triangles(GLfloat*& v,
                       const Fractal* fractal,
                       const Easy3D::Vec3& a,
                       const Easy3D::Vec3& b,
                       const Easy3D::Vec3& c);
        void subtriangles(GLfloat*& v,
                          const Fractal* fractal,
                          int pass,
                          const Easy3D::Vec3& a,
                          const Easy3D::Vec3& b,
                          const Easy3D::Vec3& c);
        //build mesh
        void buildMesh(GeodesicSphere& smanager,
                       const Fractal* fractal);
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

		GeodesicNode();
		virtual ~GeodesicNode(); //virtual?

	};

};

#endif