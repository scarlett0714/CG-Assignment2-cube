#include "openglwindow.h"
#include <OpenGL/glu.h>
#include <iostream>
#include <cmath>
#include <QHBoxLayout>

// 생성자
OpenGLWindow::OpenGLWindow(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus); // 키보드 입력을 받을 수 있도록 설정
    installEventFilter(this); // QT 이벤트 필터 감지

    // UI 버튼 생성
    zoomInButton = new QPushButton("Zoom In", this);
    zoomOutButton = new QPushButton("Zoom Out", this);

    increaseVerticalFovButton = new QPushButton("Increase Vertical FOV", this);
    decreaseVerticalFovButton = new QPushButton("Decrease Vertical FOV", this);
    increaseHorizontalFovButton = new QPushButton("Increase Horizontal FOV", this);
    decreaseHorizontalFovButton = new QPushButton("Decrease Horizontal FOV", this);

    nearIncreaseButton = new QPushButton("Near +", this);
    nearDecreaseButton = new QPushButton("Near -", this);
    farIncreaseButton = new QPushButton("Far +", this);
    farDecreaseButton = new QPushButton("Far -", this);

    // 버튼 클릭 시 해당 함수 호출
    connect(zoomInButton, &QPushButton::clicked, this, &OpenGLWindow::zoomIn);
    connect(zoomOutButton, &QPushButton::clicked, this, &OpenGLWindow::zoomOut);

    connect(increaseVerticalFovButton, &QPushButton::clicked, this, &OpenGLWindow::increaseVerticalFov);
    connect(decreaseVerticalFovButton, &QPushButton::clicked, this, &OpenGLWindow::decreaseVerticalFov);
    connect(increaseHorizontalFovButton, &QPushButton::clicked, this, &OpenGLWindow::increaseHorizontalFov);
    connect(decreaseHorizontalFovButton, &QPushButton::clicked, this, &OpenGLWindow::decreaseHorizontalFov);

    connect(nearIncreaseButton, &QPushButton::clicked, this, &OpenGLWindow::increaseNearPlane);
    connect(nearDecreaseButton, &QPushButton::clicked, this, &OpenGLWindow::decreaseNearPlane);
    connect(farIncreaseButton, &QPushButton::clicked, this, &OpenGLWindow::increaseFarPlane);
    connect(farDecreaseButton, &QPushButton::clicked, this, &OpenGLWindow::decreaseFarPlane);

    // 버튼 배치
    QHBoxLayout *zoomLayout = new QHBoxLayout();
    zoomLayout->addWidget(zoomInButton);
    zoomLayout->addWidget(zoomOutButton);

    QHBoxLayout *verticalFovLayout = new QHBoxLayout();
    verticalFovLayout->addWidget(decreaseVerticalFovButton);
    verticalFovLayout->addWidget(increaseVerticalFovButton);

    QHBoxLayout *horizontalFovLayout = new QHBoxLayout();
    horizontalFovLayout->addWidget(decreaseHorizontalFovButton);
    horizontalFovLayout->addWidget(increaseHorizontalFovButton);

    QHBoxLayout *nearLayout = new QHBoxLayout();
    nearLayout->addWidget(nearDecreaseButton);
    nearLayout->addWidget(nearIncreaseButton);

    QHBoxLayout *farLayout = new QHBoxLayout();
    farLayout->addWidget(farDecreaseButton);
    farLayout->addWidget(farIncreaseButton);

    // 전체 UI 정렬
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(zoomLayout);
    mainLayout->addLayout(verticalFovLayout);
    mainLayout->addLayout(horizontalFovLayout);
    mainLayout->addLayout(nearLayout);
    mainLayout->addLayout(farLayout);
    mainLayout->addStretch();

    setLayout(mainLayout);
}


// 소멸자
OpenGLWindow::~OpenGLWindow()
{
    delete zoomInButton;
    delete zoomOutButton;
}

// OpenGL 초기화 함수
void OpenGLWindow::initializeGL() {
    initializeOpenGLFunctions();  // Qt의 OpenGL 기능 초기화
    glEnable(GL_DEPTH_TEST);      // 깊이 테스트 활성화 (3D 렌더링 필수)

    setProjection();  // 원근 투영 설정
    setCamera();      // 카메라 설정

    setFocus();
}

// OpenGL 화면 렌더링 함수
void OpenGLWindow::paintGL() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 버퍼 초기화

    glMatrixMode(GL_PROJECTION);  // 원근 투영 적용
    glLoadIdentity();

    // 수평 FOV 수동 계산
    horizontalFov = 2 * atan(tan(verticalFov * M_PI / 360.0) * aspectRatio) * (180.0 / M_PI);

    gluPerspective(verticalFov, aspectRatio, nearPlane, farPlane); // 최신 fov 반영

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    setCamera();

    glPushMatrix();  // 변환 행렬 저장
    glTranslatef(0.0f, 0.0f, 0.0f);  // 모델 위치 설정
    glColor3f(1.0, 0.0, 0.0); // 빨강색 모델

    // 렌더링 모드 변경
    GLenum renderType;
    if (currentRenderMode == POINTS) {
        renderType = GL_POINTS;
    } else if (currentRenderMode == WIREFRAME) {
        renderType = GL_LINE_LOOP;
    } else {
        renderType = GL_TRIANGLES;
    }

    glBegin(renderType);
    for (const auto& face : objLoader.faces) {
        glVertex3f(objLoader.vertices[face.v1].x, objLoader.vertices[face.v1].y, objLoader.vertices[face.v1].z);
        glVertex3f(objLoader.vertices[face.v2].x, objLoader.vertices[face.v2].y, objLoader.vertices[face.v2].z);
        glVertex3f(objLoader.vertices[face.v3].x, objLoader.vertices[face.v3].y, objLoader.vertices[face.v3].z);
    }
    glEnd();

    glPopMatrix(); // 변환 행렬 복구

}

// 창 크기 변경 시 호출되는 함수
void OpenGLWindow::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    setProjection();
    update();
}

// 원근 투영 설정
void OpenGLWindow::setProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // 수평 FOV 수동 계산
    horizontalFov = 2 * atan(tan(verticalFov * M_PI / 360.0) * aspectRatio) * (180.0 / M_PI);

    gluPerspective(verticalFov, aspectRatio, nearPlane, farPlane);

    glMatrixMode(GL_MODELVIEW);
    update();
}

// 카메라 설정
void OpenGLWindow::setCamera() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ, targetX, targetY, targetZ, 0.0, 1.0, 0.0);

    // 디버깅용 로그 출력 (카메라 위치가 실제로 바뀌는지 확인)
    std::cout << "Camera Position: (" << cameraX << ", " << cameraY << ", " << cameraZ << ")" << std::endl;
    std::cout << "Target Position: (" << targetX << ", " << targetY << ", " << targetZ << ")";
};

// 키보드 입력 이벤트 → `handleKeyboardInput()` 호출
void OpenGLWindow::keyPressEvent(QKeyEvent *event) {
    handleKeyboardInput(event->key());
    update();
}

// 키보드 입력을 처리하는 함수 (카메라 이동)
void OpenGLWindow::handleKeyboardInput(int key) {

    bool updated = false;  // 카메라가 실제로 이동했는지 확인

    float dx = targetX - cameraX;
    float dz = targetZ - cameraZ;
    float length = sqrt(dx * dx + dz * dz);

    if (length != 0) {
        dx /= length;
        dz /= length;
    }

    switch (key) {
    case Qt::Key_W: // 앞으로 이동
        cameraX += dx * cameraSpeed;
        cameraZ += dz * cameraSpeed;
        targetX += dx * cameraSpeed;
        targetZ += dz * cameraSpeed;
        updated = true;
        break;
    case Qt::Key_S: // 뒤로 이동
        cameraX -= dx * cameraSpeed;
        cameraZ -= dz * cameraSpeed;
        targetX -= dx * cameraSpeed;
        targetZ -= dz * cameraSpeed;
        updated = true;
        break;
    case Qt::Key_A: // 왼쪽 이동
        cameraX -= dz * cameraSpeed;
        cameraZ += dx * cameraSpeed;
        targetX -= dz * cameraSpeed;
        targetZ += dx * cameraSpeed;
        updated = true;
        break;
    case Qt::Key_D: // 오른쪽 이동
        cameraX += dz * cameraSpeed;
        cameraZ -= dx * cameraSpeed;
        targetX += dz * cameraSpeed;
        targetZ -= dx * cameraSpeed;
        updated = true;
        break;
    case Qt::Key_Q: // 위로 이동
        cameraY += cameraSpeed;
        targetY += cameraSpeed;
        updated = true;
        break;
    case Qt::Key_E: // 아래로 이동
        cameraY -= cameraSpeed;
        targetY -= cameraSpeed;
        updated = true;
        break;
    case Qt::Key_Left: // 왼쪽 회전
        cameraX = targetX + cos(0.1) * (cameraX - targetX) - sin(0.1) * (cameraZ - targetZ);
        cameraZ = targetZ + sin(0.1) * (cameraX - targetX) + cos(0.1) * (cameraZ - targetZ);
        updated = true;
        break;
    case Qt::Key_Right: // 오른쪽 회전
        cameraX = targetX + cos(-0.1) * (cameraX - targetX) - sin(-0.1) * (cameraZ - targetZ);
        cameraZ = targetZ + sin(-0.1) * (cameraX - targetX) + cos(-0.1) * (cameraZ - targetZ);
        updated = true;
        break;
    case Qt::Key_Up: // 위로 회전
        cameraY += cameraSpeed;
        updated = true;
        break;
    case Qt::Key_Down: // 아래로 회전
        cameraY -= cameraSpeed;
        updated = true;
        break;
    case Qt::Key_5: // 점 모드 (렌더링)
        currentRenderMode = POINTS;
        updated = true;
        break;
    case Qt::Key_6: // 와이어프레임 모드 (렌더링)
        currentRenderMode = WIREFRAME;
        updated = true;
        break;
    case Qt::Key_7: // 표면 모드 (렌더링)
        currentRenderMode = SURFACE;
        updated = true;
        break;
    default:
        std::cout << "Unhandled key: " << key << std::endl; // 디버깅
        return;
    }

    if (updated) {
        std::cout << "Calling setCamera()... CameraX: " << cameraX << " CameraY: " << cameraY << " CameraZ: " << cameraZ << std::endl;
        setCamera();
        repaint();
    }
}

// 줌인 버튼 기능
void OpenGLWindow::zoomIn() {
    float dx = targetX - cameraX;
    float dy = targetY - cameraY;
    float dz = targetZ - cameraZ;
    float length = sqrt(dx * dx + dy * dy + dz * dz);

    if (length > 1.0f) { // 너무 가까워지는 것을 방지
        cameraX += dx * 0.1f;
        cameraY += dy * 0.1f;
        cameraZ += dz * 0.1f;
        setCamera();
        update();
    }
}

// 줌아웃 버튼 기능
void OpenGLWindow::zoomOut() {
    float dx = targetX - cameraX;
    float dy = targetY - cameraY;
    float dz = targetZ - cameraZ;

    cameraX -= dx * 0.1f;
    cameraY -= dy * 0.1f;
    cameraZ -= dz * 0.1f;
    setCamera();
    update();
}


// fov
void OpenGLWindow::increaseVerticalFov() {
    if (verticalFov < 120.0f) {  // 최대 제한
        verticalFov += 5.0f;
        std::cout << "Vertical FOV: " << verticalFov << std::endl;
        setProjection();
        update();
    }
}

void OpenGLWindow::decreaseVerticalFov() {
    if (verticalFov > 10.0f) {  // 최소 제한
        verticalFov -= 5.0f;
        std::cout << "Vertical FOV: " << verticalFov << std::endl;
        setProjection();
        update();
    }
}

void OpenGLWindow::increaseHorizontalFov() {
    aspectRatio += 0.1f;  // 종횡비 증가
    std::cout << "Aspect Ratio (Horizontal FOV 증가): " << aspectRatio << std::endl;
    setProjection();
    update();
}

void OpenGLWindow::decreaseHorizontalFov() {
    if (aspectRatio > 0.5f) {  // 최소 제한
        aspectRatio -= 0.1f;
        std::cout << "Aspect Ratio (Horizontal FOV 감소): " << aspectRatio << std::endl;
        setProjection();
        update();
    }
}



// clipping plane
void OpenGLWindow::increaseNearPlane() {
    if (nearPlane < farPlane - 0.1f) { // nearPlane이 farPlane을 넘지 않도록 제한
        nearPlane += 0.1f;
        std::cout << "Near Plane Increased: " << nearPlane << std::endl;
        setProjection();
        update();
        repaint();
    }
}

void OpenGLWindow::decreaseNearPlane() {
    if (nearPlane > 0.1f) { // nearPlane이 0.1 이하로 내려가지 않도록 제한
        nearPlane -= 0.1f;
        std::cout << "Near Plane Decreased: " << nearPlane << std::endl;
        setProjection();
        update();
        repaint();
    }
}

void OpenGLWindow::increaseFarPlane() {
    if (farPlane < 500.0f) { // farPlane 최대값 제한
        farPlane += 10.0f;
        std::cout << "Far Plane Increased: " << farPlane << std::endl;
        setProjection();
        update();
        repaint();
    }
}

void OpenGLWindow::decreaseFarPlane() {
    if (farPlane > nearPlane + 0.1f) { // farPlane이 nearPlane보다 작아지지 않도록 제한
        farPlane -= 10.0f;
        std::cout << "Far Plane Decreased: " << farPlane << std::endl;
        setProjection();
        update();
        repaint();
    }
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


// 이벤트 디버깅 함수
bool OpenGLWindow::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        std::cout << "Key Pressed: " << keyEvent->key() << std::endl; // 키 입력이 감지되는지 확인 (87)
    }
    return QOpenGLWidget::eventFilter(obj, event);
}
