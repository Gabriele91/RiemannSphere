#include <stdafx.h>
#include <Polynomial.h>
#define pi M_PI

template < class T>
void HSVtoRGB( T *r, T *g, T *b, T h, T s, T v ){
	int i;
	T f, p, q, t;
	if( s == 0 ) {
		// achromatic (grey)
		*r = *g = *b = v;
		return;
	}
	h /= 60;			// sector 0 to 5
	i = floor( h );
	f = h - i;			// factorial part of h
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );
	switch( i ) {
		case 0:
			*r = v;
			*g = t;
			*b = p;
			break;
		case 1:
			*r = q;
			*g = v;
			*b = p;
			break;
		case 2:
			*r = p;
			*g = v;
			*b = t;
			break;
		case 3:
			*r = p;
			*g = q;
			*b = v;
			break;
		case 4:
			*r = t;
			*g = p;
			*b = v;
			break;
		default:		// case 5:
			*r = v;
			*g = p;
			*b = q;
			break;
	}
}

void RiemannSphere::PolynomialColor::colors(int n,std::vector< RootColor<double> >& vout){

    double color_delta=360/(n);
    vout.resize(n);
    double val=120;
    for(int i=1;i<=n;++i)
        HSVtoRGB<double>(&vout[i-1].r,&vout[i-1].g,&vout[i-1].b,std::fmod((color_delta*i)+val,360),1,1);
    
}
void RiemannSphere::PolynomialColor::colors(int n,std::vector< RootColor<float> >& vout){
    float color_delta=360/(n);
    vout.resize(n);
    float val=120;
    for(int i=1;i<=n;++i)
        HSVtoRGB<float> (&vout[i-1].r,&vout[i-1].g,&vout[i-1].b,std::fmodf((color_delta*i)+val,360),1,1);
}


///////////////////////
using namespace Easy3D;
using namespace RiemannSphere;
///////////////////////
// EBNF3
// number_int := {<number_int>}
// number_float := [<number_int >] '.' <number_int>
// signed := '-' | '+'
// const_real := [<signed>] <number_float>
// const_complex := [<signed>] ( '(' <const_real> <const_real> 'i' ')' | <const_real> | [ <const_real> ] i)
// variable := 'x'
// grade := [ <const_complex> | <const_real> ] <variable> ['^' <number_int> ]
// poly := [<signed>] grade {[ <signed> <grade> ]}


bool isspace(const char* c){
	return (*c)==' '|(*c)=='\r'|(*c)=='\t';
}
bool isvariable(const char* c){
	return (*c)=='z';
}
bool isinteger(const char* c){
	return  (*c)=='0'|(*c)=='1'|(*c)=='2'|
    (*c)=='3'|(*c)=='4'|(*c)=='5'|
    (*c)=='6'|(*c)=='7'|(*c)=='8'|
    (*c)=='9';
}
bool isimag(const char* c){
	return (*c)=='i';
}
bool isleftp(const char* c){
	return (*c)=='(';
}
bool isrightp(const char* c){
	return (*c)==')';
}
bool isplus(const char* c){
	return (*c)=='+';
}
bool ismin(const char* c){
	return (*c)=='-';
}
bool isnumber(const char* c){
	return ((*c)=='-'&&isinteger(c+1))|
    ((*c)=='+'&&isinteger(c+1))|
    ((*c)=='.'&&isinteger(c+1))|
    isinteger(c);
}
bool ispow(const char* c){
	return (*c)=='^';
}
void jumpspace(const char*& c,int& nc){
	while(isspace(c)){ ++c; ++nc; }
}

#define next { ++c;++nc; }

struct ParseError{
    std::string error;
    int nc;
	ParseError(const std::string& er,int nc):error(er),nc(nc){}
};

template<class T>
struct powValue{
    std::complex<T> value;
    int pow;
};

int parseInt(const char*& c,int& nc){
    //parse int
    char *cend;
    double value=std::strtod(c, &cend);
    //is not a int
    if(value!=std::floor(value))
        throw ParseError("must to be an integer value",nc);
    //cout chars
    nc+=cend-c;
    //set next char
    c=cend;
    //return value
    return (int)value;
}

template <class T> T parseNumber(const char*& c,int& nc){
    //parse int
    char *cend;
    T value=(T)std::strtod(c, &cend);
    //cout chars
    nc+=cend-c;
    //set next char
    c=cend;
    //return value
    return value;
}

template <class T>
std::complex<T> const_complex(const char*& c,int& nc){
    jumpspace(c,nc);
    //(<complex> or float/real or <number>i or i
    if(!isleftp(c)){
        //real or <number>i
        if(isnumber(c)){
            //parse number
            T value=parseNumber<T>(c,nc);
            //<number>i ?
            if(isimag(c)){
                next
                return {0,value};
            }
            //is a real
            //return value
            return {value,0.0};
        }
        //i
        else if(isimag(c)){
            next //next i
            return {0.0,1.0};
        }
        else
            throw ParseError("invalid complex constant",nc);
    }
    //jump (
    next
    //(....<number>
    jumpspace(c,nc);
    if(!isnumber(c)) throw  ParseError("invalid complex real part",nc);
    T real=parseNumber<T>(c,nc);
    //imag
    T imag=0;
    //find +/-
    if(!isplus(c)&&!ismin(c)) throw  ParseError("invalid complex imag part",nc);
    imag=(isplus(c)?+1.0:-1.0);
    next
    //+/-<number>i
    if(!isnumber(c)){
        //+/-i
        if(!isimag(c)) throw  ParseError("invalid complex imag part",nc);
        next
    }
    else{
        //<number>
        imag*=parseNumber<T>(c,nc);
        //i
        if(!isimag(c)) throw  ParseError("invalid complex imag part",nc);
        next
    }
    //)
    if(!isrightp(c)) throw  ParseError("invalid complex imag part",nc);
    next
    //return
    return {real,imag};
}

template <class T>
powValue<T> grade(bool first,const char*& c,int& nc) {
    //out value
    powValue<T> vout;
    //jump
    jumpspace(c,nc);
    //+/-
    if(isplus(c)){
        vout.value=1.0;
        next
        jumpspace(c,nc);
    }
    else if(ismin(c)){
        vout.value=-1.0;
        next
        jumpspace(c,nc);
    }
    else if(first)
        vout.value=1.0;
    else
        throw  ParseError("value whiout signed",nc);
    //+/- <complex>
    if(isleftp(c)||isnumber(c)||isimag(c)){
        vout.value*=const_complex<T>(c,nc);
        jumpspace(c,nc);
    }
    //variable and pow
    if(isvariable(c)){
        //jump x
        next
        //jump space
        jumpspace(c,nc);
        //^
        if(!ispow(c)){
            //...x^1
            vout.pow=1;
            return vout;
        }
        next
        //number
        vout.pow=parseInt(c,nc);
		//
		if(vout.pow<0)
		  throw  ParseError("invalid power number",nc);
    }
    else
        vout.pow=0;
    //value return
    return vout;
}

template <class T>
std::vector< std::complex<T> > poly(const char*& c,int& nc){
    //values out
    std::vector< std::complex<T> > valueout;
    //values bool
    std::vector< bool > boolout;
    //first is true
    bool first=true;
    //loop read
    while(*c){
        //first pow
        auto xcurrent=grade<T>(first,c,nc);
        //size pow
        if(valueout.size()<=xcurrent.pow){
            valueout.resize(xcurrent.pow+1);
            boolout.resize(xcurrent.pow+1);
        }
        //is olready setted?
        if(boolout[xcurrent.pow])
            throw  ParseError("value olready setted",nc);
        //look variable
        boolout[xcurrent.pow]=true;
        //save value
        valueout[xcurrent.pow]=xcurrent.value;
        //next is not first
        first=false;
    }
    //reverse
    std::reverse(valueout.begin(),valueout.end());
    //return value
    return  valueout;
}

int  RiemannSphere::PolynomialParse::parse(const std::string& text,
                                           std::vector< std::complex<double> >& out,
                                           std::string& errors){
    int nc=1;
    const char* c=text.c_str();
    try{
        out=poly<double>(c,nc);
    }catch (ParseError e){
        errors += e.error + ":" + std::to_string( e.nc)+"\n";
        return  e.nc-1;
    }
    return -1;
}
int RiemannSphere::PolynomialParse::parse(const std::string& text,
                                           std::vector< std::complex<float> >& out,
                                           std::string& errors){
    int nc=1;
    const char* c=text.c_str();
    try{
        out=poly<float>(c,nc);
    }catch (ParseError e){
        errors += e.error + ":" + std::to_string( e.nc)+"\n";
        return  e.nc-1;
    }
    return -1;
}