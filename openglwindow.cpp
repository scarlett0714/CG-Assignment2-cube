#include "openglwindow.h"
#include <OpenGL/glu.h>
#include <iostream>

// 생성자
OpenGLWindow::OpenGLWindow(QWidget *parent)
    : QOpenGLWidget(parent)
{
}


// 소멸자
OpenGLWindow::~OpenGLWindow()
{
}

// OpenGL 초기화 함수
void OpenGLWindow::initializeGL() {
    initializeOpenGLFunctions();  // Qt의 OpenGL 기능 초기화
    glEnable(GL_DEPTH_TEST);      // 깊이 테스트 활성화 (3D 렌더링 필수)

    setProjection();  // 원근 투영 설정
    setCamera();      // 카메라 설정
}

// OpenGL 화면 렌더링 함수
void OpenGLWindow::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 버퍼 초기화

    glPushMatrix();  // 변환 행렬 저장
    glColor3f(1.0, 0.0, 0.0); // 빨강색 모델

    glBegin(GL_TRIANGLES);
    for (const auto& face : objLoader.faces) {
        glVertex3f(objLoader.vertices[face.v1].x, objLoader.vertices[face.v1].y, objLoader.vertices[face.v1].z);
        glVertex3f(objLoader.vertices[face.v3].x, objLoader.vertices[face.v3].y, objLoader.vertices[face.v3].z);
        glVertex3f(objLoader.vertices[face.v2].x, objLoader.vertices[face.v2].y, objLoader.vertices[face.v2].z);
    }
    glEnd();

    glPopMatrix(); // 변환 행렬 복구
}

// 창 크기 변경 시 호출되는 함수
void OpenGLWindow::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    setProjection();
}

// 원근 투영 설정
void OpenGLWindow::setProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)width() / height(), 1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

// 카메라 설정
void OpenGLWindow::setCamera() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(3.0, 3.0, 5.0,   // 카메라를 대각선 방향으로 이동
              0.0, 0.0, -3.0,  // 큐브 중심 (-3.0) 바라보기
              0.0, 1.0, 0.0);  // Y축을 위로 설정
}


// OBJ 모델 로드 함수 (UI 버튼과 연결 가능)
void OpenGLWindow::loadModel(const std::string& filename) {
    if (objLoader.load(filename)) {
        std::cout << "OBJ file loaded successfully!" << std::endl;
        std::cout << "Number of vertices: " << objLoader.vertices.size() << std::endl;
        std::cout << "Number of faces: " << objLoader.faces.size() << std::endl;
        update(); // 화면 다시 그리기 요청
    } else {
        std::cerr << "Failed to load OBJ file!" << std::endl;
    }
}
