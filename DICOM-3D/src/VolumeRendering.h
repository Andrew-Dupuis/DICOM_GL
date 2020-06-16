#pragma once

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

class VolumeRendering
{
	public:
		
	static char* GenerateRandomTexels(char* &buffer, int width, int height, int depth);
	static char* GenerateSequentialTexels(char*& buffer, int width, int height, int depth);
	static char* GenerateSequentialRGBTexels(char*& buffer, int width, int height, int depth);
	static GLfloat* GenerateRedTexels(int width, int height, int depth);
	static unsigned int LoadVolume(GLuint shaderProgram, GLfloat* volumeData, float* volDims);
	static void DrawArrayVolumeCube(Matrix4 transformationMatrix, GLuint volumeTexture);
	static float* Transform(float* verts, int count, Matrix4 transformationMatrix);
	static void DrawVolumeCube();
};

