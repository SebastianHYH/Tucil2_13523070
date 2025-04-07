#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_NO_EXIF
#include "include/stb_image.h"
#include "include/stb_image_write.h"
#include "include/image.hpp"
#include <iostream>
#include <sstream>
#include <filesystem>
#include <fstream>
using namespace std;

Image::Image() : width(0), height(0), channels(0) {}

Image::~Image() {
    pixels.clear();
}

long long Image::getFileSize(const string& filePath) const {
    ifstream file(filePath, ios::binary | ios::ate);
    if (!file.is_open()) {
        cerr << "Error: File gagal dibuka: " << filePath << endl;
        return -1;
    }
    return file.tellg();
}

bool Image::loadImage(const string& filepath) {
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 3);
    if (!data) {
        cerr << "Error: Gambar gagal di-load: " << filepath << endl;
        return false;
    }

    channels = 3;

    rotated = false;

    if (height > width) { // If image is portrait
        vector<unsigned char> rotatedPixels(height * width * channels);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int srcIdx = (y * width + x) * channels;
                int dstIdx = (x * height + (height - y - 1)) * channels;

                for (int c = 0; c < channels; c++) {
                    rotatedPixels[dstIdx + c] = data[srcIdx + c];
                }
            }
        }

        swap(width, height);
        pixels = move(rotatedPixels);
        rotated = true;
    } else {
        pixels.assign(data, data + (width * height * channels));
    }

    stbi_image_free(data);
    return true;
}

bool Image::saveImage(const string& filepath) const {
    if (pixels.empty()) {
        cerr << "Error: Data gambar tidak ada" << endl;
        return false;
    }

    std::filesystem::path filePathObj(filepath);
    std::filesystem::path directory = filePathObj.parent_path();

    if (!directory.empty() && !std::filesystem::exists(directory)) {
        if (!std::filesystem::create_directories(directory)) {
            return false;
        }
    }

    int outWidth = width, outHeight = height;
    vector<unsigned char> outputPixels = pixels;

    if (rotated) { // Rotate back
        vector<unsigned char> rotatedPixels(width * height * channels);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int srcIdx = (y * width + x) * channels;
                int dstIdx = ((width - x - 1) * height + y) * channels;

                for (int c = 0; c < channels; c++) {
                    rotatedPixels[dstIdx + c] = pixels[srcIdx + c];
                }
            }
        }

        outputPixels = move(rotatedPixels);
        swap(outWidth, outHeight);
    }

    if (!stbi_write_png(filepath.c_str(), outWidth, outHeight, channels, outputPixels.data(), outWidth * channels)) {
        cerr << "Error: Gagal menyimpan gambar " << filepath << endl;
        return false;
    }

    return true;
}

vector<vector<vector<int>>> Image::getPixelMatrix() const {
    vector<vector<vector<int>>> matrix(height, vector<vector<int>>(width, vector<int>(channels)));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * channels;
            for (int c = 0; c < channels; c++) {
                matrix[y][x][c] = static_cast<int>(pixels[index + c]);
            }
        }
    }

    return matrix;
}

void Image::setPixelMatrix(const vector<vector<vector<int>>>& newPixels) {
    this->height = newPixels.size();
    this->width = (this->height > 0) ? newPixels[0].size() : 0;

    if (this->width > 0 && this->height > 0) {
        int newChannels = newPixels[0][0].size();
        if (newPixels.empty() || newPixels[0].empty()) return;
        channels = newPixels[0][0].size();
    }

    pixels.clear();
    pixels.reserve(this->height * this->width * channels);

    for (const auto& row : newPixels) {
        for (const auto& pixel : row) {
            for (int c = 0; c < channels; c++) {
                pixels.push_back(static_cast<unsigned char>(pixel[c]));
            }
        }
    }
}