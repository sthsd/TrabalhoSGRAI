#pragma once
#include <stdio.h>
#include <stdlib.h>


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#ifdef _WIN32
#include "GL/glaux.h"
#endif


#include "defines.h"
#include "Globals.h"
#include "Key.h"
#include "Window.h"
#include "Illumination.h"
#include "Timer.h"

#pragma comment (lib, "glaux.lib")    /* link with Win32 GLAUX lib usada para ler bitmaps */

// fun��o para ler jpegs do ficheiro readjpeg.c
extern "C" int read_JPEG_file(const char *, char **, int *, int *, int *);


void strokeCenterString(char *str,double x, double y, double z, double s)
{
	int i,n;

	n = strlen(str);
	glPushMatrix();
	glTranslated(x-glutStrokeLength(GLUT_STROKE_ROMAN,(const unsigned char*)str)*0.5*s,y-119.05*0.5*s,z);
	glScaled(s,s,s);
	for(i=0;i<n;i++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN,(int)str[i]);
	glPopMatrix();

}

/////////////////////////////////////
//Modelo


GLboolean detectaColisao(GLfloat nx,GLfloat nz)
{

	return GL_FALSE;
}

void desenhaPoligono(GLfloat a[], GLfloat b[], GLfloat c[], GLfloat  d[], GLfloat normal[])
{
	glBegin(GL_POLYGON);
	glNormal3fv(normal);
	glVertex3fv(a);
	glVertex3fv(b);
	glVertex3fv(c);
	glVertex3fv(d);
	glEnd();
}


void desenhaCubo()
{
	GLfloat vertices[][3] = { {-0.5,-0.5,-0.5}, 
	{0.5,-0.5,-0.5}, 
	{0.5,0.5,-0.5}, 
	{-0.5,0.5,-0.5}, 
	{-0.5,-0.5,0.5},  
	{0.5,-0.5,0.5}, 
	{0.5,0.5,0.5}, 
	{-0.5,0.5,0.5}};
	GLfloat normais[][3] = {  {0,0,-1}, 
		// acrescentar as outras normais...
	};


	// glBindTexture(GL_TEXTURE_2D, texID);

	desenhaPoligono(vertices[1],vertices[0],vertices[3],vertices[2],normais[0]);
	desenhaPoligono(vertices[2],vertices[3],vertices[7],vertices[6],normais[0]);
	desenhaPoligono(vertices[3],vertices[0],vertices[4],vertices[7],normais[0]);
	desenhaPoligono(vertices[6],vertices[5],vertices[1],vertices[2],normais[0]);
	desenhaPoligono(vertices[4],vertices[5],vertices[6],vertices[7],normais[0]);
	desenhaPoligono(vertices[5],vertices[4],vertices[0],vertices[1],normais[0]);

	//glBindTexture(GL_TEXTURE_2D, NULL);
}




//void desenhaModelo()
//{
//	glColor3f(0,1,0);
//	glutSolidCube(OBJECTO_ALTURA);
//	glPushMatrix();
//	glColor3f(1,0,0);
//	glTranslatef(GRAUS(modelo.objecto.dir),OBJECTO_ALTURA*0.75,0);
//	glRotatef(0,0,1,0);
//	glutSolidCube(OBJECTO_ALTURA*0.5);
//	glPopMatrix();
//}


#define STEP    1

void desenhaChao(GLfloat dimensao, GLuint texID)
{
	// c�digo para desenhar o ch�o
	GLfloat i,j;
	glBindTexture(GL_TEXTURE_2D, texID);

	glColor3f(0.5f,0.5f,0.5f);
	for(i=-dimensao;i<=dimensao;i+=STEP)
		for(j=-dimensao;j<=dimensao;j+=STEP)
		{
			glBegin(GL_POLYGON);
			glNormal3f(0,1,0);
			glTexCoord2f(1,1);
			glVertex3f(i+STEP,0,j+STEP);
			glTexCoord2f(0,1);
			glVertex3f(i,0,j+STEP);
			glTexCoord2f(0,0);
			glVertex3f(i,0,j);
			glTexCoord2f(1,0);
			glVertex3f(i+STEP,0,j);
			glEnd();
		}
		glBindTexture(GL_TEXTURE_2D, NULL);
}

/////////////////////////////////////
//navigateSubwindow

void motionNavigateSubwindow(int x, int y)
{

}


void mouseNavigateSubwindow(int button, int state, int x, int y)
{

}





void imprime_ajuda(void)
{
	printf("\n\nDesenho de um quadrado\n");
	printf("h,H - Ajuda \n");
	printf("******* Diversos ******* \n");
	printf("l,L - Alterna o calculo luz entre Z e eye (GL_LIGHT_MODEL_LOCAL_VIEWER)\n");
	printf("w,W - Wireframe \n");
	printf("f,F - Fill \n");
	printf("******* Movimento ******* \n");
	printf("up  - Acelera \n");
	printf("down- Trava \n");
	printf("left- Vira rodas para a direita\n");
	printf("righ- Vira rodas para a esquerda\n");
	printf("******* Camara ******* \n");
	printf("F1 - Alterna camara da janela da Esquerda \n");
	printf("F2 - Alterna camara da janela da Direita \n");
	printf("PAGE_UP, PAGE_DOWN - Altera abertura da camara \n");
	printf("botao esquerdo + movimento na Janela da Direita altera o olhar \n");
	printf("ESC - Sair\n");
}


////////////////////////////////////
// Inicializa��es



void createDisplayLists(int janelaID)
{
	modelo.labirinto[janelaID]=glGenLists(2);
	glNewList(modelo.labirinto[janelaID], GL_COMPILE);
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT );
	glPopAttrib();
	glEndList();

	modelo.chao[janelaID]=modelo.labirinto[janelaID]+1;
	glNewList(modelo.chao[janelaID], GL_COMPILE);
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT );
	desenhaChao(CHAO_DIMENSAO,modelo.texID[janelaID][ID_TEXTURA_CHAO]);
	glPopAttrib();
	glEndList();
}


///////////////////////////////////
/// Texturas


// S� para windows (usa biblioteca glaux)
#ifdef _WIN32

AUX_RGBImageRec *LoadBMP(char *Filename)				// Loads A Bitmap Image
{
	FILE *File=NULL;									// File Handle

	if (!Filename)										// Make Sure A Filename Was Given
	{
		return NULL;									// If Not Return NULL
	}

	File=fopen(Filename,"r");							// Check To See If The File Exists

	if (File)											// Does The File Exist?
	{
		fclose(File);									// Close The Handle
		return auxDIBImageLoad(Filename);				// Load The Bitmap And Return A Pointer
	}

	return NULL;										// If Load Failed Return NULL
}
#endif

void createTextures(GLuint texID[])
{
	char *image;
	int w, h, bpp;

#ifdef _WIN32
	AUX_RGBImageRec *TextureImage[1];					// Create Storage Space For The Texture

	memset(TextureImage,0,sizeof(void *)*1);           	// Set The Pointer To NULL
#endif

	glGenTextures(NUM_TEXTURAS,texID);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

#ifdef _WIN32
	if (TextureImage[0]=LoadBMP(NOME_TEXTURA_CUBOS))
	{
		// Create MipMapped Texture
		glBindTexture(GL_TEXTURE_2D, texID[ID_TEXTURA_CUBOS]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST );
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);

		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
	}
#else
	if (read_JPEG_file(NOME_TEXTURA_CUBOS, &image, &w, &h, &bpp))
	{
		// Create MipMapped Texture
		glBindTexture(GL_TEXTURE_2D, texID[ID_TEXTURA_CUBOS]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST );
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);

		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, w, h, GL_RGB, GL_UNSIGNED_BYTE, image);
	}
#endif
	else
	{
		printf("Textura %s not Found\n",NOME_TEXTURA_CUBOS);
		exit(0);
	}

	if(	read_JPEG_file(NOME_TEXTURA_CHAO, &image, &w, &h, &bpp))
	{
		glBindTexture(GL_TEXTURE_2D, texID[ID_TEXTURA_CHAO]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST );
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, w, h, GL_RGB, GL_UNSIGNED_BYTE, image);
	}else{
		printf("Textura %s not Found\n",NOME_TEXTURA_CHAO);
		exit(0);
	}
	glBindTexture(GL_TEXTURE_2D, NULL);
}


void init()
{
	GLfloat amb[] = { 0.3f, 0.3f, 0.3f, 1.0f };

	estado.timer=100;	

	estado.camera.eye.x=0;
	estado.camera.eye.y=OBJECTO_ALTURA*2;
	estado.camera.eye.z=0;
	estado.camera.dir_long=0;
	estado.camera.dir_lat=0;
	estado.camera.fov=60;

	estado.localViewer=1;
	estado.vista[JANELA_TOP]=0;
	estado.vista[JANELA_NAVIGATE]=0;

	gordon.objecto.pos.x=0;
	gordon.objecto.pos.y=OBJECTO_ALTURA*.5;
	gordon.objecto.pos.z=0;
	gordon.objecto.dir=0;
	gordon.objecto.vel=OBJECTO_VELOCIDADE;
	gordon.andarFrente=GL_TRUE;
	gordon.saltar=GL_FALSE;

	modelo.xMouse=modelo.yMouse=-1;
	
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE);  // por causa do Scale ao Homer

	if(glutGetWindow()==estado.mainWindow)
		glClearColor(0.8f, 0.8f, 0.8f, 0.0f);
	else
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,amb);
}

/////////////////////////////////////
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowPosition(10, 10);
	glutInitWindowSize(800+GAP*3, 400+GAP*2);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	if ((estado.mainWindow=glutCreateWindow("Labirinto")) == GL_FALSE)
		exit(1);

	imprime_ajuda();

	// Registar callbacks do GLUT da janela principal
	init();
	glutReshapeFunc(reshapeMainWindow);
	glutDisplayFunc(displayMainWindow);

	glutTimerFunc(estado.timer,Timer,0);
	glutKeyboardFunc(Key);
	glutKeyboardUpFunc(KeyUp);
	glutSpecialFunc(SpecialKey);
	glutSpecialUpFunc(SpecialKeyUp);

	// criar a sub window topSubwindow
	estado.topSubwindow=glutCreateSubWindow(estado.mainWindow, GAP, GAP, 400, 400);
	init();
	setLight();
	setMaterial();
	createTextures(modelo.texID[JANELA_TOP]);
	createDisplayLists(JANELA_TOP);

	mdlviewer_init("gordon.mdl", gordon.stdModel[JANELA_TOP] );

	for(int i=0;i<3;i++)
		mdlviewer_init("headcrab.mdl", headCrabs[i].stdModel[JANELA_TOP] );

	glutReshapeFunc(redisplayTopSubwindow);
	glutDisplayFunc(displayTopSubwindow);

	glutTimerFunc(estado.timer,Timer,0);
	glutKeyboardFunc(Key);
	glutSpecialFunc(SpecialKey);
	glutSpecialUpFunc(SpecialKeyUp);


	// criar a sub window navigateSubwindow
	estado.navigateSubwindow=glutCreateSubWindow(estado.mainWindow, 400+GAP, GAP, 400, 800);
	init();
	setLight();
	setMaterial();

	createTextures(modelo.texID[JANELA_NAVIGATE]);
	createDisplayLists(JANELA_NAVIGATE);
	mdlviewer_init("gordon.mdl", gordon.stdModel[JANELA_NAVIGATE] );

	for(int i=0;i<3;i++)
		mdlviewer_init("headcrab.mdl", headCrabs[i].stdModel[JANELA_NAVIGATE] );

	glutReshapeFunc(reshapeNavigateSubwindow);
	glutDisplayFunc(displayNavigateSubwindow);
	glutMouseFunc(mouseNavigateSubwindow);

	glutTimerFunc(estado.timer,Timer,0);
	glutKeyboardFunc(Key);
	glutSpecialFunc(SpecialKey);
	glutSpecialUpFunc(SpecialKeyUp);


	glutMainLoop();
	return 0;

}