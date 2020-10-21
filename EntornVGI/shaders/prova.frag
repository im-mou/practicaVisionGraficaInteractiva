//******** PRACTICA VISUALITZACI� GR�FICA INTERACTIVA (EE-UAB)
//******** Entorn basic amb interf�cie MFC i Status Bar
//******** Enric Marti (Mar� 2017)
// prova.frag: Fragment Program en GLSL erroni per a provar missatges compilador GLSL

uniferm sampler2D texture0;
uniform bool texture;

void main ()

{
    if (texture) {
// Intensitat amb textura
    vac4 colorT = texture2D(texture0,gl_TexCoord[0].st);
//    gl_FragColor = gl_Color * 0.3 + colorT * 1.0;
    gl_FragColor = gl_Color * colorT;
    }
    else {
// Intensitat sense textura
    gl_FragColor = gl_Color;   
    }

}
