#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>

// Camera
float camX = -1.0f, camY = 2.0f, camZ = 5.9f;
float camAngleY = 13.0f;

// Light animation
bool lightOn = true;

// Fan animation
float fanAngle = 0.0f;
bool fanOn = true;

// Helpers
void setColor(float r, float g, float b) { glColor3f(r, g, b); }

void drawQuad(float x1, float y1, float z1,
    float x2, float y2, float z2,
    float x3, float y3, float z3,
    float x4, float y4, float z4) {
    glBegin(GL_QUADS);
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y2, z2);
    glVertex3f(x3, y3, z3);
    glVertex3f(x4, y4, z4);
    glEnd();
}

// Simple Grid Lines
void drawGrid() {
    glColor3f(0.20f, 0.20f, 0.20f);
    glBegin(GL_LINES);
    for (int i = -6; i <= 6; i += 2) {
        glVertex3f(i, 0, -6); glVertex3f(i, 0, 6);
        glVertex3f(-6, 0, i); glVertex3f(6, 0, i);
    }
    glEnd();
}

// Custom Cylinder
void drawCylinder(float radius, float height, int slices) {
    float step = 2.0f * 3.14159f / slices;
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices; i++) {
        float a = i * step;
        float x = cos(a) * radius, z = sin(a) * radius;
        glNormal3f(x / radius, 0, z / radius);
        glVertex3f(x, 0, z);
        glVertex3f(x, height, z);
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, -1, 0);
    glVertex3f(0, 0, 0);
    for (int i = 0;i <= slices;i++) { float a = i * step;glVertex3f(cos(a) * radius, 0, sin(a) * radius); }
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, 1, 0);
    glVertex3f(0, height, 0);
    for (int i = slices;i >= 0;i--) { float a = i * step;glVertex3f(cos(a) * radius, height, sin(a) * radius); }
    glEnd();
}

// Custom Cone
void drawCone(float baseRadius, float height, int slices) {
    float step = 2.0f * 3.14159f / slices;
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, height, 0);
    for (int i = 0;i <= slices;i++) { float a = i * step;glVertex3f(cos(a) * baseRadius, 0, sin(a) * baseRadius); }
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, -1, 0);
    glVertex3f(0, 0, 0);
    for (int i = slices;i >= 0;i--) { float a = i * step;glVertex3f(cos(a) * baseRadius, 0, sin(a) * baseRadius); }
    glEnd();
}

// Room Structure 
void drawRoom() {
    // Floor
    setColor(0.75f, 0.75f, 0.75f);
    drawQuad(-6, -0.01f, -6, 6, -0.01f, -6, 6, -0.01f, 6, -6, -0.01f, 6);

    // Floor grid
    drawGrid();

    // Ceiling
    setColor(0.85f, 0.85f, 0.85f);
    drawQuad(-6, 5, -6, 6, 5, -6, 6, 5, 6, -6, 5, 6);

    // Back wall
    setColor(0.80f, 0.80f, 0.80f);
    drawQuad(-6, 0, -6, 6, 0, -6, 6, 5, -6, -6, 5, -6);

    // Left wall
    setColor(0.80f, 0.80f, 0.80f);
    drawQuad(-6, 0, -6, -6, 0, 6, -6, 5, 6, -6, 5, -6);

    // Right wall
    setColor(0.80f, 0.80f, 0.80f);
    drawQuad(6, 0, -6, 6, 0, 6, 6, 5, 6, 6, 5, -6);

    // Front wall
    setColor(0.80f, 0.80f, 0.80f);
    drawQuad(-6, 0, 6, 6, 0, 6, 6, 5, 6, -6, 5, 6);
}

// SINGLE Window on back wall
void drawSingleWindowWithCurtain() {

    float ox = 1.5f;
    float drop = 0.65f;

    // Outer frame
    setColor(0.30f, 0.18f, 0.08f);
    drawQuad(ox - 0.15f, 2.75f - drop, -5.99f,
        ox + 2.15f, 2.75f - drop, -5.99f,
        ox + 2.15f, 4.35f - drop, -5.99f,
        ox - 0.15f, 4.35f - drop, -5.99f);

    // Glass
    setColor(0.68f, 0.85f, 0.97f);
    drawQuad(ox, 3.0f - drop, -5.985f,
        ox + 2.0f, 3.0f - drop, -5.985f,
        ox + 2.0f, 4.2f - drop, -5.985f,
        ox, 4.2f - drop, -5.985f);

    // Vertical divider
    setColor(0.30f, 0.18f, 0.08f);
    drawQuad(ox + 0.97f, 3.0f - drop, -5.975f,
        ox + 1.03f, 3.0f - drop, -5.975f,
        ox + 1.03f, 4.2f - drop, -5.975f,
        ox + 0.97f, 4.2f - drop, -5.975f);

    // Horizontal divider
    drawQuad(ox, 3.57f - drop, -5.975f,
        ox + 2.0f, 3.57f - drop, -5.975f,
        ox + 2.0f, 3.63f - drop, -5.975f,
        ox, 3.63f - drop, -5.975f);

    // Window sill
    setColor(0.42f, 0.26f, 0.10f);
    drawQuad(ox - 0.15f, 2.73f - drop, -5.99f,
        ox + 2.15f, 2.73f - drop, -5.99f,
        ox + 2.15f, 2.75f - drop, -5.99f,
        ox - 0.15f, 2.75f - drop, -5.99f);

    // Curtain rod
    setColor(0.65f, 0.52f, 0.35f);
    glPushMatrix();
    glTranslatef(ox - 0.4f, 2.30f - drop, -5.90f);
    drawCylinder(0.025f, 2.10f, 8);
    glPopMatrix();

    // Curtain
    setColor(0.85f, 0.75f, 0.95f);
    glPushMatrix();
    glTranslatef(ox + 0.1f, 3.5f - drop, -5.88f);
    glScalef(0.8f, 2.0f, 0.08f);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(ox + 1.9f, 3.5f - drop, -5.88f);
    glScalef(0.8f, 2.0f, 0.08f);
    glutSolidCube(1.0);
    glPopMatrix();
}

// Door on left wall — INSIDE ROOM
void drawDoor() {
    // Door frame embedded in left wall
    setColor(0.35f, 0.22f, 0.10f);
    drawQuad(-5.99f, 0, 1.2f, -5.99f, 0, 3.2f, -5.99f, 3.8f, 3.2f, -5.99f, 3.8f, 1.2f);

    // Door panels
    setColor(0.55f, 0.38f, 0.18f);
    drawQuad(-5.98f, 0.05f, 1.3f, -5.98f, 0.05f, 3.1f, -5.98f, 3.65f, 3.1f, -5.98f, 3.65f, 1.3f);

    // Door details
    setColor(0.48f, 0.32f, 0.14f);
    drawQuad(-5.97f, 0.3f, 1.5f, -5.97f, 0.3f, 2.9f, -5.97f, 1.7f, 2.9f, -5.97f, 1.7f, 1.5f);
    drawQuad(-5.97f, 2.0f, 1.5f, -5.97f, 2.0f, 2.9f, -5.97f, 3.3f, 2.9f, -5.97f, 3.3f, 1.5f);

    // Door handle
    setColor(0.35f, 0.20f, 0.10f);
    glPushMatrix();
    glTranslatef(-5.96f, 1.8f, 3.0f);
    glutSolidSphere(0.08, 10, 10);
    glPopMatrix();

    // Switch plate
    setColor(0.92f, 0.92f, 0.90f);
    drawQuad(-5.97f, 1.85f, 3.35f,
        -5.97f, 1.85f, 3.65f,
        -5.97f, 2.25f, 3.65f,
        -5.97f, 2.25f, 3.35f);

    // Two switch toggles
    setColor(0.80f, 0.80f, 0.78f);
    drawQuad(-5.965f, 1.95f, 3.42f,
        -5.965f, 1.95f, 3.52f,
        -5.965f, 2.15f, 3.52f,
        -5.965f, 2.15f, 3.42f);
    drawQuad(-5.965f, 1.95f, 3.55f,
        -5.965f, 1.95f, 3.65f,
        -5.965f, 2.15f, 3.65f,
        -5.965f, 2.15f, 3.55f);
}

// Bed
void drawBed() {
    glPushMatrix();
    glTranslatef(-3.5f, 0, -3.5f);

    setColor(0.40f, 0.25f, 0.10f);
    glPushMatrix(); glScalef(2.6f, 0.35f, 4.5f); glutSolidCube(1.0); glPopMatrix();

    setColor(0.92f, 0.88f, 0.82f);
    glPushMatrix(); glTranslatef(0, 0.30f, 0); glScalef(2.4f, 0.22f, 4.2f); glutSolidCube(1.0); glPopMatrix();

    setColor(0.98f, 0.95f, 0.92f);
    glPushMatrix(); glTranslatef(-0.50f, 0.50f, -1.6f); glRotatef(15, 1, 0, 0); glScalef(0.85f, 0.14f, 0.60f); glutSolidCube(1.0); glPopMatrix();
    glPushMatrix(); glTranslatef(0.50f, 0.50f, -1.6f); glRotatef(15, 1, 0, 0); glScalef(0.85f, 0.14f, 0.60f); glutSolidCube(1.0); glPopMatrix();

    setColor(0.35f, 0.20f, 0.08f);
    glPushMatrix(); glTranslatef(0, 0.9f, -2.35f); glScalef(2.6f, 1.5f, 0.15f); glutSolidCube(1.0); glPopMatrix();

    setColor(0.28f, 0.16f, 0.06f);
    float lx[] = { -1.1f, 1.1f, -1.1f, 1.1f };
    float lz[] = { -2.1f, -2.1f, 2.1f, 2.1f };
    for (int i = 0;i < 4;i++) {
        glPushMatrix(); glTranslatef(lx[i], -0.22f, lz[i]); glScalef(0.15f, 0.45f, 0.15f); glutSolidCube(1.0); glPopMatrix();
    }
    glPopMatrix();
}

// Study Table
void drawTable() {
    glPushMatrix();
    glTranslatef(0.5f, 0, -3.5f);

    // Modern thick top with rounded feel
    setColor(0.92f, 0.92f, 0.95f);
    glPushMatrix(); glTranslatef(0, 1.55f, 0); glScalef(2.5f, 0.12f, 1.4f); glutSolidCube(1.0); glPopMatrix();

    // Metal frame legs
    setColor(0.55f, 0.55f, 0.58f);
    // Two U-shaped side frames
    float lx[] = { -1.1f, 1.1f };
    for (int s = 0; s < 2; s++) {
        // Vertical leg front
        glPushMatrix(); glTranslatef(lx[s], 0.77f, 0.6f); glScalef(0.07f, 1.54f, 0.07f); glutSolidCube(1.0); glPopMatrix();
        // Vertical leg back
        glPushMatrix(); glTranslatef(lx[s], 0.77f, -0.6f); glScalef(0.07f, 1.54f, 0.07f); glutSolidCube(1.0); glPopMatrix();
        // Horizontal foot bar
        glPushMatrix(); glTranslatef(lx[s], 0.04f, 0); glScalef(0.07f, 0.07f, 1.2f); glutSolidCube(1.0); glPopMatrix();
    }

    // Under-shelf
    setColor(0.88f, 0.88f, 0.90f);
    glPushMatrix(); glTranslatef(0, 0.65f, 0); glScalef(2.3f, 0.06f, 1.2f); glutSolidCube(1.0); glPopMatrix();

    // Monitor
    setColor(0.12f, 0.12f, 0.14f);
    glPushMatrix(); glTranslatef(0.2f, 1.62f, 0.0f); glScalef(0.95f, 0.05f, 0.55f); glutSolidCube(1.0); glPopMatrix();
    // Monitor screen stand
    setColor(0.20f, 0.20f, 0.22f);
    glPushMatrix(); glTranslatef(0.2f, 1.77f, -0.18f); glScalef(0.05f, 0.40f, 0.05f); glutSolidCube(1.0); glPopMatrix();
    // Monitor screen
    setColor(0.12f, 0.12f, 0.16f);
    glPushMatrix(); glTranslatef(0.2f, 2.00f, -0.20f); glRotatef(-5, 1, 0, 0); glScalef(0.92f, 0.55f, 0.04f); glutSolidCube(1.0); glPopMatrix();
    // Screen display
    setColor(0.25f, 0.50f, 0.90f);
    glPushMatrix(); glTranslatef(0.2f, 2.00f, -0.18f); glRotatef(-5, 1, 0, 0); glScalef(0.84f, 0.48f, 0.01f); glutSolidCube(1.0); glPopMatrix();

    // Wireless mouse
    setColor(0.90f, 0.90f, 0.92f);
    glPushMatrix(); glTranslatef(0.82f, 1.62f, 0.28f); glScalef(0.12f, 0.025f, 0.20f); glutSolidCube(1.0); glPopMatrix();

    // pen holder
    setColor(0.10f, 0.30f, 0.15f);
    glPushMatrix(); glTranslatef(-0.85f, 1.62f, -0.42f); drawCylinder(0.09f, 0.22f, 10); glPopMatrix();
    // Pens in holder
    float penC[3][3] = { {0.9f,0.2f,0.2f},{0.90f,0.90f,0.90f},{0.10f,0.10f,0.10f} };
    float pa[] = { 0.0f, 2.0f, 4.0f };
    for (int p = 0;p < 3;p++) {
        setColor(penC[p][0], penC[p][1], penC[p][2]);
        glPushMatrix();
        glTranslatef(-0.85f + cos(pa[p]) * 0.04f, 1.68f, -0.42f + sin(pa[p]) * 0.04f);
        drawCylinder(0.012f, 0.28f, 6);
        glPopMatrix();
    }

    // Small potted plant on corner of desk
    setColor(0.40f, 0.25f, 0.10f);
    glPushMatrix(); glTranslatef(-0.85f, 1.62f, 0.45f); drawCylinder(0.07f, 0.10f, 10); glPopMatrix();
    setColor(0.15f, 0.62f, 0.22f);
    glPushMatrix(); glTranslatef(-0.85f, 1.76f, 0.45f); glutSolidSphere(0.10f, 8, 8); glPopMatrix();

    // Notepad
    setColor(0.98f, 0.95f, 0.85f);
    glPushMatrix(); glTranslatef(-0.30f, 1.63f, 0.42f); glScalef(0.32f, 0.01f, 0.22f); glutSolidCube(1.0); glPopMatrix();
    setColor(0.50f, 0.65f, 0.90f);
    glPushMatrix(); glTranslatef(-0.30f, 1.638f, 0.42f); glScalef(0.30f, 0.005f, 0.04f); glutSolidCube(1.0); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.30f, 1.638f, 0.46f); glScalef(0.30f, 0.005f, 0.04f); glutSolidCube(1.0); glPopMatrix();

    glPopMatrix();
}

// SMART Chair
void drawChair() {
    glPushMatrix();
    glTranslatef(0.5f, 0, -2.0f);
    glRotatef(180.0f, 0, 1, 0);

    // Seat
    setColor(0.22f, 0.22f, 0.24f);
    glPushMatrix(); glTranslatef(0, 1.0f, 0); glScalef(0.92f, 0.10f, 0.90f); glutSolidCube(1.0); glPopMatrix();

    // Seat cushion
    setColor(0.15f, 0.15f, 0.15f);
    glPushMatrix(); glTranslatef(0, 1.06f, 0.03f); glScalef(0.84f, 0.06f, 0.80f); glutSolidCube(1.0); glPopMatrix();

    // Back rest (tall ergonomic)
    setColor(0.22f, 0.22f, 0.24f);
    glPushMatrix(); glTranslatef(0, 1.60f, -0.42f); glScalef(0.88f, 1.00f, 0.10f); glutSolidCube(1.0); glPopMatrix();

    // Back cushion
    setColor(0.28f, 0.55f, 0.72f);
    glPushMatrix(); glTranslatef(0, 1.60f, -0.38f); glScalef(0.80f, 0.90f, 0.07f); glutSolidCube(1.0); glPopMatrix();

    // Lumbar support bump
    setColor(0.20f, 0.48f, 0.65f);
    glPushMatrix(); glTranslatef(0, 1.35f, -0.34f); glScalef(0.50f, 0.18f, 0.06f); glutSolidCube(1.0); glPopMatrix();

    // Headrest
    setColor(0.22f, 0.22f, 0.24f);
    glPushMatrix(); glTranslatef(0, 2.18f, -0.42f); glScalef(0.42f, 0.22f, 0.10f); glutSolidCube(1.0); glPopMatrix();
    setColor(0.28f, 0.55f, 0.72f);
    glPushMatrix(); glTranslatef(0, 2.18f, -0.38f); glScalef(0.38f, 0.18f, 0.07f); glutSolidCube(1.0); glPopMatrix();

    // Armrests
    setColor(0.18f, 0.18f, 0.20f);
    float ax[] = { -0.50f, 0.50f };
    for (int a = 0;a < 2;a++) {
        // Arm post
        glPushMatrix(); glTranslatef(ax[a], 1.02f, 0.0f); glScalef(0.06f, 0.50f, 0.06f); glutSolidCube(1.0); glPopMatrix();
        // Arm pad
        setColor(0.30f, 0.30f, 0.32f);
        glPushMatrix(); glTranslatef(ax[a], 1.30f, 0.05f); glScalef(0.10f, 0.05f, 0.40f); glutSolidCube(1.0); glPopMatrix();
        setColor(0.18f, 0.18f, 0.20f);
    }

    // Gas lift cylinder (center post)
    setColor(0.60f, 0.60f, 0.62f);
    glPushMatrix(); glTranslatef(0, 0.20f, 0); drawCylinder(0.055f, 1.00f, 10); glPopMatrix();

    // Star base (5 legs)
    setColor(0.25f, 0.25f, 0.28f);
    for (int leg = 0;leg < 5;leg++) {
        float angle = leg * 72.0f * 3.14159f / 180.0f;
        float ex = cos(angle) * 0.58f;
        float ez = sin(angle) * 0.58f;
        glPushMatrix();
        glTranslatef(0, 0.04f, 0);
        glBegin(GL_QUADS);
        glVertex3f(0, 0, 0);
        glVertex3f(ex - 0.04f, 0, ez - 0.04f);
        glVertex3f(ex, 0.08f, ez);
        glVertex3f(0.04f, 0.08f, 0.04f);
        glEnd();
        glPopMatrix();
        // Wheel
        setColor(0.15f, 0.15f, 0.15f);
        glPushMatrix(); glTranslatef(ex, 0.04f, ez); drawCylinder(0.04f, 0.05f, 6); glPopMatrix();
    }

    glPopMatrix();
}

// Bookshelf
void drawBookshelf() {
    glPushMatrix();
    glTranslatef(-5.6f, 0, -1.5f);
    glRotatef(90, 0, 1, 0);

    setColor(0.35f, 0.22f, 0.09f);
    glPushMatrix(); glScalef(2.4f, 4.8f, 0.55f); glutSolidCube(1.0); glPopMatrix();

    setColor(0.28f, 0.16f, 0.06f);
    glPushMatrix(); glTranslatef(0, 0, 0.24f); glScalef(2.2f, 4.7f, 0.04f); glutSolidCube(1.0); glPopMatrix();

    setColor(0.50f, 0.32f, 0.13f);
    float shelfY[] = { -2.0f, -0.9f, 0.2f, 1.3f, 2.2f };
    for (int s = 0;s < 5;s++) {
        glPushMatrix();
        glTranslatef(0, shelfY[s], 0.05f);
        glScalef(2.28f, 0.08f, 0.50f);
        glutSolidCube(1.0);
        glPopMatrix();
    }

    struct BookColor { float r, g, b; };
    BookColor palette[] = {
        {0.80f,0.10f,0.10f},{0.10f,0.50f,0.85f},{0.10f,0.72f,0.22f},
        {0.90f,0.60f,0.10f},{0.65f,0.10f,0.72f},{0.85f,0.40f,0.20f},
        {0.20f,0.55f,0.60f},{0.75f,0.75f,0.10f},{0.50f,0.20f,0.60f},
        {0.30f,0.65f,0.50f},{0.90f,0.30f,0.45f},{0.15f,0.35f,0.75f}
    };

    for (int s = 0;s < 5;s++) {
        float baseY = shelfY[s] + 0.06f;
        float x = -1.05f;
        int bookIdx = (s * 7) % 12;
        if (s % 2 == 0) {
            float widths[] = { 0.14f,0.18f,0.13f,0.16f,0.15f,0.19f,0.12f };
            float heights[] = { 0.55f,0.45f,0.60f,0.40f,0.50f,0.38f,0.52f };
            for (int b = 0;b < 7;b++) {
                BookColor& c = palette[(bookIdx + b) % 12];
                setColor(c.r, c.g, c.b);
                glPushMatrix();
                glTranslatef(x + widths[b] * 0.5f, baseY + heights[b] * 0.5f, 0.05f);
                glScalef(widths[b], heights[b], 0.32f);
                glutSolidCube(1.0);
                glPopMatrix();
                setColor(c.r * 0.7f, c.g * 0.7f, c.b * 0.7f);
                glPushMatrix();
                glTranslatef(x + widths[b] * 0.5f, baseY + heights[b] * 0.5f, 0.215f);
                glScalef(widths[b] * 0.9f, heights[b] * 0.95f, 0.01f);
                glutSolidCube(1.0);
                glPopMatrix();
                x += widths[b] + 0.02f;
            }
        }
        else {
            float widths[] = { 0.16f,0.14f,0.18f,0.15f };
            float heights[] = { 0.50f,0.55f,0.42f,0.48f };
            for (int b = 0;b < 4;b++) {
                BookColor& c = palette[(bookIdx + b) % 12];
                setColor(c.r, c.g, c.b);
                glPushMatrix();
                glTranslatef(x + widths[b] * 0.5f, baseY + heights[b] * 0.5f, 0.05f);
                glScalef(widths[b], heights[b], 0.32f);
                glutSolidCube(1.0);
                glPopMatrix();
                x += widths[b] + 0.02f;
            }
            float stackX = x + 0.22f;
            for (int h = 0;h < 3;h++) {
                BookColor& c = palette[(bookIdx + 4 + h) % 12];
                setColor(c.r, c.g, c.b);
                glPushMatrix();
                glTranslatef(stackX, baseY + 0.05f + h * 0.10f, 0.05f);
                glScalef(0.42f, 0.09f, 0.30f);
                glutSolidCube(1.0);
                glPopMatrix();
            }
        }
    }

    setColor(0.70f, 0.45f, 0.20f);
    glPushMatrix();
    glTranslatef(0.85f, 2.28f, 0.05f);
    drawCylinder(0.07f, 0.22f, 12);
    glPopMatrix();
    setColor(0.20f, 0.65f, 0.25f);
    glPushMatrix();
    glTranslatef(0.85f, 2.50f, 0.05f);
    glutSolidSphere(0.09f, 8, 8);
    glPopMatrix();

    glPopMatrix();
}

// Book Rack
void drawBookRack() {
    glPushMatrix();
    glTranslatef(5.30f, 0.0f, -5.50f);

    setColor(0.38f, 0.24f, 0.09f);
    glPushMatrix(); glTranslatef(-0.28f, 1.70f, 0); glScalef(0.06f, 3.40f, 0.50f); glutSolidCube(1.0); glPopMatrix();
    glPushMatrix(); glTranslatef(0.28f, 1.70f, 0); glScalef(0.06f, 3.40f, 0.50f); glutSolidCube(1.0); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 3.38f, 0); glScalef(0.62f, 0.06f, 0.50f); glutSolidCube(1.0); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 0.04f, 0); glScalef(0.62f, 0.08f, 0.52f); glutSolidCube(1.0); glPopMatrix();
    setColor(0.30f, 0.18f, 0.07f);
    glPushMatrix(); glTranslatef(0, 1.70f, -0.23f); glScalef(0.56f, 3.30f, 0.04f); glutSolidCube(1.0); glPopMatrix();

    setColor(0.48f, 0.30f, 0.11f);
    float shelfY[] = { 0.72f, 1.38f, 2.04f, 2.70f };
    for (int s = 0; s < 4; s++) {
        glPushMatrix(); glTranslatef(0, shelfY[s], 0); glScalef(0.56f, 0.05f, 0.48f); glutSolidCube(1.0); glPopMatrix();
    }

    struct BColor { float r, g, b; };
    BColor bp[] = {
        {0.10f,0.10f,0.10f},{0.90f,0.90f,0.90f},{0.10f,0.10f,0.10f},
        {0.90f,0.90f,0.90f},{0.10f,0.10f,0.10f},{0.90f,0.90f,0.90f},
        {0.10f,0.10f,0.10f},{0.90f,0.90f,0.90f},{0.10f,0.10f,0.10f},
        {0.90f,0.90f,0.90f},{0.10f,0.10f,0.10f},{0.90f,0.90f,0.90f}
    };
    float bookW[] = { 0.10f, 0.12f, 0.09f, 0.11f, 0.10f };
    float bookH[] = { 0.48f, 0.42f, 0.52f, 0.38f, 0.45f };

    for (int s = 0; s < 4; s++) {
        float baseY = shelfY[s] + 0.05f;
        float x = -0.24f;
        for (int b = 0; b < 4; b++) {
            BColor& c = bp[(s * 4 + b) % 12];
            float w = bookW[b % 5], h = bookH[b % 5];
            setColor(c.r, c.g, c.b);
            glPushMatrix(); glTranslatef(x + w * 0.5f, baseY + h * 0.5f, 0.0f); glScalef(w, h, 0.34f); glutSolidCube(1.0); glPopMatrix();
            setColor(c.r * 0.70f, c.g * 0.70f, c.b * 0.70f);
            glPushMatrix(); glTranslatef(x + w * 0.5f, baseY + h * 0.5f, 0.175f); glScalef(w * 0.88f, h * 0.92f, 0.01f); glutSolidCube(1.0); glPopMatrix();
            x += w + 0.015f;
        }
    }

    glPopMatrix();
}

// Carpet
void drawCarpet() {
    setColor(1.0f, 0.50f, 0.0f);
    glBegin(GL_QUADS);
    glVertex3f(-1.5f, 0.01f, -1.0f);
    glVertex3f(1.5f, 0.01f, -1.0f);
    glVertex3f(1.5f, 0.01f, 2.5f);
    glVertex3f(-1.5f, 0.01f, 2.5f);
    glEnd();
    setColor(0.90f, 0.40f, 0.0f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
    glVertex3f(-1.4f, 0.02f, -0.9f);
    glVertex3f(1.4f, 0.02f, -0.9f);
    glVertex3f(1.4f, 0.02f, 2.4f);
    glVertex3f(-1.4f, 0.02f, 2.4f);
    glEnd();
    glLineWidth(1.0f);
}

// Small Side Table
void drawSideTable() {
    glPushMatrix();
    glTranslatef(-1.5f, 0, -5.5f);

    setColor(0.50f, 0.32f, 0.14f);
    glPushMatrix(); glTranslatef(0, 0.8f, 0); glScalef(0.7f, 0.07f, 0.7f); glutSolidCube(1.0); glPopMatrix();

    setColor(0.38f, 0.22f, 0.08f);
    float lx[] = { -0.3f, 0.3f, -0.3f, 0.3f };
    float lz[] = { -0.3f, -0.3f, 0.3f, 0.3f };
    for (int i = 0;i < 4;i++) {
        glPushMatrix(); glTranslatef(lx[i], 0.4f, lz[i]); glScalef(0.06f, 0.8f, 0.06f); glutSolidCube(1.0); glPopMatrix();
    }

    // Lamp
    setColor(0.82f, 0.72f, 0.18f);
    glPushMatrix(); glTranslatef(0, 1.05f, 0); glScalef(0.08f, 0.45f, 0.08f); glutSolidCube(1.0); glPopMatrix();
    if (!lightOn) setColor(1.0f, 0.70f, 0.30f);
    else setColor(0.40f, 0.35f, 0.25f);
    glPushMatrix(); glTranslatef(0, 1.40f, 0); drawCone(0.22f, 0.30f, 12); glPopMatrix();

    glPopMatrix();
}

// Ceiling Fan
void drawFan() {
    glPushMatrix();
    glTranslatef(0.0f, 4.85f, 0.0f);

    // Rod
    setColor(0.55f, 0.55f, 0.58f);
    glPushMatrix();
    glTranslatef(0, -0.05f, 0);
    drawCylinder(0.04f, 0.35f, 12);
    glPopMatrix();

    // Motor body
    setColor(0.60f, 0.60f, 0.62f);
    glPushMatrix();
    glTranslatef(0, -0.45f, 0);
    drawCylinder(0.18f, 0.22f, 20);
    glPopMatrix();

    // Fan blades
    glPushMatrix();
    glTranslatef(0, -0.36f, 0);
    glRotatef(fanAngle, 0, 1, 0);

    int numBlades = 4;
    for (int b = 0; b < numBlades; b++) {
        glPushMatrix();
        glRotatef(b * 90.0f, 0, 1, 0);

        // Blade arm
        setColor(0.50f, 0.32f, 0.10f);
        glPushMatrix();
        glTranslatef(0.18f, 0, 0);
        glRotatef(90, 0, 1, 0);
        drawCylinder(0.03f, 0.25f, 8);
        glPopMatrix();

        // Blade top
        setColor(0.65f, 0.42f, 0.18f);
        glPushMatrix();
        glTranslatef(0.72f, 0.01f, 0);
        glRotatef(-8, 0, 0, 1);
        glScalef(0.90f, 0.04f, 0.28f);
        glutSolidCube(1.0);
        glPopMatrix();

        // Blade bottom
        setColor(0.52f, 0.33f, 0.12f);
        glPushMatrix();
        glTranslatef(0.72f, -0.01f, 0);
        glRotatef(-8, 0, 0, 1);
        glScalef(0.90f, 0.04f, 0.28f);
        glutSolidCube(1.0);
        glPopMatrix();

        glPopMatrix();
    }

    glPopMatrix();
    glPopMatrix();
}

// Tube Light
void drawTubelight() {
    glPushMatrix();
    glTranslatef(-4.0f, 4.65f, -5.95f);

    setColor(0.95f, 0.95f, 0.97f);
    glPushMatrix(); glScalef(3.20f, 0.28f, 0.32f); glutSolidCube(1.0); glPopMatrix();

    setColor(1.0f, 1.0f, 1.0f);
    glPushMatrix(); glTranslatef(0, 0, 0.17f); glScalef(2.90f, 0.15f, 0.01f); glutSolidCube(1.0); glPopMatrix();

    glPopMatrix();
}

// Digital Wall Clock
void drawDigitalClock() {
    glPushMatrix();

    // Clock above bed
    glTranslatef(-3.5f, 3.8f, -5.9f);

    glRotatef(0.0f, 0.0f, 1.0f, 0.0f);

    // Outer Frame
    setColor(0.15f, 0.15f, 0.15f);
    glBegin(GL_QUADS);
    glVertex3f(-0.40f, -0.15f, 0.0f);
    glVertex3f(0.40f, -0.15f, 0.0f);
    glVertex3f(0.40f, 0.15f, 0.0f);
    glVertex3f(-0.40f, 0.15f, 0.0f);
    glEnd();

    // Screen
    setColor(0.02f, 0.05f, 0.02f);
    glBegin(GL_QUADS);
    glVertex3f(-0.35f, -0.10f, 0.001f);
    glVertex3f(0.35f, -0.10f, 0.001f);
    glVertex3f(0.35f, 0.10f, 0.001f);
    glVertex3f(-0.35f, 0.10f, 0.001f);
    glEnd();

    // Time Text
    glPushMatrix();

    glTranslatef(-0.15f, -0.03f, 0.002f);

    setColor(1.0f, 1.0f, 1.0f);
    glLineWidth(2.5f);
    glDisable(GL_LIGHTING);
    glScalef(0.0012f, 0.0012f, 0.001f);

    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    char timeStr[6];
    strftime(timeStr, sizeof(timeStr), "%H:%M", t);

    for (int i = 0; timeStr[i] != '\0'; i++)
        glutStrokeCharacter(GLUT_STROKE_ROMAN, timeStr[i]);
    glEnable(GL_LIGHTING);

    glLineWidth(1.0f);

    glPopMatrix();

    glPopMatrix();
}

// Picture Frame on back wall
void drawPicture() {
    setColor(0.10f, 0.10f, 0.10f);
    drawQuad(-4.2f, 2.0f, -5.99f, -2.5f, 2.0f, -5.99f, -2.5f, 3.6f, -5.99f, -4.2f, 3.6f, -5.99f);

    setColor(0.20f, 0.80f, 0.20f);
    drawQuad(-4.05f, 2.15f, -5.98f, -2.65f, 2.15f, -5.98f, -2.65f, 3.45f, -5.98f, -4.05f, 3.45f, -5.98f);

    setColor(1.0f, 0.20f, 0.20f);
    glPushMatrix(); glTranslatef(-3.35f, 2.80f, -5.97f); glutSolidSphere(0.35f, 16, 16); glPopMatrix();
}

// Lighting Setup
void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_LIGHT0);
    float pos[] = { -3.5f, 4.0f, -3.5f, 1.0f };
    float amb[] = { 0.15f, 0.18f, 0.20f, 1.0f };
    float spec[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    float diff[4];
    if (lightOn) { diff[0] = 1.0f; diff[1] = 1.0f; diff[2] = 1.0f; diff[3] = 1.0f; }
    else { diff[0] = 0.15f; diff[1] = 0.15f; diff[2] = 0.15f; diff[3] = 1.0f; }
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

    if (!lightOn) {
        glEnable(GL_LIGHT1);
        float posL[] = { -1.5f, 1.5f, -5.5f, 1.0f };
        float diffL[] = { 1.0f, 0.60f, 0.20f, 1.0f };
        float ambL[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        float specL[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glLightfv(GL_LIGHT1, GL_POSITION, posL);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, diffL);
        glLightfv(GL_LIGHT1, GL_AMBIENT, ambL);
        glLightfv(GL_LIGHT1, GL_SPECULAR, specL);
    }
    else {
        glDisable(GL_LIGHT1);
    }
}

// Display
void display() {
    glClearColor(0.12f, 0.12f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glRotatef(camAngleY, 0, 1, 0);
    glTranslatef(-camX, -camY, -camZ);

    setupLighting();

    drawRoom();
    drawSingleWindowWithCurtain();
    drawDoor();
    drawBed();
    drawSideTable();
    drawTable();
    drawChair();
    drawBookshelf();
    drawBookRack();
    drawCarpet();
    drawFan();
    drawTubelight();
    drawDigitalClock();
    drawPicture();

    glDisable(GL_LIGHTING);
    glColor3f(1, 1, 1);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1000, 0, 700);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glutSwapBuffers();
}

// Idle
void idle() {
    if (fanOn) {
        fanAngle += 1.0f;
        if (fanAngle >= 360.0f) fanAngle -= 360.0f;
    }
    glutPostRedisplay();
}

// Reshape
void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)w / h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// Keyboard Controls
void keyboard(unsigned char key, int x, int y) {
    float speed = 0.3f;
    switch (key) {
    case 'w': case 'W': camZ -= speed; break;
    case 's': case 'S': camZ += speed; break;
    case 'a': case 'A': camX -= speed; break;
    case 'd': case 'D': camX += speed; break;
    case 'l': case 'L': lightOn = !lightOn; break;
    case 'f': case 'F': fanOn = !fanOn; break;
    case 27: exit(0);
    }
    glutPostRedisplay();
}

// Main 
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 700);
    glutInitWindowPosition(100, 50);
    glutCreateWindow("Interior Design of a Room");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}