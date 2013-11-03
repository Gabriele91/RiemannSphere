#include <stdafx.h>
#include <GeodesicTree.h>
#include <GeodesicSphere.h>

///////////////////////
using namespace Easy3D;
using namespace RiemannSphere;
///////////////////////

GeodesicTree::GeodesicTree()
:VirtualTree<GeodesicNode,4>(536870912*(1.0/4.0))
,sphere(NULL)
{
    
}

//build function
GeodesicNode* GeodesicTree::buildNode(GeodesicNode* parent,Easy3D::uchar i){
    //alloc
    GeodesicNode* gout=new GeodesicNode();
    //set sphere
    gout->radius=parent->radius;
    gout->level=parent->level+1;
    //set points
    parent->tri.genChild(gout->tri, i, parent->radius);
    //box
    gout->tri.getAABox(gout->box, parent->radius);
    //is virtual
    gout->isvirtual=true;
    //return gout
    return gout;
}
//delete function
bool GeodesicTree::isDeletableChild(GeodesicNode* node){
    if(node->isvirtual && ( !node->lockTask() ) ){
        bool deletablechild=true;
        for(Easy3D::uchar c=0; c!=4 && deletablechild; ++c){
            if(node->getChild(c))
                deletablechild= deletablechild && isDeletableChild(node->getChild(c));
        }
        return deletablechild;
    }
    return false;
}
bool GeodesicTree::isDeletable(GeodesicNode* node){
    if(!node) return false;
    
    bool deletablenode=
    node->isvirtual
    && ( !node->lockTask() )
    && ( node->level > (sphere->drawLevel+1) || ( !sphere->camera->boxInFrustum(node->box) ) );
    
    if(deletablenode){
        bool deletablechild=true;
        for(Easy3D::uchar c=0; c!=4 && deletablechild; ++c){
            if(node->getChild(c))
                deletablechild= deletablechild && isDeletableChild(node->getChild(c));
        }
        return deletablechild;
    }
    
    return false;
}

