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
#include "Tools.h"
#include "Matrices.h"
#include "Vectors.h"
#include "Controls.h"
#include "VolumeRendering.h"
#include <gdcmImageReader.h>
#include <gdcmImage.h>
#include "DICOMImporter.h"

#define PI 3.14159265359

using namespace std;

GLuint shaderProgram;
float backgroundColor[4] = { 1,1,1,1 };
GLfloat* volumeData;
int volDims[3] = { 2,2,2 };
char dicomFilename[] = "MRBRAIN.DCM";

bool didOnce = false;
char* buffer;

GLubyte testDataVolume[64] = {
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,

	0,0,0,0,
	0,255,255,0,
	0,255,255,0,
	0,0,0,0,

	0,0,0,0,
	0,255,255,0,
	0,255,255,0,
	0,0,0,0,

	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0 };


void onDisplay()
{
	Matrix4 ModelLocalMatrix = (Controls::modelMatrix * Controls::localMatrix);
	Vector4 CameraWorldPosition = Controls::viewMatrix.getInverse().getColumn(3);

	float camPos[4] = { CameraWorldPosition.x, CameraWorldPosition.y, CameraWorldPosition.z, CameraWorldPosition.w };
	Tools::setShaderUniform4x4f(shaderProgram, (char*)"viewMatrix", Controls::viewMatrix);
	Tools::setShaderUniform4f(shaderProgram, (char*)"cameraPosition", camPos);
	Tools::setShaderUniform3i(shaderProgram, (char*)"volume_dims", volDims);

	Tools::setShaderUniform1i(shaderProgram, (char*)"window", Controls::window);
	Tools::setShaderUniform1i(shaderProgram, (char*)"level", Controls::level);
	Tools::setShaderUniform1i(shaderProgram, (char*)"useColormaps", Controls::useColormaps);
	glUniform1i(glGetUniformLocation(shaderProgram, "colormap"), 1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	Matrix4 ProjectionMatrix;

	if (Controls::PERSPECTIVE) {
		// Perpective Projection 
		ProjectionMatrix = Tools::GeneratePerspectiveMatrix(10, 8, 8 * Controls::window_height / Controls::window_width, 2, 10);
		//gluPerspective(60, (GLdouble)Controls::window_width / Controls::window_height, 1, 10000);
		glutSetWindowTitle("Perspective");
	}
	else {
		// Orthogonal Projection 
		ProjectionMatrix = Tools::GenerateOrthographicMatrix(8, 8 * Controls::window_height / Controls::window_width, 2, 10);
		glutSetWindowTitle("Orthographic");
	}

	Tools::setShaderUniform4x4f(shaderProgram, (char*)"projectionMatrix", ProjectionMatrix);


	// Set model and camera characteristics
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glCullFace(GL_FRONT);

	GLuint textureID3D;
	glGenTextures(1, &textureID3D);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_3D, textureID3D);

	if (Controls::SIMULATED) {
		volDims[0] = 2;
		volDims[1] = 2;
		volDims[2] = 2;

		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, 4, 4, 4, 0, GL_RED, GL_UNSIGNED_BYTE, testDataVolume);
	}
	else {
		volDims[0] = DICOMImporter::width;
		volDims[1] = DICOMImporter::height;
		volDims[2] = DICOMImporter::numberOfTextures;
		DICOMImporter::ApplyTexture3D();
	}

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	VolumeRendering::DrawArrayVolumeCube(ModelLocalMatrix, 1);


	glutSwapBuffers();
}


int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Teapot");

	Tools::initGraphics(backgroundColor);
	glutDisplayFunc(onDisplay);
	glutReshapeFunc(Controls::onResize);
	glutMouseFunc(Controls::mouseButton);
	glutMotionFunc(Controls::mouseMotion);
	glutKeyboardFunc(Controls::keyboard);

	// Initialize Volumes
	string dicomDirectory = std::filesystem::current_path().string() + "\\dicoms";
	DICOMImporter::GetDicomTexturesInPath(dicomDirectory.c_str());

	string colormapDirectory = std::filesystem::current_path().string() + "\\colormaps";
	DICOMImporter::GetColormapsInPath(colormapDirectory.c_str());

	string shaderDirectory = std::filesystem::current_path().string() + "\\shaders\\";
	shaderProgram = Tools::setShaders((shaderDirectory+"volume.vert").c_str(), (shaderDirectory + "volume.frag").c_str());

	// Initialize Matrices
	Controls::modelMatrix.identity();
	Controls::localMatrix.identity();
	Controls::originMatrix.identity();
	Controls::viewMatrix.identity();
	Controls::viewMatrix.translate(Vector3(0, 3, 0));
	Controls::viewMatrix.rotate(180,Vector3(0, 0, 1));
	Controls::viewMatrix.rotate(90, Vector3(1, 0, 0));

	glutMainLoop();

	return 0;
}