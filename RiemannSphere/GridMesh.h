#ifndef GRIDMESH_H
#define GRIDMESH_H

#include <Easy3D.h>

namespace RiemannSphere {

	class GridMesh : public Easy3D::Object {
        
        GLuint vbo;
        int nbuffer;

	public:

		GridMesh();
        ~GridMesh();
        void build(int n,int m);
        void draw(Easy3D::Render *render,const Easy3D::Color& color);

	};

};

#endif