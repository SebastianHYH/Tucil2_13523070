#include <vector>
#include <string>

class Image {
public:
    int width, height, channels;
    std::vector<unsigned char> pixels;  // Stores raw image data
    bool rotated;
    int frameNumber;

    Image();  
    ~Image(); 

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    long long getFileSize(const std::string& filePath) const;

    bool loadImage(const std::string& filepath);  
    bool saveImage(const std::string& filepath) const;  
    void setPixelMatrix(const std::vector<std::vector<std::vector<int>>>& newPixels);

    std::vector<std::vector<std::vector<int>>> getPixelMatrix() const;

};

