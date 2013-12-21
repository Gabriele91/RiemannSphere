#ifndef HALLEYFRACTAL_H
#define HALLEYFRACTAL_H

#include <Easy3D.h>
#include <TFractal.h>
#include <Polynomial.h>

namespace RiemannSphere {
    
	template <class T>
	class HalleyFractal : public TFractal<T>{
    
		// halley: ( f(x)*f'(x) ) / ( (f'(x)^2) - ( (f''(x)^2)/4 ) )
		//  x function argument
		virtual std::complex<T> pass(const std::complex<T>& x) const{
            
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
            
            //halley
            auto s=vn*wn;
            auto d=(wn*wn)-vn*un;
            
            //
            return x-(s/d);
            
		}
        
        
	public:
        
        
		HalleyFractal(Polynomial<T>* polynomial):TFractal<T>(polynomial){}
        
        
        
	};
};
#endif