#include <stdafx.h>
#include <iomanip>
#include <math.h>
#include <stdlib.h>
#include <PolynomialRoots.h>
#include <algorithm>
// C++ reimplementation of the Durand-Kerner-Weierstrass method
const int MAX_COUNT = 5000;
const double epsilon = 1E-15;

// Determine if the components of two arrays are unchanging
template <class T>
bool done(const std::vector< std::complex<T> >& a,
		  const std::vector< std::complex<T> >& b) {
    bool unchanged = true;
    std::complex<T> delta;
    for (int i = 0; i < a.size(); i++) {
        delta = a[i]-b[i];
        unchanged &= std::abs(delta) < (T)epsilon;
    }
    return unchanged;
}
/*
 //https://sites.google.com/site/drjohnbmatthews/polyroots
template <class T>
std::complex<T>  horner_values(const std::vector< std::complex<T> >& ca,
					    const std::complex<T>& x) {
    std::complex<T> result = ca[0];
    for (int i = 1; i < ca.size(); i++) {
        result = result*x+ca[i];
    }
    return result;
}

template <class T>
std::vector< std::complex<T> > getRoots_values(std::vector< std::complex<T> > ca){
	//temps vars
	std::vector< std::complex<T> > a0(ca.size()-1,std::complex<T>(0.0,0.0));
    std::vector< std::complex<T> > a1(ca.size()-1,std::complex<T>(0.0,0.0));
    
    // Divide by leading coefficient if not monic
    std::complex<T> leading = ca[0];
    if (ca[0]!=std::complex<T>(1.0,0.0)) {
        for (int i = 0; i < ca.size(); i++) {
            ca[i] = ca[i]/leading;
        }
    }
    
	// Initialize a0
    std::complex<T> result = std::complex<T>(0.4, 0.9);
    a0[0] = std::complex<T>(1.0,0.0);
    for (int i = 1; i < a0.size(); i++) {
        a0[i] = a0[i - 1]*result;
    }
    
	// Iterate
    int count = 0;
    while (true) {
        for (int i = 0; i < a0.size(); i++) {
            result =  std::complex<T>(1.0,0.0);
            for (int j = 0; j < a0.size(); j++) {
                if (i != j) {
                    result = (a0[i]-a0[j])*result;
                }
            }
            a1[i] = a0[i]-(horner_values(ca, a0[i])/result);
        }
        count++;
        if (count > MAX_COUNT || done(a0, a1)) break;
		a0=a1; // a0 := a1
    }
    return a1;
    
}
*/


template <class T>
DFORCEINLINE std::complex<T> dxOnFx(const std::vector< std::complex<T> >& constants,
                                    const std::complex<T>& x) {
    
    //exit condiction
    if(constants.size()==0) return 0;
    if(constants.size()==1) return constants[0];
    
    std::complex<T> vn=constants[0];
    std::complex<T> wn=vn;
    
    //Horner
    for(int i=1;i<constants.size()-1;++i){
        vn = vn*x+constants[i];
        wn = wn*x+vn;
    }
    
    vn = vn*x+constants[constants.size()-1];
    
    return wn/vn;
    
}
template <class T>
std::vector< std::complex<T> > getRoots(std::vector< std::complex<T> > fx){
    #define CPXONE std::complex<T>(1.0,0.0)
	//temps
    std::vector< std::complex<T> > xk(fx.size()-1,std::complex<T>(0.0,0.0));
    std::vector< std::complex<T> > xkk(fx.size()-1,std::complex<T>(0.0,0.0));
    //calc px
    //std::complex<T> leading = fx[0];
    //for (size_t i = 0; i != fx.size(); i++)
    //        fx[i] /=leading;
    
    //const T offset=1.134678;
    const T offset=1.144678;
    //start values
    //root of unity
    for (size_t i = 0; i != xk.size(); ++i){
        xk[i]=std::complex<T>(std::cos(((double)M_PI*(offset+2*i))/(double)xk.size()),
                               std::sin(((double)M_PI*(offset+2*i))/(double)xk.size()));
    }
    
    //count
    int itCount=0;
    
    //while
    while(true){
        //succession
        for(size_t i=0; i!=xk.size(); ++i){
            //p1(x) / p(x)
            auto p1onpx=dxOnFx(fx,xk[i]);
            //\sigma[j=1,n && j!=i]{ 1/(x[i]-x[j]) }
            std::complex<T> sum;
            for (size_t j = 0; j != xk.size(); ++j){
                if (i != j)
                    sum += (CPXONE/(xk[i]-xk[j]));
            }
            //calc pass
            xkk[i] = xk[i]-(CPXONE/(p1onpx-sum));
        }
        //close it
        ++itCount;
        if (itCount > MAX_COUNT || done(xk, xkk)) break;
        //next k
		xk=xkk;
    }
    //ret res
    return xkk;
}

//
#define allocaDouble(x) (double*)alloca((x)*sizeof(double))
void RiemannSphere::getPolynomialRoots(const std::vector< std::complex<double> >& consts,
                                             std::vector< std::complex<double> >& output ){
    output=getRoots(consts);
}
void RiemannSphere::getPolynomialRoots(const std::vector< std::complex<float> >& consts,
                                             std::vector< std::complex<float> >& output ){
	 
    output=getRoots(consts);
}
