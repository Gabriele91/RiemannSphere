#include <stdafx.h>
#include <Debug.h>
#include <SpheresManager.h>

///////////////////////
using namespace Easy3D;
using namespace RiemannSphere;
///////////////////////

SpheresManager::SpheresManager(Easy3D::Camera* camera,
                               Fractal* fractal,
							   int rings,
							   int sgments,
							   int livels, 
							   float radius,
							   double dfPerLivel)
							   :VirtualOctree<SphereMesh>(536870912*(1.0/8.0)) //512 MByte
                               ,camera(camera)
                               ,curLevel(0)
                               ,multithread(NULL)
							   ,fractal(fractal)
							   ,virtualVBO(536870912*1.8) //512 MByte
{
    buildLivels(rings,sgments,livels,radius,dfPerLivel);
	multithread=new PoolThread(4);
}

SpheresManager::~SpheresManager(){
	delete multithread;
    //all data are not in bulding (threads are dead)
    for(auto mesh:meshToBuilds) mesh->inbuilding=false;
}

void SpheresManager::subMeshADiv(SphereMesh* meshs,
                                 int i,
                                 Easy3D::ushort idSphere,
                                 const SubSphere& sub){
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
    
	//assert
	if(part1==part2 || part2==part3 || part3==part4 || part4==part5)
		Debug::message()<<"parts:"<<part1<<" "<<part2<<" "<<part3<<" "<<part4<<" "<<part5<<"\n";
    //build parts
    switch (i) {
        //up
        case 0:  meshs->setMeshInfo(idSphere,{up,middle,part1,part2},*this); break;
        case 1:  meshs->setMeshInfo(idSphere,{up,middle,part2,part3},*this); break;
        case 2:  meshs->setMeshInfo(idSphere,{up,middle,part3,part4},*this); break;
        case 3:  meshs->setMeshInfo(idSphere,{up,middle,part4,part5},*this); break;
        //down
        case 4:  meshs->setMeshInfo(idSphere,{middle,down,part1,part2},*this); break;
        case 5:  meshs->setMeshInfo(idSphere,{middle,down,part2,part3},*this); break;
        case 6:  meshs->setMeshInfo(idSphere,{middle,down,part3,part4},*this); break;
        case 7:  meshs->setMeshInfo(idSphere,{middle,down,part4,part5},*this); break;
        default: Debug::message()<<"parts: id invalid\n"; break;
    }
    
    
}
/*
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
}*/

//build function
SphereMesh* SpheresManager::buildNode(SphereMesh* parent,Easy3D::uchar i){
    SphereMesh* tmp=new SphereMesh();
    SubSphere sub=parent->sub/getSphere(parent->idSphere)*getSphere(parent->idSphere+1);
    subMeshADiv(tmp,i,parent->idSphere+1,sub);
    return tmp;
}
//delete function
bool SpheresManager::isDeletableChild(SphereMesh* node){
    if(node->isvirtual && ( !node->lockTask() ) ){
        bool deletablechild=true;
        for(Easy3D::uchar c=0; c!=8 && deletablechild; ++c){
            if(node->getChild(c))
                deletablechild= deletablechild && isDeletableChild(node->getChild(c));
        }
        return deletablechild;
    }
    return false;
}
bool SpheresManager::isDeletable(SphereMesh* node){
    if(!node) return false;
    
    bool deletablenode=
          node->isvirtual
    && ( !node->lockTask() )
    && ( node->idSphere < ( (int)(spheres.size()*0.5+1) ) )
    && ( node->idSphere > (curLevel+1) || ( !camera->boxInFrustum(node->getAABox()) ) );
    
    if(deletablenode){
        bool deletablechild=true;
        for(Easy3D::uchar c=0; c!=8 && deletablechild; ++c){
            if(node->getChild(c))
                deletablechild= deletablechild && isDeletableChild(node->getChild(c));
        }
        return deletablechild;
    }
    
    return false;
}


//#define ENABLE_CACHE
void SpheresManager::buildLivels(int rings,int sgments,int livels, float radius,double _dfPerLivel){
    
    DEBUG_ASSERT(livels>0);
    //calc factor
    dfPerLivel=_dfPerLivel;
    ringsFactor=(double)rings / (livels+_dfPerLivel) ;//(double)pow(rings, 1.0/(livels*dfPerLivel));//
    sgmentsFactor=(double)sgments / (livels+_dfPerLivel) ;//(double)pow(sgments, 1.0/(livels*dfPerLivel));//
    spheres.resize(livels);
    
    for (int l=1; l<=livels; ++l) {
        //sphere
        spheres[livels-l].rings=rings/pow(dfPerLivel,l-1);
        spheres[livels-l].sectors=sgments/pow(dfPerLivel,l-1);
        spheres[livels-l].radius=radius+0.0004*(livels-l);
    }
    //first livel must to be allocated
    root->idSphere=-1;
    root->sub={
        0,spheres[0].rings,
        0,spheres[0].sectors
    };
    root->isvirtual=false;
    for(Easy3D::uchar c=0;c!=8;++c){
        SphereMesh* tmp=new SphereMesh();
        subMeshADiv(tmp,c,0,root->sub);
        root->setChild(tmp,c);
        getChild(root,c)->isvirtual=false;
    }
    
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

bool SpheresManager::drawSub(SphereMesh *node,int countlivel){

    bool drawFather=false;

    if(countlivel==0){
        for(uchar c=0;c!=8;++c)
            if(camera->boxInFrustum( getChild(node,c)->box )){
                //to do: separate thread
				if(!getChild(node,c)->lockTask())
                    getChild(node,c)->buildMesh(*this,fractal);
                //draw
                drawFather=getChild(node,c)->draw()&&drawFather;
            }
    }
    else{
        for(uchar c=0;c!=8;++c)
            if(camera->boxInFrustum( getChild(node,c)->box )){
				if(!drawSub(getChild(node,c),countlivel-1)){
					//draw
					drawFather=getChild(node,c)->draw()&&drawFather;
				}
			}
    }
	//draw Father?
    return drawFather;
}
void SpheresManager::draw(){
    //update memory gpu
	virtualVBO.updateMemory();
	doBuildsList();
    //update memory cpu
    updateMemory();
    //draw
    drawSub(root,curLevel);
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




