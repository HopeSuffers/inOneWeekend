// Standard C++ library headers
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

// Conditional system headers based on the OS
#if defined(__APPLE__) || defined(__linux__)
#include <cstdlib> // For system()
#elif defined(_WIN32)
#include <windows.h> // For ShellExecute()
#endif

// External library headers
#include <Magick++.h>

// Internal project-specific headers
#include "vec3.hpp"
#include "color.hpp"

void ConvertImageAndDisplay(std::string ppmFileName)
{
    try {
        Magick::Image testImage;
        testImage.read((ppmFileName + ".ppm").c_str());
        testImage.write((ppmFileName + ".png").c_str());

        // Open the output file with the default application
        std::string command;
#if defined(__APPLE__)
        command = "open " + ppmFileName + ".png";
        system(command.c_str());
#elif defined(__linux__)
        command = "xdg-open " + ppmFileName + ".png";
        system(command.c_str());
#elif defined(_WIN32)
        command = ppmFileName + ".png";
        ShellExecute(NULL, "open", command.c_str(), NULL, NULL, SW_SHOWNORMAL);
#endif
    }
    catch (const Magick::Exception &error_) {
        std::cout << "Caught exception: " << error_.what() << std::endl;
    }
}

void RenameFile(std::string ppmFileName) {
    try {
        // check if texture already exists
        if (std::filesystem::exists(ppmFileName + ".ppm")) {
            std::filesystem::remove(ppmFileName + ".ppm");
        }

        // rename
        if (std::rename((ppmFileName + ".txt").c_str(), (ppmFileName + ".ppm").c_str()) != 0) {
            throw std::runtime_error("Failed to rename file.");
        }
        std::cout << "File renamed successfully from " << ppmFileName + ".txt" << " to " << ppmFileName + ".ppm" <<
                  ".\n";
    }
    catch (const std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        perror("System error");
    }

    ConvertImageAndDisplay(ppmFileName);
}

void RenderTexture(int image_width, int image_height, std::ofstream &ppmFile) {
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    ppmFile << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            auto pixel_color = color(double(i)/(image_width-1), double(j)/(image_height-1), 0);
            write_color(std::cout, pixel_color, ppmFile);

            //ppmFile << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }

    std::clog << "\rDone.                 \n";
}

int main() {
    // Image
    int image_width = 256;
    int image_height = 256;

    std::ofstream ppmFile;

    const std::string ppmFileName = "../output/texture";
    ppmFile.open(ppmFileName + ".txt");

    // Render
    RenderTexture(image_width, image_height, ppmFile);

    ppmFile.close();

    RenameFile(ppmFileName);

    return 0;
}

