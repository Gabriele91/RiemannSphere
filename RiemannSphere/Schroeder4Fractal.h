#ifndef SCHROEDER4FRACTAL_H
#define SCHROEDER4FRACTAL_H

#include <Easy3D.h>
#include <TFractal.h>

namespace RiemannSphere {

    template <class T>
	class Schroeder4Fractal : public TFractal<T> {
        
		//  Schroeder4
		//  x function argument
		virtual std::complex<T> pass(const std::complex<T>& x) const{
            
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
            
            //Schroeder4
            auto q=vn/wn;
            auto s3=q+(q*q)*(un/wn);
            auto s4=s3-(q*q*q)*((yn/wn)-(un/(wn*wn)));
            
            //
            return x-s4;
            
		}
        
	public:
        

        
		Schroeder4Fractal(Polynomial<T>* polynomial):TFractal<T>(polynomial){}
	
        
        
        
	};

};

#endif