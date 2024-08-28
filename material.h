#include "texture.h"
struct Mat {
	Texture diffuse;
	Texture specular;
	Texture normal;
	float shininess=32.0;
	unsigned int shaderIndex=0;
	Mat(void*) {};
	Mat(Texture diffuse = Texture("res/black.png", TextureType::diffuse),
		Texture specular = Texture("res/black.png", TextureType::specular), 
		Texture normal = Texture("res/normal.png", TextureType::normal), 
		float shininess = 32.0) {
		
		this->diffuse = diffuse;
		this->specular = specular;
		this->shininess = shininess;
		this->normal = normal;
	};
};