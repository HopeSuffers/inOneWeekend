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
#include "RTweekend.hpp"
#include "Camera.hpp"
#include "HitTableList.hpp"
#include "Material.hpp"
#include "Sphere.hpp"

HitTableList SetupWorld();

void SetupMaterials(HitTableList &world);

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

std::unique_ptr<Camera> SetupCamera()
{
    auto cam = std::make_unique<Camera>();

    cam->aspectRatio = 16.0 / 9.0;
    cam->imageWidth = 1200;
    cam->samplesPerPixel = 100;
    cam->maxDepth = 20;

    cam->vFov = 20;
    cam->lookFrom = Point3(13, 2, 3);
    cam->lookAt = Point3(0, 0, 0);
    cam->vUp = Vec3(0, 1, 0);

    cam->defocusAngle = 0.6;
    cam->focusDist = 10.0;

    return cam;
}

int main()
{
    HitTableList world = SetupWorld();

    SetupMaterials(world);

    auto cam = SetupCamera();

    std::ofstream ppmFile;
    std::string const ppmFileName = "../output/texture";
    ppmFile.open(ppmFileName + ".txt");

    cam->Render(world, ppmFile);

    ppmFile.close();
    RenameFile(ppmFileName);
    ConvertImageAndDisplay(ppmFileName);

    return 0;
}

void SetupMaterials(HitTableList &world)
{
    auto material1 = std::__1::make_shared<Dielectric>(1.5);
    world.Add(std::__1::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = std::__1::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.Add(std::__1::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = std::__1::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.Add(std::__1::make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));
}

HitTableList SetupWorld()
{
    // World
    HitTableList world;

    auto groundMaterial = std::__1::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.Add(std::__1::make_shared<Sphere>(Point3(0, -1000, 0), 1000, groundMaterial));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto chooseMat = RandomDouble();
            Point3 center(a + 0.9 * RandomDouble(), 0.2, b + 0.9 * RandomDouble());

            if ((center - Point3(4, 0.2, 0)).Length() > 0.9)
            {
                std::__1::shared_ptr<Material> sphereMaterial;

                if (chooseMat < 0.8)
                {
                    // diffuse
                    auto albedo = Color::random() * Color::random();
                    sphereMaterial = std::__1::make_shared<Lambertian>(albedo);
                    world.Add(std::__1::make_shared<Sphere>(center, 0.2, sphereMaterial));
                }
                else if (chooseMat < 0.95)
                {
                    // Metal
                    auto albedo = Color::random(0.5, 1);
                    auto fuzz = RandomDouble(0, 0.5);
                    sphereMaterial = std::__1::make_shared<Metal>(albedo, fuzz);
                    world.Add(std::__1::make_shared<Sphere>(center, 0.2, sphereMaterial));
                }
                else
                {
                    // glass
                    sphereMaterial = std::__1::make_shared<Dielectric>(1.5);
                    world.Add(std::__1::make_shared<Sphere>(center, 0.2, sphereMaterial));
                }
            }
        }
    }
    return world;
}
