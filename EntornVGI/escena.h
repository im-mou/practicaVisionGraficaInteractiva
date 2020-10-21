//******** PRACTICA VISUALITZACIÓ GRÀFICA INTERACTIVA (Escola Enginyeria - UAB)
//******** Entorn bàsic VS2019 MULTIFINESTRA amb OpenGL, interfície MFC i Status Bar
//******** Ferran Poveda, Marc Vivet, Carme Julià, Débora Gil, Enric Martí (Setembre 2020)
// escena.h : interface de escena.cpp
//

#ifndef GLSOURCE_H
#define GLSOURCE_H

/* ------------------------------------------------------------------------- */
/*                            Funcions de les escenes                        */
/* ------------------------------------------------------------------------- */
// Entorn VGI: dibuixa_EscenaGL -> Dibuix de l'escena GL
void dibuixa_EscenaGL(char objecte, CColor col_object, bool ref_mat, bool sw_mat[4], bool textur, GLint texturID[NUM_MAX_TEXTURES], bool textur_map,
	int nptsU, CPunt3D PC_u[MAX_PATCH_CORBA], GLfloat pasCS, bool sw_PC);

// Entorn VGI: dibuixa -> Funció que dibuixa objectes simples de la llibreria GLUT segons obj
void dibuixa(char obj);

// Entorn VGI: Objecte arc()
void arc(bool ref_mat, bool sw_mat[4]);

// Entorn VGI: Truck - Objecte fet per alumnes.
void truck(bool textu,GLint VTextu[NUM_MAX_TEXTURES]);
void neumatic(bool textur, GLint VTextur[NUM_MAX_TEXTURES]);
void llanta(bool textur, GLint VTextur[NUM_MAX_TEXTURES]);
void sea(void);

// Entorn VGI: Tie (Nau enemiga Star Wars) - Objecte fet per alumnes.
void tie();
void Alas();
void Motor();
void Canon();
void Cuerpo();
void Cabina();

// Entorn VGI: Funcions locals a escena.cpp
// CString2char: Funció de conversió de variable CString a char * per a noms de fitxers 
char* CString2Char(CString entrada);
#endif