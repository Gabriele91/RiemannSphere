#ifndef GEODESICSPHERE_H
#define GEODESICSPHERE_H

#include <Easy3D.h>
#include <GeodesicNode.h>
#include <GeodesicTree.h>
#include <VirtualVbo.h>
#include <PoolThread.h>

namespace RiemannSphere {
    
    class GeodesicNode;
    class GeodesicTree;
    class GeodesicSphere;
    
	class GeodesicSphere {
        
        friend class GeodesicNode;
        friend class GeodesicTree;
        friend class GeodesicSphere;
        
        //tris
        GeodesicTree tris[20];
        //infos
        Easy3D::Camera *camera;
        float radius;
        int drawLevel;
        
        //build trees
        void build(float radius,size_t sBufferNodes);
        
        //math function
		Fractal* fractal;
        
        //vbo allocator
		VirtualVBO virtualVBO;
        
        //pool
		PoolThread *multithread;
		DFORCEINLINE void addBuildTask(DFUNCTION <void (void)> fn){
			multithread->addTaskFront(fn);
		}
        
		//build list
		std::list< GeodesicNode* > meshToBuilds;
		Easy3D::Mutex mutexBuildList;
		void addMeshToBuild(GeodesicNode* mesh);
		void doBuildsList();
        
        //debug draw
        void debugDraw(Easy3D::Render *render,GeodesicNode *node);
        void debugSubDraw(Easy3D::Render *render,GeodesicNode *node);
        
        //draw subs
        bool drawSub(GeodesicNode *node,
                     GeodesicTree& tri,
                     int countlivel);
        
        void allocTreeLivels(GeodesicNode *node,
                             GeodesicTree& tri,
                             int n);
        
	public:

		GeodesicSphere(Easy3D::Camera *camera,
                       Fractal* fractal,
                       float radius,
                       size_t sBufferNodes,
                       size_t sBufferVBO );
        ~GeodesicSphere();
        
        void drawNodes(Easy3D::Render* render);
        void draw(Easy3D::Render* render,int level);

	};

};

#endif