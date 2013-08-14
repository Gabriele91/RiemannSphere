#include <stdafx.h>
#include <Scene.h>
#include <Application.h>

///////////////////////
using namespace Easy3D;
///////////////////////

Easy3D::Screen* Scene::getScreen(){
	return Application::instance()->getScreen();
}
Easy3D::Audio* Scene::getAudio(){
	return Application::instance()->getAudio();
}
Easy3D::Input* Scene::getInput(){
	return Application::instance()->getInput();
}
Easy3D::Game* Scene::getGame(){
	return Application::instance()->getGame();
}