#ifndef TEXTURE_H
#define TEXTURE_H

#include <Config.h>
#include <Math3D.h>
#include <EString.h>
#include <Utility.h>
#include <iostream>
#include <Types.h>

namespace Easy3D {

	class Texture {
		//
		bool bBilinear,chBlr,bMipmaps,chMps;
		uint width,height;
        uint gpuid;
        //
        Utility::Path rpath;
        bool loaded;
		//
	public:
		//
		Texture();
		Texture(const String& pathfile);
		//destructor
		virtual ~Texture();
		//
		void bind(uint ntexture=0);
		//load methods
		virtual bool load();
		virtual bool load(const String& pathfile);
		virtual bool unload();
		bool loadFromBinaryData(std::vector<uchar>& bytes,
								uint width,
								uint height,
								uint format,
								uint type);
		//settings
		bool bilinear();
		bool bilinear(bool value);
		bool mipmaps();
		bool mipmaps(bool value);
		//query
		DFORCEINLINE uint getWidth(){ return width; }
		DFORCEINLINE uint getHeight(){ return height; }
		//overload
		bool operator ==(const Texture&) const;
		bool operator !=(const Texture&) const;
        //get if is loaded
        DFORCEINLINE bool isLoaded(){
            return loaded;
        }
	};

};

#endif