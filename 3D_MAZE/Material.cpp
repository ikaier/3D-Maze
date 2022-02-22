#include "Material.h"

Material::Material()
{
	FloorSpecularIntensity = 0.0f;
	FloorShiness = 0.0f;
}

Material::Material(GLfloat FloorSpecularIntensity, GLfloat FloorShiness)
{
	this->FloorSpecularIntensity = FloorSpecularIntensity;
	this->FloorShiness = FloorShiness;
}

void Material::UseMaterial(const Shader& shader) 
{
	shader.setFloat("material.shininess", FloorShiness);
	shader.setFloat("material.specularIntensity", FloorSpecularIntensity);
}

Material::~Material()
{
}
