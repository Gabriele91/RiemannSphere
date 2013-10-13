#include <stdafx.h>
#include <RiemannSceneGLSL.h>

///////////////////////
using namespace Easy3D;
using namespace RiemannSphere;
///////////////////////
RiemannSceneGLSL::RiemannSceneGLSL()
    :cameraManager(NULL)
    ,sceneInfo(ON_PAUSE)
    ,poly(Table("function.test.e2d"))
{
}

RiemannSceneGLSL::~RiemannSceneGLSL(){
}

void RiemannSceneGLSL::onStart(){
    //set clear color
    setClearColorState({25,128,255,255});
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
	String define=("POLYSIZE "+String::toString(poly.constants.size()));
    const char *defines[2]={NULL,0};
    defines[0]=&define[0];
	newtonShader.loadShader("assets/newton.vs.glsl","assets/newton.ps.glsl",defines);
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

    //change state
    auto mState=getMatrixsState();
    auto newMState=mState;
    //set model matrix info
    Object obj;
	obj.setPosition(Vec3::ZERO);
	obj.setScale(Vec3::ONE*5.0f);
	obj.setTurn(Quaternion::fromEulero(Vec3(0,Math::torad(30),0)));
    //draw
    newMState.modelview=newMState.modelview.mul(obj.getGlobalMatrix());
    setMatrixsState(newMState);
	//draw sfere
	setClientState(ClientState(ClientState::VERTEX|ClientState::COLOR));
    setTextureState(TextureState(TextureState::NONE));
	
	newtonShader.bind();
    
    std::vector<Vec2> consts;
    std::vector<Vec2> roots;
    std::vector<Vec4> colors;
    
    for(auto c:poly.constants)
        consts.push_back(Vec2(c,0.0));
    for(auto& r:poly.roots)
        roots.push_back(Vec2(r.real(),r.imag()));
    for(auto& rc:poly.rootsColor)
        colors.push_back(Vec4(rc.r,rc.g,rc.b,1.0));
    
	newtonShader.uniformFloat("radius",1.0f);
	newtonShader.uniformVec2Array("poly",&consts[0],(int)consts.size());
	newtonShader.uniformVec2Array("roots",&roots[0],(int)consts.size()-1);
	newtonShader.uniformVec4Array("colors",&colors[0],(int)consts.size()-1);
	drawSphere(100,200);
	newtonShader.unbind();
    //reset state
    setMatrixsState(mState);
	
	//draw text
	setClientState(ClientState(ClientState::VERTEX|ClientState::UVMAP));
    setTextureState(TextureState(TextureState::TEXTURE2D));
    aharoni.text(Vec2(10,10),
                 "Level:"+String::toString(level+1)+"\n"
                 );
    
    
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

void RiemannSceneGLSL::onKeyDown(Key::Keyboard key){}
void RiemannSceneGLSL::onMouseDown(Vec2 mousePosition, Key::Mouse button){
    // if(button==Key::BUTTON_LEFT)
    //    obj.setTranslation(Vec3(0,0,-0.1f));
    // else if(button==Key::BUTTON_RIGHT)
    //      obj.setTranslation(Vec3(0,0,0.1f));
}
