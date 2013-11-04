#include <stdafx.h>
#include <GridMesh.h>

///////////////////////
using namespace Easy3D;
using namespace RiemannSphere;
///////////////////////

GridMesh::GridMesh()
              :vbo(0)
              ,nbuffer(0)
{
}
GridMesh::~GridMesh(){
	if( vbo )
        glDeleteBuffers(1,&vbo);
}

void GridMesh::build(int n,int m){
    
    float xpass=1.0f/n;
    float zpass=1.0f/m;

    std::vector<GLfloat> vtx(6*n+6*m+2*6);
    //x values
    for(float x=-0.5f;x<0.5f;x+=xpass){
        //x y z
        vtx[nbuffer++]=x;
        vtx[nbuffer++]=0.0;
        vtx[nbuffer++]=-0.5;
        //x y z
        vtx[nbuffer++]=x;
        vtx[nbuffer++]=0.0;
        vtx[nbuffer++]=0.5;
    }
    //x y z
    vtx[nbuffer++]=0.5;
    vtx[nbuffer++]=0.0;
    vtx[nbuffer++]=-0.5;
    //x y z
    vtx[nbuffer++]=0.5;
    vtx[nbuffer++]=0.0;
    vtx[nbuffer++]=0.5;
    //z values
    for(float z=-0.5f;z<=0.5f;z+=zpass){
        //x y z
        vtx[nbuffer++]=-0.5;
        vtx[nbuffer++]=0.0;
        vtx[nbuffer++]=z;
        //x y z
        vtx[nbuffer++]=0.5;
        vtx[nbuffer++]=0.0;
        vtx[nbuffer++]=z;
    }
    //x y z
    vtx[nbuffer++]=-0.5;
    vtx[nbuffer++]=0.0;
    vtx[nbuffer++]=0.5;
    //x y z
    vtx[nbuffer++]=0.5;
    vtx[nbuffer++]=0.0;
    vtx[nbuffer++]=0.5;
    ////////////////////////////////////////////////////////////////////
    DEBUG_ASSERT( !vbo );
	//create the VBO
	glGenBuffers(1, &vbo );
	//send to gpu
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, nbuffer*sizeof(GLfloat), &vtx[0], GL_STATIC_DRAW);
    //lines vertex
    nbuffer/=3;
    ////////////////////////////////////////////////////////////////////

}
void GridMesh::draw(Easy3D::Render *render,const Easy3D::Color& color){
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
    glDrawArrays( GL_LINES, 0, nbuffer);
    //set new state
    render->setMatrixsState(matrixState);
    render->setColorState(colorState);
    render->setClientState(clientState);
}