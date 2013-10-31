#ifndef GENERICFRACTAL_H
#define GENERICFRACTAL_H

#include <Easy3D.h>
#include <Fractal.h>
#include <Polynomial.h>

namespace RiemannSphere {

	template <class T>
	class GenericFractal : public Fractal {
		//pass
		int npass;

		//function pointer
		Polynomial<T>* fun;

		//distance
		DFORCEINLINE bool complexDist(const std::complex<T>& a,
									  const std::complex<T>& b,
									  T e) const{
            return std::abs(b-a)<e;
		}

		//  f(x)/f'(x)
		//  x function argument
		DFORCEINLINE std::complex<T> horner(const std::complex<T>& x,std::vector< std::complex<T> >& cns) const{
            //exit condiction
			if(cns.size()==0) return 0;
			if(cns.size()==1) return cns[0];
            //start
			std::complex<T> vn=cns[0];
            //Horner
			for(int i=1;i<cns.size();++i) vn = vn*x+cns[i];
   
			return vn;
   
		}

		// xk+1=xk-f(x)/g(x)
		// e=error max
		// n=max iteration
		DFORCEINLINE std::complex<T> newton(const std::complex<T>& x,T e,int& n) const{
			//var dec
			std::complex<T> xk1;
			std::complex<T> xk=x;
			//loop
			while(n--){
				xk1=(horner(xk,fun->constants)/
                     horner(xk,fun->subconstants));
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

		GenericFractal(Polynomial<T>* fun,int npass=50):fun(fun),npass(npass){}
		DFORCEINLINE Values calc(const std::complex<T>& xk) const{
			//vars dec
			int xkpass=npass;
			std::complex<T> tmp;
			//calc direction
			//1E-37f
			tmp=newton(xk,0.00001f,xkpass);
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