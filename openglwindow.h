#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QKeyEvent>
#include <QPushButton>
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

private slots:
    void zoomIn();  // 줌인 함수
    void zoomOut(); // 줌아웃 함수

private:
    Ui::OpenGLWindow *ui;
    ObjLoader objLoader; // OBJ 파일 로더 객체

    QPushButton *zoomInButton; // 줌 인 버튼
    QPushButton *zoomOutButton; // 줌 아웃 버튼

    void setProjection(); // 원근 투영 설정
    void setCamera();     // 카메라 설정
    void handleKeyboardInput(int key); // 키보드 입력 처리

    // 카메라 위치 및 방향 변수
    float cameraX = 3.0f, cameraY = 3.0f, cameraZ = 5.0f;
    float targetX = 0.0f, targetY = 0.0f, targetZ = -3.0f;
    float cameraSpeed = 1.0f;
    float fov = 60.0f;

    // clipping plane
    QPushButton *nearDecreaseButton;
    QPushButton *nearIncreaseButton;
    QPushButton *farDecreaseButton;
    QPushButton *farIncreaseButton;

    float nearPlane = 0.1f; // 기본값 설정
    float farPlane = 100.0f;

    void increaseNearPlane();
    void decreaseNearPlane();
    void increaseFarPlane();
    void decreaseFarPlane();
};

#endif // OPENGLWINDOW_H
