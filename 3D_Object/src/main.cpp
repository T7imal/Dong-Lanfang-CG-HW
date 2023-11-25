#include <GL/glut.h>
#include <fstream>
#include "Model.h"
#include <math.h>

Model* g_model;

void drawModel() {
    g_model->draw();
}

double x = -5.0, y = 0.0, z = 0.0;
double lx = 1.0, ly = 0.0, lz = 0.0;
double angle = 0.0;

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);    //投影变换
    glLoadIdentity();               //重置投影矩阵
    //设置可视空间
    gluPerspective(45, (double)w / (double)h, 0.1, 100); //角度45， 窗口纵横比， 眼睛所及距离（近和远）

    glMatrixMode(GL_MODELVIEW);     //模型变换
    glLoadIdentity();
    //相机（人眼）位置， 眼睛看的点（相机位置+视线方向）， 观察者本身方向（角度，比如正立）
    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0, 0, 1);
}

void keyboard(unsigned char key, int a, int b) {
    // glMatrixMode(GL_MODELVIEW);
    switch (key) {
    case 'q':
        angle += 0.1;
        ly = sin(angle);
        lx = cos(angle);
        glLoadIdentity();
        gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0, 0, 1);
        break;
    case 'e':
        angle -= 0.1;
        ly = sin(angle);
        lx = cos(angle);
        glLoadIdentity();
        gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0, 0, 1);
        break;
    case 'w':
        x += lx * 0.05;
        y += ly * 0.05;
        glLoadIdentity();
        gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0, 0, 1);
        break;
    case 's':
        x -= lx * 0.05;
        y -= ly * 0.05;
        glLoadIdentity();
        gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0, 0, 1);
        break;
    case 'a':
        x -= ly * 0.05;
        y += lx * 0.05;
        glLoadIdentity();
        gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0, 0, 1);
        break;
    case 'd':
        x += ly * 0.05;
        y -= lx * 0.05;
        glLoadIdentity();
        gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0, 0, 1);
        break;
    }
}

void special(GLint key, GLint a, GLint b) {
    switch (key) {
    case GLUT_KEY_UP:
        z += 0.05;
        glLoadIdentity();
        gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0, 0, 1);
        break;
    case GLUT_KEY_DOWN:
        z -= 0.05;
        glLoadIdentity();
        gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0, 0, 1);
        break;
    }
}

int main(int argc, char** argv) {
    Model model("../resource/luweiqi/luweiqi.txt");
    g_model = &model;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("3D Object");
    model.load_textures();
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glutDisplayFunc(drawModel);
    glutIdleFunc(drawModel);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutMainLoop();
    return 0;
}