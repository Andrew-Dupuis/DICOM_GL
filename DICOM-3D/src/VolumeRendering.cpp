#include "VolumeRendering.h"
#include "gdcmImageReader.h"



char* VolumeRendering::GenerateRandomTexels(char*& buffer, int width, int height, int depth) {
	for (int z = 0; z < depth; z++) {
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				int texel_index = (z * width * height) + (x * height) + y;
				buffer[texel_index] = rand() % 256;
				//std::cout <<"Index: " << texel_index << " Value: "<< texelArray[texel_index] << "\n ";		
			}
		}
	}
	return buffer;
}

char* VolumeRendering::GenerateSequentialTexels(char*& buffer, int width, int height, int depth) {
	for (int z = 0; z < depth; z++) {
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				int texel_index = (z * width * height) + (x * height) + y;
				buffer[texel_index] = 255 * ((float)(x * y * z) / (float)(width * height * depth));
				//std::cout <<"Index: " << texel_index << " Value: "<< texelArray[texel_index] << "\n ";		
			}
		}
	}
	return buffer;
}

char* VolumeRendering::GenerateSequentialRGBTexels(char*& buffer, int width, int height, int depth) {
	std::vector<char> generator;
	for (int z = 0; z < depth; z++) {
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				generator.push_back(x * (float)width / (float)x);
				generator.push_back(y * (float)height / (float)y);
				generator.push_back(z * (float)depth / (float)z);

				//std::cout <<"Index: " << texel_index << " Value: "<< texelArray[texel_index] << "\n ";		
			}
		}
	}
	buffer = &generator[0];
	return buffer;
}


GLfloat* VolumeRendering::GenerateRedTexels(int width, int height, int depth) {
	GLfloat* texelArray = new GLfloat[width * height * depth * 3];
	for (int z = 0; z < depth; z++) {
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				int texel_index = (z * width * height * 3) + (x * height * 3) + y * 3;
				texelArray[texel_index] = 1;
				std::cout << texelArray[texel_index] << " ";
				texelArray[texel_index + 1] = 0;
				std::cout << texelArray[texel_index + 1] << " ";
				texelArray[texel_index + 2] = 0;
				std::cout << texelArray[texel_index + 2] << "\n";
			}
		}
	}
	return texelArray;
}



unsigned int VolumeRendering::LoadVolume(GLuint shaderProgram, GLfloat* volumeData, float* volDims) {

	unsigned int volume;
	glGenTextures(1, &volume);
	glBindTexture(GL_TEXTURE_3D, volume);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, volDims[0], volDims[1], volDims[2], 0, GL_RGB, GL_FLOAT, &volumeData);

	return volume;
}



void VolumeRendering::DrawArrayVolumeCube(Matrix4 ModelLocalMatrix, GLuint volumeTexture) {
	glLoadIdentity();             // Set up modelview transform, first cube.
	float cubeCoords[72] = {
		   1,1,1,    -1,1,1,   -1,-1,1,   1,-1,1,      // face #1
		   1,1,1,     1,-1,1,   1,-1,-1,  1,1,-1,      // face #2
		   1,1,1,     1,1,-1,  -1,1,-1,  -1,1,1,       // face #3
		   -1,-1,-1, -1,1,-1,   1,1,-1,   1,-1,-1,     // face #4
		   -1,-1,-1, -1,-1,1,  -1,1,1,   -1,1,-1,      // face #5
		   -1,-1,-1,  1,-1,-1,  1,-1,1,   -1,-1,1 };  // face #6

	float cubeNormals[72] = {
	   0,0,-1,    0,0,-1,   0,0,-1,   0,0,-1,      // front
	   -1,0,0,     -1,0,0,   -1,0,0,  -1,0,0,      // right
	   0,-1,0,     0,-1,0,   0,-1,0,  0,-1,0,       // top
	   0,0,1,    0,0,1,   0,0,1,   0,0,1,      // back
	   1,0,0,     1,0,0,   1,0,0,  1,0,0,      // left
	   0,1,0,     0,1,0,   0,1,0,  0,1,0 };       // bottom

	float cubeUVWs[72] = {
		 1,1,1,    0,1,1,   0,0,1,   1,0,1,      // face #1
		 1,1,1,    1,0,1,   1,0,0,   1,1,0,      // face #2
		 1,1,1,    1,1,0,   0,1,0,   0,1,1,       // face #3
		 0,0,0,    0,1,0,   1,1,0,   1,0,0,     // face #4
		 0,0,0,    0,0,1,   0,1,1,   0,1,0,      // face #5
		 0,0,0,    1,0,0,   1,0,1,   0,0,1 };  // face #6

	GLubyte cubeFaceColors[96] = {
			255,0,0,255,  255,0,0,255,   255,0,0,255,   255,0,0,255,       // face #1 is red
			0,255,0,255,   0,255,0,255,   0,255,0,255,   0,255,0,255,       // face #2 is green
			0,0,255,255,   0,0,255,255,   0,0,255,255,   0,0,255,255,       // face #3 is blue
			255,255,0,255,   255,255,0,255,   255,255,0,255,   255,255,0,255,       // face #4 is yellow
			0,255,255,255,   0,255,255,255,   0,255,255,255,   0,255,255,255,       // face #5 is cyan
			255,0,255,255,   255,0,255,255,   255,0,255,255,   255,0,255, 255 }; // face #6 is red

	float* cubeTransformedCoords = VolumeRendering::Transform(cubeUVWs, 72, ModelLocalMatrix); // Doesn't apply the view matrix to these, only local and model

	glVertexPointer(3, GL_FLOAT, 0, cubeTransformedCoords);  // Set data type and location, first cube.
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, cubeFaceColors);
	glNormalPointer(GL_FLOAT, 0, cubeNormals);
	glTexCoordPointer(3, GL_FLOAT, 0, cubeUVWs);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glDrawArrays(GL_QUADS, 0, 24); // Draw the cube

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

float* VolumeRendering::Transform(float* verts, int count, Matrix4 transformationMatrix) {

	float* transformedVerts = new float[count];

	for (int i = 0; i < count; i = i + 3) {
		Vector4 vertex = Vector4(verts[i], verts[i + 1], verts[i + 2], 1);
		vertex = transformationMatrix * vertex;
		transformedVerts[i] = vertex.x;
		transformedVerts[i + 1] = vertex.y;
		transformedVerts[i + 2] = vertex.z;
	}
	return transformedVerts;
}

void VolumeRendering::DrawVolumeCube() {
	glBegin(GL_QUADS);
	// top
	glColor3f(1.0f, 0.0f, 0.0f);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord3d(0, 1, 1);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glTexCoord3d(1, 1, 1);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glTexCoord3d(1, 1, 0);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glTexCoord3d(0, 1, 0);
	glVertex3f(-0.5f, 0.5f, -0.5f);

	glEnd();

	glBegin(GL_QUADS);
	// front
	glColor3f(0.0f, 1.0f, 0.0f);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord3d(0, 0, 1);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord3d(1, 0, 1);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord3d(1, 1, 1);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glTexCoord3d(0, 1, 1);
	glVertex3f(-0.5f, 0.5f, 0.5f);

	glEnd();

	glBegin(GL_QUADS);
	// right
	glColor3f(0.0f, 0.0f, 1.0f);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord3d(1, 0, 1);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord3d(1, 0, 0);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord3d(1, 1, 0);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glTexCoord3d(1, 1, 1);
	glVertex3f(0.5f, 0.5f, 0.5f);

	glEnd();

	glBegin(GL_QUADS);
	// left
	glColor3f(0.0f, 0.0f, 0.5f);
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord3d(0, 0, 1);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord3d(0, 1, 1);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glTexCoord3d(0, 1, 0);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glTexCoord3d(0, 0, 0);
	glVertex3f(-0.5f, -0.5f, -0.5f);

	glEnd();

	glBegin(GL_QUADS);
	// bottom
	glColor3f(0.5f, 0.0f, 0.0f);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord3d(0, 0, 1);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord3d(1, 0, 1);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord3d(1, 0, 0);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord3d(0, 0, 0);
	glVertex3f(-0.5f, -0.5f, -0.5f);

	glEnd();

	glBegin(GL_QUADS);
	// back
	glColor3f(0.0f, 0.5f, 0.0f);
	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord3d(1, 1, 0);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glTexCoord3d(1, 0, 0);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord3d(0, 0, 0);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord3d(0, 1, 0);
	glVertex3f(-0.5f, 0.5f, -0.5f);

	glEnd();
}