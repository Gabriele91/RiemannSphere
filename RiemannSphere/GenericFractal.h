#ifndef GENERICFRACTAL_H
#define GENERICFRACTAL_H

#include <Easy3D.h>
#include <TFractal.h>
#include <Polynomial.h>

namespace RiemannSphere {

	template <class T>
	class GenericFractal : public TFractal<T> {

		//function pointer
		Polynomial<T>* fun;
        


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

		// pass=f(x)/g(x)
		DFORCEINLINE std::complex<T> pass(const std::complex<T>& x) const{
			//return
			return horner(x,this->polynomial->constants)/horner(x,this->polynomial->subconstants);
		}

	public:


		GenericFractal(Polynomial<T>* polynomial):TFractal<T>(polynomial){}



	};

};

#endif