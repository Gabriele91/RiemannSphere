varying vec4 vPos;
void main()
{
	vPos=gl_Vertex;
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}