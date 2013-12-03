#include <stdafx.h>
#include <RiemannButton.h>
#include <RiemannMenu.h>
#include <Application.h>
#include <Debug.h>

///////////////////////
using namespace RiemannGui;
using namespace Easy3D;
///////////////////////

void RiemannButton::lock(){
    //delete handler input
    if(!islocked)
        Application::instance()->getInput()->removeHandler((Input::MouseHandler*)this);
    islocked=true;
}
void RiemannButton::unlock(){
    //add input
    if(islocked)
        Application::instance()->getInput()->addHandler((Input::MouseHandler*)this);
    islocked=false;
}

RiemannButton::RiemannButton(const String& name,const Table& config):state(NORMAL),name(name),onClick([](bool){}){
    
    mode=SINGLE;
    
    //image table
    for(int i=0;i!=2;++i){
        Table* images=NULL;
        if(i==0){
            if(!config.existsAsType("single",Table::TABLE)){
                DEBUG_ASSERT_MSG(0,"RiemannButton error : "+name+" must to be setted single params");
            }
            images=(Table*)&config.getConstTable("single");
        }
        
        if(i==1){
            if(!config.existsAsType("double",Table::TABLE)){
                textures[i][NORMAL]=NULL;
                textures[i][OVERED]=NULL;
                textures[i][ACTIVE]=NULL;
                break;
            }
          images=(Table*)&config.getConstTable("double");
        }
        //chack textures parameters exists
        if(!images->existsAsType("normal",Table::STRING)){
                DEBUG_ASSERT_MSG(0,"RiemannButton error : "+name+" must to be setted normal params");
        }
        //load textures
        //load normal texture
        textures[i][NORMAL]=new Texture(images->getTablePath().getDirectory()+"/"+images->getString("normal"));
        //load overed texture
        if(images->existsAsType("overed",Table::STRING))
            textures[i][OVERED]=new Texture(images->getTablePath().getDirectory()+"/"+images->getString("overed"));
        else
            textures[i][OVERED]=NULL;
        //load active texture
        if(images->existsAsType("active",Table::STRING))
            textures[i][ACTIVE]=new Texture(images->getTablePath().getDirectory()+"/"+images->getString("active"));
        else
            textures[i][ACTIVE]=NULL;
    }
	//get id
	id=(int)config.getFloat("id",0.0f);
	//add handler input
	Application::instance()->getInput()->addHandler((Input::MouseHandler*)this);
    islocked=false;

}
RiemannButton::~RiemannButton(){
        for(auto texture:textures[0])
            if(texture)
                delete texture;
    
        for(auto texture:textures[1])
            if(texture)
                delete texture;
		//delete handler input
        lock();
}

bool RiemannButton::mouseInBox(const Vec2& mouse){
	Vec2 vmouse(mouse.x,Application::instance()->getScreen()->getHeight()-mouse.y);
	Vec2 a(getPosition().xy()-getScale().xy()*0.5);
	Vec2 b(getPosition().xy()+getScale().xy()*0.5);
	bool bx= a.x<=vmouse.x && vmouse.x<=b.x;
	bool by= a.y<=vmouse.y && vmouse.y<=b.y;
	return bx && by;
}
void RiemannButton::onMouseMove(Vec2 mousePosition) {
    if(mouseInBox(mousePosition)) {
        if(state==NORMAL)
            state=OVERED;
    }
    else
        state=NORMAL;
}

Easy3D::Texture* RiemannButton::getCurrentTexture(){

    if (textures[mode][state])
        return textures[mode][state];
    return textures[mode][0];
}

void RiemannButton::onMouseDown(Vec2 mousePosition, Key::Mouse button) {
        onMousePress(mousePosition,button);
}
void RiemannButton::onMousePress(Vec2 mousePosition, Key::Mouse button) {
    if(mouseInBox(mousePosition)){
        if(state==OVERED) state=ACTIVE;
    }
    else
        state=NORMAL;
}

void RiemannButton::crack(){
    if(textures[DOUBLE][0]){
        if(mode==SINGLE){
            mode=DOUBLE;
            onClick(true);
        }
        else if(mode==DOUBLE){
            mode=SINGLE;
            onClick(false);
        }
    }else
        onClick(false);
    //double to single and viceversa
    //new state
    state=OVERED;
}
void RiemannButton::changeMode(){
    if(textures[DOUBLE][0]){
        if(mode==SINGLE)
            mode=DOUBLE;
        else if(mode==DOUBLE)
            mode=SINGLE;
    }
}

void RiemannButton::onMouseRelease(Vec2 mousePosition, Key::Mouse button) {
    if(mouseInBox(mousePosition)){
        if(state==ACTIVE)
            crack();
    }
    else
        state=NORMAL;
}
void RiemannButton::onMouseScroll(short scrollDelta) {}

void RiemannButton::update(RiemannMenu *menu,float dt){
	Vec2 animVel=menu->getPixelPerSecond();
    if(state==NORMAL){
        //animation
        if(getScale().x>menu->getSize().x)
            setScale(Vec3(getScale().x-animVel.x*dt,getScale().y,1.0));
        if(getScale().y>menu->getSize().y)
            setScale(Vec3(getScale().x,getScale().y-animVel.y*dt,1.0));
        //limit
        if(getScale().x<menu->getSize().x)
            setScale(Vec3(menu->getSize().x,getScale().y,1.0));
        if(getScale().y<menu->getSize().y)
            setScale(Vec3(getScale().x,menu->getSize().y,1.0));
        
    }
    else if(state==OVERED||state==ACTIVE){
        //animation
        if(getScale().x<menu->getMaxSize().x)
            setScale(Vec3(getScale().x+animVel.x*dt,getScale().y,1.0));
        if(getScale().y<menu->getMaxSize().y)
            setScale(Vec3(getScale().x,getScale().y+animVel.y*dt,1.0));
        //limit
        if(getScale().y>menu->getMaxSize().x)
            setScale(Vec3(menu->getMaxSize().x,getScale().y,1.0));	
        if(getScale().y>menu->getMaxSize().y)
            setScale(Vec3(getScale().x,menu->getMaxSize().y,1.0));	
        
    }
}

bool RiemannButton::isCalled(const String& name) const{
		return this->name==name;
}