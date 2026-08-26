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

void drawBush(float cx, float cy, float radiusX, float radiusY, float r, float g, float b)
{
    glPushMatrix();
    glTranslatef(cx, cy, 0.0f);
    glScalef(radiusX, radiusY, 1.0f);

    glColor3f(r, g, b);

    // Base Quad
    glBegin(GL_QUADS);
        glVertex2f(-1.0f, -1.0f);
        glVertex2f( 1.0f, -1.0f);
        glVertex2f( 1.0f,  1.0f);
        glVertex2f(-1.0f,  1.0f);
    glEnd();

    // Rotated Quad for an octagonal/rounder look
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(-1.0f, -1.0f);
        glVertex2f( 1.0f, -1.0f);
        glVertex2f( 1.0f,  1.0f);
        glVertex2f(-1.0f,  1.0f);
    glEnd();

    glPopMatrix();
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
    // Main Highway Deck
    drawRectangle(0, 350, 1280, 370, 0.65f, 0.65f, 0.68f);

    // Pillars
    for(int i = 20; i < 1280; i += 120) {
        drawRectangle(i, 300, i + 15, 350, 0.6f, 0.6f, 0.63f);
    }

    // Distant Lamp Posts (Aligned with pillars)
    for(int i = 26; i < 1280; i += 120) {
        // Post
        drawRectangle(i, 370, i + 2, 388, 0.45f, 0.45f, 0.48f);
        // T-shape lamp head
        drawRectangle(i - 4, 386, i + 6, 388, 0.45f, 0.45f, 0.48f);
    }
}

void drawRoads()
{
    // Right Main Road Only
    glColor3f(0.45f, 0.45f, 0.45f);
    glBegin(GL_QUADS);
        glVertex2f(940, 0);
        glVertex2f(1280, 0);
        glVertex2f(850, 300);
        glVertex2f(700, 300);
    glEnd();
}

void drawLeftScenery()
{
    glColor3f(0.48f, 0.48f, 0.48f);
    glBegin(GL_QUADS);
        glVertex2f(-10, 120);     // Bottom left
        glVertex2f(100, 0);       // Bottom right
        glVertex2f(230, 300);     // Top right (horizon)
        glVertex2f(150, 300);     // Top left (horizon)
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

void drawStraightTrack(float startX, float endX)
{
    float baseWidth = 35.0f;
    float endWidth = 5.0f;
    float baseRailThick = 4.0f;
    float endRailThick = 1.0f;

    // 1. Draw Sleepers
    glColor3f(0.3f, 0.25f, 0.2f);
    glBegin(GL_QUADS);
    for (float t = 0; t <= 1.0f; t += 0.04f) {
        float y1 = 300.0f * t;
        float sleeperHeight = 4.0f * (1.0f - t) + 1.0f;
        float y2 = y1 + sleeperHeight;

        float cx = startX + t * (endX - startX);
        float w = baseWidth * (1.0f - t) + endWidth * t;
        float sleeperLen = w * 1.3f;

        glVertex2f(cx - sleeperLen, y1);
        glVertex2f(cx + sleeperLen, y1);
        glVertex2f(cx + sleeperLen, y2);
        glVertex2f(cx - sleeperLen, y2);
    }
    glEnd();

    // 2. Draw Rails
    glColor3f(0.2f, 0.2f, 0.25f);
    glBegin(GL_QUADS);

    // Left Rail
    glVertex2f(startX - baseWidth - baseRailThick, 0);
    glVertex2f(startX - baseWidth + baseRailThick, 0);
    glVertex2f(endX - endWidth + endRailThick, 300);
    glVertex2f(endX - endWidth - endRailThick, 300);

    // Right Rail
    glVertex2f(startX + baseWidth - baseRailThick, 0);
    glVertex2f(startX + baseWidth + baseRailThick, 0);
    glVertex2f(endX + endWidth + endRailThick, 300);
    glVertex2f(endX + endWidth - endRailThick, 300);

    glEnd();
}

void drawAllTracks()
{
    drawStraightTrack(240, 390); // Far Left
    drawStraightTrack(430, 465); // Mid Left
    drawStraightTrack(620, 540); // Mid Right
    drawStraightTrack(810, 615); // Far Right
}

void drawTrackSignal()
{

    float xCenter = 516.0f;
    float yBase = 120.0f;

    // Angled Tripod Legs
    drawRectangle(xCenter - 6.0f, yBase, xCenter - 2.0f, yBase + 10.0f, 0.45f, 0.45f, 0.45f);
    drawRectangle(xCenter + 2.0f, yBase, xCenter + 6.0f, yBase + 10.0f, 0.45f, 0.45f, 0.45f);

    // Main Vertical Pole
    drawRectangle(xCenter - 2.0f, yBase, xCenter + 2.0f, yBase + 85.0f, 0.5f, 0.5f, 0.5f);

    // Tall Black Signal Box
    drawRectangle(xCenter - 7.0f, yBase + 60.0f, xCenter + 7.0f, yBase + 95.0f, 0.15f, 0.15f, 0.15f);

    // Signal Lights
    // Red Light
    drawRectangle(xCenter - 2.5f, yBase + 82.0f, xCenter + 2.5f, yBase + 88.0f, 0.9f, 0.2f, 0.2f);
    // Green Light
    drawRectangle(xCenter - 2.5f, yBase + 68.0f, xCenter + 2.5f, yBase + 74.0f, 0.2f, 0.3f, 0.2f);
}

void drawLake()
{
    glColor3f(0.25f, 0.65f, 0.85f);
    glBegin(GL_POLYGON);
        glVertex2f(1080, 180);
        glVertex2f(1280, 160);
        glVertex2f(1280, 260);
        glVertex2f(930,  260);
    glEnd();
}

void drawRightRoadLamps()
{

    // Lamp 1 (Closest)
    // Pole
    drawRectangle(895.75f, 50.0f, 900.0f, 177.5f, 0.35f, 0.35f, 0.35f);
    // Arm
    drawRectangle(900.0f, 173.25f, 933.8f, 177.5f, 0.35f, 0.35f, 0.35f);
    // Light
    drawRectangle(925.3f, 171.125f, 933.8f, 177.5f, 1.0f, 0.9f, 0.6f);

    // Lamp 2
    // Pole
    drawRectangle(840.8f, 120.0f, 844.0f, 216.0f, 0.35f, 0.35f, 0.35f);
    // Arm
    drawRectangle(844.0f, 212.8f, 869.2f, 216.0f, 0.35f, 0.35f, 0.35f);
    // Light
    drawRectangle(862.8f, 211.2f, 869.2f, 216.0f, 1.0f, 0.9f, 0.6f);

    // Lamp 3
    // Pole
    drawRectangle(793.7f, 180.0f, 796.0f, 249.0f, 0.35f, 0.35f, 0.35f);
    // Arm
    drawRectangle(796.0f, 246.7f, 813.8f, 249.0f, 0.35f, 0.35f, 0.35f);
    // Light
    drawRectangle(809.2f, 245.55f, 813.8f, 249.0f, 1.0f, 0.9f, 0.6f);

    // Lamp 4
    // Pole
    drawRectangle(754.45f, 230.0f, 756.0f, 276.5f, 0.35f, 0.35f, 0.35f);
    // Arm
    drawRectangle(756.0f, 274.95f, 767.6f, 276.5f, 0.35f, 0.35f, 0.35f);
    // Light
    drawRectangle(764.5f, 274.175f, 767.6f, 276.5f, 1.0f, 0.9f, 0.6f);

    // Lamp 5
    // Pole
    drawRectangle(727.0f, 265.0f, 728.0f, 295.8f, 0.35f, 0.35f, 0.35f);
    // Arm
    drawRectangle(728.0f, 294.8f, 735.3f, 295.8f, 0.35f, 0.35f, 0.35f);
    // Light
    drawRectangle(733.3f, 294.3f, 735.3f, 295.8f, 1.0f, 0.9f, 0.6f);

    // Lamp 6 (Farthest)
    // Pole
    drawRectangle(711.275f, 285.0f, 712.0f, 306.75f, 0.35f, 0.35f, 0.35f);
    // Arm
    drawRectangle(712.0f, 306.025f, 716.85f, 306.75f, 0.35f, 0.35f, 0.35f);
    // Light
    drawRectangle(715.4f, 305.66f, 716.85f, 306.75f, 1.0f, 0.9f, 0.6f);
}

void drawLeftRoadLamps()
{


    // Lamp 1 (Closest)
    // Pole
    drawRectangle(121.6f, 50.0f, 125.85f, 177.5f, 0.35f, 0.35f, 0.35f);
    // Arm
    drawRectangle(87.8f, 173.25f, 121.6f, 177.5f, 0.35f, 0.35f, 0.35f);
    // Light
    drawRectangle(87.8f, 171.125f, 96.3f, 177.5f, 1.0f, 0.9f, 0.6f);

    // Lamp 2
    // Pole
    drawRectangle(152.0f, 120.0f, 155.2f, 216.0f, 0.35f, 0.35f, 0.35f);
    // Arm
    drawRectangle(126.8f, 212.8f, 152.0f, 216.0f, 0.35f, 0.35f, 0.35f);
    // Light
    drawRectangle(126.8f, 211.2f, 133.2f, 216.0f, 1.0f, 0.9f, 0.6f);

    // Lamp 3
    // Pole
    drawRectangle(178.0f, 180.0f, 180.3f, 249.0f, 0.35f, 0.35f, 0.35f);
    // Arm
    drawRectangle(160.2f, 246.7f, 178.0f, 249.0f, 0.35f, 0.35f, 0.35f);
    // Light
    drawRectangle(160.2f, 245.55f, 164.8f, 249.0f, 1.0f, 0.9f, 0.6f);

    // Lamp 4
    // Pole
    drawRectangle(199.6f, 230.0f, 201.15f, 276.5f, 0.35f, 0.35f, 0.35f);
    // Arm
    drawRectangle(188.0f, 274.95f, 199.6f, 276.5f, 0.35f, 0.35f, 0.35f);
    // Light
    drawRectangle(188.0f, 274.175f, 191.1f, 276.5f, 1.0f, 0.9f, 0.6f);

    // Lamp 5
    // Pole
    drawRectangle(214.8f, 265.0f, 215.8f, 295.8f, 0.35f, 0.35f, 0.35f);
    // Arm
    drawRectangle(207.5f, 294.8f, 214.8f, 295.8f, 0.35f, 0.35f, 0.35f);
    // Light
    drawRectangle(207.5f, 294.3f, 209.5f, 295.8f, 1.0f, 0.9f, 0.6f);

    // Lamp 6 (Farthest)
    // Pole
    drawRectangle(223.5f, 285.0f, 224.225f, 306.75f, 0.35f, 0.35f, 0.35f);
    // Arm
    drawRectangle(218.65f, 306.025f, 223.5f, 306.75f, 0.35f, 0.35f, 0.35f);
    // Light
    drawRectangle(218.65f, 305.66f, 220.1f, 306.75f, 1.0f, 0.9f, 0.6f);
}

void drawFoliage()
{
    // 1. New Foliage Block: Filling the top-left space behind the road
    drawBush(10, 290, 45, 30, 0.14f, 0.36f, 0.14f);
    drawBush(60, 285, 40, 25, 0.12f, 0.33f, 0.12f);
    drawBush(110, 295, 35, 20, 0.15f, 0.38f, 0.15f);

    drawBush(0, 250, 45, 35, 0.16f, 0.39f, 0.16f);
    drawBush(50, 260, 40, 30, 0.14f, 0.37f, 0.14f);
    drawBush(90, 255, 35, 25, 0.12f, 0.34f, 0.12f);

    drawBush(0, 200, 50, 40, 0.15f, 0.38f, 0.15f);
    drawBush(40, 215, 45, 35, 0.17f, 0.40f, 0.17f);
    drawBush(80, 205, 40, 30, 0.13f, 0.35f, 0.13f);

    drawBush(0, 150, 55, 45, 0.14f, 0.36f, 0.14f);
    drawBush(35, 165, 45, 35, 0.12f, 0.34f, 0.12f);
    drawBush(70, 145, 40, 30, 0.16f, 0.39f, 0.16f);

    drawBush(0, 100, 60, 50, 0.13f, 0.35f, 0.13f);
    drawBush(25, 115, 50, 40, 0.15f, 0.38f, 0.15f);

    // 2. Bottom-left corner
    drawBush(20, 30, 70, 60, 0.13f, 0.35f, 0.13f);
    drawBush(80, 60, 60, 55, 0.14f, 0.36f, 0.14f);
    drawBush(140, 40, 55, 50, 0.12f, 0.33f, 0.12f);


    // Main line
    drawBush(140, 90, 45, 50, 0.16f, 0.39f, 0.16f);
    drawBush(165, 130, 40, 45, 0.13f, 0.35f, 0.13f);
    drawBush(190, 170, 35, 40, 0.15f, 0.38f, 0.15f);
    drawBush(220, 210, 30, 35, 0.17f, 0.40f, 0.17f);
    drawBush(250, 245, 25, 30, 0.14f, 0.36f, 0.14f);
    drawBush(275, 270, 20, 25, 0.16f, 0.39f, 0.16f);
    drawBush(300, 290, 15, 18, 0.13f, 0.35f, 0.13f);

    // Secondary line
    drawBush(180, 100, 40, 45, 0.15f, 0.38f, 0.15f);
    drawBush(205, 145, 35, 40, 0.14f, 0.36f, 0.14f);
    drawBush(235, 185, 30, 35, 0.16f, 0.39f, 0.16f);
    drawBush(265, 225, 25, 30, 0.13f, 0.35f, 0.13f);
    drawBush(290, 260, 20, 22, 0.15f, 0.38f, 0.15f);
    drawBush(315, 285, 15, 15, 0.14f, 0.36f, 0.14f);


    // Main thick line
    drawBush(935, 10,  45, 65, 0.13f, 0.35f, 0.13f);
    drawBush(910, 35,  45, 60, 0.16f, 0.39f, 0.16f);
    drawBush(875, 70,  40, 55, 0.14f, 0.36f, 0.14f);
    drawBush(835, 110, 40, 50, 0.18f, 0.42f, 0.18f);
    drawBush(805, 150, 35, 45, 0.15f, 0.38f, 0.15f);
    drawBush(780, 180, 32, 40, 0.12f, 0.34f, 0.12f);
    drawBush(755, 210, 28, 35, 0.17f, 0.40f, 0.17f);
    drawBush(735, 235, 25, 30, 0.14f, 0.36f, 0.14f);
    drawBush(715, 255, 22, 26, 0.16f, 0.39f, 0.16f);
    drawBush(695, 275, 18, 22, 0.13f, 0.35f, 0.13f);
    drawBush(680, 288, 14, 18, 0.15f, 0.38f, 0.15f);

    // Secondary inner
    drawBush(940, 20,  45, 55, 0.15f, 0.38f, 0.15f);
    drawBush(910, 50,  42, 50, 0.12f, 0.34f, 0.12f);
    drawBush(880, 90,  38, 45, 0.17f, 0.40f, 0.17f);
    drawBush(850, 130, 32, 40, 0.13f, 0.35f, 0.13f);
    drawBush(820, 165, 28, 35, 0.16f, 0.39f, 0.16f);
    drawBush(795, 195, 25, 30, 0.14f, 0.36f, 0.14f);
    drawBush(770, 225, 22, 26, 0.15f, 0.38f, 0.15f);
    drawBush(745, 245, 18, 22, 0.18f, 0.42f, 0.18f);
    drawBush(725, 265, 15, 18, 0.14f, 0.36f, 0.14f);
    drawBush(705, 282, 12, 15, 0.16f, 0.39f, 0.16f);
    drawBush(690, 292, 10, 12, 0.13f, 0.35f, 0.13f);

    // 5. Lining the EXACT right edge of the main road
    drawBush(1260, 40, 40, 60, 0.13f, 0.35f, 0.13f);
    drawBush(1225, 70, 38, 55, 0.14f, 0.37f, 0.14f);
    drawBush(1190, 100, 35, 50, 0.15f, 0.38f, 0.15f);
    drawBush(1155, 125, 32, 45, 0.13f, 0.35f, 0.13f);
    drawBush(1120, 150, 30, 40, 0.12f, 0.33f, 0.12f);
    drawBush(1080, 175, 27, 37, 0.15f, 0.38f, 0.15f);
    drawBush(1040, 200, 25, 35, 0.16f, 0.39f, 0.16f);
    drawBush(1000, 225, 22, 32, 0.13f, 0.35f, 0.13f);
    drawBush(965,  250, 20, 25, 0.14f, 0.37f, 0.14f);
    drawBush(942,  262, 19, 22, 0.16f, 0.39f, 0.16f);
    drawBush(920,  275, 18, 20, 0.15f, 0.38f, 0.15f);
    drawBush(905,  285, 15, 16, 0.12f, 0.33f, 0.12f);
    drawBush(890,  295, 12, 12, 0.13f, 0.35f, 0.13f);

    // 6. Far Right Proper Lake Area
    drawBush(960, 290, 30, 20, 0.13f, 0.35f, 0.13f);
    drawBush(1050, 285, 40, 25, 0.15f, 0.38f, 0.15f);
    drawBush(1150, 280, 45, 30, 0.16f, 0.39f, 0.16f);
    drawBush(1250, 290, 35, 20, 0.14f, 0.36f, 0.14f);

    // Bushes overlapping the back edge of the lake
    drawBush(1000, 265, 25, 20, 0.14f, 0.37f, 0.14f);
    drawBush(1100, 255, 35, 25, 0.16f, 0.39f, 0.16f);
    drawBush(1200, 260, 45, 25, 0.12f, 0.34f, 0.12f);

    // Bottom right corner framing
    drawBush(1290, 40, 80, 90, 0.12f, 0.32f, 0.12f);

    // Front Lake framing foliage
    drawBush(1150, 170, 40, 35, 0.13f, 0.35f, 0.13f);
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

    // Background Layer
    drawSky();
    drawBackgroundBuildings();
    drawElevatedHighway();

    // Ground and Paths Layer
    drawGround();
    drawRoads();
    drawGravelBed();

    // Train Tracks & Signals
    drawAllTracks();
    drawTrackSignal();

    drawLeftScenery();

    // Lake Layer
    drawLake();

    // Draw the Road Lamps
    drawRightRoadLamps();
    drawLeftRoadLamps();

    // Foliage Layer covers the bases
    drawFoliage();

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
