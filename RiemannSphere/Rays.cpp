#include <stdafx.h>
#include <Rays.h>

///////////////////////
using namespace Easy3D;
using namespace RiemannSphere;
///////////////////////

///////////////////////
void Ray::draw(Render* render,float leng){
	//VAO DRAW
	Vec3 vertices[2]={point,point+dir*leng};
	Vec3 color[2]={Vec3(0.0,1.0,0.0),Vec3(0.0,1.0,0.0)};
    //save state
    Render::ClientState cstate=render->getClientState();
    //set enable VERTEX state and COLOR state
    render->setClientState(
    Render::ClientState(Render::ClientState::VERTEX|Render::ClientState::COLOR)
    );
    //draw
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	glVertexPointer(3,  GL_FLOAT,sizeof(Vec3),&vertices[0]);
	glColorPointer (3,  GL_FLOAT,sizeof(Vec3),&color[0]);
	glDrawArrays(GL_LINES, 0, 2);
    //reset state
    render->setClientState(cstate);
	//END DRAW
}
///////////////////////////
void Segment::draw(Render* render){
	//VAO DRAW
	Vec3 vertices[2]={t[0],t[1]};
	Vec3 color[2]={Vec3(0.0,0.0,0.0),Vec3(1.0,0.0,0.0)};
    //save state
    Render::ClientState cstate=render->getClientState();
    //set enable VERTEX state and COLOR state
    render->setClientState(
    Render::ClientState(Render::ClientState::VERTEX|Render::ClientState::COLOR)
    );
    //draw
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	glVertexPointer(3,  GL_FLOAT,sizeof(Vec3),&vertices[0]);
	glColorPointer (3,  GL_FLOAT,sizeof(Vec3),&color[0]);
	glDrawArrays(GL_LINES, 0, 2);
    //reset state
    render->setClientState(cstate);
	//END DRAW
}