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
#include "rtweekend.hpp"
#include "camera.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"

void ConvertImageAndDisplay(std::string ppmFileName)
{
    try
    {
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
    catch (const Magick::Exception &error_)
    {
        std::cout << "Caught exception: " << error_.what() << std::endl;
    }
}

void RenameFile(std::string ppmFileName)
{
    try
    {
        // check if texture already exists
        if (std::filesystem::exists(ppmFileName + ".ppm"))
        {
            std::filesystem::remove(ppmFileName + ".ppm");
        }

        // rename
        if (std::rename((ppmFileName + ".txt").c_str(), (ppmFileName + ".ppm").c_str()) != 0)
        {
            throw std::runtime_error("Failed to rename file.");
        }
        std::cout << "File renamed successfully from " << ppmFileName + ".txt" << " to " << ppmFileName + ".ppm" <<
                  ".\n";
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        perror("System error");
    }
}

int main()
{
    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    std::ofstream ppmFile;
    std::string const ppmFileName = "../output/texture";
    ppmFile.open(ppmFileName + ".txt");

    cam.render(world, ppmFile);

    ppmFile.close();
    RenameFile(ppmFileName);
    ConvertImageAndDisplay(ppmFileName);

    return 0;
};
