#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include<windows.h>

#define HORIZONTAL_LIMIT 10
#define DOWN_LIMIT 18
#define HORIZONTAL_MIDDLE HORIZONTAL_LIMIT/2

void MOVER(int x);
int ** ALOCAMATRIX(int l, int c);
int ** LIBERAMATRIX(int l, int c, int **v);
void PECA();
void ORIENTACAO();
void LINHA(int ** matrix, int line);

typedef struct{
	GLfloat x;
	GLfloat y;
}COORDENADA;
COORDENADA coordenadas[4];

GLfloat r = 0.0, g = 0.0, b = 0.0, rotationFull = 0.0, rotationHalf = 0.0, horizontalMovement = 0.0, downMovement = 0.0;

int **game, angleX = 0, angleY = 0, angleZ = 0, score = 0, lvl = 1, speed = 1000, maxScore;

char type, nextPiece;

GLfloat angle, fAspect, rotX, rotY;
GLdouble obsX, obsY, obsZ;

FILE *record;

void init(void){

    /// Habilita a definição da cor.
    glEnable(GL_COLOR_MATERIAL);
    ///Habilita o uso de iluminação.
    glEnable(GL_LIGHTING);
    /// Habilita a luz.
    glEnable(GL_LIGHT0);
    /// Habilita o depth-buffering.
    glEnable(GL_DEPTH_TEST);
    /// Habilita o modelo de colorização.
    glShadeModel(GL_SMOOTH);
	/// Inicializa a variável que especifica o ângulo da projeção perspectiva.
	angle=50;
	/// Inicializa as variáveis usadas para alterar a posição do observador virtual.
	rotX = 30;
	rotY = 0;
	obsZ = 180;
	///Cor de fundo da janela.
	glClearColor (1.0, 1.0, 1.0, 1.0);
	glutTimerFunc(speed, MOVER, 1);
	game = ALOCAMATRIX(DOWN_LIMIT, HORIZONTAL_LIMIT);
	for(int i = 0; i < DOWN_LIMIT; i++){
		for(int j = 0; j < HORIZONTAL_LIMIT; j++){
			game[i][j] = 0;
		}
	}
	int kind;
	kind = rand()%7;
	if(kind == 0)
		nextPiece = 'l';
	else if(kind == 1)
		nextPiece = 'J';
	else if(kind == 2)
		nextPiece = 'L';
	else if(kind == 3)
		nextPiece = 'Q';
	else if(kind == 4)
		nextPiece = 'S';
	else if(kind == 5)
		nextPiece = 'T';
	else if(kind == 6)
		nextPiece = 'Z';
	PECA();
	glEnable(GL_CULL_FACE);
    glColor3f(0.0f, 0.0f, 1.0f);
}

void gameOver(){
	exit(0);
}

void sortColor(){
	r = (GLfloat)rand()/(RAND_MAX+1.0);
	g = (GLfloat)rand()/(RAND_MAX+1.0);
	b = (GLfloat)rand()/(RAND_MAX+1.0);
}

int ** ALOCAMATRIX(int l, int c){
	int ** v;
	int i;
	v=(int**)calloc(l,sizeof(int*));
	for (i = 0;i < l;i++){
    	v[i] = (int*)calloc(c,sizeof(int));
	}
	return (v);
}

int ** LIBERAMATRIX(int l, int c, int **v){
	int i;
	for(i = 0;i < l;i++)
    	free(v[i]);
	free(v);
	return(NULL);
}

void setCoordinates(){
	if(type == 'l'){
		if(rotationHalf == 0){
			coordenadas[0].x = HORIZONTAL_MIDDLE-1;
			coordenadas[0].y = 0;
			coordenadas[1].x = HORIZONTAL_MIDDLE;
			coordenadas[1].y = 0;
			coordenadas[2].x = HORIZONTAL_MIDDLE + 1;
			coordenadas[2].y = 0;
			coordenadas[3].x = HORIZONTAL_MIDDLE + 2;
			coordenadas[3].y = 0;
		}else{
			coordenadas[0].x = HORIZONTAL_MIDDLE;
			coordenadas[0].y = -1;
			coordenadas[1].x = HORIZONTAL_MIDDLE;
			coordenadas[1].y = 0;
			coordenadas[2].x = HORIZONTAL_MIDDLE;
			coordenadas[2].y = 1;
			coordenadas[3].x = HORIZONTAL_MIDDLE;
			coordenadas[3].y = 2;
		}
	}
	if(type == 'J'){
		if(rotationFull == 0){
			coordenadas[0].x = HORIZONTAL_MIDDLE - 1;
			coordenadas[0].y = 0;
			coordenadas[1].x = HORIZONTAL_MIDDLE - 1;
			coordenadas[1].y = 1;
			coordenadas[2].x = HORIZONTAL_MIDDLE;
			coordenadas[2].y = 1;
			coordenadas[3].x = HORIZONTAL_MIDDLE + 1;
			coordenadas[3].y = 1;
		}
		if(rotationFull == 90){
			coordenadas[0].x = HORIZONTAL_MIDDLE;
			coordenadas[0].y = 0;
			coordenadas[1].x = HORIZONTAL_MIDDLE;
			coordenadas[1].y = 1;
			coordenadas[2].x = HORIZONTAL_MIDDLE;
			coordenadas[2].y = 2;
			coordenadas[3].x = HORIZONTAL_MIDDLE + 1;
			coordenadas[3].y = 0;
		}
		if(rotationFull == 180){
			coordenadas[0].x = HORIZONTAL_MIDDLE - 1;
			coordenadas[0].y = 1;
			coordenadas[1].x = HORIZONTAL_MIDDLE;
			coordenadas[1].y = 1;
			coordenadas[2].x = HORIZONTAL_MIDDLE + 1;
			coordenadas[2].y = 1;
			coordenadas[3].x = HORIZONTAL_MIDDLE + 1;
			coordenadas[3].y = 2;
		}
		if(rotationFull == 270){
			coordenadas[0].x = HORIZONTAL_MIDDLE;
			coordenadas[0].y = 0;
			coordenadas[1].x = HORIZONTAL_MIDDLE;
			coordenadas[1].y = 1;
			coordenadas[2].x = HORIZONTAL_MIDDLE;
			coordenadas[2].y = 2;
			coordenadas[3].x = HORIZONTAL_MIDDLE - 1;
			coordenadas[3].y = 2;
		}
	}
	if(type == 'L'){
		if(rotationFull == 0){
			coordenadas[0].x = HORIZONTAL_MIDDLE - 1;
			coordenadas[0].y = 1;
			coordenadas[1].x = HORIZONTAL_MIDDLE;
			coordenadas[1].y = 1;
			coordenadas[2].x = HORIZONTAL_MIDDLE + 1;
			coordenadas[2].y = 1;
			coordenadas[3].x = HORIZONTAL_MIDDLE + 1;
			coordenadas[3].y = 0;
		}
		if(rotationFull == 90){
			coordenadas[0].x = HORIZONTAL_MIDDLE;
			coordenadas[0].y = 0;
			coordenadas[1].x = HORIZONTAL_MIDDLE;
			coordenadas[1].y = 1;
			coordenadas[2].x = HORIZONTAL_MIDDLE;
			coordenadas[2].y = 2;
			coordenadas[3].x = HORIZONTAL_MIDDLE + 1;
			coordenadas[3].y = 2;
		}
		if(rotationFull == 180){
			coordenadas[0].x = HORIZONTAL_MIDDLE - 1;
			coordenadas[0].y = 1;
			coordenadas[1].x = HORIZONTAL_MIDDLE;
			coordenadas[1].y = 1;
			coordenadas[2].x = HORIZONTAL_MIDDLE + 1;
			coordenadas[2].y = 1;
			coordenadas[3].x = HORIZONTAL_MIDDLE - 1;
			coordenadas[3].y = 2;
		}
		if(rotationFull == 270){
			coordenadas[0].x = HORIZONTAL_MIDDLE;
			coordenadas[0].y = 0;
			coordenadas[1].x = HORIZONTAL_MIDDLE;
			coordenadas[1].y = 1;
			coordenadas[2].x = HORIZONTAL_MIDDLE;
			coordenadas[2].y = 2;
			coordenadas[3].x = HORIZONTAL_MIDDLE - 1;
			coordenadas[3].y = 0;
		}
	}
	if(type == 'Q'){
		coordenadas[0].x = HORIZONTAL_MIDDLE - 1;
		coordenadas[0].y = 0;
		coordenadas[1].x = HORIZONTAL_MIDDLE;
		coordenadas[1].y = 0;
		coordenadas[2].x = HORIZONTAL_MIDDLE;
		coordenadas[2].y = 1;
		coordenadas[3].x = HORIZONTAL_MIDDLE - 1;
		coordenadas[3].y = 1;
	}
	if(type == 'S'){
		if(rotationHalf == 0){
			coordenadas[0].x = HORIZONTAL_MIDDLE - 1;
			coordenadas[0].y = 1;
			coordenadas[1].x = HORIZONTAL_MIDDLE;
			coordenadas[1].y = 1;
			coordenadas[2].x = HORIZONTAL_MIDDLE;
			coordenadas[2].y = 0;
			coordenadas[3].x = HORIZONTAL_MIDDLE + 1;
			coordenadas[3].y = 0;
		}else{
			coordenadas[0].x = HORIZONTAL_MIDDLE - 1;
			coordenadas[0].y = -1;
			coordenadas[1].x = HORIZONTAL_MIDDLE - 1;
			coordenadas[1].y = 0;
			coordenadas[2].x = HORIZONTAL_MIDDLE;
			coordenadas[2].y = 0;
			coordenadas[3].x = HORIZONTAL_MIDDLE;
			coordenadas[3].y = 1;
		}
	}
	if(type == 'T'){
		if(rotationFull == 0){
			coordenadas[0].x = HORIZONTAL_MIDDLE;
			coordenadas[0].y = 0;
			coordenadas[1].x = HORIZONTAL_MIDDLE;
			coordenadas[1].y = 1;
			coordenadas[2].x = HORIZONTAL_MIDDLE - 1;
			coordenadas[2].y = 1;
			coordenadas[3].x = HORIZONTAL_MIDDLE + 1;
			coordenadas[3].y = 1;
		}
		if(rotationFull == 90){
			coordenadas[0].x = HORIZONTAL_MIDDLE;
			coordenadas[0].y = 0;
			coordenadas[1].x = HORIZONTAL_MIDDLE;
			coordenadas[1].y = 1;
			coordenadas[2].x = HORIZONTAL_MIDDLE;
			coordenadas[2].y = 2;
			coordenadas[3].x = HORIZONTAL_MIDDLE + 1;
			coordenadas[3].y = 1;
		}
		if(rotationFull == 180){
			coordenadas[0].x = HORIZONTAL_MIDDLE - 1;
			coordenadas[0].y = 1;
			coordenadas[1].x = HORIZONTAL_MIDDLE;
			coordenadas[1].y = 1;
			coordenadas[2].x = HORIZONTAL_MIDDLE + 1;
			coordenadas[2].y = 1;
			coordenadas[3].x = HORIZONTAL_MIDDLE;
			coordenadas[3].y = 2;
		}
		if(rotationFull == 270){
			coordenadas[0].x = HORIZONTAL_MIDDLE;
			coordenadas[0].y = 0;
			coordenadas[1].x = HORIZONTAL_MIDDLE;
			coordenadas[1].y = 1;
			coordenadas[2].x = HORIZONTAL_MIDDLE;
			coordenadas[2].y = 2;
			coordenadas[3].x = HORIZONTAL_MIDDLE - 1;
			coordenadas[3].y = 1;
		}
	}
	if(type == 'Z'){
		if(rotationHalf == 0){
			coordenadas[0].x = HORIZONTAL_MIDDLE - 1;
			coordenadas[0].y = 0;
			coordenadas[1].x = HORIZONTAL_MIDDLE;
			coordenadas[1].y = 0;
			coordenadas[2].x = HORIZONTAL_MIDDLE;
			coordenadas[2].y = 1;
			coordenadas[3].x = HORIZONTAL_MIDDLE + 1;
			coordenadas[3].y = 1;
		}else{
			coordenadas[0].x = HORIZONTAL_MIDDLE + 1;
			coordenadas[0].y = 0;
			coordenadas[1].x = HORIZONTAL_MIDDLE + 1;
			coordenadas[1].y = 1;
			coordenadas[2].x = HORIZONTAL_MIDDLE;
			coordenadas[2].y = 1;
			coordenadas[3].x = HORIZONTAL_MIDDLE;
			coordenadas[3].y = 2;
		}
	}
}

void draw_l(){
	glPushMatrix();
		glTranslatef(horizontalMovement, -downMovement, 0.0);
		glTranslatef(2.0, 0.0, 0.0);
		glRotatef(-rotationHalf, 0.0, 0.0, 1.0);
		glTranslatef(-1.0, 0.0, 0.0);
		glutSolidCube(1.0);
			glColor3f(1.0f, 1.0f, 1.0f);
			glutWireCube(1.1);
            glColor3f(0.0f, 0.5f, 0.0f);
		glTranslatef(1.0, 0.0, 0.0);
		glutSolidCube(1.0);
			glColor3f(1.0f, 1.0f, 1.0f);
			glutWireCube(1.1);
            glColor3f(0.0f, 0.5f, 0.0f);
		glTranslatef(1.0, 0.0, 0.0);
		glutSolidCube(1.0);
		    glColor3f(1.0f, 1.0f, 1.0f);
			glutWireCube(1.1);
			glColor3f(0.0f, 0.5f, 0.0f);
		glTranslatef(1.0, 0.0, 0.0);
		glutSolidCube(1.0);
			glColor3f(1.0f, 1.0f, 1.0f);
			glutWireCube(1.1);
			glColor3f(0.0f, 0.5f, 0.0f);
	glPopMatrix();
}

void draw_J(){
	glPushMatrix();
		glTranslatef(horizontalMovement, -downMovement, 0.0);
		glTranslatef(2.0, -1.0, 0.0);
		glRotatef(-rotationFull, 0.0, 0.0, 1.0);
		glTranslatef(-1.0, 1.0, 0.0);
		glutSolidCube(1.0);
			glColor3f(1.0f, 1.0f, 1.0f);
			glutWireCube(1.1);
            glColor3f(0.0f, 0.5f, 0.0f);
		glTranslatef(0.0, -1.0, 0.0);
		glutSolidCube(1.0);
			glColor3f(1.0f, 1.0f, 1.0f);
			glutWireCube(1.1);
			glColor3f(0.0f, 0.5f, 0.0f);
		glTranslatef(1.0, 0.0, 0.0);
		glutSolidCube(1.0);
			glColor3f(1.0f, 1.0f, 1.0f);
			glutWireCube(1.1);
			glColor3f(0.0f, 0.5f, 0.0f);
		glTranslatef(1.0, 0.0, 0.0);
		glutSolidCube(1.0);
	     	glColor3f(1.0f, 1.0f, 1.0f);
			glutWireCube(1.1);
			glColor3f(0.0f, 0.5f, 0.0f);
	glPopMatrix();
}

void draw_L(){
	glPushMatrix();
		glTranslatef(horizontalMovement, -downMovement, 0.0);
		glTranslatef(2.0, -1.0, 0.0);
		glRotatef(-rotationFull, 0.0, 0.0, 1.0);
		glTranslatef(-1.0, 0.0, 0.0);
		glutSolidCube(1.0);
			glColor3f(1.0f, 1.0f, 1.0f);
			glutWireCube(1.1);
			glColor3f(0.0f, 0.5f, 0.0f);
		glTranslatef(1.0, 0.0, 0.0);
		glutSolidCube(1.0);
			glColor3f(1.0f, 1.0f, 1.0f);
			glutWireCube(1.1);
			glColor3f(0.0f, 0.5f, 0.0f);
		glTranslatef(1.0, 0.0, 0.0);
		glutSolidCube(1.0);
			glColor3f(1.0f, 1.0f, 1.0f);
			glutWireCube(1.1);
			glColor3f(0.0f, 0.5f, 0.0f);
		glTranslatef(0.0, 1.0, 0.0);
		glutSolidCube(1.0);
			glColor3f(1.0f, 1.0f, 1.0f);
			glutWireCube(1.1);
			glColor3f(0.0f, 0.5f, 0.0f);
	glPopMatrix();
}

void draw_Q(){
	glPushMatrix();
		glTranslatef(horizontalMovement, -downMovement, 0.0);
		glTranslatef(1.0, 0.0, 0.0);
		glutSolidCube(1.0);
			glColor3f(1.0f, 1.0f, 1.0f);
			glutWireCube(1.1);
			glColor3f(0.0f, 0.5f, 0.0f);
		glTranslatef(1.0, 0.0, 0.0);
		glutSolidCube(1.0);
			glColor3f(1.0f, 1.0f, 1.0f);
			glutWireCube(1.1);
			glColor3f(0.0f, 0.5f, 0.0f);
		glTranslatef(0.0, -1.0, 0.0);
		glutSolidCube(1.0);
			glColor3f(1.0f, 1.0f, 1.0f);
			glutWireCube(1.1);
			glColor3f(0.0f, 0.5f, 0.0f);
		glTranslatef(-1.0, 0.0, 0.0);
		glutSolidCube(1.0);
			glColor3f(1.0f, 1.0f, 1.0f);
			glutWireCube(1.1);
			glColor3f(0.0f, 0.5f, 0.0f);
	glPopMatrix();
}

void draw_S(){
	glPushMatrix();
		glTranslatef(horizontalMovement, -downMovement, 0.0);
		glTranslatef(2.0, 0.0, 0.0);
		glRotatef(-rotationHalf, 0.0, 0.0, 1.0);
		glutSolidCube(1.0);
			glColor3f(1.0f, 1.0f, 1.0f);
			glutWireCube(1.1);
			glColor3f(0.0f, 0.5f, 0.0f);
		glTranslatef(1.0, 0.0, 0.0);
		glutSolidCube(1.0);
			glColor3f(1.0f, 1.0f, 1.0f);
			glutWireCube(1.1);
			glColor3f(0.0f, 0.5f, 0.0f);
		glTranslatef(-1.0, -1.0, 0.0);
		glutSolidCube(1.0);
			glColor3f(1.0f, 1.0f, 1.0f);
			glutWireCube(1.1);
			glColor3f(0.0f, 0.5f, 0.0f);
		glTranslatef(-1.0, 0.0, 0.0);
		glutSolidCube(1.0);
			glColor3f(1.0f, 1.0f, 1.0f);
			glutWireCube(1.1);
			glColor3f(0.0f, 0.5f, 0.0f);
	glPopMatrix();
}

void draw_T(){
	glPushMatrix();
		glTranslatef(horizontalMovement, -downMovement, 0.0);
		glTranslatef(2.0, -1.0, 0.0);
		glRotatef(-rotationFull, 0.0, 0.0, 1.0);
		glTranslatef(-1.0, 0.0, 0.0);
		glutSolidCube(1.0);
			glColor3f(1.0f, 1.0f, 1.0f);
			glutWireCube(1.1);
			glColor3f(0.0f, 0.5f, 0.0f);
		glTranslatef(1.0, 0.0, 0.0);
		glutSolidCube(1.0);
			glColor3f(1.0f, 1.0f, 1.0f);
			glutWireCube(1.1);
			glColor3f(0.0f, 0.5f, 0.0f);
		glTranslatef(1.0, 0.0, 0.0);
		glutSolidCube(1.0);
			glColor3f(1.0f, 1.0f, 1.0f);
			glutWireCube(1.1);
			glColor3f(0.0f, 0.5f, 0.0f);
		glTranslatef(-1.0, 1.0, 0.0);
		glutSolidCube(1.0);
			glColor3f(1.0f, 1.0f, 1.0f);
			glutWireCube(1.1);
			glColor3f(0.0f, 0.5f, 0.0f);
	glPopMatrix();
}

void draw_Z(){
	glPushMatrix();
		glTranslatef(horizontalMovement, -downMovement, 0.0);
		glTranslatef(2.0, -1.0, 0.0);
		glRotatef(-rotationHalf, 0.0, 0.0, 1.0);
		glTranslatef(-1.0, 1.0, 0.0);
		glutSolidCube(1.0);
			glColor3f(1.0f, 1.0f, 1.0f);
			glutWireCube(1.1);
			glColor3f(0.0f, 0.5f, 0.0f);
		glTranslatef(1.0, 0.0, 0.0);
		glutSolidCube(1.0);
			glColor3f(1.0f, 1.0f, 1.0f);
			glutWireCube(1.1);
			glColor3f(0.0f, 0.5f, 0.0f);
		glTranslatef(0.0, -1.0, 0.0);
		glutSolidCube(1.1);
			glColor3f(1.0f, 1.0f, 1.0f);
			glutWireCube(1.1);
			glColor3f(0.0f, 0.5f, 0.0f);
		glTranslatef(1.0, 0.0, 0.0);
		glutSolidCube(1.0);
			glColor3f(1.0f, 1.0f, 1.0f);
			glutWireCube(1.1);
            glColor3f(0.0f, 0.5f, 0.0f);
	glPopMatrix();
}

void PECA(){
	int kind;
	type = nextPiece;
	kind = rand()%7;
	if(kind == 0)
		nextPiece = 'l';
	else if(kind == 1)
		nextPiece = 'J';
	else if(kind == 2)
		nextPiece = 'L';
	else if(kind == 3)
		nextPiece = 'Q';
	else if(kind == 4)
		nextPiece = 'S';
	else if(kind == 5)
		nextPiece = 'T';
	else if(kind == 6)
		nextPiece = 'Z';
}

void ORIENTACAO(){
	int angleType;
	angleType = rand()%4;
	if(angleType == 0){
		rotationFull = 0;
		rotationHalf = 0;
	}
	if(angleType == 1){
		rotationFull = 90;
		rotationHalf = 90;
	}
	if(angleType == 2){
		rotationFull = 180;
		rotationHalf = 0;
	}
	if(angleType == 3){
		rotationFull = 270;
		rotationHalf = 90;
	}
}

void calculateScore(){
	score = score + 1;
	if(lvl < 9){
		lvl = (score/10)+1;
		speed = 1100 - lvl*100;
	}
}

void MOVER(int x){
	bool block = false, erase;
	for(int i = 0; i < 4; i++){
		if((coordenadas[i].y + downMovement + 1) >= DOWN_LIMIT
		|| game[(int)(coordenadas[i].y + downMovement) + 1][(int)(coordenadas[i].x + horizontalMovement)] == 1)
		block = true;
	}
	if(block == true){
		for(int i = 0; i < 4; i++){
			game[(int)(coordenadas[i].y + downMovement)][(int)(coordenadas[i].x + horizontalMovement)] = 1;
		}
		horizontalMovement = 0;
		downMovement = 0;
		rotationFull = 0;
		rotationHalf = 0;
		for(int i = DOWN_LIMIT-1; i >=0 ; i--){
			erase = true;
			for(int j = 0; j < HORIZONTAL_LIMIT; j++){
				if(game[i][j] != 1)
					erase = false;
			}
			if(erase){
				LINHA(game, i);
				calculateScore();
				i++;
			}
		}
///		ORIENTACAO();
		setCoordinates();
		for(int i = 0; i < 4; i++){
			if(game[(int)coordenadas[i].y][(int)coordenadas[i].x] == 1)
			gameOver();
		}
		PECA();
	}else{
		downMovement += 1;
	}
	glutPostRedisplay();
	glutTimerFunc(speed,MOVER,1);
}

void DefineIluminacao (void){
        GLfloat luzAmbiente[4]={0.2,0.2,0.2,1.0};
        GLfloat luzDifusa[4]={0.7,0.7,0.7,1.0};       /// "cor"
        GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0};/// "brilho"
        GLfloat posicaoLuz[4]={0.0, 1.0, 1.0, 1.0};

        /// Capacidade de brilho do material
        GLfloat especularidade[4]={0.5,1.0,1.0,1.0};
        GLint especMaterial = 60;

        /// Define a refletância do material
        glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
        /// Define a concentração do brilho
        glMateriali(GL_FRONT,GL_SHININESS,especMaterial);

        /// Ativa o uso da luz ambiente
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

        /// Define os parâmetros da luz de número 0
        glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
        glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
        glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );
}

void PosicionaObservador(void){
	/// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	/// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();
	/// Especifica posição do observador e do alvo
    DefineIluminacao();
	glTranslatef(0,10.0,0);
	glRotatef(rotX,1,0,0);
	glRotatef(rotY,0,1,0);
}

/// Função usada para especificar o volume de visualização
void EspecificaParametrosVisualizacao(void){
	/// Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	/// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();
	/// Especifica a projeção perspectiva(angulo,aspecto,zMin,zMax)
	gluPerspective(angle,fAspect,0.5,500);
	PosicionaObservador();
}

void LINHA(int ** matrix, int line){
	for(int i = line; i > 0 ; i--){
		for(int j = 0; j < HORIZONTAL_LIMIT; j++){
			matrix[i][j] = matrix[i-1][j];
		}
	}
	for(int i = 0; i < HORIZONTAL_LIMIT; i++){
		matrix[0][i] = 0;
	}
}

void write() {
	char maxPontuation[10];
	char level[2];
	char *stg = "Proxima:";
	char points[11];
	glPushMatrix();
        glColor3f(0.0, 0.0, 0.0);
		glRasterPos2f(0.9, 0.5);
		for(int i = 0; i < strlen(stg); i++){
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, stg[i]);
		}
		stg = "NIVEL:";
		strcpy(level, "");
		itoa(lvl, level, 10);
		glRasterPos2f(0.9, -2.3);
		for(int i = 0; i < strlen(stg); i++){
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, stg[i]);
		}
		for(int i = 0; i < level[i] != '\0'; i++){
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, level[i]);
		}
		stg = "PONTOS:";
		strcpy(points, "");
		itoa(score, points, 10);
		glRasterPos2f(0.9, -3.3);
		for(int i = 0; i < strlen(stg); i++){
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, stg[i]);
		}
		for(int i = 0; points[i] != '\0'; i++){
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, points[i]);
		}
		stg = "RECORDES:";
		strcpy(maxPontuation, "");
		itoa(maxScore, maxPontuation, 10);
		glRasterPos2f(0.9, -4.3);
		for(int i = 0; i < strlen(stg); i++){
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, stg[i]);
		}
		for(int i = 0; maxPontuation[i] != '\0'; i++){
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, maxPontuation[i]);
		}
	glPopMatrix();
}

void drawNextPiece(){

	glPushMatrix();
		glTranslatef(7.0, 6.0, -15.0);
		glPushMatrix();
			glScalef(6.0, 1.0, 1.0);
			glutSolidCube(1.0);
			glTranslatef(0.0, -7.0, 0.0);
			glutSolidCube(1.0);
		glPopMatrix();
		glTranslatef(0.0, -3.5, 0.0);
		glPushMatrix();
			glTranslatef(-3.5, 0.0, 0.0);
			glScalef(1.0, 8.0, 1.0);
			glutSolidCube(1.0);
			glTranslatef(7.0, 0.0, 0.0);
			glutSolidCube(1.0);
		glPopMatrix();
		glColor3f(0.0f, 0.5f, 0.0f);
		glTranslatef(-2.5, 0.0, 0.0);
		if(nextPiece == 'l'){
			glTranslatef(2.0, 0.0, 0.0);
			glTranslatef(-1.0, 0.0, 0.0);
			glutSolidCube(1.0);
			glColor3f(1.0f, 1.0f, 1.0f);
    			glColor3f(1.0f, 1.0f, 1.0f);
    			glutWireCube(1.1);
                glColor3f(0.0f, 0.5f, 0.0f);
            glTranslatef(1.0, 0.0, 0.0);
			glutSolidCube(1.0);
                glColor3f(1.0f, 1.0f, 1.0f);
    			glutWireCube(1.1);
                glColor3f(0.0f, 0.5f, 0.0f);
			glTranslatef(1.0, 0.0, 0.0);
			glutSolidCube(1.0);
    			glColor3f(1.0f, 1.0f, 1.0f);
    			glutWireCube(1.1);
                glColor3f(0.0f, 0.5f, 0.0f);
			glTranslatef(1.0, 0.0, 0.0);
			glutSolidCube(1.0);
			    glColor3f(1.0f, 1.0f, 1.0f);
                glutWireCube(1.1);
                glColor3f(0.0f, 0.5f, 0.0f);
		}
		else if(nextPiece == 'J'){
			glTranslatef(2.5, -0.5, 0.0);
			glTranslatef(-1.0, 1.0, 0.0);
			glutSolidCube(1.0);
                glColor3f(1.0f, 1.0f, 1.0f);
                glutWireCube(1.1);
                glColor3f(0.0f, 0.5f, 0.0f);
			glTranslatef(0.0, -1.0, 0.0);
			glutSolidCube(1.0);
    			glColor3f(1.0f, 1.0f, 1.0f);
    			glutWireCube(1.1);
                glColor3f(0.0f, 0.5f, 0.0f);
			glTranslatef(1.0, 0.0, 0.0);
			glutSolidCube(1.0);
    			glColor3f(1.0f, 1.0f, 1.0f);
    			glutWireCube(1.1);
                glColor3f(0.0f, 0.5f, 0.0f);
			glTranslatef(1.0, 0.0, 0.0);
			glutSolidCube(1.0);
    			glColor3f(1.0f, 1.0f, 1.0f);
    			glutWireCube(1.1);
                glColor3f(0.0f, 0.5f, 0.0f);
		}
		else if(nextPiece == 'L'){
			glTranslatef(2.5, -0.5, 0.0);
			glTranslatef(-1.0, 0.0, 0.0);
			glutSolidCube(1.0);
    			glColor3f(1.0f, 1.0f, 1.0f);
    			glutWireCube(1.1);
                glColor3f(0.0f, 0.5f, 0.0f);
			glTranslatef(1.0, 0.0, 0.0);
			glutSolidCube(1.0);
                glColor3f(1.0f, 1.0f, 1.0f);
    			glutWireCube(1.1);
                glColor3f(0.0f, 0.5f, 0.0f);
			glTranslatef(1.0, 0.0, 0.0);
			glutSolidCube(1.0);
    			glColor3f(1.0f, 1.0f, 1.0f);
    			glutWireCube(1.1);
                glColor3f(0.0f, 0.5f, 0.0f);
			glTranslatef(0.0, 1.0, 0.0);
			glutSolidCube(1.0);
    			glColor3f(1.0f, 1.0f, 1.0f);
    			glutWireCube(1.1);
                glColor3f(0.0f, 0.5f, 0.0f);
		}
		else if(nextPiece == 'Q'){
			glTranslatef(2.0, 0.5, 0.0);
			glutSolidCube(1.0);
    			glColor3f(1.0f, 1.0f, 1.0f);
    			glutWireCube(1.1);
                glColor3f(0.0f, 0.5f, 0.0f);
			glTranslatef(1.0, 0.0, 0.0);
			glutSolidCube(1.0);
    			glColor3f(1.0f, 1.0f, 1.0f);
    			glutWireCube(1.1);
                glColor3f(0.0f, 0.5f, 0.0f);
			glTranslatef(0.0, -1.0, 0.0);
			glutSolidCube(1.0);
    			glColor3f(1.0f, 1.0f, 1.0f);
    			glutWireCube(1.1);
                glColor3f(0.0f, 0.5f, 0.0f);
			glTranslatef(-1.0, 0.0, 0.0);
			glutSolidCube(1.0);
    			glColor3f(1.0f, 1.0f, 1.0f);
    			glutWireCube(1.1);
                glColor3f(0.0f, 0.5f, 0.0f);
		}
		else if(nextPiece == 'S'){
			glTranslatef(2.5, 0.5, 0.0);
			glutSolidCube(1.0);
    			glColor3f(1.0f, 1.0f, 1.0f);
    			glutWireCube(1.1);
                glColor3f(0.0f, 0.5f, 0.0f);
			glTranslatef(1.0, 0.0, 0.0);
			glutSolidCube(1.0);
    			glColor3f(1.0f, 1.0f, 1.0f);
    			glutWireCube(1.1);
                glColor3f(0.0f, 0.5f, 0.0f);
			glTranslatef(-1.0, -1.0, 0.0);
			glutSolidCube(1.0);
    			glColor3f(1.0f, 1.0f, 1.0f);
    			glutWireCube(1.1);
                glColor3f(0.0f, 0.5f, 0.0f);
			glTranslatef(-1.0, 0.0, 0.0);
			glutSolidCube(1.0);
    			glColor3f(1.0f, 1.0f, 1.0f);
    			glutWireCube(1.1);
                glColor3f(0.0f, 0.5f, 0.0f);
		}
		else if(nextPiece == 'T'){
			glTranslatef(2.5, -0.5, 0.0);
			glTranslatef(-1.0, 0.0, 0.0);
			glutSolidCube(1.0);
    			glColor3f(1.0f, 1.0f, 1.0f);
    			glutWireCube(1.1);
                glColor3f(0.0f, 0.5f, 0.0f);
			glTranslatef(1.0, 0.0, 0.0);
			glutSolidCube(1.0);
    			glColor3f(1.0f, 1.0f, 1.0f);
    			glutWireCube(1.1);
                glColor3f(0.0f, 0.5f, 0.0f);
			glTranslatef(1.0, 0.0, 0.0);
			glutSolidCube(1.0);
    			glColor3f(1.0f, 1.0f, 1.0f);
    			glutWireCube(1.1);
                glColor3f(0.0f, 0.5f, 0.0f);
			glTranslatef(-1.0, 1.0, 0.0);
			glutSolidCube(1.0);
    			glColor3f(1.0f, 1.0f, 1.0f);
    			glutWireCube(1.1);
                glColor3f(0.0f, 0.5f, 0.0f);
		}
		else if(nextPiece == 'Z'){
			glTranslatef(2.5, -0.5, 0.0);
			glTranslatef(-1.0, 1.0, 0.0);
			glutSolidCube(1.0);
    			glColor3f(1.0f, 1.0f, 1.0f);
    			glutWireCube(1.1);
                glColor3f(0.0f, 0.5f, 0.0f);
			glTranslatef(1.0, 0.0, 0.0);
			glutSolidCube(1.0);
    			glColor3f(1.0f, 1.0f, 1.0f);
    			glutWireCube(1.1);
                glColor3f(0.0f, 0.5f, 0.0f);
			glTranslatef(0.0, -1.0, 0.0);
			glutSolidCube(1.0);
    			glColor3f(1.0f, 1.0f, 1.0f);
    			glutWireCube(1.1);
                glColor3f(0.0f, 0.5f, 0.0f);
			glTranslatef(1.0, 0.0, 0.0);
			glutSolidCube(1.0);
    			glColor3f(1.0f, 1.0f, 1.0f);
    			glutWireCube(1.1);
                glColor3f(0.0f, 0.5f, 0.0f);
		}
	glPopMatrix();
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawNextPiece();
	write();
	glPushMatrix();
		glTranslatef(-6.0, 8.0, -15.0);
		glTranslatef(2.0, -DOWN_LIMIT/2, 0.0);
		glRotatef((GLfloat)angleX, 1.0, 0.0, 0.0);
		glRotatef((GLfloat)angleY, 0.0, 1.0, 0.0);
		glRotatef((GLfloat)angleZ, 0.0, 0.0, 1.0);
		glTranslatef(-2.0, DOWN_LIMIT/2, 0.0);
		DefineIluminacao();
		///CIMA
        glColor3f(0.0f, 0.0f, 1.0f);
		glPushMatrix();
			glTranslatef(1.5, 0.0, 0.0);
			glScalef(HORIZONTAL_LIMIT, 1.0, 1.0);

			glutSolidCube(1.0);
			glTranslatef(0.0, -DOWN_LIMIT - 1, 0.0);
                ///BAIXO
			    glColor3f(0.0f, 0.0f, 1.0f);
			glutSolidCube(1.0);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.0, -1.0, 0.0);
			///OBJETO
			glColor3f(0.0f, 0.5f, 0.0f);
			glPushMatrix();
			glTranslatef(-(HORIZONTAL_MIDDLE-2), 0.0, 0.0);
				for(int i = 0; i < DOWN_LIMIT; i++){
					for(int j = 0; j < HORIZONTAL_LIMIT; j++){
						if(game[i][j] == 1){
							glPushMatrix();
								glTranslatef((GLfloat)j, -(GLfloat)i, 0.0);
								glutSolidCube(1.0);
    								glColor3f(1.0f, 1.0f, 1.0f);
                        			glutWireCube(1.1);
                                    glColor3f(0.0f, 0.5f, 0.0f);
							glPopMatrix();
						}
					}
				}
			glPopMatrix();
			if(type == 'l')
				draw_l();
			else if(type == 'J')
				draw_J();
			else if(type == 'L')
				draw_L();
			else if(type == 'Q')
				draw_Q();
			else if(type == 'S')
				draw_S();
			else if(type == 'T')
				draw_T();
			else if(type == 'Z')
				draw_Z();
			setCoordinates();
		glPopMatrix();
		glPushMatrix();
            ///ESQUERDA
            glColor3f(0.0f, 0.0f, 1.0f);
			glTranslatef(1.0, -0.5, 0.0);
			glTranslatef(-HORIZONTAL_MIDDLE, -DOWN_LIMIT/2, 0.0);
			glScalef(1.0, 2 + DOWN_LIMIT, 1.0);

			glutSolidCube(1.0);
			glTranslatef(HORIZONTAL_LIMIT+1.0, 0.0, 0.0);
				///DIREITA
                glColor3f(0.0f, 0.0f, 1.0f);
			glutSolidCube(1.0);
		glPopMatrix();
	glPopMatrix();
	glutSwapBuffers();
}

void special(int key, int x, int y){
	bool block = false;
	switch (key) {
		case GLUT_KEY_UP:
			if(rotationHalf == 90)
				rotationHalf = 0;
			else
				rotationHalf = 90;

			if (rotationFull == 270)
				rotationFull = 0;
			else
				rotationFull = rotationFull + 90;

			setCoordinates();

			for(int i = 0; i < 4; i++){
				if((coordenadas[i].x + horizontalMovement) < 0
				|| (coordenadas[i].x + horizontalMovement) >= HORIZONTAL_LIMIT
				|| (coordenadas[i].y + downMovement) >= DOWN_LIMIT
				|| (coordenadas[i].y + downMovement) < 0
				|| game[(int)(coordenadas[i].y + downMovement)][(int)(coordenadas[i].x + horizontalMovement)] == 1){
					block = true;
					break;
				}
			}
			if(block){
				if(rotationHalf == 90)
					rotationHalf = 0;
				else
					rotationHalf = 90;
				if (rotationFull == 0)
					rotationFull = 270;
				else
					rotationFull = rotationFull - 90;
			}
			glutPostRedisplay();

			break;

		case GLUT_KEY_LEFT:
			for(int i = 0; i < 4; i++){
				if((coordenadas[i].x + horizontalMovement - 1) < 0
				|| game[(int)(coordenadas[i].y + downMovement)][(int)(coordenadas[i].x + horizontalMovement)-1] == 1)
					block = true;
			}
			if(block == true)
				break;
			horizontalMovement -= 1;
			glutPostRedisplay();
			break;

		case GLUT_KEY_RIGHT:
			for(int i = 0; i < 4; i++){
				if((coordenadas[i].x + horizontalMovement + 1) >= HORIZONTAL_LIMIT
				|| game[(int)(coordenadas[i].y + downMovement)][(int)(coordenadas[i].x + horizontalMovement)+1] == 1)
					block = true;
			}
			if(block == true)
				break;
			horizontalMovement += 1;
			glutPostRedisplay();
			break;

		case GLUT_KEY_DOWN:
			for(int i = 0; i < 4; i++){
				if((coordenadas[i].y + downMovement + 1) >= DOWN_LIMIT
				|| game[(int)(coordenadas[i].y + downMovement)+1][(int)(coordenadas[i].x + horizontalMovement)] == 1)
					block = true;
			}
			if(block == true)
				break;
			downMovement += 1;
			glutPostRedisplay();
			break;
		default:
			break;
	}
}

void reshape (int w, int h){
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective(65.0, (GLfloat) w/(GLfloat) h, 1.0, 40.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef (0.0, 0.0, -5.0);
  glTranslatef(0.0, 2.0, 0.0);
}

void keyboard (unsigned char key, int x, int y){
	switch (key) {
		case 27:
			game = LIBERAMATRIX(DOWN_LIMIT, HORIZONTAL_LIMIT, game);
			fclose(record);
			exit(0);
			break;
		default:
			break;
	}
}

int main(int argc, char** argv){
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (640, 480);
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("TETRIS");
	init ();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMainLoop();
	return 0;
}
