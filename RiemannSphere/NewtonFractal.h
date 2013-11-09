#ifndef NEWTONFRACTAL_H
#define NEWTONFRACTAL_H

#include <Easy3D.h>
#include <Fractal.h>
#include <Polynomial.h>

namespace RiemannSphere {

	template <class T>
	class NewtonFractal : public Fractal {
		//pass
		int npass;

		//function pointer
		Polynomial<T>* fun;

		//distance
		DFORCEINLINE bool complexDist(const std::complex<T>& a,
									  const std::complex<T>& b,
									  T e) const{
            //return (std::abs(b-a)/Easy3D::Math::min(std::abs(b),std::abs(a)))<e;
            return (std::abs(b-a))<e;
		}

		//  f(x)/f'(x)
		//  x function argument
		DFORCEINLINE std::complex<T> fxOnDx(const std::complex<T>& x) const{

            //exit condiction
			if(fun->constants.size()==0) return 0;
			if(fun->constants.size()==1) return fun->constants[0];
            
			std::complex<T> vn=fun->constants[0];
			std::complex<T> wn=vn;
 
            //Horner
			for(int i=1;i<fun->constants.size()-1;++i){
			   vn = vn*x+fun->constants[i];
			   wn = wn*x+vn;
			}
   
			vn = vn*x+fun->constants[fun->constants.size()-1];
   
			return vn/wn;
   
		}

		// xk+1=xk-f(x)/f'(x)
		// e=error max
		// n=max iteration
		DFORCEINLINE std::complex<T> newton(const std::complex<T>& x,T e,int& n) const{
			//var dec
			std::complex<T> xk1;
			std::complex<T> xk=x;
			//loop
			while(n--){
				xk1=xk-fxOnDx(xk);
				//tolleranza
				if(complexDist(xk,xk1,e)){
					xk=xk1;
					break;
				}
				//
				xk=xk1;
			}
			//return
			return xk;
		}

		//near roots
		//xk newton return
		//e error;
		int nearRoots(const std::complex<T>& xk,T e) const{
            //Easy3D::Debug::message()<<" real:"<< xk.real()<<" img:"<<xk.imag()<<"\n";
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

		NewtonFractal(Polynomial<T>* fun,int npass=50):fun(fun),npass(npass){}
		DFORCEINLINE Values calc(const std::complex<T>& xk) const{
			//vars dec
			int xkpass=npass;
			std::complex<T> tmp;
			//calc direction
			//1E-37f
			tmp=newton(xk,0.00000000000001f,xkpass);
			//if found 
			if(xkpass>0)
				//return id root
				//todo calc minimal distance
				return Values(nearRoots(tmp,0.00000001f),((T)xkpass)/npass);
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