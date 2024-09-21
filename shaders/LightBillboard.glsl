#shader vertex
#version 330 core
layout (location = 0) in vec3 position; 
layout (location = 1) in vec2 texCoords; 

out vec2 iTexCoords;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(position, 1.0);
    iTexCoords = texCoords;
}       
#shader fragment
#version 330 core
uniform vec3 color;
uniform vec2 resolution;

in vec2 iTexCoords;
out vec4 fragColor;

void main()
{
    vec2 color = iTexCoords*2;
    float dist =1-dot(color, color);
    if(dist<=0.01)discard;
    dist+=0.9;
    fragColor = vec4(vec3(dist), 1.0);
}