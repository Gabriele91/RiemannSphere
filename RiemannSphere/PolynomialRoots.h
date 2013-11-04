#ifndef POLYNOMIALROOTS_H
#define POLYNOMIALROOTS_H

#include <Easy3D.h>
#include <complex>

namespace RiemannSphere{
	
	void getPolynomialRoots(const std::vector< std::complex<double> >& a,
                                  std::vector< std::complex<double> >& output );
	void getPolynomialRoots(const std::vector< std::complex<float> >& a,
                                  std::vector< std::complex<float> >& output );

};
#endif