#include <stdafx.h>
#include <RiemannDialog.h>

///////////////////////
using namespace Easy3D;
using namespace RiemannGui;
///////////////////////
RiemannDialog::RiemannDialog(const Easy3D::Table& config):isshow(false),callback([](bool){}){
    //debug asserts
    DEBUG_ASSERT_MGS_REPLACE(config.existsAsType("font",Table::STRING),"RiemannFormula, must to be set font path");
    //font table
    Easy3D::String fontpath=config.getTablePath().getDirectory()+"/"+config.getString("font");
    font=new Easy3D::Font(fontpath);
    textColor=config.getVector4D("textColor",Vec4(Vec3::ZERO,1.0f));
    //debug asserts
    DEBUG_ASSERT_MGS_REPLACE(config.existsAsType("box",Table::TABLE),"RiemannFormula, must to be set box table");
    //get table
    auto boxConfig=config.getConstTable("box");
    //image path
    Easy3D::String imagepath=config.getTablePath().getDirectory()+"/"+boxConfig.getString("image");
    //debug assets
    DEBUG_ASSERT_MGS_REPLACE(boxConfig.existsAsType("image",Table::STRING),"RiemannFormula, must to be set box.image string");
    DEBUG_ASSERT_MGS_REPLACE(boxConfig.existsAsType("mbox",Table::VECTOR2D),"RiemannFormula, must to be set box.mbox 2D vector");
    DEBUG_ASSERT_MGS_REPLACE(boxConfig.existsAsType("sbox",Table::VECTOR2D),"RiemannFormula, must to be set box.sbox 2D vector");
    DEBUG_ASSERT_MGS_REPLACE(boxConfig.existsAsType("offset",Table::VECTOR2D),"RiemannFormula, must to be set box.offset 2D vector");
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
    //backgound
    backgoundColor=config.getVector4D("backgoundColor",Vec4(0,0,0,0.5));
    /////////////////////////////////////////////////////////////////////////////////////
}
void RiemannDialog::setText(const Easy3D::String& text){
	this->text=text;
	//calc text size
	mbox=font->sizeText(text)*Vec2(1.0,-0.5);
	sizebox=mbox+sbox;
	//rebuild
    box.rebuild(mbox,sbox,offset);
}

void RiemannDialog::draw(Easy3D::Render* render){
	if(isshow){
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
		//model matrix
		Mat4 model;
		///////////////////////////////////////////////////////////////	
		render->setTextureState(Render::TextureState::NONE);	
		//calc first pos
        Vec2 spoint;
        Vec2 epoint(windowSize);
        //vertexs default left
        float vertices[8]={
            spoint.x,         spoint.y,
            spoint.x,         epoint.y,
            epoint.x,         spoint.y,
            epoint.x,         epoint.y
        };
        //client state
        render->setClientState(Render::ClientState::VERTEX);
        //save color
        Render::ColorState lastColorState=render->getColorState();
		//set matrixs
		render->setMatrixsState(Render::MatrixsState(projection,model));
        //draw
        render->setColorState(backgoundColor);
        //unbind VBO
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        //pointer to vertexs
        glVertexPointer(2, GL_FLOAT, 0, vertices);
        //draw
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        //set old state
        render->setColorState(lastColorState);
		///////////////////////////////////////////////////////////////
		//calc pos
		model.setTranslation(Vec3 (windowSize.x*0.5-sizebox.x*0.5,
								   windowSize.y*0.5-sizebox.y*0.5,
								   0.0));
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
		//draw text
		model.setTranslation(Vec3 (windowSize.x*0.5-sizebox.x*0.5+sbox.x,
								   windowSize.y*0.5+sizebox.y-sbox.y,
								   0.0));
		render->setMatrixsState(Render::MatrixsState(projection,model));
		//draw text
		font->text(text,textColor);
		////////////////////////////////////////////////////////////////
		//restore state
		render->setZBufferState(zbufferState);
		render->setBlendState(blendState);
		render->setMatrixsState(matrixsState);
		render->setClientState(clientState);
		render->setViewportState(globalViewport);
	}
}