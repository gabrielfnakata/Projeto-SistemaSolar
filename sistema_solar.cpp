
#include <windows.h>
#include <GL/glut.h>
#include <cmath>
#include <string>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct Planeta {
    std::string nome;
    float cor[3];
    float raio;
    float distancia;
    float anguloOrbita;
    float velocidadeOrbita;
    bool temLua;
    float anguloLua;
    float rotacaoEixo;
    float velocidadeRotacao;
    float inclinacaoEixo;
};

Planeta planetas[] = {
    {"Mercurio", {0.6f, 0.6f, 0.6f}, 0.1f, 2.0f, 0.0f, 4.0f, false, 0.0f, 0.0f, 0.1f, 0.03f},
    {"Venus",    {1.0f, 0.5f, 0.0f}, 0.15f, 2.8f, 0.0f, 1.6f, false, 0.0f, 0.0f, -0.02f, 177.4f},
    {"Terra",    {0.0f, 0.0f, 1.0f}, 0.18f, 3.6f, 0.0f, 1.0f, true, 0.0f, 0.0f, 1.0f, 23.5f},
    {"Marte",    {1.0f, 0.0f, 0.0f}, 0.14f, 4.4f, 0.0f, 0.53f, true, 0.0f, 0.0f, 0.98f, 25.2f},
    {"Jupiter",  {0.9f, 0.8f, 0.6f}, 0.4f, 6.0f, 0.0f, 0.084f, true, 0.0f, 0.0f, 2.5f, 3.1f},
    {"Saturno",  {1.0f, 1.0f, 0.6f}, 0.35f, 7.4f, 0.0f, 0.034f, true, 0.0f, 0.0f, 2.3f, 26.7f},
    {"Urano",    {0.5f, 1.0f, 1.0f}, 0.3f, 8.8f, 0.0f, 0.012f, true, 0.0f, 0.0f, -1.5f, 97.8f},
    {"Netuno",   {0.3f, 0.5f, 1.0f}, 0.3f, 10.2f, 0.0f, 0.006f, true, 0.0f, 0.0f, 1.8f, 28.3f},
};

float cameraDistance = 25.0f;
float cameraAngleX = 20.0f;
float cameraAngleY = 0.0f;
bool mousePressed = false;
int lastMouseX = 0, lastMouseY = 0;
bool animacaoPausada = false;
int larguraJanela = 1280;
int alturaJanela = 720;
int tempoAnterior = 0;

void desenhaEsfera(float raio) {
    GLUquadric* quad = gluNewQuadric();
    glDisable(GL_TEXTURE_2D);
    gluSphere(quad, raio, 40, 40);
    gluDeleteQuadric(quad);
}

void desenharOrbita(float raio) {
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; ++i) {
        float ang = 2 * M_PI * i / 100;
        float x = cos(ang) * raio;
        float z = sin(ang) * raio;
        glVertex3f(x, 0.0f, z);
    }
    glEnd();
}

void desenharSistemaSolar() {
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 0.0f);
    desenhaEsfera(1.2f);
    glPopMatrix();

    for (int i = 0; i < 8; i++) {
        Planeta& p = planetas[i];
        desenharOrbita(p.distancia);

        glPushMatrix();
        glRotatef(p.anguloOrbita, 0, 1, 0);
        glTranslatef(p.distancia, 0, 0);
        glRotatef(p.inclinacaoEixo, 0, 0, 1);
        glRotatef(p.rotacaoEixo, 0, 1, 0);
        glColor3fv(p.cor);
        desenhaEsfera(p.raio);

        if (p.nome == "Saturno") {
            glPushMatrix();
            glColor3f(1.0f, 1.0f, 0.8f);
            glRotatef(90.0f, 1, 0, 0);
            desenharOrbita(p.raio + 0.5f);
            glPopMatrix();
        }

        if (p.nome == "Urano") {
            glPushMatrix();
            glColor3f(0.5f, 1.0f, 1.0f);
            glRotatef(90.0f, 1, 0, 0);
            desenharOrbita(p.raio + 0.4f);
            glPopMatrix();
        }

        if (p.temLua) {
            glPushMatrix();
            glRotatef(p.anguloLua, 0, 1, 0);
            glTranslatef(p.raio + 0.4f, 0, 0);
            glColor3f(0.8f, 0.8f, 0.8f);
            desenhaEsfera(0.07f);
            glPopMatrix();
        }

        glPopMatrix();
    }
}

void configurarCena() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 1);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHT0);

    GLfloat luz_posicao[] = { 0.0f, 10.0f, 10.0f, 1.0f };
    GLfloat luz_ambiente[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat luz_difusa[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat luz_especular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, luz_posicao);
    glLightfv(GL_LIGHT0, GL_AMBIENT, luz_ambiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luz_difusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luz_especular);
}

void desenharCena() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    double radX = cameraAngleX * M_PI / 180.0;
    double radY = cameraAngleY * M_PI / 180.0;
    float camX = static_cast<float>(cameraDistance * cos(radX) * sin(radY));
    float camY = static_cast<float>(cameraDistance * sin(radX));
    float camZ = static_cast<float>(cameraDistance * cos(radX) * cos(radY));

    gluLookAt(camX, camY, camZ, 0, 0, 0, 0, 1, 0);
    desenharSistemaSolar();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    larguraJanela = w;
    alturaJanela = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void teclado(unsigned char tecla, int x, int y) {
    if (tecla == 32) animacaoPausada = !animacaoPausada;
}

void mouse(int botao, int estado, int x, int y) {
    if (botao == GLUT_LEFT_BUTTON)
        mousePressed = (estado == GLUT_DOWN);
    lastMouseX = x;
    lastMouseY = y;
}

void movimentoMouse(int x, int y) {
    if (mousePressed) {
        float deltaX = x - lastMouseX;
        float deltaY = y - lastMouseY;
        cameraAngleY += deltaX * 0.3f;
        cameraAngleX += deltaY * 0.3f;
        if (cameraAngleX > 89.0f) cameraAngleX = 89.0f;
        if (cameraAngleX < -89.0f) cameraAngleX = -89.0f;
        lastMouseX = x;
        lastMouseY = y;
    }
}

void atualizar(int valor) {
    int tempoAtual = glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = (tempoAtual - tempoAnterior) / 1000.0f;
    tempoAnterior = tempoAtual;
    if (!animacaoPausada) {
        for (int i = 0; i < 8; i++) {
            Planeta& p = planetas[i];
            p.anguloOrbita += p.velocidadeOrbita * deltaTime * 20.0f;
            p.rotacaoEixo += p.velocidadeRotacao * deltaTime * 20.0f;
            if (p.temLua)
                p.anguloLua += 6.0f * deltaTime * 20.0f;
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, atualizar, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(larguraJanela, alturaJanela);
    glutCreateWindow("Sistema Solar 3D - GLUT (Somente Cores)");
    configurarCena();
    glutDisplayFunc(desenharCena);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(teclado);
    glutMouseFunc(mouse);
    glutMotionFunc(movimentoMouse);
    tempoAnterior = glutGet(GLUT_ELAPSED_TIME);
    glutTimerFunc(16, atualizar, 0);
    glutMainLoop();
    return 0;
}