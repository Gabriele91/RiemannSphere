#ifndef GEODESICMESH_H
#define GEODESICMESH_H

#include <Easy3D.h>

namespace RiemannSphere {
    
	class GeodesicMesh : public Easy3D::Object {
        
        GLuint vbo;
        int nbuffer;
        float radius;
        //utiliti to build
        void triangles(GLfloat*& v,
                       const Easy3D::Vec3& a,
                       const Easy3D::Vec3& b,
                       const Easy3D::Vec3& c);
        void subtriangles(GLfloat*& v,
                          int pass,
                          const Easy3D::Vec3& a,
                          const Easy3D::Vec3& b,
                          const Easy3D::Vec3& c);
	public:
        
		GeodesicMesh();
        ~GeodesicMesh();
        void build(int n,float radius);
        void draw(Easy3D::Render *render);
        void draw(Easy3D::Render *render,const Easy3D::Color& color);
        
	};

};

#endif