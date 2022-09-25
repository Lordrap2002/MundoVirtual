#include "Objeto.h"

void Objeto::DibujarObjeto(float x, float y, float z, char* ruta) {
	GLMmodel* obj;
	obj = glmReadOBJ(ruta);
	if (!obj)
		exit(0);

	glmUnitize(obj);
	glmFacetNormals(obj);
	glmVertexNormals(obj, 90.0);

	glPushMatrix();
	glTranslatef(x, y, z);
	glmDraw(obj, GLM_SMOOTH | GLM_MATERIAL);
	glPopMatrix();
}