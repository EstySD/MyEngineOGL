#include "glad/glad.h"
#include <vector>
#include <iostream>
enum TextureType {
	diffuse,
	specular
};
class Texture {
public:
	unsigned int id;
	std::string path;
	TextureType type;
	Texture();
	Texture(const char* texturePath, TextureType type);
	void Bind();
private:
	void Import(const char* texturePath, TextureType type);
	void Gen();
};