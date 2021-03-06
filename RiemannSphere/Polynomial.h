#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <Easy3D.h>
#include <complex>
#include <PolynomialRoots.h>

namespace RiemannSphere {
	
	template <class T>
	struct RootColor{
		T r,g,b;
		RootColor():r(0.0f),g(0.0f),b(0.0f){}
		RootColor(T r,T g,T b):r(r),g(g),b(b){}
		RootColor(const Easy3D::Vec3& v):r(v.x),g(v.y),b(v.z){}
        /*
		RootColor(const Easy3D::Color& c)
        :r(c.rNormalize())
        ,g(c.gNormalize())
        ,b(c.bNormalize()){}
        */
		DFORCEINLINE const RootColor& operator*=(T v){
			r*=v;g*=v;b*=v;
			return (*this);
		}
		DFORCEINLINE RootColor operator * (T v) const{
			return {r*v,g*v,b*v};
		}
        
        DFORCEINLINE operator RootColor<float>(){
            return RootColor<float>((float)r,(float)g,(float)b);
        }
        DFORCEINLINE operator RootColor<double>(){
            return RootColor<float>((double)r,(double)g,(double)b);
        }
        DFORCEINLINE RootColor<T>& setFromColor(const Easy3D::Color& c){
            this->r=c.rNormalize();
            this->g=c.gNormalize();
            this->b=c.bNormalize();
            return (*this);
        }
        DFORCEINLINE operator Easy3D::Color(){
            return Easy3D::Color((Easy3D::uchar)(r*255),
                                 (Easy3D::uchar)(g*255),
                                 (Easy3D::uchar)(b*255),
                                 255);
        }
        
	};

    class PolynomialColor{
    public:
       static void colors(int n,std::vector< RootColor<double> >& vout);
       static void colors(int n,std::vector< RootColor<float> >& vout);
    };
    
    class PolynomialParse{
    public:
        static int parse(const std::string& text,
                          std::vector< std::complex<double> >& vout,
                          std::string& errors);
        
        static int parse(const std::string& text,
                          std::vector< std::complex<float> >& vout,
                          std::string& errors);
    
    };
    
    namespace Iterations{
        enum Methos{
            NEWTON=0,
            HALLEY,
            HALLEY4,
            SCHROEDER,
            SCHROEDER4,
            GENERIC,
            FRACTMAX
        };
        static Easy3D::String Names[FRACTMAX+1]={
            "newton",
            "halley",
            "halley4",
            "schroeder",
            "schroeder4",
            "generic",
            "fractmax"
        };

    }

	template <class T>
	class Polynomial {
		

        
	public:

        
        Iterations::Methos method;
        int iterations;
        
		std::vector< std::complex<T> > constants;
		std::vector< std::complex<T> > subconstants;
		std::vector< std::complex<T> > roots;
		std::vector< RootColor<T> > rootsColor;
		RootColor<T> infiniteColor;
        
        Polynomial(const Easy3D::Table& table){
            
            if(table.getDeserializeErros().size()){
                Easy3D::Debug::message()<<"Polynomial table:\n"<<table.getDeserializeErros();
            }
            
            if(table.existsAsType("constants",Easy3D::Table::TABLE)){
                const Easy3D::Table& tconstants=table.getConstTable("constants");
                for(auto cns:tconstants){
                    Easy3D::Debug::doassert((cns.second->asType(Easy3D::Table::FLOAT))||
                                            (cns.second->asType(Easy3D::Table::VECTOR2D)),
                                            "Easy3D::Table::FLOAT or Easy3D::Table::VEC2D",__FILE__,__LINE__);
                    if(cns.second->asType(Easy3D::Table::FLOAT))
                        constants.push_back( (T)cns.second->get<float>() );
                    else
                        constants.push_back( std::complex<T>(cns.second->get<Easy3D::Vec2>().x,
                                                             cns.second->get<Easy3D::Vec2>().y) );
                }
            }
            else if(table.existsAsType("constants",Easy3D::Table::STRING)){
                Easy3D::String errors;
                Easy3D::Debug::doassert(PolynomialParse::parse(table.getString("constants"),constants,errors)==-1,errors,__FILE__,__LINE__);
            }
            
            if(table.existsAsType("subconstants",Easy3D::Table::TABLE)){
                const Easy3D::Table& tconstants=table.getConstTable("subconstants");
                for(auto cns:tconstants){
                    Easy3D::Debug::doassert((cns.second->asType(Easy3D::Table::FLOAT))||
                                            (cns.second->asType(Easy3D::Table::VECTOR2D)),
                                            "Easy3D::Table::FLOAT or Easy3D::Table::VEC2D",__FILE__,__LINE__);
                    if(cns.second->asType(Easy3D::Table::FLOAT))
                        subconstants.push_back( (T)cns.second->get<float>() );
                    else
                        subconstants.push_back( std::complex<T>(cns.second->get<Easy3D::Vec2>().x,
                                                                cns.second->get<Easy3D::Vec2>().y) );
                }
            }
            else if(table.existsAsType("subconstants",Easy3D::Table::STRING)){
                Easy3D::String errors;
                Easy3D::Debug::doassert(PolynomialParse::parse(table.getString("subconstants"), subconstants,errors)==-1,errors,__FILE__,__LINE__);
            }
            
            if(table.existsAsType("roots",Easy3D::Table::TABLE)){
                const Easy3D::Table& troots=table.getConstTable("roots");
                for(auto rt:troots){
                    Easy3D::Debug::doassert((rt.second->asType(Easy3D::Table::VECTOR2D)),
                                            "Easy3D::Table::VECTOR2D",__FILE__,__LINE__);
                    std::complex<T> root(rt.second->get<Easy3D::Vec2>().x,
                                         rt.second->get<Easy3D::Vec2>().y);
                    roots.push_back(root);
                }
            }
			else calcRoots();
                
            if(table.existsAsType("rootsColors",Easy3D::Table::TABLE)){
                const Easy3D::Table& troots=table.getConstTable("rootsColors");
                for(auto rt:troots){
                    Easy3D::Debug::doassert((rt.second->asType(Easy3D::Table::VECTOR3D)),"Easy3D::Table::VECTOR3D",__FILE__,__LINE__);
                    rootsColor.push_back({rt.second->get<Easy3D::Vec3>().x,
                        rt.second->get<Easy3D::Vec3>().y, 
                        rt.second->get<Easy3D::Vec3>().z});
                }
            }
            else PolynomialColor::colors((int)roots.size(),rootsColor);
            
            //infinite root color
            infiniteColor=table.getVector3D("infiniteColor",Easy3D::Vec3::ONE);
            
            //default newton
            method=Iterations::NEWTON;
            //read from table
            Easy3D::String strmethod=table.getString("method","newton").toLower();
            if(strmethod=="newton"||strmethod=="n") method=Iterations::NEWTON;
            else if(strmethod=="halley"||strmethod=="h") method=Iterations::HALLEY;
            else if(strmethod=="halley4"||strmethod=="h4") method=Iterations::HALLEY4;
            else if(strmethod=="schroeder"||strmethod=="s") method=Iterations::SCHROEDER;
            else if(strmethod=="schroeder4"||strmethod=="s4") method=Iterations::SCHROEDER4;
            else if(strmethod=="generic"||strmethod=="g") method=Iterations::GENERIC;
            
            //get iterations
            iterations=table.getFloat("iterations",50);
            
        }
        void calcRoots(){
            if(subconstants.size()){
                size_t size=std::max(constants.size(),subconstants.size()+1);
                std::vector< std::complex<T> > pzq(size,0)
                ,p1(size,0)
                ,q1(size,0);
                //q1
                size_t size_q1=size-subconstants.size()-1;
                for(size_t i=size_q1;i!=size-1;++i)
                    q1[i]=subconstants[i-size_q1];
                //p1
                size_t size_p1=size-constants.size();
                for(size_t i=size_p1;i!=size;++i)
                    p1[i]=constants[i-size_p1];
                //p-zq
                for(size_t i=0;i!=size;++i)
                    pzq[i]=p1[i]-q1[i];
                
                getPolynomialRoots(pzq,roots);
            }
            else{
                getPolynomialRoots(constants,roots);
            }
        }
        
        bool eqRoots(){
            for(size_t i=0;i!=roots.size();++i){
                for(size_t j=0;j!=roots.size();++j){
                   if( i!=j && std::abs(roots[i]-roots[j]) < 0.0001f )
                       return true;
                }
            }
            return false;
        }
        
        bool recalcPolynomial(const Easy3D::String& poly,Easy3D::String& outerrors){
            //parse poly
			std::vector< std::complex<T> > tempConstants;
			//parsing
			Easy3D::String errors;
			int error=PolynomialParse::parse(poly,tempConstants,errors);
			//get errors
			if(error>=0){
				int start=Easy3D::Math::max((int)(error-4),(int)(0));
				int maxLen=Easy3D::Math::max(Easy3D::Math::min((int)(poly.size()-start),8),0);
				outerrors="Invalid expression: "+poly.substr(start,maxLen);
				return false;
			}
			else if(tempConstants.size()<2){
				outerrors="The polynomial mast to be at least of first degree";
				return false;
			}
			//save new constants
			constants=tempConstants;
            //clear subs
            subconstants.clear();
            //recalc roots
            roots.clear();
            calcRoots();
            //recalc colors
            rootsColor.clear();
            PolynomialColor::colors((int)roots.size(),rootsColor);
            //equivalent roots
            if(eqRoots())
                outerrors="warning: there are some equals roots";
			//default return true
			return true;
        }
        bool recalcPolynomial(const Easy3D::String& poly,const Easy3D::String& subpoly){
            //get errors
            Easy3D::String errors;
            //parse poly
            Easy3D::Debug::doassert(PolynomialParse::parse(poly,constants,errors),errors,__FILE__,__LINE__);
            //clear subpoly
            Easy3D::Debug::doassert(PolynomialParse::parse(subpoly,subconstants,errors),errors,__FILE__,__LINE__);
            //recalc roots
            roots.clear();
            calcRoots();
            //recalc colors
            rootsColor.clear();
            PolynomialColor::colors((int)roots.size(),rootsColor);
			//default return true
			return true;
        }
        
        static Easy3D::Vec3 planeToSphere(std::complex<T>& root){
            float a2=root.real()*root.real();
            float b2=root.imag()*root.imag();
            return Easy3D::Vec3( root.real()*2/(a2+b2+1), (a2+b2-1)/(a2+b2+1), root.imag()*2/(a2+b2+1));
        }
        static std::complex<T> sphereToPlane(const Easy3D::Vec3& pos){
            return std::complex<T>( pos.x/(1.0-pos.y), pos.z/(1.0-pos.y) );
        }
	};

};

#endif