#include <stdafx.h>
#include <RiemannScene.h>
#include <CameraManager.h>

///////////////////////
using namespace RiemannSphere;
using namespace Easy3D;
///////////////////////

RiemannScene::RiemannScene()
    :cameraManager(NULL)
    ,sceneInfo(ON_PAUSE)
{
}

void RiemannScene::onStart(){
    //set clear color
    setClearColorState({25,128,255,255});
    //set projection and modelview
    setCullFaceState(CullFaceState(CullFace::BACK));
    //add camera manager
    addChild(cameraManager=new CameraManager(&camera,Vec3(0,0,0)));
    cameraManager->setVelocity(Vec3(10,10,5));
    cameraManager->setProjectionInfo(Math::torad(45.0f), 9.5, 10.0);
    camera.setPosition(Vec3(0,0,-10),true);
    setMatrixsState(MatrixsState(camera));
    //init
    onResume();
}

void RiemannScene::onResume(){
    //add input
    getInput()->addHandler((Easy3D::Input::KeyboardHandler*)this);
    getInput()->addHandler((Easy3D::Input::MouseHandler*)this);
    //set state
    cameraManager->setCurrentState(CameraManager::EVENTS::ON_ENABLE);
    //save info
    sceneInfo=ON_RESUME;
}

void RiemannScene::onRun(float dt){
    
    doClear();
    camera.update();
    setMatrixsState(MatrixsState(camera));

    
    if(camera.sphereInFrustum(obj.getPosition(), 0.5))
        setClearColorState({25,128,255,255});
    else
        setClearColorState({255,0,0,255});
    
    
    obj.setScale(Vec3::ONE);
    setModelView(camera, obj);
    drawColorSphere({0,0,0,255},5,10);
    
    obj.setScale(Vec3(0.5,2.0,0.5));
    setModelView(camera, obj);
    drawColorSphere({0,128,0,255},5,10);
    
}

void RiemannScene::onPause(){
    //remove input
    getInput()->removeHandler((Easy3D::Input::KeyboardHandler*)this);
    getInput()->removeHandler((Easy3D::Input::MouseHandler*)this);
    //set state
    cameraManager->setCurrentState(CameraManager::EVENTS::ON_DISABLE);
    //save info
    sceneInfo=ON_PAUSE;
}

void RiemannScene::onEnd(){
    if(sceneInfo==ON_RESUME) onPause();
}

void RiemannScene::onKeyDown(Key::Keyboard key){}
void RiemannScene::onMouseDown(Vec2 mousePosition, Key::Mouse button){
  //  if(button==Key::BUTTON_LEFT)
  //      obj.setTranslation(Vec3(0,0,-0.1f));
  //  else if(button==Key::BUTTON_RIGHT)
  //      obj.setTranslation(Vec3(0,0,0.1f));
}
