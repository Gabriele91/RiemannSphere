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
    ,sphere(60,60,0.5f)
{
}

void RiemannScene::onStart(){
    //set clear color
    setClearColorState({25,128,255,255});
    //set projection and modelview
    setCullFaceState(CullFaceState(CullFace::BACK));
    //add camera manage
    addChild(cameraManager=new CameraManager(&camera,Vec3::ZERO));
    cameraManager->setVelocity(Vec3(10,10,5));
    cameraManager->setProjectionInfo(Math::torad(45.0f), 1.0, 9.99);
    camera.setPosition(Vec3(0,0,10),true);
    setMatrixsState(MatrixsState(camera));
    //sphere
    sphereMeshs.push_back(sphere.genMesh(15, 5,
                                         20, 10));
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
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    doClear();
    camera.update();
    setMatrixsState(MatrixsState(camera));
    
    setModelView(camera, obj);
    sphereMeshs[0]->draw();
    

    if(camera.boxInFrustum(sphereMeshs[0]->getAABox()))
        setClearColorState({25,128,255,255});
    else
        setClearColorState({255,0,0,255});
    
    
    Object aabobj;
    aabobj.setPosition(sphereMeshs[0]->getAABox().getCenter());
    aabobj.setScale(sphereMeshs[0]->getAABox().getSize());
    setModelView(camera, aabobj);
    if(getInput()->getKeyDown(Key::X))
        drawCube();
    
    
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
    // if(button==Key::BUTTON_LEFT)
    //    obj.setTranslation(Vec3(0,0,-0.1f));
    // else if(button==Key::BUTTON_RIGHT)
    //      obj.setTranslation(Vec3(0,0,0.1f));
}
