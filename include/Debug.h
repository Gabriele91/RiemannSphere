#ifndef DEBUG_H
#define DEBUG_H

#include <Config.h>
#include <EString.h>
#include <ostream>

namespace Easy3D {

	class Debug{
	public:

		static void breakpoint();
		static std::ostream& message();
		template<typename T>
		static void doassert(const T& v,
                             const char* value,
                             const char* fileSource,
                             int line){
            if(!v){
                message()<<"Assert : "<<value<<" : "<<line<<" : "<<fileSource<<"\n";
                breakpoint();
            }
        }

		static void gpucheckerrors(const char* fileSource,int line);

	};

#ifdef _DEBUG
	
	#define DEBUG_MODE "debug"
	#define DEBUG_ENABLE
	#define DEBUG_MESSAGE( x ) Easy3D::Debug::message()<<x<<"\n"
	#define DEBUG_ASSERT( x ) Easy3D::Debug::doassert((x),#x,__FILE__,__LINE__)
	#define DEBUG_ASSERT_MSG( x,y ) if(!(x)){Easy3D::Debug::message()<<y<<"\n";} Easy3D::Debug::doassert((x),#x,__FILE__,__LINE__)
	#define DEBUG_ASSERT_REPLACE( x ) DEBUG_ASSERT(x)
	#define DEBUG_ASSERT_MGS_REPLACE( x,y ) DEBUG_ASSERT_MSG(x,y)
	#define DEBUG_BREAKPOINT() Easy3D::Debug::breakpoint()
	#define CHECK_GPU_ERRORS() Easy3D::Debug::gpucheckerrors(__FILE__,__LINE__)

#else

	#define DEBUG_MODE "release"
	#define DEBUG_DISABLE
	#define DEBUG_MESSAGE( x )
	#define DEBUG_ASSERT( x )
	#define DEBUG_ASSERT_MSG( x,y )
	#define DEBUG_ASSERT_MGS_REPLACE( x,y ) x
	#define DEBUG_ASSERT_REPLACE( x ) x
	#define DEBUG_BREAKPOINT()
	#define CHECK_GPU_ERRORS()

#endif


};

#endif
