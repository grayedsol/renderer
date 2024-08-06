#include <stdio.h>
#include <string.h>
#include "Model.hpp"

Model::Model(const char* filename) {
    FILE* modelFile = fopen(filename, "rb");
    assert(modelFile && "Failure opening model file.");
    const unsigned int lineSize = 512;
    char line[lineSize];
    while (fgets(line, lineSize, modelFile)) {
        if (!strncmp(line, "v ", 2)) {
            glm::vec3 v;
            int result = sscanf(line, "v %f %f %f", &v[0], &v[1], &v[2]);
            vertices.push_back(v);
        }
        else if (!strncmp(line, "vt", 2)) {
            glm::vec3 vt;
            int result = sscanf(line, "vt %f %f %f", &vt[0], &vt[1], &vt[2]);
            if (result < 3) { vt[2] = 0.f; }
            textureUVs.push_back(vt);
        }
        else if (!strncmp(line, "f ", 2)) {
            std::vector<glm::ivec3> f(3);
            sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                &f[0][0], &f[0][1], &f[0][2],
                &f[1][0], &f[1][1], &f[1][2],
                &f[2][0], &f[2][1], &f[2][2]
            );
            /* Adjust for obj file indicies starting at 1. */
            for (auto& vec : f) {
                for (int i = 0; i < 3; i++) { vec[i]--; }
            }
            faces.push_back(f);
        }
    }
    printf("Vertices: %d Faces: %d\n", vertices.size(), faces.size());
}
