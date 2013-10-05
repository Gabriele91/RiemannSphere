#include <stdafx.h>
#include <Texture.h>
#include <Debug.h>
#include <Application.h>
#define IMAGE_LOADER_OPENGL
#include "Image/Image.h"

using namespace Easy3D;


Texture::Texture()
:rpath("")
,bBilinear(true)
,chBlr(true)
,bMipmaps(true)
,chMps(true)
,width(0)
,height(0)
,gpuid(0){
	//is not loaded
	loaded=false;
}
Texture::Texture(const String& pathfile)
:rpath(pathfile)
,bBilinear(true)
,chBlr(true)
,bMipmaps(true)
,chMps(true)
,width(0)
,height(0)
,gpuid(0){
	//is not loaded
	loaded=false;
    //load file
    load();
}

Texture::~Texture(){
	//release resource
	if(loaded) unload();
}

void Texture::bind(uint ntexture){
	//
	DEBUG_ASSERT(gpuid);
	//
    glActiveTexture( GL_TEXTURE0 + ntexture );
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, (GLuint)gpuid );
	//settings
#ifndef DISABLE_MIDMAP
	if(chBlr)
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,bBilinear?GL_LINEAR:GL_NEAREST);
	if(chMps)
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,bMipmaps?GL_LINEAR_MIPMAP_NEAREST:GL_LINEAR);
#else
	if(chBlr)
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	if(chMps)
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
#endif
}
//setting
bool Texture::bilinear(){
	return bBilinear;
}
bool Texture::bilinear(bool value){
	chBlr=bBilinear!=value;
	return bBilinear=value;
}
bool Texture::mipmaps(){
	return bMipmaps;
}
bool Texture::mipmaps(bool value){
	chMps=bMipmaps!=value;
	return bMipmaps=value;
}
//operators
bool Texture::operator ==(const Texture& t) const{
	return gpuid==t.gpuid;
}
bool Texture::operator !=(const Texture& t) const{
	return gpuid!=t.gpuid;
}

//load methods
bool Texture::load(const String& pathfile){
    DEBUG_ASSERT(!loaded);
    rpath=(pathfile);
	return load();
}
bool Texture::load(){
    DEBUG_ASSERT(!loaded);
	/////////////////////////////////////////////////////////////////////
	//cpu load
	//get raw file
	void *data=NULL; size_t len=0;
	Application::instance()->loadData(rpath,data,len);
	//load image
	Image image;
	image.loadFromData(data,len,Image::getTypeFromExtetion(rpath.getExtension()));
	//free raw file
	free(data);
	/////////////////////////////////////////////////////////////////////
	//gen gpu
	//create an GPU texture
	glGenTextures( 1, &gpuid );
	//build
	bind();
	//save width end height
	width=image.width;
	height=image.height;
	//resize
	GLuint typeInternal=image.type;
#ifndef OPENGL_ES
	GLuint type= image.type==GL_ALPHA8 ? GL_ALPHA : image.type;
#else	
	DEBUG_ASSERT_MSG(image.type!=GL_ALPHA8,"Texture: android not support alpha texture");
	GLuint type= image.type;
#endif
	//create a gpu texture
	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			typeInternal,
			width,
			height,
			0,
			type,
			GL_UNSIGNED_BYTE,
			NULL );
#ifndef DISABLE_MIDMAP
	//create mipmaps
	glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, bMipmaps );
#endif
	//send to GPU
    glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0,
					 width,
					 height,
					 type,
					 GL_UNSIGNED_BYTE,
					 image.bytes );
	
    CHECK_GPU_ERRORS();
	//is loaded
	loaded=true;
	//return
    return loaded;
}
bool Texture::unload(){
	//unload
	DEBUG_ASSERT(gpuid);
    glDeleteTextures(1, &gpuid );
	//reset values
    width = height = 0;
    gpuid = 0;
	//is not loaded
	loaded=false;
	//return
    return !loaded;
}
bool Texture::loadFromBinaryData(std::vector<uchar>& bytes,
								 uint argWidth,
								 uint argHeight,
								 uint format,
								 uint type){
	//error if olready loaded
	DEBUG_ASSERT(!loaded);
	/////////////////////////////////////////////////////////////////////
	//gen gpu
	//create an GPU texture
	glGenTextures( 1, &gpuid );
	//build
	bind();
	//save width end height
	width=argWidth;
	height=argHeight;
	//resize
	//create a gpu texture
	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			format,
			width,
			height,
			0,
			type,
			GL_UNSIGNED_BYTE,
			NULL );

#ifndef DISABLE_MIDMAP
	//create mipmaps
	glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, bMipmaps );
#endif

	//send to GPU
    glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0,
					 width,
					 height,
					 type,
					 GL_UNSIGNED_BYTE,
					 &bytes[0] );
	
    CHECK_GPU_ERRORS();
	//is loaded
	loaded=true;
	//return
    return loaded;

}
