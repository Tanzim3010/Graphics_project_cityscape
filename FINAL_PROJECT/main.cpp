#include <GL/glut.h>

void drawRectangle(float x1, float y1, float x2, float y2,
                   float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
    glEnd();
}

void drawSky()
{
    drawRectangle(0, 300, 1280, 720, 0.53f, 0.81f, 0.98f);
}

void drawGround()
{
    drawRectangle(0, 0, 1280, 300, 0.32f, 0.65f, 0.25f);
}

void drawBackgroundBuildings()
{
    drawRectangle(0,300,90,410,0.72f,0.72f,0.75f);
    drawRectangle(80,300,180,380,0.70f,0.70f,0.73f);
    drawRectangle(170,300,280,440,0.74f,0.74f,0.77f);
    drawRectangle(270,300,350,390,0.71f,0.71f,0.74f);
    drawRectangle(340,300,470,470,0.73f,0.73f,0.76f);
    drawRectangle(460,300,560,400,0.71f,0.71f,0.74f);
    drawRectangle(550,300,690,460,0.72f,0.72f,0.75f);
    drawRectangle(680,300,790,390,0.70f,0.70f,0.73f);
    drawRectangle(780,300,920,440,0.74f,0.74f,0.77f);
    drawRectangle(910,300,1020,410,0.71f,0.71f,0.74f);
    drawRectangle(1010,300,1140,480,0.73f,0.73f,0.76f);
    drawRectangle(1130,300,1280,400,0.71f,0.71f,0.74f);
}

void drawElevatedHighway()
{
    drawRectangle(0, 350, 1280, 370, 0.65f, 0.65f, 0.68f);

    for(int i = 20; i < 1280; i += 120) {
        drawRectangle(i, 300, i + 15, 350, 0.6f, 0.6f, 0.63f);
    }
}


void drawRoads()
{

    glColor3f(0.50f, 0.50f, 0.50f);
    glBegin(GL_QUADS);
        glVertex2f(0, 80);
        glVertex2f(120, 80);
        glVertex2f(250, 300);
        glVertex2f(200, 300);
    glEnd();


    glColor3f(0.45f, 0.45f, 0.45f);
    glBegin(GL_QUADS);
        glVertex2f(980, 0);
        glVertex2f(1250, 0);
        glVertex2f(820, 300);
        glVertex2f(720, 300);
    glEnd();
}

void drawGravelBed()
{
    glColor3f(0.55f, 0.55f, 0.55f);
    glBegin(GL_QUADS);
        glVertex2f(150, 0);
        glVertex2f(900, 0);
        glVertex2f(650, 300);
        glVertex2f(350, 300);
    glEnd();
}



void init()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glPointSize(5);
    gluOrtho2D(0, 1280, 0, 720);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Background
    drawSky();
    drawBackgroundBuildings();
    drawElevatedHighway();

    // Ground and Paths
    drawGround();
    drawRoads();
    drawGravelBed();

    glFlush();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("TrainTracks");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
