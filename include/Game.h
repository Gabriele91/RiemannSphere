#ifndef GAME_H
#define GAME_H

#include <Config.h>
#include <Types.h>
#include <EString.h>
#include <Scene.h>

namespace Easy3D {
	

    /** 
    * Abstract class, represent the game instance
	* @class Game <Game.h>
    */
	class Game : public Scene {
	public:

		Game(const String& name,
			 uint width, 
			 uint height, 
			 uint bites=32,
			 uint FreamPerSecond=60,
			 bool fullscreen=false,
			 int defaultNoAA=0);
        
		virtual ~Game(){};


	private:

	};

};

#endif