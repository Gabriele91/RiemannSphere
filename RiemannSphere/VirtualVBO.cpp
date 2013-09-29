#include <stdafx.h>
#include <VirtualVBO.h>

///////////////////////
using namespace Easy3D;
using namespace RiemannSphere;
///////////////////////
//pool vbo
VirtualVBO::VirtualVBO(size_t maxSize):first(NULL)
									  ,last(NULL)
									  ,gSize(0)
									  ,maxSize(maxSize)
									  ,cNode(0){}
VirtualVBO::~VirtualVBO(){
	while(first){
		glDeleteBuffers(1,&first->vbo);
		first->vbo=0;
		first->size=0;
		first=first->next;
		--cNode;
	}
	first=last=NULL;
}
void VirtualVBO::addNode(Node* in){
	//first added
	if(!last) last=in;
	//to the top
	__nodeToTop(in);
	//build vbo
	__buildVBO(in);
	//count a node
	++cNode;
}
void VirtualVBO::updateNode(Node* in){
	__nodeToTop(in);
}
void VirtualVBO::deleteLast(){
	if(last){
		//update count node
		--cNode;
		//delete vbo
		__deleteVBO(last);
		//fix pointers
		__dtLastNode();
	}
}
void VirtualVBO::updateMemory(){
	while(gSize>maxSize)deleteLast();
}
//vbo node
VirtualVBO::Node::Node():prev(NULL)
						,next(NULL)
						,vbo(0)
						,cVertices(0)
						,size(0)
						,data(NULL){}
VirtualVBO::Node::~Node(){
	if(data)
		free(data);
}
//cpu side
void  VirtualVBO::Node::allocCpu(size_t size,GLuint vertexCount){
	this->data=malloc(size);
	this->size=size;
	this->cVertices=vertexCount;
}
void* VirtualVBO::Node::getData(){
	return data;
}
void  VirtualVBO::Node::cleanInfo(){
	if(data) free(data);
	size=0;
	cVertices=0;
}
//gpu side
bool  VirtualVBO::Node::isAllocated(){
	return vbo!=0;
}
bool  VirtualVBO::Node::draw(){
	if( isAllocated() )  {
		//bind VBO
		glBindBuffer( GL_ARRAY_BUFFER, vbo );
		//set vertex	
		glVertexPointer(3, GL_FLOAT, sizeof(GLfloat)*6, 0 );
		glColorPointer (3, GL_FLOAT, sizeof(GLfloat)*6, (void*)(sizeof(GLfloat)*3) );
		//draw call
		glDrawArrays( GL_TRIANGLES, 0, cVertices);//nring*nsettors*6
		//is drawed
		return true;
	}
	return false;
}
void  VirtualVBO::Node::build(bool freedata){
	DEBUG_ASSERT( size && data )
	//create the VBO
	glGenBuffers(1, &vbo );
	//send to gpu
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	//free data
	if(freedata){
		free(data);
		data=NULL;
	}
}
void  VirtualVBO::Node::unbuild(){
	DEBUG_ASSERT( vbo )
	//dealloc vbo
	glDeleteBuffers(1,&vbo);
	//update node
	vbo=0;
	size=0;
}