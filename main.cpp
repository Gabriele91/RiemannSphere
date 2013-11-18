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
#include <RiemannInterface.h>
#include <RiemannSceneGeodesic.h>
#include <RiemannSceneGLSL.h>
#include <RiemannMenu.h>
#include <RiemannFormula.h>

//last exemple http://pastebin.com/RGFiAuBs

using namespace Easy3D;
using namespace RiemannSphere;

class RiemannApp : public Game, public Easy3D::Input::KeyboardHandler {
    
                       
    //UI
    RiemannGui::RiemannMenu menu;
    RiemannGui::RiemannFormula formula;
    //font style
    Easy3D::Font fprint;    //font
    //polynomail
    Polynomial<double> *poly;
                       
    public:
    
    
    RiemannApp()
    :Game("RiemannSphere",
          1280,
          720,
          32,
          30,
          false,
          Screen::MSAAx8)
         ,menu(Table("assets/menu.e2d"))
         ,formula(Table("assets/formula.e2d")){}
    
    enum SCENE{
        RIEMANN_SCENE_GEODESIC=0,
        RIEMANN_SCENE=1,
        RIEMANN_GLSL_SCENE=2
    };
    
    enum GUI_STATE{
        GUI_DRAW,
        GUI_PRINT
    };
    
    
                       
    void onStart(){
        //parse a poly
        Table startTable("function.test.e2d");
        poly=new RiemannSphere::Polynomial<double>(startTable);
        formula.setText(startTable.getString("constants"));
        //load font
        fprint.load("assets/game.font.e2d");
		//scenes
        addSceneAndActive(RIEMANN_SCENE_GEODESIC, fixSceneCast(new RiemannSphere::RiemannSceneGeodesic(poly)));
        addScene(RIEMANN_GLSL_SCENE, fixSceneCast(new RiemannSphere::RiemannSceneGLSL(poly)));
        //add input keyboard
        getInput()->addHandler(dynamic_cast<Easy3D::Input::KeyboardHandler *>(this));
        //exit key
        menu.addOnClick("exit", [this](bool){
            onKeyDown(Key::ESCAPE);
        });
        menu.addOnClick("reload", [this](bool){
            poly->recalcPolynomial(formula.getText());
            onKeyDown(Key::R);
        });
        menu.addOnClick("print", [this](bool){
            setCurrentState(GUI_PRINT);
        });
        menu.addOnClick("grid", [this](bool deactive){
            currentRiemann()->drawGrid(!deactive);
        });
        menu.addOnClick("roots", [this](bool deactive){
            currentRiemann()->drawRoots(!deactive);
        });
        menu.addOnClick("infinite", [this](bool deactive){
            currentRiemann()->drawInfinite(!deactive);
        });
        menu.addOnClick("zero", [this](bool deactive){
            currentRiemann()->drawZero(!deactive);
        });
        
        addState(GUI_DRAW,new Easy3D::StateLambda([this](float dt){
            //update ui
            menu.update(dt);
            //draw ui
            menu.draw(this);
            formula.draw(this);
        }));
        addState(GUI_PRINT,new Easy3D::StateLambda([this](float dt){
            //save
            Image *screenImage=Image::getImageFromScreen(getScreen()->getWidth(),getScreen()->getHeight());
            int i=0; while(1){
                Utility::Path imgPath("image"+String::toString(i)+".tga");
                if(!imgPath.existsFile()){
                    screenImage->save(imgPath);
                    break;
                }
                ++i;
            };
            
            delete screenImage;
            setCurrentState(GUI_DRAW);
        }));
        //enable draw
        setCurrentState(GUI_DRAW);
    }
    
   void onRun(float dt){	//draw text

    }
                       
   RiemannInterface *currentRiemann(){
       if( sceneActive()==RIEMANN_SCENE_GEODESIC)
           return (RiemannInterface*)((RiemannSceneGeodesic*)getScene(RIEMANN_SCENE_GEODESIC));
       else if( sceneActive()==RIEMANN_GLSL_SCENE)
           return (RiemannInterface*)((RiemannSceneGLSL*)getScene(RIEMANN_GLSL_SCENE));
       return NULL;
   }
   Easy3D::Scene *fixSceneCast(RiemannSceneGeodesic *rmgeo){
       return (Easy3D::Scene*)((RiemannSceneGeodesic*)rmgeo);
   }
   Easy3D::Scene *fixSceneCast(RiemannSceneGLSL *rmgls){
       return (Easy3D::Scene*)((RiemannSceneGLSL*)rmgls);
   }
                       
    virtual void onKeyDown(Easy3D::Key::Keyboard key){
        //if(key==Key::V) activeScene(RIEMANN_SCENE);
        if(key==Key::C && sceneActive()!=RIEMANN_SCENE_GEODESIC){
            //get options draw
            auto dpi=currentRiemann()->getDrawOptions();
            auto cpi=currentRiemann()->getCameraPositionInfo();
            //
            popScene();
            activeScene(RIEMANN_SCENE_GEODESIC);
            //
            //set options draw
            currentRiemann()->setDrawOptions(dpi);
            currentRiemann()->setCameraPositionInfo(cpi);
        }
        else
        if(key==Key::G && sceneActive()!=RIEMANN_GLSL_SCENE){
            //get options draw
            auto dpi=currentRiemann()->getDrawOptions();
            auto cpi=currentRiemann()->getCameraPositionInfo();
            //
            popScene();
            activeScene(RIEMANN_GLSL_SCENE);
            //
            //set options draw
            currentRiemann()->setDrawOptions(dpi);
            currentRiemann()->setCameraPositionInfo(cpi);
        }
        else
        if(key==Key::R && sceneActive()==RIEMANN_SCENE_GEODESIC) {
            //get options draw
            auto dpi=currentRiemann()->getDrawOptions();
            auto cpi=currentRiemann()->getCameraPositionInfo();
            //scene
            auto scene=eraseScene(RIEMANN_SCENE_GEODESIC);
            //delete scene
            delete scene;
            //new scene
            auto newscene=fixSceneCast(new RiemannSphere::RiemannSceneGeodesic(poly));
            //active
            addSceneAndActive(RIEMANN_SCENE_GEODESIC, newscene);
            //set options draw
            currentRiemann()->setDrawOptions(dpi);
            currentRiemann()->setCameraPositionInfo(cpi);
        }
        else
        if(key==Key::R && sceneActive()==RIEMANN_GLSL_SCENE) {
            //get options draw
            auto dpi=currentRiemann()->getDrawOptions();
            auto cpi=currentRiemann()->getCameraPositionInfo();
            //scene
            auto scene=eraseScene(RIEMANN_GLSL_SCENE);
            //delete scene
            delete scene;
            //new scene
            auto newscene=fixSceneCast(new RiemannSphere::RiemannSceneGLSL(poly));
            //active
            addSceneAndActive(RIEMANN_GLSL_SCENE, newscene);
            //set options draw
            currentRiemann()->setDrawOptions(dpi);
            currentRiemann()->setCameraPositionInfo(cpi);
        }
        else
        if(key==Key::ESCAPE)
            Application::instance()->exit();
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