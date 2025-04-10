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
    bool isSaved; // Menyimpan status penyimpanan gambar

    cout << "Masukkan alamat gambar: ";
    getline(cin, inputAddress);
    Image img;
    if (!img.loadImage(inputAddress)) {
        cout << "Gagal memuat gambar." << endl;
        return 1;
    }

    do {
        cout << "Pilih metode pengukuran error:" << endl;
        cout << "1. Variance" << endl;
        cout << "2. Mean Absolute Deviation (MAD)" << endl;
        cout << "3. Max Pixel Difference" << endl;
        cout << "4. Entropy" << endl;
        cout << "Masukkan pilihan (1-4): ";
        cin >> errorMeasurementMethod;
    
        if (cin.fail() || errorMeasurementMethod < 1 || errorMeasurementMethod > 4) {
            cout << "Input tidak valid. Masukkan angka antara 1 dan 4." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (errorMeasurementMethod < 1 || errorMeasurementMethod > 4);

    do {
        cout << "Masukkan ambang batas (threshold, angka positif): ";
        cin >> threshold;
    
        if (cin.fail() || threshold <= 0) {
            cout << "Input tidak valid. Masukkan angka positif." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (threshold <= 0);

    do {
        cout << "Masukkan ukuran minimum blok (angka positif): ";
        cin >> minimumBlockSize;
    
        if (cin.fail() || minimumBlockSize <= 0) {
            cout << "Input tidak valid. Masukkan angka positif." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (minimumBlockSize <= 0);

    if (minimumBlockSize < 1) {
        cout << "Ukuran minimum blok tidak valid" << endl;
        return 1;
    }
    
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
    if (img.saveImage(outputAddress)) {
        isSaved = true;
    } else {
        isSaved = false;
    }
    
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
    if (isSaved) {
        cout << "Gambar berhasil dikompresi dan disimpan di " << outputAddress << endl;
    } else {
        cout << "Gambar gagal disimpan " << endl;
    }
    cout << "============================" << endl;

    return 0;
}