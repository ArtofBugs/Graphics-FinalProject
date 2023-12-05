#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H

#include <ctype.h>
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <math.h>

#ifdef WIN32
#include <windows.h>
#endif

#ifdef WIN32
#include "glew.h"
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include "glut.h"
#include "glm/glm.hpp"
#include <map>
#include <stdarg.h>


inline int GetOSU(int flag)
{
	int i;
	glGetIntegerv(flag, &i);
	return i;
}


void	CheckGlErrors(const char*);



class GLSLProgram
{
private:
	std::map<char*, int>	AttributeLocs;
	char* Cfile;
	unsigned int		Cshader;
	char* Ffile;
	unsigned int		Fshader;
	char* Gfile;
	GLuint			Gshader;
	bool			IncludeGstap;
	GLenum			InputTopology;
	GLenum			OutputTopology;
	GLuint			Program;
	char* TCfile;
	GLuint			TCshader;
	char* TEfile;
	GLuint			TEshader;
	std::map<char*, int>	UniformLocs;
	bool			Valid;
	char* Vfile;
	GLuint			Vshader;
	bool			Verbose;

	static int		CurrentProgram;

	void	AttachShader(GLuint);
	bool	CanDoFragmentShaders;
	bool	CanDoVertexShaders;
	int	CompileShader(GLuint);
	bool	CreateHelper(char*, ...);
	int	GetAttributeLocation(char*);
	int	GetUniformLocation(char*);


public:
	GLSLProgram();

	bool	Create(char*, char* = NULL, char* = NULL, char* = NULL, char* = NULL, char* = NULL);
	void	Init();
	bool	IsExtensionSupported(const char*);
	bool	IsNotValid();
	bool	IsValid();
	void	SetAttributeVariable(char*, int);
	void	SetAttributeVariable(char*, float);
	void	SetAttributeVariable(char*, float, float, float);
	void	SetAttributeVariable(char*, float[3]);
	void	SetGstap(bool);
	void	SetInputTopology(GLenum);
	void	SetOutputTopology(GLenum);
	void	SetUniformVariable(char*, int);
	void	SetUniformVariable(char*, float);
	void	SetUniformVariable(char*, float, float, float);
	void	SetUniformVariable(char*, float[3]);
	void	SetUniformVariable(char*, glm::mat4&);
	void	SetUniformVariable(char*, glm::vec3&);
	void	SetVerbose(bool);
	void	UnUse();
	void	Use();
	void	Use(GLuint);
	void	UseFixedFunction();
};

#endif		// #ifndef GLSLPROGRAM_H
