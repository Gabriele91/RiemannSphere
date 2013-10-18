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
        
        template <class T>
        void horner(const std::complex<T>& x,
                    const std::vector< std::complex<T> >& a,
                    std::vector< std::complex<T> >& d){
            
            size_t n=a.size();
            
            for(size_t i=0;i!=n;++i) d[i]=a[0];
            
            for(size_t i=1;i!=n;++i){
                d[0]=d[0]*x+a[i];
                for(int j=1;j<std::min(n-i,d.size());++j){
                    d[j]=d[j]*x+d[j-1];
                }
            }
        }
	};

};

#endif