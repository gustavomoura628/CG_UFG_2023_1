#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <unistd.h>
#include <cmath>
#include <vector>

using namespace std;

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;
float WINDOW_LEFT = -4;
float WINDOW_RIGHT = 4;
float WINDOW_TOP = 3;
float WINDOW_BOTTOM = -3;

// usado para alterar entre perspectiva e paralela ( 0 == paralelo, perspectiva == 1 )
int PROJECTION_TYPE = 0;


class Point{
    public:
        float x;
        float y;
        float z;

        Point()
        {
        }

        Point(float _x, float _y, float _z)
        {
            x = _x;
            y = _y;
            z = _z;
        }

        friend ostream& operator<<(ostream& os, const Point & point)
        {
            return os << "(" << point.x << ", " << point.y << ", " << point.z << ")";
        }

        // funcao de teste q so faz uma rotacao hardcoded em volta de um centro (cx,cy,cz)
        void rotate_around(float cx, float cy, float cz)
        {
            Point r = Point(0,0,0);
            r.x = (x-cx)*0.999998 + (y-cy)* -0.0019999987 + (z-cz)* 0;
            r.y = (x-cx)*0.0019995987 + (y-cy)* 0.9997980071 + (z-cz)* -0.0199986667;
            r.z = (x-cx)*0.0000399973 + (y-cy)* 0.0199986267 + (z-cz)* 0.9998000067;

            x = r.x + cx;
            y = r.y + cy;
            z = r.z + cz;
        }
};

// classe linha para ajudar no cubo
class Line{
    public:
        Point p0;
        Point p1;

        Line(Point _p0, Point _p1)
        {
            p0 = _p0;
            p1 = _p1;
        }

        friend ostream& operator<<(ostream& os, const Line line)
        {
            return os << "Line{" << line.p0 << ", " << line.p1 << "}";
        }

};

Point projecao_perspectiva(Point p)
{
    float d = 2;
    Point projetado = Point(p.x / (p.z / d + 1), p.y / (p.z / d + 1), 0);
    return projetado;
}

Point projecao_paralela(Point p)
{
    Point projetado = Point(p.x, p.y, 0);
    return projetado;
}

// traduz as coodenadas da janela do mundo para as coordenadas da tela
Point window_to_viewport(Point p)
{
    Point v = Point(0,0,0);
    v.x = (p.x - WINDOW_LEFT) / (WINDOW_RIGHT - WINDOW_LEFT) * (SCREEN_WIDTH);
    v.y = (p.y - WINDOW_BOTTOM) / (WINDOW_TOP - WINDOW_BOTTOM) * (SCREEN_HEIGHT);
    return v;
}

// classe cubo
class Cube{
    public:
        Point center;
        float radius;
        vector<Point> points;
        vector<Line> lines;
        Cube()
        {
        }

        // define os pontos e linhas do cubo
        Cube(Point _center, float _radius)
        {
            center = _center;
            radius = _radius;

            points.push_back(Point(center.x + radius, center.y + radius, center.z + radius));
            points.push_back(Point(center.x + radius, center.y + radius, center.z - radius));
            points.push_back(Point(center.x + radius, center.y - radius, center.z - radius));
            points.push_back(Point(center.x + radius, center.y - radius, center.z + radius));
            points.push_back(Point(center.x - radius, center.y - radius, center.z + radius));
            points.push_back(Point(center.x - radius, center.y - radius, center.z - radius));
            points.push_back(Point(center.x - radius, center.y + radius, center.z - radius));
            points.push_back(Point(center.x - radius, center.y + radius, center.z + radius));

            for(int i = 0; i < points.size(); i++)
            {
                lines.push_back(Line(points[i], points[(i+1)%points.size()]));
            }
            lines.push_back(Line(points[0], points[3]));
            lines.push_back(Line(points[2], points[5]));
            lines.push_back(Line(points[4], points[7]));
            lines.push_back(Line(points[6], points[1]));
        }

        void draw()
        {
            for(Line line: lines)
            {
                 glBegin(GL_LINES);

                    Point p0_projetado(0,0,0);
                    if(PROJECTION_TYPE == 1) p0_projetado = projecao_perspectiva(line.p0);
                    else p0_projetado = projecao_paralela(line.p0);
                    Point p0_viewport = window_to_viewport(p0_projetado);
                    glVertex2f(p0_viewport.x, p0_viewport.y);

                    Point p1_projetado(0,0,0);
                    if(PROJECTION_TYPE == 1) p1_projetado = projecao_perspectiva(line.p1);
                    else p1_projetado = projecao_paralela(line.p1);
                    Point p1_viewport = window_to_viewport(p1_projetado);
                    glVertex2f(p1_viewport.x, p1_viewport.y);

                glEnd();
            }
        }

        // funcao de teste q so faz uma rotacao hardcoded em volta do centro do cubo
        void rotate()
        {
            for(Point & point: points)
            {
                point.rotate_around(center.x,center.y,center.z);
            }

            for(Line & line: lines)
            {
                line.p0.rotate_around(center.x,center.y,center.z);
                line.p1.rotate_around(center.x,center.y,center.z);
            }
        }
};

void init() 
{
    glClearColor(0, 0, 0, 1);
}

// inicializa o cubo
Cube cube0 = Cube(Point(0,0,0.91),0.5);
Cube cube1 = Cube(Point(2,2,2),0.5);
Cube cube2 = Cube(Point(-2,-2,5),0.5);
Cube cube3 = Cube(Point(2,-2,20),0.5);
Cube cube4 = Cube(Point(-2,2,40),0.5);

void display() 
{

    glClear(GL_COLOR_BUFFER_BIT);

    // desenha o cubo
    glColor3f(1,0,0);
    cube0.draw();
    glColor3f(0,1,0);
    cube1.draw();
    glColor3f(0,0,1);
    cube2.draw();
    glColor3f(1,0,1);
    cube3.draw();
    glColor3f(1,1,0);
    cube4.draw();

    glFlush();
}



void resize_window_to_screen(GLsizei screen_width, GLsizei screen_height) 
{
    // Defines viewport
    glViewport(0, 0, screen_width, screen_height);

    // Initializes system coordinates
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();



    // Sets window coordinates so (0,0) is the bottom left corner and (screen_width,screen_height) the top right corner
    gluOrtho2D(0, screen_width, 0, screen_height);
    SCREEN_WIDTH = screen_width;
    SCREEN_HEIGHT = screen_height;
}

int COUNTER = 0;
void idle()
{
    // Draw every 1/60 seconds ( 60 fps )
    usleep(1000000/60);

    // roda o cubo
    cube0.rotate();
    cube1.rotate();
    cube2.rotate();
    cube3.rotate();
    cube4.rotate();

    // alterna entre perspectiva e paralela a cada 5 segundos
    if(COUNTER % (60*5) == 0)
    {
        PROJECTION_TYPE = 1 - PROJECTION_TYPE;
    }

    glutPostRedisplay();
    COUNTER++;
}


int main(int argc, char **argv) 
{
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(10, 50);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("Projecao Perspectiva e Projecao Paralela");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(resize_window_to_screen);
    glutIdleFunc(idle);
    glutMainLoop();
}
