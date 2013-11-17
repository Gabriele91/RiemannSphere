#include <stdafx.h>
#include <RiemannScale9.h>

///////////////////////
using namespace Easy3D;
using namespace RiemannGui;
///////////////////////

void RiemannScale9::bindVBO(const Easy3D::Vec2& mbox,
                            const Easy3D::Vec2& sbox,
                            const Easy3D::Vec2& pos){
    
    GLuint ntris=2*9;
    GLuint nvertices=ntris*3;
    nbuffer=nvertices;
    //size buffer
    std::vector<GLfloat> vtx(nvertices*4);
    //set pointers
    GLfloat *vptr=&vtx[0];
    #define vertex(py,px,u,v) (*vptr)=px+pos.x; ++vptr; (*vptr)=py+pos.y; ++vptr; (*vptr)=u; ++vptr; (*vptr)=v; ++vptr;
    #define toquad(p1,p2,p3,p4) p1 p2 p3 p2 p4 p3
    //first quad
    toquad(
           vertex(   0.0,   0.0,0.0 ,0.0 ),
           vertex(   0.0,sbox.x,0.0 ,0.25),
           vertex(sbox.y,   0.0,0.25,0.0 ),
           vertex(sbox.y,sbox.x,0.25,0.25)
           )
    //second quad
    toquad(
           vertex(   0.0,sbox.x       ,0.0 ,0.25 ),
           vertex(   0.0,sbox.x+mbox.x,0.0 ,0.75 ),
           vertex(sbox.y,sbox.x       ,0.25,0.25 ),
           vertex(sbox.y,sbox.x+mbox.x,0.25,0.75 )
           )
    //treii quad
    toquad(
           vertex(   0.0,sbox.x  +mbox.x,0.0 ,0.75 ),
           vertex(   0.0,sbox.x*2+mbox.x,0.0 ,1.00 ),
           vertex(sbox.y,sbox.x  +mbox.x,0.25,0.75 ),
           vertex(sbox.y,sbox.x*2+mbox.x,0.25,1.00 )
           )
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    //first quad
    toquad(
           vertex(sbox.y,          0.0,0.25,0.0 ),
           vertex(sbox.y,       sbox.x,0.25,0.25),
           vertex(sbox.y+mbox.y,   0.0,0.75,0.0 ),
           vertex(sbox.y+mbox.y,sbox.x,0.75,0.25)
           )
    //second quad
    toquad(
           vertex(sbox.y,       sbox.x       ,0.25,0.25 ),
           vertex(sbox.y,       sbox.x+mbox.x,0.25,0.75 ),
           vertex(sbox.y+mbox.y,sbox.x       ,0.75,0.25 ),
           vertex(sbox.y+mbox.y,sbox.x+mbox.x,0.75,0.75 )
           )
    
    //treii quad
    toquad(
           vertex(sbox.y       ,sbox.x  +mbox.x,0.25,0.75 ),
           vertex(sbox.y       ,sbox.x*2+mbox.x,0.25,1.00 ),
           vertex(sbox.y+mbox.y,sbox.x  +mbox.x,0.75,0.75 ),
           vertex(sbox.y+mbox.y,sbox.x*2+mbox.x,0.75,1.00 )
           )
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    //first quad
    toquad(
           vertex(sbox.y+mbox.y  ,   0.0,0.75,0.0 ),
           vertex(sbox.y+mbox.y  ,sbox.x,0.75,0.25),
           vertex(sbox.y*2+mbox.y,   0.0,1.00,0.0 ),
           vertex(sbox.y*2+mbox.y,sbox.x,1.00,0.25)
           )
    //second quad
    toquad(
           vertex(sbox.y+mbox.y  ,sbox.x       ,0.75,0.25 ),
           vertex(sbox.y+mbox.y  ,sbox.x+mbox.x,0.75,0.75 ),
           vertex(sbox.y*2+mbox.y,sbox.x       ,1.00,0.25 ),
           vertex(sbox.y*2+mbox.y,sbox.x+mbox.x,1.00,0.75 )
           )
    
    //treii quad
    toquad(
           vertex(sbox.y+mbox.y  ,sbox.x  +mbox.x,0.75,0.75 ),
           vertex(sbox.y+mbox.y  ,sbox.x*2+mbox.x,0.75,1.00 ),
           vertex(sbox.y*2+mbox.y,sbox.x  +mbox.x,1.00,0.75 ),
           vertex(sbox.y*2+mbox.y,sbox.x*2+mbox.x,1.00,1.00 )
           )
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    
    
    ////////////////////////////////////////////////////////////////////
    DEBUG_ASSERT( !vbo );
	//create the VBO
	glGenBuffers(1, &vbo );
	//send to gpu
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vtx.size()*sizeof(GLfloat), &vtx[0], GL_STATIC_DRAW);
    //lines vertex
    ////////////////////////////////////////////////////////////////////
}
RiemannScale9::RiemannScale9(const Easy3D::Utility::Path& path,
                             const Easy3D::Vec2& mbox,
                             const Easy3D::Vec2& sbox,
                             const Easy3D::Vec2& pos){
    //load texture
    texture=new Texture(path);
    //bind vbo
    bindVBO(mbox,sbox,pos);
}
void RiemannScale9::build(const Easy3D::Utility::Path& path,
                          const Easy3D::Vec2& mbox,
                          const Easy3D::Vec2& sbox,
                          const Easy3D::Vec2& pos){
    //load texture
    texture=new Texture(path);
    //bind vbo
    bindVBO(mbox,sbox,pos);
}

RiemannScale9::~RiemannScale9(){
    if(vbo)
        glDeleteBuffers(1, &vbo);
    if(texture)
        delete texture;
}
void RiemannScale9::draw(){
    //enable txture
    texture->bind();
    //bind VBO
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    //set vertex
    glVertexPointer(2, GL_FLOAT, sizeof(GLfloat)*4, (void*)(0) );
    glTexCoordPointer(2, GL_FLOAT, sizeof(GLfloat)*4,  (GLvoid*)(sizeof(float)*2) );
    //draw call
    glDrawArrays( GL_TRIANGLES, 0, nbuffer);
    
}