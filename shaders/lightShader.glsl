#shader vertex
#version 330 core
layout (location = 0) in vec3 position; 
uniform mat4 transform;
out vec4 vertexPos;
void main()
{
    gl_Position = transform * vec4(position, 1.0);
    vertexPos = gl_Position;
}       
#shader fragment
#version 330 core

uniform vec3 color;
in vec4 vertexPos;
void main()
{
    gl_FragColor = vec4(gl_FragCoord.xy, 0.0, 1.0);
}