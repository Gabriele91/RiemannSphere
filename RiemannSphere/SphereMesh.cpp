#include <stdafx.h>
#include <Sphere.h>
#include <SphereMesh.h>
#include <SpheresManager.h>
#include <complex>

///////////////////////
using namespace RiemannSphere;
using namespace Easy3D;
///////////////////////
///////////////////////


SphereMesh::SphereMesh(bool avirtualVBO)
	:isvirtual(avirtualVBO)
	,inbuilding(false)
{
}
SphereMesh::~SphereMesh()
{
	//DEBUG_ASSERT(inbuilding==false);
}
void SphereMesh::setMeshInfo(Easy3D::ushort _idSphere,
                             const SubSphere& _sub,
                             const SpheresManager& smanager){
    sub=_sub;
    idSphere=_idSphere;
    box=smanager.getSphere(idSphere).genAABox(sub);
}


void SphereMesh::sendToGpu(VirtualVBO* vVBO){
	if(isvirtual)
		vVBO->addNode(&nvbo);
	else
		nvbo.build();
	//end task
	inbuilding=false;
}
void SphereMesh::freeCpuBuffers(){
	if(nvbo.getData()){
		nvbo.cleanInfo();
		//end task
		inbuilding=false;
	}
}

bool SphereMesh::draw() {
	return nvbo.draw();
}


#define pushv(v)\
vertices[count++]=v.x*smanager.getSphere(idSphere).radius;\
vertices[count++]=v.y*smanager.getSphere(idSphere).radius;\
vertices[count++]=v.z*smanager.getSphere(idSphere).radius;

#define pushRootColor(rcolor)\
vertices[count++]=(rcolor.r);\
vertices[count++]=(rcolor.g);\
vertices[count++]=(rcolor.b);

#define toComplexReal(v) ((float)(v.x/(1.0-v.y)))
#define toComplexImg(v) ((float)(v.z/(1.0-v.y)))


void SphereMesh::buildMesh(SpheresManager& smanager,
						   const Fractal* fractal){
	//asserts
	DEBUG_ASSERT(!nvbo.isAllocated());
	DEBUG_ASSERT(!inbuilding);
	//is inbuilding
	inbuilding=true;
	//gen buffers
	smanager.addBuildTask(
                          [this,&smanager,fractal](){
                              
                              if(!smanager.getCamera()->boxInFrustum( box )){
                                  inbuilding=false;
                                  return;
                              }
                              
                              const int nring=sub.rEnd-sub.rStart;
                              const int nsettors=sub.sEnd-sub.sStart;
                              //cpu side
                              GLuint nvertices=nring*nsettors*6;
                              nvbo.allocCpu(nvertices*6*sizeof(GLfloat),nvertices);
                              GLfloat *vertices=(GLfloat*)nvbo.getData();
                              DEBUG_ASSERT(vertices);
                              int count=0;
                              //var dec
                              //first for
                              float lat0,z0,zr0;
                              float lat1,z1,zr1;
                              //second for
                              float lng,x,z;
                              float lng2,x2,z2;
                              //for index
                              int i,j;
                              //center
                              for( i = sub.rStart; i<sub.rEnd; ++i){
                                  
                                   lat0 = Math::PI * (-0.5f + (float) i / smanager.getSphere(idSphere).rings);
                                   z0  =  std::sin(lat0);
                                   zr0 =  std::cos(lat0);
                                  
                                   lat1 = Math::PI * (-0.5f + (float) (i+1) / smanager.getSphere(idSphere).rings);
                                   z1 =   std::sin(lat1);
                                   zr1 =  std::cos(lat1);
                                  
                                  for( j = sub.sStart; j < sub.sEnd; ++j) {
                                       lng = 2.0f * Math::PI * (float) j / smanager.getSphere(idSphere).sectors;
                                       x = std::cos(lng);
                                       z = std::sin(lng);
                                      
                                       lng2 = 2.0f * Math::PI * (float) (j+1) / smanager.getSphere(idSphere).sectors;
                                       x2 = std::cos(lng2);
                                       z2 = std::sin(lng2);
                                      
                                      Vec3 q1( x  * zr0, z0, z  * zr0 );
                                      Vec3 q2( x  * zr1, z1, z  * zr1 );
                                      Vec3 q3( x2 * zr1, z1, z2 * zr1 );
                                      Vec3 q4( x2 * zr0, z0, z2 * zr0 );
                                      
#if 0
                                      auto color1=fractal->calcColor(toComplexReal(q1),toComplexImg(q1));
                                      auto color2=fractal->calcColor(toComplexReal(q2),toComplexImg(q2));
                                      auto color3=fractal->calcColor(toComplexReal(q3),toComplexImg(q3));
                                      auto color4=fractal->calcColor(toComplexReal(q4),toComplexImg(q4));
                                      //tri 1
                                      pushv(q1);
                                      pushRootColor(color1);
                                      
                                      pushv(q2);
                                      pushRootColor(color2);
                                      
                                      pushv(q3);
                                      pushRootColor(color3);
                                      
                                      //tri 2
                                      pushv(q4);
                                      pushRootColor(color4);
                                      
                                      pushv(q1);
                                      pushRootColor(color1);
                                      
                                      pushv(q3);
                                      pushRootColor(color3);
#else
                                      auto color1=fractal->calcColor(toComplexReal(q1),toComplexImg(q1));
                                      //tri 1
                                      pushv(q1);  
                                      pushRootColor(color1);
                                      
                                      pushv(q2);  
                                      pushRootColor(color1);
                                      
                                      pushv(q3); 
                                      pushRootColor(color1);
                                      
                                      //tri 2
                                      pushv(q4); 
                                      pushRootColor(color1);
                                      
                                      pushv(q1);  
                                      pushRootColor(color1);
                                      
                                      pushv(q3); 
                                      pushRootColor(color1);
#endif
                                      
                                  }
                              }
                              //send message
                              smanager.addMeshToBuild(this);
                          });
}
#undef pushv
#undef pushRootColor
#undef toComplex
