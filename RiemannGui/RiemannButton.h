#ifndef RIEMANNBUTTON_H
#define RIEMANNBUTTON_H

#include <functional>
#include <Easy3D.h>

namespace RiemannGui {
	//dad
	class RiemannMenu;
	//childs
	class RiemannButton : public Easy3D::Object,
							     Easy3D::Input::MouseHandler{
		
		enum StateBotton{
			NORMAL=0,
			OVERED,
			ACTIVE,
            OVERED_ACTIVE,
		};
        
        enum ModeBottom{
            SINGLE,
            DOUBLE
        };
          
        ModeBottom mode;
        StateBotton state;
        Easy3D::String name;
        Easy3D::Texture *textures[2][3];
		std::function<void(bool)> onClick;
        bool islocked;
		int id;

		//mouse in box
        bool mouseInBox(const Easy3D::Vec2& mouse);

		//mouse
		virtual void onMouseMove(Easy3D::Vec2 mousePosition) ;
		virtual void onMousePress(Easy3D::Vec2 mousePosition, Easy3D::Key::Mouse button);
		virtual void onMouseDown(Easy3D::Vec2 mousePosition, Easy3D::Key::Mouse button);
		virtual void onMouseRelease(Easy3D::Vec2 mousePosition, Easy3D::Key::Mouse button);
		virtual void onMouseScroll(short scrollDelta);
        

	public:

		RiemannButton(const Easy3D::String& name,const Easy3D::Table& config);
		virtual ~RiemannButton();
		bool isCalled(const Easy3D::String& name) const;
        const Easy3D::String& getName() const{
         return name;
        }
		void addOnClick(const std::function<void(bool)>& onClick){
			this->onClick=onClick;
		}
        void crack();
        void changeMode();
        bool isDoubleMode(){
            return textures[DOUBLE][0]!=NULL;
        }
        Easy3D::Texture* getCurrentTexture();
		int getID(){
			return id;
		}
        
        void lock();
        void unlock();
        void update(RiemannMenu *menu,float dt);

	};

};

#endif