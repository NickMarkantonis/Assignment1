/**
 * @file Assignment1.cpp
 *  
 * @brief Assignment 1 - HY-150
 * 
 * @author: CSD5449
 */

/*
The program works this way:
 - To generate a image from left to right and from to to bottom we start by calculating the gradient in which the two image are,
   in each loop then we raise the RGB value by the gradient in order to get a smooth transition as the RGb value of each pixel gradually 
   changes from the statring colour to the end colour.
 - To generate a image from inside out we start of by calculating the distance each pixel has from the center of the sphere, we then do
   the adequate operations based on how far away each pixel is with the intencity of the start and end colour to end up with a smooth
   transition from inside out.
*/

// how many different types of images the program can currently generate and how many of each type
#define totalImages 2
#define gradientTypes 3
#define patternTypes 1

#include "std_lib_facilities.h"
#include "vec3.h"
#include "color.h"
#include <fstream> // For file I/O
#include <iostream>

class WrongInput{ };

int checkInputInt(string inp, int option);
int isInteger(string input, int option);
float checkInputFloat(string input, int option);
float isFloat(string input, int option);

int errNo;

int main() 
{
    // name: name of the image, given by user | inp: input given by user at any time
    // r,g,b: the RGB values used while making the image
    // w,h: width / height of image | inType: what type of image the user wants generated
    // color#[3]: start and end collor given by user
    // #dis: the rate of displacement needed in order to make a smooth color transition
    // pixel_color: the pixel color of each different pixel
    string name, inp;
    float r, g, b;
    int w = 0, h = 0, inType1, inType2, count = 0, pos = 0, totalPixels;
    float color1[3], color2[3];
    float rdis, gdis, bdis;
    color pixel_color;

    // menu
    cout << "Image Generator - CSD5449\n\n";
    
    cout << "Give image name, no need to add the .ppm at the end: ";
    cin >> name;

    cout << "Give image width: ";
    cin >> inp;
    w = checkInputInt(inp, 0);

    cout << "Give image height: ";
    cin >> inp;
    h = checkInputInt(inp, 0);
    totalPixels = w * h / 100;

    cout << "Select Image type: \n\t1. Gradient \n\t2. Geometric Pattern\n";
    cin >> inp;
    inType2 = checkInputInt(inp, totalImages);
    
    if (inType2 == 1)
    {
        cout << "Select Gradient descending: \n\t1. left->right \n\t2. top -> bottom \n\t3. middle -> out\n";
        cin >> inp;
        inType1 = checkInputInt(inp, gradientTypes);
    }
    else if (inType2 == 2)
    {
        cout << "Select geometric pattern: \n\t1. Sine Wave\n";
        cin >> inp;
        inType1 = checkInputInt(inp, patternTypes);
    }

    cout << "Select colour 1: \n";
    cout << "Red [0-1]: ";
    cin >> inp;
    color1[0] = checkInputFloat(inp, 1);
    cout << "Green [0-1]: ";
    cin >> inp;
    color1[1] = checkInputFloat(inp, 1);
    cout << "Blue [0-1]: ";
    cin >> inp;
    color1[2] = checkInputFloat(inp, 1);



    cout << "Select colour 2: \n";
    cout << "Red [0-1]: ";
    cin >> inp;
    color2[0] = checkInputFloat(inp, 1);
    cout << "Green [0-1]: ";
    cin >> inp;
    color2[1] = checkInputFloat(inp, 1);
    cout << "Blue [0-1]: ";
    cin >> inp;
    color2[2] = checkInputFloat(inp, 1);
    cout << "\n--------------------------------------------------------------------------------------\n\n";
    
    // creating the image
    fstream fout;
    fout.open(name + ".ppm", ios::out | ios::trunc);
    fout << "P3\n" << w << ' ' << h << "\n255\n";

    rdis = color2[0] - color1[0];
    gdis = color2[1] - color1[1];
    bdis = color2[2] - color1[2];

    r = color1[0];
    g = color1[1];
    b = color1[2];

    if (inType2 == 1) { // gradient
        if (inType1 == 1) // left to right fade
        {
            rdis /= w;
            gdis /= w;
            bdis /= w;

            for (int i = 0; i < h; i++)
            {
                for (int j = 0; j < w; j++)
                {
                    color pixel_color = color(r, g, b);
                    write_color(fout, pixel_color);

                    r += rdis;
                    g += gdis;
                    b += bdis;

                    pos++;
                    if (pos == totalPixels) {
                        count++;
                        cout << "Generatting " << count << "%\n";
                        pos = 0;  
                    }

                }

                // reseting the colours
                r = color1[0];
                g = color1[1];
                b = color1[2];

            }
        }
        else if (inType1 == 2) // from top to bottom
        {
            rdis /= h;
            gdis /= h;
            bdis /= h;

            for (int i = 0; i < h; i++)
            {
                for (int j = 0; j < w; j++)
                {
                    color pixel_color = color(r, g, b);
                    write_color(fout, pixel_color);

                    pos++;
                    if (pos == totalPixels) {
                        count++;                    
                        cout << "Generatting " << count << "%\n";
                        pos = 0; 
                    }

                }

                r += rdis;
                g += gdis;
                b += bdis;
            }
        }
        else // from middle to out
    {
        float dist;
        int min;

        // finding if the height is smaller or the width
        min = w;
        if (h < w) min = h;

        color1[0] /= 2;
        color1[1] /= 2;
        color1[2] /= 2;

        color2[0] /= 2;
        color2[1] /= 2;
        color2[2] /= 2;

        for (int i = 0; i < h; i++)
        {
            for (int j = 0; j < w; j++)
            {
                dist = sqrt((i-h/2) * (i - h/2) + (j - w/2) * (j - w/2));
                r = (dist/(min-dist)) * color1[0] + ((min-dist)/(min)) * color2[0];
                if (r > 1) r = 1;
                else if (r < 0) r = 0;

                g = (dist/(min-dist)) * color1[1] + ((min-dist)/(min)) * color2[1];
                if (g > 1) g = 1;
                else if (g < 0) g = 0;

                b = (dist/(min-dist)) * color1[2] + ((min-dist)/(min)) * color2[2];
                if (b > 1) b = 1;
                else if (b < 0) b = 0;

                color pixel_color = color(r, g, b);
                write_color(fout, pixel_color);

                pos++;
                if (pos == totalPixels) {
                    count++;                    
                    cout << "Generatting " << count << "%\n";
                    pos = 0; 
                }
            }
        }
    }
    }
    else if (inType2 == 2) // making geometric shape
    {
        if (inType1 == 1) // sin wave
        {
            double LineWidth = 5.0/h, xNorm, yNorm;
            for (int x = 0; x < h; x++)
            {
                for (int y = 0; y < w; y++)
                {
                    xNorm = ( (double) y / (h-1)) - 0.5;
                    yNorm = ( (double) x / (h-1)) - 0.5;

                    if ( abs( yNorm - 0.4 * sin( xNorm / 0.1 ) ) < LineWidth )
                        pixel_color = color( color1[0], color1[1], color1[2] );
                    else 
                        pixel_color = color( color2[0], color2[1], color2[2] );
                    write_color(fout, pixel_color);

                    pos++;
                    if (pos == totalPixels) {
                        count++;                    
                        cout << "Generatting " << count << "%\n";
                        pos = 0; 
                    }                    
                }
            }
        }
    }

    cout << "Generating Done!\n";

    // closing the image
    fout.close();

    return 0;
}

/* The function checks the following:
1. If the string is a integer and if it is we return the adequate integer
   If it isn't throw a exception with errorNum == 2
2. In case it is we also check if it possitive
   If it isn't we throw a exception with errorNum == 1
3. If the string is a num but a decimal we throw an exception 
4. If the int is bigger then 
Checking if string is number: user Ben Voigt stackoverflow (https://stackoverflow.com/a/16575025)*/
int isInteger(string input, int option)
{
    char* p;
    int num;

    strtol(input.c_str(), &p, 10);
    if (*p == 0) // if it is a integer
    {
        num = stoi(input);
        if (num <= 0)
        {
            errno = 1;
            throw WrongInput();
            return 0;
        }
        if (option && num > option)
        {
            errno = 3;
            throw WrongInput();
            return 0;
        }
    } else // if  it isn't
    {
        errno = 2;
        throw WrongInput();
        return 0;
    }

    return num;
}

/* The following function given the input in string form checks 
(using the isInteger(string input)) function if the input provided is correct
In case the int option is true (!= 0) we also check that the returned int is less then 4
if it isn't we throw an exception with errno == 3 */
int checkInputInt(string input, int option)
{
    int inp = 0;
    while (!inp)
    {
        try {
            inp = isInteger(input, option);
        }
        catch (WrongInput& b)
        {
            if (errno == 1) cout << "Number must be possitive integer, please try again: ";
            else if (errno == 2) cout << "Input must be integer, please try again: ";
            else if (errno == 3) cout << "Option must be between 1 and " << option << ": ";
            cin >> input;
            // resseting the error
            errno = 0;
        }
    }

    return inp;
}


/* Same as the adequate isInteger but for floats 
Checking if string is float: user Bill the Lizzard stackoverflow (https://stackoverflow.com/a/447307)*/
float isFloat(string input, int option)
{
    bool isfloat;

    istringstream iss(input);
    float f;
    iss >> noskipws >> f;
    isfloat = iss.eof() && !iss.fail();

    // check if string is float
    if (isfloat) // if it is a float
    {
        if (f < 0)
        {
            errno = 1;
            throw WrongInput();
            return 0;
        }
        if (option && f > option)
        {
            errno = 3;
            throw WrongInput();
            return 0;
        }
    } else // if  it isn't
    {
        errno = 2;
        throw WrongInput();
        return 0;
    }

    return f;
}

/* Same as checkInputInt but for floats */
float checkInputFloat(string input, int option)
{
    float inp = -1;
    while (inp == -1)
    {
        try {
            inp = isFloat(input, option);
        }
        catch (WrongInput& b)
        {
            if (errno == 1) cout << "Number must be possitive float, please try again: ";
            else if (errno == 2) cout << "Input must be float, please try again: ";
            else if (errno == 3) cout << "Option must be between 0 and " << option << ": ";
            cin >> input;
            // resseting the error
            errno = 0;
        }
    }

    return inp;
}
