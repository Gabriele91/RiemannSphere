#include <stdafx.h>
#include <RiemannFormula.h>

///////////////////////
using namespace Easy3D;
using namespace RiemannGui;
///////////////////////

RiemannFormula::RiemannFormula(const Easy3D::Table& config){
    //setup logic
    text="z^3-1";
    textid=0;
    showpointer=false;
    textid=0;
    textidselect=0;
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
    //set input
    Application::instance()->getInput()->addHandler((Easy3D::Input::KeyboardHandler*)this);
    Application::instance()->getInput()->addHandler((Easy3D::Input::MouseHandler*)this);
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


void RiemannFormula::setFilter(std::function<bool(char c)>& filter){
    this->filter=filter;
}

RiemannFormula::~RiemannFormula(){
    delete font;
}

void RiemannFormula::onKeyPress(Easy3D::Key::Keyboard key){
    //pointer to left
    if(key==Easy3D::Key::LEFT){
        if(textid==textidselect)
            --textid;
        else if(textid<textidselect)
            textidselect=textid;
        else if(textid>textidselect)
            textid=textidselect;
    }
    //pointer to right
    else if(key==Easy3D::Key::RIGHT){
        if(textid==textidselect)
            ++textid;
        else if(textid>textidselect)
            textidselect=textid;
        else if(textid<textidselect)
            textid=textidselect;
    }
    //copy
    else if(Key::C==key&&Application::instance()->getInput()->getKeyDown(Key::RCTRL)){
        //delete substring
        int minselect= textidselect>textid? textid: textidselect;
        int maxselect= textidselect<textid? textid: textidselect;
        //
        Application::instance()->getInput()->copyString(text.substr(minselect,maxselect-minselect));
        //NO FIX POINT
        return;
    }
    //past
    else if(Key::V==key&&Application::instance()->getInput()->getKeyDown(Key::RCTRL)){
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
        Vec2 mposition(mousePosition.x-textPos.x,
                       -mousePosition.y+textPos.y);
        int selectid=font->pointChar(text, mposition);
        if(selectid>-1){
            textid=selectid;
            showpointer=true;
        }
        else
            showpointer=false;
        textidselect=textid;
    }
}
void RiemannFormula::onMouseDown(Easy3D::Vec2 mousePosition, Easy3D::Key::Mouse button) {
    if(Easy3D::Key::BUTTON_LEFT==button && showpointer){
        //mpos
        Vec2 mposition( mousePosition.x-textPos.x,
                       -mousePosition.y+textPos.y);
        //box
        Vec2 p1=textPos;
        Vec2 p2=p1+font->sizeText(text)*Vec2(1.0,-0.5);
        //select
        int selectid=font->pointChar(text, mposition);
        if(selectid>-1){
                textidselect=selectid;
        }
        else if(!(p1.x<mousePosition.x &&
                  p2.x>mousePosition.x &&
                  p1.y<mousePosition.y &&
                  p2.y>mousePosition.y)
                ){
            if(textidselect<textid)
                textidselect=0;
            else if(textidselect>textid)
                textidselect=(int)text.size();
        }
        
    }
}
void RiemannFormula::onMouseRelease(Easy3D::Vec2 mousePosition, Easy3D::Key::Mouse button) {
    
}

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
    glUseProgram(0);
	//mode (2d)
    render->setViewportState(Vec4(0,0,windowSize.x,windowSize.y));
    //ortogonal
	Mat4 projection;
	projection.setOrtho(0,windowSize.x,0,windowSize.y,0.0,1.0);
    //set pos
    Mat4 model;
    Vec3 pos(windowSize.x*0.5-sizebox.x*0.5,0,0);
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
    textPos=Vec2(pos.x+sbox.x,windowSize.y-(pos.y+mbox.y+sbox.y+offset.y)); //no viewport
    render->setViewportState(Vec4(pos.x+sbox.x,pos.y+sbox.y+offset.y,mbox.x,mbox.y)); //viewport
    //set matrix
	projection.setOrtho(0,mbox.x,0,mbox.y,0.0,1.0);
    model.setTranslation(Vec2(0,mbox.y));
    render->setMatrixsState(Render::MatrixsState(projection,model));
    //draw
    font->text(text,textColor);
    
    if(showpointer){
        //set pos
        Vec2 scalePointer(1,font->size()*0.5);
        Vec2 posPointer=font->endChar(text, textid);
        Vec2 offsetPointer(-scalePointer.x*0.5,mbox.y-sbox.y-scalePointer.y*0.5);
        //first point
        //calc final pos
        Vec3 posFinalPointer(posPointer.x+offsetPointer.x,
                             posPointer.y+offsetPointer.y,
                             0.0);
        //set client
        render->setClientState(Render::ClientState::VERTEX);
        //disable texture
        render->setTextureState(Render::TextureState::NONE);
        
        //draw pointer
        if(textidselect==textid){
            //update model
            model.setTranslation(posFinalPointer);
            model.addScale(scalePointer);
            glLoadMatrixf(model);
            render->drawColorSprite(textColor);
        }
        //or selection
        else
        {
            Vec2 posPointer=font->endChar(text, textidselect);
            //calc final pos
            Vec3 posFinal(posPointer.x+offsetPointer.x,
                          posPointer.y+offsetPointer.y,
                          0.0);
            //vertexs
            float vertices[]={
                 posFinalPointer.x,       -scalePointer.y+posFinal.y,
                 posFinalPointer.x,        scalePointer.y+posFinal.y,
                 posFinal.x,              -scalePointer.y+posFinal.y,
                 posFinal.x,               scalePointer.y+posFinal.y
            };
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
void RiemannFormula::update(float dt){

}