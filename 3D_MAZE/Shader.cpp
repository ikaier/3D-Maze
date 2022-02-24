#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
	transformLocation = 0;
	projectionLocation = 0;
	viewLocation = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string geometryString = ReadFile(geometryLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* geometryCode = geometryString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, geometryCode, fragmentCode);
}

void Shader::Validate()
{
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating program '%s'\n", eLog);
		return;
	}
}


void Shader::UseShader()
{
	glUseProgram(shaderID);
}

void Shader::ClearShader()
{
	if (shaderID != 0) {
		glDeleteProgram(shaderID);
		shaderID = 0;
	}
}

//void Shader::SetPointLights(WallLight* pLight, unsigned int lightCount)
//{
//	if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;
//	glUniform1i(pointLightCountLocation, lightCount);
//	for (size_t i = 0; i < lightCount; i++) {
//		pLight[i].Apply(uniformPointLight[i].uniformPosition);
//	}
//}

//void Shader::SetSpotLight(FlashLight Slight)
//{
//	Slight.applyFlash(spotLightPositionLocation, spotLighDirectiontLocation);
//}

void Shader::setInt(const std::string& name, const int value) const
{
	glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(shaderID,name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setvec4(const std::string& name, const glm::vec4& vec) const
{
	glUniform4f(glGetUniformLocation(shaderID, name.c_str()), vec.x,vec.y,vec.z,vec.w);
}

void Shader::setvec2(const std::string& name, const glm::vec2& vec) const
{
	glUniform2f(glGetUniformLocation(shaderID, name.c_str()), vec.x, vec.y);
}

void Shader::setvec3(const std::string& name, const glm::vec3& vec) const
{
	glUniform3f(glGetUniformLocation(shaderID, name.c_str()), vec.x, vec.y, vec.z);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(shaderID, name.c_str()), value);
}

GLuint Shader::GetTransformLocation() const
{
	return transformLocation;
}

GLuint Shader::GetPositionLocation()
{
	return transformLocation;
}


std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open()) {
		printf("failed to read %s! File doesn't exist", fileLocation);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	shaderID = glCreateProgram();

	if (!shaderID) {
		printf("Error creating shader program!\n");
		return;
	}

	GLuint verShader = AttachShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	GLuint fragShader = AttachShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	CompileProgram();

	glDeleteShader(verShader);
	glDeleteShader(fragShader);

}

void Shader::CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode)
{
	shaderID = glCreateProgram();

	if (!shaderID) {
		printf("Error creating shader program!\n");
		return;
	}

	GLuint verShader=AttachShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	GLuint geoShader=AttachShader(shaderID, geometryCode, GL_GEOMETRY_SHADER);
	GLuint fragShader =AttachShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	CompileProgram();
	glDeleteShader(verShader);
	glDeleteShader(geoShader);
	glDeleteShader(fragShader);
}

GLuint Shader::AttachShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };


	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader '%s'\n", shaderType, eLog);
		return -1;
	}
	glAttachShader(theProgram, theShader);
}

void Shader::CompileProgram()
{
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program '%s'\n", eLog);
		return;
	}
	
	transformLocation = glGetUniformLocation(shaderID, "model");
	projectionLocation = glGetUniformLocation(shaderID, "projection");
	viewLocation = glGetUniformLocation(shaderID, "view");
	pointLightCountLocation = glGetUniformLocation(shaderID, "pointLightCount");
	/*spotLightPositionLocation = glGetUniformLocation(shaderID, "spotLight.position");
	spotLighDirectiontLocation = glGetUniformLocation(shaderID, "spotLight.spotdirection");*/
	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++) {
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
		uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

	}
}
