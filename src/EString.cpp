#include <stdafx.h>
#include <stdafx.h>
#include <EString.h>
////////////////////////
using namespace Easy3D;
////////////////////////

//
int String::find(const String& value) const{
	return basic_string::find(value);
}
int String::rfind(const String& value) const{
	return basic_string::find_last_of(value);
}
///
void String::split(const String& delimiters , 
				   std::vector<String>& tokens){
    // Skip delimiters at beginning.
    std::string::size_type lastPos = find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    std::string::size_type pos     = find_first_of(delimiters, lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = find_first_of(delimiters, lastPos);
    }
}
///
String String::toLower() const{
  String out(*this); 
  for (unsigned int i=0;i<size();++i){ 
		if(out[i]<='Z' && out[i]>='A') 
			out[i]-=('Z'-'z');
  }
  return out;
} 
String String::toUpper() const{
  String out(*this); 
  for (unsigned int i=0;i<size();++i){ 
		if(out[i]<='z' && out[i]>='a') 
			out[i]+=('Z'-'z');
  }
  return out;
} 
//String buffer for printf
String Easy3D::operator*(const String& s,const char* c){
	return String::toString(c);
}
String Easy3D::operator*(const String& s,char* c){
	return String::toString(c);
}
String Easy3D::operator*(const String& s,int c){ return String::toString(c); }
String Easy3D::operator*(const String& s,unsigned int c){ return String::toString(c); }
String Easy3D::operator*(const String& s,float c){ return String::toString(c); }
String Easy3D::operator*(const String& s,double c){ return String::toString(c); }
///
//converti una stringa a qualunque tipo
double String::toDouble()  const{
	return strtod(c_str() ,NULL);
}
int String::toInt()  const{
	return atoi(c_str());
}
const char* String::toChars()  const{
	return c_str();
}
float String::toFloat()  const{
	return (float)atof(c_str());
}
//

//int Easy3D::operator
String Easy3D::operator+(const String& s,int v){ return s+String::toString(v);  }
String Easy3D::operator+(int v,const String& s){ return String::toString(v)+s;  }
String Easy3D::operator+(const String& s,unsigned int v){ return s+String::toString(v);  }
String Easy3D::operator+(unsigned int v,const String& s){ return String::toString(v)+s;  }

String Easy3D::operator+(const String& s,long v){ return s+String::toString(v);  }
String Easy3D::operator+(long v,const String& s){ return String::toString(v)+s;  }
String Easy3D::operator+(const String& s,unsigned long v){ return s+String::toString(v);  }
String Easy3D::operator+(unsigned long v,const String& s){ return String::toString(v)+s;  }

String Easy3D::operator+(const String& s,short v){ return s+String::toString(v);  }
String Easy3D::operator+(short v,const String& s){ return String::toString(v)+s;  }
String Easy3D::operator+(const String& s,unsigned short v){ return s+String::toString(v);  }
String Easy3D::operator+(unsigned short v,const String& s){ return String::toString(v)+s;  }

//float Easy3D::operator
String Easy3D::operator+(const String& s,float v){ return s+String::toString(v);  }
String Easy3D::operator+(float v,const String& s){ return String::toString(v)+s;  }
//double Easy3D::operator
String Easy3D::operator+(const String& s,double v){ return s+String::toString(v);  }
String Easy3D::operator+(double v,const String& s){ return String::toString(v)+s;  }
//char Easy3D::operator
String Easy3D::operator+(char c,const String& s){ return String::toString(c)+s;  }
String Easy3D::operator+(const String& s,char c){ return s+String::toString(c);  }