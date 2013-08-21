#ifndef SPHERESMANAGER_H
#define SPHERESMANAGER_H

#include <Easy3D.h>
#include <SphereMesh.h>

namespace RiemannSphere {

	class SpheresManager {
        
        //octree
        int livels;
        std::vector<SphereMesh> meshs;
        
        int setTreeSize(int h){
            //size: (sommatoria notevole)
            // ((8^(x+1) - 1 )/7 )
            int size= (std::pow(8,h+1)-1)/7;
            //new buffer
            meshs.resize(size);
            //return size
            return size;
        }
        int getChilds(int node){
            return (8*node)+1;
        }
        
        
        //octree manager
        void subMeshDiv8(SphereMesh* meshs,const Sphere& sphere,const SubSphere& sub);
        void subDiv8(int liv,int mid,const Sphere& sphere,const SubSphere& sub);
        void buildLivels(int rings,int sgments,int livels,float radius);
        
        //draw trees
        void drawSub(Easy3D::Camera &camera,int countlivel,int node);
        void drawSubCube(Easy3D::Camera &camera,int countlivel,int node);
        
	public:

		SpheresManager(int rings,int sgments,int livels,float radius);
        void draw(Easy3D::Camera &camera,int livel);
        void drawCube(Easy3D::Camera &camera,int livel);

	};

};

#endif