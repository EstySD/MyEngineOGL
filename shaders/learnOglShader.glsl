#shader vertex
#version 330 core
layout (location = 0) in vec3 position; 
layout (location = 1) in vec2 texCoord;
out vec2 iTexCoord;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(position, 1.0);
    iTexCoord = texCoord;
}       
#shader fragment
#version 330 core
out vec4 FragColor;  
in vec2 iTexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
void main()
{
    FragColor = mix(texture(texture1, iTexCoord), texture(texture2, iTexCoord), 0.2);
}