#include <stdafx.h>
#include <GeodesicTri.h>

///////////////////////
using namespace Easy3D;
using namespace RiemannSphere;
///////////////////////

void GeodesicTri::getAABox(Easy3D::AABox& box,float radius){
    //calc center
    Vec3 center=(point[0]+point[1]+point[2])/3.0f;
    center.normalize();
    center*=radius;
    //aabox
    box.max=Vec3(Math::max(point[0].x,point[1].x,point[2].x,center.x),
                 Math::max(point[0].y,point[1].y,point[2].y,center.y),
                 Math::max(point[0].z,point[1].z,point[2].z,center.z));
    
    box.min=Vec3(Math::min(point[0].x,point[1].x,point[2].x,center.x),
                 Math::min(point[0].y,point[1].y,point[2].y,center.y),
                 Math::min(point[0].z,point[1].z,point[2].z,center.z));
    //hack
    box.setBox(box.getCenter(), box.getSize()*4.0);
}
void GeodesicTri::genChild(GeodesicTri& c,Easy3D::uchar i,float radius){
    //points
    Vec3 p1=(point[0]+point[1])/2.0f;
    Vec3 p2=(point[0]+point[2])/2.0f;
    Vec3 p3=(point[1]+point[2])/2.0f;
    //normalize to sphere
    p1.normalize(); p1*=radius;
    p2.normalize(); p2*=radius;
    p3.normalize(); p3*=radius;
    //
    switch (i) {
        case 0:
            c.point[0]=p1;
            c.point[1]=point[0];
            c.point[2]=p2;
        break;
        case 1:
            c.point[0]=p1;
            c.point[1]=p3;
            c.point[2]=point[1];
        break;
        case 2:
            c.point[0]=p2;
            c.point[1]=point[2];
            c.point[2]=p3;
        break;
        case 3:
            c.point[0]=p1;
            c.point[1]=p2;
            c.point[2]=p3;
        break;
    }

}