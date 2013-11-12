#ifndef RIEMANNMENU_H
#define RIEMANNMENU_H

#include <Easy3D.h>
#include <RiemannButton.h>

namespace RiemannGui {

	class RiemannMenu {

		std::vector< RiemannButton* > buttons;
        Easy3D::Vec2 sizeBottons;
		Easy3D::Vec2 maxSizeBottons;
		Easy3D::Vec2 paddingBottons;
		Easy3D::Vec2 pixelPerSecond;
        //menu type
        enum MType{
            HORIZONTA,
            VERTICAL
        };
        //value type
        MType type;

	public:
		
		RiemannMenu(const Easy3D::Table& config);
		virtual ~RiemannMenu();
		bool addOnClick(const Easy3D::String& name,const std::function<void()>& onClick);
		void draw(Easy3D::Render* render);
		void update(float dt);
        void lock();
        void unlock();
		const Easy3D::Vec2& getPadding() const{
			return paddingBottons;
		}
		const Easy3D::Vec2& getSize() const{
			return sizeBottons;
		}
		const Easy3D::Vec2& getPixelPerSecond() const{
			return pixelPerSecond;
		}
		const Easy3D::Vec2& getMaxSize() const{
			return maxSizeBottons;
		}

	};

};

#endif