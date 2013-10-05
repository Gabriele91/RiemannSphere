#ifndef VIRTUALOCTREE_H
#define VIRTUALOCTREE_H

#include <Easy3D.h>

namespace RiemannSphere {
    
    class Node;
    template<class T> class VirtualOctree;
    

    
    template<class T>
	class VirtualOctree {
        
    public:
        
        class Node{
            
            T* childs[8]{NULL,NULL,NULL,NULL,
                            NULL,NULL,NULL,NULL};
            friend  class VirtualOctree<T>;
            
        public:
            
            T* getChild(Easy3D::uchar i){
                return childs[i];
            }
            void setChild(T* child,Easy3D::uchar i){
                childs[i]=child;
            }
            virtual ~Node(){
                DEBUG_ASSERT(childs[0]==NULL);
                DEBUG_ASSERT(childs[1]==NULL);
                DEBUG_ASSERT(childs[2]==NULL);
                DEBUG_ASSERT(childs[3]==NULL);
                DEBUG_ASSERT(childs[4]==NULL);
                DEBUG_ASSERT(childs[5]==NULL);
                DEBUG_ASSERT(childs[6]==NULL);
                DEBUG_ASSERT(childs[7]==NULL);
            }
            
        };
        
        
        //constructor
        VirtualOctree(size_t maxSize):gSize(0),maxSize(maxSize){
            root=new T();
        }
        //destructor
        virtual ~VirtualOctree(){
            for(Easy3D::uchar c=0;c!=8;++c)
                __deleteChilds(root,c);
            delete root;
        }
        //tree root
        T* getRoot();
        //a child
        T* getChild(T* parent,Easy3D::uchar i){
           //get child
           T* child=parent->Node::getChild(i);
           if(child) return child;
           //create a child
           gSize+=sizeof(T);
           child=buildNode(parent,i);
           parent->Node::setChild(child,i);
           return child;
        }
        
        //build function
        virtual T* buildNode(T* parent,Easy3D::uchar i)=0;
        //delete function
        virtual bool isDeletable(T* node)=0;
        //build size
        size_t globalSize(){
            return gSize;
        }
        //memory update
        void updateMemory(){
            if(globalSize()>maxSize)
                deleteChilds(root);
        }
        
        
    protected:
        
        T* root;
        size_t gSize;
        size_t maxSize;
        
        //search and deletable node
        void deleteChilds(T* node){
            for(Easy3D::uchar c=0;c!=8;++c)
                if(node->Node::getChild(c))
                    deleteChild(node,c);
        }
        void deleteChild(T* node,Easy3D::uchar c){
            if ( isDeletable(node->Node::getChild(c)) ){
                __deleteChilds(node,c);
            }
            else if(globalSize()>maxSize){
                for(Easy3D::uchar i=0;i!=8;++i){
                    if(node->Node::getChild(c)->Node::getChild(i)){
                        deleteChild(node->Node::getChild(c),i);
                    }
                }
            }
        }
        void __deleteChilds(T* node,Easy3D::uchar c){
            for(Easy3D::uchar i=0;i!=8;++i){
                if(node->Node::getChild(c)->Node::getChild(i)){
                    __deleteChilds(node->Node::getChild(c),i);
                }
            }
            //dealloc
            gSize-=sizeof(T);
            delete node->Node::getChild(c);
            node->Node::setChild(NULL,c);
        }
	};

};

#endif