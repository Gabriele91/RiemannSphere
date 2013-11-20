#include <stdafx.h>
#include <RiemannSceneGeodesic.h>

///////////////////////
using namespace RiemannSphere;
using namespace Easy3D;
///////////////////////
//#define _HD_
RiemannSceneGeodesic::RiemannSceneGeodesic(Polynomial<double> *poly)
    :RiemannInterface()
    ,cameraManager(NULL)
    ,sceneInfo(ON_PAUSE)
    ,poly(poly)
    ,newton(poly)
    ,halley(poly)
    ,halley4(poly)
    ,schroeder(poly)
    ,schroeder4(poly)
    ,genericfractal(poly)
	,drawSymbols(&camera,this,"assets/infinity.tga","assets/zero.tga","assets/point.tga")
{
    Fractal *select=NULL;
    
    if(poly->method==Iterations::NEWTON) select=&newton;
    else if(poly->method==Iterations::HALLEY) select=&halley;
    else if(poly->method==Iterations::HALLEY4) select=&halley4;
    else if(poly->method==Iterations::SCHROEDER) select=&schroeder;
    else if(poly->method==Iterations::SCHROEDER4) select=&schroeder4;
    else if(poly->method==Iterations::GENERIC) select=&genericfractal;
    
    sphere=new GeodesicSphere(&camera,select,3.0,(size_t)(536870912*0.1), (size_t)(536870912*3));
    //build grid
    grid.build(20,20);
    grid.setScale(Vec3(10,1,10));
}

RiemannSceneGeodesic::~RiemannSceneGeodesic(){
	delete sphere;
}

void RiemannSceneGeodesic::drawGridHack(){
    //draw grid
    cameraManager->changeNear(49);
    cameraManager->changeFar(100);
    setMatrixsState(MatrixsState(camera));
    setZBufferState(false);
    //draw grid
    grid.draw(this,Color(0,0,0,255));
    //renable
    setZBufferState(true);
    //reset far
    cameraManager->changeNear(20);
    cameraManager->changeFar(49);
    setMatrixsState(MatrixsState(camera));
    //draw grid
    grid.draw(this,Color(0,0,0,255));
}

void RiemannSceneGeodesic::onStart(){
    //add camera manage
    camera_sphere.radius=3.0;
	addChild(cameraManager=new CameraManager(&camera,Vec3::ZERO,&camera_sphere));
    cameraManager->setVelocity(Vec3(10,10,0.08));
    cameraManager->setProjectionInfo(Math::torad(10.0f), 20.0, 49);
    camera.setPosition(Vec3(0,0,50),true);
    setMatrixsState(MatrixsState(camera));
    //font
    aharoni.load("assets/game.font.e2d");
    //init
    onResume();
}

void RiemannSceneGeodesic::onResume(){
    //reset states
    //set clear color
    setClearColorState({255,255,255,255});
    //set projection and modelview
	setCullFaceState(CullFaceState(CullFace::DISABLE));
	//set client state
	setClientState(ClientState(ClientState::VERTEX|ClientState::COLOR));
    //add input
    getInput()->addHandler((Easy3D::Input::KeyboardHandler*)(this));
    getInput()->addHandler((Easy3D::Input::MouseHandler*)(this));
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
    if(std::abs(cameraManager->getAngle())<0.00005){
        cameraManager->setVelocity(Vec3(0.025,0.025,.0000015));
        level=13;
    }
    
	//draw sfere and greed
	setClientState(ClientState(ClientState::VERTEX|ClientState::COLOR));
    setTextureState(TextureState(TextureState::NONE));

    //draw grid
    if(dGrid) drawGridHack();
    
    //draw sphere
    sphere->draw(this,level);
    
	//draw text
    setTextureState(TextureState(TextureState::TEXTURE2D));
    
   	if(dInfinite) drawSymbols.drawInfinity(Vec3(0,camera_sphere.radius,0),Vec2(20,10),0.38,1.00);
	if(dZero)     drawSymbols.drawZero(Vec3(0,-camera_sphere.radius,0),Vec2(10,20),0.38,1.00);
    
    if(dRoots) for(size_t i=0;i<poly->roots.size();++i){
        Vec3 pos=poly->planeToSphere(poly->roots[i])*(camera_sphere.radius);
        drawSymbols.drawPoint(pos,Vec2(10,10),0.38,1.00,poly->rootsColor[i]);
    }
}

void RiemannSceneGeodesic::drawFontIn3DScene(const Easy3D::Vec3& pos,const Easy3D::String& text,const Easy3D::Vec2& scale){
	Vec2 offset=aharoni.sizeText(text)*Vec2(-0.5,0.25)*scale;
	aharoni.text(camera.getScreenPointFrom3DSpace(pos)+offset,text,scale);
}

void RiemannSceneGeodesic::onPause(){
    //remove input
    getInput()->removeHandler((Easy3D::Input::KeyboardHandler*)(this));
    getInput()->removeHandler((Easy3D::Input::MouseHandler*)(this));
    //set state
    cameraManager->setCurrentState(CameraManager::EVENTS::ON_DISABLE);
    //save info
    sceneInfo=ON_PAUSE;
}

void RiemannSceneGeodesic::onEnd(){
    if(sceneInfo==ON_RESUME) onPause();
}

CameraPositionInfo RiemannSceneGeodesic::getCameraPositionInfo(){
    return cameraManager->getCameraPositionInfo();
}
void RiemannSceneGeodesic::setCameraPositionInfo(const CameraPositionInfo& cpi){
    cameraManager->setCameraPositionInfo(cpi);
}

void RiemannSceneGeodesic::onKeyDown(Key::Keyboard key){}
void RiemannSceneGeodesic::onMouseDown(Vec2 mousePosition, Key::Mouse button){}
