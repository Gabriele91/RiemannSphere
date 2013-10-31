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
        DFORCEINLINE operator Easy3D::Color(){
            return Easy3D::Color((Easy3D::uchar)(r*255),
                                 (Easy3D::uchar)(g*255),
                                 (Easy3D::uchar)(b*255),
                                 255);
        }
        
	};

	template <class T>
	class Polynomial {
		
	public:

        
		std::vector< std::complex<T> > constants;
		std::vector< std::complex<T> > subconstants;
		std::vector< std::complex<T> > roots;
		std::vector< RootColor<T> > rootsColor;
        
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
            if(table.existsAsType("roots",Easy3D::Table::TABLE)){
                const Easy3D::Table& troots=table.getConstTable("roots");
                for(auto rt:troots){
                    Easy3D::Debug::doassert((rt.second->asType(Easy3D::Table::VECTOR2D)),"Easy3D::Table::VECTOR2D",__FILE__,__LINE__);
                    std::complex<T> root(rt.second->get<Easy3D::Vec2>().x,
                                         rt.second->get<Easy3D::Vec2>().y);
                    roots.push_back(root);
                }
            }
			else{
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
            if(table.existsAsType("rootsColors",Easy3D::Table::TABLE)){
                const Easy3D::Table& troots=table.getConstTable("rootsColors");
                for(auto rt:troots){
                    Easy3D::Debug::doassert((rt.second->asType(Easy3D::Table::VECTOR3D)),"Easy3D::Table::VECTOR3D",__FILE__,__LINE__);
                    rootsColor.push_back({rt.second->get<Easy3D::Vec3>().x,
                        rt.second->get<Easy3D::Vec3>().y, 
                        rt.second->get<Easy3D::Vec3>().z});
                }
            }
            
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