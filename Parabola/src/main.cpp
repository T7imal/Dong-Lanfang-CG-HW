#include <GL/glut.h>
#include <vector>

std::vector<std::pair<int, int>> points;

void scanConversion() {
    int x = -10;
    int y = 100 - x * x;

    while (x < 0) {
        points.push_back({ x, y });
        int midY = 100 - (x + 1) * (x + 1);
        if (midY < y) {
            y--;
            continue;
        }
        else if (midY > y) {
            y++;
            continue;
        }
        else {
            x++;
            continue;
        }
    }
    points.push_back({ 0, 100 });

    for (int i = points.size() - 1; i >= 0; i--) {
        points.push_back({ -points[i].first, points[i].second });
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glPointSize(3.0);
    glBegin(GL_POINTS);
    for (auto& point : points) {
        glVertex2i(point.first, point.second);
    }
    glEnd();
    glFlush();
}

int main(int argc, char** argv) {
    scanConversion();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Parabola");
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-51, 51, -1, 101);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}