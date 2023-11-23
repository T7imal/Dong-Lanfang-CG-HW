#include <GL/glut.h>
#include <cmath>
#include <iostream>

// 控制点
float P0[2] = { 1.0, 1.0 };
float P1[2] = { 2.0, 2.0 };
float DP0[2] = { 3.0, 3.0 };
float DP1[2] = { 4.0, 4.0 };
float B0[2] = { P0[0], P0[1] };
float B1[2] = { P0[0] + DP0[0] / 3, P0[1] + DP0[1] / 3 };
float B2[2] = { P1[0] - DP1[0] / 3, P1[1] - DP1[1] / 3 };
float B3[2] = { P1[0], P1[1] };

// Hermit 曲线参数方程
float X_Hermit(float t) {
    return (2 * pow(t, 3) - 3 * pow(t, 2) + 1) * P0[0] + (-2 * pow(t, 3) + 3 * pow(t, 2)) * P1[0] + (pow(t, 3) - 2 * pow(t, 2) + t) * DP0[0] + (pow(t, 3) - pow(t, 2)) * DP1[0];
}

float Y_Hermit(float t) {
    return (2 * pow(t, 3) - 3 * pow(t, 2) + 1) * P0[1] + (-2 * pow(t, 3) + 3 * pow(t, 2)) * P1[1] + (pow(t, 3) - 2 * pow(t, 2) + t) * DP0[1] + (pow(t, 3) - pow(t, 2)) * DP1[1];
}

float X_Bezier(float t) {
    return pow(1 - t, 3) * B0[0] + 3 * t * pow(1 - t, 2) * B1[0] + 3 * pow(t, 2) * (1 - t) * B2[0] + pow(t, 3) * B3[0];
}

float Y_Bezier(float t) {
    return pow(1 - t, 3) * B0[1] + 3 * t * pow(1 - t, 2) * B1[1] + 3 * pow(t, 2) * (1 - t) * B2[1] + pow(t, 3) * B3[1];
}

void drawCoordinate() {
    glColor3f(0.5, 0.5, 0.5); // 设置绘制颜色为灰
    glBegin(GL_LINES);
    for (float i = 0.0; i < 3.01; i += 0.1) {
        glVertex2f(i, 0.0);
        glVertex2f(i, 3.0);
        glVertex2f(0.0, i);
        glVertex2f(3.0, i);
    }
    glEnd();
    glColor3f(1.0, 1.0, 1.0); // 设置绘制颜色为白色
    glBegin(GL_LINES);
    for (float i = 0.0; i < 3.01; i += 1.0) {
        glVertex2f(i, 0.0);
        glVertex2f(i, 3.0);
        glVertex2f(0.0, i);
        glVertex2f(3.0, i);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawCoordinate();
    glColor3f(0.0, 1.0, 0.0); // 设置绘制颜色为绿色

    glBegin(GL_LINE_STRIP);
    for (float t = 0.0; t <= 1.0; t += 0.01) {
        glVertex2f(X_Hermit(t), Y_Hermit(t));
        // std::cout << "(" << X_Hermit(t) << ", " << Y_Hermit(t) << ")" << std::endl;
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (float t = 0.0; t <= 1.0; t += 0.01) {
        glVertex2f(X_Bezier(t) + 1.0, Y_Bezier(t));
        // std::cout << "(" << X_Bezier(t) << ", " << Y_Bezier(t) << ")" << std::endl;
    }
    glEnd();

    glPointSize(10.0);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POINTS);
    glVertex2f(B0[0] + 1.0, B0[1]);
    glVertex2f(B1[0] + 1.0, B1[1]);
    glVertex2f(B2[0] + 1.0, B2[1]);
    glVertex2f(B3[0] + 1.0, B3[1]);
    glEnd();



    glFlush();
}


void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0); // 设置清屏颜色为黑色
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 3.0, 0.0, 3.0); // 设置视口和投影矩阵
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hermit Bezier Curve");
    init();
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}
