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
#include <RiemannDialog.h>

//last exemple http://pastebin.com/RGFiAuBs

using namespace Easy3D;
using namespace RiemannSphere;

class RiemannApp : public Game, 
				   public Easy3D::Input::KeyboardHandler, 
				   public Easy3D::Input::MouseHandler {
    
                       
    //UI
    RiemannGui::RiemannMenu menu;
    RiemannGui::RiemannMenu method;
    RiemannGui::RiemannFormula formula;
    RiemannGui::RiemannFormula iterations;
    RiemannGui::RiemannDialog dialog;
    //font style
    Easy3D::Font fprint;    //font
    //bool is reloded
    bool isreloded;
    //polynomail
    Polynomial<double> *poly;
    
    public:
    
    
    RiemannApp()
    :Game("RiemannSphere",
          1280,
          720,
          32,
          24,
          false,
          Screen::MSAAx4)
         ,menu(Table(Application::instance()->appResourcesDirectory()+'/'+"assets/menu.e2d"))
         ,method(Table(Application::instance()->appResourcesDirectory()+'/'+"assets/method.e2d"))
         ,formula(Table(Application::instance()->appResourcesDirectory()+'/'+"assets/formula.e2d"))
         ,iterations(Table(Application::instance()->appResourcesDirectory()+'/'+"assets/iterations.e2d"))
	     ,dialog(Table(Application::instance()->appResourcesDirectory()+'/'+"assets/dialog.e2d"))
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
        //init is reloded
        isreloded=true;
        ////////////////////////////////////////////////////////
        //get fullscreen
        Utility::Path dirtable(Application::instance()->appDataDirectory()+"/fullscreen.e2d");
        Table fullscreen;
        //read file
        if(dirtable.existsFile())
            fullscreen.loadFromFile(dirtable);
        else
            fullscreen.set("fullscreen", getScreen()->isFullscreen()? "true" : "false");
        //enable o disable full screen mode
        bool bScreenMode=fullscreen.getString("fullscreen", getScreen()->isFullscreen()? "true" : "false")=="true";
        //set mode
        if(getScreen()->isFullscreen()!=bScreenMode)
            getScreen()->setFullscreen(bScreenMode);
        //set ico menu
        if(bScreenMode)
            menu.crackAButton("fullscreen");
        ////////////////////////////////////////////////////////
        //default poly
        Table startTable(Application::instance()->appResourcesDirectory()+'/'+"function.test.e2d");
        poly=new RiemannSphere::Polynomial<double>(startTable);
        ////////////////////////////////////////////////////////
        //SCENES
        ////////////////////////////////////////////////////////
        //load font
        fprint.load(Application::instance()->appResourcesDirectory()+'/'+"assets/game.font.e2d");
		//scenes
        addSceneAndActive(RIEMANN_SCENE_GEODESIC, fixSceneCast(new RiemannSphere::RiemannSceneGeodesic(poly)));
        addScene(RIEMANN_GLSL_SCENE, fixSceneCast(new RiemannSphere::RiemannSceneGLSL(poly)));
        //add input keyboard
        getInput()->addHandler((Easy3D::Input::KeyboardHandler *)(this));
		getInput()->addHandler((Easy3D::Input::MouseHandler *)(this));
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
		iterations.setFocusEvent(focusEvent);
		formula.setFocusEvent(focusEvent);
        ////////////////////////////////////////////////////////
        menu.addOnClick("exit", [this](bool){
            onKeyDown(Key::ESCAPE);
        });
        menu.addOnClick("reload", [this](bool){
			reloadPoly();
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
        menu.addOnClick("fullscreen", [this](bool deactive){
            getScreen()->setFullscreen(deactive);
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
            onKeyDown(Key::R);
        });
        ////////////////////////////////////////////////////////
        method.crackAButton(Iterations::Names[poly->method]);
        menu.crackAButton("grid");
        ////////////////////////////////////////////////////////
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
            dialog.draw(this);
        }));
        addState(GUI_PRINT,new Easy3D::StateLambda([this](float dt){
            //save
            Image *screenImage=Image::getImageFromScreen(getScreen()->getWidth(),getScreen()->getHeight());

            Utility::Path savepath(".");
            std::vector<String> types;
            types.push_back(String("image (*.tga)"));
            types.push_back(String("tga"));

            if(Application::instance()->openSaveDialog("Save image","",types,savepath))
                screenImage->save(savepath);
            
            delete screenImage;
            setCurrentState(GUI_DRAW);
        }));
        addState(CLEAN_DRAW,new Easy3D::StateLambda([this](float dt){}));
        //enable draw
        setCurrentState(GUI_DRAW);
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
            //reload
            if(isreloded){
                onKeyDown(Key::R);
                isreloded=false;
            }
		}
	}
	void reloadPoly(){
		Easy3D::String errors;
		if(poly->recalcPolynomial(formula.getText(),errors)){
			poly->iterations=iterations.getText().toInt();
			onKeyDown(Key::R);
		}
		else{
			dialog.setText(errors);
			dialog.show();
		}
	}

	virtual void onMousePress(Easy3D::Vec2 pos,Easy3D::Key::Mouse buttom){
		//hide dialog
		dialog.hide();
	}
	virtual void onKeyPress(Easy3D::Key::Keyboard key){		
		//hide dialog
		dialog.hide();
		//disable/enable gui
		if(Key::F5==key && getCurrentStateID()==CLEAN_DRAW)
			setCurrentState(GUI_DRAW);
		else if(Key::F5==key)
			setCurrentState(CLEAN_DRAW);
	}
	virtual void onKeyDown(Easy3D::Key::Keyboard key){

		//key events
        if(key==Key::RETURN && (formula.hasFocus() || iterations.hasFocus()))
			reloadPoly();
        else
		if(key==Key::C)
			selectScene(RIEMANN_SCENE_GEODESIC);
		else
		if(key==Key::G)
			selectScene(RIEMANN_GLSL_SCENE);
		else
		if(key==Key::R) {
            //is reloded
            isreloded=true;
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
        //serialize full screen mode
        Table fullscreen;
        fullscreen.set("fullscreen", getScreen()->isFullscreen()? "true" : "false");
        String fullscreenString=fullscreen.serialize();
        //save seralize data
        Utility::Path dirtable(Application::instance()->appDataDirectory()+"/fullscreen.e2d");
        std::FILE *file=std::fopen(dirtable, "wb");
        if(file){
            std::fwrite(fullscreenString.c_str(), fullscreenString.size(), 1, file);
            std::fclose(file);
        }
        //remove data
		getInput()->removeHandler((Easy3D::Input::KeyboardHandler*)this);
		getInput()->removeHandler((Easy3D::Input::MouseHandler*)this);
		if(poly) delete poly;   
	}
    
    
        
};

int main(int argc,const char *args[]){
    
    Application::create();
    Application::instance()->exec(new RiemannApp());
    delete Application::instance()->getGame();
    return 0;
}