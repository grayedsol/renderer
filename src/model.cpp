/**
 * @file model.cpp
 * @author ssloy
 */
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.hpp"

Model::Model(const char *filename) : verts_(), faces_() {
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            glm::vec3 v;
            for (int i=0;i<3;i++) iss >> v[i];
            verts_.push_back(v);
        } else if (!line.compare(0, 2, "vt")) {
            iss >> trash >> trash;
            glm::vec3 vt;
            for (int i=0;i<3;i++) {
                iss >> vt[i];
            }
            textures_.push_back(vt);
        } else if (!line.compare(0, 2, "f ")) {
            std::vector<glm::vec3> f;
            int itrash;
            glm::vec3 vec;
            iss >> trash;
            while (iss >> vec[0] >> trash >> vec[1] >> trash >> vec[2]) {
                for (int i=0;i<3;i++) { vec[i]--; } // in wavefront obj all indices start at 1, not zero
                f.push_back(vec);
            }
            faces_.push_back(f);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# "  << faces_.size() << std::endl;
}

int Model::nverts() const {
    return (int)verts_.size();
}

int Model::nfaces() const {
    return (int)faces_.size();
}

std::vector<glm::vec3> Model::face(int idx) const {
    return faces_[idx];
}

glm::vec3 Model::vert(int i) const {
    return verts_[i];
}

glm::vec3 Model::texture(int i) const {
	return textures_[i];
}
