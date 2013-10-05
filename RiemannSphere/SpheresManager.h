#ifndef SPHERESMANAGER_H
#define SPHERESMANAGER_H

#include <Easy3D.h>
#include <SphereMesh.h>
#include <NewtonFractal.h>
#include <PoolThread.h>
#include <VirtualOctree.h>
#include <VirtualVBO.h>

namespace RiemannSphere {

	class SpheresManager : public VirtualOctree<SphereMesh>  {
        
        //infos
        Easy3D::Camera *camera;
        int curLevel;
        std::vector<Sphere> spheres;
        double dfPerLivel,ringsFactor,sgmentsFactor;
        //octree
        void subMeshADiv(SphereMesh* mesh,
                         int i,
                         Easy3D::ushort idSphere,
                         const SubSphere& sub);
        //build function
        virtual SphereMesh* buildNode(SphereMesh* parent,Easy3D::uchar i);
        //delete function
        bool isDeletableChild(SphereMesh* node);
        virtual bool isDeletable(SphereMesh* node);
        
        //spheres
        Sphere& getSphere(Easy3D::ushort i){
            return spheres[i];
        }
        const Sphere& getSphere(Easy3D::ushort i) const{
            return spheres[i];
        }
        
        //octree manager
        void buildLivels(int rings,int sgments,int livels,float radius,double dfPerLivel=2.0);
        
        //draw trees
        bool drawSub(SphereMesh *node,int countlivel);

		//math function
		Polynomial<double> poly;
		NewtonFractal<double> fractal;
		
		//pool
		PoolThread *multithread;
		DFORCEINLINE void addBuildTask(DFUNCTION <void (void)> fn){
			multithread->addTaskFront(fn);
		}
		//build list
		std::list< SphereMesh* > meshToBuilds;
		Easy3D::Mutex mutexBuildList;

		void addMeshToBuild(SphereMesh* mesh);
		void doBuildsList();

		//vbo allocator
		VirtualVBO virtualVBO;

		//friends class
		friend class SphereMesh;

	public:

		SpheresManager(Easy3D::Camera* camera,
                       const Easy3D::Utility::Path& polyfunction,
					   int rings,int sgments,
					   int livels,float radius,
					   double dfPerLivel=2.0);
        
        void setLevel(int level){
            curLevel=level;
        }
        int getLevel(){
            return curLevel;
        }
        
        void draw();
        
		virtual ~SpheresManager();
        
        Easy3D::ulong getVboSize(){
            return virtualVBO.globalSize();
        }
        Easy3D::ulong getTreeSize(){
            return gSize;
        }
        Easy3D::ulong getTreeNodes(){
            return gSize/sizeof(SphereMesh);
        }
        
        /*
         
         
         //void drawCube(int livel);
         //void drawSubCube(Easy3D::Camera &camera,int countlivel,int node);
         //void subMeshDiv8(SphereMesh* meshs,const Sphere& sphere,const SubSphere& sub);
         //void subDiv8(int liv,int mid,const Sphere& sphere,const SubSphere& sub);
         
         */
	};

};

#endif