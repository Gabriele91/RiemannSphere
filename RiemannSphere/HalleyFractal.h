#ifndef HALLEYFRACTAL_H
#define HALLEYFRACTAL_H

#include <Easy3D.h>
#include <Easy3D.h>
#include <Fractal.h>
#include <Polynomial.h>

namespace RiemannSphere {
    
	template <class T>
	class HalleyFractal : public Fractal{
    
		//function pointer
		Polynomial<T>* fun;
        
		//distance
		//distance
		DFORCEINLINE bool complexDist(const std::complex<T>& a,
									  const std::complex<T>& b,
									  T e) const{
            return std::abs(b-a)<e;
		}
        
		//  ( f(x)*f'(x) ) / ( (f'(x)^2) - ( (f''(x)^2)/4 ) )
		//  x function argument
		DFORCEINLINE std::complex<T> horner(const std::complex<T>& x) const{
            
            //exit condiction
			if(fun->constants.size()==0) return 0;
			if(fun->constants.size()==1) return fun->constants[0];
            
            std::complex<T> vn=fun->constants[0];
            std::complex<T> wn=vn;
            std::complex<T> un=wn;
            
            //Horner
            for(int i=1;i<(int)(fun->constants.size())-2;++i){
                vn = vn*x+fun->constants[i];
                wn = wn*x+vn;
                un = un*x+wn;
            }
            
            vn = vn*x+fun->constants[fun->constants.size()-2];
            wn = wn*x+vn;
            vn = vn*x+fun->constants[fun->constants.size()-1];
            
            auto s=vn*wn;
            auto d=(wn*wn)-vn*un;
            return s/d;
            
		}
        
		// xk+1=xk-f(x)/f'(x)
		// e=error max
		// n=max iteration
		DFORCEINLINE std::complex<T> halley(const std::complex<T>& x,T e,int& n) const{
			//var dec
			std::complex<T> kx1;
			std::complex<T> xk=x;
			//loop
			while(n--){
				kx1=xk-horner(xk);
				//tolleranza
				if(complexDist(xk,kx1,e)){
					xk=kx1;
					break;
				}
				//
				xk=kx1;
			}
			//return
			return xk;
		}
        
		//near roots
		//xk newton return
		//e error;
		int nearRoots(const std::complex<T>& xk,T e) const{
			for(int i=0;i<fun->roots.size();++i){
				if(complexDist(fun->roots[i],xk,e))
					return i;
			}
			return -1;
		}
        
        
        
	public:
        
		struct Values{
			int idroot;
			T intensity;
			Values(int idroot=-1,float intensity=0.0f)
            :idroot(idroot)
            ,intensity(intensity){}
		};
        
		HalleyFractal(Polynomial<T>* fun,int npass=100):fun(fun){}
		DFORCEINLINE Values calc(const std::complex<T>& xk) const{
			//vars dec
			int xkpass=fun->iterations;
			std::complex<T> tmp;
			//calc direction
			//1E-37f
			tmp=halley(xk,0.000000001f,xkpass);
			//if found
			if(xkpass>0)
				//return id root
				//todo calc minimal distance
				return Values(nearRoots(tmp,0.0001f),((T)xkpass)/fun->iterations);
			//return 0
			return Values();
		}
        
        virtual RootColor<float> calcColor(float real,float img) const{
			//vars dec
			Values values=calc(std::complex<T>(real,img));
			if(values.idroot!=-1)
				return ( RootColor<float> )(fun->rootsColor[values.idroot]*values.intensity);
			return RootColor<float>();
        }
        
        
        
	};
};
#endif