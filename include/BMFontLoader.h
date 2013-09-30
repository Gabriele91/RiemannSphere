#ifndef BMFONTLOADER_H
#define BMFONTLOADER_H

#include <Config.h>
#include <Font.h>
#include <Texture.h>
#include <Utility.h>

namespace Easy3D {

	class BMFontLoader{
	public:
		static bool load(Font& font,const Easy3D::Utility::Path& file);
	
	};

};

#endif