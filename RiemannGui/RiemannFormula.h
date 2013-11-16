#ifndef RIEMANNFORMULA_H
#define RIEMANNFORMULA_H

#include <Easy3D.h>
#include <RiemannScale9.h>

namespace RiemannGui {

	class RiemannFormula : public Easy3D::Input::MouseHandler,
                           public Easy3D::Input::KeyboardHandler{
        /* TEXT */
        Easy3D::Font *font;
        Easy3D::String text;
        bool showpointer;
        int textid,textidselect;
        Easy3D::Vec2  textSize;
        Easy3D::Vec2  textPos;
        Easy3D::Color textColor;
        Easy3D::Color selectColor;
        Easy3D::Vec2  lastPointSelect;
        Easy3D::Vec2  textOffest;
        //utility
        void calcTextSize();
        void recalcTextOffset();
        //types
        enum VerticalType{
            TOP,
            BOTTOM
        };
        enum HorizontalType{
            LEFT,
            CENTER,
            RIGHT
        };
        //
        VerticalType vertical;
        HorizontalType horizontal;
        //
        //keyboard
        virtual void onKeyPress(Easy3D::Key::Keyboard key);
        virtual void onKeyRelease(Easy3D::Key::Keyboard key);
        virtual void onKeyDown(Easy3D::Key::Keyboard key);
                               
        //mouse
        virtual void onMousePress(Easy3D::Vec2 mousePosition, Easy3D::Key::Mouse button);
        virtual void onMouseDown(Easy3D::Vec2 mousePosition, Easy3D::Key::Mouse button);
        virtual void onMouseRelease(Easy3D::Vec2 mousePosition, Easy3D::Key::Mouse button);
                               
        //filter
        std::function<bool(char c)> filter;
                               
        /* BOX */
        RiemannScale9 box;
        Easy3D::Vec2  mbox,sbox,offset;
        Easy3D::Vec2  sizebox;
                               
	public:

		RiemannFormula(const Easy3D::Table& config);
        ~RiemannFormula();
        void setFilter(std::function<bool(char c)>& filter);
        void draw(Easy3D::Render* render);
        const Easy3D::String& getText() const{
            return text;
        }
        void setText(const Easy3D::String& text);

	};

};

#endif