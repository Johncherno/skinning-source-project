#include "shaderProgram.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include "gtc/type_ptr.hpp"
#include"glm.hpp"
bool shaderProgram::loadShaders(const char* vsFilename, const char* fsFilename)
{
	string vsString = fileToString(vsFilename);
	string fsString = fileToString(fsFilename);

	const GLchar* vertexShaderSrc;
	const GLchar* fragmentShaderSrc;
	vertexShaderSrc = vsString.c_str();
	fragmentShaderSrc = fsString.c_str();
	
	

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertexShaderSrc, NULL);//将输入字符串中所含顶点坐标  的字符串指针地址赋值到vs
	glCompileShader(vs);//获取vs地址进行编译
	checkCompileErrors(vs,VERTEX);

	
	glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vs, sizeof(infoLog), NULL, infoLog);
		std::cout << "error vertxex failed compile" << infoLog << std::endl;
	}
      
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragmentShaderSrc, NULL);//将输入字符串中所含颜色数据  的字符串指针地址赋值到fs
	glCompileShader(fs);//获取fs地址进行编译
	checkCompileErrors(fs, FRAGMENT);



	glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fs, sizeof(infoLog), NULL, infoLog);
		std::cout << "error fragment failed compile" << infoLog << std::endl;
	}

	mHandle = glCreateProgram();
	glAttachShader(mHandle, vs);
	glAttachShader(mHandle, fs);
	glLinkProgram(mHandle);
	
	checkCompileErrors(mHandle,PROGRAM);





	glDeleteShader(vs);
	glDeleteShader(fs);
	return true;
}
void shaderProgram::Add_Geometry_shader(const char* vsFilename, const char* fsFilename, const char* gsFilename)
{
	string vsString = fileToString(vsFilename);
	string fsString = fileToString(fsFilename);
	string gsString = fileToString(gsFilename);
	const GLchar* vertexShaderSrc;
	const GLchar* fragmentShaderSrc;
	const GLchar* geometryShaderSrc;

	vertexShaderSrc = vsString.c_str();
	fragmentShaderSrc = fsString.c_str();
	geometryShaderSrc = gsString.c_str();


	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertexShaderSrc, NULL);//将输入字符串中所含顶点坐标  的字符串指针地址赋值到vs
	glCompileShader(vs);//获取vs地址进行编译
	checkCompileErrors(vs, VERTEX);


	glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vs, sizeof(infoLog), NULL, infoLog);
		std::cout << "error vertxex failed compile" << infoLog << std::endl;
	}//顶点着色器




	GLuint gs = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(gs, 1, &geometryShaderSrc, NULL);//将输入字符串中所含几何信息  的字符串指针地址赋值到vs
	glCompileShader(gs);//获取vs地址进行编译
	checkCompileErrors(gs, GEOMETRY);


	glGetShaderiv(gs, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(gs, sizeof(infoLog), NULL, infoLog);
		std::cout << "error geometry failed compile" << infoLog << std::endl;
	}//几何着色器





	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragmentShaderSrc, NULL);//将输入字符串中所含颜色数据  的字符串指针地址赋值到fs
	glCompileShader(fs);//获取fs地址进行编译
	checkCompileErrors(fs, FRAGMENT);



	glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fs, sizeof(infoLog), NULL, infoLog);
		std::cout << "error fragment failed compile" << infoLog << std::endl;
	}//颜色着色器




	mHandle = glCreateProgram();
	glAttachShader(mHandle, vs);
	glAttachShader(mHandle, gs);
	glAttachShader(mHandle, fs);
	glLinkProgram(mHandle);

	checkCompileErrors(mHandle, PROGRAM);





	glDeleteShader(vs);
	glDeleteShader(gs);
	glDeleteShader(fs);
}
GLuint shaderProgram::getprogram()const
{
	return mHandle;
}
void shaderProgram::use()
{
	if (mHandle>0)
	{
		glUseProgram(mHandle);

	}
}
string shaderProgram::fileToString(const string& filename)
{
	std::stringstream ss;
	std::ifstream file;
	try
	{
		file.open(filename,std::ios::in);
		if (!file.fail())
	  {
		ss << file.rdbuf();
	  }
	  file.close(); 
	}
	catch (std::exception ex)
	{
		std::cerr << "error reading shader filename" << std::endl;
	}
	return ss.str();
}
void shaderProgram::checkCompileErrors(GLuint shader, ShaderType type)
{
	int status=1;
	if (type==PROGRAM)
	{
		glGetProgramiv(mHandle, GL_LINK_STATUS, &status);
		if (status==GL_FALSE)
		{
			GLint length = 0;
			glGetProgramiv(mHandle,GL_INFO_LOG_LENGTH,&length);
			string errorLog(length,' ');
			glGetProgramInfoLog(mHandle, length,&length,&errorLog[0]);
			std::cerr << "error program link" << errorLog << std::endl;
		}
	}
	else
	{
		glGetShaderiv(mHandle, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint length = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			string errorLog(length, ' ');
			glGetShaderInfoLog(mHandle, length, &length, &errorLog[0]);
			std::cerr << "error shader compile" << errorLog << std::endl;
		}
	}
}
GLint shaderProgram::getUniformLocation(const GLchar* name)
{

	GLint it = glGetUniformLocation(mHandle, name);;
	return it;
}
void shaderProgram::setUniform(const GLchar* name, const glm::vec2& v)
{
	GLint loc = getUniformLocation(name);
	glUniform2f(loc,v.x,v.y);
}
void shaderProgram::setUniform(const GLchar* name, const glm::vec3& v)
{
	GLint loc = getUniformLocation(name);
	glUniform3f(loc, v.x, v.y,v.z);
}
void shaderProgram::setUniform(const GLchar* name, const glm::vec4& v)
{
   GLint loc = getUniformLocation(name);
   glUniform4f(loc, v.x, v.y,v.z,v.w);
}
void shaderProgram::setUniform(const GLchar* name, const glm::mat4& m)
{
	GLint loc = getUniformLocation(name);
	glUniformMatrix4fv(loc,1,GL_FALSE,&m[0][0]);
}
void shaderProgram::setUniform(const GLchar* name, float& m)
{
	GLint loc = getUniformLocation(name);
	glUniform1f(loc, m);
}
void shaderProgram::setUniform(const std::string& name, const glm::mat4& m)
{
	GLint loc = getUniformLocation(name.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}
void shaderProgram::setUniform(const std::string& name, const glm::vec3& m)
{
	GLint loc = getUniformLocation(name.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0]);
}
void shaderProgram::setUniform(const std::string& name, const glm::mat4 m[])
{
	GLint loc = getUniformLocation(name.c_str());
	glUniformMatrix4fv(loc, sizeof(m)/ sizeof(glm::mat4), GL_FALSE, glm::value_ptr(m[0]));
}