#ifndef SHADER_PROGRAM_H
#define  SHADER_PROGRAM_H
#include<GL/glew.h>
#include<string>
#include<glm.hpp>
#include<map>
#include "gtc/type_ptr.hpp"
using std::string;
class shaderProgram
{
public:
	enum  ShaderType
	{
		VERTEX,
	    FRAGMENT,
		GEOMETRY,
		PROGRAM
	};
	bool loadShaders(const char* vsFilename, const char* fsFilename);
	void Add_Geometry_shader(const char* vsFilename, const char* fsFilename, const char* gsFilename);
	void use();
	void setUniform(const GLchar* name, const glm::vec2& v);
    void setUniform(const GLchar* name, const glm::vec3& v);
	void setUniform(const GLchar* name, const glm::vec4& v);
	void setUniform(const GLchar* name, const glm::mat4& m);
	void setUniform(const std::string&name,const glm::mat4&m);
	void setUniform(const std::string&name,const glm::vec3& m);
	void setUniform(const std::string& name, const glm::mat4 m[]);
	void setUniform(const GLchar* name, float& m);
	GLuint getprogram()const;
private:
	GLint getUniformLocation(const GLchar *name);
	string fileToString(const string& filename);
	void checkCompileErrors(GLuint shader, ShaderType type);
	GLuint mHandle;
	GLint result;
	GLchar infoLog[512];
};
#endif 
