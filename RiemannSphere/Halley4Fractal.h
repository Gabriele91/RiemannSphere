#ifndef HALLEY4FRACTAL_H
#define HALLEY4FRACTAL_H

#include <Easy3D.h>
#include <TFractal.h>
#include <Polynomial.h>

namespace RiemannSphere {
    
	template <class T>
	class Halley4Fractal : public TFractal<T> {
        
		//  halley 4
		DFORCEINLINE std::complex<T> pass(const std::complex<T>& x) const{
            
            //exit condiction
			if(this->polynomial->constants.size()==0) return 0;
			if(this->polynomial->constants.size()==1) return this->polynomial->constants[0];

            std::complex<T> vn=this->polynomial->constants[0];
            std::complex<T> wn=vn;
            std::complex<T> un=wn;
            std::complex<T> yn=un;
            
            //Horner
            for(int i=1;i<(int)(this->polynomial->constants.size())-3;++i){
                vn = vn*x+this->polynomial->constants[i];
                wn = wn*x+vn;
                un = un*x+wn;
                yn = yn*x+un;
            }
            
            vn = vn*x+this->polynomial->constants[this->polynomial->constants.size()-3];
            wn = wn*x+vn;
            un = un*x+wn;
            
            vn = vn*x+this->polynomial->constants[this->polynomial->constants.size()-2];
            wn = wn*x+vn;
            
            vn = vn*x+this->polynomial->constants[this->polynomial->constants.size()-1];
            
            //halley4
            auto wn2=wn*wn;
            auto s=(vn*wn2)-(vn*vn*un);
            auto d=((vn*vn)*yn)+(wn2*wn)-(2.0*vn*wn*un);
            
            //
            return x-(s/d);
            
		}
        
        
	public:
        
		Halley4Fractal(Polynomial<T>* polynomial):TFractal<T>(polynomial){}
        
	};
};
#endif