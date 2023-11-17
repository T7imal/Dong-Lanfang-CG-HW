
#include <GL/glut.h>
#include <vector>
#include <stack>
#include <list>
#include <chrono>
#include <thread>

enum type {
    BOUNDARY,
    FILL,
    NULL_POINT
};

#define WIDTH 640
#define HEIGHT 480

std::vector<std::vector<type>> points;          //点的类型
// std::vector<std::pair<int, int>> fill_points;   //填充区域的点
// std::vector<std::pair<int, int>> boundary;      //边界点
std::stack<std::pair<int, int>> stack;          //扫描线栈

void update() {
    if (stack.empty()) {
        return;
    }

    auto point = stack.top();
    stack.pop();
    int x = point.first;
    int y = point.second;
    if (points[x][y] != NULL_POINT) {
        return;
    }
    int tmp = x;
    while (points[tmp][y] != BOUNDARY) {
        points[tmp][y] = FILL;
        if (points[tmp + 1][y + 1] == BOUNDARY && points[tmp][y + 1] == NULL_POINT) {
            stack.push({ tmp , y + 1 });
        }
        if (points[tmp + 1][y - 1] == BOUNDARY && points[tmp][y - 1] == NULL_POINT) {
            stack.push({ tmp, y - 1 });
        }
        tmp++;
    }
    if (points[tmp - 1][y + 1] == NULL_POINT) {
        stack.push({ tmp - 1, y + 1 });
    }
    if (points[tmp - 1][y - 1] == NULL_POINT) {
        stack.push({ tmp - 1, y - 1 });
    }

    tmp = x - 1;
    while (points[tmp][y] != BOUNDARY) {
        points[tmp][y] = FILL;
        if (points[tmp - 1][y + 1] == BOUNDARY && points[tmp][y + 1] == NULL_POINT) {
            stack.push({ tmp , y + 1 });
        }
        if (points[tmp - 1][y - 1] == BOUNDARY && points[tmp][y - 1] == NULL_POINT) {
            stack.push({ tmp , y - 1 });
        }
        tmp--;
    }
    if (points[tmp + 1][y + 1] == NULL_POINT) {
        stack.push({ tmp + 1, y + 1 });
    }
    if (points[tmp + 1][y - 1] == NULL_POINT) {
        stack.push({ tmp + 1, y - 1 });
    }

    return;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(3.0);
    glBegin(GL_POINTS);
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            if (points[i][j] == BOUNDARY) {
                glVertex2i(i, j);
            }
        }
    }
    glEnd();
    glColor3f(0.0, 0.0, 1.0);
    glPointSize(3.0);
    glBegin(GL_POINTS);
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            if (points[i][j] == FILL) {
                glVertex2i(i, j);
            }
        }
    }
    glEnd();
    glFlush();
    update();
}

void init() {
    points.resize(WIDTH);
    for (auto& row : points) {
        row.resize(HEIGHT);
    }
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            points[i][j] = NULL_POINT;
        }
    }
    for (int i = 100; i < 200; i++) {
        points[i][100] = BOUNDARY;
    }
    for (int i = 100; i < 200; i++) {
        points[i][200] = BOUNDARY;
    }
    for (int j = 100; j < 200; j++) {
        points[100][j] = BOUNDARY;
    }
    for (int j = 100; j < 200; j++) {
        points[200][j] = BOUNDARY;
    }
    for (int i = 125; i < 175; i++) {
        points[i][125] = BOUNDARY;
    }
    for (int i = 125; i < 175; i++) {
        points[i][175] = BOUNDARY;
    }
    for (int j = 125; j < 175; j++) {
        points[125][j] = BOUNDARY;
    }
    for (int j = 125; j < 175; j++) {
        points[175][j] = BOUNDARY;
    }
    stack.push({ 110,110 });
}

int main(int argc, char** argv) {
    init();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("OpenGL Window");
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, WIDTH / 2, 0, HEIGHT / 2);
    // glutDisplayFunc(display);
    glutIdleFunc(display);
    glutMainLoop();
    return 0;
}
