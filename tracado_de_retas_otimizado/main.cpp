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

class Point{
    public:
        int x;
        int y;
        Point(int _x, int _y){
            x = _x;
            y = _y;
        };
};

class Normalization{
    public:
        bool vertical_flip = false;
        bool diagonal_flip = false;
        Normalization(Point p1, Point p2){
            // Sorts points so that point 1 is the bottom one
            if(p1.y > p2.y){
                swap(p1.x,p2.x);
                swap(p1.y,p2.y);
            }

            if(p1.x > p2.x){
                vertical_flip = true;
            }

            p1 = flip_vertical(p1);
            p2 = flip_vertical(p2);

            if(abs(p1.x - p2.x) < abs(p1.y - p2.y)){
                diagonal_flip = true;
            }
        };

        Point flip_vertical(Point p){
            p.y = -p.y;
            return p;
        };

        Point flip_diagonal(Point p){
            swap(p.x,p.y);
            return p;
        };

        Point change(Point p){
            if(vertical_flip) p = flip_vertical(p);
            if(diagonal_flip) p = flip_diagonal(p);
            return p;
        };

        Point undo(Point p){
            if(diagonal_flip) p = flip_diagonal(p);
            if(vertical_flip) p = flip_vertical(p);
            return p;
        };
};


void draw_line(int x1, int y1, int x2, int y2)
{
    // Normalization
    Normalization norm(Point(x1,y1),Point(x2,y2));
    Point p1 = norm.change(Point(x1,y1));
    Point p2 = norm.change(Point(x2,y2));
    x1 = p1.x; y1 = p1.y;
    x2 = p2.x; y2 = p2.y;

    // Sorts points so that point 1 is the bottom one
    if(y1 > y2){
        swap(x1,x2);
        swap(y1,y2);
    }

    int dx, dy, incE, incNE, d, x, y;

    dx = x2 - x1;
    dy = y2 - y1;
    d = 2 * dy - dx; /* Valor inicial de d */
    incE = 2 * dy; /* Incremento de E */
    incNE = 2 * (dy - dx); /* Incremento de NE */
    x = x1;
    y = y1;
    Point p = norm.undo(Point(x,y));
    draw_pixel(p.x,p.y);
    while (x < x2){
        if (d <= 0){
            /* Escolhe E */
            d = d + incE;
            x = x + 1;
        }else{
            /* Escolhe NE */
            d = d + incNE;
            x = x + 1;
            y = y + 1;
        }/* end if */
        Point p = norm.undo(Point(x,y));
        draw_pixel(p.x,p.y);
    }/* end while */
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

    draw_line(x1,x2,MOUSE_X_ORTHO,MOUSE_Y_ORTHO);
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
