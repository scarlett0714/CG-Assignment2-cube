# Assignment 2 - Compilation Guide
Before compiling the project, ensure you have the following installed on your system:
- Qt Creator (Recommended version: Qt 6.x or Qt 5.x)
- Qt SDK (with necessary modules)
- C++ Compiler (e.g., MSVC, MinGW, or Clang)
- CMake (if using CMake-based build)

🛠️ Build Instructions:
Clone the Repository
```bash
git clone https://github.com/scarlett0714/CG-Assignment2-cube.git
```

Open the Project in Qt Creator
1. **Launch Qt Creator**  
2. Click **File** → **Open Project...**  
3. Select the `.pro` file (if using qmake) or `CMakeLists.txt` file (if using CMake)  
4. Configure the project with the appropriate kit (e.g., **Qt 6.2 + MinGW**)  

Configure Build Settings
1. Go to **Projects** (`Ctrl+5`) → **Build & Run**
2. Choose a build kit (e.g., **Desktop Qt 6.2.2 MinGW 64-bit**)
3. Set the build directory (**`./build`** or **`./out`** recommended)
4. Make sure **Shadow Build** is enabled (recommended)

Build the Project
#### **Using Qt Creator**
- Click **Build** → **Build All** (`Ctrl+B`)

Run the Application
Click Run (Ctrl+R) in Qt Creator

If compilation fails, check the Build Output tab in Qt Creator.
Ensure the correct Qt version and compiler are selected in Build Settings.
