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
                              //center
                              for(int i = sub.rStart; i<sub.rEnd; ++i){
                                  
                                  float lat0 = Math::PI * (-0.5f + (float) i / smanager.getSphere(idSphere).rings);
                                  float z0  =  std::sin(lat0);
                                  float zr0 =  std::cos(lat0);
                                  
                                  float lat1 = Math::PI * (-0.5f + (float) (i+1) / smanager.getSphere(idSphere).rings);
                                  float z1 =   std::sin(lat1);
                                  float zr1 =  std::cos(lat1);
                                  
                                  for(int j = sub.sStart; j < sub.sEnd; ++j) {
                                      float lng = 2.0f * Math::PI * (float) j / smanager.getSphere(idSphere).sectors;
                                      float x = std::cos(lng);
                                      float z = std::sin(lng);
                                      
                                      float lng2 = 2.0f * Math::PI * (float) (j+1) / smanager.getSphere(idSphere).sectors;
                                      float x2 = std::cos(lng2);
                                      float z2 = std::sin(lng2);
                                      
                                      Vec3 q1( x  * zr0, z0, z  * zr0 );
                                      Vec3 q2( x  * zr1, z1, z  * zr1 );
                                      Vec3 q3( x2 * zr1, z1, z2 * zr1 );
                                      Vec3 q4( x2 * zr0, z0, z2 * zr0 );
                                      
#if 0
                                      auto color1=fractal->calcColor(toComplexReal(q1),toComplexImg(q1));
                                      auto color2=fractal->calcColor(toComplexReal(q2),toComplexImg(q2));
                                      auto color3=fractal->calcColor(toComplexReal(q3),toComplexImg(q3));
                                      auto color4=fractal->calcColor(toComplexReal(q4),toComplexImg(q4));
#else
                                      auto color1=fractal->calcColor(toComplexReal(q1),toComplexImg(q1));
                                      auto color2=color1,color3=color1,color4=color1;
#endif
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
                                      
                                  }
                              }
                              //send message
                              smanager.addMeshToBuild(this);
                          });
}
#undef pushv
#undef pushRootColor
#undef toComplex
