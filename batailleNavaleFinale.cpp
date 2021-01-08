#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <GL/GL.h>
#include "glut.h"
#include <vector>
#include "boardFinal.cpp"

using namespace std;

string Board::endGame = "Coulé ! Fin du jeu.";

Board botBoard("config1.txt");
Board playerBoard("config1.txt");

/// <summary>
/// \brief returns a vector of int corresponding to the color which has the initial of the entry
/// Returns (0,0,1) for 'b' (blue), (1,0,0) for 'r' (red) , (1,1,1) for 'w' (white) and (0,0,0) (black) for anything else.
/// Used to know if the case have been it and if there was a ship here
/// </summary>
/// <param name="c">Initial of the color</param>
/// <returns> Vector<int> of size 3 whith a rgb color</returns>
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
/// <summary>
/// \brief returns a vector of int corresponding to the type of the ship
/// Used to color the player ships on it's grid
/// </summary>
/// <param name="c">Number corresponding the ship</param>
/// <returns> Vector<int> of size 3 whith a rgb color</returns>
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

/// <summary>
/// \brief The callback for the resize of the window
/// </summary>
/// <param name="width">New width of the window</param>
/// <param name="height">New height of the window</param>
static void resize(int width, int height) //callback pour la taille de fenêtre
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1,10,-10,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}
/// <summary>
/// \brief The display callback for the computer window
/// Show initialy the grid with every case in blue, which become red or white when hit depending if there was a ship or not
/// </summary>
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
/// <summary>
/// \brief The callback for the display of the player grid
/// Display the grid of the player, all white except on cases where there is a ship, it's color depending on it's length
/// </summary>
static void displayallie() //gère l'affichage des alliés
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    for (int i=0;i<10;i++){
        for (int j=0;j<10;j++){
            if (playerBoard.couleurs[i][j] == 'r') {
                glColor3f(1, 0, 0);
            }
            else{
                vector<int> color = getColorbateau(playerBoard.grid[i][j]);
                glColor3f(color[0], color[1], color[2]);
            }
            glutSolidCube(0.5);
            glTranslated(1,0,0);
        }
        glTranslated(-10,0,0);
        glTranslated(0,-1,0);
    }

    glutSwapBuffers();
}

/// <summary>
/// \brief The callback for the keyboard, only 'q' to quit
/// </summary>
/// <param name="key">The key pressed</param>
/// <param name="x"> X coordinate of the mouse</param>
/// <param name="y">Y coordinate of the mouse</param>
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

/// <summary>
/// \brief Idle callback, it's the main loop to play
/// When called, trigger a round (a player and a computer attack) and checks for end of game
/// </summary>
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
/// <summary>
/// \brief Initialize everything in Glut
/// </summary>

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



