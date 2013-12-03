#include <stdafx.h>
#include <RiemannColors.h>
///////////////////////
template < class T> void HSVtoRGB( T *r, T *g, T *b, T h, T s, T v ){
	int i;
	T f, p, q, t;
	if( s == 0 ) {
		// achromatic (grey)
		*r = *g = *b = v;
		return;
	}
	h /= 60;			// sector 0 to 5
	i = floor( h );
	f = h - i;			// factorial part of h
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );
	switch( i ) {
		case 0:
			*r = v;
			*g = t;
			*b = p;
			break;
		case 1:
			*r = q;
			*g = v;
			*b = p;
			break;
		case 2:
			*r = p;
			*g = v;
			*b = t;
			break;
		case 3:
			*r = p;
			*g = q;
			*b = v;
			break;
		case 4:
			*r = t;
			*g = p;
			*b = v;
			break;
		default:		// case 5:
			*r = v;
			*g = p;
			*b = q;
			break;
	}
}
///////////////////////
using namespace Easy3D;
using namespace RiemannGui;
///////////////////////
RiemannColors::RiemannColors(const Easy3D::Table& config):isshow(false){
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
    radius=std::floor(config.getFloat("radius",64.0f));
    scale=config.getFloat("scale",64.0f)*0.5;
    Utility::Path path=Application::instance()->appDataDirectory()+"/colors_"+radius+".tga";
    if(path.existsFile()){
        texture.load(path);
    }
    else{
    /* create texture */
        uint imagesize=(uint)this->radius*2;
        Easy3D::Image::rgba defaultcolor(255,255,255,0);
        Easy3D::Image radialImage(imagesize,imagesize,4,true,defaultcolor);
        
        for(uint x=0;x!=imagesize;++x){
            for(uint y=0;y!=imagesize;++y){
                Vec2 point(Vec2(x,y)-Vec2(radius,radius));
                float leng=point.length()/radius;
                if(leng<=1.0){
                    float deAngle=Easy3D::Math::todeg(std::atan2(point.y, point.x))-180;
                    float angle=deAngle < 0 ? deAngle+360 : deAngle;
                    Debug::message()<<angle<<"\n";
                    float r,g,b;
                    HSVtoRGB(&r,&g,&b,angle,leng,255.0f);
                    Easy3D::Image::rgba rgba(r,g,b,255);
                    radialImage.setPixel(x,y,rgba);
                }
            }
        }
        //save image
        radialImage.save(path);
        //load texture
        texture.loadFromImage(&radialImage);
    }
    /////////////////////////////////////////////////////////////////////////////////////
}

void RiemannColors::draw(Easy3D::Render* render){
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
		model.setTranslation(Vec3 (windowSize.x*0.5-mbox.x*0.5-sbox.x,
								   windowSize.y*0.5-mbox.y*0.5-sbox.y,
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
        //draw sprite
        texture.bind();
        model.setTranslation(windowSize*0.5);
        model.addScale(Vec3(scale,scale,1));
        render->setMatrixsState(Render::MatrixsState(projection,model));
        render->drawColorUVSprite(Color(255,255,255,255));
		////////////////////////////////////////////////////////////////
		//restore state
		render->setZBufferState(zbufferState);
		render->setBlendState(blendState);
		render->setMatrixsState(matrixsState);
		render->setClientState(clientState);
		render->setViewportState(globalViewport);
	}
}