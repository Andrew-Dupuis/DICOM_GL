#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#include <string.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <windows.h>
#include <random>
#include "Vectors.h"
#include "Matrices.h"

class Tools
{

public:
	static char* fileReadToCharArray(const char* fn);
	static void CompileShader(GLuint shader);
	static GLuint setShaders(const char* vertex, const char* fragment);
	static void setShaderUniform1f(GLuint program, char* variableName, GLfloat value);
	static void setShaderUniform2f(GLuint program, char* variableName, GLfloat value[2]);
	static void setShaderUniform3f(GLuint program, char* variableName, GLfloat value[3]);
	static void setShaderUniform4f(GLuint program, char* variableName, GLfloat value[4]);

	static void setShaderUniform1i(GLuint program, char* variableName, GLint value);
	static void setShaderUniform2i(GLuint program, char* variableName, GLint value[2]);
	static void setShaderUniform3i(GLuint program, char* variableName, GLint value[3]);
	static void initGraphics(float backgroundColor[4]);
	static Matrix4 GeneratePerspectiveMatrix(float ViewPlane, float ViewWidth, float ViewHeight, float NearPlane, float FarPlane);
	static Matrix4 GenerateOrthographicMatrix(float ViewWidth, float ViewHeight, float NearPlane, float FarPlane);
	static void drawCoordinateAxes(Matrix4 transformationMatrix, Color xAxis, Color yAxis, Color zAxis);
	static void setShaderUniform4i(GLuint program, char* variableName, GLint value[3]);

	static void setShaderUniform4x4f(GLuint program, char* variableName, Matrix4 value);

	//static void setShaderUniform4x4f(GLuint program, char* variableName, float* values);


};

