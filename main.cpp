//=============================================================================
// Sample Application: Lighting (Per Fragment Phong)
//=============================================================================

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glApplication.h"
#include "glutWindow.h"
#include <iostream>
#include "glsl.h"
#include <time.h>
#include "glm.h"
#include "puma.h"
#include "Objeto.h"
#include "Planta.h"
#include <string>
#include <FreeImage.h> //*** Para Textura: Incluir librería

//-----------------------------------------------------------------------------


class myWindow : public cwc::glutWindow
{
protected:
   cwc::glShaderManager SM;
   cwc::glShader *shader;
   cwc::glShader* shader1; //Para Textura: variable para abrir los shader de textura
   GLuint ProgramObject;
   clock_t time0,time1;
   float timer010;  // timer counting 0->1->0
   bool bUp;        // flag if counting up or down.
   GLMmodel* cajas;
   Puma* MiPuma;
   Objeto* MiObjeto;
   Planta* MiPlanta;
   GLMmodel* llama; //*** Para Textura: variable para objeto texturizado
   GLuint texid; //*** Para Textura: variable que almacena el identificador de textura
   bool xI, xD, yU, yD, zF, zB;
   float xP, yP, zP;

public:
	myWindow(){}

	//*** Para Textura: aqui adiciono un método que abre la textura en JPG
	void initialize_textures(void)
	{
		int w, h;
		GLubyte* data = 0;
		//data = glmReadPPM("soccer_ball_diffuse.ppm", &w, &h);
		//std::cout << "Read soccer_ball_diffuse.ppm, width = " << w << ", height = " << h << std::endl;

		//dib1 = loadImage("soccer_ball_diffuse.jpg"); //FreeImage

		glGenTextures(1, &texid);
		glBindTexture(GL_TEXTURE_2D, texid);
		glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		// Loading JPG file
		FIBITMAP* bitmap = FreeImage_Load(
			FreeImage_GetFileType("./Mallas/llamita.jpg", 0),
			"./Mallas/llamita.jpg");  //*** Para Textura: esta es la ruta en donde se encuentra la textura

		FIBITMAP* pImage = FreeImage_ConvertTo32Bits(bitmap);
		int nWidth = FreeImage_GetWidth(pImage);
		int nHeight = FreeImage_GetHeight(pImage);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight,
			0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));

		FreeImage_Unload(pImage);
		//
		glEnable(GL_TEXTURE_2D);
	}

	void moverX() {
		if(xD){
			xP += 0.01;
		}else if(xI){
			xP -= 0.01;
		}
	}

	void moverY() {
		if (yU) {
			yP += 0.01;
		}
		else if (yD) {
			yP -= 0.01;
		}
	}

	void moverZ() {
		if (zB) {
			zP += 0.01;
		}
		else if (zF) {
			zP -= 0.01;
		}
	}

	virtual void OnRender(void)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
      //timer010 = 0.09; //for screenshot!

      glPushMatrix();
	  //glRotatef(timer010 * 360, 0.5, 1.0f, 0.1f);
	  moverX();
	  moverY();
	  moverZ();
	  glTranslatef(xP, yP, zP);
	  glPushMatrix();
		  if (shader) shader->begin();

		  MiPuma->DibujarPuma(15, 0, 1.5,"./Mallas/pumita.obj");
		  MiPlanta->DibujarPlanta(-5, 0, 2, "./Mallas/palma1.obj");
		  MiPlanta->DibujarPlanta(-7, 0, 2, "./Mallas/palma2.obj");
		  MiObjeto->DibujarObjeto(15, 0, 1.5, "./Mallas/jaulita.obj");
		  MiObjeto->DibujarObjeto(-10, 0, 0, "./Mallas/bote.obj");
		  MiObjeto->DibujarObjeto(10, 0, -2, "./Mallas/cajas.obj");
		  MiObjeto->DibujarObjeto(0, -2, 0, "./Mallas/ObjPropio.obj");
		  /*
			  glPushMatrix();
			  glTranslatef(-2.0f, 0.0f, 0.0f);
			  glmDraw(cajas, GLM_SMOOTH | GLM_MATERIAL);
			  glPopMatrix();
			  //glutSolidTeapot(1.0);

			  glPushMatrix();
			  glTranslatef(4.0f, 0.0f, 0.0f);
			  glmDraw(puma, GLM_SMOOTH | GLM_MATERIAL);
			  glPopMatrix();
		  */
		  if (shader) shader->end();

		  //*** Para Textura: llamado al shader para objetos texturizados
		  if (shader1) shader1->begin();

			  glPushMatrix();
			  glTranslatef(7.0f, 0.0f, 15.0f);
			  glBindTexture(GL_TEXTURE_2D, texid);
			  glmDraw(llama, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
			  glPopMatrix();
		  //glutSolidTeapot(1.0);
		  if (shader1) shader1->end();
		 glPopMatrix();


      glutSwapBuffers();
      glPopMatrix();

      UpdateTimer();

		Repaint();
	}

	virtual void OnIdle() {}

	// When OnInit is called, a render context (in this case GLUT-Window) 
	// is already available!
	virtual void OnInit()
	{
		glClearColor(0.5f, 0.5f, 1.0f, 0.0f);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);

		xI = false;
		xD = false;
		xP = 0;
		yU = false;
		yD = false;
		yP = 0;
		zF = false;
		zB = false;
		zP = 0;

		shader = SM.loadfromFile("vertexshader.txt","fragmentshader.txt"); // load (and compile, link) from file
		if (shader==0) 
         std::cout << "Error Loading, compiling or linking shader\n";
      else
      {
         ProgramObject = shader->GetProgramObject();
      }

	 //*** Para Textura: abre los shaders para texturas
		shader1 = SM.loadfromFile("vertexshaderT.txt", "fragmentshaderT.txt"); // load (and compile, link) from file
		if (shader1 == 0)
			std::cout << "Error Loading, compiling or linking shader\n";
		else
		{
			ProgramObject = shader1->GetProgramObject();
		}

      time0 = clock();
      timer010 = 0.0f;
      bUp = true;

	  //Abrir mallas
	  cajas = NULL;

	  if (!cajas)
	  {
		  cajas = glmReadOBJ("./Mallas/cajas.obj");
		  if (!cajas)
			  exit(0);

		  glmUnitize(cajas);
		  glmFacetNormals(cajas);
		  glmVertexNormals(cajas, 90.0);
	  }
	  /*
	  puma = NULL;

	  if (!puma)
	  {
		  puma = glmReadOBJ("./Mallas/puma.obj");
		  if (!puma)
			  exit(0);

		  glmUnitize(puma);
		  glmFacetNormals(puma);
		  glmVertexNormals(puma, 90.0);
	  }
	  */
	  //*** Para Textura: abrir malla de objeto a texturizar
	  llama = NULL;

	  if (!llama)
	  {
		  llama = glmReadOBJ("./Mallas/llamita.obj");
		  if (!llama)
			  exit(0);

		  glmUnitize(llama);
		  glmFacetNormals(llama);
		  glmVertexNormals(llama, 90.0);
	  }
 
	  //*** Para Textura: abrir archivo de textura
	  initialize_textures();
      DemoLight();

	}

	virtual void OnResize(int w, int h)
   {
      if(h == 0) h = 1;
	   float ratio = 1.0f * (float)w / (float)h;

      glMatrixMode(GL_PROJECTION);
	   glLoadIdentity();
	
	   glViewport(0, 0, w, h);

      gluPerspective(45,ratio,1,100);
	   glMatrixMode(GL_MODELVIEW);
	   glLoadIdentity();
	   gluLookAt(0.0f,0.0f,4.0f, 
		          0.0,0.0,-1.0,
			       0.0f,1.0f,0.0f);
   }
	virtual void OnClose(void){}
	virtual void OnMouseDown(int button, int x, int y) {}    
	virtual void OnMouseUp(int button, int x, int y) {}
	virtual void OnMouseWheel(int nWheelNumber, int nDirection, int x, int y){}

	virtual void OnKeyDown(int nKey, char cAscii){   
		switch(cAscii){
			case 27:
				this->Close();
				break;
			case 'a':
				xI = true;
				break;
			case 'd':
				xD = true;
				break;
			case 'w':
				zB = true;
				break;
			case 's':
				zF = true;
				break;
			case 'e':
				yD = true;
				break;
			case 'q':
				yU = true;
				break;
			default:
				break;
		}
	};

	virtual void OnKeyUp(int nKey, char cAscii){
		switch(cAscii){
			case 'a':
				xI = false;
				break;
			case 'd':
				xD = false;
				break;
			case 'w':
				zB = false;
				break;
			case 's':
				zF = false;
				break;
			case 'e':
				yD = false;
				break;
			case 'q':
				yU = false;
				break;
			default:
				break;
		}
      if (cAscii == 's')      // s: Shader
         shader->enable();
      else if (cAscii == 'f') // f: Fixed Function
         shader->disable();
	}

   void UpdateTimer()
   {
      time1 = clock();
      float delta = static_cast<float>(static_cast<double>(time1-time0)/static_cast<double>(CLOCKS_PER_SEC));
      delta = delta / 4;
      if (delta > 0.00005f)
      {
         time0 = clock();
         if (bUp)
         {
            timer010 += delta;
            if (timer010>=1.0f) { timer010 = 1.0f; bUp = false;}
         }
         else
         {
            timer010 -= delta;
            if (timer010<=0.0f) { timer010 = 0.0f; bUp = true;}
         }
      }
   }

   void DemoLight(void)
   {
     glEnable(GL_LIGHTING);
     glEnable(GL_LIGHT0);
     glEnable(GL_NORMALIZE);
     
     // Light model parameters:
     // -------------------------------------------
     
     GLfloat lmKa[] = {0.0, 0.0, 0.0, 0.0 };
     glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);
     
     glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
     glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);
     
     // -------------------------------------------
     // Spotlight Attenuation
     
     GLfloat spot_direction[] = {1.0, -1.0, -1.0 };
     GLint spot_exponent = 30;
     GLint spot_cutoff = 180;
     
     glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
     glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, spot_exponent);
     glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);
    
     GLfloat Kc = 1.0;
     GLfloat Kl = 0.0;
     GLfloat Kq = 0.0;
     
     glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,Kc);
     glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Kl);
     glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Kq);
     
     
     // ------------------------------------------- 
     // Lighting parameters:

     GLfloat light_pos[] = {0.0f, 5.0f, 5.0f, 1.0f};
     GLfloat light_Ka[]  = {1.0f, 0.5f, 0.5f, 1.0f};
     GLfloat light_Kd[]  = {1.0f, 0.1f, 0.1f, 1.0f};
     GLfloat light_Ks[]  = {1.0f, 1.0f, 1.0f, 1.0f};

     glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
     glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
     glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
     glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);

     // -------------------------------------------
     // Material parameters:

     GLfloat material_Ka[] = {0.5f, 0.0f, 0.0f, 1.0f};
     GLfloat material_Kd[] = {0.4f, 0.4f, 0.5f, 1.0f};
     GLfloat material_Ks[] = {0.8f, 0.8f, 0.0f, 1.0f};
     GLfloat material_Ke[] = {0.1f, 0.0f, 0.0f, 0.0f};
     GLfloat material_Se = 20.0f;

     glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_Ka);
     glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_Kd);
     glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_Ks);
     glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_Ke);
     glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material_Se);
   }
};

//-----------------------------------------------------------------------------

class myApplication : public cwc::glApplication
{
public:
	virtual void OnInit() {std::cout << "Hello World!\n"; }
};

//-----------------------------------------------------------------------------

int main(void)
{
	myApplication*  pApp = new myApplication;
	myWindow* myWin = new myWindow();

	pApp->run();
	delete pApp;
	return 0;
}

//-----------------------------------------------------------------------------

