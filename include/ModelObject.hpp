#pragma once
#include "Material.hpp"
#include "glm/vec3.hpp"
#include "glm/mat3x3.hpp"
#include <vector>

class Model;
class ModelObject {
	friend class Model;

private:
	enum {
		VERTEX = 0,
		TEXTURE = 1,
		NORMAL = 2
	};
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> textureUVs;
	std::vector<glm::mat3> tbns;
	std::vector<std::vector<glm::ivec3>> faces;
	
	const Model* model;

public:
	const char* name = nullptr;
	Material* material = nullptr;

	ModelObject(const Model* model, const char* objName) : model(model) {
		name = strcpy(new char[strlen(objName) + 1], objName);
	}
	~ModelObject() { delete[] name; }
	ModelObject(const ModelObject&) = delete;
	ModelObject& operator=(const ModelObject&) = delete;

	friend void swap(ModelObject& lhs, ModelObject& rhs) {
		using std::swap;
		swap(lhs.name, rhs.name);
		swap(lhs.material, rhs.material);
		swap(lhs.vertices, rhs.vertices);
		swap(lhs.textureUVs, rhs.textureUVs);
		swap(lhs.tbns, rhs.tbns);
		swap(lhs.faces, rhs.faces);
		swap(lhs.model, rhs.model);
	}

	ModelObject(ModelObject&& other) noexcept { swap(*this, other); }

	glm::vec3 getVertex(unsigned int i) const { return vertices.at(i); }
	glm::vec3 getVertex(glm::ivec3 vec) const { return vertices.at(vec[VERTEX]); }

	glm::vec3 getTextureUV(unsigned int i) const { return textureUVs.at(i); }
	glm::vec3 getTextureUV(glm::ivec3 vec) const { return textureUVs.at(vec[TEXTURE]); }

	glm::vec3 getNormal(unsigned int i) const { return tbns.at(i)[2]; }
	glm::vec3 getNormal(glm::ivec3 vec) const { return tbns.at(vec[NORMAL])[2]; }

	glm::mat3 getTBN(unsigned int i) const { return tbns.at(i); }
	glm::mat3 getTBN(glm::ivec3 vec) const { return tbns.at(vec[VERTEX]); }

	std::vector<glm::ivec3> getFace(int i) const { return faces.at(i); }
	const std::vector<std::vector<glm::ivec3>>& getFaces() const { return faces; }

	const Model* getModel() const { return model; }
};