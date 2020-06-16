

#pragma once
#include <string>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <gdcmImageReader.h>
#include <gl\glew.h>
#include <iterator>
#include <vector>

class DICOMImporter
{
public:
	static int width;
	static int height;
	static int numberOfTextures, numberOfColormaps;
	static void ApplyDicomTexture(int i);
	static void GetDicomTexturesInPath(const char* input);
	static void ApplyTexture3D();
	static void ApplyColormap(int i);
	static void GetColormapsInPath(const char* input);
	static std::vector <char*> textureBufferPointers;
	static std::vector<std::string> dicomFiles;
	static std::vector<std::string> colormapFiles;
};

