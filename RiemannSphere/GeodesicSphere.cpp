#include <stdafx.h>
#include <GeodesicSphere.h>

///////////////////////
using namespace Easy3D;
using namespace RiemannSphere;
///////////////////////
GeodesicSphere::GeodesicSphere(Easy3D::Camera *camera,
                               Fractal* fractal,
                               float radius,
                               size_t sBufferNodes,
                               size_t sBufferVBO )
                               :camera(camera)
                                ,fractal(fractal)
                                ,drawLevel(0)
                                ,multithread(NULL)
                                ,virtualVBO(sBufferVBO)
{
    multithread=new PoolThread(4);
    build(radius,sBufferNodes);
}
GeodesicSphere::~GeodesicSphere(){
    delete  multithread;
}


//debug draw
void GeodesicSphere::debugDraw(Easy3D::Render *render,GeodesicNode *node){
    //save state
    Render::ClientState lastClientState=render->getClientState();
    Render::ColorState  lastColorState=render->getColorState();
    //only vertex
    render->setClientState(Render::ClientState(Render::ClientState::VERTEX));
    
    //red color
    render->setColorState(Render::ColorState(Color(255,0,0,255)));
    
    //draw tri
    #define tripoint(v) v.x,v.y,v.z
    GLfloat verticesTri[]={
        tripoint(node->tri.point[0]),
        tripoint(node->tri.point[1]),
        tripoint(node->tri.point[2]),
        tripoint(node->tri.point[0])
    };
    #undef tripoint
    //unbind VBO
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
    //pointer to vertexs
    glVertexPointer(3, GL_FLOAT, 0, &verticesTri[0]);
    // deactivate vertex arrays after drawing
    glDrawArrays(GL_LINE_STRIP, 0, 4);
    
    //green color
    render->setColorState(Render::ColorState(Color(0,255,0,255)));
    //get box
    AABox box;
    box.setBox(node->box.getCenter(), node->box.getSize());
    //draw box
    GLfloat verticesBox[]={
        //bottom
        box.min.x,box.min.y,box.min.z,
        box.min.x,box.max.y,box.min.z,
        
        box.min.x,box.max.y,box.min.z,
        box.max.x,box.max.y,box.min.z,
        
        box.max.x,box.max.y,box.min.z,
        box.max.x,box.min.y,box.min.z,
        
        box.max.x,box.min.y,box.min.z,
        box.min.x,box.min.y,box.min.z,
        //up
        box.min.x,box.min.y,box.max.z,
        box.min.x,box.max.y,box.max.z,
        
        box.min.x,box.max.y,box.max.z,
        box.max.x,box.max.y,box.max.z,
        
        box.max.x,box.max.y,box.max.z,
        box.max.x,box.min.y,box.max.z,
        
        box.max.x,box.min.y,box.max.z,
        box.min.x,box.min.y,box.max.z,
        //conects
        box.min.x,box.min.y,box.min.z,
        box.min.x,box.min.y,box.max.z,
        
        box.min.x,box.max.y,box.min.z,
        box.min.x,box.max.y,box.max.z,
        
        box.max.x,box.max.y,box.min.z,
        box.max.x,box.max.y,box.max.z,
        
        box.max.x,box.min.y,box.min.z,
        box.max.x,box.min.y,box.max.z
        
    };
    
    //unbind VBO
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
    //pointer to vertexs
    glVertexPointer(3, GL_FLOAT, 0, &verticesBox[0]);
    // deactivate vertex arrays after drawing
    glDrawArrays(GL_LINES, 0, 24);
    
    //set old client state
    render->setColorState(lastColorState);
    render->setClientState(lastClientState);
}
void GeodesicSphere::debugSubDraw(Easy3D::Render *render,GeodesicNode *node){
    if(!node) return;
    for(Easy3D::uchar c=0;c!=4;++c){
        if(node->getChild(c)){
            debugSubDraw(render,node->getChild(c));
        }
    }
    debugDraw(render,node);
}

void GeodesicSphere::drawNodes(Easy3D::Render* render){
    for(size_t t=0;t!=20;++t)
        debugSubDraw(render,tris[t].getRoot());
}

void GeodesicSphere::addMeshToBuild(GeodesicNode* mesh){
	mutexBuildList.lock();
	meshToBuilds.push_front(mesh);
	mutexBuildList.unlock();
}
void GeodesicSphere::doBuildsList(){
	mutexBuildList.lock();
    for(auto mesh:meshToBuilds)
        mesh->sendToGpu(&virtualVBO);
    meshToBuilds.clear();
	mutexBuildList.unlock();
}


bool GeodesicSphere::drawSub(GeodesicNode *node,
                             GeodesicTree& tri,
                             int countlivel){
    bool childIsDraw=true;
    if(countlivel==0){
        for(uchar c=0;c!=4;++c)
            if(camera->boxInFrustum( tri.getChild(node,c)->box )){
                //to do: separate thread
				if(!tri.getChild(node,c)->lockTask())
                    tri.getChild(node,c)->buildMesh(*this,fractal);
                //draw
                childIsDraw=tri.getChild(node,c)->draw()&&childIsDraw;
            }
    }
    else{
        for(uchar c=0;c!=4;++c)
            if(camera->boxInFrustum( tri.getChild(node,c)->box )){
				if(!drawSub(tri.getChild(node,c),tri,countlivel-1)){
					//draw
					childIsDraw=tri.getChild(node,c)->draw()&&childIsDraw;
				}
			}
    }
	//draw Father?
    return childIsDraw;
}
void GeodesicSphere::draw(Easy3D::Render *render,int level){
    //update memory gpu
	virtualVBO.updateMemory();
	doBuildsList();
    //update memory cpu
    for(size_t i=0;i!=20;++i)
        tris[i].updateMemory();
    //save level
    drawLevel=level;
    //draw
    for(size_t i=0;i!=20;++i){
        if(camera->boxInFrustum( tris[i].getRoot()->box )){
            if(level==0){
                if(!tris[i].getRoot()->lockTask()){
                        tris[i].getRoot()->buildMesh(*this,fractal);
                }
                tris[i].getRoot()->draw();
            }
            else{
                for(uchar c=0;c!=4;++c){
                    if(!drawSub(tris[i].getChild(tris[i].getRoot(),c), tris[i], drawLevel-1))
                        tris[i].getRoot()->draw();
                }
            }
        }
    }
}


void GeodesicSphere::build(float radius,size_t sBufferNodes){

    //Edge length = golden ratio
    const float sqrt5 = sqrt (5.0f);
    const float phi = (1.0f + sqrt5) * 0.5f;
    // Circumscribed radius
    const float cRadius = sqrt (10.0f + (2.0f * sqrt5)) / (4.0f * phi);
    //Now we define constants which will define our Icosahedron's vertices
    double a = (radius / cRadius) * 0.5;
    double b = (radius / cRadius) / (2.0f * phi);
    
    // Vertices of the Icosahedron:
    Vec3 v1 ( 0,  b, -a);
    Vec3 v2 ( b,  a,  0);
    Vec3 v3 (-b,  a,  0);
    Vec3 v4 ( 0,  b,  a);
    Vec3 v5 ( 0, -b,  a);
    Vec3 v6 (-a,  0,  b);
    Vec3 v7 ( 0, -b, -a);
    Vec3 v8 ( a,  0, -b);
    Vec3 v9 ( a,  0,  b);
    Vec3 v10(-a,  0, -b);
    Vec3 v11( b, -a,  0);
    Vec3 v12(-b, -a,  0);
    
    //set roots
    for(size_t i=0;i!=20;++i){
        //node max size
        tris[i].setMaxBufferSize(sBufferNodes/20);
        //disable virtual
        tris[i].getRoot()->isvirtual=false;
        //set radius
        tris[i].getRoot()->radius=radius;
        tris[i].getRoot()->level=0;
        //set ptr
        tris[i].sphere=this;
    }
    
    tris[0].getRoot()->tri={v1, v2, v3};
    tris[1].getRoot()->tri={v4, v3, v2};
    tris[2].getRoot()->tri={v4, v5, v6};
    tris[3].getRoot()->tri={v4, v9, v5};
    
    tris[4].getRoot()->tri={v1, v7, v8};
    tris[5].getRoot()->tri={v1, v10, v7};
    tris[6].getRoot()->tri={v5, v11, v12};
    tris[7].getRoot()->tri={v7, v12, v11};
    
    tris[8].getRoot()->tri={v3, v6, v10};
    tris[9].getRoot()->tri={v12, v10, v6};
    tris[10].getRoot()->tri={v2, v8, v9};
    tris[11].getRoot()->tri={v11, v9, v8};
    
    tris[12].getRoot()->tri={v4, v6, v3};
    tris[13].getRoot()->tri={v4, v2, v9};
    tris[14].getRoot()->tri={v1, v3, v10};
    tris[15].getRoot()->tri={v1, v8, v2};
    
    tris[16].getRoot()->tri={v7, v10, v12};
    tris[17].getRoot()->tri={v7, v11, v8};
    tris[18].getRoot()->tri={v5, v12, v6};
    tris[19].getRoot()->tri={v5, v9, v11};
    //set boxs
    for(size_t i=0;i!=20;++i)
        //calc aabox
        tris[i].getRoot()->tri.getAABox(tris[i].getRoot()->box, radius);
    /*
    // draw the icosahedron
    //drawIcosahedron (v1, v2, v3, maxEdgeLength, radius);//0
    //drawIcosahedron (v4, v3, v2, maxEdgeLength, radius);//1
    //drawIcosahedron (v4, v5, v6, maxEdgeLength, radius);//2
    //drawIcosahedron (v4, v9, v5, maxEdgeLength, radius);//3
    
    //drawIcosahedron (v1, v7, v8, maxEdgeLength, radius);//4
    //drawIcosahedron (v1, v10, v7, maxEdgeLength, radius);//5
    //drawIcosahedron (v5, v11, v12, maxEdgeLength, radius);//6
    //drawIcosahedron (v7, v12, v11, maxEdgeLength, radius);//7
    
    //drawIcosahedron (v3, v6, v10, maxEdgeLength, radius);//8
    //drawIcosahedron (v12, v10, v6, maxEdgeLength, radius);//9
    //drawIcosahedron (v2, v8, v9, maxEdgeLength, radius);//10
    //drawIcosahedron (v11, v9, v8, maxEdgeLength, radius);//11
    
    //drawIcosahedron (v4, v6, v3, maxEdgeLength, radius);//12
    //drawIcosahedron (v4, v2, v9, maxEdgeLength, radius);//13
    //drawIcosahedron (v1, v3, v10, maxEdgeLength, radius);//14
    //drawIcosahedron (v1, v8, v2, maxEdgeLength, radius);//15
    
    //drawIcosahedron (v7, v10, v12, maxEdgeLength, radius);//16
    //drawIcosahedron (v7, v11, v8, maxEdgeLength*0.25, radius);//17
    //drawIcosahedron (v5, v12, v6, maxEdgeLength*0.5, radius);//18
    //drawIcosahedron (v5, v9, v11, maxEdgeLength*0.5, radius);//19
     */
}