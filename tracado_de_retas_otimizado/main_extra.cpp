#include <GL/glut.h>
#include <iostream>
#include <cmath>

using namespace std;

void draw_pixel(float x, float y, float size)
{
    glBegin(GL_QUADS);
        glVertex2f(x,y);
        glVertex2f(x+size,y);
        glVertex2f(x+size,y+size);
        glVertex2f(x,y+size);
    glEnd();
}

void draw_pixel(float x, float y){
    draw_pixel(x,y,1);
}

void draw_circle(float x, float y, float r)
{
    for(int i = -r; i<r; i++){
        for(int j = -sqrt(r*r - i*i); j<sqrt(r*r - i*i); j++){
            draw_pixel(x+j,y+i);
        }
    }
}

void init() {
    glClearColor(0, 0, 0, 0);
}

void draw_line_wikipedia(int x0, int y0, int x1, int y1)
{
    if(x0 > x1){
        swap(x0,x1);
        swap(y0,y1);
    }

    int dx = x1 - x0;
    int dy = y1 - y0;
    int D = 2*dy - dx;
    int y = y0;

    for(int x = x0; x <= x1; x++)
    {
        draw_pixel(x,y);
        if(D > 0)
        {
            y = y + 1;
            D = D - 2*dx;
        }
        D = D + 2*dy;
    }
}

void draw_line(int x0, int y0, int x1, int y1)
{
    if(x0 > x1){
        swap(x0,x1);
        swap(y0,y1);
    }

    float dx = x1 - x0;
    float dy = y1 - y0;
    float B = y0 - x0*(dy/dx);

    float a = dy;
    float b = -dx;
    float c = B*dx;

    int y = y0;
    for(int x = x0; x <= x1; x++)
    {
        draw_pixel(x,y);
        float d = a*(x+1) + b*(y+.5) + c;
        if(d > 0)
        {
            y = y + 1;
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    int x0 = 0;
    int y0 = 0;
    int x1 = 200;
    int y1 = 100;

    glColor3f(1,0,0);
    draw_circle(x0,y0,10);

    glColor3f(0,0,1);
    draw_circle(x1,y1,10);

    glColor3f(0,1,0);
    draw_line(x0,y0,x1,y1);

    glFlush();
}

float WINDOW_WIDTH = 1;
float WINDOW_HEIGHT = 1;

void resize_window_to_custom(GLsizei screen_width, GLsizei screen_height) {
    // Target window size
    float window_width = 250;
    float window_height = 250;

    // Defines viewport
    glViewport(0, 0, screen_width, screen_height);

    // Initializes system coordinates
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Resizes window to match smallest dimension
    if (screen_width > screen_height){
        window_width *= (float) screen_width / screen_height;
    } else {
        window_height *= (float) screen_height / screen_width;
    }

    // Sets window coordinates
    gluOrtho2D(0.0f, window_width, 0.0f, window_height);

    // Sets global variables
    WINDOW_WIDTH = window_width;
    WINDOW_HEIGHT = window_height;
}

void resize_window_to_screen(GLsizei screen_width, GLsizei screen_height) {
    // Defines viewport
    glViewport(0, 0, screen_width, screen_height);

    // Initializes system coordinates
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Sets window coordinates
    //gluOrtho2D(0.0f, screen_width, 0.0f, screen_height);
    gluOrtho2D(-screen_width/2.0, screen_width/2.0, -screen_height/2.0, screen_height/2.0);

    // Sets global variables
    WINDOW_WIDTH = screen_width;
    WINDOW_HEIGHT = screen_height;
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(10, 50);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("Bresenham's Line Algorithm");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(resize_window_to_screen);
    glutMainLoop();
}
