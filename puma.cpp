#include "puma.h"

void Puma::DibujarPuma(float x, float y, float z, char* ruta) {
		GLMmodel* puma;
		puma = glmReadOBJ(ruta);
		if (!puma)
			exit(0);

		glmUnitize(puma);
		glmFacetNormals(puma);
		glmVertexNormals(puma, 90.0);

	glPushMatrix();
		glTranslatef(x, y, z);
		glmDraw(puma, GLM_SMOOTH | GLM_MATERIAL);
	glPopMatrix();
}