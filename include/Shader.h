#ifndef SHADER_H
#define SHADER_H

#include <Config.h>
#include <Math3D.h> 
#include <Utility.h> 
#include <Texture.h> 

namespace Easy3D{
	class Shader{

	public:
		//costruttore
		Shader();
		Shader(const Utility::Path& vs, 
			   const Utility::Path& fs,
			   const char *defines[]=NULL);
		//distruttore
		~Shader();
		//inizializza
		void loadShader(const Utility::Path& vs, 
						const Utility::Path& fs,
						const char *defines[]=NULL);		

		//uniform whit name
		void uniformInt(const char *name,int v);
		void uniformFloat(const char *name,float v);
		void uniformTexture(const char *name,const Texture& v);
		void uniformVector2D(const char *name,const Vector2D& v);
		void uniformVector3D(const char *name,const Vector3D& v);
		void uniformVector4D(const char *name,const Vector4D& v);
		void uniformMatrix4x4(const char *name,const Matrix4x4& v);
		void uniformFloatArray(const char *name,float *v,unsigned int n);
		void uniformVec2Array(const char *name,Vector2D  *v,unsigned int n);
		void uniformVec3Array(const char *name,Vector3D  *v,unsigned int n);
		void uniformVec4Array(const char *name,Vector4D  *v,unsigned int n);
		void uniformMat4Array(const char *name,Matrix4x4 *v,unsigned int n);
		//get uniform id
		uint getUniformID(const char *name);
		//uniform whit id		
		void uniformInt(uint id,int v);
		void uniformFloat(uint id,float v);
		void uniformTexture(uint id,const Texture& v);
		void uniformVector2D(uint id,const Vector2D& v);
		void uniformVector3D(uint id,const Vector3D& v);
		void uniformVector4D(uint id,const Vector4D& v);
		void uniformMatrix4x4(uint id,const Matrix4x4& v);
		void uniformFloatArray(uint id,float *v,unsigned int n);
		void uniformVec2Array(uint id,Vector2D  *v,unsigned int n);
		void uniformVec3Array(uint id,Vector3D  *v,unsigned int n);
		void uniformVec4Array(uint id,Vector4D  *v,unsigned int n);
		void uniformMat4Array(uint id,Matrix4x4 *v,unsigned int n);
		//imposta shader
		void bind();
		void unbind();
		//id programma
		uint programID();

	private:
		unsigned int texture_id[3];
		uint shader_id;
		uint shader_vs;
		uint shader_fs;
		bool deleteProgram;


	};
};
#endif
