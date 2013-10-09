#include <stdafx.h>
#include <RiemannScene.h>

///////////////////////
using namespace RiemannSphere;
using namespace Easy3D;
///////////////////////

RiemannScene::RiemannScene()
    :cameraManager(NULL)
    ,sceneInfo(ON_PAUSE)
    ,sphere(&camera,"function.test.e2d",
            1000000, 1000000*2,/*8000,8000,*/
            10,/*livels*/
            3.0f,/*sphere radius*/
            2.35/*detail per livels*/)
{
}

void RiemannScene::onStart(){
    //set clear color
    setClearColorState({25,128,255,255});
    //set projection and modelview
    setCullFaceState(CullFaceState(CullFace::BACK));
	//set client state
	setClientState(ClientState(ClientState::VERTEX|ClientState::COLOR));
    //add camera manage
	addChild(cameraManager=new CameraManager(&camera,Vec3::ZERO,&sphere));
    cameraManager->setVelocity(Vec3(10,10,.0015));
    cameraManager->setProjectionInfo(Math::torad(10.0f), 1.0, 49);
    camera.setPosition(Vec3(0,0,50),true);
    setMatrixsState(MatrixsState(camera));
    //font
    aharoni.load("assets/game.font.e2d");
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
	//clear and draw
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
}

void RiemannScene::onRun(float dt){
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
        cameraManager->setVelocity(Vec3(10,10,.005));
        level=1;
    }
    if(std::abs(cameraManager->getAngle())<0.040){
        cameraManager->setVelocity(Vec3(5,5,.002));
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
    
    
	//draw sfere
	setClientState(ClientState(ClientState::VERTEX|ClientState::COLOR));
    setTextureState(TextureState(TextureState::NONE));
    sphere.setLevel(level);
    sphere.draw();
#if 1
    //draw ray
    Vec3 dir=camera.getNormalPointFrom2DScreen(getInput()->getMouse());
    Vec3 pos=camera.getPointFrom2DScreen(getInput()->getMouse());
    Ray ray(pos,dir);
    ray.draw(this,50);
    //draw collision / point
    Segment out;
    auto collide=sphere.getCurSphere().rayCast(ray, out);
    out.draw(this);
    //draw point
    {
        //change state
        auto mState=getMatrixsState();
        auto newMState=mState;
        //set model matrix info
        Object obj;
        obj.setPosition(out.t[0]);
        obj.setScale(Vec3::ONE*0.0001);
        //draw
        newMState.modelview=newMState.modelview.mul(obj.getGlobalMatrix());
        setMatrixsState(newMState);
        drawColorCube(Color(255,255,0,255));
        //reset state
        setMatrixsState(mState);
    }
#endif
	//draw text
	setClientState(ClientState(ClientState::VERTEX|ClientState::UVMAP));
    setTextureState(TextureState(TextureState::TEXTURE2D));
    aharoni.text(Vec2(10,10),
                 "Level:"+String::toString(level+1)+"\n"+
                 "vbo size:"+(size_t)(sphere.getVboSize()/pow(1024,2))+" mb\n"+
                 "tree size:"+(size_t)(sphere.getTreeSize()/pow(1024,2))+" mb\n"+
                 "tree nodes:"+sphere.getTreeNodes()+"\n"
                 );
    
    
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
