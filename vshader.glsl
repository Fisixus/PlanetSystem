in  vec4 vPosition;
in  vec4 vColor;
out vec4 color;

uniform vec3 theta;

void main() 
{

    color = vColor;
    gl_Position = vPosition;
} 