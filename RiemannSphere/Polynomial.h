#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <Easy3D.h>
#include <complex>

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
        
	};

	template <class T>
	class Polynomial {
		
	public:


		std::vector<T> constants;
		std::vector< std::complex<T> > roots;
		std::vector< RootColor<T> > rootsColor;
        
        Polynomial(const Easy3D::Table& table){
            
            if(table.getDeserializeErros().size()){
                Easy3D::Debug::message()<<"Polynomial table:\n"<<table.getDeserializeErros();
            }
            
            if(table.existsAsType("constants",Easy3D::Table::TABLE)){
                const Easy3D::Table& tconstants=table.getConstTable("constants");
                for(auto cns:tconstants){
                    Easy3D::Debug::doassert((cns.second->asType(Easy3D::Table::FLOAT)),"Easy3D::Table::FLOAT",__FILE__,__LINE__);
                    constants.push_back( (T)cns.second->get<float>() );
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

	};

};

#endif