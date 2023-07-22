#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <unistd.h>
#include <cmath>
#include <vector>

using namespace std;

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;
int MOUSE_X = 0;
int MOUSE_X_ORTHO = 0;
int MOUSE_Y = 0;
int MOUSE_Y_ORTHO = 0;

void draw_pixel(float x, float y)
{
    glBegin(GL_QUADS);
        glVertex2f(x,y);
        glVertex2f(x+1,y);
        glVertex2f(x+1,y+1);
        glVertex2f(x,y+1);
    glEnd();
}

void CirclePoints(int x_centro, int y_centro, int x, int y)
{
    draw_pixel(x_centro + x, y_centro + y);
    draw_pixel(x_centro - x, y_centro + y);
    draw_pixel(x_centro - x, y_centro - y);
    draw_pixel(x_centro + x, y_centro - y);
    draw_pixel(x_centro + y, y_centro + x);
    draw_pixel(x_centro - y, y_centro + x);
    draw_pixel(x_centro - y, y_centro - x);
    draw_pixel(x_centro + y, y_centro - x);
}

void draw_circle(int x_centro, int y_centro, int r)
{
    int x, y, h, incE, incSE;
    //float d;

    /* Valores iniciais */
    x = 0;
    y = r;
    //d = 5/4.0 - r;
    h = 1 - r;
    incE = h + 2 * x + 3; /* Incremento de E */
    incSE = h + 2 * ( x - y ) + 5; /* Incremento de SE */
    CirclePoints(x_centro,y_centro,x,y);

    while (y > x){
        if (h < 0){
            /* Selecione E */
            h = incE;
            x++;
        }else{
            /* Selecione SE */
            h = incSE;
            x++;
            y--;
        }
        CirclePoints(x_centro, y_centro, x, y);
    }
}
void init() 
{
    glClearColor(0, 0, 0, 1);
}

void display() 
{

    glClear(GL_COLOR_BUFFER_BIT);

    //Draws red lines between POINTS
    glColor3f(1,0,0);
    int x1 = 10;
    int x2 = 20;

    draw_circle(MOUSE_X_ORTHO,MOUSE_Y_ORTHO,sqrt(MOUSE_X_ORTHO*MOUSE_X_ORTHO + MOUSE_Y_ORTHO*MOUSE_Y_ORTHO));
    //Draws yellow circles on POINTS
    glFlush();
}


void resize_window_to_screen(GLsizei screen_width, GLsizei screen_height) 
{
    // Defines viewport
    glViewport(0, 0, screen_width, screen_height);

    // Initializes system coordinates
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();



    // Sets window coordinates so (0,0) is its center and it has the same size as the screen
    gluOrtho2D(-screen_width/2.0, screen_width/2.0, -screen_height/2.0, screen_height/2.0);
    SCREEN_WIDTH = screen_width;
    SCREEN_HEIGHT = screen_height;
}

void idle()
{
    usleep(1000000/60);
    glutPostRedisplay();
}

void update_mouse_pos(int x, int y)
{
    MOUSE_X = x;
    MOUSE_Y = y;
    MOUSE_X_ORTHO = x - SCREEN_WIDTH/2;
    MOUSE_Y_ORTHO = -1*(y - SCREEN_HEIGHT/2);
}

int main(int argc, char **argv) 
{
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(10, 50);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("Bezier curve");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(resize_window_to_screen);
    glutIdleFunc(idle);
    glutMotionFunc(update_mouse_pos);
    glutPassiveMotionFunc(update_mouse_pos);
    glutMainLoop();
}
