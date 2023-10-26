#include <iostream>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

// 粒子的结构体
typedef struct {
    float position[3]; // 粒子位置
    float speed[3]; // 粒子速度
    float color[3]; // 粒子颜色
    int time; // 粒子存活时间
} Water_Point;

// 粒子数组
Water_Point waterPoint[1000];

// 粒子存活时间
int waterPointLifeTime = 200;

// 粒子半径
float waterPointRadius = 0.1;

// 粒子个数
int waterPointNum = 1000;

// 粒子发射器位置
float emitterPosition[3] = { 0.0, 0.0, 0.0 };

// 粒子发射器速度
float emitterSpeed[3] = { 0.0, 10.0, 0.0 };

// 粒子发射器半径
float emitterRadius = 0.2;

int x = 0;

// 粒子系统初始化
void initFountain(int a, int b) {
    for (int i = a; i < b; i++) {
        // 粒子位置
        waterPoint[i].position[0] = emitterPosition[0];
        waterPoint[i].position[1] = emitterPosition[1];
        waterPoint[i].position[2] = emitterPosition[2];
        // 粒子速度
        waterPoint[i].speed[0] = emitterSpeed[0] + rand() % 10 - 5;
        waterPoint[i].speed[1] = emitterSpeed[1] + rand() % 10 - 5;
        waterPoint[i].speed[2] = emitterSpeed[2] + rand() % 10 - 5;
    }
}

// 粒子系统更新
void updateFountain() {
    initFountain(x, x + waterPointNum / waterPointLifeTime);
    x += waterPointNum / waterPointLifeTime;
    if (x >= waterPointNum)
        x = 0;
    for (int i = 0; i < waterPointNum; i++) {
        // 粒子存活时间
        waterPoint[i].time += 1;

        // 粒子位置
        waterPoint[i].position[0] += waterPoint[i].speed[0] * 0.01;
        waterPoint[i].position[1] += waterPoint[i].speed[1] * 0.01;
        waterPoint[i].position[2] += waterPoint[i].speed[2] * 0.01;

        // 粒子速度
        waterPoint[i].speed[0] += 0.0;
        waterPoint[i].speed[1] += -9.8 * 0.01;
        waterPoint[i].speed[2] += 0.0;

        // 粒子颜色
        waterPoint[i].color[0] = 1.0;
        waterPoint[i].color[1] = 1.0;
        waterPoint[i].color[2] = 1.0;

        // 粒子落地
        if (waterPoint[i].position[1] < 0) {
            waterPoint[i].position[1] = 0;
            waterPoint[i].speed[1] = -waterPoint[i].speed[1] / 2;
        }
    }
}

// 粒子系统绘制
void drawFountain() {
    for (int i = 0; i < waterPointNum; i++) {
        glPushMatrix();
        glTranslatef(waterPoint[i].position[0], waterPoint[i].position[1], waterPoint[i].position[2]);
        glColor3f(waterPoint[i].color[0], waterPoint[i].color[1], waterPoint[i].color[2]);
        glutSolidSphere(waterPointRadius, 10, 10);
        glPopMatrix();
    }
}

// 粒子系统
void fountain() {
    updateFountain();
    drawFountain();
}

// 场景绘制
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0, 10, 0);
    fountain();
    glutSwapBuffers();
}

// 窗口大小变化
void reshape(int w, int h) {
    if (h == 0) {
        h = 1;
    }
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0 * w / h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// 窗口初始化
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    initFountain(0, waterPointNum / waterPointLifeTime);
}

// 窗口主函数
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Fountain");
    init();
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}