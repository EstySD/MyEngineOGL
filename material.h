#include "texture.h"
#include "shader.h"

#ifndef MATERIAL_H_
#define MATERIAL_H_
enum MatType {
	opaque,
	alphaBlend,
	alphaTest
};
class Mat {
public:
	Texture diffuse;
	Texture specular;
	Texture normal;
	float shininess = 32.0;
	unsigned int shaderIndex = 0;
	MatType matType = MatType::opaque;
	bool empty = false; //empty material flag
	Mat() {
		empty = true;
	};
	Mat(Texture diffuse,
		Texture specular = Texture(),
		Texture normal = Texture(),
		float shininess = 32.0);
	void setMode(MatType matType);
	MatType& getMode();
	void assign(Shader& shader);
};
#endif // !MATERIAL_H_

