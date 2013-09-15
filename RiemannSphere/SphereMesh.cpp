#include <stdafx.h>
#include <Sphere.h>
#include <SphereMesh.h>
#include <complex>

///////////////////////
using namespace RiemannSphere;
using namespace Easy3D;
///////////////////////

//  f(x)/f'(x)
//  a[] is list of coefficients, g number of coefficients
//  x is function's argument
template<class T> inline std::complex<T> fxOnDx(T *a,int g,const std::complex<T>& x){
   
    if(g<2) return 0;
   
    std::complex<T> vn=a[g-1];
    std::complex<T> wn=vn;
 
    for(int i=g-2;i>0;--i){
       vn = vn*x+a[i];
       wn = wn*x+vn;
    }
   
    vn = vn*x+a[0];
   
    return vn/wn;
   
}

//newton
template<class T> inline std::complex<T> newton(T *a,int g,const std::complex<T>& xk,int n){
	while(n--)
		xk=xk-fxOnDx(a,g,xk);
	//determinare dove cade:

}
 

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

void SphereMesh::buildMesh(){

    
    const int nring=sub.rEnd-sub.rStart;
    const int nsettors=sub.sEnd-sub.sStart;
    
    std::vector<GLfloat> vertices(nring*nsettors*3*6);
    int count=0;
    
    #define push(x,y,z)\
    vertices[count++]=(x)*sphere.radius;\
    vertices[count++]=(y)*sphere.radius;\
    vertices[count++]=(z)*sphere.radius;
    
    //center
    for(int i = sub.rStart; i<sub.rEnd; ++i){
        
        double lat0 = Math::PI * (-0.5 + (double) i / sphere.rings);
        double z0  =  std::sin(lat0);
        double zr0 =  std::cos(lat0);
        
        double lat1 = Math::PI * (-0.5 + (double) (i+1) / sphere.rings);
        double z1 =   std::sin(lat1);
        double zr1 =  std::cos(lat1);
        
        for(int j = sub.sStart; j < sub.sEnd; ++j) {
            double lng = 2 * Math::PI * (double) j / sphere.sectors;
            double x = std::cos(lng);
            double z = std::sin(lng);
            
            double lng2 = 2 * Math::PI * (double) (j+1) / sphere.sectors;
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * nring*nsettors*3*6, &vertices[0], GL_STATIC_DRAW);
    vertexBufferSize=vertices.size()/3;
}


void SphereMesh::draw() const{
    //bind VBO
	glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
	//set vertex
	glVertexPointer(3, GL_FLOAT, sizeof(GLfloat)*3, 0 );
    //draw call
    glDrawArrays( GL_TRIANGLES, 0, (GLuint)vertexBufferSize);
}
