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
	};

	template <class T>
	class Polynomial {
		
	public:


		std::vector<T> constants;
		std::vector< std::complex<T> > roots;
		std::vector< RootColor<T> > rootsColor;

		Polynomial(const Easy3D::Table& table){

			if(table.getDeserializeErros().size()){
				Debug::message()<<"Polynomial table:\n"<<table.getDeserializeErros();
			}

			if(table.existsAsType("constants",Easy3D::Table::TABLE)){
				const Table& tconstants=table.getConstTable("constants");
				for(auto cns:tconstants){
					DEBUG_ASSERT(cns.second->asType(Easy3D::Table::FLOAT));
					constants.push_back( (T)cns.second->get<float>() );
				}
			}
			if(table.existsAsType("roots",Easy3D::Table::TABLE)){
				const Table& troots=table.getConstTable("roots");
				for(auto rt:troots){
					DEBUG_ASSERT(rt.second->asType(Easy3D::Table::VECTOR2D));
					std::complex<T> root(rt.second->get<Vec2>().x,
										 rt.second->get<Vec2>().y);
					roots.push_back(root);
				}
			}
			if(table.existsAsType("rootsColors",Easy3D::Table::TABLE)){
				const Table& troots=table.getConstTable("rootsColors");
				for(auto rt:troots){
					DEBUG_ASSERT(rt.second->asType(Easy3D::Table::VECTOR3D));
					rootsColor.push_back({rt.second->get<Vec3>().x, 
										  rt.second->get<Vec3>().y, 
										  rt.second->get<Vec3>().z});
				}
			}

		}

	};

};

#endif