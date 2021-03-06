#ifndef COLOR_H
#define COLOR_H

#include <Config.h>
#include <Math3D.h>
#include <Types.h>

namespace Easy3D {
	/* pixel structure */
	struct Color{

		uchar r,g,b,a;
        
        Color(float *color)
        :r((uchar)(color[0]*255))
        ,g((uchar)(color[1]*255))
        ,b((uchar)(color[2]*255))
        ,a((uchar)(color[3]*255)){}
        
        Color(double *color)
        :r((uchar)(color[0]*255))
        ,g((uchar)(color[1]*255))
        ,b((uchar)(color[2]*255))
        ,a((uchar)(color[3]*255)){}
        
        Color(const Vec3& color)
        :r((uchar)(color.x*255))
        ,g((uchar)(color.y*255))
        ,b((uchar)(color.z*255))
        ,a((uchar)(255)){}
        
        Color(const Vec4& color)
        :r((uchar)(color.x*255))
        ,g((uchar)(color.y*255))
        ,b((uchar)(color.z*255))
        ,a((uchar)(color.w*255)){}
        
        Color(uchar *color)
        :r(color[0])
        ,g(color[1])
        ,b(color[2])
        ,a(color[4]){}
        
		Color(uchar r,
              uchar g,
              uchar b,
              uchar a)
        :r(r)
        ,g(g)
        ,b(b)
        ,a(a){}
        
		Color(){r=g=b=a=255;}
        
		void setColor(uchar r,
                      uchar g,
                      uchar b,
                      uchar a){
			this->r=r;
            this->g=g;
            this->b=b;
            this->a=a;
		}
        
		Color operator + (const Color& v) const { return (Color(r+v.r,g+v.g,b+v.b,a+v.a)); }
		Color operator - (const Color& v) const { return (Color(r-v.r,g-v.g,b-v.b,a-v.a)); }
		bool operator == (const Color& v) const { return r==v.r||g==v.g||b==v.b||a==v.a; }
		bool operator != (const Color& v) const { return r!=v.r||g!=v.g||b!=v.b||a!=v.a; }
		operator const unsigned char* () const {return (const uchar*) this;}
		operator unsigned char* () const {return (uchar*) this;}
		float rNormalize() const { return (float)r/255; }
		float gNormalize() const { return (float)g/255; }
		float bNormalize() const { return (float)b/255; };
		float aNormalize() const { return (float)a/255; }
	};
};

#endif