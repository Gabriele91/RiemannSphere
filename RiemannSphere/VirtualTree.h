#ifndef VIRTUALOCTREE_H
#define VIRTUALOCTREE_H

#include <Easy3D.h>

namespace RiemannSphere {
    
    class Node;
    template<class T, int N> class VirtualTree;
    

    
    template<class T, int N>
	class VirtualTree {
        
    public:
        
        class Node{
            
            T* childs[N];
            friend  class VirtualTree<T,N>;
            
        public:
            
			Node(){
                for(Easy3D::uchar i=0;i!=N;++i) childs[i]=NULL;
			}
            T* getChild(Easy3D::uchar i){
                DEBUG_ASSERT(i<N);
                return childs[i];
            }
            void setChild(T* child,Easy3D::uchar i){
                childs[i]=child;
            }
            virtual ~Node(){
                for(Easy3D::uchar i=0;i!=N;++i) {
                    DEBUG_ASSERT(childs[i]==NULL);
                }
            }
            
        };
        
        
        //constructor
        VirtualTree(Easy3D::uchar  maxSize):gSize(0),maxSize(maxSize){
            root=new T();
        }
        //set max size
        void setMaxBufferSize(size_t msize){
            maxSize=msize;
        }
        //destructor
        virtual ~VirtualTree(){
            for(Easy3D::uchar  c=0;c!=N;++c)
                if(root->Node::getChild(c))
                    __deleteChilds(root,c);
            delete root;
        }
        //tree root
        T* getRoot(){
            return root;
        }
        //a child
        T* getChild(T* parent,Easy3D::uchar  i){
           //get child
           T* child=parent->VirtualTree<T,N>::Node::getChild(i);
           if(child) return child;
           //create a child
           gSize+=sizeof(T);
           child=buildNode(parent,i);
           parent->VirtualTree<T,N>::Node::setChild(child,i);
           return child;
        }
        
        //build function
		virtual T* buildNode(T* parent,Easy3D::uchar i){ return NULL; }
        //delete function
		virtual bool isDeletable(T* node){ return true; }
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
            for(Easy3D::uchar  c=0;c!=N;++c)
                if(node->Node::getChild(c))
                    deleteChild(node,c);
        }
        void deleteChild(T* node,Easy3D::uchar c){
            DEBUG_ASSERT(c<N);
            if ( isDeletable(node->Node::getChild(c)) ){
                __deleteChilds(node,c);
            }
            else if(globalSize()>maxSize){
                for(Easy3D::uchar i=0;i!=N;++i){
                    if(node->Node::getChild(c)->Node::getChild(i)){
                        deleteChild(node->Node::getChild(c),i);
                    }
                }
            }
        }
        void __deleteChilds(T* node,Easy3D::uchar c){
            DEBUG_ASSERT(c<N);
            for(Easy3D::uchar i=0;i!=N;++i){
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