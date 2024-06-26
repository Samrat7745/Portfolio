#define _USE_MATH_DEFINES

#include <cstdlib>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

int fill = 1;

int menu_id = 1;
float a = 10.0, b = 20.0, c = 20.0, m = 0, n = 0.0, o = 0.0;

static float vertices_cube[] =
    {
        // Define vertices for a cube
        1.0, -1.0, 1.0,
        1.0, 1.0, 1.0,
        1.0, 1.0, -1.0,
        1.0, -1.0, -1.0,
        -1.0, -1.0, 1.0,
        -1.0, 1.0, 1.0,
        -1.0, 1.0, -1.0,
        -1.0, -1.0, -1.0};

// Indices that'll make a cube
static unsigned char cube_vertices[] = {5, 4, 1, 0, 2, 3, 6, 7, 5, 4};
static unsigned char cube_top[] = {5, 1, 6, 2};
static unsigned char cube_bottom[] = {0, 4, 3, 7};

void drawbody(float x, float y, float z, float s1, float s2, float s3, float c1, float c2, float c3)
{
    glPushMatrix();
    glColor3f(c1, c2, c3);
    glTranslatef(x, y, z);
    glScalef(s1, s2, s3);
    glDrawElements(GL_TRIANGLE_STRIP, 10, GL_UNSIGNED_BYTE, cube_vertices);
    glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_BYTE, cube_top);
    glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_BYTE, cube_bottom);
    glPopMatrix();
}

void drawlimb(float x, float y, float z, float s1, float s2, float s3, float c1, float c2, float c3)
{
    glPushMatrix();
    glColor3f(c1, c2, c3);
    glTranslatef(x, y, z);
    glScalef(s1, s2, s3);
    glDrawElements(GL_TRIANGLE_STRIP, 10, GL_UNSIGNED_BYTE, cube_vertices);
    glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_BYTE, cube_top);
    glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_BYTE, cube_bottom);
    glPopMatrix();
}
void eye(float R, float theta, float phi)
{
    // Polar to Axial Coods
    float x = R * cos(theta) * cos(phi);
    float y = R * sin(theta);
    float z = R * cos(theta) * sin(phi);
    gluLookAt(x, y, z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

// void leftarm(void) {

//  drawbody();
//}

void drawman(void)
{
    drawbody(0.0, 0.9, 0, 1, 0.5, 0.3, 0, 1, 0);
    drawlimb(-0.6, 0, 0, 0.2, 0.6, 0.2, 1, 0, 0);
    drawlimb(0.6, 0, 0, 0.2, 0.6, 0.2, 0, 0, 1);
    drawlimb(1.1, 0.9, 0, 0.2, 0.6, 0.2, 0, 1, 1);
    drawlimb(-1.1, 0.9, 0, 0.2, 0.6, 0.2, 1, 0, 1);
    drawbody(0.0, 1.6, 0, 0.5, 0.5, 0.2, 1, 1, 0);
    drawbody(0.3, 1.7, 0.2, 0.1, 0.1, 0.05, 1, 1, 1);
    drawbody(-0.3, 1.7, 0.2, 0.1, 0.1, 0.05, 1, 1, 1);
}

// Drawing routine.
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // gluLookAt(x, y, z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glColor3f(0.0, 0.0, 0.0);

    glLoadIdentity();

    eye(a, b, c);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices_cube);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    drawman();
    glDisable(GL_CULL_FACE);

    glutSwapBuffers();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 97:
        // For "a"
        c -= 0.1;
        glutPostRedisplay();
        break;
    case 100:
        // For "d"
        c += 0.1;
        glutPostRedisplay();
        break;
    case 115:
        // For "s"
        a += 0.1;
        glutPostRedisplay();
        break;
    case 119:
        // For "w"
        a -= 0.1;
        glutPostRedisplay();
        break;
    case 113:
        // For "q"
        b -= 0.1;
        glutPostRedisplay();
        break;
    case 101:
        // For "e"
        b += 0.1;
        glutPostRedisplay();
        break;
    case 27:
        exit(0);
        break;
    default:
        break;
    }
}

// The right button menu callback function.
void rightMenu(int id)
{
    if (id == 1)
    {
        menu_id = 2;
        a = 5.0;
        b = M_PI * 0.5;
        c = -M_PI * 0.5;
        glutPostRedisplay();
    }
    else if (id == 2)
    {
        menu_id = 1;
        a = 5.0;
        b = 0.0;
        c = M_PI * 0.5;
        glutPostRedisplay();
    }
    else if (id == 3)
    {
        menu_id = 3;
        a = 5.0;
        b = M_PI * 0.25;
        c = -M_PI * 0.25;
        glutPostRedisplay();
    }
    else if (id == 4)
    {
        exit(0);
    }
    glutPostRedisplay();
}

void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70, (float)w / (float)h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
    std::cout << "Interaction:" << std::endl;
    std::cout << "Left click on a box on the left to select a primitive." << std::endl
              << "Then left click on the drawing area: once for point, twice for line or rectangle." << std::endl
              << "Right click for menu options." << std::endl
              << "1.Perspective View top " << std::endl
              << "2.Perspective view front " << std::endl
              << "3.Diagonal view " << std::endl
              << "4. Quit" << std::endl;
}

void makeMenu(void)
{
    glutCreateMenu(rightMenu);
    glutAddMenuEntry("Perspective View top", 1);
    glutAddMenuEntry("Perspective view front", 2);
    glutAddMenuEntry("Diagonal view", 3);
    glutAddMenuEntry("Quit", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void setup(void)
{
    printInteraction();
    glClearColor(1.0, 1.0, 1.0, 0.0);

    makeMenu();
}

// Main routine.
int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
    glutInitWindowSize(750, 750);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("squareOfWalls.cpp");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);

    glewExperimental = GL_TRUE;
    glewInit();

    setup();

    glutMainLoop();
}