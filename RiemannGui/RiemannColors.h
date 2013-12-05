#ifndef RIEMANNCOLORS_H
#define RIEMANNCOLORS_H

#include <Easy3D.h>
#include <RiemannScale9.h>
#include <RiemannColors.h>

namespace RiemannGui {

	class RiemannColors : public Easy3D::Input::MouseHandler {
         //dialog 
		 bool isshow;
         //color box
         Easy3D::Texture texture;
         float radius;
         float scale;
         //pointer texture
         Easy3D::Texture pointer;
         Easy3D::Vec2 pointerScale;
         std::vector<Easy3D::Color> colors;
         int idCS;
         //box
         RiemannScale9 box;
         Easy3D::Vec2  mbox,sbox,offset;
         Easy3D::Vec2  sizebox;
         Easy3D::Color backgoundColor;
         //
         std::function<void(bool)> callback;
         //events
        //mouse
        virtual void onMousePress(Easy3D::Vec2 mousePosition, Easy3D::Key::Mouse button);
        virtual void onMouseDown(Easy3D::Vec2 mousePosition, Easy3D::Key::Mouse button);
        virtual void onMouseRelease(Easy3D::Vec2 mousePosition, Easy3D::Key::Mouse button);

	public:
        
		RiemannColors(const Easy3D::Table& config);
        ~RiemannColors();

        void addColor(const Easy3D::Color& color){
            colors.push_back(color);
        }
        void clearColors(){
            colors.clear();
        }
        
        void setCallack(std::function<void(bool)> callback){
            this->callback=callback;
        }
        
        const std::vector<Easy3D::Color>& getColors(){
            return colors;
        }
        
		void show(){
			isshow=true;
            callback(true);
		}
		void hide(){
			isshow=false;
            callback(false);
		}
		bool isShow(){
			return isshow;
		}
		bool isHide(){
			return !isshow;
		}

        void draw(Easy3D::Render* render);
        

	};

};

#endif