
#define NUMIT 100

uniform vec2 poly[POLYSIZE];
uniform vec2 subpoly[SUBPOLYSIZE];
uniform vec2 roots[POLYSIZE-1];
uniform vec4 colors[POLYSIZE-1];
uniform float radius;

varying vec4 vPos;



////////////////////////////////////////////////////////////////////////////////

vec2 complexMult(in vec2 z0, in vec2 z1)
{
  float a = z0.x*z1.x - z0.y*z1.y;
  float b = z0.x*z1.y + z0.y*z1.x;
  return vec2(a,b);
}

vec2 complexSquare(in vec2 z)
{
  return vec2(z.x*z.x-z.y*z.y,2.0*z.x*z.y);
}

vec2 complexDivide(in vec2 z0, in vec2 z1){
    float c2d2=(z1.x*z1.x)+(z1.y*z1.y);
	return vec2((z0.x*z1.x+z0.y*z1.y)/c2d2,(z0.y*z1.x-z0.x*z1.y)/c2d2);
}
////////////////////////////////////////////////////////////////////////////////
vec2 toPlane(in vec4 v){
    return vec2(v.x/(radius-v.y),v.z/(radius-v.y));
}

vec2 hornerPolygon(in vec2 x){
    
    vec2 vn=poly[0];
    
    for(int i=1;i<POLYSIZE;++i)
        vn = complexMult(vn,x)+poly[i];
    
    return vn;
}
vec2 hornerSubPolygon(in vec2 x){
    
    vec2 vn=subpoly[0];
    
    for(int i=1;i<SUBPOLYSIZE;++i)
        vn = complexMult(vn,x)+subpoly[i];
    
    return vn;
}

vec3 generic(in vec2 x,in float e){

    vec2 cx=x; 
    vec2 nextx; 
    
    for(int i=NUMIT;i>0;--i){
    
        nextx=complexDivide(hornerPolygon(cx),hornerSubPolygon(cx));
        
        if( distance(nextx.x,cx.x) < e || distance(nextx.y,cx.y) < e ) 
        {
            return vec3(cx,float(i)/float(NUMIT));
        }
        
        cx=nextx;
        
    }
    
    return vec3(cx,0.0);
}

vec4 colorFromPoint(in vec3 p,in float e){
    for(int i=0;i<(POLYSIZE-1);++i){
        if( distance(roots[i],p.xy) < e )
        {
                return  colors[i]*p.z;
        }
    }
    return vec4(0,0,0,1);
}

void main()
{    
    vec3 point=generic(toPlane(vPos*2.0),1E-10);
    vec4 pointColor=colorFromPoint(point,0.0001);
    gl_FragColor = vec4(pointColor.xyz,1.0);
}

