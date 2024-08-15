#pragma once
#include "tgaimage.hpp"
#include <assert.h>
#include <memory>
#include <string.h>

struct MatTexture {
	const char* path = nullptr;
	const char* tangentMapPath = nullptr;
	std::unique_ptr<TGAImage> texture;
	std::unique_ptr<TGAImage> tangentMap;

	MatTexture(const char* texturePath) {
		path = strcpy(new char[strlen(texturePath) + 1], texturePath);

		texture = std::make_unique<TGAImage>();

		assert(texture.get()->read_tga_file(path) && "Error reading .tga file.");
	}
	MatTexture(const char* texturePath, const char* tangentPath) {
		path = strcpy(new char[strlen(texturePath) + 1], texturePath);
		tangentMapPath = strcpy(new char[strlen(tangentPath) + 1], tangentPath);

		texture = std::make_unique<TGAImage>();
		tangentMap = std::make_unique<TGAImage>();

		assert(texture.get()->read_tga_file(path) && "Error reading .tga file.");
		assert(tangentMap.get()->read_tga_file(tangentMapPath) && "Error reading .tga file.");
	}
	~MatTexture() {
		delete[] path;
		delete[] tangentMapPath;
	}
	MatTexture(const MatTexture&) = delete;
	MatTexture& operator=(const MatTexture&) = delete;

	friend void swap(MatTexture& lhs, MatTexture& rhs) {
		using std::swap;
		swap(lhs.path, rhs.path);
		swap(lhs.tangentMapPath, rhs.tangentMapPath);
		swap(lhs.texture, rhs.texture);
		swap(lhs.tangentMap, rhs.tangentMap);
	}

	MatTexture(MatTexture&& other) noexcept { swap(*this, other); }
};

struct Material {
	const char* name = nullptr;
	MatTexture* texture = nullptr;

	Material(const char* matName) {
		name = strcpy(new char[strlen(matName) + 1], matName);
	}
	~Material() { delete[] name; }
	Material(const Material&) = delete;
	Material& operator=(const Material&) = delete;

	friend void swap(Material& lhs, Material& rhs) {
		using std::swap;
		swap(lhs.name, rhs.name);
		swap(lhs.texture, rhs.texture);
	}

	Material(Material&& other) noexcept { swap(*this, other); }
};
