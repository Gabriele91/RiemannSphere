//
//  main.cpp
//  RiemannSphere
//
//  Created by Gabriele Di Bari on 14/08/13.
//  Copyright (c) 2013 Gabriele Di Bari. All rights reserved.
//
#include <stdafx.h>
#include <stdio.h>
#include <Config.h>
#include <Application.h>
#include <Game.h>
#include <RiemannScene.h>
#include <RiemannSceneGeodesic.h>
#include <RiemannSceneGLSL.h>

//last exemple http://pastebin.com/RGFiAuBs

using namespace Easy3D;

class RiemannApp : public Game,
                   public Easy3D::Input::KeyboardHandler {
    
    public:
    
    
		RiemannApp():Game("RiemannSphere",1280,720,32,30,true,Screen::MSAAx4){}
    
    enum SCENE{
        RIEMANN_SCENE_GEODESIC=0,
        RIEMANN_SCENE=1,
        RIEMANN_GLSL_SCENE=2
    };
    
    void onStart(){
        //add input keyboard
        getInput()->addHandler((Easy3D::Input::KeyboardHandler*)this);
		//scenes
        addSceneAndActive(RIEMANN_SCENE_GEODESIC, new RiemannSphere::RiemannSceneGeodesic());
        //addScene(RIEMANN_SCENE, new RiemannSphere::RiemannScene());
        addScene(RIEMANN_GLSL_SCENE, new RiemannSphere::RiemannSceneGLSL());
    }
    
    void onRun(float dt){
    }

    virtual void onKeyDown(Easy3D::Key::Keyboard key){
        //if(key==Key::V) activeScene(RIEMANN_SCENE);
        if(key==Key::C) activeScene(RIEMANN_SCENE_GEODESIC);
		if(key==Key::G) activeScene(RIEMANN_GLSL_SCENE);
        
        /*
         if(key==Key::R && sceneActive()==RIEMANN_SCENE) {
         auto scene=eraseScene(RIEMANN_SCENE);
         delete scene;
         addSceneAndActive(RIEMANN_SCENE, new RiemannSphere::RiemannScene());
         }*/
		if(key==Key::R && sceneActive()==RIEMANN_SCENE_GEODESIC) {
            //scene
            auto scene=eraseScene(RIEMANN_SCENE_GEODESIC);
            //save position
            auto cpi=((RiemannSphere::RiemannSceneGeodesic*)(scene))->getCameraPositionInfo();
            //delete scene
            delete scene;
            //new scene
            auto newscene=new RiemannSphere::RiemannSceneGeodesic();
            //active
            addSceneAndActive(RIEMANN_SCENE_GEODESIC, newscene);
            //set position
            newscene->setCameraPositionInfo(cpi);
        }
		if(key==Key::R && sceneActive()==RIEMANN_GLSL_SCENE) {
            //scene
            auto scene=eraseScene(RIEMANN_GLSL_SCENE);
            //save position
            auto cpi=((RiemannSphere::RiemannSceneGeodesic*)(scene))->getCameraPositionInfo();
            //delete scene
            delete scene;
            //new scene
            auto newscene=new RiemannSphere::RiemannSceneGLSL();
            //active
            addSceneAndActive(RIEMANN_GLSL_SCENE, newscene);
            //set position
            newscene->setCameraPositionInfo(cpi);
        }
        
	}

    void onEnd(){
		getInput()->removeHandler((Easy3D::Input::KeyboardHandler*)this);    
    }
    
    
        
};

int main(int argc,const char *args[]){
    
    Application::create();
    Application::instance()->exec(new RiemannApp());
    delete Application::instance()->getGame();
    return 0;
}