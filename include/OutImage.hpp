#pragma once
#include "tgaimage.hpp"

class OutImage {
private:
	TGAImage image;
	float* zBuffer = nullptr;

public:
	OutImage(int w, int h, TGAImage::Format fmt);
	~OutImage();
	OutImage(const OutImage&) = delete;
	OutImage& operator=(const OutImage&) = delete;

	float* getZBuffer() { return zBuffer; }

	int getWidth() { return image.get_width(); }
	int getHeight() { return image.get_height(); }

	TGAColor get(int x, int y) { return image.get(x, y); }
	void set(int x, int y, TGAColor color) { image.set(x, y, color); }

	void writeTGAFile(const char* filename) { image.write_tga_file(filename); }
};