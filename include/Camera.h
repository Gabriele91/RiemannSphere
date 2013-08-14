#ifndef CAMERA_H
#define CAMERA_H

#include <Config.h>
#include <Object.h>


namespace Easy3D {

	class Camera : public Object {

		enum{
			BACK=0,
			FRONT=1,
			TOP=2,
			BOTTOM=3,
			LEFT=4,
			RIGHT=5
		};
		//planes
		Plane planes[6];
		//matrix
		Mat4 mProjMatrix;
		Mat4 mViewProjMatrix;
		//
	public:
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
		DFORCEINLINE const Mat4& getProjectionMatrix(){
			return mProjMatrix;
		}
		DFORCEINLINE const Mat4& getViewProjMatrix(){
			return mViewProjMatrix;
		}
		Vec2 getPointIn3DSpace(const Vec3& point);
	};

};

#endif
