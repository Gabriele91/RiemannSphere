#include <stdafx.h>
#include <DrawSymbols.h>

///////////////////////
using namespace Easy3D;
using namespace RiemannSphere;
///////////////////////

///////////////////////

DrawSymbols::DrawSymbols(Easy3D::Camera *camera,
						 Easy3D::Render *render,
						 const Easy3D::Utility::Path& infinity,
						 const Easy3D::Utility::Path& zero,
						 const Easy3D::Utility::Path& point)
						:infinity(infinity)
						,zero(zero)
						,point(point)
						,camera(camera)
						,render(render)
{

}
			
void DrawSymbols::drawInfinity(const Easy3D::Vec3& pos,
							   const Easy3D::Vec2& scale,
							   float cstart,float cend,
							   Easy3D::Color color){
	infinity.bind();
	color.a=getColorIntensity(pos,cstart,cend)*255;
	drawSymbolIn3DScene(pos,scale,color);
}

void DrawSymbols::drawZero(const Easy3D::Vec3& pos,
						   const Easy3D::Vec2& scale,
						   float cstart,float cend,
						   Easy3D::Color color){
	zero.bind();
	color.a=getColorIntensity(pos,cstart,cend)*255;
	drawSymbolIn3DScene(pos,scale,color);
}
void DrawSymbols::drawPoint(const Easy3D::Vec3& pos,
							const Easy3D::Vec2& scale,
							float cstart,float cend,
							Easy3D::Color color){
	point.bind();
	color.a=getColorIntensity(pos,cstart,cend)*255;
	drawSymbolIn3DScene(pos,scale,color);
}

float DrawSymbols::getColorIntensity(const Easy3D::Vec3& vec,
									 float cstart,float cend){
	Vec3 dir=camera->getGlobalView().mul(Vec4(vec,0.0)).xyz().getNormalize();
	float zcord=Math::saturate(((dir.z+1.0f)*0.5f-cstart)/(cend-cstart));
	return zcord;
}

void DrawSymbols::drawSymbolIn3DScene(const Easy3D::Vec3& pos,
									  const Easy3D::Vec2& scale,
									  const Easy3D::Color& color){
    //change state
	auto mZbuffer=render->getZBufferState();
    auto mState=render->getMatrixsState();
	Easy3D::Render::MatrixsState localMState;
	//3D to 2D
	Easy3D::Vec2 post2d=camera->getScreenPointFrom3DSpace(pos);
	//disable zbuffer
	render->setZBufferState(false);
	//projection matrix	
	//projection is always the same
	Vec2 viewport(Application::instance()->getScreen()->getWidth(),
				  Application::instance()->getScreen()->getHeight());
	localMState.projection.setOrtho(0,viewport.x, 0,viewport.y, 0,1);
	//model matrix
	localMState.modelview.m00=scale.x;
	localMState.modelview.m11=scale.y;
	localMState.modelview.m30=post2d.x;
	localMState.modelview.m31=viewport.y-post2d.y;
    //draw
    render->setMatrixsState(localMState);
	render->drawColorUVSprite(color);
	//reset
    render->setMatrixsState(mState);
    render->setZBufferState(mZbuffer);

}
///////////////////////