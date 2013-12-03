#include "stdafx.h"
#include <Timer.h>
#include <WindowsApp.h>
#include <WindowsScreen.h>
#include <WindowsInput.h>
#include <Debug.h>
//window
#include <direct.h>
#include <shlobj.h>
//
///////////////////////
using namespace Easy3D;

WindowsApp::WindowsApp(const String& name)
		   :Application(){
   screen=(Screen*)new WindowsScreen();
   input=(Input*)new WindowsInput();
   //savename
   appname=name;
   /////////////////////////////////////
   //create appdirectory
   //init appdata folder
   TCHAR szPath[MAX_PATH];
   SHGetFolderPathA(((WindowsScreen*)getScreen())->hWind,
                    CSIDL_APPDATA|CSIDL_FLAG_CREATE,
                    NULL,
                    0,
                    szPath);
   dataPath = String(szPath) + '/' + appname;
   //create directory
   CreateDirectory(dataPath.c_str(),0);
   //get errors
   DWORD error=GetLastError();
   DEBUG_ASSERT(error != ERROR_PATH_NOT_FOUND);
   //DEBUG_ASSERT(error != ERROR_ALREADY_EXISTS);
   /////////////////////////////////////
	//not exit form loop
	doexit=false;
}

WindowsApp::~WindowsApp(){
	//delete screen
	delete screen;
	screen=NULL;
	//delete input
	delete input;
	input=NULL;
}

bool WindowsApp::loadData(const String& path,void*& ptr,size_t &len){
	//open
	FILE *pfile=fopen(path,"rb");
	DEBUG_MESSAGE("load file: "<<path); 
	DEBUG_ASSERT_MSG(pfile,"error load file: "<<path);
		//get size
		fseek(pfile,0,SEEK_END);
		len=ftell(pfile);
		fseek(pfile,0,SEEK_SET);
		//read
		ptr=malloc(len*sizeof(char)+1);
		fread(ptr,len,1,pfile);
		(*((char*)ptr+len))='\0';
	//close
	fclose(pfile);
	return pfile!=NULL;
}

String WindowsApp::appDataDirectory(){
	return dataPath;
}

String WindowsApp::appWorkingDirectory(){
	char cCurrentPath[MAX_PATH];
	if(!_getcwd(cCurrentPath, sizeof(cCurrentPath))){
		DEBUG_MESSAGE("Error get working directory: "<<errno);
	}
	return cCurrentPath;
}

String WindowsApp::appResourcesDirectory(){
	return appWorkingDirectory();
}	

void WindowsApp::exit(){
	PostQuitMessage(0);
	doexit=true;
}

void WindowsApp::loop(){	
	//
	Timer timer;
	double msToSleep=1000.0/(static_cast<double>(screen->getFrameRate()));
	double millipass=0;
	double dt=0;
	double sleepTime=0;
	//start timer	
	timer.start();
	//set current context
	screen->acquireContext();
	//draw loop
	while( !input->getClose() && !doexit ) {
		//get timer values
		millipass=timer.getGetCounter();
		//calc dt and sleep time
		sleepTime=msToSleep-millipass;
		while(sleepTime>0 && sleepTime<60000 ){
			Sleep(sleepTime>10?1:0);
			millipass=timer.getGetCounter();
			sleepTime=msToSleep-millipass;
		}
		//calc dt
		dt=millipass/1000.0;
		timer.reset();
        //save dt
        lastDeltaTime=dt;
		//update
		update(dt);	
		//update opengl
		screen->swap();
    }
}

void WindowsApp::exec(Game *ptrMainInstance){
	mainInstance=ptrMainInstance;
	mainInstance->start();
	loop();
	mainInstance->end();
}

void WindowsApp::update(float dt){
	input->update();
	mainInstance->run(dt);
}

bool WindowsApp::onlyPO2(){
	return true;
}

bool  WindowsApp::openSaveDialog(const String& title,
								 const String& path,
								 const std::vector<String>& types,
								 Utility::Path& out){
	//
	DEBUG_ASSERT(types.size()%2==0);
	//open dialog
	OPENFILENAME saveDialog;
	char szFileName[MAX_PATH] = "";
	ZeroMemory(&saveDialog, sizeof(OPENFILENAME));
	String typesList;

	for(size_t i=0;i!=types.size();++i){
		if((i+1)&0x1)
			typesList+=types[i]+'\0';
		else
			typesList+="*."+types[i]+'\0';

	}

	saveDialog.lStructSize= sizeof(OPENFILENAME);
	saveDialog.hwndOwner = ((WindowsScreen*)getScreen())->hWind;
	saveDialog.lpstrFilter = TEXT(typesList.c_str());
	saveDialog.lpstrTitle  = TEXT(title.c_str());
	saveDialog.lpstrFile = szFileName;
	saveDialog.nMaxFile = MAX_PATH;
	saveDialog.Flags = OFN_EXPLORER | 
					   OFN_PATHMUSTEXIST | 
					   OFN_HIDEREADONLY | 
					   OFN_OVERWRITEPROMPT;

     bool isfullscreen=getScreen()->isFullscreen();
     bool success=false;
    
     if(isfullscreen){
        getScreen()->setFullscreen(false);
		//update draw (driver bug...)
		getScreen()->swap();
	 }

	if(GetSaveFileName(&saveDialog)){
		out=szFileName;
		success=true;
	}

    if(isfullscreen)
        getScreen()->setFullscreen(true);

	return success;
}