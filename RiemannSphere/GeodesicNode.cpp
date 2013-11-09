#include <stdafx.h>
#include <GeodesicNode.h>
#include <GeodesicSphere.h>

///////////////////////
using namespace Easy3D;
using namespace RiemannSphere;
///////////////////////

GeodesicNode::GeodesicNode()
:isvirtual(true)
,inbuilding(false)
,level(0)
,radius(0.0)
{
}
GeodesicNode::~GeodesicNode(){
//false
}


void GeodesicNode::sendToGpu(VirtualVBO* vVBO){
	if(isvirtual)
		vVBO->addNode(&nvbo);
	else
		nvbo.build();
	//end task
	inbuilding=false;
}
void GeodesicNode::freeCpuBuffers(){
	if(nvbo.getData()){
		nvbo.cleanInfo();
		//end task
		inbuilding=false;
	}
}

bool GeodesicNode::draw() {
	return nvbo.draw();
}



#define toComplexReal(v) ((float)(v.x/(radius-v.y)))
#define toComplexImag(v) ((float)(v.z/(radius-v.y)))

DFORCEINLINE void GeodesicNode::triangles(GLfloat*& v,
                                           const Fractal* fractal,
                                           const Easy3D::Vec3& a,
                                           const Easy3D::Vec3& b,
                                           const Easy3D::Vec3& c){

    //points
    Vec3 p1=(a+b)/2.0f;
    Vec3 p2=(a+c)/2.0f;
    Vec3 p3=(b+c)/2.0f;
    //normalize to sphere
    p1.normalize(); p1*=radius;
    p2.normalize(); p2*=radius;
    p3.normalize(); p3*=radius;
    //calc vetexs
    #define setVec(i,p,c)\
    v[0+i*6]=p.x; v[1+i*6]=p.y; v[2+i*6]=p.z;\
    v[3+i*6]=c.r; v[4+i*6]=c.g; v[5+i*6]=c.b;
    
    RootColor<float> color1=fractal->calcColor(toComplexReal(a),toComplexImag(a));
    setVec(0,p1,color1)
    setVec(1,a,color1)
    setVec(2,p2,color1)
    
    RootColor<float> color2=fractal->calcColor(toComplexReal(b),toComplexImag(b));
    setVec(3,p1,color2)
    setVec(4,p3,color2)
    setVec(5,b,color2)
    
    RootColor<float> color3=fractal->calcColor(toComplexReal(c),toComplexImag(c));
    setVec(6,p2,color3)
    setVec(7,c,color3)
    setVec(8,p3,color3)
    
    RootColor<float> color4=fractal->calcColor(toComplexReal(p3),toComplexImag(p3));
    setVec(9,p1,color4)
    setVec(10,p2,color4)
    setVec(11,p3,color4)
    //jump
    v=&v[12*6];
}
DFORCEINLINE void GeodesicNode::subtriangles(GLfloat*& v,
                                              const Fractal* fractal,
                                              int pass,
                                              const Easy3D::Vec3& a,
                                              const Easy3D::Vec3& b,
                                              const Easy3D::Vec3& c){
    if(pass==1){
        triangles(v,fractal,a,b,c);
    }
    else{
        //points
        Vec3 p1=(a+b)/2.0f;
        Vec3 p2=(a+c)/2.0f;
        Vec3 p3=(b+c)/2.0f;
        //normalize to sphere
        p1.normalize(); p1*=radius;
        p2.normalize(); p2*=radius;
        p3.normalize(); p3*=radius;
        //subtris
        subtriangles(v,fractal,pass-1,p1,a,p2);
        subtriangles(v,fractal,pass-1,p1,p3,b);
        subtriangles(v,fractal,pass-1,p2,c,p3);
        subtriangles(v,fractal,pass-1,p1,p2,p3);
    }
}

void GeodesicNode::buildMesh(GeodesicSphere& gsphere,
						     const Fractal* fractal){
	//asserts
	DEBUG_ASSERT(!nvbo.isAllocated());
	DEBUG_ASSERT(!inbuilding);
	//is inbuilding
	inbuilding=true;
	//gen buffers
	gsphere.addBuildTask([this,&gsphere,fractal](){
                              
                              if(!gsphere.camera->boxInFrustum( box )){
                                  inbuilding=false;
                                  return;
                              }
                              //cpu side
                              const int divs=6;
                              GLuint ntris=pow(4,divs);
                              GLuint nvertices=ntris*3;
                              nvbo.allocCpu(sizeof(GLfloat)*nvertices*6,nvertices);
                              //alloc
                              GLfloat *vertices=(GLfloat*)nvbo.getData();
                              DEBUG_ASSERT(vertices);
                              //
                              //calc
                              subtriangles(vertices,
                                           fractal,
                                           divs,
                                           tri.point[0],
                                           tri.point[1],
                                           tri.point[2]);
                              //send message
                              gsphere.addMeshToBuild(this);
                          });
}
void GeodesicNode::forceBuildMesh(GeodesicSphere& gsphere,
                                  const Fractal* fractal){
	//asserts
	DEBUG_ASSERT(!nvbo.isAllocated());
	DEBUG_ASSERT(!inbuilding);
	//is inbuilding
	inbuilding=true;
	//gen buffers
	gsphere.addBuildTask([this,&gsphere,fractal](){
        //cpu side
        const int divs=6;
        GLuint ntris=pow(4,divs);
        GLuint nvertices=ntris*3;
        nvbo.allocCpu(sizeof(GLfloat)*nvertices*6,nvertices);
        //alloc
        GLfloat *vertices=(GLfloat*)nvbo.getData();
        DEBUG_ASSERT(vertices);
        //
        //calc
        subtriangles(vertices,
                     fractal,
                     divs,
                     tri.point[0],
                     tri.point[1],
                     tri.point[2]);
        //send message
        gsphere.addMeshToBuild(this);
    });
}
