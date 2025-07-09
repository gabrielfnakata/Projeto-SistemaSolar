#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>

int winWidth = 800, winHeight = 600;

float camDistance = 20.0f;
float camAngleX = 20.0f, camAngleY = -30.0f;
int lastMouseX, lastMouseY;
bool rotating = false;
bool paused = false;
bool nightMode = false;

double timeElapsed = 0.0;

typedef struct Celestial {
    std::string name;
    float radius;        
    float distance;        
    float orbitSpeed;      
    float selfSpeed;    
    glm::vec3 color;
    std::vector<Celestial> satellites;
} Celestial;

std::vector<Celestial> bodies;

void drawText(const std::string &text, float x, float y) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); glLoadIdentity();
    gluOrtho2D(0, winWidth, 0, winHeight);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix(); glLoadIdentity();
    glRasterPos2f(x, y);
    for(char c: text) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void initBodies() {
    Celestial sun = {"Sol", 2.0f, 0.0f, 0.0f, 10.0f, {1.0f, 1.0f, 0.0f}};
    Celestial earth = {"Terra", 0.6f, 8.0f, 30.0f, 60.0f, {0.2f, 0.2f, 1.0f}};
    Celestial moon = {"Lua", 0.16f, 1.2f, 80.0f, 80.0f, {0.8f, 0.8f, 0.8f}};
    earth.satellites.push_back(moon);
    Celestial mars = {"Marte", 0.4f, 12.0f, 24.0f, 40.0f, {1.0f, 0.3f, 0.0f}};
    bodies.push_back(sun);
    bodies.push_back(earth);
    bodies.push_back(mars);
}

void setLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat pos[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
}

void displayBody(const Celestial &body) {
    glColor3fv(&body.color[0]);
    glutSolidSphere(body.radius, 32, 32);
    
    for(const auto &sat : body.satellites) {
        glPushMatrix();
        double angleOrbit = timeElapsed * sat.orbitSpeed;
        glRotatef(angleOrbit, 0.0f, 1.0f, 0.0f);
        glTranslatef(sat.distance, 0, 0);
        glRotatef(timeElapsed * sat.selfSpeed, 0.0f, 1.0f, 0.0f);
        displayBody(sat);
        glPopMatrix();
    }
}

void display() {
    glClearColor(nightMode ? 0.0f : 0.1f, 0.0f, nightMode ? 0.0f : 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -camDistance);
    glRotatef(camAngleX, 1.0f, 0.0f, 0.0f);
    glRotatef(camAngleY, 0.0f, 1.0f, 0.0f);

    if(!paused) timeElapsed += 0.016; 

    setLighting();
    

    for(const auto &body : bodies) {
        glPushMatrix();
        if(body.distance > 0) {
            double angleOrbit = timeElapsed * body.orbitSpeed;
            glRotatef(angleOrbit, 0.0f, 1.0f, 0.0f);
            glTranslatef(body.distance, 0, 0);
        }
        glRotatef(timeElapsed * body.selfSpeed, 0.0f, 1.0f, 0.0f);
        displayBody(body);
        glPopMatrix();
    }

    drawText("Press 'p' to pause/resume, 'n' for night mode", 10, winHeight - 20);

    glutSwapBuffers();
}

void reshape(int w, int h) {
    winWidth = w; winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w/h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void mouse(int button, int state, int x, int y) {
    if(button == GLUT_LEFT_BUTTON) {
        if(state == GLUT_DOWN) {
            rotating = true;
            lastMouseX = x; lastMouseY = y;
        } else rotating = false;
    } else if(button == 3) { 
        camDistance -= 1.0f;
    } else if(button == 4) { 
        camDistance += 1.0f;
    }
    glutPostRedisplay();
}

void motion(int x, int y) {
    if(rotating) {
        camAngleY += (x - lastMouseX) * 0.5f;
        camAngleX += (y - lastMouseY) * 0.5f;
        lastMouseX = x; lastMouseY = y;
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case 'p': paused = !paused; break;
        case 'n': nightMode = !nightMode; break;
        case 27: exit(0);
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Sistema Solar 3D");
    glEnable(GL_DEPTH_TEST);

    initBodies();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(display);

    glutMainLoop();
    return 0;
}
