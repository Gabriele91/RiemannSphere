#include <stdafx.h>
#include <Sphere.h>
#include <SphereMesh.h>

///////////////////////
using namespace RiemannSphere;
using namespace Easy3D;
///////////////////////

SphereMesh::SphereMesh(const Sphere& sphere)
:vertexBuffer(0)
,vertexBufferSize(0)
{
    buildMesh(sphere,
              0,
              0,
              sphere.getRings(),
              sphere.getSectors());
}

SphereMesh::SphereMesh(const Sphere& sphere,
                       int rStart,
                       int sStart,
                       int rEnd,
                       int sEnd)
:vertexBuffer(0)
,vertexBufferSize(0)
{
    buildMesh(sphere, rStart, sStart, rEnd, sEnd);
}



void SphereMesh::buildMesh(const Sphere& sphere,
               int rStart,
               int sStart,
               int rEnd,
               int sEnd){

    
    const int nring=rEnd-rStart;
    const int nsettors=sEnd-sStart;
    
    std::vector<GLfloat> vertices(nring*nsettors*3*6);
    int count=0;
    
    #define push(x,y,z)\
    vertices[count++]=(x)*sphere.getRadius();\
    vertices[count++]=(y)*sphere.getRadius();\
    vertices[count++]=(z)*sphere.getRadius();
    
    //center
    for(int i = rStart; i<rEnd; ++i){
        
        double lat0 = Math::PI * (-0.5 + (double) i / sphere.getRings());
        double z0  =  std::sin(lat0);
        double zr0 =  std::cos(lat0);
        
        double lat1 = Math::PI * (-0.5 + (double) (i+1) / sphere.getRings());
        double z1 =   std::sin(lat1);
        double zr1 =  std::cos(lat1);
        
        for(int j = sStart; j < sEnd; ++j) {
            double lng = 2 * Math::PI * (double) j / sphere.getSectors();
            double x = std::cos(lng);
            double z = std::sin(lng);
            
            double lng2 = 2 * Math::PI * (double) (j+1) / sphere.getSectors();
            double x2 = std::cos(lng2);
            double z2 = std::sin(lng2);
            
            //tri 1
            push( x * zr0, z0, z * zr0);
            push( x * zr1, z1, z * zr1);
            push(x2 * zr1, z1, z2 * zr1);
            //tri 2
            push(x2 * zr0, z0,z2 * zr0);
            push( x * zr0, z0,z * zr0);
            push(x2 * zr1, z1,z2 * zr1);
        }
    }
    //create the VBO
    if( !vertexBuffer )  glGenBuffers(1, &vertexBuffer );
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    vertexBufferSize=vertices.size()/3;
}


void SphereMesh::draw(){
    //bind VBO
	glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
	//set vertex
	glVertexPointer(3, GL_FLOAT, sizeof(GLfloat)*3, 0 );
    //draw call
    glDrawArrays( GL_TRIANGLES, 0, (GLuint)vertexBufferSize);
}
