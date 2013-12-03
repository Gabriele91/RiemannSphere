#ifndef RIEMANNCOLORS_H
#define RIEMANNCOLORS_H

#include <Easy3D.h>
#include <RiemannScale9.h>
#include <RiemannColors.h>

namespace RiemannGui {

	class RiemannColors {
         //dialog 
		 bool isshow;
        
         //color box
        Easy3D::Texture texture;
        float radius;
        float scale;
         //box
         RiemannScale9 box;
         Easy3D::Vec2  mbox,sbox,offset;
         Easy3D::Vec2  sizebox;
         Easy3D::Color backgoundColor;

	public:
        
		RiemannColors(const Easy3D::Table& config);

		void show(){
			isshow=true;
		}
		void hide(){
			isshow=false;
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