#include "Planta.h"

void Planta::DibujarPlanta(float x, float y, float z, char* ruta) {
	GLMmodel* planta;
	planta = glmReadOBJ(ruta);
	if (!planta)
		exit(0);

	glmUnitize(planta);
	glmFacetNormals(planta);
	glmVertexNormals(planta, 90.0);

	glPushMatrix();
	glTranslatef(x, y, z);
	glmDraw(planta, GLM_SMOOTH | GLM_MATERIAL);
	glPopMatrix();
}
