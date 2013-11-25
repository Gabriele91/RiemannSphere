#include <stdafx.h>
#include <RiemannFormula.h>

///////////////////////
using namespace Easy3D;
using namespace RiemannGui;
///////////////////////

RiemannFormula::RiemannFormula(const Easy3D::Table& config){
    //font table
    Easy3D::String fontpath=config.getTablePath().getDirectory()+"/"+config.getString("font");
    font=new Easy3D::Font(fontpath);
    textColor=config.getVector4D("textColor",Vec4(Vec3::ZERO,1.0f));
    selectColor=config.getVector4D("selectColor",Vec4(0.0f,0.0f,0.5f,0.5f));
    //get table
    auto boxConfig=config.getConstTable("box");
    //image path
    Easy3D::String imagepath=config.getTablePath().getDirectory()+"/"+boxConfig.getString("image");
    //build vbo and texture
    box.build(imagepath,
              boxConfig.getVector2D("mbox"),
              boxConfig.getVector2D("sbox"),
              boxConfig.getVector2D("offset"));
    //sizebox
    sizebox=boxConfig.getVector2D("mbox")+ boxConfig.getVector2D("sbox");
    mbox=boxConfig.getVector2D("mbox");
    sbox=boxConfig.getVector2D("sbox");
    offset=boxConfig.getVector2D("offset");
    /////////////////////////////////////////////////////////////////////////////////////
    //set vertical
    vertical=boxConfig.getString("vertical","bottom")=="top" ? TOP : BOTTOM;
    //set vertical
    horizontal=CENTER; //default
    String infohorizontal=boxConfig.getString("horizontal","center");
    //get left
    if(infohorizontal=="left")
        horizontal=LEFT;
    //else is right
    else if(infohorizontal=="right")
        horizontal=RIGHT;
    /////////////////////////////////////////////////////////////////////////////////////
    //set input
    Application::instance()->getInput()->addHandler((Easy3D::Input::KeyboardHandler*)this);
    Application::instance()->getInput()->addHandler((Easy3D::Input::MouseHandler*)this);

    //setup logic
    text="";
    textid=0;
    showpointer=false;
    textid=0;
    textidselect=0;
    dbclick.start();
    calcTextSize();
	//default focus event
	focus=[](bool){};
	//set default filter
    filter=[](char cin)->bool{
        return
         ( 0<=(cin-'0') && (cin-'0')<10 ) ||//numbers
         cin=='.'||
         cin=='e'||
         cin=='E'||
         cin=='-'||
         cin=='+'||
         cin=='z'||
         cin=='^'||
         cin=='('||
         cin==')'||
         cin=='i';
    };
}
void RiemannFormula::setFilter(std::function<bool(char c)> filter){
    this->filter=filter;
}
void RiemannFormula::setFocusEvent(std::function<void(bool c)> focus){
    this->focus=focus;
}

RiemannFormula::~RiemannFormula(){
    //delete input
    Application::instance()->getInput()->removeHandler((Easy3D::Input::KeyboardHandler*)this);
    Application::instance()->getInput()->removeHandler((Easy3D::Input::MouseHandler*)this);
    //delete font
    delete font;
}
//utility
void RiemannFormula::calcTextSize(){
    textSize=font->sizeText(text)*Vec2(1.0,-0.5);
}
void RiemannFormula::recalcTextOffset(){
    if( textSize.x>mbox.x )
    {
        //min is 0
        textOffest.x=textOffest.x > 0 ? 0:textOffest.x;
        //max is text size(- size box)
        textOffest.x=textOffest.x<(-textSize.x+mbox.x)?(-textSize.x+mbox.x):textOffest.x;
    }
    else
        textOffest.x=0;
}
void RiemannFormula::recalcPointerTextOffset(){
    //offset
    if( textSize.x>mbox.x ){
        Vec2 pointerPos=font->endChar(text, textid);
        //left
        if((pointerPos.x+textOffest.x) < 0)
            textOffest.x+=font->size()*4;
        //right
        if((pointerPos.x+textOffest.x) > mbox.x)
            textOffest.x-=font->size()*4;
    }
    //recalc
    recalcTextOffset();
}
void RiemannFormula::showPointer(){
	if(!showpointer){
		showpointer= true;
		focus(true);
	}
}
void  RiemannFormula::hidePointer(){
	if(showpointer){
		showpointer=false;
		focus(false);
	}
}
//keyboard
void RiemannFormula::onKeyPress(Easy3D::Key::Keyboard key){
#define CTRL_LEFT_OR_RIGHT\
	(Application::instance()->getInput()->getKeyDown(Key::RCTRL) ||\
	 Application::instance()->getInput()->getKeyDown(Key::LCTRL)) 
    if(!showpointer) return;
    //pointer to left
    if(key==Easy3D::Key::LEFT){
        if(textid==textidselect)
            --textid;
        else if(textid<textidselect)
            textidselect=textid;
        else if(textid>textidselect)
            textid=textidselect;
        //offset
        if( textSize.x>mbox.x ){
            Vec2 pointerPos=font->endChar(text, textid);
            //left
            if((pointerPos.x+textOffest.x) < 0){
                textOffest.x+=font->size()*4;
                recalcTextOffset();
            }
        }
    }
    //pointer to right
    else if(key==Easy3D::Key::RIGHT){
        if(textid==textidselect)
            ++textid;
        else if(textid>textidselect)
            textidselect=textid;
        else if(textid<textidselect)
            textid=textidselect;
        //offset
        if( textSize.x>mbox.x ){
            Vec2 pointerPos=font->endChar(text, textid);
            //right
            if((pointerPos.x+textOffest.x) > mbox.x){
                textOffest.x-=font->size()*4;
                recalcTextOffset();
            }
        }
    }
    //copy or cut
    else if(Key::list(key, Key::C, Key::X)&&CTRL_LEFT_OR_RIGHT){
        //delete substring
        int minselect= textidselect>textid? textid: textidselect;
        int maxselect= textidselect<textid? textid: textidselect;
        //
        Application::instance()->getInput()->copyString(text.substr(minselect,maxselect-minselect));
        //is a cut command
        if(key==Key::X){
            //delete substring
            text.erase(minselect, maxselect-minselect);
            //recalc text size and offset
            calcTextSize();
            recalcPointerTextOffset();
        }
        else //NO FIX POINT (no change, only copy)
            return;
    }
    //past
    else if(Key::V==key&&CTRL_LEFT_OR_RIGHT){
        //get input string
        String input=Application::instance()->getInput()->pasteString();
        //delete substring
        int minselect= textidselect>textid? textid: textidselect;
        int maxselect= textidselect<textid? textid: textidselect;
        text.erase(minselect, maxselect-minselect);
        textid=minselect;
        //add chars
        for(auto cin:input){
            if(filter(cin)){
                text=text.insert(textid, String::toString(cin));
                ++textid;
            }
        }
        //recalc text size and offset
        calcTextSize();
        recalcPointerTextOffset();
        //recalc pos pointer
        if( textSize.x>mbox.x ){
            //get pos pinter into the text
            Vec2 pointerPos=font->endChar(text, textid);
            //right offset recalc
            if((pointerPos.x+textOffest.x) > mbox.x){
                textOffest.x=-textSize.x;
                recalcTextOffset();
            }
        }
    }
    //insert a key or delete a key
    else if(Key::BACKSPACE==key||filter(Application::instance()->getInput()->getInputString()[0])){
        //delete substring
        int minselect= textidselect>textid? textid: textidselect;
        int maxselect= textidselect<textid? textid: textidselect;
        text.erase(minselect, maxselect-minselect);
        textid=minselect;
        //add chars
        for(auto cin:Application::instance()->getInput()->getInputString()){
            if(filter(cin)){
                text=text.insert(textid, String::toString(cin));
                ++textid;
            }
        }
        //delete a char
        if(Key::BACKSPACE==key&& //delete key
           textid&&              //is not 0
           minselect==maxselect  //no selection
           ){
            --textid;
            text.erase(textid,1);
        }
        //recalc text size and offset
        calcTextSize();
        recalcPointerTextOffset();
    }
    //NO EVENT
    else return;
    //FIX POINT
    textidselect=textid=textid<0?0:textid;
    textidselect=textid=textid<text.size()? textid : (int)(text.size());
}
void RiemannFormula::onKeyRelease(Easy3D::Key::Keyboard key) {}
void RiemannFormula::onKeyDown(Easy3D::Key::Keyboard key) {}
//mouse
void RiemannFormula::onMousePress(Easy3D::Vec2 mousePosition, Easy3D::Key::Mouse button) {
    if(Easy3D::Key::BUTTON_LEFT==button){
        //disable all text selection
        alltext=false;
        //calc end point
        Vec2 textAreaEndPoint=textPos+mbox;
        //in text area box
        bool inTextAreaBox=(textPos.x          <mousePosition.x &&
                            textAreaEndPoint.x >mousePosition.x &&
                            textPos.y          <mousePosition.y &&
                            textAreaEndPoint.y >mousePosition.y);
        
        
        if(!inTextAreaBox){
			//disable visiable pointer
			hidePointer();
            //reset slection
            textidselect=textid;
            return;
        }
		else 
			//enable visiable pointer
			showPointer();
        
        //double click?
        if(dbclick.getGetCounter()<400){
            textid=0;
            textidselect=(int)text.size();
            alltext=true;
            lastPointSelect=mousePosition;
            dbclick.reset();
            return;
        }
        else{
            //reset
            dbclick.reset();
        }
        
        //single click
        Vec2 mposition( mousePosition.x-textPos.x,
                       -mousePosition.y+textPos.y);
        
        int selectid=font->pointChar(text, mposition-textOffest);
        if(selectid>-1){
            textid=selectid;
            lastPointSelect=mousePosition;
        }
		else{
			textid=(int)text.size();
		}
        //reset selection area
        textidselect=textid;
    }
}
void RiemannFormula::onMouseDown(Easy3D::Vec2 mousePosition, Easy3D::Key::Mouse button) {
    if(Easy3D::Key::BUTTON_LEFT==button && showpointer && !alltext){
        //mpos
        Vec2 mposition( mousePosition.x-textPos.x,
                       -mousePosition.y+textPos.y);
        //end text point
        Vec2 textEndPoint=textPos+textSize;
        Vec2 textAreaEndPoint=textPos+mbox;
        //in text box
        bool inTextBox=(textPos.x      <mousePosition.x &&
                        textEndPoint.x >mousePosition.x &&
                        textPos.y      <mousePosition.y &&
                        textEndPoint.y >mousePosition.y);
        //in text area box
        bool inTextAreaBox=(textPos.x          <mousePosition.x &&
                            textAreaEndPoint.x >mousePosition.x &&
                            textPos.y          <mousePosition.y &&
                            textAreaEndPoint.y >mousePosition.y);
        //select
        int selectid=font->pointChar(text, mposition-textOffest);
        if(selectid>-1){
                textidselect=selectid;
        }
        else if(!inTextBox){
            if(lastPointSelect.x>mousePosition.x)
                textidselect=0;
            else if(lastPointSelect.x<mousePosition.x)
                textidselect=(int)text.size();
        }
        /* change offset */
        if( !inTextAreaBox && textSize.x>mbox.x ){
            if(mousePosition.x<textPos.x)
                textOffest.x+=(textPos.x-mousePosition.x)*5*Application::instance()->getLastDeltaTime();
            else if(mousePosition.x>textAreaEndPoint.x)
                textOffest.x+=(textAreaEndPoint.x-mousePosition.x)*5*Application::instance()->getLastDeltaTime();
            //recalc offset
            recalcTextOffset();
        }
    }
}
void RiemannFormula::onMouseRelease(Easy3D::Vec2 mousePosition, Easy3D::Key::Mouse button) {
    
}
//text
void RiemannFormula::setText(const Easy3D::String& intext){
    //text=void
    text="";
    //add chars
    for(auto cin:intext){
        if(filter(cin))
            text+=cin;
    }
    //reset
    textidselect=textid=0;
    showpointer=false;
    textOffest=Vec2::ZERO;
    //recalc text size and offset
    calcTextSize();
    //recalcTextOffset();
}
//to draw
void RiemannFormula::draw(Easy3D::Render* render){
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
    //glUseProgram(0);
	//mode (2d)
    render->setViewportState(Vec4(0,0,windowSize.x,windowSize.y));
    //ortogonal
	Mat4 projection;
	projection.setOrtho(0,windowSize.x,0,windowSize.y,0.0,1.0);
    //set pos
    Mat4 model;
    Vec3 pos;
    //////////////////////////
    //vertical
    if(vertical==TOP)
        pos.y=windowSize.y-sizebox.y;
    else if(vertical==BOTTOM)
        pos.y=0;
    //horizontal
    if(horizontal==CENTER)
        pos.x=windowSize.x*0.5-sizebox.x*0.5;
    else if(horizontal==LEFT)
        pos.x=0;
    else if(horizontal==RIGHT)
        pos.x=windowSize.x-sizebox.x;
    //////////////////////////
    model.setTranslation(pos);
    //set matrixs
    render->setMatrixsState(Render::MatrixsState(projection,model));
	///////////////////////////////////////////////////////////////
    //set client
    render->setClientState(Render::ClientState::VERTEX|Render::ClientState::UVMAP);
    //enable texture
    render->setTextureState(Render::TextureState::TEXTURE2D);
    //draw scale 9
    box.draw();
    ////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////
    //draw formula
    textPos=Vec2(pos.x+sbox.x+offset.x,windowSize.y-(pos.y+mbox.y+sbox.y+offset.y)); //no viewport
    Vec4 viewportText(pos.x+sbox.x+offset.x,pos.y+sbox.y+offset.y,mbox.x,mbox.y);
    render->setViewportState(viewportText); //viewport
    //set matrix
	projection.setOrtho(0,mbox.x,0,mbox.y,0.0,1.0);
    model.setTranslation(Vec2(textOffest.x,textOffest.y+mbox.y));
    render->setMatrixsState(Render::MatrixsState(projection,model));
    //draw
    font->text(text,textColor);
    
    if(showpointer){
        //set pos
        Vec2 scalePointer(1,font->size()*0.5);
        Vec2 posPointer=font->endChar(text, textid)+textOffest;
        Vec2 offsetPointer(-scalePointer.x*0.5,mbox.y-scalePointer.y);
        //set client
        render->setClientState(Render::ClientState::VERTEX);
        //disable texture
        render->setTextureState(Render::TextureState::NONE);
        //draw pointer
        if(textidselect==textid){
            //reset viewport and projection
            //ortogonal
            Mat4 projection;
            projection.setOrtho(0,windowSize.x,0,windowSize.y,0.0,1.0);
            //set viewport
            render->setViewportState(Vec4(0,0,windowSize.x,windowSize.y));
            //calc pos
            Vec2 realPosPointer(viewportText.x+posPointer.x+offsetPointer.x,
                                viewportText.y+posPointer.y+offsetPointer.y);
            //update model
            model.setTranslation(realPosPointer);
            model.addScale(scalePointer);
            //set matrixs
            render->setMatrixsState(Render::MatrixsState(projection,model));
            //render
            render->drawColorSprite(textColor);
        }
        //or selection
        else
        {
            //calc first pos
            Vec3 posFinalPointer(posPointer.x+offsetPointer.x,
                                 posPointer.y+offsetPointer.y,
                                 0.0);
            //pos pointer in the text
            Vec2 posPointer=font->endChar(text, textidselect)+textOffest;;
            //calc final pos
            Vec3 posFinal(posPointer.x+offsetPointer.x,
                          posPointer.y+offsetPointer.y,
                          0.0);
            //vertexs default left
            float vertices[8]={
                posFinalPointer.x,       -scalePointer.y+posFinal.y,
                posFinalPointer.x,        scalePointer.y+posFinal.y,
                posFinal.x,              -scalePointer.y+posFinal.y,
                posFinal.x,               scalePointer.y+posFinal.y
            };
            //vertexs right //back face culling
            if(textidselect>textid){
                Math::swap(vertices[2], vertices[4]);
                Math::swap(vertices[3], vertices[5]);
            }
            //client state
            render->setClientState(Render::ClientState::VERTEX);
            //save color
            Render::ColorState lastColorState=render->getColorState();
            //void matrix
            glLoadMatrixf(Mat4());
            //draw
            render->setColorState(selectColor);
            //unbind VBO
            glBindBuffer( GL_ARRAY_BUFFER, 0 );
            //pointer to vertexs
            glVertexPointer(2, GL_FLOAT, 0, vertices);
            //draw
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            //set old state
            render->setColorState(lastColorState);
            //only vertex
        }
    }
    //restore state
	render->setZBufferState(zbufferState);
	render->setBlendState(blendState);
	render->setMatrixsState(matrixsState);
    render->setClientState(clientState);
    render->setViewportState(globalViewport);
}