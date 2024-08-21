#shader vertex
#version 330 core
layout (location = 0) in vec3 position; 
uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(position, 1.0);
}       
#shader fragment
#version 330 core
out vec4 FragColor;  

uniform vec3 color;

void main()
{
    FragColor = vec4(color, 1.0f);
}