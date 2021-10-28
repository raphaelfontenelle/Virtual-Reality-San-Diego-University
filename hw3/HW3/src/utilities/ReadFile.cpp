/* ReadFile.cpp

	==> Parses the file with filename to a Scene object 
*/

#include <fstream>
#include <iostream>
#include <vector>

#include "ReadFile.h"
#include "../model/shapes/Sphere.h"
#include "TransformStack.h"
#include "../model/shapes/Triangle.h"

#ifndef DEFAULT_FILE_NAME
#define DEFAULT_FILE_NAME "raytrace.png"
#endif

using namespace std;

// Function to read the input data values
// From HW2 skeleton.
bool ReadFile::readvals(stringstream& s, const int numvals, float* values)
{
	for (int i = 0; i < numvals; i++) {
		s >> values[i];
		if (s.fail()) {
			cout << "Failed reading value " << i << " will skip\n";
			return false;
		}
	}
	return true;
}

void ReadFile::ignoreCommandWithMessage(string cmd, int number_of_arguments, float* arguments) {
	cout << "Ignoring command " << cmd << " with " << (number_of_arguments == 1 ? "argument" : "arguments");
	for (int i = 0; i < number_of_arguments; i++) {
		cout << " " << arguments[i];
	}
	cout << endl;
}

// Parse the file with filename to a Scene object. Returns the Scene with additional raytracing settings as specified in the file.
ReadFile::FileData ReadFile::readfile(const char* filename)
{
	// Scene settings
	int width = 0, height = 0;		// Initialised to 0 so we can check if they aren't defined in the test file.
	int maxdepth = 5;				// Default recursive depth of 5.

	// Scene objects
	Camera camera;  
	vector<Point> vertexes; 
	vector<Triangle> triangles;
	vector<Sphere> spheres;
	vector<DirectionalLight> directional_lights;
	vector<PointLight> point_lights;

	// Material properties. Start with additive identity (black), since Colors are additive.
	unique_ptr<Color> diffuse(new Color(0.0f, 0.0f, 0.0f)); 
	unique_ptr<Color> specular(new Color(0.0f, 0.0f, 0.0f));
	float shininess = 0;
	unique_ptr<Color> emission(new Color(0.0f, 0.0f, 0.0f));		// Objects do not emit light by default
	unique_ptr<Color> ambient(new Color(0.2f, 0.2f, 0.2f));			// Default value for ambient property of objects

	Attenuation attenuation = { 1.0f, 0.0f, 0.0f };

	// Start a new matrix transform stack with the identity matrix
	TransformStack transform_stack = TransformStack();

	// Start reading file
	string str, cmd, output_file_name;
	ifstream in;
	in.open(filename);

	if (in.is_open()) {
		getline(in, str);
		while (in) {
			if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
				// #comment		-> ignore
				// blank line	-> ignore

				stringstream s(str);
				s >> cmd;
				float values[10]; // Stores the parameters for the command. Need up to 10 for camera.
				bool validinput;

				// size width height
				if (cmd == "size") {
					validinput = readvals(s, 2, values);
					if (validinput) {
						width = (int)values[0];
						height = (int)values[1];
					}
				}

				// maxdepth depth
				else if (cmd == "maxdepth") {
					validinput = readvals(s, 1, values);
					if (validinput) {
						maxdepth = (int)values[0];
					}
				}

				// output filename
				else if (cmd == "output") {
					s >> output_file_name;
				}

				// camera lookfromx lookfromy lookfromz lookatx lookaty lookatz upx upy upz fovy
				else if (cmd == "camera") {
					validinput = readvals(s, 10, values);
					if (validinput) {
						Point lookfrom = Point(values[0], values[1], values[2]);
						Point lookat = Point(values[3], values[4], values[5]);
						Vector3 up = Vector3(values[6], values[7], values[8]).normalize();
						float fovy_degrees = values[9];
						float fovy_radians = glm::radians(fovy_degrees);
						camera = { lookfrom, lookat, up, fovy_radians };
					}
				}

				// sphere x y z radius
				else if (cmd == "sphere") {
					validinput = readvals(s, 4, values);
					if (validinput) {
						Point center = Point(values[0], values[1], values[2]);
						float radius = values[3];
						Sphere sphere = Sphere(*diffuse, *specular, shininess, *emission, *ambient,
							transform_stack.top(), center, radius);
						spheres.push_back(sphere);
					}
				}

				// maxverts number
				else if (cmd == "maxverts") {
					validinput = readvals(s, 1, values);
					if (validinput) {
						ignoreCommandWithMessage(cmd, 1, values);
					}
				}

				// maxvertnorms number
				else if (cmd == "maxvertnorms") {
					validinput = readvals(s, 1, values);
					if (validinput) {
						ignoreCommandWithMessage(cmd, 1, values);
					}
				}

				// vertex x y z
				else if (cmd == "vertex") {
					validinput = readvals(s, 3, values);
					if (validinput) {
						Point vertex = Point(values[0], values[1], values[2]);
						vertexes.push_back(vertex);
					}
				}

				// vertexnormal x y z nx ny nz
				else if (cmd == "vertexnormal") {
					validinput = readvals(s, 6, values);
					if (validinput) {
						ignoreCommandWithMessage(cmd, 6, values);
					}
				}

				// tri v1 v2 v3
				else if (cmd == "tri") {
					validinput = readvals(s, 3, values);
					if (validinput && diffuse.get()) {
						// Assumes the vertexes have already been declared and are referenced by indexes into the vertexes list.
						Triangle triangle = Triangle(*diffuse, *specular, shininess, *emission, *ambient, transform_stack.top(),
							vertexes.at((int)values[0]),
							vertexes.at((int)values[1]),
							vertexes.at((int)values[2]));

						triangles.push_back(triangle);
					}
				}

				// trinormal
				else if (cmd == "trinormal") {
					validinput = readvals(s, 3, values);
					if (validinput) {
						ignoreCommandWithMessage(cmd, 3, values);
					}
				}

				// translate x y z
				else if (cmd == "translate") {
					validinput = readvals(s, 3, values);
					if (validinput) {
						transform_stack.translate(values[0], values[1], values[2]);
					}
				}

				// rotate x y z angle
				else if (cmd == "rotate") {
					validinput = readvals(s, 4, values);
					if (validinput) {
						float angle_radians = glm::radians(values[3]);
						Vector3 axis = Vector3(values[0], values[1], values[2]).normalize();
						transform_stack.rotate(angle_radians, axis);
					}
				}

				// scale x y z
				else if (cmd == "scale") {
					validinput = readvals(s, 3, values);
					if (validinput) {
						transform_stack.scale(values[0], values[1], values[2]);
					}
				}

				// pushTransform
				else if (cmd == "pushTransform") {
					transform_stack.pushTransform();
				}

				// popTransform
				else if (cmd == "popTransform") {
					transform_stack.popTransform();
				}

				// directional x y z r g b
				else if (cmd == "directional") {
					validinput = readvals(s, 6, values);
					if (validinput) {
						Vector3 direction = Vector3(values[0], values[1], values[2]).normalize();
						Color color = Color(values[3], values[4], values[5]);
						DirectionalLight directional_light = DirectionalLight(color, direction);
						directional_lights.push_back(directional_light);
					}
				}

				// point x y z r g b
				else if (cmd == "point") {
					validinput = readvals(s, 6, values);
					if (validinput) {
						Point point = Point(values[0], values[1], values[2]);
						Color color = Color(values[3], values[4], values[5]);
						PointLight point_light = PointLight(color, attenuation, point);
						point_lights.push_back(point_light);
					}
				}

				// attenuation const linear quadractic
				else if (cmd == "attenuation") {
					validinput = readvals(s, 3, values);
					if (validinput) {
						attenuation = { values[0], values[1], values[2] };
					}
				}

				// ambient r g b
				else if (cmd == "ambient") {
					validinput = readvals(s, 3, values);
					if (validinput) {
						ambient.reset(new Color(values[0], values[1], values[2]));
					}
				}

				// diffuse r g b
				else if (cmd == "diffuse") {
					validinput = readvals(s, 3, values);
					if (validinput) {
						diffuse.reset(new Color(values[0], values[1], values[2]));
					}
				}

				// specular r g b
				else if (cmd == "specular") {
					validinput = readvals(s, 3, values);
					if (validinput) {
						specular.reset(new Color(values[0], values[1], values[2]));
					}
				}

				// shininess s
				else if (cmd == "shininess") {
					validinput = readvals(s, 1, values);
					if (validinput) {
						shininess = values[0];
					}
				}

				// emission r g b
				else if (cmd == "emission") {
					validinput = readvals(s, 3, values);
					if (validinput) {
						emission.reset(new Color(values[0], values[1], values[2]));
					}
				}

				else {
					cerr << "Unknown Command: " << cmd << " Skipping \n";
				}
			}
			getline(in, str);
		}

		// ============= BEGIN POST-PARSE PROCESSING =================

		// Asserts successful parse of mandatory commands
		if (width == 0) {
			cerr << "Width was not specified.\n";
			throw 2;
		}
		if (height == 0) {
			cerr << "Height was not specified.\n";
			throw 2;
		}

		// Use default file name if none given
		if (output_file_name.size() == 0) {
			output_file_name = DEFAULT_FILE_NAME;
		}

		// Build scene from parsed values
		Scene scene = Scene(width, height, camera, triangles, spheres, directional_lights, point_lights); 

		FileData returnData = { scene, output_file_name, maxdepth };
		return returnData;
	}

	else {
		cerr << "Unable to Open Input Data File " << filename << "\n";
		throw 2;
	}
}