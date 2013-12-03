#ifndef TEXTURE_H
#define TEXTURE_H

#include <Config.h>
#include <Math3D.h>
#include <EString.h>
#include <Utility.h>
#include <iostream>
#include <Types.h>
#include <Image.h>

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
		bool loadFromImage(Easy3D::Image *image);
		//settings
		bool bilinear();
		bool bilinear(bool value);
		bool mipmaps();
		bool mipmaps(bool value);
		//query
		DFORCEINLINE uint getWidth() const { return width; }
		DFORCEINLINE uint getHeight() const { return height; }
		DFORCEINLINE uint getGpuID() const { return gpuid; }
		//overload
		bool operator ==(const Texture&) const;
		bool operator !=(const Texture&) const;
        //get if is loaded
        DFORCEINLINE bool isLoaded() const {
            return loaded;
        }
	};

};

#endif