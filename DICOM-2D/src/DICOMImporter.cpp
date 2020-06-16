#include "DICOMImporter.h"
#include <SOIL.h>

namespace fs = std::filesystem;
char* allVolumeData;
bool loaded = false;
int xDim, yDim, zDim;
std::vector<char> results;
std::vector<int> stringTest;

int DICOMImporter::width;
int DICOMImporter::height;
int DICOMImporter::numberOfTextures;
int DICOMImporter::numberOfColormaps;

std::vector <char*> DICOMImporter::textureBufferPointers;
std::vector<std::string> DICOMImporter::dicomFiles;
std::vector<std::string> DICOMImporter::colormapFiles;

char* currentBuffer;
int oldDicomNumber = -1;


void DICOMImporter::ApplyDicomTexture(int i) {
	if (i != oldDicomNumber) {
		gdcm::ImageReader imReader;
		std::cout << "Loading " << dicomFiles[i].c_str() << "\n";
		imReader.SetFileName(dicomFiles[i].c_str());
		if (!imReader.Read()) {
			std::cerr << "Could not read dicom. Quitting importer.";
		}
		else {
			gdcm::Image& image = imReader.GetImage();

			currentBuffer = NULL;
			currentBuffer = new char[image.GetBufferLength()];
			image.GetBuffer(currentBuffer);
		}
		oldDicomNumber = i;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R16, width, height, 0, GL_RED, GL_UNSIGNED_SHORT, currentBuffer);
}

void DICOMImporter::GetDicomTexturesInPath(const char* input) {
	textureBufferPointers.clear();
	std::string path = input;
	for (const auto& entry : fs::directory_iterator(path)) {
		DICOMImporter::dicomFiles.push_back(entry.path().string());
	}
	gdcm::ImageReader imReader;
	imReader.SetFileName(dicomFiles[0].c_str());
	if (!imReader.Read()) {
		std::cerr << "Could not read first dicom. Quitting importer.";
	}
	else {
		gdcm::Image& image = imReader.GetImage();
		DICOMImporter::width = image.GetDimension(0);
		DICOMImporter::height = image.GetDimension(1);
		gdcm::PixelFormat pixFormat = image.GetPixelFormat();
		DICOMImporter::numberOfTextures = dicomFiles.size();
	}
}

void DICOMImporter::ApplyColormap(int i) {
	int colormapWidth, colormapHeight, colormapChannels;
	unsigned char* colorMap = SOIL_load_image(colormapFiles[i].c_str(), &colormapWidth, &colormapHeight, &colormapChannels, SOIL_LOAD_AUTO);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, colormapWidth, colormapHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, colorMap);
}

void DICOMImporter::GetColormapsInPath(const char* input) {
	std::string path = input;
	for (const auto& entry : fs::directory_iterator(path)) {
		DICOMImporter::colormapFiles.push_back(entry.path().string());
	}
	numberOfColormaps = colormapFiles.size();
}


