#include "objloader.h"

bool ObjLoader::load(const std::string& filename) {
    std::ifstream fin(filename); // include <fstream>
    if (!fin.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl; // include <iostream>
        return false;
    }

    std::string line;
    while (std::getline(fin, line)) {

        //std::istringstream : 공백(띄어쓰기, 탭, 개행)을 자동으로 구분자(separator)로 사용해서 데이터를 읽음
        std::istringstream ss(line); // include <sstream>
        std::string type;
        ss >> type;

        if (type == "v") {
            Vertex v;
            ss >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        } else if (type == "f") {
            Face f;
            ss >> f.v1 >> f.v2 >> f.v3;

            f.v1--;
            f.v2--;
            f.v3--;

            faces.push_back(f);
        }
    }

    std::cout << "Loaded Vertices:" << std::endl;
    for (size_t i = 0; i < vertices.size(); i++) {
        std::cout << i+1 << ": (" << vertices[i].x << ", " << vertices[i].y << ", " << vertices[i].z << ")" << std::endl;
    }

    std::cout << "Loaded Faces:" << std::endl;
    for (const auto& face : faces) {
        std::cout << "Face: " << face.v1 << ", " << face.v2 << ", " << face.v3 << std::endl;
    }

    fin.close();
    return true;
}
