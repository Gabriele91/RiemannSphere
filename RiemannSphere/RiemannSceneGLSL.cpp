#include <stdafx.h>
#include <RiemannSceneGLSL.h>

///////////////////////
using namespace Easy3D;
using namespace RiemannSphere;

RiemannSceneGLSL::RiemannSceneGLSL(Polynomial<double> *poly)
    :RiemannInterface()
    ,cameraManager(NULL)
    ,sceneInfo(ON_PAUSE)
    ,poly(poly)
	,fractal(*poly)
	,drawSymbols(&camera,this,
				"assets/infinity.tga",
				"assets/zero.tga",
				"assets/point.tga")
{
}

RiemannSceneGLSL::~RiemannSceneGLSL(){
}



RiemannSceneGLSL::FractalShader::FractalShader(Polynomial<double>& poly):shader(NULL){
    for(auto c:poly.constants)
		constants.push_back(Vec2(c.real(),c.imag()));
    for(auto c:poly.subconstants)
		subconstants.push_back(Vec2(c.real(),c.imag()));
    for(auto& r:poly.roots)
        roots.push_back(Vec2(r.real(),r.imag()));
    for(auto& rc:poly.rootsColor)
        colors.push_back(Vec4(rc.r,rc.g,rc.b,1.0));
    //infinite color
    infiniteColor.r=(float)poly.infiniteColor.r;
    infiniteColor.g=(float)poly.infiniteColor.g;
    infiniteColor.b=(float)poly.infiniteColor.b;
    infiniteColor.a=(float)1.0f;
}
void RiemannSceneGLSL::shaderbind(){
	fractal.shader->bind();
	fractal.shader->uniformFloat("radius",6.0f);
    //uniform polygon
    fractal.shader->uniformVec2Array("poly",&fractal.constants[0],(int)fractal.constants.size());
    //uniform subpolygon if is supportated
    GLint idsubpoly=fractal.shader->getUniformID("subpoly");
    if(idsubpoly!=-1)
		fractal.shader->uniformVec2Array(idsubpoly,&fractal.subconstants[0],(int)fractal.subconstants.size());
    //uniform infinitecolor if is supportated
    GLint infcolor=fractal.shader->getUniformID("infcolor");
    if(idsubpoly!=-1)
		fractal.shader->uniformVector4D(infcolor,fractal.infiniteColor);
	//uniform roots
    fractal.shader->uniformVec2Array("roots",&fractal.roots[0],(int)fractal.roots.size());
	//uniform colors
    fractal.shader->uniformVec4Array("colors",&fractal.colors[0],(int)fractal.colors.size());
}
void RiemannSceneGLSL::shaderunbind(){
	fractal.shader->unbind();
}

void RiemannSceneGLSL::onStart(){
    //add camera manage
	sphere.radius=3;
	addChild(cameraManager=new CameraManager(&camera,Vec3::ZERO,&sphere));
    cameraManager->setVelocity(Vec3(10,10,0.08));
    cameraManager->setProjectionInfo(Math::torad(10.0f), 1.0, 100);
    camera.setPosition(Vec3(0,0,50),true);
    setMatrixsState(MatrixsState(camera));
    //font
    aharoni.load("assets/game.font.e2d");
	//shader
	//load sheders
	String definePolySize=("POLYSIZE "+String::toString(poly->constants.size()));
	String defineSubPolySize=("SUBPOLYSIZE "+String::toString(Math::max(poly->subconstants.size(),(size_t)1)));
	String defineItPoly=("NUMIT "+String::toString(poly->iterations));
    const char *defines[4]={NULL,NULL,NULL,0};
    defines[0]=&definePolySize[0];
    defines[1]=&defineSubPolySize[0];
    defines[2]=&defineItPoly[0];
	shader[Iterations::SCHROEDER].loadShader("assets/base.vs.glsl","assets/schroeder.ps.glsl",defines);
	shader[Iterations::SCHROEDER4].loadShader("assets/base.vs.glsl","assets/schroeder4.ps.glsl",defines);
    shader[Iterations::HALLEY].loadShader("assets/base.vs.glsl","assets/halley.ps.glsl",defines);
    shader[Iterations::HALLEY4].loadShader("assets/base.vs.glsl","assets/halley4.ps.glsl",defines);
    shader[Iterations::NEWTON].loadShader("assets/base.vs.glsl","assets/newton.ps.glsl",defines);
    shader[Iterations::GENERIC].loadShader("assets/base.vs.glsl","assets/generic.ps.glsl",defines);
    //select shader
    fractal.shader=&shader[poly->method];
    //build grid
    grid.build(20,20);
    grid.setScale(Vec3(10,1,10));
    //build mesh
    gfSphere.build(5, 3.0);
    
    //init
    onResume();
	
}

void RiemannSceneGLSL::onResume(){
    //reset states
    //set clear color
    setClearColorState({255,255,255,255});
    //set projection and modelview
    setCullFaceState(CullFaceState(CullFace::BACK));
	//set client state
	setClientState(ClientState(ClientState::VERTEX|ClientState::COLOR));
    //add input
    getInput()->addHandler(dynamic_cast<Easy3D::Input::KeyboardHandler*>(this));
    getInput()->addHandler(dynamic_cast<Easy3D::Input::MouseHandler*>(this));
    //set state
    cameraManager->setCurrentState(CameraManager::EVENTS::ON_ENABLE);
    //save info
    sceneInfo=ON_RESUME;
	//clear and draw
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
}


void RiemannSceneGLSL::onRun(float dt){
    doClear();
    camera.update();
    setMatrixsState(MatrixsState(camera));
    
    
    if(getInput()->getKeyDown(Key::Q)){
        Camera cam2;
        cam2.setPerspective(Math::torad(25.0f), 1.0, 20.0);
        cam2.setPosition(Vec3(0,0,20),true);
        cam2.update();
        setMatrixsState(MatrixsState(cam2));
    }
    
    int level=0;
    
    if(std::abs(cameraManager->getAngle())<0.08){
        cameraManager->setVelocity(Vec3(10,10,.02));
        level=1;
    }
    if(std::abs(cameraManager->getAngle())<0.040){
        cameraManager->setVelocity(Vec3(5,5,.01));
        level=2;
    }
    if(std::abs(cameraManager->getAngle())<0.020){
        cameraManager->setVelocity(Vec3(2,2,.001));
        level=3;
    }
    if(std::abs(cameraManager->getAngle())<0.010){
        cameraManager->setVelocity(Vec3(1,1,.0005));
        level=4;
    }
    if(std::abs(cameraManager->getAngle())<0.005){
        cameraManager->setVelocity(Vec3(0.5,0.5,.0002));
        level=5;
    }
    if(std::abs(cameraManager->getAngle())<0.001){
        cameraManager->setVelocity(Vec3(0.25,0.25,.0002));
        level=6;
    }
    if(std::abs(cameraManager->getAngle())<0.0004){
        cameraManager->setVelocity(Vec3(0.1,0.1,.0001));
        level=7;
    }
    if(std::abs(cameraManager->getAngle())<0.0001){
        cameraManager->setVelocity(Vec3(0.05,0.05,.00002));
        level=8;
    }
    if(std::abs(cameraManager->getAngle())<0.00005){
        cameraManager->setVelocity(Vec3(0.025,0.025,.00001));
        level=9;
    }
    if(std::abs(cameraManager->getAngle())<0.000025){
        cameraManager->setVelocity(Vec3(0.025,0.025,.000005));
        level=10;
    }
    if(std::abs(cameraManager->getAngle())<0.00001){
        cameraManager->setVelocity(Vec3(0.025,0.025,.000002));
        level=11;
    }
    if(std::abs(cameraManager->getAngle())<0.000005){
        cameraManager->setVelocity(Vec3(0.025,0.025,.000001));
        level=12;
    }
    if(std::abs(cameraManager->getAngle())<0.000001){
        cameraManager->setVelocity(Vec3(0.025,0.025,.0000001));
        level=13;
    }
    if(std::abs(cameraManager->getAngle())<0.00000004){
        cameraManager->setVelocity(Vec3(0.025,0.025,.00000000001));
        level=14;
    }
    if(std::abs(cameraManager->getAngle())<0.000000005){
        cameraManager->setVelocity(Vec3(0.025,0.025,.000000001));
        level=15;
    }
    
	//draw sfere
    setTextureState(TextureState(TextureState::NONE));
    //draw sphere
	shaderbind();
    //draw
	gfSphere.draw(this);
    //unbind
	shaderunbind();
	//draw grid
    if(dGrid) grid.draw(this,Color(0,0,0,255));
    
	//draw text
    setTextureState(TextureState(TextureState::TEXTURE2D));
    
	if(dInfinite) drawSymbols.drawInfinity(Vec3(0,sphere.radius,0),Vec2(20,10),0.38,1.00);
	if(dZero) drawSymbols.drawZero(Vec3(0,-sphere.radius,0),Vec2(10,20),0.38,1.00);
    
    if(dRoots) for(size_t i=0;i<poly->roots.size();++i){
        Vec3 pos=poly->planeToSphere(poly->roots[i]).to<'x','y','z'>()*(sphere.radius);
        drawSymbols.drawPoint(pos,Vec2(10,10),0.38,1.00,poly->rootsColor[i]);
    }
	
}

void RiemannSceneGLSL::drawFontIn3DScene(const Easy3D::Vec3& pos,const Easy3D::String& text,const Easy3D::Vec2& scale,const Easy3D::Color& color){
	Vec2 offset=aharoni.sizeText(text)*Vec2(-0.5,0.25)*scale;
	aharoni.text(camera.getScreenPointFrom3DSpace(pos)+offset,text,scale,color);
}

void RiemannSceneGLSL::onPause(){
    //remove input
    getInput()->removeHandler(dynamic_cast<Easy3D::Input::KeyboardHandler*>(this));
    getInput()->removeHandler(dynamic_cast<Easy3D::Input::MouseHandler*>(this));
    //set state
    cameraManager->setCurrentState(CameraManager::EVENTS::ON_DISABLE);
    //save info
    sceneInfo=ON_PAUSE;
    
}

void RiemannSceneGLSL::onEnd(){
    if(sceneInfo==ON_RESUME) onPause();
}

CameraPositionInfo RiemannSceneGLSL::getCameraPositionInfo(){
    return cameraManager->getCameraPositionInfo();
}
void RiemannSceneGLSL::setCameraPositionInfo(const CameraPositionInfo& cpi){
    cameraManager->setCameraPositionInfo(cpi);
}

void RiemannSceneGLSL::onKeyDown(Key::Keyboard key){
    /*
	if(key==Key::N) fractal.fselected=NEWTON;
    else if(key==Key::H) fractal.fselected=HALLEY;
    else if(key==Key::S) fractal.fselected=SCHROEDER;
    else if(key==Key::F) fractal.fselected=GENERIC;
    else if(key==Key::N4) {
        if(fractal.fselected==HALLEY) fractal.fselected=HALLEY4;
        if(fractal.fselected==SCHROEDER) fractal.fselected=SCHROEDER;
    }
     */
}
void RiemannSceneGLSL::onMouseDown(Vec2 mousePosition, Key::Mouse button){
    // if(button==Key::BUTTON_LEFT)
    //    obj.setTranslation(Vec3(0,0,-0.1f));
    // else if(button==Key::BUTTON_RIGHT)
    //      obj.setTranslation(Vec3(0,0,0.1f));
}
