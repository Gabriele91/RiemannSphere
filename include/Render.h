#ifndef SOLARRENDER_H
#define SOLARRENDER_H

#include <Config.h>
#include <Math3D.h>
#include <Blend.h>
#include <Color.h>
#include <Camera.h>

namespace Easy3D {
    
    ////////////
    class Scene;
    ////////////
    
	class Render {
        
        friend class Scene;
		void __renderInit();
        
    public:
        
        enum class CullFace :  GLint {
            FRONT=GL_FRONT,
            BACK=GL_BACK,
            DISABLE=0
        };
        
		struct BlendState{
			int src,dst,enable;
            //constructor
            BlendState():enable(false){}
            BlendState(int src,int dst):src(src),dst(dst),enable(true){}
            //operators
			bool operator==(const BlendState& bs) const {
				return src==bs.src && dst==bs.dst && enable == bs.enable;
			}
			bool operator!=(const BlendState& bs) const {
				return src!=bs.src || dst!=bs.dst || enable != bs.enable;
			}
		};
        
		struct MatrixsState{
			Mat4 projection,modelview;
            //values
            MatrixsState(){}
            MatrixsState(const Mat4 & proj, const Mat4 & modelView)
            :projection(proj)
            ,modelview(modelView){}
            
            MatrixsState(Camera & cam)
            :projection(cam.getProjectionMatrix())
            ,modelview(cam.getGlobalView()){}
            
            //operators
			bool operator==(const MatrixsState& bs) const {
				return modelview==bs.modelview && projection==bs.projection;
			}
			bool operator!=(const MatrixsState& bs) const {
				return modelview!=bs.modelview || projection!=bs.projection;
			}
		};
        
        struct CullFaceState{
            CullFace cullface;
            //cullface
            CullFaceState(CullFace culllFace=CullFace::DISABLE):cullface(culllFace){}
            CullFaceState(int cullFace){
                switch (cullFace) {
                    case GL_FRONT: cullface=CullFace::FRONT;    break;
                    case GL_BACK:  cullface=CullFace::BACK;     break;
                    default:       cullface=CullFace::DISABLE; break;
                }
            }
            //operators
			bool operator==(const CullFaceState& cl)const{
				return cullface==cl.cullface;
			}
			bool operator!=(const CullFaceState& cl)const{
				return cullface!=cl.cullface;
			}
            //cast operator
            operator int () const {
                return (int)cullface;
            }
            operator CullFace () const {
                return cullface;
            }
            
        };
        
        struct ZBufferState{
            bool zbuffer;
            //zbuffer
            ZBufferState(bool zBuffer):zbuffer(zBuffer){}
            //operators
			bool operator==(const ZBufferState& zb)const{
				return zbuffer==zb.zbuffer;
			}
			bool operator!=(const ZBufferState& zb)const{
				return zbuffer!=zb.zbuffer;
			}
            //cast operator
            operator bool () const{
                return zbuffer;
            }
            
        };
        
        struct ClearColorState{
            Color color;
            //color
            ClearColorState(const Color& color=Color()):color(color){}
            ClearColorState(uchar *color):color(Color(color)){}
            ClearColorState(uchar r,uchar g,uchar b,uchar a):color(Color(r,g,b,a)){}
            //operators
			bool operator==(const ClearColorState& c)const{
				return color==c.color;
			}
			bool operator!=(const ClearColorState& c)const{
				return color!=c.color;
			}
            //cast operator
            operator Color& (){
                return color;
            }
            operator const Color& () const{
                return color;
            }
            
        };
        
        struct ColorState{
            Color color;
            //color
            ColorState(const Color& color=Color()):color(color){}
            ColorState(uchar *color):color(Color(color)){}
            ColorState(uchar r,uchar g,uchar b,uchar a):color(Color(r,g,b,a)){}
            //operators
			bool operator==(const ClearColorState& c)const{
				return color==c.color;
			}
			bool operator!=(const ClearColorState& c)const{
				return color!=c.color;
			}
            //cast operator
            operator Color& (){
                return color;
            }
            operator const Color& () const{
                return color;
            }
            
        };
        
        struct ClientState{
            
            int clientState;
            
            enum{
                NONE=0,
                VERTEX=1,
                UVMAP=2,
                COLOR=4,
                NORMAL=8
            };
            
            //ClientState
            ClientState(int clientState=NONE):clientState(clientState){}
            //methos
            void addState(int state){
                clientState|=state;
            }
            void deleteState(int state){
                clientState^=state;
            }
            
            //vertexs
            void enableVertexs(){
                addState(VERTEX);
            }
            void disableVertexs(){
                deleteState(VERTEX);
            }
            
            //uvmapping
            void enableUVMap(){
                addState(UVMAP);
            }
            void disableUVMap(){
                deleteState(UVMAP);
            }
            
            //colors
            void enableVertexsColor(){
                addState(COLOR);
            }
            void disableVertexsColor(){
                deleteState(COLOR);
            }
            
            //normals
            void enableVertexsNormals(){
                addState(NORMAL);
            }
            void disableVertexsNormals(){
                deleteState(NORMAL);
            }
            
            //operators
			bool operator==(const ClientState& zb)const{
				return clientState==zb.clientState;
			}
			bool operator!=(const ClientState& zb)const{
				return clientState!=zb.clientState;
			}
            //cast operator
            operator int () const{
                return clientState;
            }
            
            
            
        };

        struct TextureState{
        
            int textureState;
            
            enum{
                NONE=0,     //000
                TEXTURE1D=1,//001
                TEXTURE2D=2,//010
                TEXTURE3D=4 //100
            };
            
            //ClientState
            TextureState(int textureState=NONE):textureState(textureState){}
            
            //methos
            void addState(int state){
                textureState|=state;
            }
            void deleteState(int state){
                textureState^=state;
            }
            
            
            void enableTexture1D(){
                addState(TEXTURE1D);
            }
            void disableTexture1D(){
                deleteState(TEXTURE1D);
            }
            
            
            void enableTexture2D(){
                addState(TEXTURE2D);
            }
            void disableTexture2D(){
                deleteState(TEXTURE2D);
            }
            
            
            void enableTexture3D(){
                addState(TEXTURE3D);
            }
            void disableTexture3D(){
                deleteState(TEXTURE3D);
            }
            
            //operators
			bool operator==(const TextureState& t)const{
				return textureState==t.textureState;
			}
			bool operator!=(const TextureState& t)const{
				return textureState!=t.textureState;
			}
        
        };
        
		Render();
        
        ClearColorState getClearColorState();
        void setClearColorState(const ClearColorState& cf);
        void doClear();
        
        ColorState getColorState();
        void setColorState(const ColorState& cs);
        
        ClientState getClientState();
        void setClientState(const ClientState& cf);
        
        ZBufferState getZBufferState();
        void setZBufferState(const ZBufferState& cf);
        
        CullFaceState getCullFaceState();
        void setCullFaceState(const CullFaceState& cf);
        
		BlendState getBlendState() const;
		void setBlendState(const BlendState& bs);
        
		TextureState getTextureState() const;
		void setTextureState(const TextureState& ts);
		
		MatrixsState getMatrixsState() const;
		void getMatrixsState(MatrixsState& ms) const;
		void setMatrixsState(const MatrixsState& ms);
        
        void setModelView( Object& modelview );
        void setModelView( const Mat4& modelview );
        void setModelView( Object& view, Object& model );
        void setModelView( Camera& view, Object& model );
        void setModelView( const Mat4& view, const Mat4& model );
	
        //easy
        void drawCube();
        void drawSphere(int ring=20,int settor=20);
        void drawColorCube(const Color& color);
        void drawColorSphere(const Color& color,int ring=20,int settor=20);
		
	};

};

#endif