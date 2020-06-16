# View most DICOM files easily and quickly
DICOM-GL is a rendering program that lets you directly view DICOM images as either individual 2D slices or as 3D volumes. This project was developed to further my understanding of OpenGL's shader system.

## Overview
This project is meant to be a root-level implementation of a DICOM image viewing system from "scratch", as much as is feasible/logical. Therefore, this program fully implements Matrix and Vector classes rather than using an existing math package. Wherever possible, the native OpenGL and C++ calls were used rather than 3rd party packages. Features are implemented following the modern OpenGL pipeline, with the shaders implementing all elements of the Fixed Function Pipeline. This was done in order to learn how the modern implementations compare. 

DICOM-2D is a lightweight implementation of a DICOM rendering engine that allows for all of the standard clinical radiology controls, as well as extensible colormaps. Shaders are used to greatly accelerate the colormapping/window/level process for the rendered DICOM images. Textures are imported, sorted, and stored at full bit depth, with care taken to reduce load times and memory constraints. Images are fully interactable. 

DICOM-3D is a lightweight volume rendering application for DICOM stacks. It is implemented from scratch similarly to DICOM-2D, and uses the same architecture. Rather than rendering to a 2D plane, DICOM-3D places all of the DICOM image data into a Texture3D which is then sampled by an iterative raymarching shader to create a 3D visualization of the dataset. DICOM-3D still has some stability issues with importing large numbers of DICOMs, specifically related to the storage methods used to create the 3D volume buffer from the seperate image buffers. Therefore, the implementation shown below uses a manually-generated sample dataset to demonstrate that the shader elements of the pipeline are working. 

## Dependencies
The following dependencies are necessary evils to avoid bulk replication of others' work, especially with regards to non-OpenGL related elements such as the DICOM headers and PNG importing:
* Grassroots DICOM (gdcm): for the reading of image data directly from any file that complies with the DICOM standards
  * https://sourceforge.net/projects/gdcm/
* GLUT: for windowing and pipeline management.
  * NOTE: we used GLUTdlls37beta.zip, which is stable but old. This should probably be replaced with FreeGLUT.
  * https://www.opengl.org/resource/libraries/glut/glutdlls37beta.zip 
* GLEW: to enumerate all the OpenGL extensions necessary for modern shader support
  * http://glew.sourceforge.net/
* SOIL: to read PNGs into the application for colormaps, etc. 
  * https://www.lonesock.net/soil.html
  
## DICOM-2D
![GIF of DICOM-2D changing slice and window/level](https://github.com/apd47/DICOM-GL/raw/master/Pages/changing_slices_window_level.gif?raw=true)

DICOM-2D is a traditional "flat" DICOM renderer that runs from the command line or from a structured directory containing the target DICOM files. 

### Overview / Methods
DICOM-2D uses GLSL shaders to perform all transformations, colormapping, and window/level adjustment to a DICOM dataset. This piece of the program was intended as a technical exploration of the inner workings of the OpenGl shaders and an opportunity to learn C++. The rendering process is as follows:

1. Index all DICOM and colormap files within the directories provided, and store to a buffer of filepaths. 
2. Configure a shader program to use files in the shader directory, and compile the shaders with debugging to see if there are errors.  
3. Initialize 

### Quick Start
To get started quickly, download a prebuilt .zip file that includes sample colormaps and a sample DICOM dataset at:https://github.com/apd47/DICOM-GL/raw/master/Builds/DICOM-2D.zip

### Building from Source
In order to build DICOM-2D, you'll need to ensure you have all of the dependencies installed in accessible locations for Visual Studio. I recommend using vcpkg, Microsoft's dependency-management tool, to automatically install the above dependencies. 

### Usage
To render a DICOM dataset, first place all of the DICOM files into a single directory. This can be the DICOM directory within the root of the application's path, or can be somewhere else on your system. I highly recommend that DICOM files be ordered and named sequentially - this increases the chance that they will be read into the application in the proper order by the C++17 generic filesystem iterator. If you choose to use an external directory, you must launch the DICOM-2D.exe from the command line with the DICOM directory as the first argument:
 
```
DICOM-2D.exe //Loads the dataset within the application's root "Dicom" directory
DICOM-2D.exe "C://Dicom" // Loades the dataset present at C:/Dicom/
```

### Controls
If your DICOM set is loaded correctly, once the rendering has launched you will see a window that looks like this:

![Image of DICOM-2D at launch](https://github.com/apd47/DICOM-GL/blob/master/Pages/Launch_Screenshot.PNG?raw=true)

This window is displaying the first slice from your DICOM set. From here, you are able to use your mouse and keyboard to directly interact with all of the slices in your dataset using the following bindings

Action | Control Binding
------------ | -------------
Adjust Window | Left Click and Drag Left/Right
Adjuts Level | Left Click and Drag Up/Down
Cycle Through Dataset Slices | Up and Down Arrow Keys
Rotate 90 Degrees  | Left and Right Arrow Keys
Zoom (fixed) | Page Up / Page Down
Zoom (fluid) | Right Click and Drag Up/Down
Move Down,Left,Right,Up (fixed) | Numpad 2,4,6,8
Move (fluid) | Center Click and Drag Down,Left,Right,Up
Toggle Colormapping | c
Cycle Through Colormaps | Home / End

### Colormaps
Colormaps are defined by 1-dimensional textures that are sampled at runtime by the shader to map from the DICOM's windowed/leveld grayscale value to a color. Colormaps can be easily generated as PNGs in matlab for use with DICOM-2D, then changed in realtime within the program. 

![Gif of colormaps swapping out in real time](https://github.com/apd47/DICOM-GL/raw/master/Pages/colormaps_zooming.gif?raw=true)

## DICOM-3D
![GIF of DICOM-2D changing slice and window/level](https://github.com/apd47/DICOM-GL/blob/master/Pages/volume_render.gif?raw=true)

DICOM-3D is a volumetric DICOM renderer that runs from the command line or from a structured directory containing the target DICOM files. 








