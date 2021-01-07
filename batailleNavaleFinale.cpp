#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <GL/GL.h>
#include "glut.h"
#include <vector>
#include "board final.cpp"

using namespace std;

string Board::endGame = "Coulé ! Fin du jeu.";

Board botBoard("config1.txt");
Board playerBoard("config1.txt");


vector<int> getColortouche(char c){ //renvoie une couleur selon le charactère en entrée
    if (c=='b'){
        return {0,0,1};
    }
    if (c=='r'){
        return {1,0,0};
    }
    if (c=='w'){
        return {1,1,1};
    }
    return {0,0,0};
}

vector<int> getColorbateau(int c){ //renvoie une couleur selon le type de bateau
    switch(c){
    case(0):{
        return {1,1,1};
    }
    case(1):{
        return {1,0,0};
    }
    case (2):{
        return {0,1,0};
    }
    case (3):{
        return {0,0,1};
    }
    case (4):{
        return {1,1,0};
    }
    case (5):{
        return {0,1,1};
    }
    default:{
        return {0,0,0};
    }
    }
}


static void resize(int width, int height) //callback pour la taille de fenêtre
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1,10,-10,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

static void displayennemi() //gère l'affichage des ennemis
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    for (int i=0;i<10;i++){
        for (int j=0;j<10;j++){
            vector<int> color = getColortouche(botBoard.couleurs[i][j]);
            glColor3f(color[0],color[1],color[2]);
            glutSolidCube(0.5);
            glTranslated(1,0,0);
        }
        glTranslated(-10,0,0);
        glTranslated(0,-1,0);
    }

    glutSwapBuffers();
}

static void displayallie() //gère l'affichage des alliés
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    for (int i=0;i<10;i++){
        for (int j=0;j<10;j++){
            vector<int> color = getColorbateau(playerBoard.grid[i][j]);
            glColor3f(color[0],color[1],color[2]);
            glutSolidCube(0.5);
            glTranslated(1,0,0);
        }
        glTranslated(-10,0,0);
        glTranslated(0,-1,0);
    }

    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y) //callback du clavier
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;
    }

    glutPostRedisplay();
}

static void idle(void) 
{
    int choiceX; //chiffre
	int choiceY; //alphabet
	char Y;
	string playerMoveRes, computerMoveRes;
    		//Player turn
		cout << "C'est au joueur de jouer." << endl;
		cout << "Où tirer ? X(1-10) :";
		cin >> choiceX;
		choiceX--;
		cout << "Y(A-J):";
		cin >> Y;
		choiceY = Y - 'A';
		cout << choiceY;
		playerMoveRes = botBoard.hit(choiceX, choiceY);
		cout << playerMoveRes << endl;
		if (playerMoveRes == Board::endGame)
		{
			cout << "Gagné ! Bien joué." << endl;
		}
		//Bot turn
		cout << "C'est à l'ordinateur de jouer." << endl;
		cout << "Votre plateau est :" << endl;
		playerBoard.displayGrid();
		choiceX = rand() % 10;
		choiceY = rand() % 10;
		cout << "L'ordinateur tire en " << choiceX << "," << choiceY << " !" << endl;
		computerMoveRes = playerBoard.hit(choiceX, choiceY);
		cout << computerMoveRes << endl;
		if (computerMoveRes == Board::endGame)
		{
			cout << "Perdu ! Dommage." << endl;
		}
    glutPostRedisplay();
}

int main(int argc, char** argv) //main
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Grille adverse");
    glutInitWindowPosition(0, 0);
    glutReshapeFunc(resize);
    glutDisplayFunc(displayennemi);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    glutCreateWindow("Ma grille");
    glutInitWindowPosition(600, 600);
    glutReshapeFunc(resize);
    glutDisplayFunc(displayallie);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(0,0,0,0);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
	srand(time(NULL));
	playerBoard.placerplayer();
	botBoard.placerordi();
    glutMainLoop();
	return 0;
}



