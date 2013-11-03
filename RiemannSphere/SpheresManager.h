#ifndef SPHERESMANAGER_H
#define SPHERESMANAGER_H

#include <Easy3D.h>
#include <SphereMesh.h>
#include <Fractal.h>
#include <PoolThread.h>
#include <VirtualTree.h>
#include <VirtualVBO.h>

namespace RiemannSphere {

	class SpheresManager : public VirtualTree<SphereMesh,8>  {
        
        //infos
        Easy3D::Camera *camera;
        
        //sphere
        int curLevel;
        std::vector<Sphere> spheres;
        double dfPerLivel,ringsFactor,sgmentsFactor;
        
        //math function
		Fractal* fractal;
		
        //vbo allocator
		VirtualVBO virtualVBO;
        
        //octree
        void subMeshADiv(SphereMesh* mesh,int i,Easy3D::ushort idSphere,const SubSphere& sub);
        
        //build function
        virtual SphereMesh* buildNode(SphereMesh* parent,Easy3D::uchar i);
       
        //delete function
        bool isDeletableChild(SphereMesh* node);
        virtual bool isDeletable(SphereMesh* node);
        
        //octree manager
        void buildLivels(int rings,int sgments,int livels,float radius,double dfPerLivel=2.0);
        
        //draw trees
        bool drawSub(SphereMesh *node,int countlivel);


		
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

		//friends class
		friend class SphereMesh;

	public:

		SpheresManager(Easy3D::Camera* camera,
                       Fractal* fractal,
					   int rings,int sgments,
					   int livels,float radius,
					   double dfPerLivel=2.0);
        //query
        void setLevel(int level){
            curLevel=level;
        }
        int getLevel(){
            return curLevel;
        }
        //spheres
        Sphere& getSphere(Easy3D::ushort i){
            return spheres[i];
        }
        const Sphere& getSphere(Easy3D::ushort i) const{
            return spheres[i];
        }
        Sphere& getCurSphere(){
            return spheres[curLevel];
        }
        const Sphere& getCurSphere() const{
            return spheres[curLevel];
        }
        //size info
        Easy3D::ulong getVboSize(){
            return virtualVBO.globalSize();
        }
        Easy3D::ulong getTreeSize(){
            return gSize;
        }
        Easy3D::ulong getTreeNodes(){
            return gSize/sizeof(SphereMesh);
        }
        //get current camera
        Easy3D::Camera *getCamera(){
            return camera;
        }
        //draw
        void draw();
        //destructor
		virtual ~SpheresManager();
        //draw cubes
        void drawCubes(Easy3D::Render* render);
        /*
         
         
         //void drawCube(int livel);
         //void drawSubCube(Easy3D::Camera &camera,int countlivel,int node);
         //void subMeshDiv8(SphereMesh* meshs,const Sphere& sphere,const SubSphere& sub);
         //void subDiv8(int liv,int mid,const Sphere& sphere,const SubSphere& sub);
         
         */
	};

};

#endif