#include <stdafx.h>
#include <Object.h>
/* Easy3D */
using namespace Easy3D;
//////////////////////
Object::Object()
			   :data(NULL),
		        parentMode(DISABLE_PARENT),
		        parent(NULL),
				changeValue(false),
				deleteHard(false){}
Object::~Object(){
	for(auto obj : *this){
		obj->parent=NULL;
		if(obj->deleteHard)
			delete obj;
	}
	childs.clear();
}

void Object::setScale(const Vector3D &scale,bool global){
	if(!global||!parent)
		transform.scale=scale;
	else{
		//transform.scale=scale*(transform.scale/getGlobalMatrix().getScale3D());
		transform.scale/=getGlobalMatrix().getScale3D();
		transform.scale*=scale;
	}
	change();
}
void Object::setPosition(const Vector3D &position,bool global){
	if(!global||!parent)
		transform.position=position;
	else
		//transform.position=position+(transform.position-getGlobalMatrix().getTranslation3D());
		transform.position-=getGlobalMatrix().getTranslation3D()-position;

	change();
}
void Object::setRotation(const Quaternion& rotation,bool global){
	if(!global||!parent)
		transform.rotation=rotation;
	else{
		transform.rotation=rotation.getInverse().mul(Quaternion::fromMatrix(getGlobalMatrix()));
	}
	change();
}

void Object::setTranslation(const Vector3D &translation){
	transform.position+=translation;
	change();
}
void Object::setMove(const Vector3D &move){
	Vec3 mov;
	const Mat4& tmp=transform.rotation.getMatrix();
	mov.x =   tmp(0,0)*move.x + tmp(1,0)*move.y + tmp(2,0)*move.z + tmp(3,0);
	mov.y =   tmp(0,1)*move.x + tmp(1,1)*move.y + tmp(2,1)*move.z + tmp(3,1);
	mov.z =   tmp(0,2)*move.x + tmp(1,2)*move.y + tmp(2,2)*move.z + tmp(3,2);
	transform.position+=mov;
	change();
}
void Object::setTurn(const Quaternion& rotation){
    transform.rotation=rotation.getNormalize().mul(transform.rotation);
	change();
}
//
Vector3D Object::getScale(bool global){
	if(!global||!parent)
		return transform.scale;
	else
		return getGlobalMatrix().getScale3D();
}
Vector3D Object::getPosition(bool global){
	if(!global||!parent)
		return transform.position;
	else
		return getGlobalMatrix().getTranslation3D();
}
Quaternion Object::getRotation(bool global){
	if(!global||!parent)
		return transform.rotation;
	else{
		return Quaternion::fromMatrix(getGlobalMatrix());
	}
}
Object::ParentMode Object::getParentMode() const{
	return parentMode;
}
//
void Object::change(){
	if(!changeValue){
		for(auto obj : *this ){
		  obj->change();
		}
		changeValue=true;
	}
}
void Object::addChild(Object *child,ParentMode type,bool hard){

	if(child->parent==this) return;
	if(child==this) return;
	if(child->parent) child->parent->erseChild(child);

	child->parent=this;
	child->parentMode=type;
	child->deleteHard=hard;
    child->change();
	this->childs.push_back(child);

}
void Object::erseChild(Object *child){
	if(child->parent==this){
		childs.remove(child);
		child->parent=NULL;
		child->deleteHard=false;
        child->change();
	}
}
//
std::list<Object*>::iterator Object::begin(){ return childs.begin(); }
std::list<Object*>::iterator Object::end(){ return childs.end(); }
std::list<Object*>::reverse_iterator Object::rbegin(){ return childs.rbegin(); }
std::list<Object*>::reverse_iterator Object::rend(){ return childs.rend(); }
//
const Matrix4x4& Object::getGlobalMatrix(){
    
	if(changeValue==true){
		//
		globalMat.identity();
        Mat4 tmpScale;
		//
		if(parent){
			//
			const Vec3& globalScale=getGlobalParentScale();
			//translation and rotation
			if(parentMode & (ENABLE_PARENT)){
                
				//global posiction and rotation
				Matrix4x4 mtmp=parent->getGlobalMatrix();
                tmpScale.setScale(Vec3(1.0/globalScale.x,  1.0/globalScale.y, 1.0/globalScale.z));
                mtmp=mtmp.mul(tmpScale);
                
				//////////////////////////////////////////////
				//position local
				globalMat[12]=transform.position.x;
				globalMat[13]=transform.position.y;
				globalMat[14]=transform.position.z;
				//rotarion local
				globalMat=globalMat.mul(transform.rotation.getMatrix());
				//////////////////////////////////////////////
				//local*global
				globalMat=mtmp.mul(globalMat);
				//////////////////////////////////////////////
			}
            //scale
			if(parentMode & ENABLE_SCALE)
                tmpScale.setScale(getGlobalParentScale()*transform.scale);
			else
                tmpScale.setScale(transform.scale);
            
			globalMat=globalMat.mul(tmpScale);

		}
		else{
			//position
			globalMat.entries[12]=transform.position.x;
			globalMat.entries[13]=transform.position.y;
			globalMat.entries[14]=transform.position.z;
			//rotarion
			globalMat=globalMat.mul(transform.rotation.getMatrix());
			//scale
            tmpScale.setScale(transform.scale);
			globalMat=globalMat.mul(tmpScale);
		}
		//
		changeValue=false;
	 }

return globalMat;
    
}


Mat4 Object::__getGlobalView(){
    
    return getGlobalMatrix().getInverse();
}

Vector3D  Object::getGlobalParentScale(){
           //no steck over flu....
           Object *p=NULL;
		   Vector3D out(Vec3::ONE);
           for(p=this->parent; p; p=p->parent ){
                out*=p->transform.scale;
           }
           return out;
}

