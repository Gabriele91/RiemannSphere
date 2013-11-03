#include <stdafx.h>
#include <RiemannSceneGeodesic.h>

///////////////////////
using namespace RiemannSphere;
using namespace Easy3D;
///////////////////////
//#define _HD_
RiemannSceneGeodesic::RiemannSceneGeodesic()
    :cameraManager(NULL)
    ,sceneInfo(ON_PAUSE)
    ,polynomialConfig("function.test.e2d")
    ,poly(polynomialConfig)
    ,newton(&poly)
    ,halley(&poly)
    ,halley4(&poly)
    ,schroeder(&poly)
    ,schroeder4(&poly)
    ,genericfractal(&poly)
	,drawSymbols(&camera,this,
				"assets/infinity.tga",
				"assets/zero.tga",
				"assets/point.tga")
{
    Fractal *select=NULL;
    String method=polynomialConfig.getString("method","newton").toLower();
    if(method=="newton"||method=="n") select=&newton;
    else if(method=="halley"||method=="h") select=&halley;
    else if(method=="halley4"||method=="h4") select=&halley4;
    else if(method=="schroeder"||method=="s") select=&schroeder;
    else if(method=="schroeder4"||method=="s4") select=&schroeder4;
    else if(method=="generic"||method=="g") select=&genericfractal;
    DEBUG_ASSERT_MSG(select, "Must to be selected a valid method");
    sphere=new GeodesicSphere(&camera,
                              select,
                              3.0,
                              (size_t)(536870912*0.5),
                              (size_t)(536870912*3));
}

RiemannSceneGeodesic::~RiemannSceneGeodesic(){
	delete sphere;
}

void RiemannSceneGeodesic::onStart(){
    //set clear color
    setClearColorState({25,128,255,255});
    //set projection and modelview
	setCullFaceState(CullFaceState(CullFace::DISABLE));
	//set client state
	setClientState(ClientState(ClientState::VERTEX|ClientState::COLOR));
    //add camera manage
    camera_sphere.radius=3.0;
	addChild(cameraManager=new CameraManager(&camera,Vec3::ZERO,&camera_sphere));
    cameraManager->setVelocity(Vec3(10,10,0.08));
    cameraManager->setProjectionInfo(Math::torad(10.0f), 1.0, 49);
    camera.setPosition(Vec3(0,0,50),true);
    setMatrixsState(MatrixsState(camera));
    //font
    aharoni.load("assets/game.font.e2d");
    //init
    onResume();
}

void RiemannSceneGeodesic::onResume(){
    //add input
    getInput()->addHandler((Easy3D::Input::KeyboardHandler*)this);
    getInput()->addHandler((Easy3D::Input::MouseHandler*)this);
    //set state
    cameraManager->setCurrentState(CameraManager::EVENTS::ON_ENABLE);
    //save info
    sceneInfo=ON_RESUME;
	//clear and draw
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
}

void RiemannSceneGeodesic::onRun(float dt){
    doClear();
    camera.update();
    setMatrixsState(MatrixsState(camera));
    
    
    if(getInput()->getKeyDown(Key::Q)){
        Camera cam2;
        cam2.setPerspective(Math::torad(25.0f), 1.0, 200.0);
        cam2.setPosition(Vec3(0,0,60),true);
        cam2.update();
        setMatrixsState(MatrixsState(cam2));
    }
    
    int level=0;
    
    if(std::abs(cameraManager->getAngle())<0.20){
        cameraManager->setVelocity(Vec3(10,10,.02));
        level=1;
    }
    if(std::abs(cameraManager->getAngle())<0.08){
        cameraManager->setVelocity(Vec3(5,5,.01));
        level=2;
    }
    if(std::abs(cameraManager->getAngle())<0.06){
        cameraManager->setVelocity(Vec3(2,2,.001));
        level=3;
    }
    if(std::abs(cameraManager->getAngle())<0.04){
        cameraManager->setVelocity(Vec3(1,1,.0005));
        level=4;
    }
    if(std::abs(cameraManager->getAngle())<0.02){
        cameraManager->setVelocity(Vec3(0.5,0.5,.00025));
        level=5;
    }
    if(std::abs(cameraManager->getAngle())<0.01){
        cameraManager->setVelocity(Vec3(0.25,0.25,.00012));
        level=6;
    }
    if(std::abs(cameraManager->getAngle())<0.005){
        cameraManager->setVelocity(Vec3(0.1,0.1,.00006));
        level=7;
    }
    if(std::abs(cameraManager->getAngle())<0.0025){
        cameraManager->setVelocity(Vec3(0.05,0.05,.00003));
        level=8;
    }
    if(std::abs(cameraManager->getAngle())<0.00125){
        cameraManager->setVelocity(Vec3(0.025,0.025,.00001));
        level=9;
    }
    if(std::abs(cameraManager->getAngle())<0.0006){
        cameraManager->setVelocity(Vec3(0.025,0.025,.000005));
        level=10;
    }
    if(std::abs(cameraManager->getAngle())<0.0003){
        cameraManager->setVelocity(Vec3(0.025,0.025,.0000025));
        level=11;
    }
    if(std::abs(cameraManager->getAngle())<0.00015){
        cameraManager->setVelocity(Vec3(0.025,0.025,.0000015));
        level=12;
    }
    
	//draw sfere
	setClientState(ClientState(ClientState::VERTEX|ClientState::COLOR));
    setTextureState(TextureState(TextureState::NONE));
    sphere->draw(this,level);
    sphere->drawNodes(this);
    //sphere->draw();
	//fustrum
	//cameraManager->drawFrustum(this);
	//draw text
	setClientState(ClientState(ClientState::VERTEX|ClientState::UVMAP));
    setTextureState(TextureState(TextureState::TEXTURE2D));
   	drawSymbols.drawInfinity(Vec3(0,camera_sphere.radius,0),Vec2(20,10),0.38,1.00);
	drawSymbols.drawZero(Vec3(0,-camera_sphere.radius,0),Vec2(10,20),0.38,1.00);
    
    for(size_t i=0;i<poly.roots.size();++i){
        Vec3 pos=poly.planeToSphere(poly.roots[i])*(camera_sphere.radius);
        drawSymbols.drawPoint(pos,Vec2(10,10),0.38,1.00,poly.rootsColor[i]);
    }
}

void RiemannSceneGeodesic::drawFontIn3DScene(const Easy3D::Vec3& pos,const Easy3D::String& text,const Easy3D::Vec2& scale){
	Vec2 offset=aharoni.sizeText(text)*Vec2(-0.5,0.25)*scale;
	aharoni.text(camera.getScreenPointFrom3DSpace(pos)+offset,text,scale);
}

void RiemannSceneGeodesic::onPause(){
    //remove input
    getInput()->removeHandler((Easy3D::Input::KeyboardHandler*)this);
    getInput()->removeHandler((Easy3D::Input::MouseHandler*)this);
    //set state
    cameraManager->setCurrentState(CameraManager::EVENTS::ON_DISABLE);
    //save info
    sceneInfo=ON_PAUSE;
}

void RiemannSceneGeodesic::onEnd(){
    if(sceneInfo==ON_RESUME) onPause();
}

void RiemannSceneGeodesic::onKeyDown(Key::Keyboard key){}
void RiemannSceneGeodesic::onMouseDown(Vec2 mousePosition, Key::Mouse button){
    // if(button==Key::BUTTON_LEFT)
    //    obj.setTranslation(Vec3(0,0,-0.1f));
    // else if(button==Key::BUTTON_RIGHT)
    //      obj.setTranslation(Vec3(0,0,0.1f));
}
