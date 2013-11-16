#ifndef RIEMANNSCALE9_H
#define RIEMANNSCALE9_H

#include <Easy3D.h>

namespace RiemannGui {

	class RiemannScale9 {
        
        GLuint vbo;
        int nbuffer;
        Easy3D::Texture *texture;
        void bindVBO(const Easy3D::Vec2& mbox,
                     const Easy3D::Vec2& sbox,
                     const Easy3D::Vec2& pos);
        
	public:
        
		RiemannScale9():vbo(0),texture(NULL){};
		RiemannScale9(const Easy3D::Utility::Path& path,
                      const Easy3D::Vec2& mbox,
                      const Easy3D::Vec2& sbox,
                      const Easy3D::Vec2& pos);
        ~RiemannScale9();
		void build(const Easy3D::Utility::Path& path,
                   const Easy3D::Vec2& mbox,
                   const Easy3D::Vec2& sbox,
                   const Easy3D::Vec2& pos);
        void draw();
        

	};

};

#endif