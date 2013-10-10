#ifndef FRACTAL_H
#define FRACTAL_H

#include <Easy3D.h>
#include <Polynomial.h>

namespace RiemannSphere {

    class Fractal {

	public:
        
		struct Values{
			int idroot;
			float intensity;
			Values(int idroot=-1,float intensity=0.0f)
            :idroot(idroot),intensity(intensity){}
		};
        
        virtual RootColor<float> calcColor(float real,float img) const=0;

	};

};

#endif