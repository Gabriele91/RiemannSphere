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
			ACTIVE
		};

                                     
        StateBotton state;
        Easy3D::String name;
        Easy3D::Texture *textures[3];
		std::function<void()> onClick;
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
		bool isCalled(const Easy3D::String& name);
		void addOnClick(const std::function<void()>& onClick){
			this->onClick=onClick;
		}
        Easy3D::Texture* getCurrentTexture(){
			return textures[state];
		}
		int getID(){
			return id;
		}
        
        void lock();
        void unlock();
        void update(RiemannMenu *menu,float dt);

	};

};

#endif