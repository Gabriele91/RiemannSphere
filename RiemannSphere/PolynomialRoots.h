#ifndef POLYNOMIALROOTS_H
#define POLYNOMIALROOTS_H

#include <Easy3D.h>
#include <complex>

namespace RiemannSphere{
	
	void getPolynomialRoots( std::vector<double>& a,std::vector< std::complex<double> >& output );
	void getPolynomialRoots( std::vector<float>& a,std::vector< std::complex<float> >& output );

};
#endif