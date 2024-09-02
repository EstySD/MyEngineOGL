#include <glad/glad.h>
#include <vector>
#include <iostream>
#ifndef TEXTURE_H_
#define TEXTURE_H_
enum TextureType {
	diffuse,
	specular,
	normal
};
class Texture {
public:
	unsigned int id;
	const char* path;
	TextureType type;
	Texture();
	Texture(const char* texturePath, TextureType type);
	void Bind();
private:
	void Import(const char* texturePath, TextureType type);
	void Gen();
};
#endif // !TEXTURE_H_
