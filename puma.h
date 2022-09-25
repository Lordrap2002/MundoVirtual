#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glApplication.h"
#include "glutWindow.h"
#include "glsl.h"
#include "glm.h"
#include <FreeImage.h>
#include <string>

using namespace std;

class Puma
{
public:
	Puma() {};
	void DibujarPuma(float x, float y, float z, char* ruta);
};

