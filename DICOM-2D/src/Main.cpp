#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Tools.h"
#include "Matrices.h"
#include "Vectors.h"
#include "Controls.h"
#include "DICOMImporter.h"

using namespace std;
float backgroundColor[4] = { 1,1,1,1 };

void onDisplay()
{
	Matrix4 ModelLocalMatrix = (Controls::modelMatrix * Controls::localMatrix);
	Vector4 CameraWorldPosition = Controls::viewMatrix.getInverse().getColumn(3);

	float camPos[4] = { CameraWorldPosition.x, CameraWorldPosition.y, CameraWorldPosition.z, CameraWorldPosition.w };
	Tools::setShaderUniform4x4f(Controls::shaderProgram, (char*)"viewMatrix", Controls::viewMatrix);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Tools::setShaderUniform1i(Controls::shaderProgram, (char*)"window", Controls::window);
	Tools::setShaderUniform1i(Controls::shaderProgram, (char*)"level", Controls::level);
	Tools::setShaderUniform1i(Controls::shaderProgram, (char*)"useColormaps", Controls::useColormaps);

	glUniform1i(glGetUniformLocation(Controls::shaderProgram, "tex"), 0);
	glUniform1i(glGetUniformLocation(Controls::shaderProgram, "colormap"), 1);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	GLuint dicomTexture;
	glGenTextures(1, &dicomTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, dicomTexture);
	DICOMImporter::ApplyDicomTexture(Controls::currentDicom);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	GLuint colormapTexture;
	glGenTextures(1, &colormapTexture);
	glGenTextures(1, &colormapTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, colormapTexture);
	DICOMImporter::ApplyColormap(Controls::currentColormap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	glLoadIdentity();
	float planeCoords[12] = {
		 -1,-1,0,    -1,1,0,   1,1,0,   1,-1,0
	};

	float planeUVs[8] = {
		 0,0,   0,1,   1,1,   1,0
	};

	float* planeCoordsTransformed = Tools::Transform(planeCoords, 12, ModelLocalMatrix); // Doesn't apply the view matrix to these, only local and model

	glEnableVertexAttribArray(glGetAttribLocation(Controls::shaderProgram, "pos"));
	glVertexAttribPointer(glGetAttribLocation(Controls::shaderProgram, "pos"), 3, GL_FLOAT, false, 0, (GLvoid*)planeCoordsTransformed);

	glEnableVertexAttribArray(glGetAttribLocation(Controls::shaderProgram, "uv"));
	glVertexAttribPointer(glGetAttribLocation(Controls::shaderProgram, "uv"), 2, GL_FLOAT, false, 0, (GLvoid*)planeUVs);

	glDrawArrays(GL_QUADS, 0, 4); // Draw the cube

	glDisableVertexAttribArray(glGetAttribLocation(Controls::shaderProgram, "pos"));
	glDisableVertexAttribArray(glGetAttribLocation(Controls::shaderProgram, "uv"));

	char pre[] = "Filename: ";
	char* post = (char*)DICOMImporter::dicomFiles[Controls::currentDicom].c_str();
	char* dest = Tools::ConcatenateStrings(pre, post);

	Tools::writeStringToScreen(dest, Vector3(-1.25, -1.25, 0), Vector3(1, 1, 1));

	glutSwapBuffers();
}



int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(500, 500);
	glutCreateWindow("GL 2D DICOM Renderer");

	Tools::initGraphics(backgroundColor);
	glutDisplayFunc(onDisplay);
	glutReshapeFunc(Controls::onResize);
	glutMouseFunc(Controls::mouseButton);
	glutMotionFunc(Controls::mouseMotion);
	glutKeyboardFunc(Controls::keyboard);
	glutSpecialFunc(Controls::specialInputs);

	// Initialize Textures
	string shaderDirectory = std::filesystem::current_path().string() + "\\shaders";
	string dicomDirectory = std::filesystem::current_path().string() + "\\dicoms";
	string colormapDirectory = std::filesystem::current_path().string() + "\\colormaps";

	//if (argc == 1) {
	//	directoryPath = argv[1];
	//}
	Controls::shaderProgram = Tools::setShaders((shaderDirectory + "\\windowLevel.vert").c_str(), (shaderDirectory + "\\windowLevel.frag").c_str());
	DICOMImporter::GetDicomTexturesInPath(dicomDirectory.c_str());
	DICOMImporter::GetColormapsInPath(colormapDirectory.c_str());

	// Initialize Matrices
	Controls::modelMatrix.identity();
	Controls::localMatrix.identity();
	Controls::originMatrix.identity();
	Controls::viewMatrix.identity();
	Controls::viewMatrix.translate(-Vector3(0, 0, 8));
	Matrix4 ProjectionMatrix = Tools::GenerateOrthographicMatrix(3, 3, 0, 10);
	Tools::setShaderUniform4x4f(Controls::shaderProgram, (char*)"projectionMatrix", ProjectionMatrix);

	glutMainLoop();

	return 0;
}