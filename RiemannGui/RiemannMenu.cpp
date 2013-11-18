#include <stdafx.h>
#include <Application.h>
#include <RiemannMenu.h>
#include <algorithm>

///////////////////////
using namespace RiemannGui;
using namespace Easy3D;
///////////////////////

void RiemannMenu::lock(){
    for(auto button:buttons) button->lock();
}
void RiemannMenu::unlock(){
    for(auto button:buttons) button->unlock();
}

RiemannMenu::RiemannMenu(const Table& config){
    //get default values
	sizeBottons=config.getVector2D("size",Vec2(50,50));
	maxSizeBottons=config.getVector2D("maxSize",sizeBottons);
	paddingBottons=config.getVector2D("padding",Vec2(25.0f,10.0f));
	pixelPerSecond=config.getVector2D("velocity",(maxSizeBottons-sizeBottons)*0.5);
	//get table buttons
	if(!config.existsAsType("buttons",Table::TABLE)){
			DEBUG_ASSERT_MSG(0,"RiemannMenu error : "
							   "buttons must to be a table");
	}
	//get buttons
	for(auto val: config.getConstTable("buttons")){
		if(!val.first.isString()){
			DEBUG_ASSERT_MSG(0,"RiemannMenu.buttons error : "
							   "must to be only associative table");
		}
		else
		if(val.second->asType(Table::TABLE)){
			RiemannButton* nsb=new RiemannButton(val.first.string(), val.second->get<Table>());
			nsb->setScale(Vec3(sizeBottons,1.0));
			buttons.push_back(nsb);
		}
		else{
			DEBUG_ASSERT_MSG(0,"RiemannMenu.buttons error : "+
								 (val.first.isString()?
								  val.first.string():
								  String::toString(val.first.integer()))+
								" isn't a table");
		}
	}
    
    //set vertical or horizontal
    //n.b. default is vertical
    String configtype=config.getString("type","bottom").toLower();
    if(configtype=="bottom")
        type=BOTTOM;
    else if(configtype=="top")
        type=TOP;
    else if(configtype=="left")
        type=LEFT;
    else if(configtype=="right")
        type=RIGHT;
    //
    
	//sort
	std::sort(buttons.begin(),buttons.end(),[](RiemannButton* lb,RiemannButton* rb)->bool{
		return lb->getID()<rb->getID();
	});
	//set position
	Vec2 realsize=sizeBottons+paddingBottons;
	Vec2 realsizeH=realsize*0.5;
    //size screen
    Vec2 sizeScreen(Application::instance()->getScreen()->getWidth(),
                    Application::instance()->getScreen()->getHeight());

    if(type==BOTTOM||type==TOP){
        //calc center
        Vec2 menucenter(sizeScreen.x*0.5,
                        type==BOTTOM ? realsize.y*0.5 : sizeScreen.y-realsize.y*0.5);
        //pos buttons
        Vec2 leftPointer(menucenter);
        Vec2 rightPointer(menucenter);
        int leftS=int(buttons.size()/2-1);
        int rightS=int(buttons.size()/2);
        //center
        if(buttons.size()%2){
            buttons[rightS]->setPosition(Vec3(menucenter,0.0));
            leftPointer.x-=realsize.x;
            rightPointer.x+=realsize.x;
            ++rightS;
        }
        else{
            leftPointer.x-=realsizeH.x;
            rightPointer.x+=realsizeH.x;
        }
        //left
        for(int x1=leftS;x1>=0;--x1){
            buttons[x1]->setPosition(Vec3(leftPointer,0.0));
            leftPointer.x-=realsize.x;
        }
        //right
        for(int x2=rightS;x2<buttons.size();++x2){
            buttons[x2]->setPosition(Vec3(rightPointer,0.0));
            rightPointer.x+=realsize.x;
        }
    }
    else if(type==LEFT||type==RIGHT){
        //calc center
        Vec2 menucenter(type==LEFT ? realsize.x*0.5 : sizeScreen.x-realsize.x*0.5,
                        sizeScreen.y*0.5);
        //pos buttons
        Vec2 topPointer(menucenter);
        Vec2 bottomPointer(menucenter);
        int topS=int(buttons.size()/2-1);
        int bottomS=int(buttons.size()/2);
        //cernter
        if(buttons.size()%2){
            buttons[bottomS]->setPosition(Vec3(menucenter,0.0));
            topPointer.y+=realsize.y;
            bottomPointer.y-=realsize.y;
            ++bottomS;
        }
        else{
            topPointer.y+=realsizeH.y;
            bottomPointer.y-=realsizeH.y;
        }
        //top
        for(int y1=topS;y1>=0;--y1){
            buttons[y1]->setPosition(Vec3(topPointer,0.0));
            topPointer.y+=realsize.y;
        }
        //bottom
        for(int y2=bottomS;y2<buttons.size();++y2){
            buttons[y2]->setPosition(Vec3(bottomPointer,0.0));
            bottomPointer.y-=realsize.y;
        }
    }
}
RiemannMenu::~RiemannMenu(){
	for(auto button:buttons)
		delete button;
}

bool RiemannMenu::addOnClick(const String& name,const std::function<void(bool)>& onClick){
	for(auto button:buttons){
		if(button->isCalled(name)){
			button->addOnClick(onClick);
			return true;
		}
	}
	return false;
}
void RiemannMenu::update(float dt){
	for(auto button:buttons)
		button->update(this,dt);
}
void RiemannMenu::draw(Easy3D::Render* render){
	///////////////////////////////////////////////////////////////
    //save states
	//save view port
    Render::ViewportState globalViewport=render->getViewportState();
    //save blend
	Render::BlendState blendState;
	blendState=render->getBlendState();
    //save matrix
	Render::MatrixsState matrixsState;
	render->getMatrixsState(matrixsState);
    //save zbuffer mode
    Render::ZBufferState zbufferState=render->getZBufferState();
    //save client state
    Render::ClientState clientState=render->getClientState();
	///////////////////////////////////////////////////////////////
    //disable z buffer
	render->setZBufferState(false);
	//query
	Screen *screen=Application::instance()->getScreen();
	Vec2 windowSize(screen->getWidth(),screen->getHeight());
	///////////////////////////////////////////////////////////////
    //2D MODE
	//disable shader
    glUseProgram(0);
	//mode (2d)
    render->setViewportState(Vec4(0,0,windowSize.x,windowSize.y));
    //ortogonal
	Matrix4x4 projection;
	projection.setOrtho(0,windowSize.x,0,windowSize.y,0.0,1.0);
    render->setMatrixsState(Render::MatrixsState(projection,Matrix4x4()));
	///////////////////////////////////////////////////////////////
    //set client
    render->setClientState(Render::ClientState::VERTEX|Render::ClientState::UVMAP);
    //enable texture
    render->setTextureState(Render::TextureState::TEXTURE2D);
	//draw
	static const float //back face culling
	xyUV[]={
             0.5,-0.5,1.0,1.0,
             0.5, 0.5,1.0,0.0,
            -0.5,-0.5,0.0,1.0,
            -0.5, 0.5,0.0,0.0,
	        };
    
	//disable vbo
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	for(auto button:buttons){
		//matrix button
		glLoadMatrixf(button->getGlobalMatrix());
		//texture
		button->getCurrentTexture()->bind();
		//set vertex
		glVertexPointer(  2, GL_FLOAT, sizeof(float)*4,  &xyUV[0]);
		glTexCoordPointer(2, GL_FLOAT, sizeof(float)*4,  &xyUV[2]);
		//draw
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	//restore state
	render->setZBufferState(zbufferState);
	render->setBlendState(blendState);
	render->setMatrixsState(matrixsState);
    render->setClientState(clientState);
    render->setViewportState(globalViewport);


}
