#include "material.h"

Mat::Mat(Texture diffuse, Texture specular, Texture normal, float shininess)
{
	empty = false;

	this->diffuse = diffuse;
	this->specular = specular;
	this->shininess = shininess;
	this->normal = normal;
}

void Mat::setMode(MatType matType)
{
		this->matType = matType;
}

MatType& Mat::getMode()
{
	return matType;
}

void Mat::assign(Shader& shader)
{
	shader.use();
	shader.setFloat("material.ambient", 0.05f);
	glActiveTexture(GL_TEXTURE0);
	diffuse.Bind();
	shader.setInt("material.diffuse", 0);
	glActiveTexture(GL_TEXTURE1);
	specular.Bind();
	shader.setInt("material.specular", 1);
	shader.setFloat("material.shininess", 32.0f);
}
