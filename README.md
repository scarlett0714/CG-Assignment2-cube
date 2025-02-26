# Assignment 2 - Compilation Guide

Prerequisites
Before compiling the project, ensure you have the following installed on your system:

Qt Creator (Recommended version: Qt 6.x or Qt 5.x)
Qt SDK (with necessary modules)
C++ Compiler (e.g., MSVC, MinGW, or Clang)
CMake (if using CMake-based build)


🛠️ Build Instructions
Follow these steps to successfully compile and run the project:

### 1️⃣ Clone the Repository
```bash
git clone [https://github.com/your-repo-name/project.git](https://github.com/scarlett0714/CG-Assignment2-cube.git)
```

### 2️⃣ Open the Project in Qt Creator
1. **Launch Qt Creator**  
2. Click **File** → **Open Project...**  
3. Select the `.pro` file (if using qmake) or `CMakeLists.txt` file (if using CMake)  
4. Configure the project with the appropriate kit (e.g., **Qt 6.2 + MinGW**)  

### 3️⃣ Configure Build Settings
1. Go to **Projects** (`Ctrl+5`) → **Build & Run**
2. Choose a build kit (e.g., **Desktop Qt 6.2.2 MinGW 64-bit**)
3. Set the build directory (**`./build`** or **`./out`** recommended)
4. Make sure **Shadow Build** is enabled (recommended)

### 4️⃣ Build the Project
#### **Using Qt Creator**
- Click **Build** → **Build All** (`Ctrl+B`)

#### **Using Terminal in Qt Creator**
For **qmake-based projects**:
```bash
qmake && make -j$(nproc)
```

or for CMake-based projects:
```bash
mkdir -p build && cd build
cmake ..
cmake --build .
```

### 5️⃣ Run the Application
Click Run (Ctrl+R) in Qt Creator
Or run the compiled binary manually:
```bash
./build/your_project_name
```

🛠️ Troubleshooting
If you see missing DLL errors, make sure Qt runtime libraries are included. Use:
windeployqt your_project.exe  # Windows
macdeployqt your_project.app  # macOS
If compilation fails, check the Build Output tab in Qt Creator.
Ensure the correct Qt version and compiler are selected in Build Settings.
