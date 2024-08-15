#include "OutImage.hpp"
#include <algorithm>

OutImage::OutImage(int w, int h, TGAImage::Format fmt) : image(w, h, fmt) {
	zBuffer = new float[w * h];
	std::fill(zBuffer, &zBuffer[w * h], -10000.f);
}

OutImage::~OutImage() {
	delete[] zBuffer;
}
