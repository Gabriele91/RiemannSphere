#include <stdafx.h>
#include <Debug.h>
#include <SpheresManager.h>

///////////////////////
using namespace Easy3D;
using namespace RiemannSphere;
///////////////////////

SpheresManager::SpheresManager(const Easy3D::Utility::Path& polyfunction,
							   int rings,
							   int sgments,
							   int livels, 
							   float radius,
							   float dfPerLivel)
							   :multithread(NULL)
							   ,poly(polyfunction)
							   ,fractal(&poly)
							   ,virtualVBO(536870912) //512 MByte
{
    buildLivels(rings,sgments,livels,radius,dfPerLivel);
	multithread=new PoolThread(4);
}

SpheresManager::~SpheresManager(){
	delete multithread;
}

void SpheresManager::subMeshDiv8(SphereMesh* meshs,const Sphere& sphere,const SubSphere& sub){
    //calc division rigns
    const int
        up=sub.rStart,
        middle=(int)(sub.rStart+(sub.rEnd-sub.rStart)/2.0f),
        down=sub.rEnd;
    //calc division sections
    const int hlpart=(int)((sub.sEnd-sub.sStart)/4.0f);
    const int
    part1=sub.sStart,
    part2=sub.sStart+hlpart,
    part3=sub.sStart+hlpart*2,
    part4=sub.sStart+hlpart*3,
    part5=sub.sEnd;

	//hasert
	if(part1==part2 || part2==part3 || part3==part4 || part4==part5)
		Debug::message()<<"parts:"<<part1<<" "<<part2<<" "<<part3<<" "<<part4<<" "<<part5<<"\n";
    //build parts
    //up
    meshs[0].setMeshInfo(sphere,{up,middle,part1,part2});
    meshs[1].setMeshInfo(sphere,{up,middle,part2,part3});
    meshs[2].setMeshInfo(sphere,{up,middle,part3,part4});
    meshs[3].setMeshInfo(sphere,{up,middle,part4,part5});
    //down
    meshs[4].setMeshInfo(sphere,{middle,down,part1,part2});
    meshs[5].setMeshInfo(sphere,{middle,down,part2,part3});
    meshs[6].setMeshInfo(sphere,{middle,down,part3,part4});
    meshs[7].setMeshInfo(sphere,{middle,down,part4,part5});
}
void SpheresManager::subDiv8(int liv,int mid,const Sphere& sphere,const SubSphere& sub){
    //gen mesh
    if(liv<=0){
        subMeshDiv8(&meshs[getChilds(mid)],sphere,sub);
        return;
    }
    //sub mesh
    //calc division rigns
    const int
    up=sub.rStart,
    middle=(int)(sub.rStart+(sub.rEnd-sub.rStart)/2.0f),
    down=sub.rEnd;
    //calc division sections
    const int hlpart=(int)((sub.sEnd-sub.sStart)/4.0f);
    const int
    part1=sub.sStart,
    part2=sub.sStart+hlpart,
    part3=sub.sStart+hlpart*2,
    part4=sub.sStart+hlpart*3,
    part5=sub.sEnd;
    //build parts
    //up
    subDiv8(liv-1,getChilds(mid),sphere,{up,middle,part1,part2});
    subDiv8(liv-1,getChilds(mid)+1,sphere,{up,middle,part2,part3});
    subDiv8(liv-1,getChilds(mid)+2,sphere,{up,middle,part3,part4});
    subDiv8(liv-1,getChilds(mid)+3,sphere,{up,middle,part4,part5});
    //down
    subDiv8(liv-1,getChilds(mid)+4,sphere,{middle,down,part1,part2});
    subDiv8(liv-1,getChilds(mid)+5,sphere,{middle,down,part2,part3});
    subDiv8(liv-1,getChilds(mid)+6,sphere,{middle,down,part3,part4});
    subDiv8(liv-1,getChilds(mid)+7,sphere,{middle,down,part4,part5});
}

//#define ENABLE_CACHE
void SpheresManager::buildLivels(int rings,int sgments,int livels, float radius,float dfPerLivel){
    
    DEBUG_ASSERT(livels>0);
    //savelivels
    this->livels=livels;
    //calc factor
    float ringsFactor=(float)rings / (livels*dfPerLivel);
    float sgmentsFactor=(float)sgments / (livels*dfPerLivel);
    //set tree size
    setTreeSize(livels);

#ifdef ENABLE_CACHE
    /*
     get file
    */
    Utility::Path path(String("temp/")+rings+"_"+sgments+"_"+livels+"_"+radius+"_"+dfPerLivel+"_"+DEBUG_MODE+".save");
    
    if(path.existsFile() ){
       FILE *file=fopen(path, "rb");
       if(file){
		   for(size_t i=0;i!=meshs.size();++i){
				fread(&meshs[i].sub,            sizeof(SubSphere), 1, file);
				fread(&meshs[i].sphere,         sizeof(Sphere), 1, file);
				fread(&meshs[i].box,            sizeof(Easy3D::AABox), 1, file);
				fread(&meshs[i].isvirtual,      sizeof(bool), 1, file);
			}
            fclose(file);
       }
   }
   else
   {
#endif
        //gen meshs
        for (int l=0; l<livels; ++l) {
            //sphere
            Sphere sphere(
               (int)( ringsFactor*(l+1)*(dfPerLivel/(livels-l)) ),
               (int)( sgmentsFactor*(l+1)*(dfPerLivel/(livels-l)) ),
                radius+0.0004f*l
            );
            //divs
            subDiv8(l, 0, sphere, {  0, sphere.rings, 0, sphere.sectors  });
            
        }
	   //first livel must to be allocated
       for(int c=0;c<8;++c) meshs[getChilds(0)+c].isvirtual=false;
       
#ifdef ENABLE_CACHE
        /*
         Save into the file
		*/
        FILE *file=fopen(path, "wb");
        if(file){
			for(size_t i=0;i!=meshs.size();++i){
				fwrite(&meshs[i].sub,            sizeof(SubSphere), 1, file);
				fwrite(&meshs[i].sphere,         sizeof(Sphere), 1, file);
				fwrite(&meshs[i].box,            sizeof(Easy3D::AABox), 1, file);
				fwrite(&meshs[i].isvirtual,    sizeof(bool), 1, file);
			}
            fclose(file);
        }
    }
#endif
}

void SpheresManager::addMeshToBuild(SphereMesh* mesh){
	mutexBuildList.lock();
	meshToBuilds.push_front(mesh);
	mutexBuildList.unlock();
}
void SpheresManager::doBuildsList(){
	mutexBuildList.lock();
		for(auto mesh:meshToBuilds)
			mesh->sendToGpu(&virtualVBO);
		meshToBuilds.clear();
	mutexBuildList.unlock();
}

bool SpheresManager::drawSub(Easy3D::Camera &camera,int countlivel,int node){

    bool drawFather=false;

    if(countlivel==0){
        for(int c=0;c<8;++c)
            if(camera.boxInFrustum( meshs[getChilds(node)+c].box )){
                //to do: separate thread
				if(!meshs[getChilds(node)+c].lockTask())
                    meshs[getChilds(node)+c].buildMesh(*this,camera,fractal);
                //draw
                drawFather=meshs[getChilds(node)+c].draw()&&drawFather;
            }
    }
    else{

        for(int c=0;c<8;++c)
            if(camera.boxInFrustum( meshs[getChilds(node)+c].box )){
				if(!drawSub(camera,countlivel-1,getChilds(node)+c)){
					//draw
					drawFather=meshs[getChilds(node)+c].draw()&&drawFather;
				}
			}
    }
	//draw Father?
    return drawFather;
}
void SpheresManager::draw(Easy3D::Camera &camera,int livel){
	virtualVBO.updateMemory();
	doBuildsList();
    drawSub(camera,livel,0);
}

/*
TODO
void SpheresManager::drawSubCube(Easy3D::Camera &camera,int countlivel,int node){
    
    if(countlivel==0){
        for(int c=0;c<8;++c)
            if(camera.boxInFrustum( meshs[getChilds(node)+c].box )){
                //to do: separate thread
                if(!meshs[getChilds(node)+c].isBuild())
                    meshs[getChilds(node)+c].buildMesh();
                //
                meshs[getChilds(node)+c].draw();
            }
    }
    else{
        for(int c=0;c<8;++c)
            if(camera.boxInFrustum( meshs[getChilds(node)+c].box ))
                drawSub(camera,countlivel-1,getChilds(node)+c);
    }
    
}
void SpheresManager::drawCube(Easy3D::Camera &camera,int livel){
    drawSub(camera,livel,0);
}*/




