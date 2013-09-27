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


SphereMesh::SphereMesh()
:cpuVertexBuffer(NULL)
,cpuVertexBufferSize(0)
,vertexBuffer(0)
,vertexBufferSize(0)
,canDraw(false)
{
}
SphereMesh::~SphereMesh()
{
	//free cpu memory
	freeCpuBuffers();
	//free gpu memory
	freeGpuBuffers();
}
void SphereMesh::setMeshInfo(const Sphere& _sphere,
                             const SubSphere& _sub){
    sub=_sub;
    sphere=_sphere;
    box=sphere.genAABox(sub);
}


void SphereMesh::cpuBufferToGpu(){
	DEBUG_ASSERT(!canDraw);
	DEBUG_ASSERT(vertexBuffer);
	//send to gpu
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, cpuVertexBufferSize, cpuVertexBuffer, GL_STATIC_DRAW);
	//now can draw
	canDraw=true;
}
void SphereMesh::freeGpuBuffers(){
	if(vertexBuffer){
		glDeleteBuffers(1,&vertexBuffer);
		vertexBuffer=0;
		canDraw=false;
	}
}
void SphereMesh::freeCpuBuffers(){
	if(cpuVertexBuffer){
		free(cpuVertexBuffer);
		cpuVertexBuffer=NULL;
		cpuVertexBufferSize=0;
	}
}

#define pushv(v)\
vertices[count++]=v.x*sphere.radius;\
vertices[count++]=v.y*sphere.radius;\
vertices[count++]=v.z*sphere.radius;

#define pushRootColor(rcolor)\
vertices[count++]=(rcolor.r);\
vertices[count++]=(rcolor.g);\
vertices[count++]=(rcolor.b);

#define toComplex(v) std::complex<float>(v.x/(1.0-v.y),v.z/(1.0-v.y))

void SphereMesh::buildMesh(SpheresManager& smanager,const NewtonFractal<float>& newton){
	//asserts
	DEBUG_ASSERT(!cpuVertexBuffer);	
	DEBUG_ASSERT(!vertexBuffer);
	//create the VBO
	if( !vertexBuffer )  	
		glGenBuffers(1, &vertexBuffer );
	//gen buffers
	smanager.addBuildTask(
		[this,&smanager,newton](){

		const int nring=sub.rEnd-sub.rStart;
		const int nsettors=sub.sEnd-sub.sStart;
         //cpu side
		cpuVertexBufferSize=nring*nsettors*6*6*sizeof(GLfloat);
		cpuVertexBuffer=malloc(cpuVertexBufferSize);
		GLfloat *vertices=(GLfloat*)cpuVertexBuffer;
		int count=0;
		DEBUG_ASSERT(cpuVertexBuffer);	
		//gpu side
		vertexBufferSize=nring*nsettors*6;
		
		//center
		for(int i = sub.rStart; i<sub.rEnd; ++i){
        
			float lat0 = Math::PI * (-0.5 + (float) i / sphere.rings);
			float z0  =  std::sin(lat0);
			float zr0 =  std::cos(lat0);
        
			float lat1 = Math::PI * (-0.5 + (float) (i+1) / sphere.rings);
			float z1 =   std::sin(lat1);
			float zr1 =  std::cos(lat1);
        
			for(int j = sub.sStart; j < sub.sEnd; ++j) {
				float lng = 2 * Math::PI * (float) j / sphere.sectors;
				float x = std::cos(lng);
				float z = std::sin(lng);
            
				float lng2 = 2 * Math::PI * (float) (j+1) / sphere.sectors;
				float x2 = std::cos(lng2);
				float z2 = std::sin(lng2);
            
				Vec3 q1( x  * zr0, z0, z  * zr0 );
				Vec3 q2( x  * zr1, z1, z  * zr1 );
				Vec3 q3( x2 * zr1, z1, z2 * zr1 );
				Vec3 q4( x2 * zr0, z0, z2 * zr0 );
			
				auto color1=newton.calcColor(toComplex(q1));
				auto color2=newton.calcColor(toComplex(q1));
				auto color3=newton.calcColor(toComplex(q1));
				auto color4=newton.calcColor(toComplex(q1));

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
void SphereMesh::buildMesh(SpheresManager& smanager,const NewtonFractal<double>& newton){
	//asserts
	DEBUG_ASSERT(!cpuVertexBuffer);	
	DEBUG_ASSERT(!vertexBuffer);
	//create the VBO
	if( !vertexBuffer )  	
		glGenBuffers(1, &vertexBuffer );
	//gen buffers	
	smanager.addBuildTask(
		[this,&smanager,newton](){
			
		const int nring=sub.rEnd-sub.rStart;
		const int nsettors=sub.sEnd-sub.sStart;
         //cpu side
		cpuVertexBufferSize=nring*nsettors*6*6*sizeof(GLfloat);
		cpuVertexBuffer=malloc(cpuVertexBufferSize);
		GLfloat *vertices=(GLfloat*)cpuVertexBuffer;
		int count=0;
		DEBUG_ASSERT(cpuVertexBuffer);	
		//gpu side
		vertexBufferSize=nring*nsettors*6;

		//center
		for(int i = sub.rStart; i<sub.rEnd; ++i){
        
			float lat0 = Math::PI * (-0.5 + (float) i / sphere.rings);
			float z0  =  std::sin(lat0);
			float zr0 =  std::cos(lat0);
        
			float lat1 = Math::PI * (-0.5 + (float) (i+1) / sphere.rings);
			float z1 =   std::sin(lat1);
			float zr1 =  std::cos(lat1);
        
			for(int j = sub.sStart; j < sub.sEnd; ++j) {
				float lng = 2 * Math::PI * (float) j / sphere.sectors;
				float x = std::cos(lng);
				float z = std::sin(lng);
            
				float lng2 = 2 * Math::PI * (float) (j+1) / sphere.sectors;
				float x2 = std::cos(lng2);
				float z2 = std::sin(lng2);
            
				Vec3 q1( x  * zr0, z0, z  * zr0 );
				Vec3 q2( x  * zr1, z1, z  * zr1 );
				Vec3 q3( x2 * zr1, z1, z2 * zr1 );
				Vec3 q4( x2 * zr0, z0, z2 * zr0 );
			
				auto color1=newton.calcColor(toComplex(q1));
				auto color2=newton.calcColor(toComplex(q1));
				auto color3=newton.calcColor(toComplex(q1));
				auto color4=newton.calcColor(toComplex(q1));

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

bool SphereMesh::draw() {
	if( canDraw )  {
		//bind VBO
		glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
		//set vertex	
		glVertexPointer(3, GL_FLOAT, sizeof(GLfloat)*6, 0 );
		glColorPointer (3, GL_FLOAT, sizeof(GLfloat)*6, (void*)(sizeof(GLfloat)*3) );
		//draw call
		glDrawArrays( GL_TRIANGLES, 0, (GLuint)vertexBufferSize);
		//is drawed
		return true;
	}
	return false;
}
