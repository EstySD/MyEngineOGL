#include "texture.h"
struct Mat {
	Texture diffuse;
	Texture specular;
	Texture normal;
	float shininess;
	Mat(Texture diffuse = Texture("res/black.png", TextureType::diffuse),
		Texture specular = Texture("res/black.png", TextureType::specular), 
		Texture normal = Texture("res/black.png", TextureType::specular), 
		float shininess = 32.0) {
		
		this->diffuse = diffuse;
		this->specular = specular;
		this->shininess = shininess;
		this->normal = normal;
	};
};