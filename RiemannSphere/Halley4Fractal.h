#ifndef HALLEY4FRACTAL_H
#define HALLEY4FRACTAL_H

#include <Easy3D.h>
#include <Fractal.h>
#include <Polynomial.h>

namespace RiemannSphere {
    
	template <class T>
	class Halley4Fractal : public Fractal {
		//pass
		int npass;
        
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
		DFORCEINLINE std::complex<T> fxOnDx(const std::complex<T>& x) const{
            
            //exit condiction
			if(fun->constants.size()==0) return 0;
			if(fun->constants.size()==1) return fun->constants[0];

            std::complex<T> vn=fun->constants[0];
            std::complex<T> wn=vn;
            std::complex<T> un=wn;
            std::complex<T> yn=un;
            
            //Horner
            for(int i=1;i<(int)(fun->constants.size())-3;++i){
                vn = vn*x+fun->constants[i];
                wn = wn*x+vn;
                un = un*x+wn;
                yn = yn*x+un;
            }
            
            vn = vn*x+fun->constants[fun->constants.size()-3];
            wn = wn*x+vn;
            un = un*x+wn;
            
            vn = vn*x+fun->constants[fun->constants.size()-2];
            wn = wn*x+vn;
            
            vn = vn*x+fun->constants[fun->constants.size()-1];
            
            auto wn2=wn*wn;
            auto s=(vn*wn2)-(vn*vn*un);
            auto d=((vn*vn)*yn)+(wn2*wn)-(2.0*vn*wn*un);
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
				kx1=xk-fxOnDx(xk);
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
        
		Halley4Fractal(Polynomial<T>* fun,int npass=100):fun(fun),npass(npass){}
		DFORCEINLINE Values calc(const std::complex<T>& xk) const{
			//vars dec
			int xkpass=npass;
			std::complex<T> tmp;
			//calc direction
			//1E-37f
			tmp=halley(xk,0.000000001f,xkpass);
			//if found
			if(xkpass>0)
				//return id root
				//todo calc minimal distance
				return Values(nearRoots(tmp,0.0001f),((T)xkpass)/npass);
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