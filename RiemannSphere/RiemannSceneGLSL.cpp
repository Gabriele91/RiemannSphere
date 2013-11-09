#include <stdafx.h>
#include <RiemannSceneGLSL.h>

///////////////////////
using namespace Easy3D;
using namespace RiemannSphere;

RiemannSceneGLSL::RiemannSceneGLSL()
    :cameraManager(NULL)
    ,sceneInfo(ON_PAUSE)
    ,polynomialConfig("function.test.e2d")
    ,poly(polynomialConfig)
	,fractal(poly)
	,drawSymbols(&camera,this,
				"assets/infinity.tga",
				"assets/zero.tga",
				"assets/point.tga")
{
}

RiemannSceneGLSL::~RiemannSceneGLSL(){
}



RiemannSceneGLSL::FractalShader::FractalShader(Polynomial<double>& poly){
    for(auto c:poly.constants)
		constants.push_back(Vec2(c.real(),c.imag()));
    for(auto c:poly.subconstants)
		subconstants.push_back(Vec2(c.real(),c.imag()));
    for(auto& r:poly.roots)
        roots.push_back(Vec2(r.real(),r.imag()));
    for(auto& rc:poly.rootsColor)
        colors.push_back(Vec4(rc.r,rc.g,rc.b,1.0));
}
void RiemannSceneGLSL::FractalShader::bind(){
	sheder->bind();
	sheder->uniformFloat("radius",1.0f);
    //uniform polygon
    sheder->uniformVec2Array("poly",&constants[0],(int)constants.size());
    //unifor subpolygon if is supportated
    GLint idsubpoly=sheder->getUniformID("subpoly");
    if(idsubpoly!=-1) 
		sheder->uniformVec2Array(idsubpoly,&subconstants[0],(int)subconstants.size());
	//uniform roots
    sheder->uniformVec2Array("roots",&roots[0],(int)roots.size());
	//uniform colors
    sheder->uniformVec4Array("colors",&colors[0],(int)colors.size());
}
void RiemannSceneGLSL::FractalShader::unbind(){
	sheder->unbind();
}

void RiemannSceneGLSL::onStart(){
    //set clear color
    setClearColorState({255,255,255,255});
    //set projection and modelview
    setCullFaceState(CullFaceState(CullFace::BACK));
	//set client state
	setClientState(ClientState(ClientState::VERTEX|ClientState::COLOR));
    //add camera manage
	sphere.radius=2.5;
	addChild(cameraManager=new CameraManager(&camera,Vec3::ZERO,&sphere));
    cameraManager->setVelocity(Vec3(10,10,0.08));
    cameraManager->setProjectionInfo(Math::torad(10.0f), 1.0, 100);
    camera.setPosition(Vec3(0,0,50),true);
    setMatrixsState(MatrixsState(camera));
    //font
    aharoni.load("assets/game.font.e2d");
	//shader
	//load sheders
	String definePolySize=("POLYSIZE "+String::toString(poly.constants.size()));
	String defineSubPolySize=("SUBPOLYSIZE "+String::toString(Math::max(poly.subconstants.size(),(size_t)1)));
	String defineItPoly=("NUMIT "+String::toString(polynomialConfig.getFloat("iterations",50)));
    const char *defines[4]={NULL,NULL,NULL,0};
    defines[0]=&definePolySize[0];
    defines[1]=&defineSubPolySize[0];
    defines[2]=&defineItPoly[0];
	schroederShader.loadShader("assets/base.vs.glsl","assets/schroeder.ps.glsl",defines);
	schroeder4Shader.loadShader("assets/base.vs.glsl","assets/schroeder4.ps.glsl",defines);
    halleyShader.loadShader("assets/base.vs.glsl","assets/halley.ps.glsl",defines);
    halley4Shader.loadShader("assets/base.vs.glsl","assets/halley4.ps.glsl",defines);
    newtonShader.loadShader("assets/base.vs.glsl","assets/newton.ps.glsl",defines);
    genericShader.loadShader("assets/base.vs.glsl","assets/generic.ps.glsl",defines);
    
    String method=polynomialConfig.getString("method","newton").toLower();
    if(method=="newton"||method=="n") fractal.sheder=&newtonShader;
    else if(method=="halley"||method=="h") fractal.sheder=&halleyShader;
    else if(method=="halley4"||method=="h4") fractal.sheder=&halley4Shader;
    else if(method=="schroeder"||method=="s") fractal.sheder=&schroederShader;
    else if(method=="schroeder4"||method=="s4") fractal.sheder=&schroeder4Shader;
    else if(method=="generic"||method=="g") fractal.sheder=&genericShader;
    
    //build grid
    grid.build(20,20);
    grid.setScale(Vec3(2,1,2));
    
    //init
    onResume();
	
}

void RiemannSceneGLSL::onResume(){
    //add input
    getInput()->addHandler((Easy3D::Input::KeyboardHandler*)this);
    getInput()->addHandler((Easy3D::Input::MouseHandler*)this);
    //set state
    cameraManager->setCurrentState(CameraManager::EVENTS::ON_ENABLE);
    //save info
    sceneInfo=ON_RESUME;
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

    //change state
    auto mState=getMatrixsState();
    auto newMState=mState;
    //set model matrix info
    Object obj;
	obj.setPosition(Vec3::ZERO);
	obj.setScale(Vec3::ONE*5.0f);
    //draw
    newMState.modelview=newMState.modelview.mul(obj.getGlobalMatrix());
    setMatrixsState(newMState);
	//draw sfere
	setClientState(ClientState(ClientState::VERTEX|ClientState::COLOR));
    setTextureState(TextureState(TextureState::NONE));
	//draw grid
    grid.draw(this,Color(0,0,0,255));
    //draw sphere
	fractal.bind();
	drawSphere(300,100);
	fractal.unbind();
    //reset state
    setMatrixsState(mState);
	
	
	//draw text
	setClientState(ClientState(ClientState::VERTEX|ClientState::UVMAP));
    setTextureState(TextureState(TextureState::TEXTURE2D));
    //aharoni.text(Vec2(10,10),"Level:"+String::toString(level+1)+"\n");
	//infinity point
	drawSymbols.drawInfinity(Vec3(0,sphere.radius,0),Vec2(20,10),0.38,1.00);
	drawSymbols.drawZero(Vec3(0,-sphere.radius,0),Vec2(10,20),0.38,1.00);
    
    for(size_t i=0;i<poly.roots.size();++i){
        Vec3 pos=poly.planeToSphere(poly.roots[i]).to<'x','y','z'>()*(sphere.radius);
        drawSymbols.drawPoint(pos,Vec2(10,10),0.38,1.00,poly.rootsColor[i]);
    }
	
}

void RiemannSceneGLSL::drawFontIn3DScene(const Easy3D::Vec3& pos,const Easy3D::String& text,const Easy3D::Vec2& scale,const Easy3D::Color& color){
	Vec2 offset=aharoni.sizeText(text)*Vec2(-0.5,0.25)*scale;
	aharoni.text(camera.getScreenPointFrom3DSpace(pos)+offset,text,scale,color);
}

void RiemannSceneGLSL::onPause(){
    //remove input
    getInput()->removeHandler((Easy3D::Input::KeyboardHandler*)this);
    getInput()->removeHandler((Easy3D::Input::MouseHandler*)this);
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
	if(key==Key::N) fractal.sheder=&newtonShader;
    else if(key==Key::H) fractal.sheder=&halleyShader;
    else if(key==Key::S) fractal.sheder=&schroederShader;
    else if(key==Key::G) fractal.sheder=&genericShader;
    else if(key==Key::N4) {
        if(fractal.sheder==&schroederShader) fractal.sheder=&schroeder4Shader;;
        if(fractal.sheder==&halleyShader) fractal.sheder=&halley4Shader;;
    }
}
void RiemannSceneGLSL::onMouseDown(Vec2 mousePosition, Key::Mouse button){
    // if(button==Key::BUTTON_LEFT)
    //    obj.setTranslation(Vec3(0,0,-0.1f));
    // else if(button==Key::BUTTON_RIGHT)
    //      obj.setTranslation(Vec3(0,0,0.1f));
}
