//******** PRACTICA VISUALITZACIÓ GRÀFICA INTERACTIVA (Escola Enginyeria - UAB)
//******** Entorn bàsic VS2019 MULTIFINESTRA amb OpenGL 3.x, interfície MFC i primitives GLUT en codi font
//******** Ferran Poveda, Marc Vivet, Carme Julià, Débora Gil, Enric Martí (Setembre 2020)
// escena.cpp : Aqui es on ha d'anar el codi de les funcions que 
//              dibuixin les escenes.
//
//    Versió 2.0:	- Objectes Cub, Esfera, Tetera (primitives libreria GLUT) i Truck (camió americà fet per alumnes)
//
//	  Versió 2.2:	- Objectes Cub, Esfera, Tetera definides en fitxer font glut_geometry amb altres primitives GLUT
//
//	  Versió 2.5:	- Objectes cubRGB i Tie (nau Star Wars fet per alumnes)
//
//	  Versió 2.6:	- Inclusió de texte en BITMAP o caracters gràfics (STROKE)
//

#include "stdafx.h"
#include "material.h"
#include "visualitzacio.h"
#include "escena.h"
#include "fg_font.h"

// dibuixa_EscenaGL: Dibuix de l'escena amb comandes GL
void dibuixa_EscenaGL(char objecte, CColor col_object, bool ref_mat, bool sw_mat[4], 
			bool textur, GLint texturID[NUM_MAX_TEXTURES], bool textur_map,
			int nptsU, CPunt3D PC_u[MAX_PATCH_CORBA], GLfloat pasCS, bool sw_PC)
{
	float altfar = 0;
	GLint npunts = 0, nvertexs = 0;
	int i, j, k;
	GLdouble tras[3]; //Sierpinski Sponge
	CColor color_Mar;
	tras[0] = 0.0;	tras[1] = 0.0; tras[2] = 0.0;

// Parametrització i activació/desactivació de textures
	if (texturID[0] != -1) SetTextureParameters(texturID[0], true, true, textur_map, false);
	if (textur) glEnable(GL_TEXTURE_2D);
		else glDisable(GL_TEXTURE_2D);

	switch (objecte)
	{
// Arc
	case ARC:
		SeleccionaColor(ref_mat, sw_mat, col_object);
		arc(ref_mat, sw_mat);
		// Dibuix geometria Mar
		color_Mar.r = 0.5;	color_Mar.g = 0.4; color_Mar.b = 0.9; color_Mar.a = 1.0;
		SeleccionaColor(ref_mat, sw_mat, color_Mar);
		sea();
		break;

// Dibuix de l'objecte TIE (Nau enemiga Star Wars)
	case TIE:
		tie();
		break;

// Dibuix del Truck
	case TRUCK:
		glDisable(GL_TEXTURE_2D);
		truck(textur, texturID);
		// Dibuix geometria Mar
		color_Mar.r = 0.5;	color_Mar.g = 0.4; color_Mar.b = 0.9; color_Mar.a = 1.0;
		SeleccionaColor(ref_mat, sw_mat, color_Mar);
		sea();
		break;

// Dibuix de l'objecte 3DS
	case OBJ3DS:
		// Definició propietats de reflexió (emissió, ambient, difusa, especular) del material.
		SeleccionaColor(ref_mat, sw_mat, col_object);
		// Dibuix de l'objecte 3DS amb textures (OBJECTE3DST) o sense textures (OBJECTE3DS)
		if (textur) glCallList(OBJECTE3DST);
		else glCallList(OBJECTE3DS);
		break;

// Dibuix de l'objecte OBJ
	case OBJOBJ:
		// Definició propietats de reflexió (emissió, ambient, difusa, especular) del material.
		SeleccionaColor(ref_mat, sw_mat, col_object);
		// Objecte OBJ: Dibuix de l'objecte OBJ amb textures (OBJECTEOBJT) o sense textures (OBJECTEOBJ)
		if (textur) glCallList(OBJECTEOBJT);
		else glCallList(OBJECTEOBJ);
		break;

// Corba Bezier
	case C_BEZIER:
		// Dibuixar Punts de Control
		if (sw_PC)
		{	glColor3f(1.0, 0.0, 0.0);
			npunts=gluSphere_VBO(5.0, 20, 20);
			for (int i = 0; i < nptsU; i++)
			{	glPushMatrix();
				  glTranslatef(PC_u[i].x, PC_u[i].y, PC_u[i].z);
				  //gluSphere(5.0, 20, 20);
				  drawgluSphere(npunts);
				glPopMatrix();
				
			}
			deleteVBO(GLU_SPHERE);
		}
		SeleccionaColor(ref_mat, sw_mat, col_object);
		draw_Bezier_Curve(PC_u, nptsU, pasCS, false);
		break;

// Corba Lemniscata
	case C_LEMNISCATA:
		SeleccionaColor(ref_mat, sw_mat, col_object);
		// Dibuixa Lemniscata 3D en VBO
		draw_Lemniscata3D(800, pasCS*20.0);
		break;

// Corba Spline
	case C_BSPLINE:
		// Dibuixar Punts de Control
		if (sw_PC)
		{	glColor3f(1.0, 0.0, 0.0);
			npunts = gluSphere_VBO(5.0, 20, 20);
			for (int i = 0; i < nptsU; i++)
			{	glPushMatrix();
				  glTranslatef(PC_u[i].x, PC_u[i].y, PC_u[i].z);
				  //glutSolidSphere(5.0, 20, 20);
				  drawgluSphere(npunts);
				glPopMatrix();
				
			}
			deleteVBO(GLU_SPHERE);
		}
		SeleccionaColor(ref_mat, sw_mat, col_object);
		draw_BSpline_Curve(PC_u, nptsU, pasCS);
		break;

// Matriu de Primitives SENSE pre-compilació prèvia en VBO (precompilació, draw i alliberació VBO en una funció)
	case MATRIUP:
		//glColor3f(1.0,1.0,1.0);
		SeleccionaColor(ref_mat, sw_mat, col_object);
		for (i = 0; i < 10; i++)
			for (j = 0; j < 10; j++)
				for (k = 0; k < 10; k++)
				{	glPushMatrix();
					  glTranslated(i * 15.0, j * 15.0, k * 15.0);
					  glScaled(5.0, 5.0, 5.0);
					  glutSolidCube(1.0);
					  //glutSolidTorus(1.0,2.0,20,20);
					  //glutSolidOctahedron();
					  //glutSolidTetrahedron();
					  //glutSolidIcosahedron();
					  //glutSolidRhombicDodecahedron();
					  //glutSolidSierpinskiSponge(3,tras,1.0);
					  //glutSolidTeapot(1.0);
					  //gluCylinder(0.5,0.5,1.0,20,20);
					  //gluSphere(1.0, 20, 20);
					glPopMatrix();
				}
		for (i = 0; i < 10; i++)
			for (j = 0; j < 10; j++)
			{	glPushMatrix();
				  glTranslated(i * 15.0, j * 15.0, 160.0);
				  //glScaled(5.0, 5.0, 5.0);
				  //glutSolidCube(1.0);
				  glutSolidTorus(2.0,3.0,20,20);
				  //glutSolidOctahedron();
				  //glutSolidTetrahedron();
				  //glutSolidIcosahedron();
				  //glutSolidRhombicDodecahedron();
				  //glutSolidSierpinskiSponge(3,tras,1.0);
				  //glutSolidTeapot(1.0);
				  //gluCylinder(0.5,0.5,1.0,20,20);
				  //gluSphere(1.0, 20, 20);
				glPopMatrix();
			}
// Dibuix una esfera
		glPushMatrix();
		  glTranslated(200.0, 200.0, 200.0);
		  glScaled(5.0, 5.0, 5.0);
		  //glutSolidCube(1.0);
		  gluSphere(1.0, 20, 20);
		glPopMatrix();
		break;

// Matriu de Primitives AMB pre-compilació prèvia en VBO 
//	(separació en 3 funcions: *_VBO() per precompilació, draw*() per dibuix i deleteVBO() per alliberar VBO)
	case MATRIUP_VBO:
		//		glColor3f(1.0,1.0,1.0);
		SeleccionaColor(ref_mat, sw_mat, col_object);
		glutSolidCube_VBO(1.0);	// Càrrega de la geometria del Cub al VBO i a placa gràfica
		//glutSolidCubeRGB_VBO(1.0);
		//nvertexs = glutSolidTorus_VBO(1.0,2.0,20,20);
		//glutSolidOctahedron_VBO();
		//glutSolidTetrahedron_VBO();
		//glutSolidIcosahedron_VBO();
		//glutSolidRhombicDodecahedron_VBO();
		//nvertexs = glutSolidSierpinskiSponge_VBO(3,tras,1.0);
		//glutSolidTeapot_VBO(1.0);
		//nvertexs = gluCylinder_VBO(0.5,0.5,1.0,20,20);
		//nvertexs = gluSphere_VBO(1.0, 20, 20);
		for (i = 0; i < 10; i++)
			for (j = 0; j < 10; j++)
				for (k = 0; k < 10; k++)
				{	glPushMatrix();
					  glTranslated(i * 15.0, j * 15.0, k * 15.0);
					  glScaled(5.0, 5.0, 5.0);
					  //glutSolidCube(1.0);
					  drawSolidCube();	// Dibuix del cub dins VBO.
					  //drawSolidCubeRGB();
					  //drawSolidTorus(nvertexs);
					  //drawSolidOctahedron();
					  //drawSolidTetrahedron();
					  //drawSolidIcosahedron();
					  //drawSolidRhombicDodecahedron();
					  //drawSolidSierpinskiSponge(nvertexs);
					  //glutSolidSierpinskiSponge(3, tras, 1.0);
					  //drawSolidTeapot(); 
					  //drawgluSphere(nvertexs);
					glPopMatrix();
				}
		deleteVBO(GLUT_CUBE);	// Eliminació del VBO cub.
		nvertexs=glutSolidTorus_VBO(2.0,3.0, 20,20);
		for (i = 0; i < 10; i++)
			for (j = 0; j < 10; j++)
			{	glPushMatrix();
				  glTranslated(i * 15.0, j * 15.0, 160.0);
				  //glScaled(5.0, 5.0, 5.0);
				  //drawSolidCube();	// Dibuix del cub dins VBO.
				  //drawSolidCubeRGB();
				  drawSolidTorus(nvertexs);
				  //drawSolidOctahedron();
				  //drawSolidTetrahedron();
				  //drawSolidIcosahedron();
				  //drawSolidRhombicDodecahedron();
				  //glutSierpinskiSponge(nvertexs);
				  //drawSolidTeapot(); // glutSolidTeapot(1.0);
				  //drawgluSphere(nvertexs);
				glPopMatrix();
			}
		//deleteVBO(GLUT_CUBE);		// Eliminació del VBO cub.
		//deleteVBO(GLUT_CUBERGB);
		deleteVBO(GLUT_TORUS);
		//deleteVBO(GLUT_OCTAHEDRON);
		//deleteVBO(GLUT_TETRAHEDRON);
		//deleteVBO(GLUT_ICOSAHEDRON);
		//deleteVBO(GLUT_RHOMBICDODECAHEDRON);
		//deleteVBO(GLUT_SIERPINSKISPONGE);
		//deleteVBO(GLUT_TEAPOT);
		//deleteVBO(GLU_SPHERE);
// Dibuix una esfera
		glPushMatrix();
		  glTranslated(200.0, 200.0, 200.0);
		  glScaled(5.0, 5.0, 5.0);
		  //glutSolidCube(1.0);
		  gluSphere(1.0, 20, 20);
		glPopMatrix();
		break;

// Dibuix de la resta d'objectes
	default:
		// Definició propietats de reflexió (emissió, ambient, difusa, especular) del material.
		SeleccionaColor(ref_mat, sw_mat, col_object);
		dibuixa(objecte);
		break;
	}

// Enviar les comandes gràfiques a pantalla
//	glFlush();
}


// dibuixa: Funció que dibuixa objectes simples de la llibreria GLUT segons obj
void dibuixa(char obj)
{
	CString String;
	char* string;
	switch(obj)
	{

// Cub
	case CUB:
// Dibuix un cub
		glPushMatrix();
		  glScaled(5.0,5.0,5.0);
		  glutSolidCube(1.0);
		glPopMatrix();
		break;

// Esfera
	case ESFERA:
		glPushMatrix();
		  glScalef(5.0f,5.0f,5.0f);
		  gluSphere(1.0, 40, 40);
		glPopMatrix();
		break;

// Tetera
	case TETERA:
		glPushMatrix();
		  //glScalef(5.0f,5.0f,5.0f);
		  glutSolidTeapot(1.0);
		glPopMatrix();
		break;

// Texte Bitmap
	case TEXTE_BITMAP:
		glPushMatrix();
		  String = "Visualització Gràfica Interactiva-Times Roman 24";
		  string = CString2Char(String);
		  PrintBitmapText(0.0,0.0, 00.0, GLUT_BITMAP_TIMES_ROMAN_24, string);

		  String = "Visualització Gràfica Interactiva-Times Roman 10";
		  string = CString2Char(String);
		  PrintBitmapText(0.0, 0.0, 5.0, GLUT_BITMAP_TIMES_ROMAN_10, string);
		
		  String = "Visualització Gràfica Interactiva-Helvetica 18";
		  string = CString2Char(String);
		  PrintBitmapText(0.0, 0.0, 10.0, GLUT_BITMAP_HELVETICA_18, string);
		
		  String = "Visualització Gràfica Interactiva-Helvetica 12";
		  string = CString2Char(String);
		  PrintBitmapText(0.0, 0.0, 15.0, GLUT_BITMAP_HELVETICA_12, string);

		  String = "Visualització Gràfica Interactiva-Helvetica 10";
		  string = CString2Char(String);
		  PrintBitmapText(0.0, 0.0, 20.0, GLUT_BITMAP_HELVETICA_10, string);

		  String = "Visualització Gràfica Interactiva-9 by 15";
		  string = CString2Char(String);
		  PrintBitmapText(0.0, 0.0, 25.0, GLUT_BITMAP_9_BY_15, string);

		  String = "Visualitzacio Grafica Interactiva-8 by 13"; // Tipus sense accents
		  string = CString2Char(String);
		  PrintBitmapText(0.0, 0.0, 30.0, GLUT_BITMAP_8_BY_13, string);
		glPopMatrix();
		break;

// Texte STROKE
	case TEXTE_STROKE:
		glPushMatrix();
		  String = "Visualització Gràfica Interactiva-Roman";
		  string = CString2Char(String);
		  PrintStrokeText(0.0, 0.0, 160.0, GLUT_STROKE_ROMAN, string);
		glPopMatrix();
		break;

	}
}


// OBJECTE ARC
void arc(bool ref_mat, bool sw_mat[4])
{	CColor col_object;

// Definir VBO del Cub
	glutSolidCube_VBO(1.0);

// Pota Esquerra
	glPushMatrix();
	  glTranslated(0.0, 0.0, 2.5);
	  glScaled(1.0, 1.0, 5.0);
	  drawSolidCube();	//glutSolidCube(1.0);
	glPopMatrix();

// Pota Dreta
	glPushMatrix();
	  glTranslated(0.0, 5.0, 2.5);
	  glScaled(1.0, 1.0, 5.0);
	  drawSolidCube();	//glutSolidCube(1.0);	
	glPopMatrix();

// Travesser
	glPushMatrix();  
	  glTranslated(0.0, 2.5, 5.5);
	  glRotated(90.0, 1.0, 0.0, 0.0);
	  glScaled(1.0, 1.0, 6.0);
	  drawSolidCube();	//glutSolidCube(1.0);
	glPopMatrix();

// Eliminar VBO del Cub
	deleteVBO(GLUT_CUBE);

// Esfera
	col_object.r = 0.0;		col_object.g = 1.0;		col_object.b = 1.0;		col_object.a = 1.0;	// Color blau clar
	SeleccionaColor(ref_mat, sw_mat, col_object);
	glPushMatrix();
	  glTranslated(0.0, 5.0, 6.5);
	  gluSphere(0.5, 20,20);
	glPopMatrix();

// Tetera
	col_object.r = 1.0;		col_object.g = 1.0;		col_object.b = 0.0;		col_object.a = 1.0;	// Color groc
	SeleccionaColor(ref_mat, sw_mat, col_object);
	glPushMatrix();
	  glTranslated(0.0, 0.0, 6.0);
	  glRotated(-90.0, 0.0, 0.0, 1.0);
	  glRotated(90.0, 1.0, 0.0, 0.0);
	  glScaled(0.25, 0.25, 0.25);
	  glutSolidTeapot(1.0);
	glPopMatrix();
}

// OBJECTE Truck amb imatges textura si són actives
// Truck: Paràmetres:
//    - textu: Flag de textures activades o no.
//    - VTextu: Vector d'imatges textura.
void truck(bool textu,GLint VTextu[NUM_MAX_TEXTURES])
{
// VBO
	GLuint vboId = 0;
	std::vector <double> vertices, normals, colors, textures;	// Definició vectors dinàmics per a vertexs, normals i textures 
	vertices.resize(0);		normals.resize(0);		colors.resize(0);	textures.resize(0);			// Reinicialitzar vectors

	glPushMatrix();
	  glTranslatef(0.0f,0.0f,20.0f);

// MOTOR
// Textura carrosseria
	  if (textu)  
		{	GLfloat sPlane[4] = { 0.05f, 0.0f, 0.00f, 0.250f };
		  	GLfloat tPlane[4] = { 0.00f, 0.05f, 0.1f, 0.00f };
// Activar textura planxa
			glBindTexture(GL_TEXTURE_2D,VTextu[5]) ;

			glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
			glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);
			
			glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			glEnable(GL_TEXTURE_2D);
		}

// Color carrosseria sense textura
	glColor4f(0.5f,0.7f,1.0f,0.5f); 

	glPushMatrix();		
		glScalef(2.0f, 4.4f, 2.0f);
		glutSolidCube(10.0f);
	glPopMatrix();

// CABINA
// Cabina inferior
	glPushMatrix();
		glTranslatef(0.0f,32.0f,0.0f);
		glScalef(4.0f, 2.0f, 2.0f);
		glutSolidCube(10.0f);
	glPopMatrix();

// Cabina superior
	glPushMatrix();
		glTranslatef(0.0f,32.0f,15.0f);
		glScalef(2.0f, 2.0f, 1.0f);
		glutSolidCube(10.0f);
	glPopMatrix();

// Cabina trasera
	glPushMatrix();
		glTranslatef(0.0f,49.5f,12.5f);
		glScalef(4.0f, 1.5f, 4.5f);
		glutSolidCube(10.0f);
	glPopMatrix();

// CANTONADES
// Cantonada esquerra
	glPushMatrix() ;
	  glTranslatef(10.0f,22.0f,-10.0f);

 // Obtenir color actual definit en OpenGL amb glColor();
	  GLdouble cColor[4];
	  glGetDoublev(GL_CURRENT_COLOR, cColor);

  	  //glBegin(GL_QUADS);  // Cara de 4 costats
	 	//glNormal3d(200.0, 200.0, 0.0);
		//glVertex3f(10.0f ,  0.0f , 0.0f ); //P1
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); 	// Vector Colors
		normals.push_back(200.0);		normals.push_back(200.0);		normals.push_back(0.0);			// Vector Normals
		textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
		vertices.push_back(10.0);		vertices.push_back(0.0);		vertices.push_back(0.0);		// Vector Vertices

		//glVertex3f(10.0f , 0.0f , 20.0f ); //P2
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
		normals.push_back(200.0);		normals.push_back(200.0);		normals.push_back(0.0);			// Vector Normals
		textures.push_back(0.0);		textures.push_back(1.0);										// Vector Textures
		vertices.push_back(10.0);		vertices.push_back(0.0);		vertices.push_back(20.0);		// Vector Vertices

		//glVertex3f(0.0f , -10.0f ,20.0f ); //P3
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
		normals.push_back(200.0);		normals.push_back(200.0);		normals.push_back(0.0);			// Vector Normals
		textures.push_back(1.0);		textures.push_back(1.0);										// Vector Textures
		vertices.push_back(0.0);		vertices.push_back(-10.0);			vertices.push_back(20.0);	// Vector Vertices

		//glVertex3f(0.0f ,  -10.0f ,0.0f ); //P4
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
		normals.push_back(200.0);		normals.push_back(200.0);		normals.push_back(0.0);			// Vector Normals
		textures.push_back(1.0);		textures.push_back(0.0);										// Vector Textures
		vertices.push_back(0.0);		vertices.push_back(-10.0);		vertices.push_back(0.0);		// Vector Vertices
	  
		//glEnd();
// ----------------------- VBO
		std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.
		draw_GL_TRIANGLE_FAN_VBO(vertices, normals, colors, textures, nv / 3);
		vertices.resize(0);	normals.resize(0);	colors.resize(0);	textures.resize(0);	// Reinicialitzar vectors

	  //glBegin(GL_POLYGON);  // Triangle d'abaix
		//glNormal3f(0.0f, 0.0f, -200.0f);
		//glVertex3f(0.0f ,  0.0f , 0.0f );  // T1
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
		normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-200.0);		// Vector Normals
		textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
		vertices.push_back(0.0);		vertices.push_back(0.0);		vertices.push_back(0.0);		// Vector Vertices

		//glVertex3f(10.0f ,  0.0f ,0.0f );  // P1
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
		normals.push_back(200.0);		normals.push_back(200.0);		normals.push_back(0.0);			// Vector Normals
		textures.push_back(0.0);		textures.push_back(1.0);										// Vector Textures
		vertices.push_back(10.0);		vertices.push_back(0.0);		vertices.push_back(0.0);		// Vector Vertices

		//glVertex3f(0.0f , -10.0f , 0.0f ); // P4
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
		normals.push_back(200.0);		normals.push_back(200.0);		normals.push_back(0.0);			// Vector Normals
		textures.push_back(1.0);		textures.push_back(0.0);										// Vector Textures
		vertices.push_back(0.0);		vertices.push_back(-10.0);		vertices.push_back(0.0);		// Vector Vertices
	  //glEnd();
// ----------------------- VBO
		nv = vertices.size();	// Tamany del vector vertices en elements.
		draw_GL_POLYGON_VBO(vertices, normals, colors, textures, nv / 3);
		vertices.resize(0);	normals.resize(0);	colors.resize(0);	textures.resize(0);	// Reinicialitzar vectors
	  
	  //glBegin(GL_POLYGON);  // Triangle de dalt
		//glNormal3f(0.0f, 0.0f, 200.0f);
		//glVertex3f(0.0f ,  0.0f , 20.0f );	// T2
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
		normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
		textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
		vertices.push_back(0.0);		vertices.push_back(0.0);		vertices.push_back(20.0);		// Vector Vertices

	    //glVertex3f(0.0f , -10.0f , 20.0f ); // P3
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
		normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
		textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
		vertices.push_back(0.0);		vertices.push_back(-10.0);		vertices.push_back(20.0);		// Vector Vertices

		//glVertex3f(10.0f ,  0.0f, 20.0f );  // P2
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
		normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
		textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
		vertices.push_back(10.0);		vertices.push_back(0.0);		vertices.push_back(20.0);		// Vector Vertices
	  //glEnd();
// ----------------------- VBO
		nv = vertices.size();	// Tamany del vector vertices en elements.
		draw_GL_POLYGON_VBO(vertices, normals, colors, textures, nv / 3);
		vertices.resize(0);	normals.resize(0);	colors.resize(0);	textures.resize(0);	// Reinicialitzar vectors

	glPopMatrix();

// Cantonada dreta
	glPushMatrix() ;
	  glTranslatef(-10.0f,22.0f,-10.0f);
	  glRotatef(-90.0f,0.0f,0.0f,1.0f);

  	  //glBegin(GL_QUADS);  // Cara de 4 costats
	 	//glNormal3f(200.0f, 200.0f, 0.0f);
		//glVertex3f(10.0f ,  0.0f , 0.0f ); //P1
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
		normals.push_back(200.0);		normals.push_back(200.0);		normals.push_back(0.0);			// Vector Normals
		textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
		vertices.push_back(10.0);		vertices.push_back(0.0);		vertices.push_back(0.0);		// Vector Vertices

		//glVertex3f(10.0f , 0.0f , 20.0f ); //P2
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
		normals.push_back(200.0);		normals.push_back(200.0);		normals.push_back(0.0);			// Vector Normals
		textures.push_back(0.0);		textures.push_back(1.0);										// Vector Textures
		vertices.push_back(10.0);		vertices.push_back(0.0);		vertices.push_back(20.0);		// Vector Vertices

		//glVertex3f(0.0f , -10.0f ,20.0f ); //P3
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
		normals.push_back(200.0);		normals.push_back(200.0);		normals.push_back(0.0);			// Vector Normals
		textures.push_back(1.0);		textures.push_back(1.0);										// Vector Textures
		vertices.push_back(0.0);		vertices.push_back(-10.0);		vertices.push_back(20.0);		// Vector Vertices
		
		//glVertex3f(0.0f ,  -10.0f ,0.0f ); //P4
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
		normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
		textures.push_back(1.0);		textures.push_back(0.0);										// Vector Textures
		vertices.push_back(0.0);		vertices.push_back(-10.0);		vertices.push_back(0.0);		// Vector Vertices
	  //glEnd();
// ----------------------- VBO
		nv = vertices.size();	// Tamany del vector vertices en elements.
		draw_GL_QUADS_VBO(vertices, normals, colors, textures, nv / 3);
		vertices.resize(0);	normals.resize(0);	colors.resize(0);	textures.resize(0);	// Reinicialitzar vectors

	  //glBegin(GL_POLYGON);  // Triangle d'abaix
		//glNormal3f(0.0f, 0.0f, -200.0f);
		//glVertex3f(0.0f ,  0.0f , 0.0f );  // T1
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
		normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-200.0);		// Vector Normals
		textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
		vertices.push_back(0.0);		vertices.push_back(0.0);		vertices.push_back(0.0);		// Vector Vertices
		
		//glVertex3f(10.0f ,  0.0f ,0.0f );  // P1
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
		normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-200.0);		// Vector Normals
		textures.push_back(1.0);		textures.push_back(0.0);										// Vector Textures
		vertices.push_back(10.0);		vertices.push_back(0.0);		vertices.push_back(0.0);		// Vector Vertices

		//glVertex3f(0.0f , -10.0f , 0.0f ); // P4
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
		normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-200.0);		// Vector Normals
		textures.push_back(0.0);		textures.push_back(1.0);										// Vector Textures
		vertices.push_back(0.0);		vertices.push_back(-10.0);		vertices.push_back(0.0);		// Vector Vertices
	  //glEnd();
// ----------------------- VBO
		nv = vertices.size();	// Tamany del vector vertices en elements.
		draw_GL_POLYGON_VBO(vertices, normals, colors, textures, nv / 3);
		vertices.resize(0);	normals.resize(0);	colors.resize(0);	textures.resize(0);	// Reinicialitzar vectors

	  //glBegin(GL_POLYGON);  // Triangle de dalt
		//glNormal3f(0.0f, 0.0f, 200.0f);
		//glVertex3f(0.0f ,  0.0f , 20.0f );	// T2
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
		normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
		textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
		vertices.push_back(0.0);		vertices.push_back(0.0);		vertices.push_back(20.0);		// Vector Vertices

		//glVertex3f(0.0f , -10.0f , 20.0f ); // P3
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
		normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
		textures.push_back(1.0);		textures.push_back(0.0);										// Vector Textures
		vertices.push_back(0.0);		vertices.push_back(-10.0);		vertices.push_back(20.0);		// Vector Vertices

		//glVertex3f(10.0f ,  0.0f, 20.0f );  // P2
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
		normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
		textures.push_back(1.0);		textures.push_back(1.0);										// Vector Textures
		vertices.push_back(10.0);		vertices.push_back(0.0);		vertices.push_back(20.0);		// Vector Vertices

	  //glEnd();
// ----------------------- VBO
		nv = vertices.size();	// Tamany del vector vertices en elements.
		draw_GL_POLYGON_VBO(vertices, normals, colors, textures, nv / 3);
		vertices.resize(0);	normals.resize(0);	colors.resize(0);	textures.resize(0);	// Reinicialitzar vectors

	glPopMatrix();


// PROTECCIONS DE RODA
// Textura Proteccions de roda (metall)
//	if (textu)
//		{				
//			glBindTexture(GL_TEXTURE_2D,VTextu[5]) ;
//			glEnable(GL_TEXTURE_2D) ;
//			glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
//			glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
//			glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
//			glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
//			}

// Color proteccions roda sense textura
	glColor4d(0.5,0.7,1.0,0.5);

// Protecció de roda esquerra
	glPushMatrix();		
		glTranslated(10.0, 0.0, 3.0);

// Obtenir color actual definit en OpenGL amb glColor();
		glGetDoublev(GL_CURRENT_COLOR, cColor);

		//glBegin(GL_QUADS);
		//  glNormal3f(200.0f, 200.0f, 0.0f);
			//glVertex3f(7.0f , -5.0f , 0.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(7.0);		vertices.push_back(-5.0);		vertices.push_back(0.0);		// Vector Vertices
			//glVertex3f(7.0f , 5.0f , 0.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(1.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(7.0);		vertices.push_back(5.0);		vertices.push_back(0.0);		// Vector Vertices
			//glVertex3f(0.0f , 5.0f , 0.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(1.0);		textures.push_back(1.0);										// Vector Textures
			vertices.push_back(0.0);		vertices.push_back(5.0);		vertices.push_back(0.0);		// Vector Vertices
			//glVertex3f(0.0f , -5.0f , 0.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(0.0);		textures.push_back(1.0);										// Vector Textures
			vertices.push_back(0.0);		vertices.push_back(-5.0);		vertices.push_back(0.0);		// Vector Vertices

			//glVertex3f(7.0f , -12.0f , -8.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(7.0);		vertices.push_back(-12.0);		vertices.push_back(-8.0);		// Vector Vertices
			//glVertex3f(7.0f , -5.0f , 0.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(1.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(7.0);		vertices.push_back(-5.0);		vertices.push_back(0.0);		// Vector Vertices
			//glVertex3f(0.0f , -5.0f , 0.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(1.0);		textures.push_back(1.0);										// Vector Textures
			vertices.push_back(0.0);		vertices.push_back(-5.0);		vertices.push_back(0.0);		// Vector Vertices
			//glVertex3f(0.0f , -12.0f , -8.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	// Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(0.0);		textures.push_back(1.0);										// Vector Textures
			vertices.push_back(0.0);		vertices.push_back(-12.0);		vertices.push_back(-8.0);		// Vector Vertices

			//glVertex3f(0.0f , 15.0f , -10.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(0.0);		vertices.push_back(15.0);		vertices.push_back(-10.0);		// Vector Vertices
			//glVertex3f(0.0f , 5.0f , 0.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(1.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(0.0);		vertices.push_back(5.0);		vertices.push_back(0.0);		// Vector Vertices
			//glVertex3f(7.0f , 5.0f , 0.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(1.0);		textures.push_back(1.0);										// Vector Textures
			vertices.push_back(7.0);		vertices.push_back(5.0);		vertices.push_back(0.0);		// Vector Vertices
			//glVertex3f(7.0f , 15.0f , -10.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(0.0);		textures.push_back(1.0);										// Vector Textures
			vertices.push_back(7.0);		vertices.push_back(15.0);		vertices.push_back(-10.0);		// Vector Vertices

			//glVertex3f(7.5f , -5.0f , -3.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(7.5);		vertices.push_back(-5.0);		vertices.push_back(-3.0);		// Vector Vertices
			//glVertex3f(7.5f , 5.0f , -3.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(1.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(7.5);		vertices.push_back(5.0);		vertices.push_back(-3.0);		// Vector Vertices
			//glVertex3f(7.0f , 5.0f , 0.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(1.0);		textures.push_back(1.0);										// Vector Textures
			vertices.push_back(7.0);		vertices.push_back(5.0);		vertices.push_back(0.0);		// Vector Vertices
			//glVertex3f(7.0f , -5.0f , 0.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(0.0);		textures.push_back(1.0);										// Vector Textures
			vertices.push_back(7.0);		vertices.push_back(-5.0);		vertices.push_back(0.0);		// Vector Vertices
				
			//glVertex3f(7.5f , -12.0f , -8.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(7.5);		vertices.push_back(-12.0);		vertices.push_back(-8.0);		// Vector Vertices
			//glVertex3f(7.5f , -5.0f , -3.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(1.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(7.5);		vertices.push_back(-5.0);		vertices.push_back(-3.0);		// Vector Vertices
			//glVertex3f(7.0f , -5.0f , 0.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(1.0);		textures.push_back(1.0);										// Vector Textures
			vertices.push_back(7.0);		vertices.push_back(-5.0);		vertices.push_back(0.0);		// Vector Vertices
			//glVertex3f(7.0f , -12.0f , -8.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(0.0);		textures.push_back(1.0);										// Vector Textures
			vertices.push_back(7.0);		vertices.push_back(-12.0);		vertices.push_back(-8.0);		// Vector Vertices
				
			//glVertex3f(7.0f , 15.0f , -10.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(7.0);		vertices.push_back(15.0);		vertices.push_back(-10.0);		// Vector Vertices
			//glVertex3f(7.0f , 5.0f , 0.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(1.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(7.0);		vertices.push_back(5.0);		vertices.push_back(0.0);		// Vector Vertices
			//glVertex3f(7.5f , 5.0f , -3.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(7.5);		vertices.push_back(5.0);		vertices.push_back(-3.0);		// Vector Vertices
			//glVertex3f(7.5f , 15.0f , -10.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(7.5);		vertices.push_back(15.0);		vertices.push_back(-10.0);		// Vector Vertices

		//glEnd();
// ----------------------- VBO
		nv = vertices.size();	// Tamany del vector vertices en elements.
		draw_GL_QUADS_VBO(vertices, normals, colors, textures, nv / 3);
		vertices.resize(0);	normals.resize(0);	colors.resize(0);	textures.resize(0);	// Reinicialitzar vectors
	glPopMatrix();

// Protecció de roda dreta
	glPushMatrix();		
		glTranslatef(-16.0f, 0.0f, 3.0f);
		//glBegin(GL_QUADS);
		//  glNormal3f(200.0f, 200.0f, 0.0f);
			//glVertex3f(7.0f , -5.0f , 0.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(7.0);		vertices.push_back(-5.0);		vertices.push_back(0.0);		// Vector Vertices
			//glVertex3f(7.0f , 5.0f , 0.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(1.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(7.0);		vertices.push_back(5.0);		vertices.push_back(0.0);		// Vector Vertices
			//glVertex3f(0.0f , 5.0f , 0.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(1.0);		textures.push_back(1.0);										// Vector Textures
			vertices.push_back(0.0);		vertices.push_back(5.0);		vertices.push_back(0.0);		// Vector Vertices
			//glVertex3f(0.0f , -5.0f , 0.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(0.0);		vertices.push_back(-5.0);		vertices.push_back(0.0);		// Vector Vertices

			//glVertex3f(7.0f , -12.0f , -8.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(7.0);		vertices.push_back(-12.0);		vertices.push_back(-8.0);		// Vector Vertices
			//glVertex3f(7.0f , -5.0f , 0.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(1.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(7.0);		vertices.push_back(-5.0);		vertices.push_back(0.0);		// Vector Vertices
			//glVertex3f(0.0f , -5.0f , 0.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(0.0);		vertices.push_back(-5.0);		vertices.push_back(0.0);		// Vector Vertices
			//glVertex3f(0.0f , -12.0f , -8.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(0.0);		vertices.push_back(-12.0);		vertices.push_back(-8.0);		// Vector Vertices

			//glVertex3f(0.0f , 15.0f , -10.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(0.0);		vertices.push_back(15.0);		vertices.push_back(-10.0);		// Vector Vertices
			//glVertex3f(0.0f , 5.0f , 0.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(1.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(0.0);		vertices.push_back(5.0);		vertices.push_back(0.0);		// Vector Vertices
			//glVertex3f(7.0f , 5.0f , 0.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(1.0);		textures.push_back(1.0);										// Vector Textures
			vertices.push_back(7.0);		vertices.push_back(5.0);		vertices.push_back(0.0);		// Vector Vertices
			//glVertex3f(7.0f , 15.0f , -10.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(0.0);		textures.push_back(1.0);										// Vector Textures
			vertices.push_back(7.0);		vertices.push_back(15.0);		vertices.push_back(-10.0);		// Vector Vertices

			//glVertex3f(7.5f , -5.0f , -3.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(7.5);		vertices.push_back(-5.0);		vertices.push_back(-3.0);		// Vector Vertices
			//glVertex3f(7.5f , 5.0f , -3.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(1.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(7.5);		vertices.push_back(5.0);		vertices.push_back(-3.0);		// Vector Vertices
			//glVertex3f(7.0f , 5.0f , 0.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(1.0);		textures.push_back(1.0);										// Vector Textures
			vertices.push_back(7.0);		vertices.push_back(5.0);		vertices.push_back(0.0);		// Vector Vertices
			//glVertex3f(7.0f , -5.0f , 0.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(0.0);		textures.push_back(1.0);										// Vector Textures
			vertices.push_back(7.0);		vertices.push_back(-5.0);		vertices.push_back(0.0);		// Vector Vertices
				
			//glVertex3f(7.5f , -12.0f , -8.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(7.5);		vertices.push_back(-12.0);		vertices.push_back(-8.0);		// Vector Vertices
			//glVertex3f(7.5f , -5.0f , -3.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(1.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(7.5);		vertices.push_back(-5.0);		vertices.push_back(-3.0);		// Vector Vertices
			//glVertex3f(7.0f , -5.0f , 0.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(1.0);		textures.push_back(1.0);										// Vector Textures
			vertices.push_back(7.0);		vertices.push_back(-5.0);		vertices.push_back(0.0);		// Vector Vertices
			//glVertex3f(7.0f , -12.0f , -8.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(7.0);		vertices.push_back(-12.0);		vertices.push_back(-8.0);		// Vector Vertices
				
			//glVertex3f(7.0f , 15.0f , -10.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(7.0);		vertices.push_back(15.0);		vertices.push_back(-10.0);		// Vector Vertices
			//glVertex3f(7.0f , 5.0f , 0.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(1.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(7.0);		vertices.push_back(5.0);		vertices.push_back(0.0);		// Vector Vertices
			//glVertex3f(7.5f , 5.0f , -3.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(1.0);		textures.push_back(1.0);										// Vector Textures
			vertices.push_back(7.5);		vertices.push_back(5.0);		vertices.push_back(-3.0);		// Vector Vertices
			//glVertex3f(7.5f , 15.0f , -10.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
			textures.push_back(0.0);		textures.push_back(1.0);										// Vector Textures
			vertices.push_back(7.5);		vertices.push_back(15.0);		vertices.push_back(-10.0);		// Vector Vertices
		//glEnd();
// ----------------------- VBO
		nv = vertices.size();	// Tamany del vector vertices en elements.
		draw_GL_QUADS_VBO(vertices, normals, colors, textures, nv / 3);
		vertices.resize(0);	normals.resize(0);	colors.resize(0);	textures.resize(0);	// Reinicialitzar vectors
	glPopMatrix();

// Textura Reixeta Motor
	if (textu)  
		{	GLfloat sPlane[4] = { 0.05f, 0.25f, 0.00f, 0.00f };
		  	GLfloat tPlane[4] = { 0.00f, 0.25f, 0.05f, 0.00f };
// Activar textura
			glBindTexture(GL_TEXTURE_2D,VTextu[3]) ;
	
			glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
			glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);
			
			glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glEnable(GL_TEXTURE_2D);
		}

// Color reixeta motor sense textura
	glColor4f(0.5f,0.7f,1.0f,0.5f);	

// Reixeta Motor
	glPushMatrix();
		glTranslatef(0.0f,-22.0f,0.0f);

// Obtenir color actual definit en OpenGL amb glColor();
		glGetDoublev(GL_CURRENT_COLOR, cColor);

		//glBegin(GL_QUADS);
		    //glNormal3f(0.0f,-1.0f,0.0f);	
			//glTexCoord2f(0.0f,0.0f);
			//glVertex3f(-10.0f , -2.0f , -10.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(-1.0);		normals.push_back(0.0);			// Vector Normals
			textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(-10.0);		vertices.push_back(-2.0);		vertices.push_back(-10.0);		// Vector Vertices

			//glTexCoord2f(1.0f,0.0f);
			//glVertex3f(10.0f , -2.0f , -10.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(-1.0);		normals.push_back(0.0);			// Vector Normals
			textures.push_back(1.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(10.0);		vertices.push_back(-2.0);		vertices.push_back(-10.0);		// Vector Vertices

			//glTexCoord2f(1.0f,1.0);
			//glVertex3f(10.0f , -2.0f , 10.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(-1.0);		normals.push_back(0.0);			// Vector Normals
			textures.push_back(1.0);		textures.push_back(1.0);										// Vector Textures
			vertices.push_back(10.0);		vertices.push_back(-2.0);		vertices.push_back(10.0);		// Vector Vertices
			
			//glTexCoord2f(0.0f,1.0f);	
			//glVertex3f(-10.0f , -2.0f , 10.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(-1.0);		normals.push_back(0.0);			// Vector Normals
			textures.push_back(0.0);		textures.push_back(1.0);										// Vector Textures
			vertices.push_back(-10.0);		vertices.push_back(-2.0);		vertices.push_back(10.0);		// Vector Vertices
		//glEnd();
// ----------------------- VBO
		nv = vertices.size();	// Tamany del vector vertices en elements.
		draw_GL_QUADS_VBO(vertices, normals, colors, textures, nv / 3);
		vertices.resize(0);	normals.resize(0);	colors.resize(0);	textures.resize(0);	// Reinicialitzar vectors

	glPopMatrix();

// Desactivar textura
	if (textu) glDisable(GL_TEXTURE_2D);

// Color eix sense textura
	glColor4f(0.1f,0.4f,1.0f,0.5f);

// Eix Davanter
	glPushMatrix();
	  glTranslatef(0.0f,0.0f,-10.0f);
	  glRotatef(90.0f,0.0f,1.0f,0.0f);
  	  glTranslatef(0.0f,0.0f,-13.0f);
	  gluCylinder(2.0,2.0,26.0f,6,1);
	glPopMatrix();
	
// Roda Dreta Davantera
	glPushMatrix();
		glTranslatef(-10.0f,0.0f,-10.0f);
		neumatic(textu,VTextu);
	glPopMatrix();

// Roda Esquerra Davantera
	glPushMatrix();
		glTranslatef(17.0f,0.0f,-10.0f);
		neumatic(textu,VTextu);
	glPopMatrix();

// Desactivar textura
	if (textu) glDisable(GL_TEXTURE_2D);

// FARS	
// Color fars sense textura
	glColor4f(1.0f,0.0f,0.0f,0.5f);

// Far Esquerra	
	glPushMatrix();
		glTranslatef(-13.5f,-17.0f,-8.0f);
		glScalef(7.0f, 2.0f, 4.0f);
		glutSolidCube(1.0f);
	glPopMatrix();

// Far dret
	glPushMatrix();
		glTranslatef(13.5f,-17.0f,-8.0f);
		glScalef(7.0f, 2.0f, 4.0f);
		glRotatef(90.0f,1.0f,0.0f,0.0f); // Per a la textura
		glutSolidCube(1.0f);
	glPopMatrix();

// Textura frontal fars
	  if (textu)  
		{	GLfloat sPlane[4] = { 0.15f, 0.0f, 0.0f, 0.5f };
	  		GLfloat tPlane[4] = { 0.0f, 0.35f, 0.25f, 0.15f };
// Activar textura
			glBindTexture(GL_TEXTURE_2D,VTextu[4]) ;
	
			glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
			glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);
				
			glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glEnable(GL_TEXTURE_2D);
			}

// Color frontal fars sense textura
	glColor4f(1.0f,1.0f,1.0f,0.5f);

// Frontal far dret
	glPushMatrix();
		glTranslatef(-13.5f,-16.5f,-8.0f);

// Obtenir color actual definit en OpenGL amb glColor();
		glGetDoublev(GL_CURRENT_COLOR, cColor);

		//glBegin(GL_QUADS);
			//glNormal3f(0.0f,-1.0f,0.0f);	
			//glTexCoord2f(0.0f,0.0f);
			//glVertex3f(-3.5f , -2.0f , -2.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(-1.0);		normals.push_back(0.0);			// Vector Normals
			textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(-3.5);		vertices.push_back(-2.0);		vertices.push_back(-2.0);		// Vector Vertices
			
			//glTexCoord2f(1.0f,0.00f);
			//glVertex3f(3.5f , -2.0f , -2.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(-1.0);		normals.push_back(0.0);			// Vector Normals
			textures.push_back(1.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(3.5);		vertices.push_back(-2.0);		vertices.push_back(-2.0);		// Vector Vertices
			
			//glTexCoord2f(1.0f,1.0);
			//glVertex3f(3.5f , -2.0f , 2.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(-1.0);		normals.push_back(0.0);			// Vector Normals
			textures.push_back(1.0);		textures.push_back(1.0);										// Vector Textures
			vertices.push_back(3.5);		vertices.push_back(-2.0);		vertices.push_back(2.0);		// Vector Vertices
			
			//glTexCoord2f(0.0f,1.0f);	
			//glVertex3f(-3.5f , -2.0f , 2.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(-1.0);		normals.push_back(0.0);			// Vector Normals
			textures.push_back(0.0);		textures.push_back(1.0);										// Vector Textures
			vertices.push_back(-3.5);		vertices.push_back(-2.0);		vertices.push_back(2.0);		// Vector Vertices
		//glEnd();
// ----------------------- VBO
		nv = vertices.size();	// Tamany del vector vertices en elements.
		draw_GL_QUADS_VBO(vertices, normals, colors, textures, nv / 3);
		vertices.resize(0);	normals.resize(0);	colors.resize(0);	textures.resize(0);	// Reinicialitzar vectors
	glPopMatrix();

// Frontal far esquerre
	glPushMatrix();
		glTranslatef(13.5f,-16.5f,-8.0f);
		//glBegin(GL_QUADS);
			//glNormal3f(0.0f,-1.0f,0.0f);	
			//glTexCoord2f(0.0f,0.0f);
			//glVertex3f(-3.5f , -2.0f , -2.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(-1.0);		normals.push_back(0.0);			// Vector Normals
			textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(-3.5);		vertices.push_back(-2.0);		vertices.push_back(-2.0);		// Vector Vertices

			//glTexCoord2f(1.0f,0.00f);
			//glVertex3f(3.5f , -2.0f , -2.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(-1.0);		normals.push_back(0.0);			// Vector Normals
			textures.push_back(1.0);		textures.push_back(0.0);										// Vector Textures
			vertices.push_back(3.5);		vertices.push_back(-2.0);		vertices.push_back(-2.0);		// Vector Vertices

			//glTexCoord2f(1.0f,1.0);
			//glVertex3f(3.5f , -2.0f , 2.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(-1.0);		normals.push_back(0.0);			// Vector Normals
			textures.push_back(1.0);		textures.push_back(1.0);										// Vector Textures
			vertices.push_back(3.5);		vertices.push_back(-2.0);		vertices.push_back(2.0);		// Vector Vertices

			//glTexCoord2f(0.0f,1.0f);	
			//glVertex3f(-3.5f , -2.0f , 2.0f );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			normals.push_back(0.0);			normals.push_back(-1.0);		normals.push_back(0.0);			// Vector Normals
			textures.push_back(0.0);		textures.push_back(1.0);										// Vector Textures
			vertices.push_back(-3.5);		vertices.push_back(-2.0);		vertices.push_back(2.0);		// Vector Vertices

		//glEnd();
// ----------------------- VBO
		nv = vertices.size();	// Tamany del vector vertices en elements.
		draw_GL_QUADS_VBO(vertices, normals, colors, textures, nv / 3);
		vertices.resize(0);	normals.resize(0);	colors.resize(0);	textures.resize(0);	// Reinicialitzar vectors
	glPopMatrix();

// Desactivar textura
	if (textu) glDisable(GL_TEXTURE_2D);

// VIDRE CABINA (SENSE TEXTURA)
// Activar transparència
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

// Color vidre cabina sense textura
	glColor4f(0.3f,0.8f,0.7f,0.5f);

// Vidre
	glPushMatrix() ;
	  glTranslatef(0.0f,22.0f,10.0f);
	  glRotatef(-90.0f,1.0f,0.0f,0.0f);
	  glRotatef(90.0f,0.0f,1.0f,0.0f);
	  glTranslatef(0.0f,00.0f,-10.0f);

 // Obtenir color actual definit en OpenGL amb glColor();
	  glGetDoublev(GL_CURRENT_COLOR, cColor);

   	  //glBegin(GL_QUADS);  // Cara de 4 costats
	 	//glNormal3f(200.0f, 200.0f, 0.0f);
		//glVertex3f(10.0f ,  0.0f , 0.0f ); //P1
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);	// Vector Colors
		normals.push_back(200.0);		normals.push_back(200.0);		normals.push_back(0.0);			// Vector Normals
		textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
		vertices.push_back(10.0);		vertices.push_back(0.0);		vertices.push_back(0.0);		// Vector Vertices

		//glVertex3f(10.0f,0.0f,20.0f); //P2
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
		normals.push_back(200.0);		normals.push_back(0.0);			normals.push_back(20.0);			// Vector Normals
		textures.push_back(1.0);		textures.push_back(0.0);										// Vector Textures
		vertices.push_back(10.0);		vertices.push_back(0.0);		vertices.push_back(20.0);		// Vector Vertices

		//glVertex3f(0.0f,-10.0f,20.0f); //P3
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
		normals.push_back(200.0);		normals.push_back(200.0);		normals.push_back(0.0);			// Vector Normals
		textures.push_back(1.0);		textures.push_back(1.0);										// Vector Textures
		vertices.push_back(0.0);		vertices.push_back(-10.0);		vertices.push_back(20.0);		// Vector Vertices

		//glVertex3f(0.0f,-10.0f,0.0f); //P4
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
		normals.push_back(200.0);		normals.push_back(200.0);		normals.push_back(0.0);			// Vector Normals
		textures.push_back(0.0);		textures.push_back(1.0);										// Vector Textures
		vertices.push_back(0.0);		vertices.push_back(-10.0);		vertices.push_back(0.0);		// Vector Vertices

	  //glEnd();
 // ----------------------- VBO
	  nv = vertices.size();	// Tamany del vector vertices en elements.
	  draw_GL_QUADS_VBO2(vertices, normals, colors, textures, nv / 3);
	  vertices.resize(0);	normals.resize(0);	colors.resize(0);	textures.resize(0);	// Reinicialitzar vectors

	  //glBegin(GL_POLYGON);  // Triangle d'abaix
		//glNormal3f(0.0f,0.0f,-200.0f);
		//glVertex3f(0.0f,0.0f,0.0f);  // T1
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
		normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-200.0);		// Vector Normals
		textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
		vertices.push_back(0.0);		vertices.push_back(0.0);		vertices.push_back(0.0);		// Vector Vertices
		
		//glVertex3f(10.0f,0.0f,0.0f);  // P1
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
		normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-200.0);		// Vector Normals
		textures.push_back(0.0);		textures.push_back(1.0);										// Vector Textures
		vertices.push_back(10.0);		vertices.push_back(0.0);		vertices.push_back(0.0);		// Vector Vertices
		
		//glVertex3f(0.0f,-10.0f,0.0f); // P4
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
		normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-200.0);		// Vector Normals
		textures.push_back(1.0);		textures.push_back(1.0);										// Vector Textures
		vertices.push_back(0.0);		vertices.push_back(-10.0);		vertices.push_back(0.0);		// Vector Vertices

	  //glEnd();
 // ----------------------- VBO
	  nv = vertices.size();	// Tamany del vector vertices en elements.
	  draw_GL_POLYGON_VBO(vertices, normals, colors, textures, nv / 3);
	  vertices.resize(0);	normals.resize(0);	colors.resize(0);	textures.resize(0);	// Reinicialitzar vectors

	  //glBegin(GL_POLYGON);  // Triangle de dalt
		//glNormal3f(0.0f,0.0f,200.0f);
		//glVertex3f(0.0f,0.0f,20.0f);	// T2
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
		normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
		textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
		vertices.push_back(0.0);		vertices.push_back(0.0);		vertices.push_back(20.0);		// Vector Vertices

	    //glVertex3f(0.0f,-10.0f,20.0f); // P3
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
		normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
		textures.push_back(1.0);		textures.push_back(0.0);										// Vector Textures
		vertices.push_back(0.0);		vertices.push_back(-10.0);		vertices.push_back(20.0);		// Vector Vertices

		//glVertex3f(10.0f,0.0f,20.0f);  // P2
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
		normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(200.0);		// Vector Normals
		textures.push_back(0.0);		textures.push_back(1.0);										// Vector Textures
		vertices.push_back(10.0);		vertices.push_back(0.0);		vertices.push_back(20.0);		// Vector Vertices

	  //glEnd();
 // ----------------------- VBO
	  nv = vertices.size();	// Tamany del vector vertices en elements.
	  draw_GL_POLYGON_VBO(vertices, normals, colors, textures, nv / 3);
	  vertices.resize(0);	normals.resize(0);	colors.resize(0);	textures.resize(0);	// Reinicialitzar vectors

	glPopMatrix();

// Desactivar transparència
	glDisable(GL_BLEND);

// TUBS D'ESCAPAMENT (SENSE TEXTURA)
// Color tub d'escapament sense textura
	glColor4f(1.0f,0.0f,0.0f,0.5f);

// Tub d'escapament esquerre
	glPushMatrix();
		glTranslatef(18.0f,42.0f,10.0f);	
		gluCylinder(2.0f, 2.0f, 40.0f, 12, 1);
	glPopMatrix();
	
// Tub d'escapament dret
	glPushMatrix();
		glTranslatef(-18.0f,42.0f,10.0f);	
		gluCylinder(2.0f, 2.0f, 40.0f, 12, 1);
	glPopMatrix();

// CARROSSERIA
// Textura carrosseria
	if (textu)
		{	GLfloat sPlane[4] = { 0.05f, 0.0f, 0.00f, 0.250f };
		  	GLfloat tPlane[4] = { 0.00f, 0.05f, 0.1f, 0.00f };

// Activar textura
			glBindTexture(GL_TEXTURE_2D,VTextu[5]) ;
	
			glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
			glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);
			
			glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glEnable(GL_TEXTURE_2D);
		}

// Color porta-eix motriu sense textura
	glColor4f(0.5f,0.7f,1.0f,0.5f);

// Porta-eix motriu mig
	glPushMatrix();
	  glTranslatef(0.0f,82.0f,-5.0f);
	  glScalef(1.0f, 5.0f, 1.0f);
	  glutSolidCube(10.0f);
	glPopMatrix();

	if (textu) glDisable(GL_TEXTURE_2D);

// Conjunt eixos i rodes eix mig
	glPushMatrix();
// Desplaçament Eix Mig
	  glTranslatef(0.0f,72.0f,0.0f);

// Color eix sense textura
	  glColor4f(0.1f,0.4f,1.0f,0.5f);

// Eix Mig Davanter
	  glPushMatrix();
	    glTranslatef(0.0f,0.0f,-10.0f);
	    glRotatef(90.0f,0.0f,1.0f,0.0f);
	    glTranslatef(0.0f,0.0f,-13.0f);
		gluCylinder(2.0f, 2.0f, 26.0f, 6, 1);
	  glPopMatrix();
	
// Roda Dreta Davantera Eix Mig Davanter
	  glPushMatrix();
	    glTranslatef(-10.0f,0.0f,-10.0f);
	    neumatic(textu,VTextu);
	  glPopMatrix();

// Roda Esquerra Davantera Eix Mig Davanter
	  glPushMatrix();
	    glTranslatef(17.0f,0.0f,-10.0f);
	    neumatic(textu,VTextu);
	  glPopMatrix();

// Desactivar textura
	if (textu) glDisable(GL_TEXTURE_2D);

// Desplaçament entre dos eixos
	  glTranslatef(0.0f,25.0f,0.0f);

// Color eix sense textura
	  glColor4f(0.1f,0.4f,1.0f,0.5f);

// Eix Mig Darrera
	  glPushMatrix();
	    glTranslatef(0.0f,0.0f,-10.0f);
	    glRotatef(90.0f,0.0f,1.0f,0.0f);
	    glTranslatef(0.0f,0.0f,-13.0f);
		gluCylinder(2.0, 2.0, 26.0f, 6, 1);
	  glPopMatrix();
	
// Roda Dreta Davantera Eix Mig Darrera
	  glPushMatrix();
	    glTranslatef(-10.0f,0.0f,-10.0f);
	    neumatic(textu,VTextu);
	  glPopMatrix();

// Roda Esquerra Davantera Eix Mig Darrera
	  glPushMatrix();
	    glTranslatef(17.0f,0.0f,-10.0f);
	    neumatic(textu,VTextu);
	  glPopMatrix();

// Fi conjunt d'eixos i rodes eix mig
    glPopMatrix();

// Desactivar textura
	if (textu) glDisable(GL_TEXTURE_2D);
		
// REMOLC	
// Textura carrosseria
	if (textu)
		{	GLfloat sPlane[4] = { 0.05f, 0.0f, 0.00f, 0.250f };
		  	GLfloat tPlane[4] = { 0.00f, 0.05f, 0.1f, 0.00f };

// Activar textura
			glBindTexture(GL_TEXTURE_2D,VTextu[5]);
	
			glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
			glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);
			
			glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glEnable(GL_TEXTURE_2D);
		}


// Color plataforma sense textura
	glColor4f(0.5f,0.7f,1.0f,0.5f);

// Plataforma
	glPushMatrix();
		glTranslatef(0.0f,127.0f,4.0f);
		glScalef(3.0f, 14.0f, 0.8f);
		glutSolidCube(10.0f);
	glPopMatrix();

// Textura Cisterna
	if (textu)
		{	glBindTexture(GL_TEXTURE_2D,VTextu[6]);
			glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glEnable(GL_TEXTURE_2D);			
		}

// Color cisterna sense textura
	glColor4f(0.9f,0.9f,1.0f,0.5f);

// Cisterna
	glPushMatrix();
	  glTranslatef(0.0f,60.0f,20.0f);
	  glScalef(0.75f,1.0f,0.5f);
	  glRotatef(270.0f,1.0f,0.0f,0.0f);
// Part de davant
	  gluCylinder(20.0f, 25.0f, 5.0f, 25, 1);
	  glPushMatrix();
	  	glRotatef(180.0f,1.0f,0.0f,0.0f);
		gluDisk(0.0f, 20.0f, 25, 1);	
	  glPopMatrix();
// Part Central
	  glTranslatef(0.0f,0.0f,5.0f);
	  gluCylinder(25, 25, 125.0f, 25, 1);
// Part Darrera
	  glPushMatrix();
		glTranslatef(0.0f,0.0f,125.0f);
		gluCylinder(25.0f, 20.0f, 5.0f, 25, 1);
		glTranslatef(0.0f,0.0f,5.0f);
		gluDisk(0.0f, 20.0f, 25, 1);	
	  glPopMatrix();
	glPopMatrix();

// Desactivar textura
//	if (textu) glDisable(GL_TEXTURE_2D);

// Textura planxa
	if (textu)  
		{	
			GLfloat sPlane[4] = { 0.05f, 0.0f, 0.00f, 0.250f };
		  	GLfloat tPlane[4] = { 0.00f, 0.05f, 0.1f, 0.00f };
// Activar textura planxa
			glBindTexture(GL_TEXTURE_2D,VTextu[5]) ;
	
			glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
			glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);
			
			glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glEnable(GL_TEXTURE_2D);
		}

// Color porta-eix motriu
	glColor4f(0.5f,0.7f,1.0f,0.5f);

// Porta-eix motriu darrera
	glPushMatrix();
	  glTranslatef(0.0f,172.0f,-5.0f);
	  glScalef(1.0f, 5.0f, 1.0f);
	  glutSolidCube(10.0f);
	glPopMatrix();

// Desactivar textura
	if (textu) glDisable(GL_TEXTURE_2D);

// Conjunt eixos i rodes eix darrera
	glPushMatrix();

// Desplaçament Eix Darrera
	  glTranslatef(0.0f,162.0f,0.0f);

// Color eix sense textura
	  glColor4f(0.1f,0.4f,1.0f,0.5f);

// Eix Darrera Davanter
	  glPushMatrix();
	    glTranslatef(0.0f,0.0f,-10.0f);
	    glRotatef(90.0f,0.0f,1.0f,0.0f);
	    glTranslatef(0.0f,0.0f,-13.0f);
		gluCylinder(2.0,2.0,26.0f,6,1);
	  glPopMatrix();
	
// Roda Dreta Davantera Eix Darrera Davanter
	  glPushMatrix();
	    glTranslatef(-10.0f,0.0f,-10.0f);
	    neumatic(textu,VTextu);
	  glPopMatrix();

// Roda Esquerra Davantera Eix Darrera Davanter
	  glPushMatrix();
	    glTranslatef(17.0f,0.0f,-10.0f);
	    neumatic(textu,VTextu);
	  glPopMatrix();

// Desactivar textura
	if (textu) glDisable(GL_TEXTURE_2D);

// Desplaçament entre dos eixos
	  glTranslatef(0.0f,25.0f,0.0f);

// Color eix sense textura
	  glColor4f(0.1f,0.4f,1.0f,0.5f);

// Eix Darrera Darrera
	  glPushMatrix();
		glTranslatef(0.0f,0.0f,-10.0f);
	 	glRotatef(90.0f,0.0f,1.0f,0.0f);
		glTranslatef(0.0f,0.0f,-13.0f);
		gluCylinder(2.0f,2.0f,26.0f,6,1);
	  glPopMatrix();
	
// Roda Dreta Davantera Eix Darrera Darrera
	  glPushMatrix();
	    glTranslatef(-10.0f,0.0f,-10.0f);
	    neumatic(textu,VTextu);
	  glPopMatrix();

// Roda Esquerra Davantera Eix Darrera Darrera
	  glPushMatrix();
	    glTranslatef(17.0f,0.0f,-10.0f);
	    neumatic(textu,VTextu);
	  glPopMatrix();
// Fi conjunt eixos i rodes eix darrera
	glPopMatrix();

// Fi dibuix Truck
glPopMatrix();

// Restaurar textura[0]
if (textu)
	{	glBindTexture(GL_TEXTURE_2D,VTextu[0]);
// Desactivar genaració automàtica de coordenades textura
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
//		glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
//		glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
		glEnable(GL_TEXTURE_2D);			
	}
}


// Dibuix de roda
// neumatic: Paràmetres:
//    - textur: Flag de textures activades o no.
//    - VTextur: Vector d'imatges textura.
void neumatic(bool textur, GLint VTextur[NUM_MAX_TEXTURES])
{
// Textura neumàtic
	if (textur)  
		{	
// Activar textura neumàtic
			glBindTexture(GL_TEXTURE_2D,VTextur[1]) ;
			
			glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glEnable(GL_TEXTURE_2D);
		}

// Color neumàtic sense textura
	glColor4f(0.2f,0.2f,0.2f,0.5f);	

	glPushMatrix();
// Tapa inferior neumàtic
	  glRotatef(90.0f,0.0f,1.0f,0.0f);
	  gluDisk(5.0f, 10.0f, RESOLUCIO_RODA, 1);

// Dibuix de la llanta
	  llanta(textur,VTextur);

// Textura neumàtic
	  if (textur)  
		{	
// Activar textura neumàtic
			glBindTexture(GL_TEXTURE_2D,VTextur[1]) ;	
			glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glEnable(GL_TEXTURE_2D);
		}

// Color neumàtic sense textura
	glColor4f(0.2f,0.2f,0.2f,0.5f);	

// Gruix del neumàtic
	  glTranslatef(0.0f,0.0f,-6.0f);
	  gluCylinder(10.0f,10.0f,6.0f,RESOLUCIO_RODA,1);
// Tapa superior neumàtic
	  glRotatef(180.0f,0.0f,1.0f,0.0f);
	  gluDisk(5.0f,10.0f,RESOLUCIO_RODA,1);
// Dibuix de la llanta
	  llanta(textur,VTextur);
	glPopMatrix();

// Desactivar textures
//	if (textur) glDisable(GL_TEXTURE_2D);

}

// Dibuix de la llanta de la roda
// llanta: Paràmetres:
//    - textur: Flag de textures activades o no.
//    - VTextur: Vector d'imatges textura.
void llanta(bool textur, GLint VTextur[NUM_MAX_TEXTURES])
{

// Textura de la llanta
	if (textur)  
		{	GLfloat sPlane[4] = { 0.0f, -0.1f, 0.5f, 0.5f };
		  	GLfloat tPlane[4] = { 0.1f, 0.0f, 0.5f, 0.5f };
// Activar la textura
			glBindTexture(GL_TEXTURE_2D,VTextur[2]) ;
		
			glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
			glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);
			
			glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR) ;
			glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR) ;
			glEnable(GL_TEXTURE_GEN_S) ;
			glEnable(GL_TEXTURE_GEN_T) ;

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			glEnable(GL_TEXTURE_2D);
		}

// Dibuix de la llanta (disc de radi 5 de color clar)
	glColor4f(0.7f,0.7f,0.7f,0.5f);	
	gluDisk(0.0f,5.0f,RESOLUCIO_RODA,1);

}


// Mar amb ondulacions
void sea()
{
	int i, j, step, it1, it2;
	double angle, delay, Nx;

// VBO
	GLuint vboId = 0;
	std::vector <double> vertices, normals, colors, textures;	// Definició vectors dinàmics per a vertexs, normals i textures 
	vertices.resize(0);		normals.resize(0);		colors.resize(0);	textures.resize(0);			// Reinicialitzar vectors

// Aigua amb ondulacions simulades a partir de normals sinusoidals
	step = 10; //step = 5;
	delay = 0;
	it1 = 0;

	//glColor3f(0.5f, 0.4f, 0.9f);
// Obtenir color actual definit en OpenGL amb glColor();
	GLdouble cColor[4];
	glGetDoublev(GL_CURRENT_COLOR, cColor);
	
	float h = 2 * pi*step / 500;
	for (j = -250; j<250 - step; j = j + step)
	{	delay = 1.0*h*it1;
		it2 = 0;
		for (i = -250; i<250 - step; i = i + step)
		{
			//glBegin(GL_POLYGON);
			  angle = 1.0*it2*h * 15;
			  Nx = -cos(angle);
			  //glNormal3f(-cos(angle), 0, 1);
			  //glVertex3f(i, j, 0);
			  colors.push_back(cColor[0]);		colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			  normals.push_back(Nx);			normals.push_back(0.0);			normals.push_back(1.0);			// Vector Normals
			  textures.push_back(0.0);			textures.push_back(0.0);										// Vector Textures
			  vertices.push_back(i);			vertices.push_back(j);			vertices.push_back(0.0);		// Vector Vertices

			  angle = 1.0*(it2 + 1)*h * 15;
			  Nx = -cos(angle);
			  //glNormal3f(-cos(angle), 0, 1);
			  //glVertex3f(i + step, j, 0);
			  colors.push_back(cColor[0]);		colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			  normals.push_back(Nx);			normals.push_back(0.0);			normals.push_back(1.0);			// Vector Normals
			  textures.push_back(1.0);			textures.push_back(0.0);										// Vector Textures
			  vertices.push_back(i+step);		vertices.push_back(j);			vertices.push_back(0.0);		// Vector Vertices

			  angle = 1.0*(it2 + 1)*h * 15;
			  Nx = -cos(angle);
			  //glNormal3f(-cos(angle), 0, 1);
			  //glVertex3f(i + step, j + step, 0);
			  colors.push_back(cColor[0]);		colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			  normals.push_back(Nx);			normals.push_back(0.0);			normals.push_back(1.0);			// Vector Normals
			  textures.push_back(1.0);			textures.push_back(1.0);										// Vector Textures
			  vertices.push_back(i+step);		vertices.push_back(j+step);		vertices.push_back(0.0);		// Vector Vertices

			//glEnd();
// ----------------------- VBO
			std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.
			draw_GL_POLYGON_VBO(vertices, normals, colors, textures, nv / 3);
			vertices.resize(0);	normals.resize(0);	colors.resize(0);	textures.resize(0);	// Reinicialitzar vectors

			//glBegin(GL_POLYGON);
			  angle = 1.0*it2*h * 15;
			  //glNormal3f(-cos(angle), 0, 1);
			  //glVertex3f(i, j, 0);
			  colors.push_back(cColor[0]);		colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			  normals.push_back(-cos(angle));	normals.push_back(0.0);			normals.push_back(1.0);			// Vector Normals
			  textures.push_back(1.0);			textures.push_back(1.0);										// Vector Textures
			  vertices.push_back(i);			vertices.push_back(j);			vertices.push_back(0.0);		// Vector Vertices

			  angle = 1.0*(it2 + 1)*h * 15;
			  //glNormal3f(-cos(angle), 0, 1);
			  //glVertex3f(i + step, j + step, 0);
			  colors.push_back(cColor[0]);		colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			  normals.push_back(-cos(angle));	normals.push_back(0.0);			normals.push_back(1.0);			// Vector Normals
			  textures.push_back(1.0);			textures.push_back(1.0);										// Vector Textures
			  vertices.push_back(i + step);		vertices.push_back(j + step);	vertices.push_back(0.0);		// Vector Vertices

			  angle = 1.0*it2*h * 15;
			  //glNormal3f(-cos(angle), 0, 1);
			  //glVertex3f(i, j + step, 0);
			  colors.push_back(cColor[0]);		colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
			  normals.push_back(-cos(angle));	normals.push_back(0.0);			normals.push_back(1.0);			// Vector Normals
			  textures.push_back(0.0);			textures.push_back(1.0);										// Vector Textures
			  vertices.push_back(i);			vertices.push_back(j + step);		vertices.push_back(0.0);	// Vector Vertices

			//glEnd();
// ----------------------- VBO
			nv = vertices.size();	// Tamany del vector vertices en elements.
			draw_GL_POLYGON_VBO(vertices, normals, colors, textures, nv / 3);
			vertices.resize(0);	normals.resize(0);	colors.resize(0);	textures.resize(0);	// Reinicialitzar vectors

			it2++;
		}
		it1++;
	}
}


// OBJECTE TIE: FETS PER ALUMNES -----------------------------------------------------------------

// Objecte TIE
void tie(){
	Motor();
	Alas();
	Canon();
	Cuerpo();
	Cabina();
};

void Alas()
{
// Ala 1 Part Central
	int nvertexs = gluCylinder_VBO(5.0f, 5.0f, 0.5f, 6, 1);
//Parte Exterior
	glPushMatrix();
	  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	  glTranslatef(25.25f, 0.0f, 0.0f);
	  glRotated(90.0f, 0.0f, 1.0f, 0.0f);
	  glScaled(10.0f, 10.0f, 5.0f);
	  gluCylinder(5.0f, 5.0f, 0.5f, 6, 1);
	glPopMatrix();

//Part Interior
	glPushMatrix();
	  glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	  glTranslated(25.25f, 0.0f, 0.0f);
	  glRotated(90.0f, 0.0f, 1.0f, 0.0f);
	  glScaled(2.0f, 2.0f, 2.0f);
 	  gluCylinder(5.0f, 5.0f, 2.0f, 6, 1);
	glPopMatrix();
	deleteVBO(GLU_CYLINDER);

//Tapes Part Exterior
	glPushMatrix();
	  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	  glTranslatef(25.25f, 0.0f, 0.0f);
	  glRotated(90.0f, 0.0f, 1.0f, 0.0f);
	  glScaled(10.0f, 10.0f, 5.0f);
	  gluDisk(0.0f, 5.0f, 6, 1);
	glPopMatrix();

	glPushMatrix();
	  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	  glTranslatef(27.75f, 0.0f, 0.0f);
	  glRotated(90.0f, 0.0f, 1.0f, 0.0f);
	  glScaled(10.0f, 10.0f, 5.0f);
	  gluDisk(0.0f, 5.0f, 6, 1);
	glPopMatrix();

//Tapas Parte Interior
	glPushMatrix();
	  glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	  glTranslatef(25.25f, 0.0f, 0.0f);
	  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	  glScalef(2.0f, 2.0f, 2.0f);
	  gluDisk(0.0f, 5.0f, 6, 1);
	glPopMatrix();

	glPushMatrix();
	  glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	  glTranslatef(29.25f, 0.0f, 0.0f);
	  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	  glScalef(2.0f, 2.0f, 2.0f);
	  gluDisk(0.0f, 5.0f, 6, 1);
	glPopMatrix();

//Decoracion Exterior
	int j;
	glutSolidCube_VBO(1.0f);
	glPushMatrix();
	  glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	  glTranslatef(26.5f, 0.0f, 0.0f);
	  glScalef(15.75f, 13.75f, 13.75f);
	  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	  glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
	  for (j = 0; j<3; j = j + 1)
		{	glRotatef(60, 0, 0, 1);
			glPushMatrix();
			 glScalef(12.7f * 0.583f, 0.1f, 0.3f);
			 drawSolidCube();	//glutSolidCube(1.0f);
			glPopMatrix();
		}
	glPopMatrix();

	int u;
	glPushMatrix();
	  glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	  for (u = 0; u < 6; u = u + 1)
		{	glRotated(60, 1.0, 0.0, 0.0);
			glPushMatrix();
			  glTranslatef(26.5f, 0.0f, 43.25f);
			  glRotatef(90, 0.0, 0.0, 1.0);
			  glScalef(10 * 5.155f, 5.0f, 3.0f);
			  
			  drawSolidCube();	//glutSolidCube(1.0f);
			glPopMatrix();
	    }
	  deleteVBO(GLUT_CUBE);
	glPopMatrix();

//Ala 2 Parte central 

//Parte Exterior
	glPushMatrix();
	  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	  glTranslated(-27.75f, 0.0f, 0.0f);
	  glRotated(90.0f, 0.0f, 1.0f, 0.0f);
	  glScaled(10.0, 10.0, 5.0);
	  gluCylinder(5.0f, 5.0f, 0.5f, 6, 1);
	glPopMatrix();

//Parte Interior
	glPushMatrix();
	  glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	  glTranslatef(-29.25f, 0.0f, 0.0f);
	  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	  glScalef(2.0f, 2.0f, 2.0f);
	  gluCylinder(5.0f, 5.0f, 2.0f, 6, 1);
	glPopMatrix();

//Tapas Parte Exterior
	glPushMatrix();
	  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	  glTranslatef(-27.75f, 0.0f, 0.0f);
	  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	  glScalef(10.0f, 10.0f, 5.0f);
	  gluDisk(0.0f, 5.0f, 6, 1);
	glPopMatrix();

	glPushMatrix();
	  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	  glTranslatef(-25.25f, 0.0f, 0.0f);
	  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	  glScalef(10.0f, 10.0f, 5.0f);
	  gluDisk(0.0f, 5.0f, 6, 1);
	glPopMatrix();

//Tapas Parte Interior
	glPushMatrix();
	  glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
 	  glTranslatef(-29.25f, 0.0f, 0.0f);
	  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	  glScalef(2.0f, 2.0f, 2.0f);
	  gluDisk(0.0f, 5.0f, 6, 1);
	glPopMatrix();

	glPushMatrix();
	  glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	  glTranslatef(-25.25f, 0.0f, 0.0f);
	  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	  glScalef(2.0f, 2.0f, 2.0f);
	  gluDisk(0.0f, 5.0f, 6, 1);
	glPopMatrix();

//Decoracion Exterior
	int w;
	glutSolidCube_VBO(1.0f);
	glPushMatrix();
	  glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	  glTranslatef(-26.5f, 0.0f, 0.0f);
	  glScalef(15.75f, 13.75f, 13.75f);
	  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	  glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
	  for (w = 0; w<3; w = w + 1)
		{	glRotatef(60.0f, 0.0f, 0.0f, 1.0f);
			glPushMatrix();
			  glScalef(12.7f * 0.583f, 0.1f, 0.3f);
			  //glutSolidCube(1.0f);
			  drawSolidCube();
			glPopMatrix();
		}
	glPopMatrix();
	

	int h;
	glPushMatrix();
	  glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	  for (h = 0; h < 6; h = h + 1)
	  {	glRotated(60, 1.0, 0.0, 0.0);
		glPushMatrix();
		  glTranslatef(-26.5f, 0.0f, 43.25f);
		  glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		  glScalef(10 * 5.155f, 5.0f, 3.0f);
		  //glutSolidCube(1.0f);
		  drawSolidCube();
		glPopMatrix();
	  }
	glPopMatrix();
	deleteVBO(GLUT_CUBE);
};


void Motor()
{
//Helices Motor
	int j;
	glutSolidCube_VBO(1.0f);
	glPushMatrix();
	  glColor4f(0.58f, 0.58f, 0.58f, 0.0f);
	  glTranslatef(0.0f, -18.0f, 0.0f);
	  glRotated(-90.0f, 0.0f, 0.0f, 1.0f);
	  glRotated(90.0f, 0.0f, 1.0f, 0.0f);
	  glRotatef(45, 0, 0, 1);
	  for (j = 0; j<6; j = j + 1)
	  {	glRotatef(30, 0, 0, 1);
		glPushMatrix();
		  glScalef(12 * 0.583f, 0.1f, 0.3f);
		  //glutSolidCube(1.0f);
		  drawSolidCube();
		glPopMatrix();
	  }
	glPopMatrix();
	deleteVBO(GLUT_CUBE);

//Soporte Motor
	glPushMatrix();
	  glColor4f(0.28f, 0.28f, 0.28f, 0.0f);
	  glTranslatef(0.0f, -18.0f, 0.0f);
	  glRotated(-90.0f, 0.0f, 0.0f, 1.0f);
	  glRotated(90.0f, 0.0f, 1.0f, 0.0f);
	  glutSolidTorus(1.0, 5.0, 20, 20);
	glPopMatrix();
};

void Canon()
{

//Cañones

// Salida cañon 1
	int nvertexs= gluCylinder_VBO(1.0f, 0.5f, 5.0f, 60, 1);
	glPushMatrix();
	  glColor4d(0.28, 0.28, 0.28, 0.0);
	  glTranslated(5.0, 8.0, -10.0);
	  glRotated(-90.0, 1.0, 0.0, 0.0);
	  //gluCylinder(1.0f, 0.5f, 5.0f, 60, 1);
	  drawgluCylinder(nvertexs);
	glPopMatrix();

// Salida cañon 2
	glPushMatrix();
	  glColor4d(0.28, 0.28, 0.28, 0.0);
	  glTranslated(-5.0, 8.0, -10.0);
	  glRotated(-90.0, 1.0, 0.0, 0.0);
	  //gluCylinder(1.0f, 0.5f, 5.0f, 60, 1);
	  drawgluCylinder(nvertexs);
	glPopMatrix();
	deleteVBO(GLU_CYLINDER);

// Cañon 1
	nvertexs = gluCylinder_VBO(0.35f, 0.35f, 5.0f, 80, 1);
	glPushMatrix();
	  glColor4d(0.58, 0.58, 0.58, 0.0);
	  glTranslated(5.0, 10.0, -10.0);
	  glRotated(-90.0, 1.0, 0.0, 0.0);
	  //gluCylinder(0.35f, 0.35f, 5.0f, 80, 1);
	  drawgluCylinder(nvertexs);
	glPopMatrix();

//Cañon 2
	glPushMatrix();
	  glColor4d(0.58, 0.58, 0.58, 0.0);
 	  glTranslated(-5.0, 10.0, -10.0);
	  glRotated(-90.0, 1.0, 0.0, 0.0);
	  //gluCylinder(0.35f, 0.35f, 5.0f, 80, 1);
	  drawgluCylinder(nvertexs);
	glPopMatrix();
	deleteVBO(GLU_CYLINDER);
}

void Cuerpo()
{
//Sujeccion de las Alas

//Lado2
	int nvertexs = gluCylinder_VBO(4.0f, 2.0f, 10.25f, 40, 1);
	glPushMatrix();
	  glColor4d(0.16, 0.16, 0.16, 1.0);
	  glRotated(-90.0, 0.0, 1.0, 0.0);
	  glRotated(90.0, 0.0, 0.0, 1.0);
	  glScaled(2.0, 2.0, 2.5);
	  //gluCylinder(4.0f, 2.0f, 10.25f, 40, 1);
	  drawgluCylinder(nvertexs);
	glPopMatrix();

//Lado 1
	glPushMatrix();
	  glColor4f(0.16f, 0.16f, 0.16f, 1.0f);
	  glRotated(90.0, 0.0, 1.0, 0.0);
	  glRotated(90.0, 0.0, 0.0, 1.0);
	  glScaled(2.0, 2.0, 2.5);
	  //gluCylinder(4.0f, 2.0f, 10.25f, 40, 1);
	  drawgluCylinder(nvertexs);
	glPopMatrix();
	deleteVBO(GLU_CYLINDER);

//X2
	nvertexs = glutSolidTorus_VBO(1.0, 5.0, 20, 20);
	glPushMatrix();
	  glColor4d(0.16, 0.16, 0.16, 1.0);
	  glTranslated(-25.0, 0.0, 0.0);
	  glRotated(90.0, 0.0, 1.0, 0.0);
	  //glutSolidTorus(1.0, 5.0, 20, 20);
	  drawSolidTorus(nvertexs);
	glPopMatrix();

//X1
	glPushMatrix();
	  glColor4d(0.16, 0.16, 0.16, 1.0);
	  glTranslated(25.25, 0.0, 0.0);
	  glRotated(90.0, 0.0, 1.0, 0.0);
	  //glutSolidTorus(1.0, 5.0, 20, 20);
	  drawSolidTorus(nvertexs);
	glPopMatrix();
	deleteVBO(GLUT_TORUS);

//Cuerpo
	glPushMatrix();
	  glColor4d(0.16, 0.16, 0.16, 1.0);
	  glScaled(1.75f, 1.75f, 1.5f);
	  //glutSolidSphere(10.0f, 80, 80);
	  gluSphere(10.0f, 80, 80);
	glPopMatrix();
}

void Cabina()
{
	glEnable(GL_BLEND);

// Tapa Cabina
	glPushMatrix();
	  glColor4d(1.0, 0.058, 0.058, 0.5);
	  glTranslated(0.0, 19.45, 0.0);
	  glRotated(-90.0, 0.0, 0.0, 1.0);
	  glRotated(90.0, 0.0, 1.0, 0.0);
	  glScaled(1.75, 1.75, 1.75);
	  gluDisk(0.0f, 1.5f, 8, 1);
	glPopMatrix();

// Cristal Cabina
	glPushMatrix();
	  glColor4d(1.0, 0.058, 0.058, 0.5);
	  glTranslated(0.0f, 19.45f, 0.0f);
	  glRotated(-90.0f, 0.0f, 0.0f, 1.0f);
	  glRotated(90.0f, 0.0f, 1.0f, 0.0f);
	  glScaled(1.75f, 1.75f, 1.75f);
	  gluCylinder(1.5f, 4.5f, 2.0f, 8, 1);
	glPopMatrix();

	glDisable(GL_BLEND);
}
// FI OBJECTE TIE: FETS PER ALUMNES -----------------------------------------------------------------

// ---------------- Entorn VGI: Funcions locals a escena.cpp
// CString2char: Funció de conversió de variable CString a char * per a noms de fitxers 
char* CString2Char(CString entrada)
{
//char * par_sortida=" ";
// Variable de tipus CString 'entrada' conté un string tipus CString
//-------------------------------------------------------------------------------------
// Conversió de la variable CString entrada a la variable char *sortida, 
//	compatible amb la funció Carregar3DS, de càrrega de fitxers 3DS
//	char * nomfitx = (char *)(LPCTSTR)nom;

// Conversió variable w_char --> char *
//	char *nomf="  ";
//	wcstombs(nomf,strFileName.GetBuffer(3),90);
//	char *nomf = reinterpret_cast<char *> (nom.GetBuffer(3));

	size_t origsize = wcslen(entrada.GetBuffer(3)) + 1;
	size_t convertedChars = 0;

// Use a multibyte string to append the type of string
// to the new string before displaying the result.
	char strConcat[] = " (char *)";
	size_t strConcatsize = (strlen(strConcat) + 1) * 2;

// Allocate two bytes in the multibyte output string for every wide
// character in the input string (including a wide character
// null). Because a multibyte character can be one or two bytes,
// you should allot two bytes for each character. Having extra
// space for the new string is not an error, but having
// insufficient space is a potential security problem.
	const size_t newsize = origsize * 2;
// The new string will contain a converted copy of the original
// string plus the type of string appended to it.
//	char *nomfitx = new char[newsize + strConcatsize];
	char* par_sortida = new char[newsize + strConcatsize];

// Put a copy of the converted string into nstring
	wcstombs_s(&convertedChars, par_sortida, newsize, entrada.GetBuffer(3), _TRUNCATE);
// append the type of string to the new string.
//----------------------------------------------------------------------------------

// Variable de tipus char *nomfitx conté el nom del fitxer seleccionat
	return par_sortida;
}