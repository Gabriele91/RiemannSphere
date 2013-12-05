#ifndef RIEMANNDIALOG_H
#define RIEMANNDIALOG_H

#include <Easy3D.h>
#include <RiemannScale9.h>

namespace RiemannGui {

	class RiemannDialog {
         //dialog 
		 bool isshow;

		 //text
		 Easy3D::Font *font;
		 Easy3D::String text;
		 Easy3D::Color  textColor;

         //box
         RiemannScale9 box;
         Easy3D::Vec2  mbox,sbox,offset;
         Easy3D::Vec2  sizebox;
        Easy3D::Color backgoundColor;
        //
        std::function<void(bool)> callback;

	public:
        
		RiemannDialog(const Easy3D::Table& config);
		void setText(const Easy3D::String& text);
        
        void setCallack(std::function<void(bool)> callback){
            this->callback=callback;
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