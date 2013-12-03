#ifndef APPLICATION_H
#define APPLICATION_H

#include <Config.h>
#include <Screen.h>
#include <Input.h>
#include <Utility.h>
#include <EString.h>

namespace Easy3D {
	
	class Game;
	class Audio;
    class ResourcesGroup;

	/**
    * Abstract class, represent the application instance
	* @class Application <Application.h>
    */
	class Application{

	public:
		/**
		* create singleton (if already created raises an exception)
		*/
		static Application *create(const String& name);
		/**
		* return singleton istance
		*/
		static Application *instance();
		/**
		* destroy an application
		*/
		virtual ~Application();
		/**
		* load a binary file
		* @return succes
		*/
		virtual bool loadData(const String& path,void*& ptr,size_t &len)=0;
		virtual bool loadFromResouce(const String& path,void*& ptr,size_t &len){
            return loadData(appResourcesDirectory()+'/'+path, ptr, len);
        }
		virtual bool loadFromData(const String& path,void*& ptr,size_t &len){
            return loadData(appDataDirectory()+'/'+path, ptr, len);
        }
		virtual bool loadFromWDir(const String& path,void*& ptr,size_t &len){
            return loadData(appWorkingDirectory()+'/'+path, ptr, len);
        }
		/**
		* where you can seve files data
		* @return path
		*/
		virtual String appDataDirectory()=0;
		/**
		* application root (read only)
		* @return path
		*/
		virtual String appWorkingDirectory()=0;
		/**
		* resources directory (read only)
		* @return path
		*/
		virtual String appResourcesDirectory()=0;			
		/**
		* application exit method
		*/
		virtual void exit()=0;
		/**
		* application loop
		*/
		virtual void loop()=0;		
		/**
		* execute a instance application
		*/
		virtual void exec(Game *game)=0;
		/**
		* application update
		*/
		virtual void update(float dt)=0;
		/**
		* return true if device supports only power of two texture
		*/
		virtual bool onlyPO2()=0;
        /**
        * return last delta time
        */
        virtual float getLastDeltaTime(){
            return lastDeltaTime;
        }
		/**
		* return main instance 
		*/
		Game* getGame(){
			return mainInstance;
		}
		/**
		* return screen device
		*/
		Screen* getScreen(){
			return screen;
		}
		/**
		* return audio device
		*/
		Audio* getAudio(){
			return audio;
		}
		/**
		* return input device
		*/
		Input* getInput(){
			return input;
		}
		/**
		* open save dialog
		*/
		virtual bool openSaveDialog(const String& title,
									const String& path,
									const std::vector<String>& types,
									Utility::Path& out)=0;
		/**
		* save a resourcesGroup
		*/
		virtual void addResourcesGroup(ResourcesGroup *rsGr){};
		/**
		* erase a resourcesGroup
		*/
		virtual void eraseResourcesGroup(ResourcesGroup *rsGr){};

	protected:
        
        String appname;
        float lastDeltaTime;
		Game *mainInstance;
		Screen *screen;
		Input *input;
		Audio *audio;
		Application();
		static Application* appSingleton;

	};

};

#endif
