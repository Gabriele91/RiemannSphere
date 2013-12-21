#ifndef SCHROEDERFRACTAL_H
#define SCHROEDERFRACTAL_H

#include <Easy3D.h>
#include <TFractal.h>

namespace RiemannSphere {

    template <class T>
	class SchroederFractal : public TFractal<T> {
        
		//  Schroeder
		//  x function argument
		DFORCEINLINE std::complex<T> pass(const std::complex<T>& x) const{
            
            //exit condiction
			if(this->polynomial->constants.size()==0) return 0;
			if(this->polynomial->constants.size()==1) return this->polynomial->constants[0];
            
            std::complex<T> vn=this->polynomial->constants[0];
            std::complex<T> wn=vn;
            std::complex<T> un=wn;
            
            //Horner
            for(int i=1;i<(int)(this->polynomial->constants.size())-2;++i){
                vn = vn*x+this->polynomial->constants[i];
                wn = wn*x+vn;
                un = un*x+wn;
            }
            
            vn = vn*x+this->polynomial->constants[this->polynomial->constants.size()-2];
            wn = wn*x+vn;
            vn = vn*x+this->polynomial->constants[this->polynomial->constants.size()-1];
            
            //Schroeder
            auto q=vn/wn;
            auto s1=q+q*q*un/wn;
            
            //
            return x-s1;
            
		}
        
	public:
        

        
		SchroederFractal(Polynomial<T>* polynomial):TFractal<T>(polynomial){}
        
        
        
	};

};

#endif