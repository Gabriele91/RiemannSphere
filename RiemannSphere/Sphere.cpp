#include <stdafx.h>
#include <Sphere.h>
#include <SphereMesh.h>

///////////////////////
using namespace RiemannSphere;
using namespace Easy3D;
///////////////////////
//http://acko.net/blog/making-worlds-1-of-spheres-and-cubes/


SubSphere::SubSphere(): rStart(0)
                        ,sStart(0)
                        ,rEnd(0)
                        ,sEnd(0){}

SubSphere::SubSphere(int rStart,
                     int rEnd,
                     int sStart,
                     int sEnd):  rStart(rStart)
,rEnd(rEnd)
,sStart(sStart)
,sEnd(sEnd){}
SubSphere::SubSphere(double rStart,
                     double rEnd,
                     double sStart,
                     double sEnd):  rStart(rStart)
                                ,rEnd(rEnd)
                                ,sStart(sStart)
                                ,sEnd(sEnd){}

Sphere::Sphere():rings(0)
                ,sectors(0)
                ,radius(0)
                {}

Sphere::Sphere(int rings,
               int sectors,
               float radius):rings(rings)
                            ,sectors(sectors)
                            ,radius(radius)
                            {}
bool Sphere::rayCast(const Ray& r,Segment& sg) const{
	/*
     http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter1.htm
     http://www.gamedev.net/topic/476158-ray-sphere-intersection/
     */
	//calc B
	// We solve this second-degree equation in t:
	// distance(p+t*v,center)==radius
	// If we define w = p-center
	// we can write that as
	// <w+t*v,w+t*v> == radius*radius
	// <w,w> + 2.0f*t*<w,v> + t*t*<v,v> - radius*radius == 0
	// <v,v>*t*t + 2.0f*<w,v>*t + <w,w>-radius*radius == 0
	// A*t*t + B*t*t + C*t*t == 0
	Vector3D w = r.point;
	float A = r.dir.dot(r.dir);
	float B = 2.0f*w.dot(r.dir);
	float C = w.dot(w) - radius*radius;
	//calc d
	float D = B*B-4.0f*A*C;
	float sqrtD=sqrt(D);
	//points
	sg.t[0]=r.point+r.dir*(-B - sqrtD)/(2.0f*A);
	sg.t[1]=r.point+r.dir*(-B + sqrtD)/(2.0f*A);
	//
	return D >=0;
}
DFORCEINLINE int nearPivot(int pivot,int min,int max){
    int maxv;
    
    if(min<pivot && pivot<max)
        maxv=pivot;
    else if(pivot<=min)
        maxv=min;
    else
        maxv=max;
    
    return maxv;
    
}

DFORCEINLINE int farPivot(int maxRing,int min, int max){
    if((maxRing-max)<min) return max;
    return min;
}

DFORCEINLINE void sectorMinMaxHalf(
                      //in
                      const Sphere& self,
                      int maxring,int minring,
                      int sStart,int sEnd,
                      //out
                      float& minX,float& minZ,
                      float& maxX,float& maxZ) {
    //max
    Vec3 maxS =self.getPoint(maxring,sStart);
    Vec3 maxE =self.getPoint(maxring,sEnd);
    //min
    Vec3 minS =self.getPoint(minring,sStart);
    Vec3 minE =self.getPoint(minring,sEnd);
    
    //middle
    int m1=self.sectors*0.25;
    int m2=self.sectors*0.75;
    Vec3 maxM=maxS;
    Vec3 minM=minS;
    
    if(sStart<m1 && m1<sEnd){
        maxM=self.getPoint(maxring,m1);
        minM=self.getPoint(minring,m1);
    }
    else if(sStart<m2 && m2<sEnd){
        maxM=self.getPoint(maxring,m2);
        minM=self.getPoint(minring,m2);
    }
    
    
    minX=Math::min(maxS.x,maxM.x,maxE.x,minS.x,minM.x,minE.x);
    minZ=Math::min(maxS.z,maxM.z,maxE.z,minS.z,minM.z,minE.z);
    
    maxX=Math::max(maxS.x,maxM.x,maxE.x,minS.x,minM.x,minE.x);
    maxZ=Math::max(maxS.z,maxM.z,maxE.z,minS.z,minM.z,minE.z);
}

Easy3D::AABox Sphere::genAABox(const SubSphere& sub) const {

    
    
    Vec3 h0(getPoint(sub.rStart,0));
    Vec3 h1(getPoint(sub.rEnd,0));
    
    //get max size ring
    int minring=farPivot(rings,sub.rStart,sub.rEnd);
    int maxring=nearPivot(rings*0.5,sub.rStart,sub.rEnd);
    
        
    
    AABox box;
    int sMid=sectors*0.5;
    
    if((sub.sEnd-sub.sStart)<=sMid){
        
        float minX,minZ;
        float maxX,maxZ;
        
        sectorMinMaxHalf(//in
                         *this,
                         maxring,minring,
                         sub.sStart,sub.sEnd,
                         //out
                         minX,minZ,
                         maxX,maxZ);
        box.min.x=minX;
        box.min.y=h0.y;
        box.min.z=minZ;
        
        box.max.x=maxX;
        box.max.y=h1.y;
        box.max.z=maxZ;
    }
    else{
        //half 1
        float minX1,minZ1;
        float maxX1,maxZ1;
        sectorMinMaxHalf(//in
                         *this,
                         maxring,minring,
                         sub.sStart,sMid,
                         //out
                         minX1,minZ1,
                         maxX1,maxZ1);
        //half 2
        float minX2,minZ2;
        float maxX2,maxZ2;
        sectorMinMaxHalf(//in
                         *this,
                         maxring,minring,
                         sMid,sub.sEnd,
                         //out
                         minX2,minZ2,
                         maxX2,maxZ2);
        
        box.min.x=Math::min(minX1,minX2);
        box.min.y=h0.y;
        box.min.z=Math::min(minZ1,minZ2);
        
        box.max.x=Math::max(maxX1,maxX2);
        box.max.y=h1.y;
        box.max.z=Math::max(maxZ1,maxZ2);
    }
    
    return box;
    
}


Easy3D::Vec3 Sphere::getPoint(int pRings,int pSectors) const {
    
    
    double lat0 = Math::PI * (-0.5 + (double) pRings / rings);
    double z0  =  std::sin(lat0);
    double zr0 =  std::cos(lat0);
    
    double lng = 2 * Math::PI * (double) pSectors / sectors;
    double x = std::cos(lng);
    double z = std::sin(lng);
    
    return Vec3 (x * zr0, z0, z * zr0) * radius;
    
}