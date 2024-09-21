#shader vertex
#version 330 core
layout (location = 0) in vec3 position; 
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 transform;
uniform mat4 model;

out vec3 fragPos;
out vec3 inormal;
out vec2 iTexCoord;
void main()
{
    gl_Position = transform * vec4(position, 1.0);
    fragPos = vec3(model * vec4(position, 1.0));
    inormal = mat3(transpose(inverse(model))) * normal;  
    iTexCoord = texCoord;
}       
#shader fragment
#version 330 core

//global
//out
out vec4 fragColor;  
//in
in vec3 fragPos; 
in vec3 inormal;
in vec2 iTexCoord;

struct Material {
    float ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
    sampler2D emission;
}; 
struct MapsSample{
    float ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    vec3 emission;
};
uniform Material material;
struct PointLight {
    vec3 position;
    vec3 color;
    vec3 clq;
};
#define MAX_POINT_LIGHTS 4
uniform int pointLightsCount;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
struct DirLight{
    vec3 direction;
    vec3 color;
};
uniform DirLight dirLight;
struct SpotLight{
    vec3 position;
    vec3 direction;
    vec3 color;
    float cutOff;
    float outercutOff;
};
#define MAX_SPOT_LIGHTS 4
uniform int spotLightsCount;
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
vec3 CalcDirLight(DirLight light, vec3 norm, vec3 viewDir, MapsSample mapsSample){
    vec3 lightDir =-normalize(light.direction);
    vec3 diffuse = mapsSample.diffuse*max(dot(norm, lightDir), 0.0); // value
    
    // specular
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mapsSample.shininess);
    vec3 specular = mapsSample.specular*spec;
    
    return (diffuse+specular)*light.color;
};
vec3 CalcPointLight(PointLight light, vec3 norm, vec3 viewDir, MapsSample mapsSample, vec3 fragPos){
    vec3 lightDir =normalize(light.position-fragPos);
    vec3 diffuse = mapsSample.diffuse*max(dot(norm, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mapsSample.shininess);
    vec3 specular = mapsSample.specular*spec;

    float dist    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.clq.x + light.clq.y * dist + 
  			     light.clq.z * (dist * dist));

    return (diffuse+specular)*attenuation*light.color;
};
vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 viewDir, MapsSample mapsSample, vec3 fragPos){
    vec3 lightDir =normalize(light.position-fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    if(theta<light.outercutOff) return vec3(0.0);
    float epsilon   = light.cutOff - light.outercutOff;
    float intensity = clamp((theta - light.outercutOff) / epsilon, 0.0, 1.0);

    vec3 diffuse = mapsSample.diffuse*max(dot(norm, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mapsSample.shininess);
    vec3 specular = mapsSample.specular*spec;


    return (diffuse+specular)*light.color*intensity;
};

uniform vec3 viewPos;

void main()
{
    MapsSample mapsSample;
    mapsSample.ambient =material.ambient;
    mapsSample.diffuse = vec3(texture(material.diffuse, iTexCoord));
    mapsSample.specular = vec3(texture(material.specular, iTexCoord));
    mapsSample.shininess = material.shininess;

    vec3 viewDir=vec3(0.0);

    viewDir = normalize(viewPos - fragPos);

    //only after creating sampling, viewDir

    vec3 res = vec3(mapsSample.ambient*mapsSample.diffuse);
    res+= CalcDirLight(dirLight, inormal,viewDir, mapsSample);
    for(int i=0; i<pointLightsCount; i++){
        res+=CalcPointLight(pointLights[i], inormal, viewDir, mapsSample, fragPos);
    }
    for(int i=0;i<spotLightsCount; i++){
        res += CalcSpotLight(spotLights[i],inormal, viewDir, mapsSample, fragPos);
    }
    fragColor = vec4(res, 1.0);
}