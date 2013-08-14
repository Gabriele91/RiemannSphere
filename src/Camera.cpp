#include <stdafx.h>
#include <Camera.h>
#include <Application.h>
////////////////////////////
using namespace Easy3D;
////////////////////////////
//set prospetive
void Camera::setPerspective(float angle,float n,float f){
	//set Perspective
	mProjMatrix.setPerspective(
		angle,
		Application::instance()->getScreen()->getWidth()/
		Application::instance()->getScreen()->getHeight(),
		n,
		f);
}
void Camera::setPerspective(float angle,float spectre,float n,float f){
	//set Perspective
	mProjMatrix.setPerspective(angle,spectre,n,f);
}
void Camera::setPerspective(float left, float right, float bottom,float top, float n, float f){
	//set Perspective
	mProjMatrix.setPerspective(left,right,bottom,top,n,f);
}
void Camera::setOrtogonal(float left, float right, float bottom,float top, float n, float f){
	//set Perspective
	mProjMatrix.setOrtho(left,right,bottom,top,n,f);
}
//culling methods
int Camera::sphereInFrustum(const Vec3& point, float radius) const{

	int result = INSIDE;
	float distance;

	for(int i=0; i < 6; ++i) {
		distance = planes[i].distance(point);
		if (distance < -radius) return OUTSIDE;
		else if (distance < radius) result = INTERSECT;
	}

	return result;

}
int Camera::boxInFrustum(const AABox &b) const{
    
	int result = INSIDE;
	for(int i=0; i < 6; i++) {
        
		if (planes[i].distance(b.getVertexP(planes[i].normal)) < 0)
			return OUTSIDE;
		else if (planes[i].distance(b.getVertexN(planes[i].normal)) < 0)
			result =  INTERSECT;
	}
	return(result);
    
}
int Camera::pointInFrustum(const Vec3 &point) const{
	int result = INSIDE;
	for(int i=0; i < 6; ++i) 
		if (planes[i].distance(point) < 0)
			return OUTSIDE;
	return result;
}
//set camera
void Camera::update(){
	//compute fastrum
	mViewProjMatrix=mProjMatrix.mul(getGlobalMatrix());
	//left
	planes[LEFT].normal.x = mViewProjMatrix[ 3] + mViewProjMatrix[ 0];
	planes[LEFT].normal.y = mViewProjMatrix[ 7] + mViewProjMatrix[ 4];
	planes[LEFT].normal.z = mViewProjMatrix[11] + mViewProjMatrix[ 8];
	planes[LEFT].d =		mViewProjMatrix[15] + mViewProjMatrix[12];
	planes[LEFT].normalize();
	//right
	planes[RIGHT].normal.x = mViewProjMatrix[ 3] - mViewProjMatrix[ 0];
	planes[RIGHT].normal.y = mViewProjMatrix[ 7] - mViewProjMatrix[ 4];
	planes[RIGHT].normal.z = mViewProjMatrix[11] - mViewProjMatrix[ 8];
	planes[RIGHT].d		   = mViewProjMatrix[15] - mViewProjMatrix[12];
	planes[RIGHT].normalize();
	//bottom
	planes[BOTTOM].normal.x = mViewProjMatrix[ 3] + mViewProjMatrix[ 1];
	planes[BOTTOM].normal.y = mViewProjMatrix[ 7] + mViewProjMatrix[ 5];
	planes[BOTTOM].normal.z = mViewProjMatrix[11] + mViewProjMatrix[ 9];
	planes[BOTTOM].d        = mViewProjMatrix[15] + mViewProjMatrix[13];
	planes[BOTTOM].normalize();
	//top
	planes[TOP].normal.x = mViewProjMatrix[ 3] - mViewProjMatrix[ 1];
	planes[TOP].normal.y = mViewProjMatrix[ 7] - mViewProjMatrix[ 5];
	planes[TOP].normal.z = mViewProjMatrix[11] - mViewProjMatrix[ 9];
	planes[TOP].d        = mViewProjMatrix[15] - mViewProjMatrix[13];
	planes[TOP].normalize();
	//back
	planes[BACK].normal.x = mViewProjMatrix[ 3] + mViewProjMatrix[ 2];
	planes[BACK].normal.y = mViewProjMatrix[ 7] + mViewProjMatrix[ 6];
	planes[BACK].normal.z = mViewProjMatrix[11] + mViewProjMatrix[10];
	planes[BACK].d        = mViewProjMatrix[15] + mViewProjMatrix[14];
	planes[BACK].normalize();
	//front
	planes[FRONT].normal.x = mViewProjMatrix[ 3] - mViewProjMatrix[ 2];
	planes[FRONT].normal.y = mViewProjMatrix[ 7] - mViewProjMatrix[ 6];
	planes[FRONT].normal.z = mViewProjMatrix[11] - mViewProjMatrix[10];
	planes[FRONT].d        = mViewProjMatrix[15] - mViewProjMatrix[14];
	planes[FRONT].normalize();
	//set matrix
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(mProjMatrix);
	//now change only MODELVIEW
	glMatrixMode(GL_MODELVIEW);
}
//get a projectate point
Vec2 Camera::getPointIn3DSpace(const Vec3& point){
	
	Vec4 vpp(point,1.0);
	vpp=getGlobalMatrix().mul(vpp);	
	vpp=mProjMatrix.mul(vpp);	
	if(vpp.w==0) return Vec2::ZERO;

	vpp.x/=vpp.w;
	vpp.y/=vpp.w;
	vpp.z/=vpp.w;

    /* Map x, y and z to range 0-1 */
    vpp.x = vpp.x * 0.5 + 0.5;
    vpp.y = vpp.y * 0.5 + 0.5;
    vpp.z = vpp.z * 0.5 + 0.5;    

	return vpp.xy();
}
