#ifndef NEWTONFRACTAL_H
#define NEWTONFRACTAL_H

#include <Easy3D.h>
#include <Polynomial.h>

namespace RiemannSphere {

	template <class T>
	class NewtonFractal {
		//pass
		int npass;

		//function pointer
		Polynomial<T>* fun;

		//distance
		DFORCEINLINE bool complexDist(const std::complex<T>& a,
									  const std::complex<T>& b,
									  T e) const{
			return ( std::abs(b.imag()-a.imag()) < e && std::abs(b.real()-a.real()) < e );
		}

		//  f(x)/f'(x)
		//  x function argument
		DFORCEINLINE std::complex<T> fxOnDx(const std::complex<T>& x) const{

			if(fun->constants.size()<2) return 0;
   
			std::complex<T> vn=fun->constants[fun->constants.size()-1];
			std::complex<T> wn=vn;
 
			for(int i=fun->constants.size()-2;i>0;--i){
			   vn = vn*x+fun->constants[i];
			   wn = wn*x+vn;
			}
   
			vn = vn*x+fun->constants[0];
   
			return vn/wn;
   
		}

		// xk+1=xk-f(x)/f'(x)
		// e=error max
		// n=max iteration
		DFORCEINLINE std::complex<T> newton(const std::complex<T>& x,T e,int& n) const{
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

		NewtonFractal(Polynomial<T>* fun,int npass=100):fun(fun),npass(npass){}
		DFORCEINLINE Values calc(const std::complex<T>& xk) const{
			//vars dec
			int xkpass=npass;
			std::complex<T> tmp;
			//calc direction
			//1E-37f
			tmp=newton(xk,0.000000001f,xkpass);
			//if found 
			if(xkpass>0)
				//return id root
				//todo calc minimal distance
				return Values(nearRoots(tmp,0.0001f),((T)xkpass)/npass);
			//return 0
			return Values();
		}		
		DFORCEINLINE RootColor<T> calcColor(const std::complex<T>& xk) const{
			//vars dec
			Values values=calc(xk);
			if(values.idroot!=-1)
				return fun->rootsColor[values.idroot]*values.intensity;	
			return RootColor<T>();
		}



	};

};

#endif