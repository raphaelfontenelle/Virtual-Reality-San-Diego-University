/* main.cpp

	==> Reads input scene-description file given by single argument on the command-line
	==> Parses to a Scene object
	==> Raytraces the Scene
	==> Outputs the raytraced image

*/

#include <FreeImage.h>
#include <iostream>

#include "./utilities/ReadFile.h"

using namespace std;

void printWelcomeMessage(char* fname) {
	cout << "\t\tHello and Welcome to the Glorious Raytracer!\n\n";
	
	cout << "Today we are going to attempt to paint the picture described by " << fname << "\n\n";
}

// Adapted from HW2 skeleton code.
void saveImage(int width, int height, BYTE* pixels, string fname) {
	FreeImage_Initialise();

	FIBITMAP* img = FreeImage_ConvertFromRawBits(pixels, width, height, width * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, true);

	cout << "Saving screenshot: " << fname << "\n";
	FreeImage_Save(FIF_PNG, img, fname.c_str(), 0);

	FreeImage_Unload(img);  // Free the FIBITMAP
	FreeImage_DeInitialise();
}

int main(int argc, char* argv[])
{
	if (argc != 2) {
		cerr << "Usage: Raytracer.exe scenefile\n";
		exit(-1);
	}

	char* fname = argv[1];
	printWelcomeMessage(fname);
	
	cout << "Let's get started by parsing " << fname << "\n";
	ReadFile::FileData sceneData = ReadFile::readfile(fname);
	Scene scene = sceneData.scene;
	cout << "We're done parsing " << fname << "\n\n";
	
	cout << "Ok here comes the hard part! Let's get started with the raytracing! This may take a while...\n";
	cout << "Why not grab a cuppa and go relax for a while? We'll be working hard in the meantime.\n";
	BYTE* pixels = scene.raytrace(sceneData.max_recursion_depth);
	cout << "Phew! We're done raytracing!\n";

	saveImage(scene.width(), scene.height(), pixels, sceneData.output_filename);
	cout << "All done now! Enjoy your beautiful picture! Goodbye!\n";

	delete[] pixels; pixels = nullptr;  // Free the raw pixel data generated by Scene.raytrace()
	return 0;
}
