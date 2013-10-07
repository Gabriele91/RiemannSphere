#ifndef SPHERE_H
#define SPHERE_H

#include <Easy3D.h>
#include <Rays.h>

namespace RiemannSphere {
    
    
    struct SubSphere;
    struct Sphere;
    
    //sphere
	struct Sphere {
        
        int rings;
        int sectors;
        float radius;
        
        Sphere();
        Sphere(int rings,
               int sectors,
               float radius);
        
        Easy3D::AABox genAABox(const SubSphere& sub) const;
        Easy3D::Vec3 getPoint(int pRings,int pSectors) const;
        bool rayCast(const Ray& r,Segment& sg) const;
	};
    
    //subpart
    struct SubSphere {
        
        double rStart;
        double rEnd;
        double sStart;
        double sEnd;
        
        SubSphere();
        SubSphere(int rStart,int rEnd,int sStart, int sEnd);
        SubSphere(double rStart,double rEnd,double sStart, double sEnd);
        SubSphere operator/(const Sphere& sphere){
            return { rStart/sphere.rings,rEnd/sphere.rings,
                     sStart/sphere.sectors,sEnd/sphere.sectors };
        }
        SubSphere operator*(const Sphere& sphere){
            return { rStart*sphere.rings,rEnd*sphere.rings,
                     sStart*sphere.sectors,sEnd*sphere.sectors };
        }
    };
};

#endif