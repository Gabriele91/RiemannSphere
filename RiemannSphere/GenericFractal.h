#ifndef GENERICFRACTAL_H
#define GENERICFRACTAL_H

#include <Easy3D.h>
#include <Fractal.h>
#include <Polynomial.h>

namespace RiemannSphere {

	template <class T>
	class GenericFractal : public Fractal {

		//function pointer
		Polynomial<T>* fun;
        
        //is infinite
        DFORCEINLINE bool isinf(const std::complex<T>& a) const{
			return Easy3D::Math::isinf(a.real())||Easy3D::Math::isinf(a.imag());
		}

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
		DFORCEINLINE std::complex<T> itfun(const std::complex<T>& x,T e,int& n) const{
			//var dec
			std::complex<T> xk1;
			std::complex<T> xk=x;
			//loop
			while(n--){
				xk1=(horner(xk,fun->constants)/
                     horner(xk,fun->subconstants));
				//tolleranza
				if(complexDist(xk,xk1,e)||isinf(xk1)){
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

		GenericFractal(Polynomial<T>* fun):fun(fun){}
		DFORCEINLINE Values calc(const std::complex<T>& xk) const{
			//vars dec
			int xkpass=fun->iterations;
			std::complex<T> tmp;
			//calc direction
			//1E-37f
			tmp=itfun(xk,0.0000001f,xkpass);
			//if found 
			if(xkpass>0){
                //infinite color
                if(isinf(tmp)) return Values(-2,((T)xkpass)/(fun->iterations+1));
				//return id root
				//todo calc minimal distance
				return Values(nearRoots(tmp,0.0001f),((T)xkpass)/(fun->iterations+1));
            }
			//return 0
			return Values();
		}
        
        virtual RootColor<float> calcColor(float real,float img) const{
			//vars dec
			Values values=calc(std::complex<T>(real,img));
            //root color
			if(values.idroot>-1)
				return ( RootColor<float> )(fun->rootsColor[values.idroot]*values.intensity);
            //infinite color
			if(values.idroot==-2)
                return ( RootColor<float> )(fun->infiniteColor*values.intensity);
            //not found color
			return RootColor<float>();
        }



	};

};

#endif