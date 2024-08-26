#pragma once
#include "tgaimage.hpp"
#include <assert.h>
#include <memory>
#include <string.h>

struct MatTexture {
	const char* path = nullptr;
	std::unique_ptr<TGAImage> texture;

	MatTexture(const char* texturePath) {
		path = strcpy(new char[strlen(texturePath) + 1], texturePath);

		texture = std::make_unique<TGAImage>();

		bool textureRead = texture.get()->read_tga_file(path);

		assert(textureRead && "Error reading .tga file.");
	}
	~MatTexture() { delete[] path; }
	MatTexture(const MatTexture&) = delete;
	MatTexture& operator=(const MatTexture&) = delete;

	friend void swap(MatTexture& lhs, MatTexture& rhs) {
		using std::swap;
		swap(lhs.path, rhs.path);
		swap(lhs.texture, rhs.texture);
	}

	MatTexture(MatTexture&& other) noexcept { swap(*this, other); }

	const TGAImage* get() { return texture.get(); }
};

struct Material {
	const char* name = nullptr;
	MatTexture* texture = nullptr;
	MatTexture* tangentMap = nullptr;
	TGAColor specularColor = TGAColor{ 255, 255, 255, 255 };
	float shine = 0.f;

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
		swap(lhs.tangentMap, rhs.tangentMap);
		swap(lhs.specularColor, rhs.specularColor);
		swap(lhs.shine, rhs.shine);
	}

	Material(Material&& other) noexcept { swap(*this, other); }
};
