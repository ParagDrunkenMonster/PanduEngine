uniform float scaleValue;

void main(void)
{
   vec4 a = gl_Vertex;
   a.x = a.x * scaleValue;
   a.y = a.y * scaleValue;


   gl_Position = gl_ModelViewProjectionMatrix * a;
}