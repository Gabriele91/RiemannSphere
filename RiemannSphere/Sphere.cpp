#include <stdafx.h>
#include <Sphere.h>
#include <SphereMesh.h>

///////////////////////
using namespace RiemannSphere;
using namespace Easy3D;
///////////////////////
//http://acko.net/blog/making-worlds-1-of-spheres-and-cubes/

Sphere::Sphere(int rings,int sectors,float radius)
:rings(rings)
,sectors(sectors)
,radius(radius)
{
}

SphereMesh *Sphere::genMesh(float rStart,float sStart,float rEnd,float sEnd){
    SphereMesh* mesh=new SphereMesh(*this, rStart, sStart, rEnd, sEnd);
    mesh->box=genAABox(rStart, sStart, rEnd, sEnd);
    return mesh;
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
    int m1=self.getSectors()*0.25;
    int m2=self.getSectors()*0.75;
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

Easy3D::AABox Sphere::genAABox(float rStart,float sStart,float rEnd,float sEnd) const {

    
    
    Vec3 h0(getPoint(rStart,0));
    Vec3 h1(getPoint(rEnd,0));
    
    //get max size ring
    int minring=farPivot(getRings(),rStart,rEnd);
    int maxring=nearPivot(getRings()*0.5,rStart,rEnd);
    
    
#if 0
    
    double latMin = Math::PI * (-0.5 + (double) minring / getRings());
    double zrMin =  std::cos(latMin);
    
    double latMax = Math::PI * (-0.5 + (double) maxring / getRings());
    double zrMax =  std::cos(latMax);
    
    Vec3 min=Vec3::MAX,max=-Vec3::MAX;
    
    //for vars
    float xMax,zMax,xMin,zMin;
    float x,z,lng,invSectors=1.0f/getSectors();
    
    for(int s=rStart;s<sEnd;++s){
        
        lng = Math::PI2 * (float) s * invSectors;
        x = std::cos(lng);
        z = std::sin(lng);
        
        xMax=x*zrMax;
        zMax=z*zrMax;
        
        xMin=x*zrMin;
        zMin=z*zrMin;
        
        min.x=Math::min(min.x,xMax,xMin);
        min.z=Math::min(min.z,zMax,zMin);
        max.x=Math::max(max.x,xMax,xMin);
        max.z=Math::max(max.z,zMax,zMin);
        
    }
    
    min*=getRadius();
    max*=getRadius();
    
    AABox box;
    box.min.x=min.x;
    box.min.y=h0.y;
    box.min.z=min.z;
    
    box.max.x=max.x;
    box.max.y=h1.y;
    box.max.z=max.z;
    
#else
    
    
    AABox box;
    int sMid=getSectors()*0.5;
    
    if((sEnd-sStart)<=sMid){
        
        float minX,minZ;
        float maxX,maxZ;
        
        sectorMinMaxHalf(//in
                         *this,
                         maxring,minring,
                         sStart,sEnd,
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
                         sStart,sMid,
                         //out
                         minX1,minZ1,
                         maxX1,maxZ1);
        //half 2
        float minX2,minZ2;
        float maxX2,maxZ2;
        sectorMinMaxHalf(//in
                         *this,
                         maxring,minring,
                         sMid,sEnd,
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
    
    
#endif
    return box;
    
}


Easy3D::Vec3 Sphere::getPoint(int pRings,int pSectors) const {
    
    
    double lat0 = Math::PI * (-0.5 + (double) pRings / getRings());
    double z0  =  std::sin(lat0);
    double zr0 =  std::cos(lat0);
    
    double lng = 2 * Math::PI * (double) pSectors / getSectors();
    double x = std::cos(lng);
    double z = std::sin(lng);
    
    return Vec3 (x * zr0, z0, z * zr0) * getRadius();
    
}