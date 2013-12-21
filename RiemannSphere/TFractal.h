#ifndef TFRACTAL_H
#define TFRACTAL_H

#include <Easy3D.h>
#include <Fractal.h>

namespace RiemannSphere {

    template <class T>
	class TFractal : public Fractal {
        
    protected:
        
		//function pointer
		Polynomial<T>* polynomial;

	public:
        //cconstructor
        TFractal(Polynomial<T>* polynomial):polynomial(polynomial){}
        //function pass
        virtual std::complex<T> pass(const std::complex<T>&) const =0 ;
        // x=start value
		// e=error max
		// n=max iteration
		DFORCEINLINE std::complex<T> succession(const std::complex<T>& x,T e,int& n) const{
			//var dec
			std::complex<T> xk1;
			std::complex<T> xk=x;
			//loop
			while(n--){
				xk1=pass(xk);
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
        //xk=start value
        DFORCEINLINE Fractal::Values calc(const std::complex<T>& xk) const{
			//vars dec
			int xkpass=TFractal<T>::polynomial->iterations;
			std::complex<T> tmp;
			//calc direction
			//1E-37f
			tmp=succession(xk,0.00000000000001f,xkpass);
			//if found
			if(xkpass>0)
				//return id root
                if(isinf(tmp)) return Values(-2,((T)xkpass)/(polynomial->iterations+1));
				//todo calc minimal distance
				return Fractal::Values(this->nearRoots(tmp,0.00000001f),((T)xkpass)/polynomial->iterations);
			//return 0
			return Fractal::Values();
		}
        //implement virtual metod (color of succrssion return)
        virtual RootColor<float> calcColor(float real,float img) const{
			//vars dec
			Values values=calc(std::complex<T>(real,img));
            //root color
			if(values.idroot>-1)
				return ( RootColor<float> )(polynomial->rootsColor[values.idroot]*values.intensity);
            //infinite color
			if(values.idroot==-2)
                return ( RootColor<float> )(polynomial->infiniteColor*values.intensity);
            //not found color
			return RootColor<float>();
        }
        //serch the most near root from z
        int nearRoots(const std::complex<T>& xk,T e) const{
			for(int i=0;i<polynomial->roots.size();++i){
				if(complexDist(polynomial->roots[i],xk,e))
					return i;
			}
			return -1;
		}

	};

};

#endif