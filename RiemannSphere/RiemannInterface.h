#ifndef RIEMANNINTERFACE_H
#define RIEMANNINTERFACE_H

#include <Easy3D.h>

namespace RiemannSphere {

	class RiemannInterface {

    protected:
        
        bool dGrid;
        bool dRoots;
        bool dInfinite;
        bool dZero;
        
        RiemannInterface()
        :dGrid(true)
        ,dRoots(true)
        ,dInfinite(true)
        ,dZero(true){}
        
	public:
        
        void drawGrid(bool dg){ dGrid=dg; }
        void drawRoots(bool dr){ dRoots=dr; }
        void drawInfinite(bool di){ dInfinite=di; }
        void drawZero(bool dz){ dZero=dz; }
        
	};

};

#endif