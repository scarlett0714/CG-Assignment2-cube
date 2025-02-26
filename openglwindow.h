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

    void mousePressEvent(QMouseEvent *event) override;  // 마우스 클릭 이벤트
    void mouseMoveEvent(QMouseEvent *event) override;   // 마우스 이동 이벤트
    void mouseReleaseEvent(QMouseEvent *event) override; // 마우스 해제 이벤트

private slots:
    void zoomIn();  // 줌인 함수
    void zoomOut(); // 줌아웃 함수

    void increaseVerticalFov();
    void decreaseVerticalFov();
    void increaseHorizontalFov();
    void decreaseHorizontalFov();


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

    // 마우스 이벤트
    bool isDragging = false;  // 드래그 상태 확인
    bool isModelRotating = false; // 모델 회전 상태 확인
    QPoint lastMousePosition; // 이전 마우스 위치 저장

    float yaw = -90.0f;   // Y축 회전 (초기값: -90도)
    float pitch = 0.0f;   // X축 회전 (초기값: 0도)
    float sensitivity = 0.2f; // 마우스 감도 조절

    void rotateCamera(float dx, float dy); // 카메라 회전 처리

    // 모델 회전 변수
    float modelRotationX = 0.0f;    // 모델의 X축 회전 각도
    float modelRotationY = 0.0f;    // 모델의 Y축 회전 각도
    float modelRotationZ = 0.0f;    // 모델의 Z축 회전 각도 (선택 사항)
    float modelRotationSensitivity = 0.5f; // 모델 회전 감도

    // fov
    float verticalFov = 60.0f;  // 기본 수직 FOV
    float aspectRatio = 1.5f;   // 기본 가로/세로 비율 (16:9 기준)
    float horizontalFov;        // 계산된 수평 FOV

    QPushButton *increaseVerticalFovButton;
    QPushButton *decreaseVerticalFovButton;
    QPushButton *increaseHorizontalFovButton;
    QPushButton *decreaseHorizontalFovButton;


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

    // 렌더링
    enum RenderMode { POINTS, WIREFRAME, SURFACE };
    RenderMode currentRenderMode = SURFACE;  // 기본값: 표면 렌더링
};

#endif // OPENGLWINDOW_H
