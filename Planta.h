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

class Planta
{
public:
	Planta() {};
	void DibujarPlanta(float x, float y, float z, char* ruta);
};
