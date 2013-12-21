#ifndef NEWTONFRACTAL_H
#define NEWTONFRACTAL_H

#include <Easy3D.h>
#include <TFractal.h>
#include <Polynomial.h>

namespace RiemannSphere {

	template <class T>
	class NewtonFractal : public TFractal<T> {
        

		//  f(x)/f'(x)
        virtual std::complex<T> pass(const std::complex<T>& x) const{
            //exit condiction
			if(this->polynomial->constants.size()==0) return 0;
			if(this->polynomial->constants.size()==1) return this->polynomial->constants[0];
            
			std::complex<T> vn=this->polynomial->constants[0];
			std::complex<T> wn=vn;
 
            //Horner
			for(int i=1;i<this->polynomial->constants.size()-1;++i){
			   vn = vn*x+this->polynomial->constants[i];
			   wn = wn*x+vn;
			}
   
			vn = vn*x+this->polynomial->constants[this->polynomial->constants.size()-1];
            
            //newton
			return  x-(vn/wn);
   
		}


	public:

		NewtonFractal(Polynomial<T>* polynomial):TFractal<T>(polynomial){}

	};

};

#endif