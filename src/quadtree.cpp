#include "include/quadtree.hpp"
#include <iostream>
#include <queue>
using namespace std;


Quadtree::Node::Node(int x, int y, int size, bool isLeaf, vector<int> color)
    : x(x), y(y), size(size), isLeaf(isLeaf), color(color) {
    for (int i = 0; i < 4; i++) {
        children[i] = nullptr;
    }
}

Quadtree::Node::~Node() {
    for (int i = 0; i < 4; i++) {
        delete children[i];
    }
}

Quadtree::Quadtree(int threshold, ErrorMethod method) {
    this->threshold = threshold;
    this->method = method;
}

Quadtree::~Quadtree() {
}

void Quadtree::build(const vector<vector<vector<int>>>& image, int imgWidth, int imgHeight, int minBlockSize) {
    originalWidth = imgWidth;
    originalHeight = imgHeight;
    root = compressBlock(image, 0, 0, originalWidth, originalHeight, minBlockSize);
}

Quadtree::Node* Quadtree::compressBlock(
    const vector<vector<vector<int>>>& image, 
    int x, int y, 
    int width, int height, 
    int minBlockSize
) {
    if (width == 1 && height == 1) {
        return new Node(x, y, width, false, vector<int>{0, 0, 0});
    }

    vector<vector<vector<int>>> block(height, vector<vector<int>>(width, vector<int>(3)));
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            block[i][j] = image[x + i][y + j]; //nilai RGB

    double error = 0;

    switch (this->method) {
        case ErrorMethod::VARIANCE:
            error = ErrorMeasurementMethod::computeVariance(block);
            break;
        case ErrorMethod::MEAN_ABSOLUTE_DEVIATION:
            error = ErrorMeasurementMethod::computeMAD(block);
            break;
        case ErrorMethod::MAX_PIXEL_DIFFERENCE:
            error = ErrorMeasurementMethod::computeMaxPixelDiff(block);
            break;
        case ErrorMethod::ENTROPY:
            error = ErrorMeasurementMethod::computeEntropy(block);
            break;
    }

    if (error <= threshold || width <= minBlockSize || height <= minBlockSize) {
        vector<int> avgColor(3, 0);
        for (const auto& row : block)
            for (const auto& pixel : row)
                for (int c = 0; c < 3; c++)
                    avgColor[c] += pixel[c];

        for (int c = 0; c < 3; c++)
            avgColor[c] /= (width * height);

        return new Node(x, y, width, true, avgColor);
    }

    int newWidth = width / 2;
    int newHeight = height / 2;
    
    vector<int> avgColor(3, 0);
    if (x == 0 && y == 0 && width == image.size() && height == image[0].size()) {
        for (const auto& row : image) {
            for (const auto& pixel : row) {
                for (int c = 0; c < 3; c++) {
                    avgColor[c] += pixel[c];
                }
            }
        }
        for (int c = 0; c < 3; c++) {
            avgColor[c] /= (width * height);
        }
    } 
    
    Node* node = new Node(x, y, width, false, avgColor);

    node->children[0] = compressBlock(image, x, y, newWidth, newHeight, minBlockSize); 
    node->children[1] = (width > newWidth) ? compressBlock(image, x, y + newWidth, width - newWidth, newHeight, minBlockSize) : nullptr;
    node->children[2] = (height > newHeight) ? compressBlock(image, x + newHeight, y, newWidth, height - newHeight, minBlockSize) : nullptr;
    node->children[3] = (width > newWidth && height > newHeight) ? compressBlock(image, x + newHeight, y + newWidth, width - newWidth, height - newHeight, minBlockSize) : nullptr;


    return node;
}

vector<vector<vector<int>>> Quadtree::getCompressedImage() {
    if (!root) return {};

    vector<vector<vector<int>>> compressedImage(
        originalHeight, vector<vector<int>>(originalWidth, vector<int>(3, 255))
    );

    int frameNumber = 0;
    Image img;

    int maxDepth = getMaxDepth(root);
    for (int depth = 0; depth <= maxDepth; depth++) {
        reconstructImage(root, compressedImage, frameNumber, img, 0, depth);
    }

    return compressedImage;
}

void Quadtree::reconstructImage(Node* root, vector<vector<vector<int>>>& image, int& frameNumber, Image& img, int depth, int maxDepth) {
    if (!root) return;

    queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        int levelSize = q.size();
        for (int i = 0; i < levelSize; i++) {
            Node* node = q.front();
            q.pop();

            if (node->isLeaf) {
                for (int i = 0; i < node->size; i++) {
                    for (int j = 0; j < node->size; j++) {
                        int px = node->x + i;
                        int py = node->y + j;
                        if (px >= image.size() || py >= image[0].size()) continue;
                        image[px][py] = node->color;
                    }
                }
            } else {
                for (int i = 0; i < 4; i++) {
                    if (node->children[i]) q.push(node->children[i]);
                }
            }
        }
    }
}

int Quadtree::getNodeCount(Node* node) const {
    if (!node) return 0;
    int count = 1;
    for (int i = 0; i < 4; i++) {
        count += getNodeCount(node->children[i]);
    }
    return count;
}

int Quadtree::getNodeCount() const {
    return getNodeCount(root);
}

int Quadtree::getMaxDepth(Node* node) const {
    if (!node) return 0;
    if (node->isLeaf) return 0;

    int maxChildDepth = 0;
    for (int i = 0; i < 4; i++) {
        if (node->children[i]) {
            maxChildDepth = max(maxChildDepth, getMaxDepth(node->children[i]));
        }
    }
    return maxChildDepth + 1;
}

int Quadtree::getMaxDepth() const {
    return getMaxDepth(root);
}