#include <GL/glew.h>
#include "shader.h"
#include<string>
#include <iostream>
#include <fstream>

bool ReadFile(const char* pFileName, std::string& outFile)
{
	std::ifstream f(pFileName);

	bool ret = false;

	if (f.is_open()) {
		std::string line;
		while (getline(f, line)) {
			outFile.append(line);
			outFile.append("\n");
		}

		f.close();

		ret = true;
	}
	else {
		std::cerr << "Cannot open file " << pFileName << " for read" << std::endl;
	}

	return ret;
}

bool AddShader(GLuint program, GLenum ShaderType, const char * pFilename, GLuint* shader)
{
	std::string s;

	if (!ReadFile(pFilename, s)) {
		return false;
	}

	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		return false;
	}

	// Save the shader object - will be deleted in the destructor
	//m_shaderObjList.push_back(ShaderObj);
	*shader = ShaderObj;

	const GLchar* p[1];
	p[0] = s.c_str();
	GLint Lengths[1] = { (GLint)s.size() };

	glShaderSource(ShaderObj, 1, p, Lengths);

	glCompileShader(ShaderObj);

	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling '%s': '%s'\n", pFilename, InfoLog);
		return false;
	}

	glAttachShader(program, ShaderObj);
	//std::cout << s << std::endl;
	return true;
}

//std::vector<std::string> Shader::GetUniformNames()
//{
//	return std::vector<std::string>();
//}

//void Shader::LocateUniform(std::string name, Uniform & uniform)
//{
//}

void CShader::UseProgram()
{
	glUseProgram(this->program);
}

CShader::CShader()
{
	/*auto names = this->GetUniformNames();
	for (auto n = names.begin(); n != names.end(); n++) {
		this->uniforms.push_back(std::pair<std::string, int>(*n, -1));
	}*/
}

void CShader::Load(const char * vs, const char * fs, const char * gs)
{
	this->program = glCreateProgram();
	GLuint shader_vs = 0;
	GLuint shader_fs = 0;
	GLuint shader_gs = 0;
	vs && AddShader(this->program, GL_VERTEX_SHADER, vs, &shader_vs);
	fs && AddShader(this->program, GL_FRAGMENT_SHADER, fs, &shader_fs);
	gs && AddShader(this->program, GL_GEOMETRY_SHADER, gs, &shader_gs);


	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(this->program);

	glGetProgramiv(this->program, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(this->program, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		return;
	}

	glValidateProgram(this->program);
	glGetProgramiv(this->program, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(this->program, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		//   return false;
	}

	// Delete the intermediate shader objects that have been added to the program
	if (shader_vs) glDeleteShader(shader_vs);
	if (shader_fs) glDeleteShader(shader_fs);
	if (shader_gs) glDeleteShader(shader_gs);


	/*for (auto u = this->uniforms.begin(); u != this->uniforms.end(); u++) {
		u->second = glGetUniformLocation(this->program, u->first.c_str());
		if (u->second < 0) {
			std::cerr << "Cannot find uniform `" << u->first << "`" << std::endl;
		}
	}*/

}

//void Shader::LocateUniform(std::string name, Uniform * uniform)
//{
//	uniform->location = glGetUniformLocation(this->program, name.c_str());
//	if (uniform->location < 0) {
//		std::cerr << "Cannot find uniform `" << name << "`" << std::endl;
//	}
//}

void CShader::LocateUniform(std::string name, Uniform & uniform)
{
	uniform.location = glGetUniformLocation(this->program, name.c_str());
	if (uniform.location < 0) {
		std::cerr << "Cannot find uniform `" << name << "`" << std::endl;
		uniform.program = -1;
	}
	else {
		uniform.program = this->program;
	}
}


CShader::~CShader()
{
}

Shader CShader::Create()
{
	return Shader(new CShader);
}

CBlockShader::CBlockShader()
{
	/*this->Load("shader/block/block.vs", "shader/block/block.fs", "shader/block/block.gs");
	LocateUniform("MVP", &mvp);*/
}

BlockShader CBlockShader::Create()
{
	return BlockShader(new CBlockShader);
}



ScreenShader CScreenShader::Create()
{
	return ScreenShader(new CScreenShader);
}
