# Tucil2_13523070

Program ini mampu melakukan kompresi gambar dengan sistem quadtree sesuai dengan ambang batas serta ukuran blok dari pengguna

## Requirements
- C++
- Images to compress (.jpg, .png)

## How to run
1. Clone repository menggunakan
   ```sh
   git clone https://github.com/SebastianHYH/Tucil2_13523070.git
2. Buka directory program di
   ```sh
   ..\src\
3. Compile program dengan
   ```sh
   g++ -o ../bin/imageCompression main.cpp image.cpp Quadtree.cpp errorMeasurements.cpp
   ```
   Program akan dicompile dan diletakkan di folder bin

4. Jalankan program dengan
    ```sh
    ./imageCompression.exe