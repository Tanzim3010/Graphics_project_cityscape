#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>

// 1. Expressway Cars
struct Car {
    float x;
    float y;
    float speed;
    int dir;
    float r, g, b;
};

const int NUM_CARS = 4;
Car cars[NUM_CARS] = {
    {1280, 371, 2.5f, -1, 0.8f, 0.2f, 0.2f},
    {-100, 371, 3.2f,  1, 0.2f, 0.3f, 0.8f},
    {2080, 371, 2.8f, -1, 0.9f, 0.9f, 0.9f},
    {-400, 371, 3.5f,  1, 0.8f, 0.4f, 0.1f}
};

// 2. Left Road cars
struct LeftRoadCar {
    float t;
    float speed;
    int dir;
    float r, g, b;
};

const int NUM_LEFT_CARS = 6;
LeftRoadCar leftCars[NUM_LEFT_CARS] = {
    {0.9f, 0.012f,  1, 0.8f, 0.2f, 0.2f},
    {0.5f, 0.012f,  1, 0.2f, 0.3f, 0.8f},
    {0.1f, 0.012f,  1, 0.9f, 0.9f, 0.9f},

    {0.1f, 0.012f, -1, 0.8f, 0.8f, 0.2f},
    {0.5f, 0.012f, -1, 0.2f, 0.8f, 0.2f},
    {0.9f, 0.012f, -1, 0.1f, 0.1f, 0.1f}
};

// 3. Right Road Vehicles
struct RightRoadVehicle {
    float t;
    float speed;
    int dir;
    int type;
    float r, g, b;
};

const int NUM_RIGHT_VEHICLES = 4;
RightRoadVehicle rightVehicles[NUM_RIGHT_VEHICLES] = {
    {0.8f, 0.010f,  1, 1, 0.8f, 0.15f, 0.15f},
    {0.2f, 0.010f,  1, 0, 0.9f, 0.9f,  0.9f},
    {0.4f, 0.010f, -1, 1, 0.2f, 0.6f,  0.3f},
    {0.9f, 0.010f, -1, 0, 0.2f, 0.3f,  0.8f}
};

// 4. Single Train Controller
int activeTrainTrack = 0;
bool activeTrainApproaching = false;
float activeTrainT = 1.5f; // Start safely off-screen
float trainWaitTimer = 2.0f; // Initial wait before first train spawns


float getRandomFloat(float min, float max) {
    return min + ((float)rand() / (float)RAND_MAX) * (max - min);
}

// --- TRAIN PERSPECTIVE HELPERS ---

float getTrainX(float t, int track) {
    if (track == 0) return 390.0f * (1.0f - t) + 240.0f * t; // Far Left Track
    if (track == 1) return 465.0f * (1.0f - t) + 430.0f * t; // Mid Left Track
    if (track == 2) return 540.0f * (1.0f - t) + 620.0f * t; // Mid Right Track
    if (track == 3) return 615.0f * (1.0f - t) + 810.0f * t; // Far Right Track
    return 0.0f;
}
float getTrainY(float t) { return 300.0f * (1.0f - t); }
float getTrainScale(float t) { return 0.25f + 0.75f * t; }


float clampT(float t) { return (t < 0.01f) ? 0.01f : t; }

void getTrainPoint(float t_car, float v_height, float h_offset, float &outX, float &outY, int track) {
    float scale = getTrainScale(t_car);
    outX = getTrainX(t_car, track) + h_offset * scale;
    outY = getTrainY(t_car) + v_height * scale;
}

void drawDistortedQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glVertex2f(x3, y3);
        glVertex2f(x4, y4);
    glEnd();
}

void drawTrainSideRect(float t_far, float t_near, float h_offset, float v_bottom, float v_top, float r, float g, float b, int track) {
    float x_f_b, y_f_b, x_n_b, y_n_b, x_n_t, y_n_t, x_f_t, y_f_t;
    getTrainPoint(t_far, v_bottom, h_offset, x_f_b, y_f_b, track);
    getTrainPoint(t_near, v_bottom, h_offset, x_n_b, y_n_b, track);
    getTrainPoint(t_near, v_top, h_offset, x_n_t, y_n_t, track);
    getTrainPoint(t_far, v_top, h_offset, x_f_t, y_f_t, track);
    drawDistortedQuad(x_f_b, y_f_b, x_n_b, y_n_b, x_n_t, y_n_t, x_f_t, y_f_t, r, g, b);
}

void drawTrainFrontRect(float t_near, float h_left, float h_right, float v_bottom, float v_top, float r, float g, float b, int track) {
    float xl_b, yl_b, xr_b, yr_b, xl_t, yl_t, xr_t, yr_t;
    getTrainPoint(t_near, v_bottom, h_left, xl_b, yl_b, track);
    getTrainPoint(t_near, v_bottom, h_right, xr_b, yr_b, track);
    getTrainPoint(t_near, v_top, h_right, xr_t, yr_t, track);
    getTrainPoint(t_near, v_top, h_left, xl_t, yl_t, track);
    drawDistortedQuad(xl_b, yl_b, xr_b, yr_b, xr_t, yr_t, xl_t, yl_t, r, g, b);
}

void drawTrainRoofRect(float t_far, float t_near, float h_left, float h_right, float v_top, float r, float g, float b, int track) {
    float xl_far, yl_far, xr_far, yr_far, xl_near, yl_near, xr_near, yr_near;
    getTrainPoint(t_far, v_top, h_left, xl_far, yl_far, track);
    getTrainPoint(t_far, v_top, h_right, xr_far, yr_far, track);
    getTrainPoint(t_near, v_top, h_right, xr_near, yr_near, track);
    getTrainPoint(t_near, v_top, h_left, xl_near, yl_near, track);
    drawDistortedQuad(xl_far, yl_far, xr_far, yr_far, xr_near, yr_near, xl_near, yl_near, r, g, b);
}

// --- TRAIN CAR DESIGNS ---

void drawLocomotive(float t_far, float t_near, int track, bool isFacingViewer) {
    float hw = 40.0f;
    float h = 100.0f;
    // Tracks on the left side of the screen see the right face (+hw). Tracks on the right see the left face (-hw).
    float side_hw = (track < 2) ? hw : -hw;

    // Side Faces
    drawTrainSideRect(t_far, t_near, side_hw, 2, 18, 0.1f, 0.1f, 0.1f, track);     // Undercarriage
    drawTrainSideRect(t_far, t_near, side_hw, 18, 35, 0.85f, 0.15f, 0.15f, track); // Red lower
    drawTrainSideRect(t_far, t_near, side_hw, 35, 60, 0.95f, 0.85f, 0.1f, track);  // Yellow mid
    drawTrainSideRect(t_far, t_near, side_hw, 60, h, 0.1f, 0.45f, 0.85f, track);   // Blue top

    // Windows
    float win_far = isFacingViewer ? t_near - 0.05f : t_far + 0.01f;
    float win_near = isFacingViewer ? t_near - 0.01f : t_far + 0.05f;
    drawTrainSideRect(win_far, win_near, side_hw, 65, 85, 0.1f, 0.15f, 0.2f, track);

    // Roof
    drawTrainRoofRect(t_far, t_near, -hw, hw, h, 0.3f, 0.3f, 0.3f, track);

    if (isFacingViewer) {
        // Full Front Face
        drawTrainFrontRect(t_near, -hw, hw, 2, 18, 0.1f, 0.1f, 0.1f, track);
        drawTrainFrontRect(t_near, -hw, hw, 18, 35, 0.85f, 0.15f, 0.15f, track);
        drawTrainFrontRect(t_near, -hw, hw, 35, 60, 0.95f, 0.85f, 0.1f, track);
        drawTrainFrontRect(t_near, -hw, hw, 60, h, 0.1f, 0.45f, 0.85f, track);

        drawTrainFrontRect(t_near, -30, -6, 65, 85, 0.1f, 0.15f, 0.2f, track);
        drawTrainFrontRect(t_near, 6, 30, 65, 85, 0.1f, 0.15f, 0.2f, track);

        drawTrainFrontRect(t_near, -12, 12, 38, 56, 0.85f, 0.15f, 0.15f, track);
        drawTrainFrontRect(t_near, -8, 8, 42, 52, 0.95f, 0.95f, 0.95f, track);
        drawTrainFrontRect(t_near, -6, 6, 88, 94, 1.0f, 1.0f, 0.8f, track);
    } else {
        // Back Connection (When driving away)
        drawTrainFrontRect(t_near, -hw, hw, 18, h, 0.1f, 0.4f, 0.2f, track);
        drawTrainFrontRect(t_near, -24, 24, 18, h-5, 0.1f, 0.1f, 0.1f, track);
    }
}

void drawCoach(float t_far, float t_near, int track, bool isLastCoach) {
    float hw = 40.0f;
    float h = 100.0f;
    float side_hw = (track < 2) ? hw : -hw;

    // Side Faces
    drawTrainSideRect(t_far, t_near, side_hw, 2, 18, 0.1f, 0.1f, 0.1f, track);
    drawTrainSideRect(t_far, t_near, side_hw, 18, h, 0.15f, 0.6f, 0.25f, track);

    // Stripes
    drawTrainSideRect(t_far, t_near, side_hw, 30, 35, 0.95f, 0.95f, 0.95f, track);
    drawTrainSideRect(t_far, t_near, side_hw, 85, 90, 0.95f, 0.95f, 0.95f, track);

    // Doors
    float dt = t_near - t_far;
    drawTrainSideRect(t_far + 0.02f*dt, t_far + 0.15f*dt, side_hw, 18, 90, 0.85f, 0.15f, 0.15f, track);
    drawTrainSideRect(t_near - 0.15f*dt, t_near - 0.02f*dt, side_hw, 18, 90, 0.85f, 0.15f, 0.15f, track);

    // Windows
    int num_windows = 6;
    for(int i = 1; i <= num_windows; i++) {
        float wt_far = t_far + dt * (0.15f + 0.70f * (i - 0.8f) / num_windows);
        float wt_near = t_far + dt * (0.15f + 0.70f * (i - 0.2f) / num_windows);
        drawTrainSideRect(wt_far, wt_near, side_hw, 45, 75, 0.1f, 0.15f, 0.2f, track);
    }

    // Roof
    drawTrainRoofRect(t_far, t_near, -hw, hw, h, 0.3f, 0.3f, 0.3f, track);

    if (isLastCoach) {
        // Flat Back Face (When driving away)
        drawTrainFrontRect(t_near, -hw, hw, 2, 18, 0.1f, 0.1f, 0.1f, track);
        drawTrainFrontRect(t_near, -hw, hw, 18, h, 0.15f, 0.6f, 0.25f, track);
        drawTrainFrontRect(t_near, -hw, hw, 30, 35, 0.95f, 0.95f, 0.95f, track);
        drawTrainFrontRect(t_near, -hw, hw, 85, 90, 0.95f, 0.95f, 0.95f, track);
        drawTrainFrontRect(t_near, -18, 18, 18, 90, 0.85f, 0.15f, 0.15f, track);
        drawTrainFrontRect(t_near, -12, 12, 45, 75, 0.1f, 0.15f, 0.2f, track);
    } else {
        // Inter-coach Connection
        drawTrainFrontRect(t_near, -hw, hw, 18, h, 0.1f, 0.4f, 0.2f, track);
        drawTrainFrontRect(t_near, -24, 24, 18, h-5, 0.1f, 0.1f, 0.1f, track);
    }
}


void drawBangladeshTrain(int track, float frontT, bool isApproaching) {
    float engine_f, engine_b, c1_f, c1_b, c2_f, c2_b, c3_f, c3_b;

    if (isApproaching) {
        engine_f = frontT;
        engine_b = engine_f - 0.14f;
        c1_f = engine_b - 0.01f; c1_b = c1_f - 0.16f;
        c2_f = c1_b - 0.01f; c2_b = c2_f - 0.16f;
        c3_f = c2_b - 0.01f; c3_b = c3_f - 0.16f;

        if (c3_f > 0.01f && c3_b < 1.2f) drawCoach(clampT(c3_b), clampT(c3_f), track, false);
        if (c2_f > 0.01f && c2_b < 1.2f) drawCoach(clampT(c2_b), clampT(c2_f), track, false);
        if (c1_f > 0.01f && c1_b < 1.2f) drawCoach(clampT(c1_b), clampT(c1_f), track, false);
        if (engine_f > 0.01f && engine_b < 1.2f) drawLocomotive(clampT(engine_b), clampT(engine_f), track, true);
    } else {
        engine_f = frontT;
        engine_b = engine_f + 0.14f;
        c1_f = engine_b + 0.01f; c1_b = c1_f + 0.16f;
        c2_f = c1_b + 0.01f; c2_b = c2_f + 0.16f;
        c3_f = c2_b + 0.01f; c3_b = c3_f + 0.16f;

        if (engine_b > 0.01f && engine_f < 1.2f) drawLocomotive(clampT(engine_f), clampT(engine_b), track, false);
        if (c1_b > 0.01f && c1_f < 1.2f) drawCoach(clampT(c1_f), clampT(c1_b), track, false);
        if (c2_b > 0.01f && c2_f < 1.2f) drawCoach(clampT(c2_f), clampT(c2_b), track, false);
        if (c3_b > 0.01f && c3_f < 1.2f) drawCoach(clampT(c3_f), clampT(c3_b), track, true);
    }
}



void drawRectangle(float x1, float y1, float x2, float y2, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
    glEnd();
}

void drawBush(float x, float y, float sx, float sy,
              float r, float g, float b)
{
    glPushMatrix();

    glTranslatef(x, y, 0);
    glScalef(sx, sy, 1);

    glColor3f(r, g, b);

    // Main bush
    glBegin(GL_QUADS);

        glVertex2f(-1, -1);
        glVertex2f(1, -1);
        glVertex2f(1, 1);
        glVertex2f(-1, 1);

    glEnd();

    // Rotated part
    glRotatef(45, 0, 0, 1);

    glBegin(GL_QUADS);

        glVertex2f(-1, -1);
        glVertex2f(1, -1);
        glVertex2f(1, 1);
        glVertex2f(-1, 1);

    glEnd();

    glPopMatrix();
}

void drawSky() { drawRectangle(0, 300, 1280, 720, 0.53f, 0.81f, 0.98f); }

void drawGround() { drawRectangle(0, 0, 1280, 300, 0.32f, 0.65f, 0.25f); }

void drawBackgroundBuildings() {
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



void drawCars() {
    for(int i = 0; i < NUM_CARS; i++) {
        drawRectangle(cars[i].x - 1, cars[i].y - 1, cars[i].x + 15, cars[i].y, 0.55f, 0.55f, 0.58f);
        drawRectangle(cars[i].x, cars[i].y, cars[i].x + 14, cars[i].y + 4, cars[i].r, cars[i].g, cars[i].b);
        drawRectangle(cars[i].x + 3, cars[i].y + 4, cars[i].x + 11, cars[i].y + 7, 0.7f, 0.8f, 0.9f);
    }
}

void drawLeftRoadCars() {
    for(int pass = 0; pass < 2; pass++) {
        for(int i = 0; i < NUM_LEFT_CARS; i++) {
            float t = leftCars[i].t;
            if(t < 0.0f || t > 1.2f) continue;
            if ((pass == 0 && t > 0.5f) || (pass == 1 && t <= 0.5f)) continue;

            float scale = 0.25f + 0.75f * t;
            float cx, cy;
            cy = 300.0f * (1.0f - t);

            if (leftCars[i].dir == 1) {
                cx = 210.0f * (1.0f - t) + 60.0f * t;
            } else {
                cx = 170.0f * (1.0f - t) + (-40.0f) * t;
            }

            float width = 60.0f * scale;
            float height = 24.0f * scale;
            float roof = 16.0f * scale;

            drawRectangle(cx - width/2 - 2*scale, cy - 4*scale, cx + width/2 + 2*scale, cy + 2*scale, 0.4f, 0.4f, 0.42f);
            drawRectangle(cx - width/2, cy, cx + width/2, cy + height, leftCars[i].r, leftCars[i].g, leftCars[i].b);

            if (leftCars[i].dir == 1) {
                drawRectangle(cx - width/2 + 8*scale, cy + height, cx + width/2 - 8*scale, cy + height + roof, 0.2f, 0.2f, 0.25f);
                drawRectangle(cx - width/2 + 2*scale, cy + 6*scale, cx - width/2 + 14*scale, cy + 10*scale, 1.0f, 0.0f, 0.0f);
                drawRectangle(cx + width/2 - 14*scale, cy + 6*scale, cx + width/2 - 2*scale, cy + 10*scale, 1.0f, 0.0f, 0.0f);
            } else {
                drawRectangle(cx - width/2 + 6*scale, cy + height, cx + width/2 - 6*scale, cy + height + roof, 0.6f, 0.8f, 0.9f);
                drawRectangle(cx - width/2 + 2*scale, cy + 8*scale, cx - width/2 + 14*scale, cy + 14*scale, 1.0f, 0.9f, 0.6f);
                drawRectangle(cx + width/2 - 14*scale, cy + 8*scale, cx + width/2 - 2*scale, cy + 14*scale, 1.0f, 0.9f, 0.6f);
                drawRectangle(cx - 10*scale, cy + 4*scale, cx + 10*scale, cy + 10*scale, 0.15f, 0.15f, 0.15f);
            }
        }
    }
}

void drawRightRoadVehicles() {
    for(int pass = 0; pass < 2; pass++) {
        for(int i = 0; i < NUM_RIGHT_VEHICLES; i++) {
            float t = rightVehicles[i].t;
            if(t < 0.0f || t > 1.2f) continue;
            if ((pass == 0 && t > 0.5f) || (pass == 1 && t <= 0.5f)) continue;

            float scale = 0.25f + 0.80f * t;
            float cx, cy;
            cy = 300.0f * (1.0f - t);

            if (rightVehicles[i].dir == 1) {
                cx = 750.0f * (1.0f - t) + 1060.0f * t;
            } else {
                cx = 800.0f * (1.0f - t) + 1190.0f * t;
            }

            float width, height, roof;
            if (rightVehicles[i].type == 1) {
                width = 65.0f * scale;
                height = 80.0f * scale;
                roof = 0.0f;
            } else {
                width = 65.0f * scale;
                height = 26.0f * scale;
                roof = 18.0f * scale;
            }

            drawRectangle(cx - width/2 - 2*scale, cy - 4*scale, cx + width/2 + 2*scale, cy + 2*scale, 0.4f, 0.4f, 0.42f);
            drawRectangle(cx - width/2, cy, cx + width/2, cy + height, rightVehicles[i].r, rightVehicles[i].g, rightVehicles[i].b);

            if (rightVehicles[i].dir == 1) {
                if (rightVehicles[i].type == 1) {
                    drawRectangle(cx - width/2, cy, cx + width/2, cy + 10*scale, 0.15f, 0.15f, 0.15f);
                    drawRectangle(cx - width/2 + 15*scale, cy + 15*scale, cx + width/2 - 15*scale, cy + 35*scale, 0.2f, 0.2f, 0.2f);
                    drawRectangle(cx - width/2 + 8*scale, cy + 40*scale, cx + width/2 - 8*scale, cy + height - 15*scale, 0.2f, 0.2f, 0.25f);
                    drawRectangle(cx - width/2 + 15*scale, cy + height - 12*scale, cx + width/2 - 15*scale, cy + height - 4*scale, 0.1f, 0.1f, 0.1f);
                    drawRectangle(cx - width/2 + 4*scale, cy + 15*scale, cx - width/2 + 12*scale, cy + 30*scale, 1.0f, 0.0f, 0.0f);
                    drawRectangle(cx + width/2 - 12*scale, cy + 15*scale, cx + width/2 - 4*scale, cy + 30*scale, 1.0f, 0.0f, 0.0f);
                } else {
                    drawRectangle(cx - width/2 + 8*scale, cy + height, cx + width/2 - 8*scale, cy + height + roof, 0.2f, 0.2f, 0.25f);
                    drawRectangle(cx - width/2 + 2*scale, cy + 6*scale, cx - width/2 + 14*scale, cy + 10*scale, 1.0f, 0.0f, 0.0f);
                    drawRectangle(cx + width/2 - 14*scale, cy + 6*scale, cx + width/2 - 2*scale, cy + 10*scale, 1.0f, 0.0f, 0.0f);
                }
            } else {
                if (rightVehicles[i].type == 1) {
                    drawRectangle(cx - width/2, cy, cx + width/2, cy + 10*scale, 0.15f, 0.15f, 0.15f);
                    drawRectangle(cx - width/2 + 5*scale, cy + 30*scale, cx + width/2 - 5*scale, cy + height - 15*scale, 0.6f, 0.8f, 0.9f);
                    drawRectangle(cx - width/2 + 10*scale, cy + height - 12*scale, cx + width/2 - 10*scale, cy + height - 3*scale, 0.1f, 0.1f, 0.1f);
                    drawRectangle(cx - width/2 + 14*scale, cy + height - 10*scale, cx + width/2 - 14*scale, cy + height - 5*scale, 1.0f, 0.8f, 0.1f);
                    drawRectangle(cx - width/2 + 6*scale, cy + 14*scale, cx - width/2 + 20*scale, cy + 22*scale, 1.0f, 0.9f, 0.6f);
                    drawRectangle(cx + width/2 - 20*scale, cy + 14*scale, cx + width/2 - 6*scale, cy + 22*scale, 1.0f, 0.9f, 0.6f);
                    drawRectangle(cx - width/2 - 6*scale, cy + 35*scale, cx - width/2, cy + 55*scale, 0.1f, 0.1f, 0.1f);
                    drawRectangle(cx + width/2, cy + 35*scale, cx + width/2 + 6*scale, cy + 55*scale, 0.1f, 0.1f, 0.1f);
                } else {
                    drawRectangle(cx - width/2 + 6*scale, cy + height, cx + width/2 - 6*scale, cy + height + roof, 0.6f, 0.8f, 0.9f);
                    drawRectangle(cx - width/2 + 2*scale, cy + 8*scale, cx - width/2 + 14*scale, cy + 14*scale, 1.0f, 0.9f, 0.6f);
                    drawRectangle(cx + width/2 - 14*scale, cy + 8*scale, cx + width/2 - 2*scale, cy + 14*scale, 1.0f, 0.9f, 0.6f);
                    drawRectangle(cx - 10*scale, cy + 4*scale, cx + 10*scale, cy + 10*scale, 0.15f, 0.15f, 0.15f);
                }
            }
        }
    }
}

void drawElevatedHighway() {
    drawRectangle(0, 350, 1280, 370, 0.65f, 0.65f, 0.68f);
    for(int i = 20; i < 1280; i += 120) {
        drawRectangle(i, 300, i + 15, 350, 0.6f, 0.6f, 0.63f);
    }
    drawCars();
    for(int i = 26; i < 1280; i += 120) {
        drawRectangle(i, 370, i + 2, 388, 0.45f, 0.45f, 0.48f);
        drawRectangle(i - 4, 386, i + 6, 388, 0.45f, 0.45f, 0.48f);
    }
}

void drawRoads() {
    glColor3f(0.45f, 0.45f, 0.45f);
    glBegin(GL_QUADS);
        glVertex2f(940, 0);
        glVertex2f(1280, 0);
        glVertex2f(850, 300);
        glVertex2f(700, 300);
    glEnd();
}

void drawLeftScenery() {
    glColor3f(0.48f, 0.48f, 0.48f);
    glBegin(GL_QUADS);
        glVertex2f(-10, 120);
        glVertex2f(100, 0);
        glVertex2f(230, 300);
        glVertex2f(150, 300);
    glEnd();
}

void drawGravelBed() {
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
    // Rails
    glColor3f(0.20f, 0.20f, 0.25f);

    glBegin(GL_QUADS);

        // Left rail
        glVertex2f(startX - 35, 0);
        glVertex2f(startX - 30, 0);
        glVertex2f(endX - 5, 300);
        glVertex2f(endX - 8, 300);

        // Right rail
        glVertex2f(startX + 30, 0);
        glVertex2f(startX + 35, 0);
        glVertex2f(endX + 8, 300);
        glVertex2f(endX + 5, 300);

    glEnd();


    // Sleepers
    for(int y = 10; y < 300; y += 12)
    {
        float x = startX + (endX - startX) * y / 300.0f;

        float width = 35 - (y * 30 / 300.0f);

        drawRectangle(
            x - width,
            y,
            x + width,
            y + 4,
            0.30f, 0.25f, 0.20f
        );
    }
}

void drawAllTracks() {
    drawStraightTrack(240, 390); // Track 0
    drawStraightTrack(430, 465); // Track 1
    drawStraightTrack(620, 540); // Track 2
    drawStraightTrack(810, 615); // Track 3
}

void drawTrackSignal() {
    float xCenter = 516.0f;
    float yBase = 120.0f;
    drawRectangle(xCenter - 6.0f, yBase, xCenter - 2.0f, yBase + 10.0f, 0.45f, 0.45f, 0.45f);
    drawRectangle(xCenter + 2.0f, yBase, xCenter + 6.0f, yBase + 10.0f, 0.45f, 0.45f, 0.45f);
    drawRectangle(xCenter - 2.0f, yBase, xCenter + 2.0f, yBase + 85.0f, 0.5f, 0.5f, 0.5f);
    drawRectangle(xCenter - 7.0f, yBase + 60.0f, xCenter + 7.0f, yBase + 95.0f, 0.15f, 0.15f, 0.15f);
    drawRectangle(xCenter - 2.5f, yBase + 82.0f, xCenter + 2.5f, yBase + 88.0f, 0.9f, 0.2f, 0.2f);
    drawRectangle(xCenter - 2.5f, yBase + 68.0f, xCenter + 2.5f, yBase + 74.0f, 0.2f, 0.3f, 0.2f);
}

void drawLake() {
    glColor3f(0.25f, 0.65f, 0.85f);
    glBegin(GL_POLYGON);
        glVertex2f(1080, 180);
        glVertex2f(1280, 160);
        glVertex2f(1280, 260);
        glVertex2f(930,  260);
    glEnd();
}

void drawRightRoadLamps() {
    drawRectangle(895.75f, 50.0f, 900.0f, 177.5f, 0.35f, 0.35f, 0.35f);
    drawRectangle(900.0f, 173.25f, 933.8f, 177.5f, 0.35f, 0.35f, 0.35f);
    drawRectangle(925.3f, 171.125f, 933.8f, 177.5f, 1.0f, 0.9f, 0.6f);

    drawRectangle(840.8f, 120.0f, 844.0f, 216.0f, 0.35f, 0.35f, 0.35f);
    drawRectangle(844.0f, 212.8f, 869.2f, 216.0f, 0.35f, 0.35f, 0.35f);
    drawRectangle(862.8f, 211.2f, 869.2f, 216.0f, 1.0f, 0.9f, 0.6f);

    drawRectangle(793.7f, 180.0f, 796.0f, 249.0f, 0.35f, 0.35f, 0.35f);
    drawRectangle(796.0f, 246.7f, 813.8f, 249.0f, 0.35f, 0.35f, 0.35f);
    drawRectangle(809.2f, 245.55f, 813.8f, 249.0f, 1.0f, 0.9f, 0.6f);

    drawRectangle(754.45f, 230.0f, 756.0f, 276.5f, 0.35f, 0.35f, 0.35f);
    drawRectangle(756.0f, 274.95f, 767.6f, 276.5f, 0.35f, 0.35f, 0.35f);
    drawRectangle(764.5f, 274.175f, 767.6f, 276.5f, 1.0f, 0.9f, 0.6f);

    drawRectangle(727.0f, 265.0f, 728.0f, 295.8f, 0.35f, 0.35f, 0.35f);
    drawRectangle(728.0f, 294.8f, 735.3f, 295.8f, 0.35f, 0.35f, 0.35f);
    drawRectangle(733.3f, 294.3f, 735.3f, 295.8f, 1.0f, 0.9f, 0.6f);

    drawRectangle(711.275f, 285.0f, 712.0f, 306.75f, 0.35f, 0.35f, 0.35f);
    drawRectangle(712.0f, 306.025f, 716.85f, 306.75f, 0.35f, 0.35f, 0.35f);
    drawRectangle(715.4f, 305.66f, 716.85f, 306.75f, 1.0f, 0.9f, 0.6f);
}

void drawLeftRoadLamps() {
    drawRectangle(121.6f, 50.0f, 125.85f, 177.5f, 0.35f, 0.35f, 0.35f);
    drawRectangle(87.8f, 173.25f, 121.6f, 177.5f, 0.35f, 0.35f, 0.35f);
    drawRectangle(87.8f, 171.125f, 96.3f, 177.5f, 1.0f, 0.9f, 0.6f);

    drawRectangle(152.0f, 120.0f, 155.2f, 216.0f, 0.35f, 0.35f, 0.35f);
    drawRectangle(126.8f, 212.8f, 152.0f, 216.0f, 0.35f, 0.35f, 0.35f);
    drawRectangle(126.8f, 211.2f, 133.2f, 216.0f, 1.0f, 0.9f, 0.6f);

    drawRectangle(178.0f, 180.0f, 180.3f, 249.0f, 0.35f, 0.35f, 0.35f);
    drawRectangle(160.2f, 246.7f, 178.0f, 249.0f, 0.35f, 0.35f, 0.35f);
    drawRectangle(160.2f, 245.55f, 164.8f, 249.0f, 1.0f, 0.9f, 0.6f);

    drawRectangle(199.6f, 230.0f, 201.15f, 276.5f, 0.35f, 0.35f, 0.35f);
    drawRectangle(188.0f, 274.95f, 199.6f, 276.5f, 0.35f, 0.35f, 0.35f);
    drawRectangle(188.0f, 274.175f, 191.1f, 276.5f, 1.0f, 0.9f, 0.6f);

    drawRectangle(214.8f, 265.0f, 215.8f, 295.8f, 0.35f, 0.35f, 0.35f);
    drawRectangle(207.5f, 294.8f, 214.8f, 295.8f, 0.35f, 0.35f, 0.35f);
    drawRectangle(207.5f, 294.3f, 209.5f, 295.8f, 1.0f, 0.9f, 0.6f);

    drawRectangle(223.5f, 285.0f, 224.225f, 306.75f, 0.35f, 0.35f, 0.35f);
    drawRectangle(218.65f, 306.025f, 223.5f, 306.75f, 0.35f, 0.35f, 0.35f);
    drawRectangle(218.65f, 305.66f, 220.1f, 306.75f, 1.0f, 0.9f, 0.6f);
}

void drawFoliage() {
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

    drawBush(20, 30, 70, 60, 0.13f, 0.35f, 0.13f);
    drawBush(80, 60, 60, 55, 0.14f, 0.36f, 0.14f);
    drawBush(140, 40, 55, 50, 0.12f, 0.33f, 0.12f);

    drawBush(140, 90, 45, 50, 0.16f, 0.39f, 0.16f);
    drawBush(165, 130, 40, 45, 0.13f, 0.35f, 0.13f);
    drawBush(190, 170, 35, 40, 0.15f, 0.38f, 0.15f);
    drawBush(220, 210, 30, 35, 0.17f, 0.40f, 0.17f);
    drawBush(250, 245, 25, 30, 0.14f, 0.36f, 0.14f);
    drawBush(275, 270, 20, 25, 0.16f, 0.39f, 0.16f);
    drawBush(300, 290, 15, 18, 0.13f, 0.35f, 0.13f);

    drawBush(180, 100, 40, 45, 0.15f, 0.38f, 0.15f);
    drawBush(205, 145, 35, 40, 0.14f, 0.36f, 0.14f);
    drawBush(235, 185, 30, 35, 0.16f, 0.39f, 0.16f);
    drawBush(265, 225, 25, 30, 0.13f, 0.35f, 0.13f);
    drawBush(290, 260, 20, 22, 0.15f, 0.38f, 0.15f);
    drawBush(315, 285, 15, 15, 0.14f, 0.36f, 0.14f);

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

    // 6. Far Right Lake Area
    drawBush(960, 290, 30, 20, 0.13f, 0.35f, 0.13f);
    drawBush(1050, 285, 40, 25, 0.15f, 0.38f, 0.15f);
    drawBush(1150, 280, 45, 30, 0.16f, 0.39f, 0.16f);
    drawBush(1250, 290, 35, 20, 0.14f, 0.36f, 0.14f);

    // Bushes overlapping the lake
    drawBush(1000, 265, 25, 20, 0.14f, 0.37f, 0.14f);
    drawBush(1100, 255, 35, 25, 0.16f, 0.39f, 0.16f);
    drawBush(1200, 260, 45, 25, 0.12f, 0.34f, 0.12f);

    // Bottom right corner
    drawBush(1290, 40, 80, 90, 0.12f, 0.32f, 0.12f);
    drawBush(1150, 170, 40, 35, 0.13f, 0.35f, 0.13f);
}

void init() {
    srand((unsigned int)time(NULL));
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glPointSize(5);
    gluOrtho2D(0, 1280, 0, 720);
}

void timer(int value) {
    // Expressway Cars
    for (int i = 0; i < NUM_CARS; i++) {
        cars[i].x += cars[i].speed * cars[i].dir;
        if (cars[i].dir == -1 && cars[i].x < -100) {
            cars[i].x = 1350;
        } else if (cars[i].dir == 1 && cars[i].x > 1380) {
            cars[i].x = -100;
        }
    }

    // Left Road Cars
    for (int i = 0; i < NUM_LEFT_CARS; i++) {
        if (leftCars[i].dir == 1) {
            leftCars[i].t -= leftCars[i].speed * (leftCars[i].t + 0.3f);
            if (leftCars[i].t < -0.1f) leftCars[i].t = 1.2f;
        } else {
            leftCars[i].t += leftCars[i].speed * (leftCars[i].t + 0.3f);
            if (leftCars[i].t > 1.2f) leftCars[i].t = -0.1f;
        }
    }

    // Right Road Vehicles
    for (int i = 0; i < NUM_RIGHT_VEHICLES; i++) {
        if (rightVehicles[i].dir == 1) {
            rightVehicles[i].t -= rightVehicles[i].speed * (rightVehicles[i].t + 0.3f);
            if (rightVehicles[i].t < -0.1f) rightVehicles[i].t = 1.2f;
        } else {
            rightVehicles[i].t += rightVehicles[i].speed * (rightVehicles[i].t + 0.3f);
            if (rightVehicles[i].t > 1.2f) rightVehicles[i].t = -0.1f;
        }
    }

    // 4. Single Train Controller
    if (trainWaitTimer > 0.0f) {
        trainWaitTimer -= 0.03f;
        if (trainWaitTimer <= 0.0f) {
            activeTrainTrack = rand() % 4;
            activeTrainApproaching = (activeTrainTrack >= 2);

            if (activeTrainApproaching) {
                activeTrainT = -0.2f; // Start deep behind horizon
            } else {
                activeTrainT = 1.5f; // Start off-screen foreground
            }
        }
    } else {
        if (activeTrainApproaching) {
            float tSpeed = (activeTrainT < 0.0f) ? 0.003f : (0.008f * (activeTrainT + 0.3f));
            activeTrainT += tSpeed;

            if (activeTrainT - 0.65f > 1.2f) { // Train tail completely passed screen
                trainWaitTimer = getRandomFloat(2.0f, 10.0f);
            }
        } else {
            float tSpeed = (activeTrainT > 1.2f) ? 0.005f : (0.008f * (activeTrainT + 0.3f));
            if (tSpeed < 0.003f) tSpeed = 0.003f;
            activeTrainT -= tSpeed;

            if (activeTrainT + 0.7f < 0.0f) { // Train tail completely passed horizon
                trainWaitTimer = getRandomFloat(2.0f, 10.0f);
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(30, timer, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawSky();
    drawBackgroundBuildings();
    drawElevatedHighway();

    drawGround();
    drawRoads();
    drawGravelBed();

    drawAllTracks();
    drawTrackSignal();

    // Draw the single active train
    if (trainWaitTimer <= 0.0f) {
        drawBangladeshTrain(activeTrainTrack, activeTrainT, activeTrainApproaching);
    }

    drawLeftScenery();

    drawLeftRoadCars();
    drawRightRoadVehicles();

    drawLake();

    drawRightRoadLamps();
    drawLeftRoadLamps();

    drawFoliage();

    glutSwapBuffers();
}

int main(int argc, char **argv) {
    srand((unsigned int)time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("TrainTracks");

    init();
    glutDisplayFunc(display);
    glutTimerFunc(30, timer, 0);
    glutMainLoop();

    return 0;
}
