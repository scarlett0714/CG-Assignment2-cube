#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QKeyEvent>
#include "objloader.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class OpenGLWindow;
}
QT_END_NAMESPACE

class OpenGLWindow : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit OpenGLWindow(QWidget *parent = nullptr);
    ~OpenGLWindow();

    // OBJ 모델 로드 함수
    void loadModel(const std::string& filename);

protected:
    void initializeGL() override;  // OpenGL 초기화
    void paintGL() override;       // OpenGL 렌더링
    void resizeGL(int w, int h) override; // OpenGL 창 크기 변경 시 처리
    void keyPressEvent(QKeyEvent *event) override; // 키보드 입력 처리
    bool eventFilter(QObject *obj, QEvent *event) override; // 이벤트 디버깅

private:
    Ui::OpenGLWindow *ui;
    ObjLoader objLoader; // OBJ 파일 로더 객체

    void setProjection(); // 원근 투영 설정
    void setCamera();     // 카메라 설정
    void handleKeyboardInput(int key); // 키보드 입력 처리

    // 카메라 위치 및 방향 변수
    float cameraX = 3.0f, cameraY = 3.0f, cameraZ = 5.0f;
    float targetX = 0.0f, targetY = 0.0f, targetZ = -3.0f;
    float cameraSpeed = 1.0f;
};

#endif // OPENGLWINDOW_H
