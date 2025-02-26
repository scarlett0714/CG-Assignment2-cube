#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
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

private:
    Ui::OpenGLWindow *ui;
    ObjLoader objLoader; // OBJ 파일 로더 객체

    void setProjection(); // 원근 투영 설정
    void setCamera();     // 카메라 설정
};

#endif // OPENGLWINDOW_H
