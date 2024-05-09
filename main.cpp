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
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"

color ray_color(const ray &r, const hittable &world)
{
    hit_record rec;
    if (world.hit(r, interval(0, RT_INFINITY), rec))
    {
        return 0.5 * (rec.normal + color(1, 1, 1));
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

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

    ConvertImageAndDisplay(ppmFileName);
}

void RenderTexture(int image_width, int image_height, auto pixel_delta_u, auto pixel_delta_v, auto pixel00_loc,
                   auto camera_center, hittable_list &world, std::ofstream &ppmFile)
{
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    ppmFile << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j)
    {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++)
        {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r, world);
            write_color(std::cout, pixel_color, ppmFile);
        }
    }

    std::clog << "\rDone.                 \n";
}

void SetupCamera(int image_width, int image_height, vec3 &camera_center, vec3 &pixel_delta_u, vec3 &pixel_delta_v,
                 vec3 &pixel00_loc)
{
    // Camera
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width) / image_height);
    camera_center = point3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
}

void SetupImage(int &image_width, int &image_height)
{
    image_width = 400;
    auto aspect_ratio = 16.0 / 9.0;
    image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;
}

int main()
{
    int image_width;
    int image_height;
    SetupImage(image_width, image_height);

    vec3 camera_center;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
    vec3 pixel00_loc;
    SetupCamera(image_width, image_height, camera_center, pixel_delta_u, pixel_delta_v, pixel00_loc);

    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    std::ofstream ppmFile;
    std::string const ppmFileName = "../output/texture";
    ppmFile.open(ppmFileName + ".txt");

    // Render
    RenderTexture(image_width, image_height, pixel_delta_u, pixel_delta_v, pixel00_loc, camera_center, world, ppmFile);

    ppmFile.close();

    RenameFile(ppmFileName);

    return 0;
};