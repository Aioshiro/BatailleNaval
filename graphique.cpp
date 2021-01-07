#include <windows.h>
#include <GL/glut.h>
#include <vector>
using namespace std;

#include <stdlib.h>

class board{
public :
    vector<vector<char>> couleurs;
    int taille=10;
    board();
};

board::board(){
    couleurs.resize(taille);
    for (int i=0;i<taille;i++){
        couleurs[i].resize(taille);
        for (int j=0;j<taille;j++){
            couleurs[i][j]='b';
        }
    }
}

board grille;

vector<int> getColortouche(char c){
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

vector<int> getColorbateau(int c){
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

static void resize(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1,10,-1,10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

static void displayennemi()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    for (int i=0;i<grille.taille;i++){
        for (int j=0;j<grille.taille;j++){
            vector<int> color = getColortouche(grille.couleurs[i][j]);
            glColor3f(color[0],color[1],color[2]);
            glutSolidCube(0.5);
            glTranslated(1,0,0);
        }
        glTranslated(-grille.taille,0,0);
        glTranslated(0,1,0);
    }

    glutSwapBuffers();
}

static void displayallie()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    for (int i=0;i<grille.taille;i++){
        for (int j=0;j<grille.taille;j++){
            vector<int> color = getColorbateau(grille.couleurs[i][j]);
            glColor3f(color[0],color[1],color[2]);
            glutSolidCube(0.5);
            glTranslated(1,0,0);
        }
        glTranslated(-grille.taille,0,0);
        glTranslated(0,1,0);
    }

    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
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
    glutPostRedisplay();
}

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Grille adverse");
    glutReshapeFunc(resize);
    glutDisplayFunc(displayennemi);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    glutCreateWindow("Ma grille");
    glutReshapeFunc(resize);
    glutDisplayFunc(displayallie);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(0,0,0,0);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);


    glutMainLoop();

    return EXIT_SUCCESS;
}
