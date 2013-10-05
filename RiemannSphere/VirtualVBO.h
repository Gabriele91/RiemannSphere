#ifndef VIRTUALVBO_H
#define VIRTUALVBO_H

#include <Easy3D.h>

namespace RiemannSphere {
	
	class VirtualVBO {

	public:
		//node class
		class Node{

			Node *prev,*next;
			GLuint vbo; 
			GLuint cVertices;
			
			size_t size;
			void  *data;

			friend class VirtualVBO;

		public:
			//methos
			Node();
			~Node();
			//cpu side
			void  allocCpu(size_t size,GLuint vertexCount);
			void* getData();
			void  cleanInfo();
			//gpu side
			bool  isAllocated();
			bool  draw();
			void  build(bool freedata=true);
			void  unbuild();

		};

		VirtualVBO(size_t maxSize);
		~VirtualVBO();

		void addNode(Node* in);
		void updateNode(Node* in);
		void deleteLast();
		void updateMemory();
        long long int globalSize(){
            return gSize;
        }

	protected:
		
		//node objects
		Node *first, *last;
		//size vbo allocations
		long long int gSize;
		long long int maxSize;
		//count nodes
		long long int  cNode;

	private:

		DFORCEINLINE void __dtLastNode(){
			if(last){
				//dt
				if(last->prev) last->prev->next=NULL;
				//last = prev
				last=last->prev;
				//if last==null then first = null
				if(!last) first=NULL;
			}
		}
		DFORCEINLINE void __nodeToBottom(Node *in){
			if(in==last) return;
			//swap (dt)
			if(in->prev) in->prev=in->next;
			if(in->next) in->next=in->prev;
			in->prev=in->next=NULL;
			//into last
			if(last) last->next=in;
			in->prev=last;
			//become last
			last=in;
		}
		DFORCEINLINE void __nodeToTop(Node *in){
			if(in==first) return;
			//swap (dt)
			if(in->prev) in->prev=in->next;
			if(in->next) in->next=in->prev;
			in->prev=in->next=NULL;
			//into top
			if(first) first->prev=in;
			in->next=first;
			//become first
			first=in;
		}
		DFORCEINLINE void __buildVBO(Node *in){
			//create the VBO
			in->build();
			//calc all size
			gSize+=in->size;
		}
		DFORCEINLINE void __deleteVBO(Node *in){
			//recalc size
			gSize-=last->size;
			//dealloc vbo
			in->unbuild();
		}
	};

};

#endif