#include "vec3.h"
#include "color.h"
#include <fstream> // For file I/O
#include <iostream>

using namespace std;

int image_width = 256;
int image_height = 256;

int main() 
{
    fstream fout;
    fout.open("image.ppm", ios::out | ios::trunc); // Opens a file for output and overwrites the file if it already exists.

    // fout variable is the file stream. It can be used the same way as cout, but it writes to the file instead of the console.
    fout << "P3\n" << image_width << ' ' << image_height << "\n255\n"; // P3 is the PPM file format, 255 is the maximum color value.

    for (int j = 0; j < image_height; ++j)
    {
        for (int i = 0; i < image_width; ++i)
        {
            // The color values must be in the range [0, 1]
            double r = 0;                                           // The red color value
            double g = (double(i) / (image_height-1));              // The green color value.
            double b = (double(j) / (image_width-1));               // The blue color value

            color pixel_color = color(r, g, b);

            write_color(fout, pixel_color); // Writes the pixels color to the file.
        }
    }

    fout.close(); // Close the file.

    return 0;     
}