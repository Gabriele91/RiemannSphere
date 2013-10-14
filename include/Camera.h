#ifndef CAMERA_H
#define CAMERA_H

#include <Config.h>
#include <Object.h>


namespace Easy3D {

	class Camera : public Object {

		//planes
		Plane planes[6];
		//matrix
		Mat4 mProjMatrix;
		Mat4 mViewProjMatrix;
		//
	public:
        
		enum{
			BACK=0,
			FRONT=1,
			TOP=2,
			BOTTOM=3,
			LEFT=4,
			RIGHT=5
		};
        
        const Plane& getPlane(uchar i) const {
            return planes[i];
        }
		//
		Camera():Object(){}
		//set prospetive
		void setPerspective(float angle,float n,float f);
		void setPerspective(float angle,float spectre,float n,float f);
		void setPerspective(float left, float right, float bottom,float top, float n, float f);
		void setOrtogonal(float left, float right, float bottom,float top, float n, float f);
		//culling methods
		enum {OUTSIDE, INTERSECT, INSIDE};
		int sphereInFrustum(const Vec3& point, float radius) const;
        int boxInFrustum(const AABox &b) const;
		int pointInFrustum(const Vec3 &point) const;
		//update camera
		void update();
		//return matrix
		DFORCEINLINE const Mat4& getProjectionMatrix() const {
			return mProjMatrix;
		}
		DFORCEINLINE const Mat4& getViewProjMatrix() const {
			return mViewProjMatrix;
		}
		Vec2 getClipPointFrom3DSpace(const Vec3& point);
		Vec2 getScreenPointFrom3DSpace(const Vec3& point);

        Vec3 getPointFrom2DClipSpace(const Vec2& point);
        Vec3 getPointFrom2DScreen(const Vec2& point);
        Vec3 getNormalPointFrom2DClipSpace(const Vec2& point);
        Vec3 getNormalPointFrom2DScreen(const Vec2& point);
        //get view
        Mat4 getGlobalView();
	};

};

#endif
