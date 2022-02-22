#include<GL/glew.h>
#include"Shader.h"

class Material
{
public:
	Material();
	Material(GLfloat FloorSpecularIntensity, GLfloat FloorShiness);

	void UseMaterial(const Shader& shader);
	~Material();
private:
	GLfloat FloorSpecularIntensity;
	GLfloat FloorShiness;
};

