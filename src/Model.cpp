#include <stdio.h>
#include <string.h>
#include "Model.hpp"

void Model::calculateNormals(ModelObject& object) {
    object.computedNormals.resize(object.vertices.size(), glm::vec3{ 0.f, 0.f, 0.f });
    for (auto& face : object.faces) {
        glm::vec3 vtx[3];
        for (int i = 0; i < 3; i++) {
            vtx[i] = object.vertices.at(face[i][VERTEX]);
        }
        glm::vec3 surfaceNormal = glm::normalize(glm::cross(vtx[1] - vtx[0], vtx[2] - vtx[0]));
        for (int i = 0; i < 3; i++) {
            object.computedNormals.at(face[i][VERTEX]) += surfaceNormal;
        }
    }
    for (auto& normal : object.computedNormals) { normal = glm::normalize(normal); }
}

void Model::loadObject(char line[], int lineSize, FILE *objFile) {
    char objectName[512];
    int scanResult = sscanf(line, "o %s", objectName);
    ModelObject object(objectName);
    while (fgets(line, lineSize, objFile)) {
        if (line[0] == '#') { continue; }
        else if (!strncmp(line, "v ", 2)) {
            glm::vec3 v;
            scanResult = sscanf(line, "v %f %f %f", &v[0], &v[1], &v[2]);
            object.vertices.push_back(v);
        }
        else if (!strncmp(line, "vt", 2)) {
            glm::vec3 vt;
            scanResult = sscanf(line, "vt %f %f %f", &vt[0], &vt[1], &vt[2]);
            if (scanResult < 3) { vt[2] = 0.f; }
            while (vt.x < 0.f) { vt.x += 1.f; }
            while (vt.x > 1.f) { vt.x -= 1.f; }
            while (vt.y < 0.f) { vt.y += 1.f; }
            while (vt.y > 1.f) { vt.y -= 1.f; }
            object.textureUVs.push_back(vt);
        }
        else if (!strncmp(line, "vn", 2)) {
            glm::vec3 vn;
            scanResult = sscanf(line, "vn %f %f %f", &vn[0], &vn[1], &vn[2]);
            object.normals.push_back(vn);
        }
        else if (!strncmp(line, "usemtl", 6)) {
            char materialName[512];
            scanResult = sscanf(line, "usemtl %s", materialName);
            object.material = getMaterial(materialName);
        }
        else if (!strncmp(line, "f ", 2)) {
            std::vector<glm::ivec3> f(3);
            scanResult = sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                &f[0][0], &f[0][1], &f[0][2],
                &f[1][0], &f[1][1], &f[1][2],
                &f[2][0], &f[2][1], &f[2][2]
            );
            if (scanResult < 9) {
                scanResult = sscanf(line, "f %d//%d %d//%d %d//%d",
                    &f[0][0], &f[0][2],
                    &f[1][0], &f[1][2],
                    &f[2][0], &f[2][2]
                );
                for (int i = 0; i < 3; i++) { f[i][1] = 1; }
            }
            /* Adjust for obj file indicies starting at 1. */
            for (auto& vec : f) {
                vec[VERTEX] -= vertexIndex + 1;
                vec[TEXTURE] -= textureUVIndex + 1;
                vec[NORMAL] -= normalIndex + 1;
            }
            object.faces.push_back(f);
        }
        else if (!strncmp(line, "o ", 2)) { break; }
    }
    printf("Object \"%s\"\n", objectName);
    printf("Vertices: %d | Faces: %d | ", object.vertices.size(), object.faces.size());
    printf("Texture vertices: %d | Vertex normals: %d\n", object.textureUVs.size(), object.normals.size());
    calculateNormals(object);
    vertexIndex += object.vertices.size();
    textureUVIndex += object.textureUVs.size();
    normalIndex += object.normals.size();
    objects.push_back(std::move(object));
    if (!strncmp(line, "o ", 2)) { loadObject(line, lineSize, objFile); }
}

void Model::loadMaterial(char line[], int lineSize, FILE *materialFile) {
	char materialName[512];
    int scanResult = sscanf(line, "newmtl %s", materialName);
    Material material(materialName);
    while (fgets(line, lineSize, materialFile)) {
        if (line[0] == '#') { continue; }
        else if (!strncmp(line, "map_Kd", 6)) {
            char texturePath[512];
            scanResult = sscanf(line, "map_Kd %s", texturePath);
            material.texture = getTexture(texturePath);
        }
        else if (!strncmp(line, "newmtl", 6)) { break; }
    }
    materials.push_back(std::move(material));
    if (!strncmp(line, "newmtl", 6)) { loadMaterial(line, lineSize, materialFile); }
}

void Model::loadMaterials(const char *materialPath) {
	FILE* materialFile = fopen(materialPath, "rb");
    assert(materialFile && "Failure opening material file.");
    const unsigned int lineSize = 512;
    char line[lineSize];
    int scanResult;
    while (fgets(line, lineSize, materialFile)) {
        if (line[0] == '#') { continue; }
        else if (!strncmp(line, "newmtl", 6)) {
            loadMaterial(line, lineSize, materialFile);
        }
    }
}

Model::Model(const char *filename) {
	FILE* objFile = fopen(filename, "rb");
    assert(objFile && "Failure opening object file.");
    printf("Model \"%s\":\n", filename);
    const unsigned int lineSize = 512;
    char line[lineSize];
    int scanResult;
    while (fgets(line, lineSize, objFile)) {
        if (line[0] == '#') { continue; }
        else if (!strncmp(line, "mtllib", 6)) {
            char pathBuf[512];
            scanResult = sscanf(line, "mtllib %s", pathBuf);
            loadMaterials(pathBuf);
        }
        else if (!strncmp(line, "o ", 2)) {
            loadObject(line, lineSize, objFile);
        }
    }
}

MatTexture* Model::getTexture(const char* path) {
    /* Search for existing texture first */
    for (auto& texture : textures) {
        if (!strcmp(texture->path, path)) { return texture.get(); }
    }
    /* Load new texture if there wasn't a match */
    textures.push_back(std::make_unique<MatTexture>(path));
    return textures.back().get();
}

Material *Model::getMaterial(const char *materialName) {
	for (auto& material : materials) {
        if (!strcmp(material.name, materialName)) { return &material; }
    }
    assert(false && "Could not find material with that name.");
    return nullptr;
}
