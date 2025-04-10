#ifndef IMAGECOMPRESSION_H
#define IMAGECOMPRESSION_H

#include "errorMeasurements.hpp"
#include "image.hpp"
#include <vector>

enum class ErrorMethod {
    VARIANCE,
    MEAN_ABSOLUTE_DEVIATION,
    MAX_PIXEL_DIFFERENCE,
    ENTROPY
};

class Quadtree {
    public:
        int originalWidth, originalHeight;  // Store original image dimensions
        struct Node {
            int x, y, size;
            bool isLeaf;
            std::vector<int> color;  // Store RGB values
            Node* children[4] = {nullptr, nullptr, nullptr, nullptr};

            Node(int x, int y, int size, bool isLeaf, std::vector<int> color);
            ~Node();
        };
        Quadtree(int threshold, ErrorMethod method);
        ~Quadtree();
    
        void build(const std::vector<std::vector<std::vector<int>>>& image, int imgWidth, int imgHeight, int minBlockSize);
        std::vector<std::vector<std::vector<int>>> getCompressedImage();
        
        int getNodeCount() const;
        int getMaxDepth() const;

    private:    
        Node* root = nullptr;
        int threshold;
        ErrorMethod method;
    
        Node* compressBlock(const std::vector<std::vector<std::vector<int>>>& image, int x, int y, int width, int height, int minBlockSize);
        void reconstructImage(Node* node, std::vector<std::vector<std::vector<int>>>& image, int& frameNumber, Image& img, int depth, int maxDepth);

        int getNodeCount(Node* node) const;
        int getMaxDepth(Node* node) const;
    };    


    
#endif 
