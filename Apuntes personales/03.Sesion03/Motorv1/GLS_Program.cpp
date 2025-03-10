#include "GLS_Program.h"
#include <vector>
#include <fstream>
#include "Error.h"

void GLS_Program::compileShader(const string& filePath, GLuint id)
{
	string fileContents = "";
	string line = "";
	ifstream shaderFile(filePath);

	if (shaderFile.fail())
	{
		fatalError("El archivo " + filePath + " no se encuentra disponible");
	}

	while (getline(shaderFile, line))
	{
		fileContents += line + "\n";
	}

	shaderFile.close();

	const char* contents = fileContents.c_str();
	glShaderSource(id, 1, &contents, nullptr);
	glCompileShader(id);

	GLint isCompiled = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		vector<GLchar> erroLog(maxLength);

		glGetShaderInfoLog(id, maxLength, &maxLength, &erroLog[0]);
		fatalError("Shaders no compilaron " + printf("%s", &(erroLog[0])));
		glDeleteShader(id);
		return;
	}
}

GLS_Program::GLS_Program()
	: numAttributes(0), programID(0), vertexShaderID(0), fragmentShaderID(0)
{
}

GLS_Program::~GLS_Program()
{
}

void GLS_Program::compileShaders(const string& vertexShaderFilePath, const string& fragmentShaderFilePath)
{
	programID = glCreateProgram();
	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(vertexShaderFilePath, vertexShaderID);
	compileShader(fragmentShaderFilePath, fragmentShaderID);
}

void GLS_Program::linkShaders()
{
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);
	GLint isLinked = 0;
	glGetProgramiv(programID, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(programID, maxLength, &maxLength, &infoLog[0]);
		glDeleteProgram(programID);
		fatalError("Shaders no linkearon " + printf("%s", &(infoLog[0])));
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		return;
	}

	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

void GLS_Program::addAttribute(const string attributeName)
{
	glBindAttribLocation(programID, numAttributes++, 
		attributeName.c_str());
}

void GLS_Program::use()
{
	glUseProgram(programID);
	for (int i = 0; i < numAttributes; i++)
	{
		glEnableVertexAttribArray(i);
	}
}

void GLS_Program::unuse()
{
	glUseProgram(0);
	for (int i = 0; i < numAttributes; i++)
	{
		glDisableVertexAttribArray(i);
	}
}

GLuint GLS_Program::getUniformLocation(const string& uniformName)
{
	GLuint location = glGetUniformLocation(programID, 
		uniformName.c_str());

	if (location == GL_INVALID_INDEX)
	{
		fatalError("Variable uniform " + uniformName + " no encontrado");
	}
	return location;
}
