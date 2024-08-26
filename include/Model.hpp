#pragma once
#include "ModelObject.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/mat4x4.hpp"

class Model {
private:
	enum {
		VERTEX = 0,
		TEXTURE = 1,
		NORMAL = 2
	};

	int vertexIndex = 0;
	int textureUVIndex = 0;
	int normalIndex = 0;

	std::vector<std::unique_ptr<MatTexture>> textures;
	std::vector<Material> materials;
	std::vector<ModelObject> objects;

	glm::mat4 modelMatrix = glm::identity<glm::mat4>();

	void loadObject(char line[], int lineSize, FILE* objFile);
	void calculateNormals(ModelObject& object);
	void calculateTBNs(ModelObject& object);

	void loadMaterial(char line[], int lineSize, FILE* materialFile);
	void loadMaterials(const char* materialPath);

public:
	Model(const char* filename);

	const std::vector<ModelObject>& getModelObjects() const { return objects; }

	MatTexture* getTexture(const char* path);
	Material* getMaterial(const char* materialName);

	glm::mat4 getModelMatrix() const { return modelMatrix; }

	void move(glm::vec3 translationVec) {
		modelMatrix = glm::translate(modelMatrix, translationVec);
	}

	void rotate(float angle, glm::vec3 rotationVec) {
		modelMatrix = glm::rotate(modelMatrix, angle, rotationVec);
	}

	void rotateX(float angle) { rotate(angle, glm::vec3{ 1.f, 0.f, 0.f }); }
	void rotateY(float angle) { rotate(angle, glm::vec3{ 0.f, 1.f, 0.f }); }
	void rotateZ(float angle) { rotate(angle, glm::vec3{ 0.f, 0.f, 1.f }); }

	void scale(glm::vec3 scaleVec) {
		modelMatrix = glm::scale(modelMatrix, scaleVec);
	}
};
