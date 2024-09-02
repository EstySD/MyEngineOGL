#include "texture.h"
#ifndef MATERIAL_H_
#define MATERIAL_H_
struct Mat {
	Texture diffuse;
	Texture specular;
	Texture normal;
	float shininess = 32.0;
	unsigned int shaderIndex = 0;
	bool empty = false; //empty material flag
	Mat() {
		empty = true;
	};
	Mat(Texture diffuse,
		Texture specular = Texture(),
		Texture normal = Texture(),
		float shininess = 32.0) {
		empty = false;

		this->diffuse = diffuse;
		this->specular = specular;
		this->shininess = shininess;
		this->normal = normal;
	};
};
#endif // !MATERIAL_H_

