#include <stdafx.h>
#include <Shader.h>
#include <Application.h>
#include <Debug.h>
////////////////////////
using namespace Easy3D;
////////////////////////

//file da leggere
DFORCEINLINE static String textFileRead(const Utility::Path &path) {
	String out;
	/////////////////////////////////////////////////////////////////////
	//cpu load
	//get raw file
	void *data=NULL; size_t len=0;
	Application::instance()->loadData(path,data,len);
	out=(char*)data;
	//free raw file
	free(data);
	/////////////////////////////////////////////////////////////////////
	return out;
}
DFORCEINLINE static bool logError(unsigned int shader,int status) {

			if(!status)
			{
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
				if(infoLen > 1)
				{
					char* infoLog =(char*) malloc(sizeof(char) * infoLen);
					glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
					DEBUG_MESSAGE("Error compiling shader:\n" << infoLog);
					free(infoLog);
				}
				return false;
			}
			return true;

}
//costruttore
Shader::Shader(){
	shader_fs=shader_vs=shader_id=0;
}
Shader::Shader(const Utility::Path& vs,
			   const Utility::Path& fs,
			   const char *list_define[]){
	shader_fs=shader_vs=shader_id=0;
    loadShader(vs,fs,list_define);
}
//inizializza
void Shader::loadShader(const Utility::Path& vsFile, const Utility::Path& fsFile, const char *list_define[]){
	//delete last shader
	this->~Shader();
	shader_fs=shader_vs=shader_id=0;
	GLint compiled=0,linked=0;
	// load shaders files
	String fileVS=textFileRead(vsFile);
	String fileFS=textFileRead(fsFile);
	//list define
	String defines_string;
	for(;list_define&&(*list_define);++list_define)
		defines_string=(defines_string+"#define ")+(*list_define)+"\n";
	//precision highp or mediump (or lowp) float
	fileVS=//"#version 120\n"+
            defines_string+
			"#define saturate(x) clamp( x, 0.0, 1.0 )       \n"+
			"#define lerp        mix                        \n"+
			"#line 0\n"+
			fileVS;
	fileFS=//"#version 120\n"+
            defines_string+
			"#define saturate(x) clamp( x, 0.0, 1.0 )       \n"+
			"#define lerp        mix                        \n"+
			"#define _bestp\n"+
			"#define _highp\n"+
			"#define _mediump\n"+
			"#define _lowp\n"+
			"#line 0\n"+
			fileFS;
	//create a pixel shader
	shader_fs = glCreateShader(GL_FRAGMENT_SHADER);
	//create a vertex shader
	shader_vs = glCreateShader(GL_VERTEX_SHADER);
	//send source
	const char* cVSfile=fileVS;
	const char* cFSfile=fileFS;
	glShaderSource(shader_vs, 1,&(cVSfile), 0);
	glShaderSource(shader_fs, 1,&(cFSfile), 0);
	//compiling
	compiled=0;
	glCompileShader(shader_vs);
	glGetShaderiv(shader_vs, GL_COMPILE_STATUS, &compiled);
	if(!logError(shader_vs,compiled)){	glDeleteShader(shader_vs);	}

	compiled=0;
	glCompileShader(shader_fs);
	glGetShaderiv(shader_fs, GL_COMPILE_STATUS, &compiled);
	if(!logError(shader_fs,compiled)){ glDeleteShader(shader_fs); }
	//made a shader program
	shader_id = glCreateProgram();
	glAttachShader(shader_id, shader_vs);
	glAttachShader(shader_id, shader_fs);
	glLinkProgram(shader_id);
	//get link status
	glGetProgramiv(shader_id, GL_LINK_STATUS, &linked);
	//out errors
	if(!linked){
			GLint infoLen = 0;
			glGetProgramiv(shader_id, GL_INFO_LOG_LENGTH, &infoLen);
			if(infoLen > 1){
				char* infoLog =(char*) malloc(sizeof(char) * infoLen);
				glGetProgramInfoLog(shader_id, infoLen, NULL, infoLog);
				DEBUG_MESSAGE("Error linking program:\n" << infoLog);
				free(infoLog);
			}
			DEBUG_MESSAGE("Error linking\n" );
            //"stacca" gli schader dal shader program
            glDetachShader(shader_id, shader_fs);
            glDetachShader(shader_id, shader_vs);
            //cancella gli shader
            glDeleteShader(shader_fs);
            glDeleteShader(shader_vs);
            //cancella lo shader program
            glDeleteProgram(shader_id);
            shader_vs=shader_fs=shader_id=0;
	}

}
//distruttore
Shader::~Shader() {
	if( shader_id ){
		glUseProgram(0);
		//"stacca" gli schader dal shader program
		glDetachShader(shader_id, shader_fs);
		glDetachShader(shader_id, shader_vs);
		//cancella gli shader
		glDeleteShader(shader_fs);
		glDeleteShader(shader_vs);
		//cancella lo shader program
		glDeleteProgram(shader_id);
	}

}
//imposta shader
void Shader::bind(){
    glUseProgram(shader_id);
}
/////////////////////////////////
void Shader::uniformInt(const char *name,int v){ glUniform1i(glGetUniformLocation(shader_id, name), v); }
void Shader::uniformFloat(const char *name,float v){ glUniform1f(glGetUniformLocation(shader_id, name), v); }
void Shader::uniformTexture(const char *name,const Texture& v){ glUniform1i(glGetUniformLocation(shader_id, name), v.getGpuID()); }
void Shader::uniformVector2D(const char *name,const Vector2D& v){ glUniform2fv(glGetUniformLocation(shader_id, name), 1,&v.x); }
void Shader::uniformVector3D(const char *name,const Vector3D& v){ glUniform3fv(glGetUniformLocation(shader_id, name), 1,&v.x); }
void Shader::uniformVector4D(const char *name,const Vector4D& v){ glUniform4fv(glGetUniformLocation(shader_id, name), 1,&v.x); }
void Shader::uniformMatrix4x4(const char *name,const Matrix4x4& v) { glUniformMatrix4fv(glGetUniformLocation(shader_id, name),1, false,v.entries); }

void Shader::uniformFloatArray(const char *name,float *v,unsigned int n){ glUniform1fv(glGetUniformLocation(shader_id, name), n,v); }
void Shader::uniformVec2Array(const char *name,Vector2D *v,unsigned int n){ glUniform2fv(glGetUniformLocation(shader_id, name), n,&v[0].x); }
void Shader::uniformVec3Array(const char *name,Vector3D *v,unsigned int n){ glUniform3fv(glGetUniformLocation(shader_id, name), n,&v[0].x); }
void Shader::uniformVec4Array(const char *name,Vector4D *v,unsigned int n){ glUniform4fv(glGetUniformLocation(shader_id, name), n,&v[0].x); }
void Shader::uniformMat4Array(const char *name,Matrix4x4 *v,unsigned int n) { glUniformMatrix4fv(glGetUniformLocation(shader_id, name),n, false,v->entries); }
///////////////////////////////////
//get uniform id
uint Shader::getUniformID(const char *name){ return glGetUniformLocation(shader_id, name); }
//uniform whit id
void Shader::uniformInt(uint id,int v){ glUniform1i(id, v); }
void Shader::uniformFloat(uint id,float v){ glUniform1f(id, v); }
void Shader::uniformTexture(uint id,const Texture& v){ glUniform1i(id, v.getGpuID()); }
void Shader::uniformVector2D(uint id,const Vector2D& v){ glUniform2fv(id, 1,&v.x); }
void Shader::uniformVector3D(uint id,const Vector3D& v){ glUniform3fv(id, 1,&v.x); }
void Shader::uniformVector4D(uint id,const Vector4D& v){ glUniform4fv(id, 1,&v.x); }
void Shader::uniformMatrix4x4(uint id,const Matrix4x4& v){ glUniformMatrix4fv(id,1, false,v.entries); }

void Shader::uniformFloatArray(uint id,float *v,unsigned int n){ glUniform1fv(id, n,v); }
void Shader::uniformVec2Array(uint id,Vector2D  *v,unsigned int n){ glUniform2fv(id, n,&v[0].x); }
void Shader::uniformVec3Array(uint id,Vector3D  *v,unsigned int n){ glUniform3fv(id, n,&v[0].x); }
void Shader::uniformVec4Array(uint id,Vector4D  *v,unsigned int n){ glUniform2fv(id, n,&v[0].x); }
void Shader::uniformMat4Array(uint id,Matrix4x4 *v,unsigned int n) { glUniformMatrix4fv(id,n, false,v->entries); }

//disabilita shader
void Shader::unbind(){
    glUseProgram(0);
}
//id programma
unsigned int Shader::programID(){
	return shader_id;
}
