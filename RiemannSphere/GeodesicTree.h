#ifndef GEODESICTREE_H
#define GEODESICTREE_H

#include <Easy3D.h>
#include <Fractal.h>
#include <VirtualTree.h>
#include <GeodesicNode.h>

namespace RiemannSphere {
    
    class GeodesicNode;
    class GeodesicTree;
    class GeodesicSphere;
    
	class GeodesicTree : public VirtualTree<GeodesicNode,4> {
        
        friend class GeodesicNode;
        friend class GeodesicTree;
        friend class GeodesicSphere;
        
        //ptr manager
        GeodesicSphere *sphere;
        
        //build function
        virtual GeodesicNode* buildNode(GeodesicNode* parent,
                                        Easy3D::uchar i);
        
        //delete function
        bool isDeletableChild(GeodesicNode* node);
        virtual bool isDeletable(GeodesicNode* node);
	
    public:

		GeodesicTree();

	};

};

#endif