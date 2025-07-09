
#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#else
#include <iostream>
#endif

#include <GL/glut.h>
#include <cmath>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

GLuint carregarTextura(const char* caminho); 

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
    {"Mercurio", {0.6f, 0.6f, 0.6f}, 0.2f, 4.0f, 0.0f, 4.0f, false, 0.0f, 0.0f, 0.2f, 0.03f},
    {"Venus",    {1.0f, 0.5f, 0.0f}, 0.6f, 5.6f, 0.0f, 1.6f, false, 0.0f, 0.0f, -0.02f, 1714.8f},
    {"Terra",    {0.0f, 0.0f, 1.0f}, 0.36f, 7.2f, 0.0f, 1.0f, true, 0.0f, 0.0f, 1.0f, 23.5f},
    {"Marte",    {1.0f, 0.0f, 0.0f}, 0.28f, 17.6f, 0.0f, 0.53f, true, 0.0f, 0.0f, 0.98f, 25.2f},
    {"Jupiter",  {0.9f, 0.8f, 0.6f}, 0.8f, 12.0f, 0.0f, 0.084f, true, 0.0f, 0.0f, 2.5f, 3.1f},
    {"Saturno",  {1.0f, 1.0f, 0.6f}, 0.7f, 14.8f, 0.0f, 0.034f, true, 0.0f, 0.0f, 2.3f, 26.7f},
    {"Urano",    {0.5f, 1.0f, 1.0f}, 0.6f, 17.6f, 0.0f, 0.012f, true, 0.0f, 0.0f, -1.5f, 97.8f},
    {"Netuno",   {0.6f, 0.5f, 1.0f}, 0.6f, 20.4f, 0.0f, 0.006f, true, 0.0f, 0.0f, 1.8f, 28.3f},
};

GLuint texturas[8];
GLuint texturaSol;
GLuint texturaLua;

float cameraDistance = 45.0f;
float cameraAngleX = 20.0f;
float cameraAngleY = 0.0f;
bool mousePressed = false;
int lastMouseX = 0, lastMouseY = 0;
bool animacaoPausada = false;
int larguraJanela = 1280;
int alturaJanela = 720;
int tempoAnterior = 0;

void desenhaEsfera(float raio, int texturaIndex, GLuint texturaEspecifica = 0) {
    GLuint texturaAtiva = 0;
    
    if (texturaEspecifica != 0) {
        texturaAtiva = texturaEspecifica;
    } else if (texturaIndex >= 0) {
        texturaAtiva = texturas[texturaIndex];
    }
    
    if (texturaAtiva != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texturaAtiva);
    }
    
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluSphere(quad, raio, 40, 40);
    gluDeleteQuadric(quad);
    
    if (texturaAtiva != 0) {
        glDisable(GL_TEXTURE_2D);
    }
}

void desenharOrbita(float raio) {
    glColor3f(0.8f, 0.8f, 0.8f);
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
    glColor3f(1.0f, 1.0f, 1.0f);
    desenhaEsfera(1.2f, -1, texturaSol);
    glPopMatrix();

    for (int i = 0; i < 8; i++) {
        Planeta& p = planetas[i];
        desenharOrbita(p.distancia);

        glPushMatrix();
        glRotatef(p.anguloOrbita, 0, 1, 0);
        glTranslatef(p.distancia, 0, 0);
        glRotatef(p.inclinacaoEixo, 0, 0, 1);
        glRotatef(p.rotacaoEixo, 0, 1, 0);

        glColor3f(1.0f, 1.0f, 1.0f);
        desenhaEsfera(p.raio, i);

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
            desenharOrbita(p.raio + 0.8f);
            glPopMatrix();
        }

        if (p.temLua) {
            glPushMatrix();
            glRotatef(p.anguloLua, 0, 1, 0);
            glTranslatef(p.raio + 0.4f, 0, 0);
            glColor3f(1.0f, 1.0f, 1.0f);
            desenhaEsfera(0.07f, -1, texturaLua);
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

    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

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
    else if (botao == 3 && estado == GLUT_DOWN)
        cameraDistance -= 1.0f;
    else if (botao == 4 && estado == GLUT_DOWN)
        cameraDistance += 1.0f;

    if (cameraDistance < 5.0f) cameraDistance = 5.0f;
    if (cameraDistance > 200.0f) cameraDistance = 200.0f;

    lastMouseX = x;
    lastMouseY = y;
}


void movimentoMouse(int x, int y) {
    if (mousePressed) {
        float deltaX = x - lastMouseX;
        float deltaY = y - lastMouseY;
        cameraAngleY += deltaX * 0.6f;
        cameraAngleX += deltaY * 0.6f;
        if (cameraAngleX > 89.0f) cameraAngleX = 89.0f;
        if (cameraAngleX < -89.0f) cameraAngleX = -89.0f;
        lastMouseX = x;
        lastMouseY = y;
    }
}

void tocarSomAmbienteEspacial(float camX, float camZ);

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
                p.anguloLua += 12.0f * deltaTime * 20.0f;
        }
    }
    float camX = cameraDistance * cos(cameraAngleX * M_PI / 180.0f) * sin(cameraAngleY * M_PI / 180.0f);
float camZ = cameraDistance * cos(cameraAngleX * M_PI / 180.0f) * cos(cameraAngleY * M_PI / 180.0f);
tocarSomAmbienteEspacial(camX, camZ);
glutPostRedisplay();
    glutTimerFunc(16, atualizar, 0);
}

void init() {
    glEnable(GL_DEPTH_TEST);

    const char* nomes[] = {"mercurio", "venus", "terra", "marte", "jupiter", "saturno", "urano", "netuno"};
    
    for (int i = 0; i < 8; i++) {
        char caminho[256];
        sprintf(caminho, "texturas/%s.jpg", nomes[i]);
        texturas[i] = carregarTextura(caminho);
        
        if (texturas[i] == 0) {
            printf("Falha ao carregar textura: %s\n", caminho);
        } else {
            printf("Textura carregada com sucesso: %s (ID: %u)\n", caminho, texturas[i]);
        }
    }
    
    texturaSol = carregarTextura("texturas/sol.jpg");
    if (texturaSol == 0) {
        printf("Falha ao carregar textura do Sol - usando cor amarela\n");
    } else {
        printf("Textura do Sol carregada com sucesso (ID: %u)\n", texturaSol);
    }
    
    texturaLua = carregarTextura("texturas/lua.jpg");
    if (texturaLua == 0) {
        printf("Falha ao carregar textura da Lua - usando cor cinza\n");
    } else {
        printf("Textura da Lua carregada com sucesso (ID: %u)\n", texturaLua);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(larguraJanela, alturaJanela);
    glutCreateWindow("Sistema Solar 3D");
    configurarCena();

#ifdef _WIN32
    PlaySound(TEXT("espaço.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
#else
    std::cout << "Som de fundo não disponível no Linux" << std::endl;
#endif

    init();
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

void tocarSomAmbienteEspacial(float camX, float camZ) {
#ifdef _WIN32
    float dist = sqrt(camX * camX + camZ * camZ);
    int volume = 1000 - std::min((int)(dist * 300), 0x7FFF);
    waveOutSetVolume(NULL, MAKELONG(volume, volume));
#endif
}

GLuint carregarTextura(const char* caminho) {
    int largura, altura, canais;
    unsigned char* imagem = stbi_load(caminho, &largura, &altura, &canais, 0);

    if (!imagem) {
        printf("Erro ao carregar imagem: %s\n", caminho);
        return 0;
    }

    GLuint texturaID;
    glGenTextures(1, &texturaID);
    glBindTexture(GL_TEXTURE_2D, texturaID);

    GLenum formato = (canais == 3) ? GL_RGB : GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, formato, largura, altura, 0, formato, GL_UNSIGNED_BYTE, imagem);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_image_free(imagem);
    return texturaID;
}

