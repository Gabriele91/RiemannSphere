#include <stdafx.h>
#include <Render.h>
#include <Application.h>

///////////////////////
using namespace Easy3D;

///////////////////////
//GLOBALS
static Render::ClientState __currentClientState;
//////////////////////

Render::Render()
{
}

void Render::__renderInit(){
	//OPENGL
	//view port
	glViewport(0, 0, Application::instance()->getScreen()->getWidth(),
                     Application::instance()->getScreen()->getHeight());
	//enable culling
    setCullFaceState(CullFaceState(CullFace::BACK));
	//enable z buffer
	setZBufferState(ZBufferState(true));
	//enable texturing	
	glEnable( GL_TEXTURE_2D );
	//enable state
    ClientState state(ClientState::VERTEX|ClientState::UVMAP);
    setClientState(state);
	//default status for blending    
	glEnable(GL_ALPHA_TEST);
    setBlendState(BlendState(BLEND::SRC::ALPHA,BLEND::ONE::MINUS::SRC::ALPHA));
}

Render::ClearColorState Render::getClearColorState(){
    float color[4];
    glGetFloatv(GL_COLOR_CLEAR_VALUE,color);
    return ClearColorState(Color(&color[0]));
}
void Render::setClearColorState(const ClearColorState& clearColor){
    glClearColor(clearColor.color.rNormalize(),
                 clearColor.color.gNormalize(),
                 clearColor.color.bNormalize(),
                 clearColor.color.aNormalize());
}


Render::ColorState Render::getColorState(){
    float color[4];
    glGetFloatv(GL_COLOR_CLEAR_VALUE,color);
    return ColorState(Color(&color[0]));
}
void Render::setColorState(const ColorState& cs){
    glColor4ubv(cs.color);
}

Render::ClientState Render::getClientState(){
    return __currentClientState;
}
void Render::setClientState(const Render::ClientState& cs){
    __currentClientState=cs;
    
    if(__currentClientState.clientState & ClientState::VERTEX)
        glEnableClientState(GL_VERTEX_ARRAY);
    else
        glDisableClientState(GL_VERTEX_ARRAY);
    
    if(__currentClientState.clientState & ClientState::UVMAP)
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    else
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
    if(__currentClientState.clientState & ClientState::NORMAL)
        glEnableClientState(GL_NORMAL_ARRAY);
    else
        glDisableClientState(GL_NORMAL_ARRAY);
    
    if(__currentClientState.clientState & ClientState::COLOR)
        glEnableClientState(GL_COLOR_ARRAY);
    else
        glDisableClientState(GL_COLOR_ARRAY);
        
}

void Render::doClear(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

Render::ZBufferState Render::getZBufferState(){
    
	GLboolean out;
	glGetBooleanv(GL_DEPTH_TEST,&out);
    return ZBufferState(out);
    
}
void Render::setZBufferState(const ZBufferState& zb){
    
    if(zb.zbuffer)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
    
}

Render::CullFaceState Render::getCullFaceState(){
    
	GLboolean out;
	glGetBooleanv(GL_CULL_FACE,&out);
    
    if(out){
        GLint out;
        glGetIntegerv(GL_CULL_FACE_MODE,&(out));
        return CullFaceState(out);
    }
    
    return CullFaceState(CullFace::DISABLE);
    
}
void Render::setCullFaceState(const CullFaceState& cf){
	if(cf.cullface==CullFace::DISABLE)
        glDisable( GL_CULL_FACE );
    else{
        glEnable( GL_CULL_FACE );
        glCullFace(cf.cullface==CullFace::FRONT?GL_FRONT:GL_BACK);
    }
}

Render::BlendState Render::getBlendState() const{
	Render::BlendState bs;
	//save blend
	glGetIntegerv(GL_BLEND_SRC_RGB , &bs.src);
	glGetIntegerv(GL_BLEND_DST_RGB , &bs.dst);
	bs.enable=glIsEnabled(GL_BLEND)!=false;
	return bs;
}
void Render::setBlendState(const BlendState& bs){
	//restore old blend state
	if(bs.enable) glEnable( GL_BLEND );
    else glDisable( GL_BLEND );
	glBlendFunc( bs.src, bs.dst );
}

Render::MatrixsState Render::getMatrixsState() const{
	Render::MatrixsState ms;
	//save matrixs
	glGetFloatv(GL_PROJECTION_MATRIX ,  ms.projection );
	glGetFloatv(GL_MODELVIEW_MATRIX , ms.modelview );
	return ms;
}
void Render::getMatrixsState(MatrixsState& ms) const{
	//save matrixs
	glGetFloatv(GL_PROJECTION_MATRIX,  ms.projection );
	glGetFloatv(GL_MODELVIEW_MATRIX , ms.modelview );
}
void Render::setMatrixsState(const MatrixsState& ms){
	//restore old matrixs state
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(ms.projection);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(ms.modelview);
}

void Render::setModelView( Object& modelview ){
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(modelview.getGlobalMatrix());
}
void Render::setModelView( const Mat4& modelview ){
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(modelview);
}
void Render::setModelView( Object& view, Object& model ){
    Mat4 modelview=view.getGlobalMatrix().mul(model.getGlobalMatrix());
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(modelview);
}
void Render::setModelView( Camera& view, Object& model ){
    Mat4 modelview=view.getGlobalView().mul(model.getGlobalMatrix());
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(modelview);
}
void Render::setModelView( const Mat4& view, const Mat4& model ){
    Mat4 modelview=view.mul(model);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(modelview);
}

void Render::drawCube(){
    //save state
    ClientState lastClientState=getClientState();
    //only vertex
    setClientState(ClientState(ClientState::VERTEX));
    // 8 of vertex coords
    GLfloat vertices[] = {
        0.5, 0.5, 0.5,  -0.5, 0.5, 0.5,  -0.5,-0.5, 0.5,   0.5,-0.5, 0.5,   // v0,v0.5,v2,v3 (front)
        0.5, 0.5, 0.5,   0.5,-0.5, 0.5,   0.5,-0.5,-0.5,   0.5, 0.5,-0.5,   // v0,v3,v4,v5 (right)
        0.5, 0.5, 0.5,   0.5, 0.5,-0.5,  -0.5, 0.5,-0.5,  -0.5, 0.5, 0.5,   // v0,v5,v6,v0.5 (top)
       -0.5, 0.5, 0.5,  -0.5, 0.5,-0.5,  -0.5,-0.5,-0.5,  -0.5,-0.5, 0.5,   // v0.5,v6,v7,v2 (left)
       -0.5,-0.5,-0.5,   0.5,-0.5,-0.5,   0.5,-0.5, 0.5,  -0.5,-0.5, 0.5,   // v7,v4,v3,v2 (bottom)
        0.5,-0.5,-0.5,  -0.5,-0.5,-0.5,  -0.5, 0.5,-0.5,   0.5, 0.5,-0.5    // v4,v7,v6,v5 (back)
    };
    //indices
    GLubyte indices[]  = {  0, 1, 2,   2, 3, 0,      // front
                            4, 5, 6,   6, 7, 4,      // right
                            8, 9,10,  10,11, 8,      // top
                            12,13,14, 14,15,12,      // left
                            16,17,18, 18,19,16,      // bottom
                            20,21,22, 22,23,20       // back
    };
    //pointer to vertexs
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    // deactivate vertex arrays after drawing
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);
    //set old client state
    setClientState(lastClientState);
}
void Render::drawSphere(int ring,int settor){
    
    //save state
    ClientState lastClientState=getClientState();
    //only vertex
    setClientState(ClientState(ClientState::VERTEX));
    
    //refactory: http://ozark.hendrix.edu/~burch/cs/490/sched/feb8/
    //will see:http://paulbourke.net/texture_colour/tiling/
    std::vector<GLfloat> vertices(ring*settor*3*6);
    int count=0;
    float scale=0.5f;
    #define push(x,y,z)\
     vertices[count++]=(x)*scale; vertices[count++]=(y)*scale; vertices[count++]=(z)*scale;

    //center
    for(int i = 0; i<ring; ++i){
    
        double lat0 = Math::PI * (-0.5 + (double) i / ring);
        double z0  =  std::sin(lat0);
        double zr0 =  std::cos(lat0);
        
        double lat1 = Math::PI * (-0.5 + (double) (i+1) / ring);
        double z1 =   std::sin(lat1);
        double zr1 =  std::cos(lat1);
        
        for(int j = 0; j < settor; ++j) {
            double lng = 2 * Math::PI * (double) j / settor;
            double x = std::cos(lng);
            double z = std::sin(lng);
            
            double lng2 = 2 * Math::PI * (double) (j+1)/ settor;
            double x2 = std::cos(lng2);
            double z2 = std::sin(lng2);
            
            //tri 1
            push( x * zr0, z0, z * zr0);
            push( x * zr1, z1, z * zr1);
            push(x2 * zr1, z1, z2 * zr1);
            //tri 2
            push(x2 * zr0, z0,z2 * zr0);
            push( x * zr0, z0,z * zr0);
            push(x2 * zr1, z1,z2 * zr1);
        }
    }
    
    //pointer to vertexs
    glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
    // deactivate vertex arrays after drawing
    glDrawArrays(GL_TRIANGLES, 0, vertices.size()/3.0);
    //set old client state
    setClientState(lastClientState);
}

void Render::drawColorCube(const Color& color){
    //save color
    ColorState lastColorState=getColorState();
    //draw
    setColorState(ColorState(color));
    drawCube();
    //set old state
    setColorState(lastColorState);
}
void Render::drawColorSphere(const Color& color,int ring,int settor){
    //save color
    ColorState lastColorState=getColorState();
    //draw
    setColorState(ColorState(color));
    drawSphere(ring,settor);
    //set old state
    setColorState(lastColorState);
}

