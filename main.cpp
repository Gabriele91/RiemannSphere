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
#include <RiemannScene.h>

//last exemple http://pastebin.com/RGFiAuBs

using namespace Easy3D;

class RiemannApp : public Game {
    
    public:
    
    
    RiemannApp():Game("RiemannSphere",1280,720){}
    
    enum SCENE{
        RIEMANN_SCENE=0
    };
    
    void onStart(){
        
        addSceneAndActive(RIEMANN_SCENE, new RiemannSphere::RiemannScene());
    
    }
    
    void onRun(float dt){
    }

    void onEnd(){
    
    }
    
    
        
};

int main(int argc,const char *args[]){
    
    Application::create();
    Application::instance()->exec(new RiemannApp());
    delete Application::instance()->getGame();
    return 0;
}