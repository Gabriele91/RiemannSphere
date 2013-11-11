#include <stdafx.h>
#include <GeodesicMesh.h>

///////////////////////
using namespace Easy3D;
using namespace RiemannSphere;
///////////////////////

GeodesicMesh::GeodesicMesh()
:vbo(0)
,nbuffer(0)
,radius(1.0)
{
}
GeodesicMesh::~GeodesicMesh(){
	if( vbo )
        glDeleteBuffers(1,&vbo);
}

void GeodesicMesh::build(int divs,float _radius){
    //save radius
    radius=_radius;
    GLuint ntris=pow(4,divs);
    GLuint nvertices=ntris*3;
    nbuffer=20*(nvertices*3);
    //size buffer
    std::vector<GLfloat> vtx(nbuffer);
    //build points
    const float sqrt5 = sqrt (5.0f);
    const float phi = (1.0f + sqrt5) * 0.5f;
    // Circumscribed radius
    const float cRadius = sqrt (10.0f + (2.0f * sqrt5)) / (4.0f * phi);
    //Now we define constants which will define our Icosahedron's vertices
    double a = (radius / cRadius) * 0.5;
    double b = (radius / cRadius) / (2.0f * phi);
    // Vertices of the Icosahedron:
    Vec3 v1 ( 0,  b, -a);
    Vec3 v2 ( b,  a,  0);
    Vec3 v3 (-b,  a,  0);
    Vec3 v4 ( 0,  b,  a);
    Vec3 v5 ( 0, -b,  a);
    Vec3 v6 (-a,  0,  b);
    Vec3 v7 ( 0, -b, -a);
    Vec3 v8 ( a,  0, -b);
    Vec3 v9 ( a,  0,  b);
    Vec3 v10(-a,  0, -b);
    Vec3 v11( b, -a,  0);
    Vec3 v12(-b, -a,  0);
    
    GLfloat *vptr=&vtx[0];
    
#define setTri(a,b,c)\
subtriangles(vptr,divs,a,b,c);
    
    setTri(v1,v2,v3);
    setTri(v4,v3,v2);
    setTri(v4,v5,v6);
    setTri(v4,v9,v5);
    
    setTri(v1,v7,v8);
    setTri(v1,v10,v7);
    setTri(v5,v11,v12);
    setTri(v7,v12,v11);
    
    setTri(v3,v6,v10);
    setTri(v12,v10,v6);
    setTri(v2,v8,v9);
    setTri(v11,v9,v8);
    
    setTri(v4, v6, v3);
    setTri(v4, v2, v9);
    setTri(v1, v3, v10);
    setTri(v1, v8, v2);
    
    setTri(v7, v10, v12);
    setTri(v7, v11, v8);
    setTri(v5, v12, v6);
    setTri(v5, v9, v11);

    ////////////////////////////////////////////////////////////////////
    DEBUG_ASSERT( !vbo );
	//create the VBO
	glGenBuffers(1, &vbo );
	//send to gpu
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, nbuffer*sizeof(GLfloat), &vtx[0], GL_STATIC_DRAW);
    //nvertex
    nbuffer/=3; //3 float
    ////////////////////////////////////////////////////////////////////
    
}

DFORCEINLINE void GeodesicMesh::triangles(GLfloat*& v,
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
#define setVec(i,p)\
v[0+i*3]=p.x; v[1+i*3]=p.y; v[2+i*3]=p.z;
    
    setVec(0,p1)
    setVec(1,a)
    setVec(2,p2)
    
    setVec(3,p1)
    setVec(4,p3)
    setVec(5,b)
    
    setVec(6,p2)
    setVec(7,c)
    setVec(8,p3)
    
    setVec(9,p1)
    setVec(10,p2)
    setVec(11,p3)
    //jump
    v=&v[12*3];
}
DFORCEINLINE void GeodesicMesh::subtriangles(GLfloat*& v,
                                             int pass,
                                             const Easy3D::Vec3& a,
                                             const Easy3D::Vec3& b,
                                             const Easy3D::Vec3& c){
    if(pass==1){
        triangles(v,a,b,c);
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
        subtriangles(v,pass-1,p1,a,p2);
        subtriangles(v,pass-1,p1,p3,b);
        subtriangles(v,pass-1,p2,c,p3);
        subtriangles(v,pass-1,p1,p2,p3);
    }
}


void GeodesicMesh::draw(Easy3D::Render *render){
    //save state
    auto clientState=render->getClientState();
    auto matrixState=render->getMatrixsState();
    //only vertex
    render->setClientState(Render::ClientState::VERTEX);
    //set matrix
    auto newMatrixState=matrixState;
    newMatrixState.modelview=matrixState.modelview.mul(getGlobalMatrix());
    render->setMatrixsState(newMatrixState);
    //bind VBO
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    //set vertex
    glVertexPointer(3, GL_FLOAT, 0, (void*)0 );
    //draw call
    glDrawArrays( GL_TRIANGLES, 0, nbuffer);
    //set new state
    render->setMatrixsState(matrixState);
    render->setClientState(clientState);
}

void GeodesicMesh::draw(Easy3D::Render *render,const Easy3D::Color& color){
    //save state
    auto clientState=render->getClientState();
    auto colorState=render->getColorState();
    auto matrixState=render->getMatrixsState();
    //only vertex
    render->setClientState(Render::ClientState::VERTEX);
    //set matrix
    auto newMatrixState=matrixState;
    newMatrixState.modelview=matrixState.modelview.mul(getGlobalMatrix());
    render->setMatrixsState(newMatrixState);
    //set color
    render->setColorState(color);
    //bind VBO
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    //set vertex
    glVertexPointer(3, GL_FLOAT, 0, (void*)0 );
    //draw call
    glDrawArrays( GL_TRIANGLES, 0, nbuffer);
    //set new state
    render->setMatrixsState(matrixState);
    render->setColorState(colorState);
    render->setClientState(clientState);
}