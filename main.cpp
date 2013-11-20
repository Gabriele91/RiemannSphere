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
    RiemannGui::RiemannMenu method;
    RiemannGui::RiemannFormula formula;
    RiemannGui::RiemannFormula iterations;
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
         ,method(Table("assets/method.e2d"))
         ,formula(Table("assets/formula.e2d"))
         ,iterations(Table("assets/iterations.e2d"))
	     ,poly(NULL){}
    
	virtual ~RiemannApp(){}

    enum SCENE{
        RIEMANN_SCENE_GEODESIC=0,
        RIEMANN_SCENE=1,
        RIEMANN_GLSL_SCENE=2
    };
    
    enum GUI_STATE{
        GUI_DRAW,
        GUI_PRINT,
        CLEAN_DRAW
    };
    
    
                       
    void onStart(){
        ////////////////////////////////////////////////////////
        //default poly
        Table startTable("function.test.e2d");
        poly=new RiemannSphere::Polynomial<double>(startTable);
        ////////////////////////////////////////////////////////
        //GUI
        ////////////////////////////////////////////////////////
        formula.setText(startTable.getString("constants"));
        //set filters
        iterations.setFilter([](char cin)->bool{
            return ( 0<=(cin-'0') && (cin-'0')<10 ) ;//numbers
        });
        iterations.setText(String::toString(poly->iterations));
		//focus events
		auto focusEvent=[this](bool diable){
			if(diable)
				currentRiemann()->lock();
			else 
				currentRiemann()->unlock();
		};
		//set focus events
		iterations.setFocus(focusEvent);
		formula.setFocus(focusEvent);
        ////////////////////////////////////////////////////////
        menu.addOnClick("exit", [this](bool){
            onKeyDown(Key::ESCAPE);
        });
        menu.addOnClick("reload", [this](bool){
            poly->recalcPolynomial(formula.getText());
            poly->iterations=iterations.getText().toInt();
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
        ////////////////////////////////////////////////////////
        method.addRadioEvent([this](const Easy3D::String& name){
            if(name=="newton")
                poly->method=Iterations::NEWTON;
            else if(name=="halley")
                poly->method=Iterations::HALLEY;
            else if(name=="halley4")
                poly->method=Iterations::HALLEY4;
            else if(name=="schroeder")
                poly->method=Iterations::SCHROEDER;
            else if(name=="schroeder4")
                poly->method=Iterations::SCHROEDER4;
        });
        method.crackAButton(Iterations::Names[poly->method]);
        ////////////////////////////////////////////////////////
        addState(GUI_DRAW,new Easy3D::StateLambda([this](float dt){
            //update ui
            menu.update(dt);
            method.update(dt);
            //draw ui
            menu.draw(this);
            method.draw(this);
            formula.draw(this);
            iterations.draw(this);
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
        addState(CLEAN_DRAW,new Easy3D::StateLambda([this](float dt){}));
        //enable draw
        setCurrentState(GUI_DRAW);
        ////////////////////////////////////////////////////////
        //SCENES
        ////////////////////////////////////////////////////////
        //load font
        fprint.load("assets/game.font.e2d");
		//scenes
        addSceneAndActive(RIEMANN_SCENE_GEODESIC, fixSceneCast(new RiemannSphere::RiemannSceneGeodesic(poly)));
        addScene(RIEMANN_GLSL_SCENE, fixSceneCast(new RiemannSphere::RiemannSceneGLSL(poly)));
        //add input keyboard
        getInput()->addHandler(dynamic_cast<Easy3D::Input::KeyboardHandler *>(this));

    }
    void onRun(float dt){}       

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
	Easy3D::Scene *reloadScene(int uid){
		if(uid==RIEMANN_SCENE_GEODESIC)
			return fixSceneCast(new RiemannSphere::RiemannSceneGeodesic(poly));
		else if(uid==RIEMANN_GLSL_SCENE)
			return fixSceneCast(new RiemannSphere::RiemannSceneGLSL(poly));
		return NULL;
	}
	void selectScene(int uid){
		if(sceneActive()!=uid){
			//get options draw
			auto dpi=currentRiemann()->getDrawOptions();
			auto cpi=currentRiemann()->getCameraPositionInfo();
			//
			popScene();
			activeScene(uid);
			//
			//set options draw
			currentRiemann()->setDrawOptions(dpi);
			currentRiemann()->setCameraPositionInfo(cpi);
		}
	}

	virtual void onKeyPress(Easy3D::Key::Keyboard key){
		if(Key::F5==key && getCurrentStateID()==CLEAN_DRAW)
			setCurrentState(GUI_DRAW);
		else if(Key::F5==key)
			setCurrentState(CLEAN_DRAW);
	}
	virtual void onKeyDown(Easy3D::Key::Keyboard key){
		if(key==Key::C)
			selectScene(RIEMANN_SCENE_GEODESIC);
		else
		if(key==Key::G)
			selectScene(RIEMANN_GLSL_SCENE);
		else
		if(key==Key::R) {
			//get scene id
			int uid=sceneActive();
			//get options draw
			auto dpi=currentRiemann()->getDrawOptions();
			auto cpi=currentRiemann()->getCameraPositionInfo();
			//scene
			auto scene=eraseScene(uid);
			scene->onEnd();
			//delete scene
			delete scene;
			//active
			addSceneAndActive(uid, reloadScene(uid));
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
		if(poly) delete poly;   
	}
    
    
        
};

int main(int argc,const char *args[]){
    
    Application::create();
    Application::instance()->exec(new RiemannApp());
    delete Application::instance()->getGame();
    return 0;
}