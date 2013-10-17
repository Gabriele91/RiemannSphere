
#define NEWTONIT 100

uniform vec2 poly[POLYSIZE];
uniform vec2 roots[POLYSIZE-1];
uniform vec4 colors[POLYSIZE-1];
uniform float radius;

varying vec4 vPos;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
vec2 complexMult(in vec2 z0, in vec2 z1);
vec2 complexDivide(in vec2 z0, in vec2 z1);
vec2 complexSquare(in vec2 z);
vec2 complexConjugate(in vec2 z);
vec2 complexInvert(in vec2 z);
float realPart(in vec2 z);
float imagPart(in vec2 z);
float arg(in vec2 z);
float magnitude(in vec2 z);
vec2 polar(in vec2 z);
vec2 mkPolar(in float r, in float phi);
vec2 cis(in float phi);

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

float realPart(in vec2 z)
{
  return z.x;
}

float imagPart(in vec2 z)
{
  return z.y;
}

float arg(in vec2 z)
{
  return atan(z.y,z.x);
}

float magnitude(in vec2 z)
{
  return length(z);
}

vec2 complexConjugate(in vec2 z)
{
  return vec2(z.x,-z.y);
}

vec2 polar(in vec2 z)
{
  return vec2(length(z), arg(z));
}

// e^ix = cos(x) + i*sin(x)
vec2 cis(in float phi)
{
  return vec2(cos(phi),sin(phi));
}

vec2 mkPolar(in float r, in float phi)
{
  return r*cis(phi);
}

vec2 complexInvert(in vec2 z)
{
  // return 1.0/(z.x^2 + z.y^2 ) * complexConjugate(z);
  float rinv = 1.0/length(z);
  return mkPolar(rinv, -arg(z));
}

vec2 complexDivide(in vec2 z0, in vec2 z1){
    float c2d2=(z1.x*z1.x)+(z1.y*z1.y);
	return vec2((z0.x*z1.x+z0.y*z1.y)/c2d2,(z0.y*z1.x-z0.x*z1.y)/c2d2);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
vec2 toPlane(vec4 v){
    return vec2(v.x/(radius-v.y),v.z/(radius-v.y));
}

vec2 horner(vec2 x){

    vec2 vn=poly[0];
    vec2 wn=poly[0];
    vec2 un=poly[0];
    vec2 yn=poly[0];
	
    for(int i=1;i<POLYSIZE-3;++i){
        vn = complexMult(vn,x)+poly[i];
        wn = complexMult(wn,x)+vn;
        un = complexMult(un,x)+wn;
        yn = complexMult(yn,x)+un;
    }
	
    vn = complexMult(vn,x)+poly[POLYSIZE-3];
    wn = complexMult(wn,x)+vn;
    un = complexMult(un,x)+wn;
    
    vn = complexMult(vn,x)+poly[POLYSIZE-2];
    wn = complexMult(wn,x)+vn;
    
    vn = complexMult(vn,x)+poly[POLYSIZE-1];
    
    vec2 q=complexDivide(vn,wn);
    vec2 q2=complexMult(q,q);
    vec2 q3=complexMult(q,q2);
    vec2 s3=q+complexDivide(complexMult(q2,un),wn);
    vec2 ynOnwn=complexDivide(yn,wn);
    vec2 unOnwn2=complexDivide(un,complexMult(wn,wn));
    return s3-complexMult(q3,ynOnwn-unOnwn2);
}

vec3 schroeder(vec2 x,float e){

    vec2 cx=x; 
    vec2 nextx; 
    
    for(int i=NEWTONIT;i>0;--i){
    
        nextx=cx-horner(cx);
        
        if( distance(nextx.x,cx.x) < e || distance(nextx.y,cx.y) < e ) 
        {
            return vec3(cx,float(i)/float(NEWTONIT));
        }
        
        cx=nextx;
        
    }
    
    return vec3(cx,0.0);
}

vec4 colorFromPoint(vec3 p,float e){
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
    vec3 point=schroeder(toPlane(vPos*2.0),1E-10);
    vec4 pointColor=colorFromPoint(point,0.0001);
    gl_FragColor = vec4(pointColor.xyz,1.0);
}

