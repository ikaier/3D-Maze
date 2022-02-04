#pragma once
#include<GL/glew.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/quaternion.hpp>

class MyTransform
{
public:
	MyTransform();
	MyTransform(GLfloat transX,GLfloat transY = 0.0f,GLfloat transZ = 0.0f,
		GLfloat scaleX=1.0f, GLfloat scaleY = 1.0f, GLfloat scaleZ = 1.0f,
		GLfloat RotateX = 0.0f, GLfloat RotateY = 0.0f, GLfloat RotateZ = 0.0f);
	void ApplyTransform(GLuint uniformLocation);
	~MyTransform();
private:
	/*GLfloat transX,  transY,  transZ,
		 scaleX,  scaleY,  scaleZ,
		 RotateAngle;*/
	glm::mat4 model;

};

