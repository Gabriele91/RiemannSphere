#include <stdafx.h>
#include <iomanip>
#include <math.h>
#include <stdlib.h>
#include <PolynomialRoots.h>
// C++ reimplementation of the Durand-Kerner-Weierstrass method
//https://sites.google.com/site/drjohnbmatthews/polyroots
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

template <class T>
std::complex<T>  horner(const std::vector< std::complex<T> >& ca,
					    const std::complex<T>& x) {
    std::complex<T> result = ca[0];
    for (int i = 1; i < ca.size(); i++) {
        result = result*x+ca[i];
    }
    return result;
}

template <class T>
std::vector< std::complex<T> > getRoots(std::vector< std::complex<T> > ca){
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
            a1[i] = a0[i]-(horner(ca, a0[i])/result);
        }
        count++;
        if (count > MAX_COUNT || done(a0, a1)) break;
		a0=a1; // a0 := a1
    }
    return a1;
    
}
//
#define allocaDouble(x) (double*)alloca((x)*sizeof(double))
void RiemannSphere::getPolynomialRoots(std::vector< std::complex<double> >& consts,
                                       std::vector< std::complex<double> >& output ){

    output=getRoots(consts);
}
void RiemannSphere::getPolynomialRoots(std::vector< std::complex<float> >& consts,
                                       std::vector< std::complex<float> >& output ){
	 
    output=getRoots(consts);
}
