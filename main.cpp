//
//  main.cpp
//  RiemannSphere
//
//  Created by Gabriele Di Bari on 14/08/13.
//  Copyright (c) 2013 Gabriele Di Bari. All rights reserved.
//

#include <stdio.h>
#include <Config.h>
#include <Application.h>
#include <Game.h>
#include <Math3D.h>
#include <Timer.h>
#include <iostream>
#include <Camera.h>
#include <Render.h>
#include <Screen.h>
//last exemple http://pastebin.com/RGFiAuBs

using namespace Easy3D;

class RiemannSphere : public Game,Input::KeyboardHandler,Input::MouseHandler {
    
    public:
    
    Camera camera;
    Vec3   turnVel;
    Vec3   moveDir;
    Object obj1,obj2;
    
    RiemannSphere():Game("RiemannSphere",1280,720){}
    
    void onStart(){
        //set clear color
        setClearColorState({25,128,255,255});
        //set projection and modelview
        camera.setPerspective(45, 0.5, 1000.0);
        setMatrixsState(MatrixsState(camera));
        setCullFaceState(CullFaceState(CullFace::BACK));
        //camera vel
        turnVel=Vec3::ONE;
        
        obj1.setTranslation(Vec3(0,0,-10));
        obj2.setTranslation(Vec3(2,0,-10));
        
        //set input
        getInput()->addHandler((Input::KeyboardHandler*)this);
        getInput()->addHandler((Input::MouseHandler*)this);
    
    
    }
    
    void onRun(float dt){
        
        doClear();
        camera.update();
        
        if(camera.boxInFrustum(AABox(obj1.getPosition(),obj1.getScale()))){
            setModelView(camera,obj1);
            drawColorCube({255,255,0,255});
        }
        if(camera.sphereInFrustum(obj2.getPosition(),0.5f)){
            setModelView(camera,obj1);
            drawColorCube({255,255,0,255});
        }
        setModelView(camera,obj2);
        drawColorSphere({0,128,0,255});
        
        
    }

    void onEnd(){
    
    }
    
    
    void onKeyDown(Key::Keyboard key){
        
        if(Key::list(key,Key::W,Key::UP))
            camera.setMove(Vec3(0,0,.1));
        
        else if(Key::list(key,Key::S,Key::DOWN))
            camera.setMove(Vec3(0,0,-.1));
        
        else if(Key::list(key,Key::A,Key::LEFT))
            camera.setMove(Vec3(.1,0,0));
        
        else if(Key::list(key,Key::D,Key::RIGHT))
            camera.setMove(Vec3(-.1,0,0));
    }
    
    void onMouseDown(Vec2 mousePosition, Key::Mouse button){
        
        if(Key::list(button, Key::BUTTON_LEFT, Key::BUTTON_RIGHT)){
            
            Vec2 center(getScreen()->getWidth()*0.5,
                        getScreen()->getHeight()*0.5);
            //get direction and intensity
            Vec2 dir=Application::instance()->getInput()->getMouse()-center;
            float leng=dir.length();
            
            if(button==Key::BUTTON_LEFT){
                if(leng>0.0f){
                    dir/=leng;//normalize
                    
                    leng=Math::min(leng,10.0f);
                    
                    Vec3 euler;
                    camera.getRotation().getEulero(euler);
                    
                    if(dir.x>0) euler.y-=Math::torad(1)*leng*turnVel.y;
                    if(dir.x<0) euler.y+=Math::torad(1)*leng*turnVel.y;
                    
                    if(dir.y>0) euler.x-=Math::torad(1)*leng*turnVel.x;
                    if(dir.y<0) euler.x+=Math::torad(1)*leng*turnVel.x;
                    
                    camera.setRotation(Quaternion::fromEulero(euler));
                    
                }
            }
            getScreen()->setPositionCursor(center);
        }
    }
    
};

int main(int argc,const char *args[]){
    Application::create();
    Application::instance()->exec(new RiemannSphere());
    delete Application::instance()->getGame();
    return 0;
}