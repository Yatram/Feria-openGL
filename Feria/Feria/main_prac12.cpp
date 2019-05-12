//Semestre 2019 - 1
//************************************************************//
//************************************************************//
//************** Alumno (s): *********************************//
//*************											******//
//*************											******//
//************************************************************//

#include "texture.h"
#include "figuras.h"
#include "Camera.h"

#include "cmodel/CModel.h"
//Solo para Visual Studio 2015 y en adelante
#if (_MSC_VER >= 1900)
#   pragma comment( lib, "legacy_stdio_definitions.lib" )
#endif

//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////
static GLuint ciudad_display_list;	//Display List for the Monito


//NEW// Keyframes
float posX =0, posY = 2.5, posZ =-3.5, rotRodIzq = 0;
float giroMonito = 0;
float movBrazoDer = 0.0;

#define MAX_FRAMES 9
int i_max_steps = 90;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float incX;		//Variable para IncrementoX
	float incY;		//Variable para IncrementoY
	float incZ;		//Variable para IncrementoZ
	float rotRodIzq;
	float rotInc;
	float giroMonito;
	float giroMonitoInc;
	float movBrazoDer;
	float movBrazoDerInc;

	
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex=5;			//introducir datos
bool play=false;
int playIndex=0;


//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////

int w = 500, h = 500;
int frame=0,time,timebase=0;
char s[30];

CCamera objCamera;	//Create objet Camera

GLfloat g_lookupdown = 0.0f;    // Look Position In The Z-Axis (NEW) 

int font=(int)GLUT_BITMAP_HELVETICA_18;

//Otras opciones son:
/*GLUT_BITMAP_8_BY_13
GLUT_BITMAP_9_BY_15
GLUT_BITMAP_TIMES_ROMAN_10
GLUT_BITMAP_TIMES_ROMAN_24
GLUT_BITMAP_HELVETICA_10
GLUT_BITMAP_HELVETICA_12
GLUT_BITMAP_HELVETICA_18*/



//GLfloat Diffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };				// Diffuse Light Values
GLfloat Diffuse[]= { 0.5f, 0.5f, 0.5f, 1.0f };				// Diffuse Light Values
GLfloat Specular[] = { 1.0, 1.0, 1.0, 1.0 };				// Specular Light Values
GLfloat Position[]= { 0.0f, 7.0f, -5.0f, 0.0f };			// Light Position
GLfloat Position2[]= { 0.0f, 0.0f, -5.0f, 1.0f };			// Light Position

GLfloat m_dif1[] = { 0.0f, 0.2f, 1.0f, 1.0f };				// Diffuse Light Values
GLfloat m_spec1[] = { 0.0, 0.0, 0.0, 1.0 };				// Specular Light Values
GLfloat m_amb1[] = { 0.0, 0.0, 0.0, 1.0 };				// Ambiental Light Values
GLfloat m_s1[] = {18};

GLfloat m_dif2[] = { 0.8f, 0.2f, 0.0f, 1.0f };				// Diffuse Light Values
GLfloat m_spec2[] = { 0.0, 0.0, 0.0, 1.0 };				// Specular Light Values
GLfloat m_amb2[] = { 0.0, 0.0, 0.0, 1.0 };				// Ambiental Light Values
GLfloat m_s2[] = {22};

CTexture text1;
CTexture text2;
CTexture text3;	//Flecha
CTexture text4;	//Pavimento
CTexture text5;	//Pasto01
CTexture text6;	//Casa01
CTexture centro;
CTexture text7;//torre roja
CTexture text8;//torre blanca
CTexture text9;//torre azul
CTexture text10;//torre azul
CTexture text11;// estrellas
CTexture text12;//cadenas
CTexture text13;//silla amarilla
CTexture text25;
CTexture sky;
CTexture sky2;
CTexture sky3;

CTexture tree;

CFiguras fig1;
CFiguras fig2;
CFiguras fig3;
CFiguras fig4;	//Pasto01
CFiguras fig5;	//Casa01
CFiguras fig6;
CFiguras fig7;	//Para crear Monito
CFiguras fig8;
CFiguras fig10;
CFiguras fig11;
CFiguras cen;


//Figuras de 3D Studio
CModel quiosco;
CModel banca;
CModel faro;
CModel jardinera;
CModel palomitas;

//Animación del coche
float angRot = 0.0;
float movKitX = 0.0;
float movKitZ = 0.0;
float rotKit = 0.0;
float rotTires = 0.0;
bool g_fanimacion = false;
bool g_avanza = false;

bool circuito = false;
bool recorrido1 = true;
bool recorrido2 = false;
bool recorrido3 = false;
bool recorrido4 = false;
bool recorrido5 = false;


void saveFrame(void)
{

	printf("frameindex %d\n", FrameIndex);

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].giroMonito = giroMonito;
	KeyFrame[FrameIndex].movBrazoDer = movBrazoDer;

	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;
	movBrazoDer = KeyFrame[0].movBrazoDer;

}

void interpolation(void)
{
	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	KeyFrame[playIndex].rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	KeyFrame[playIndex].giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;
	KeyFrame[playIndex].movBrazoDerInc = (KeyFrame[playIndex + 1].movBrazoDer - KeyFrame[playIndex].movBrazoDer) / i_max_steps;

}

void ciudad ()
{
		

		glPushMatrix(); //Camino1
			glTranslatef(37.5,0.0,0.0);
			glScalef(55,0.1,7);
			glDisable(GL_LIGHTING);
			fig3.prisma2(text4.GLindex, 0);
			glEnable(GL_LIGHTING);
		glPopMatrix();

		glPushMatrix(); //Camino2
			glTranslatef(-37.5,0.0,0.0);
			glScalef(55,0.1,7);
			glDisable(GL_LIGHTING);
			fig3.prisma2(text4.GLindex, 0);
			glEnable(GL_LIGHTING);
		glPopMatrix();
		
		glPushMatrix(); //Camino3
		glTranslatef(0.0, 0.0, 36.0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		glScalef(58.0, 0.1, 7);
		glDisable(GL_LIGHTING);
		fig3.prisma2(text4.GLindex, 0);
		glEnable(GL_LIGHTING);
		glPopMatrix();

		glPushMatrix(); //Camino4
		glTranslatef(0.0, 0.0, -36.0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		glScalef(58, 0.1, 7);
		glDisable(GL_LIGHTING);
		fig3.prisma2(text4.GLindex, 0);
		glEnable(GL_LIGHTING);
		glPopMatrix();


		glPushMatrix(); //Pasto
			glTranslatef(-37.5,0.0,-34.0);
			glScalef(55,0.1,61.0);
			glDisable(GL_LIGHTING);
			fig4.prisma2(text5.GLindex, 0);
			glEnable(GL_LIGHTING);
		glPopMatrix();

		glPushMatrix(); //Pasto
		glTranslatef(-37.5, 0.0, 34.0);
		glScalef(55, 0.1, 61.0);
		glDisable(GL_LIGHTING);
		fig4.prisma2(text5.GLindex, 0);
		glEnable(GL_LIGHTING);
		glPopMatrix();

		glPushMatrix(); //Pasto
			glTranslatef(37.5,0.0,34.0);
			glScalef(55,0.1,61.0);
			glDisable(GL_LIGHTING);
			fig4.prisma2(text5.GLindex, 0);
			glEnable(GL_LIGHTING);
		glPopMatrix();

		glPushMatrix(); //Pasto
		glTranslatef(37.5, 0.0, -34.0);
		glScalef(55, 0.1, 61.0);
		glDisable(GL_LIGHTING);
		fig4.prisma2(text5.GLindex, 0);
		glEnable(GL_LIGHTING);
		glPopMatrix();

		glPushMatrix(); //Pasto
		glTranslatef(6.7, 0.0, -36.0);
		glScalef(6.6, 0.1, 58.0);
		glDisable(GL_LIGHTING);
		fig4.prisma2(text5.GLindex, 0);
		glEnable(GL_LIGHTING);
		glPopMatrix();

		glPushMatrix(); //Pasto
		glTranslatef(-6.7, 0.0, -36.0);
		glScalef(6.6, 0.1, 58.0);
		glDisable(GL_LIGHTING);
		fig4.prisma2(text5.GLindex, 0);
		glEnable(GL_LIGHTING);
		glPopMatrix();

		glPushMatrix(); //Pasto
		glTranslatef(6.7, 0.0, 36.0);
		glScalef(6.6, 0.1, 58.0);
		glDisable(GL_LIGHTING);
		fig4.prisma2(text5.GLindex, 0);
		glEnable(GL_LIGHTING);
		glPopMatrix();


		glPushMatrix(); //Pasto
		glTranslatef(-6.7, 0.0, 36.0);
		glScalef(6.6, 0.1, 58.0);
		glDisable(GL_LIGHTING);
		fig4.prisma2(text5.GLindex, 0);
		glEnable(GL_LIGHTING);
		glPopMatrix();
}

void arbol_alpha()
{
	glPushMatrix();
		glDisable(GL_LIGHTING);
		glEnable( GL_ALPHA_TEST );
		glAlphaFunc( GL_GREATER, 0.1 );
		glBindTexture(GL_TEXTURE_2D, tree.GLindex);
		glBegin(GL_QUADS); //plano
			glColor3f(1.0, 1.0, 1.0);
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0, 0.0, 0.0);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0, 0.0, 0.0);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0, 20.0, 0.0);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0, 20.0, 0.0);
		glEnd();
				
	glPopMatrix();

	glPushMatrix();
		glRotatef(45, 0, 1, 0);
		glBegin(GL_QUADS); //plano
			glColor3f(1.0, 1.0, 1.0);
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0, 0.0, 0.0);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0, 0.0, 0.0);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0, 20.0, 0.0);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0, 20.0, 0.0);
		glEnd();
	glPopMatrix();

	glPushMatrix();
		glRotatef(-45, 0, 1, 0);
		glBegin(GL_QUADS); //plano
			glColor3f(1.0, 1.0, 1.0);
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0, 0.0, 0.0);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0, 0.0, 0.0);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0, 20.0, 0.0);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0, 20.0, 0.0);
		glEnd();
	glPopMatrix();

	glPushMatrix();
		glRotatef(90, 0, 1, 0);
		glBegin(GL_QUADS); //plano
			glColor3f(1.0, 1.0, 1.0);
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0, 0.0, 0.0);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0, 0.0, 0.0);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0, 20.0, 0.0);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0, 20.0, 0.0);
		glEnd();
		glDisable( GL_ALPHA_TEST );
		glEnable(GL_LIGHTING);
				
	glPopMatrix();
}

void arbol_blend()
{
	glPushMatrix();
		glDisable(GL_LIGHTING);
		glEnable(GL_BLEND);     // Turn Blending On
		//glDisable(GL_DEPTH_TEST);   // Turn Depth Testing Off
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D, tree.GLindex);
		glBegin(GL_QUADS); //plano
			glColor3f(1.0, 1.0, 1.0);
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0, 0.0, 0.0);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0, 0.0, 0.0);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0, 20.0, 0.0);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0, 20.0, 0.0);
		glEnd();	
	glPopMatrix();

	glPushMatrix();
		glRotatef(45, 0, 1, 0);
		glBegin(GL_QUADS); //plano
			glColor3f(1.0, 1.0, 1.0);
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0, 0.0, 0.0);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0, 0.0, 0.0);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0, 20.0, 0.0);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0, 20.0, 0.0);
		glEnd();	
	glPopMatrix();

	glPushMatrix();
		glRotatef(-45, 0, 1, 0);
		glBegin(GL_QUADS); //plano
			glColor3f(1.0, 1.0, 1.0);
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0, 0.0, 0.0);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0, 0.0, 0.0);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0, 20.0, 0.0);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0, 20.0, 0.0);
		glEnd();				
	glPopMatrix();

	glPushMatrix();
		glRotatef(90, 0, 1, 0);
		glBegin(GL_QUADS); //plano
			glColor3f(1.0, 1.0, 1.0);
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0, 0.0, 0.0);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0, 0.0, 0.0);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0, 20.0, 0.0);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0, 20.0, 0.0);
		glEnd();		
	glPopMatrix();
	glDisable(GL_BLEND);        // Turn Blending Off
	//glEnable(GL_DEPTH_TEST);    // Turn Depth Testing On
	glEnable(GL_LIGHTING);
}

void montana(void) {

	glPushMatrix();

	//rieles horizontales
	glPushMatrix();
	glTranslatef(-25, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-24, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-23, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-22, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-21, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-20, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-19, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-18, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-17, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-16, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-15, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-14, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-13, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-12, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-11, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-10, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-9, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-8, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-7, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-6, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-5, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-4, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-3, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-2, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-1, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(2, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(3, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(4, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(6, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(7, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(8, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(9, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(10, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(11, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(12, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(13, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(14, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(16, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(17, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(18, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(19, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(20, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(21, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(22, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(23, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(24, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(25, 1, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();
	//hacia atras

	glPushMatrix();
	glTranslatef(26.3, 1, -2);

	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(26.3, 1, -3);

	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(26.3, 1, -4);

	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(26.3, 1, -5);

	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(26.3, 1, -6);

	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(26.3, 1, -7);

	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(26.3, 1, -8);

	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(26.3, 1, -9);

	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(26.3, 1, -10);

	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(26.3, 1, -11);

	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(26.3, 1, -12);

	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(26.3, 1, -13);

	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(26.3, 1, -14);

	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(26.3, 1, -15);

	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();
	//hacia atras recto izquierda
	glPushMatrix();
	glTranslatef(25, 1, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(24, 1, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(23, 1, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(22, 1, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(21, 1, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(20, 1, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(19, 1, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(18, 1, -17.56);
	glRotatef(90, 0, 1, 0);

	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	//rieles largos
	glPushMatrix();
	glTranslatef(0, 1, .4);
	glScalef(50, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 1, -.4);
	glScalef(50, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(25.86, 1, -8.8);
	glRotatef(90, 0, 1, 0);
	glScalef(15, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(26.7, 1, -8.8);
	glRotatef(90, 0, 1, 0);
	glScalef(15, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();
	//rieles de atras
	glPushMatrix();
	glTranslatef(21.5, 1, -18);
	glScalef(7, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(21.5, 1, -17.15);
	glScalef(7, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	//rieles de atras hacia arriba
	glPushMatrix();

	glPushMatrix();
	glTranslatef(17.6, 1.3, -18);
	glRotatef(-40, 0, 0, 1);
	glScalef(1, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(17.6, 1.3, -17.15);
	glRotatef(-40, 0, 0, 1);
	glScalef(1, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(12.98, 8.96, -18);
	glRotatef(-60, 0, 0, 1);
	glScalef(17, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(12.98, 8.96, -17.15);
	glRotatef(-60, 0, 0, 1);
	glScalef(17, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(17, 2, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(16, 3.7, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(14.9, 5.7, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(13, 8.9, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(12, 10.7, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(11, 12.4, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, 14.1, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(9, 15.8, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPopMatrix();//hacia arriba fin
				  //descenso

	glPushMatrix();
	glPushMatrix();
	glTranslatef(2, 8.96, -18);
	glRotatef(60, 0, 0, 1);
	glScalef(17, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2, 8.96, -17.15);
	glRotatef(60, 0, 0, 1);
	glScalef(17, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(6.67, 16.6, -17.15);
	glRotatef(35, 0, 0, 1);
	glScalef(1, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);

	glPopMatrix();

	glPushMatrix();
	glTranslatef(6.67, 16.6, -18);
	glRotatef(35, 0, 0, 1);
	glScalef(1, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);

	glPopMatrix();

	glPushMatrix();
	glTranslatef(7.46, 16.9, -17.15);
	glScalef(.8, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);

	glPopMatrix();

	glPushMatrix();
	glTranslatef(7.46, 16.9, -18);
	glScalef(.8, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);

	glPopMatrix();

	glPushMatrix();
	glRotatef(-35, 0, 0, 1);
	glTranslatef(-2.75, 18.4, -17.15);
	glScalef(1.1, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);

	glPopMatrix();

	glPushMatrix();
	glRotatef(-35, 0, 0, 1);
	glTranslatef(-2.75, 18.4, -18);
	glScalef(1.1, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);

	glPopMatrix();

	glPushMatrix();
	glTranslatef(8, 16.8, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(7, 16.8, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(6, 15.8, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5, 14.2, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(4, 12.4, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3, 10.6, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2, 8.9, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1, 7.3, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 5.56, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1, 3.8, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2, 2, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3, 1, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-4, 1, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-5, 1, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-6, 1, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-7, 1, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-8, 1, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-9, 1, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-10, 1, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-11, 1, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-12, 1, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-13, 1, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-14, 1, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-15, 1, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-16, 1, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-17, 1, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-18, 1, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-19, 1, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-20, 1, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-21, 1, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-22, 1, -17.56);
	glRotatef(90, 0, 1, 0);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-23.3, 1, -1);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-23.3, 1, -2);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-23.3, 1, -3);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-23.3, 1, -4);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-23.3, 1, -5);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-23.3, 1, -6);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-23.3, 1, -7);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-23.3, 1, -8);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-23.3, 1, -9);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-23.3, 1, -10);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-23.3, 1, -11);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-23.3, 1, -12);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-23.3, 1, -13);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-23.3, 1, -14);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-23.3, 1, -15);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-23.3, 1, -16);
	glScalef(.9, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPopMatrix();//aqui termina el descenso

				  //rieles atras despues del descenso
	glPushMatrix();

	glPushMatrix();
	glTranslatef(-2.63, 1.3, -18);
	glRotatef(40, 0, 0, 1);
	glScalef(1, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.63, 1.3, -17.15);
	glRotatef(40, 0, 0, 1);
	glScalef(1, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-12.5, 1, -18);
	glScalef(19, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-12.5, 1, -17.15);
	glScalef(19, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();
	glPopMatrix();//recta despues del descenso

	glPushMatrix();//giro despues del descenso
	glTranslatef(3, 0, -17.58);
	glRotatef(180, 0, 1, 0);


	glPushMatrix();
	glTranslatef(25.26, 1, .31);
	glRotatef(20, 0, 1, 0);
	glScalef(.5, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(25.2, 1, -.5);
	glRotatef(20, 0, 1, 0);
	glScalef(.5, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(25.65, 1, .1);
	glRotatef(35, 0, 1, 0);
	glScalef(.5, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(25.6, 1, -.75);
	glRotatef(45, 0, 1, 0);
	glScalef(.5, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(26, 1, -.2);
	glRotatef(50, 0, 1, 0);
	glScalef(.5, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(25.8, 1, -1.11);
	glRotatef(75, 0, 1, 0);
	glScalef(.5, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(26.45, 1, -.86);
	glRotatef(60, 0, 1, 0);
	glScalef(1.1, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();



	glPopMatrix();

	//aqui termine el giro

	//hacia atras despues del giro de descenso

	//rieles de atras

	glPushMatrix();
	glTranslatef(-23.7, 1, -8.4);
	glRotatef(90, 0, 1, 0);
	glScalef(16, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-22.85, 1, -8.4);
	glRotatef(90, 0, 1, 0);
	glScalef(16, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();


	glPushMatrix();//giro
	glPushMatrix();
	glTranslatef(25.26, 1, .31);
	glRotatef(20, 0, 1, 0);
	glScalef(.5, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(25.2, 1, -.5);
	glRotatef(20, 0, 1, 0);
	glScalef(.5, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(25.65, 1, .1);
	glRotatef(35, 0, 1, 0);
	glScalef(.5, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(25.6, 1, -.75);
	glRotatef(45, 0, 1, 0);
	glScalef(.5, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(26, 1, -.2);
	glRotatef(50, 0, 1, 0);
	glScalef(.5, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(25.8, 1, -1.11);
	glRotatef(75, 0, 1, 0);
	glScalef(.5, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(26.45, 1, -.86);
	glRotatef(60, 0, 1, 0);
	glScalef(1.1, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();



	glPopMatrix();

	//aqui termine el giro

	glPushMatrix();//giro
	glTranslatef(26.3, 0, 8.7);
	glRotatef(90, 0, 1, 0);


	glPushMatrix();
	glTranslatef(25.26, 1, .31);
	glRotatef(20, 0, 1, 0);
	glScalef(.5, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(25.2, 1, -.5);
	glRotatef(20, 0, 1, 0);
	glScalef(.5, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(25.65, 1, .1);
	glRotatef(35, 0, 1, 0);
	glScalef(.5, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(25.6, 1, -.75);
	glRotatef(45, 0, 1, 0);
	glScalef(.5, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(26, 1, -.2);
	glRotatef(50, 0, 1, 0);
	glScalef(.5, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(25.8, 1, -1.11);
	glRotatef(75, 0, 1, 0);
	glScalef(.5, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(26.45, 1, -.86);
	glRotatef(60, 0, 1, 0);
	glScalef(1.1, .1, .1);
	fig11.prisma3(text9.GLindex, text9.GLindex);
	glPopMatrix();



	glPopMatrix();

	//aqui termine el giro





	//soportes
	glPushMatrix();
	glTranslatef(0, 0, -.4);
	fig10.cilindro(.1, 1, 100, text8.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, .4);
	fig10.cilindro(.1, 1, 100, text8.GLindex);

	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, 0, -.4);
	fig10.cilindro(.1, 1, 100, text8.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, 0, .4);
	fig10.cilindro(.1, 1, 100, text8.GLindex);

	glPopMatrix();

	glPushMatrix();
	glTranslatef(-10, 0, -.4);
	fig10.cilindro(.1, 1, 100, text8.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-10, 0, .4);
	fig10.cilindro(.1, 1, 100, text8.GLindex);

	glPopMatrix();

	glPushMatrix();
	glTranslatef(23, 0, -.4);
	fig10.cilindro(.1, 1, 100, text8.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(23, 0, .4);
	fig10.cilindro(.1, 1, 100, text8.GLindex);

	glPopMatrix();

	glPushMatrix();
	glTranslatef(-23, 0, -.4);
	fig10.cilindro(.1, 1, 100, text8.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-23, 0, .4);
	fig10.cilindro(.1, 1, 100, text8.GLindex);

	glPopMatrix();

	glPushMatrix();
	glTranslatef(26.6, 0, -2);
	fig10.cilindro(.1, 1, 100, text8.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(26, 0, -2);
	fig10.cilindro(.1, 1, 100, text8.GLindex);

	glPopMatrix();

	glPushMatrix();
	glTranslatef(26.6, 0, -11);
	fig10.cilindro(.1, 1, 100, text8.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(26, 0, -11);
	fig10.cilindro(.1, 1, 100, text8.GLindex);

	glPopMatrix();


	glPushMatrix();
	glTranslatef(16, 0, -18);
	fig10.cilindro(.1, 3.7, 100, text8.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(16, 0, -17.2);
	fig10.cilindro(.1, 3.7, 100, text8.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(14, 0, -18);
	fig10.cilindro(.1, 7, 100, text8.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(14, 0, -17.2);
	fig10.cilindro(.1, 7, 100, text8.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(8, 0, -18);
	fig10.cilindro(.1, 17, 100, text8.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(8, 0, -17.2);
	fig10.cilindro(.1, 17, 100, text8.GLindex);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(4, 0, -18);
	fig10.cilindro(.1, 12, 100, text8.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(4, 0, -17.2);
	fig10.cilindro(.1, 12, 100, text8.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3, 0, -18);
	fig10.cilindro(.1, 1, 100, text8.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3, 0, -17.2);
	fig10.cilindro(.1, 1, 100, text8.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-7, 0, -18);
	fig10.cilindro(.1, 1, 100, text8.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-7, 0, -17.2);
	fig10.cilindro(.1, 1, 100, text8.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-15, 0, -18);
	fig10.cilindro(.1, 1, 100, text8.GLindex);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-15, 0, -17.2);
	fig10.cilindro(.1, 1, 100, text8.GLindex);
	glPopMatrix();

	glPopMatrix();
}

void carrito(void) {

	glPushMatrix();



	glPopMatrix();
	glPushMatrix();
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1);
	fig8.prisma2(text25.GLindex, text7.GLindex);
	glDisable(GL_ALPHA_TEST);
	glPopMatrix();





	glPopMatrix();

}

void monito()
{
	//glNewList(1, GL_COMPILE);
	glPushMatrix();//Pecho
		glScalef(0.5, 0.5, 0.5);
		fig7.prisma(2.0 ,2.0 ,1 ,text2.GLindex);

		glPushMatrix();//Cuello
			glTranslatef(0, 1.0, 0.0);
			fig7.cilindro(0.25, 0.25, 15, 0);
			glPushMatrix();//Cabeza
				glTranslatef(0, 1.0, 0);
				fig7.esfera(0.75, 15, 15, 0);
			glPopMatrix();
		glPopMatrix();

		glPushMatrix(); //Brazo derecho-->
			glTranslatef(1.25, 0.65, 0);
			fig7.esfera(0.5, 12, 12, 0);
			glPushMatrix();
				glTranslatef(0.25, 0, 0);
				glRotatef(movBrazoDer, 0.0, 0.0, 1.0);
				glRotatef(-45, 0, 1, 0); 
				glTranslatef(0.75, 0, 0);
				fig7.prisma(0.7, 1.5, 0.7, 0);
			glPopMatrix();
		glPopMatrix();

		glPushMatrix(); //Brazo izquierdo <--
			glTranslatef(-1.25, 0.65, 0);
			fig7.esfera(0.5, 12, 12, 0);
			glPushMatrix();
				glTranslatef(-0.25, 0, 0);
				glRotatef(45, 0, 1, 0); 
				glRotatef(25, 0, 0, 1);
				glRotatef(25, 1, 0, 0); 
				glTranslatef(-0.75, 0, 0);
				fig7.prisma(0.7, 1.5, 0.7, 0);
			glPopMatrix();
		glPopMatrix();

		glPushMatrix();//Cintura
			glColor3f(0, 0, 1);
			glTranslatef(0, -1.5, 0);
			fig7.prisma(1, 2, 1, 0);

			glPushMatrix(); //Pie Derecho -->
				glTranslatef(0.75, -0.5, 0);
				glRotatef(-15, 1, 0, 0);
				glTranslatef(0, -0.5, 0);
				fig7.prisma(1.0, 0.5, 1, 0);

				glPushMatrix();
					glTranslatef(0, -0.5, 0);
					glRotatef(15, 1, 0, 0);
					glTranslatef(0, -0.75, 0);
					fig7.prisma(1.5, 0.5, 1, 0);

					glPushMatrix();
						glTranslatef(0, -0.75, 0.3);
						fig7.prisma(0.2, 1.2, 1.5, 0);
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();


			glPushMatrix(); //Pie Izquierdo -->
				glTranslatef(-0.75, -0.5, 0);
				glRotatef(-5, 1, 0, 0);
				glTranslatef(0, -0.5, 0);
				fig7.prisma(1.0, 0.5, 1, 0);

				glPushMatrix();
					glTranslatef(0, -0.5, 0);
					glRotatef(15+rotRodIzq, 1, 0, 0);
					glTranslatef(0, -0.75, 0);
					fig7.prisma(1.5, 0.5, 1, 0);

					glPushMatrix();
						glTranslatef(0, -0.75, 0.3);
						fig7.prisma(0.2, 1.2, 1.5, 0);
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();


		glPopMatrix();


		glColor3f(1, 1, 1);
	glPopMatrix();
	//glEndList();
}

GLuint createDL() 
{
	GLuint ciudadDL;
	//GLuint cieloDL;

	// Create the id for the list
	ciudadDL = glGenLists(1);
	// start list
	glNewList(ciudadDL,GL_COMPILE);
	// call the function that contains 
	// the rendering commands
		ciudad();
		//monito();
	// endList
	glEndList();

	return(ciudadDL);
}
			
void InitGL ( GLvoid )     // Inicializamos parametros
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// Negro de fondo	

	glEnable(GL_TEXTURE_2D);

	glShadeModel (GL_SMOOTH);
	//Para construir la figura comentar esto
	glLightfv(GL_LIGHT1, GL_POSITION, Position);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, Diffuse);
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, Position2);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);

	//glEnable ( GL_COLOR_MATERIAL );
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glPolygonMode(GL_BACK, GL_LINE);

	glClearDepth(1.0f);									// Configuramos Depth Buffer
	glEnable(GL_DEPTH_TEST);							// Habilitamos Depth Testing
	glDepthFunc(GL_LEQUAL);								// Tipo de Depth Testing a realizar
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	/* setup blending */
    //glBlendFunc(GL_SRC_ALPHA,GL_ONE);			// Set The Blending Function For Translucency
    //glColor4f(1.0f, 1.0f, 1.0f, 0.5); 
    

	sky.LoadTGA("texp/sky.tga");
	sky.BuildGLTexture();
	sky.ReleaseImage();

	sky2.LoadTGA("texp/sky2.tga");
	sky2.BuildGLTexture();
	sky2.ReleaseImage();

	sky3.LoadTGA("texp/sky3.tga");
	sky3.BuildGLTexture();
	sky3.ReleaseImage();


    text1.LoadBMP("01.bmp");
	text1.BuildGLTexture();
	text1.ReleaseImage();

	text2.LoadBMP("logopumas.bmp");
	//text2.LoadBMP("hulkcara.bmp");
	text2.BuildGLTexture();
	text2.ReleaseImage();

	centro.LoadTGA("texp/centro.tga");
	centro.BuildGLTexture();
	centro.ReleaseImage();

	text3.LoadTGA("city/arrow.tga");
	text3.BuildGLTexture();
	text3.ReleaseImage();

	text4.LoadTGA("texp/adoquin.tga");
	text4.BuildGLTexture();
	text4.ReleaseImage();

	text5.LoadTGA("texp/pasto.tga");
	text5.BuildGLTexture();
	text5.ReleaseImage();

	text6.LoadTGA("city/casa01.tga");
	text6.BuildGLTexture();
	text6.ReleaseImage();

	tree.LoadTGA("Tree.tga");
	tree.BuildGLTexture();
	tree.ReleaseImage();

	text7.LoadTGA("texp/fondorojo.tga");
	text7.BuildGLTexture();
	text7.ReleaseImage();

	text8.LoadTGA("texp/metalblanco.tga");
	text8.BuildGLTexture();
	text8.ReleaseImage();

	text9.LoadTGA("texp/metalazul.tga");
	text9.BuildGLTexture();
	text9.ReleaseImage();

	text25.LoadTGA("texp/trans.tga");
	text25.BuildGLTexture();
	text25.ReleaseImage();

	palomitas._3dsLoad("palomitasC.3ds");	
	//kit.VertexNormals();
	
	banca._3dsLoad("bancasC.3ds");

	jardinera._3dsLoad("jardineraC.3ds");

	quiosco._3dsLoad("quioscoC.3ds");
	quiosco.LoadTextureImages();
	quiosco.GLIniTextures();
	quiosco.ReleaseTextureImages();
	
	objCamera.Position_Camera(0,2.5f,3, 0,2.5f,0, 0, 1, 0);

	//NEW Crear una lista de dibujo
	ciudad_display_list = createDL();

	//NEW Iniciar variables de KeyFrames
	/*for(int i=0; i<MAX_FRAMES; i++)
	{
		KeyFrame[i].posX =0;
		KeyFrame[i].posY =0;
		KeyFrame[i].posZ =0;
		KeyFrame[i].incX =0;
		KeyFrame[i].incY =0;
		KeyFrame[i].incZ =0;
		KeyFrame[i].rotRodIzq =0;
		KeyFrame[i].rotInc =0;
		KeyFrame[i].giroMonito =0;
		KeyFrame[i].giroMonitoInc =0;
		KeyFrame[i].giroBrazo =0;
		KeyFrame[i].giroBrazoInc = 0;

	}*/

	KeyFrame[0].posX = 0;
	KeyFrame[0].posY = 2.5;
	KeyFrame[0].posZ = -3.5;
	KeyFrame[0].rotRodIzq = 0;
	KeyFrame[0].giroMonito = 0;
	KeyFrame[0].movBrazoDer = 0;

	KeyFrame[1].posX = 20;
	KeyFrame[1].posY = 2.5;
	KeyFrame[1].posZ = -3.5;
	KeyFrame[1].rotRodIzq = 60;
	KeyFrame[1].giroMonito = -90.0;
	KeyFrame[1].movBrazoDer = 40;

	KeyFrame[2].posX = 20;
	KeyFrame[2].posY = 2.5;
	KeyFrame[2].posZ = 4.0;
	KeyFrame[2].rotRodIzq = -20.0;
	KeyFrame[2].giroMonito = 0;
	KeyFrame[2].movBrazoDer = 0;

	KeyFrame[3].posX = 20;
	KeyFrame[3].posY = 2.5;
	KeyFrame[3].posZ = 4.0;
	KeyFrame[3].rotRodIzq = 40.0;
	KeyFrame[3].giroMonito = 45;
	KeyFrame[3].movBrazoDer = 90;

	KeyFrame[4].posX = 20;
	KeyFrame[4].posY = 2.5;
	KeyFrame[4].posZ = 4.0;
	KeyFrame[4].rotRodIzq = -40.0;
	KeyFrame[4].giroMonito = -45;
	KeyFrame[4].movBrazoDer = -90;
	//NEW//////////////////NEW//////////////////NEW//////////////////

}

void pintaTexto(float x, float y, float z, void *font,char *string)
{
  
  char *c;     //Almacena los caracteres a escribir
  glRasterPos3f(x, y, z);	//Posicion apartir del centro de la ventana
  //glWindowPos2i(150,100);
  for (c=string; *c != '\0'; c++) //Condicion de fin de cadena
  {
    glutBitmapCharacter(font, *c); //imprime
  }
}

void display ( void )   // Creamos la funcion donde se dibuja
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	
	
	glPushMatrix();

	glRotatef(g_lookupdown,1.0f,0,0);

		gluLookAt(	objCamera.mPos.x,  objCamera.mPos.y,  objCamera.mPos.z,	
					objCamera.mView.x, objCamera.mView.y, objCamera.mView.z,	
					objCamera.mUp.x,   objCamera.mUp.y,   objCamera.mUp.z);
	

		glPushMatrix();		
			glPushMatrix(); //Creamos cielo
				glDisable(GL_LIGHTING);
				glTranslatef(0,60,0);
				fig1.skybox(130.0, 130.0, 130.0,sky.GLindex);
				glEnable(GL_LIGHTING);
			glPopMatrix();

			glPushMatrix();
				glEnable ( GL_COLOR_MATERIAL );
					glColor3f(1, 1, 1);
					//glScalef(0.5, 0.5, 0.5);
					//monito();
					glCallList(ciudad_display_list);
					glTranslatef(posX, posY, posZ);
					glRotatef(giroMonito, 0, 1, 0);
					monito();
				glDisable ( GL_COLOR_MATERIAL );
			glPopMatrix();

			glPushMatrix();
				glTranslatef(-20.0, 0.0, -30.0);
				glRotatef(90.0, 0.0, 1.0, 0.0);
				glScalef(1.0, 2.0, 2.0);
				montana();
			glPopMatrix();

			glPushMatrix(); //quiosco
			glTranslatef(0.0, 2.0, 0.0);
			glRotatef(90.0, 0.0, 1.0, 0.0);
			//glScalef(1.0, 2.0, 2.0);
			quiosco.GLrender(NULL, _SHADED, 1);
			glPopMatrix();

			glPushMatrix();//palomaiz
			glTranslatef(10.0, 0.95, -7.0);
			glRotatef(45.0, 0.0, 1.0, 0.0);
			glRotatef(5.0, 1.0, 0.0, 0.0);
			glScalef(2.0, 2.0, 2.0);
			palomitas.GLrender(NULL, _SHADED, 1);
			glPopMatrix();


			glPushMatrix(); //quiosco
			glTranslatef(-5.0, 0.5, 30.0);
			glRotatef(0.0, 0.0, 1.0, 0.0);
			//glScalef(1.0, 2.0, 2.0);
			jardinera.GLrender(NULL, _SHADED, 1);
			glPopMatrix();

			glPushMatrix(); //quiosco
			glTranslatef(0.0, 2.0, 0.0);
			glRotatef(90.0, 0.0, 1.0, 0.0);
			//glScalef(1.0, 2.0, 2.0);
			faro.GLrender(NULL, _SHADED, 1);
			glPopMatrix();

			glPushMatrix(); //Centro
				glScalef(2,0.1,2);
				glDisable(GL_LIGHTING);
				cen.prisma(0.1,10.0, 7, centro.GLindex);
				glEnable(GL_LIGHTING);
			glPopMatrix();


			glPushMatrix();
				glTranslatef(-30, 0, 10);
				arbol_alpha();
			glPopMatrix();
			glColor3f(1.0,1.0,1.0);
		glPopMatrix(); 

		glPushMatrix();//arbol 2
			glTranslatef(30, 0, -10);
			arbol_alpha();
		glPopMatrix();
	/*		glColor3f(1.0, 1.0, 1.0);
		glPopMatrix();*/

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
			glColor3f(1.0,0.0,0.0);
			pintaTexto(-11,12.0,-14.0,(void *)font,"Practica 12");
			//pintaTexto(-11,10.5,-14,(void *)font,"Listas de Dibujo");
			pintaTexto(-11,8.5,-14,(void *)font,s);
			glColor3f(1.0,1.0,1.0);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);

	glutSwapBuffers ( );

}

void animacion()
{
	fig3.text_izq-= 0.01;
	fig3.text_der-= 0.01;
	if(fig3.text_izq<-1)
		fig3.text_izq=0;
	if(fig3.text_der<0)
		fig3.text_der=1;

	

	//Movimiento del monito
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex>FrameIndex - 2)	//end of total animation?
			{
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += KeyFrame[playIndex].incX;
			posY += KeyFrame[playIndex].incY;
			posZ += KeyFrame[playIndex].incZ;

			rotRodIzq += KeyFrame[playIndex].rotInc;
			giroMonito += KeyFrame[playIndex].giroMonitoInc;
			movBrazoDer += KeyFrame[playIndex].movBrazoDerInc;

			i_curr_steps++;
		}

	}

	/*frame++;
	time=glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf(s,"FPS:%4.2f",frame*1000.0/(time-timebase));
		timebase = time;		
		frame = 0;
	}*/

	glutPostRedisplay();
}

void reshape ( int width , int height )   // Creamos funcion Reshape
{
  if (height==0)										// Prevenir division entre cero
	{
		height=1;
	}

	glViewport(0,0,width,height);	

	glMatrixMode(GL_PROJECTION);						// Seleccionamos Projection Matrix
	glLoadIdentity();

	// Tipo de Vista
	
	glFrustum (-0.1, 0.1,-0.1, 0.1, 0.1, 170.0);

	glMatrixMode(GL_MODELVIEW);							// Seleccionamos Modelview Matrix
	glLoadIdentity();
}

void keyboard ( unsigned char key, int x, int y )  // Create Keyboard Function
{
	switch ( key ) {

		case 'w':   //Movimientos de camara
		case 'W':
			objCamera.Move_Camera( CAMERASPEED+0.2 );
			break;

		case 's':
		case 'S':
			objCamera.Move_Camera(-(CAMERASPEED+0.2));
			break;

		case 'a':
		case 'A':
			objCamera.Strafe_Camera(-(CAMERASPEED+0.4));
			break;

		case 'd':
		case 'D':
			objCamera.Strafe_Camera( CAMERASPEED+0.4 );
			break;

		case 'O':		//  
		case 'o':
				g_fanimacion^= true; //Activamos/desactivamos la animacíon
				circuito = false;
			break;

		case 'i':		//  
		case 'I':
				circuito^= true; //Activamos/desactivamos la animacíon
				g_fanimacion = false;
			break;

		case 'k':		//
		case 'K':
			if (FrameIndex<MAX_FRAMES)
			{
				saveFrame();
			}

			break;

		case 'l':
		case 'L':
			if (play == false && (FrameIndex>1))
			{

				resetElements();
				//First Interpolation				
				interpolation();

				play = true;
				playIndex = 0;
				i_curr_steps = 0;
			}
			else
			{
				play = false;
			}
			break;

		case 'y':						
		case 'Y':
			posZ++;
			//printf("%f \n", posZ);
			break;

		case 'g':						
		case 'G':
			posX--;
			//printf("%f \n", posX);
			break;

		case 'h':						
		case 'H':
			posZ--;
			//printf("%f \n", posZ);
			break;

		case 'j':						
		case 'J':
			posX++;
			//printf("%f \n", posX);
			break;

		case 'b':						
			rotRodIzq++;
			//printf("%f \n", rotRodIzq);
			break;

		case 'B':						
			rotRodIzq--;
			//printf("%f \n", rotRodIzq);
			break;

		case 'p':						
			giroMonito++;
			//printf("%f \n", giroMonito);
			break;

		case 'P':						
			giroMonito--;
			//printf("%f \n", giroMonito);
			break;

		case 27:        // Cuando Esc es presionado...
			exit ( 0 );   // Salimos del programa
			break;        
		default:        // Cualquier otra
			break;
  }

  glutPostRedisplay();
}

void arrow_keys ( int a_keys, int x, int y )  // Funcion para manejo de teclas especiales (arrow keys)
{
  switch ( a_keys ) {
	case GLUT_KEY_PAGE_UP:
		//pos_camY -= 0.5f;
		//pos_cam.y += 0.5f;
		//eye_cam.y += 0.5f;
		objCamera.UpDown_Camera(CAMERASPEED);
		break;

	case GLUT_KEY_PAGE_DOWN:
		//pos_camY += 0.5f;
		//pos_cam.y -= 0.5f;
		//eye_cam.y -= 0.5f;
		objCamera.UpDown_Camera(-CAMERASPEED);
		break;

    case GLUT_KEY_UP:     // Presionamos tecla ARRIBA...
		g_lookupdown -= 1.0f;
		break;

    case GLUT_KEY_DOWN:               // Presionamos tecla ABAJO...
		g_lookupdown += 1.0f;
		break;

	case GLUT_KEY_LEFT:
		objCamera.Rotate_View(-CAMERASPEED);
		break;

	case GLUT_KEY_RIGHT:
		objCamera.Rotate_View( CAMERASPEED);
		break;

    default:
		break;
  }
  glutPostRedisplay();
}

void menuKeyFrame(int id)
{
	switch (id)
	{
	case 0:	//Save KeyFrame
		if (FrameIndex<MAX_FRAMES)
		{
			saveFrame();
		}
		break;

	case 1:	//Play animation
		if (play == false && FrameIndex >1)
		{

			resetElements();
			//First Interpolation
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
		}
		break;


	}
}


void menu(int id)
{

}

int main ( int argc, char** argv )   // Main Function
{
	int submenu;

  glutInit            (&argc, argv); // Inicializamos OpenGL
  glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); // Display Mode (Clores RGB y alpha | Buffer Doble )
  glutInitWindowSize  (500, 500);	// Tamaño de la Ventana
  glutInitWindowPosition (0, 0);	//Posicion de la Ventana
  glutCreateWindow    ("Practica 12"); // Nombre de la Ventana
  //glutFullScreen     ( );         // Full Screen
  InitGL ();						// Parametros iniciales de la aplicacion
  glutDisplayFunc     ( display );  //Indicamos a Glut función de dibujo
  glutReshapeFunc     ( reshape );	//Indicamos a Glut función en caso de cambio de tamano
  glutKeyboardFunc    ( keyboard );	//Indicamos a Glut función de manejo de teclado
  glutSpecialFunc     ( arrow_keys );	//Otras
  glutIdleFunc		  ( animacion );

  submenu = glutCreateMenu(menuKeyFrame);
  glutAddMenuEntry("Guardar KeyFrame", 0);
  glutAddMenuEntry("Reproducir Animacion", 1);
  glutCreateMenu(menu);
  glutAddSubMenu("Animacion Monito", submenu);

  glutAttachMenu(GLUT_RIGHT_BUTTON);

  glutMainLoop        ( );          // 

  return 0;
}
