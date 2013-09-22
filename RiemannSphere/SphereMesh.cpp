#include <stdafx.h>
#include <Sphere.h>
#include <SphereMesh.h>
#include <complex>

///////////////////////
using namespace RiemannSphere;
using namespace Easy3D;
///////////////////////


SphereMesh::SphereMesh()
:vertexBuffer(0)
,vertexBufferSize(0)
{
}


void SphereMesh::setMeshInfo(const Sphere& _sphere,
                             const SubSphere& _sub){
    sub=_sub;
    sphere=_sphere;
    box=sphere.genAABox(sub);
}

void SphereMesh::buildMesh(const NewtonFractal<float>& newton){
	
    
    const int nring=sub.rEnd-sub.rStart;
    const int nsettors=sub.sEnd-sub.sStart;
    
    std::vector<GLfloat> vertices(nring*nsettors*6*6);
    int count=0;
    
    #define push(x,y,z)\
    vertices[count++]=(x)*sphere.radius;\
    vertices[count++]=(y)*sphere.radius;\
    vertices[count++]=(z)*sphere.radius;

    #define pushv(v)\
    vertices[count++]=v.x*sphere.radius;\
    vertices[count++]=v.y*sphere.radius;\
    vertices[count++]=v.z*sphere.radius;

    #define pushColor(r,g,b)\
    vertices[count++]=(r);\
    vertices[count++]=(g);\
    vertices[count++]=(b);

    #define pushRootColor(rcolor)\
    vertices[count++]=(rcolor.r);\
    vertices[count++]=(rcolor.g);\
    vertices[count++]=(rcolor.b);
	
	#define toRC(xz,y) (xz/(this->sphere.radius-y))
	#define toN(x)	((x+1.0f)*0.5f)
	#define toRCV(xz,y) (toN(xz)/(1.0-toN(y)))
	#define toComplex(v) std::complex<float>(v.x/1.0-v.y,v.z/1.0-v.y)

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
    //create the VBO
    if( !vertexBuffer )  
		glGenBuffers(1, &vertexBuffer );

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    vertexBufferSize=vertices.size()/6;

}
void SphereMesh::buildMesh(const NewtonFractal<double>& newton){
	
    
    const int nring=sub.rEnd-sub.rStart;
    const int nsettors=sub.sEnd-sub.sStart;
    
    std::vector<GLfloat> vertices(nring*nsettors*6*6);
    int count=0;
    
    #define push(x,y,z)\
    vertices[count++]=(x)*sphere.radius;\
    vertices[count++]=(y)*sphere.radius;\
    vertices[count++]=(z)*sphere.radius;

    #define pushv(v)\
    vertices[count++]=v.x*sphere.radius;\
    vertices[count++]=v.y*sphere.radius;\
    vertices[count++]=v.z*sphere.radius;

    #define pushColor(r,g,b)\
    vertices[count++]=(r);\
    vertices[count++]=(g);\
    vertices[count++]=(b);

    #define pushRootColor(rcolor)\
    vertices[count++]=(rcolor.r);\
    vertices[count++]=(rcolor.g);\
    vertices[count++]=(rcolor.b);
	
	#define toRC(xz,y) (xz/(this->sphere.radius-y))
	#define toN(x)	((x+1.0f)*0.5f)
	#define toRCV(xz,y) (toN(xz)/(1.0-toN(y)))
	#define toComplex(v) std::complex<float>(v.x/1.0-v.y,v.z/1.0-v.y)

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
    //create the VBO
    if( !vertexBuffer )  
		glGenBuffers(1, &vertexBuffer );

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    vertexBufferSize=vertices.size()/6;

}

void SphereMesh::draw() const{
    //bind VBO
	glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
	//set vertex	
	glVertexPointer(3, GL_FLOAT, sizeof(GLfloat)*6, 0 );
	glColorPointer (3, GL_FLOAT, sizeof(GLfloat)*6, (void*)(sizeof(GLfloat)*3) );
    //draw call
    glDrawArrays( GL_TRIANGLES, 0, (GLuint)vertexBufferSize);
}
