#include <QApplication>
#include "openglwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    OpenGLWindow window;
    window.resize(800, 600);
    window.show();

    // OBJ 파일 로드
    window.loadModel("/Users/hwang-yoonseon/Desktop/konkuk/wsu/cg/cube.obj");

    return app.exec();
}
