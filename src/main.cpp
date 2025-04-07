#include <iostream>
#include <string>
#include <chrono>
#include "include\quadtree.hpp"
#include "include\errorMeasurements.hpp"
using namespace std;

int main() {
    string inputAddress; // Alamat gambar yang akan dikompresi
    string outputAddress; // Alamat gambar hasil kompresi
    int errorMeasurementMethod; // Metode kompresi yang dipilih
    int threshold; // Ambang batas untuk kompresi
    int minimumBlockSize;

    cout << "Masukkan alamat gambar: ";
    getline(cin, inputAddress);
    Image img;
    if (!img.loadImage(inputAddress)) {
        cout << "Gagal memuat gambar." << endl;
        return 1;
    }

    cout << "1. Variance" << endl;
    cout << "2. Mean Absolute Deviation (MAD)" << endl;
    cout << "3. Max Pixel Difference" << endl;
    cout << "4. Entropy" << endl;
    cout << "Pilih metode pengukuran error: ";
    cin >> errorMeasurementMethod;

    if (errorMeasurementMethod < 1 || errorMeasurementMethod > 4) {
        cout << "Metode tidak valid" << endl;
        return 1;
    }

    cout << "Masukkan ambang batas: ";
    cin >> threshold;

    cout << "Masukkan ukuran minimum blok: ";
    cin >> minimumBlockSize;
    
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cout << "Masukkan alamat gambar hasil kompresi: ";
    getline(cin, outputAddress);

    Quadtree qt(threshold, static_cast<ErrorMethod>(errorMeasurementMethod-1));
    std::vector<std::vector<std::vector<int>>> pixelMatrix = img.getPixelMatrix();
    std::vector<std::vector<std::vector<int>>> compressedMatrix;
    auto startTime = chrono::high_resolution_clock::now();

    if (errorMeasurementMethod == 1) {
        cout << "Menggunakan metode Variance" << endl;
        qt.build(pixelMatrix, img.getWidth(), img.getHeight(), minimumBlockSize);
        compressedMatrix = qt.getCompressedImage();
    } else if (errorMeasurementMethod == 2) {
        cout << "Menggunakan metode Mean Absolute Deviation (MAD)" << endl;
        qt.build(pixelMatrix, img.getWidth(), img.getHeight(), minimumBlockSize);
        compressedMatrix = qt.getCompressedImage();
    } else if (errorMeasurementMethod == 3) {
        cout << "Menggunakan metode Max Pixel Difference" << endl;
        qt.build(pixelMatrix, img.getWidth(), img.getHeight(), minimumBlockSize);
        compressedMatrix = qt.getCompressedImage();
    } else if (errorMeasurementMethod == 4) {
        cout << "Menggunakan metode Entropy" << endl;
        qt.build(pixelMatrix, img.getWidth(), img.getHeight(), minimumBlockSize);
        compressedMatrix = qt.getCompressedImage();
    } else {
        cout << "Metode tidak valid" << endl;
        return 1;
    }

    img.setPixelMatrix(compressedMatrix);
    img.saveImage(outputAddress);
    
    auto endTime = chrono::high_resolution_clock::now();
    chrono::duration<double> executionTime = endTime - startTime;

    long long originalSize = img.getFileSize(inputAddress) / 1000;
    long long compressedSize = img.getFileSize(outputAddress) / 1000;
    double compressionPercentage = 100.0 * (1.0 - static_cast<double>(compressedSize) / originalSize);

    cout << "============================" << endl;
    cout << "Waktu eksekusi: " << executionTime.count() << " detik" << endl;
    cout << "Ukuran gambar sebelum: " << originalSize << " KB" << endl;
    cout << "Ukuran gambar setelah: " << compressedSize << " KB" << endl;
    cout << "Persentase kompresi: " << compressionPercentage << "%" << endl;
    cout << "Kedalaman pohon: " << qt.getMaxDepth() << endl;
    cout << "Banyak simpul pada pohon: " << qt.getNodeCount() << endl;
    cout << "Gambar berhasil dikompresi dan disimpan di " << outputAddress << endl;
    cout << "============================" << endl;

    return 0;
}