/**
 * @file Assignment1.cpp
 *  
 * @brief Assignment 1 - HY-150
 * 
 * @author: CSD5449
 */

#include "std_lib_facilities.h"
#include "vec3.h"
#include "color.h"
#include <fstream> // For file I/O
#include <iostream>

// WrongInput: a class to be used as an exception
class WrongInput {};

int isInteger(string input, int option);
int checkInputInt(string input, int option);
float isFloat(string input);
float checkInputFloat(string input);

// errorNum: saves what error has appeared in the code
// errorNum == 0: there is no error
int errorNum = 0;

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
    int w = 0, h = 0, inType;
    float color1[3], color2[3];
    float rdis, gdis, bdis;
    color pixel_color;

    // menu
    // added the braces during production to be able to retract the code for the menu and make it more readable
    {
        cout << "Image Generator - CSD5449\n\n";
        
        cout << "Give image name (no need to add .ppm at the end): ";
        cin >> name;

        cout << "Give image height: ";
        cin >> inp;
        h = checkInputInt(inp, 0);
        cout << "Give image width: ";
        cin >> inp;
        w = checkInputInt(inp, 0);

        cout << "What image do you want to generate: \n\t1. left -> right\n\t2. top -> bottom\n\t3. middle -> out\n";
        cin >> inp;
        inType = checkInputInt(inp, 1);

        cout << "Give RGB values for starting color\n";
        cout << "Red: ";
        cin >> inp;
        color1[0] = checkInputFloat(inp);
        cout << "Green: ";
        cin >> inp;
        color1[1] = checkInputFloat(inp);
        cout << "Blue: ";
        cin >> inp;
        color1[2] = checkInputFloat(inp);

        cout << "Give RGB values for ending color\n";
        cout << "Red: ";
        cin >> inp;
        color2[0] = checkInputFloat(inp);
        cout << "Green: ";
        cin >> inp;
        color2[1] = checkInputFloat(inp);
        cout << "Blue: ";
        cin >> inp;
        color2[2] = checkInputFloat(inp);
    }
    
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

    if (inType == 1) // left to right fade
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
            }

            // reseting the colours
            r = color1[0];
            g = color1[1];
            b = color1[2];

        }
    }
    else if (inType == 2) // from top to bottom
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
                g = (dist/(min-dist)) * color1[1] + ((min-dist)/(min)) * color2[1];
                b = (dist/(min-dist)) * color1[2] + ((min-dist)/(min)) * color2[2];

                color pixel_color = color(r, g, b);
                write_color(fout, pixel_color);
            }
        }
    }

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
        if (option && num > 3) 
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

/* Same as in isInteger(string input) but for floats
Checking if string is number: user Ben Voigt stackoverflow (https://stackoverflow.com/a/16575025)*/
float isFloat(string input)
{
    char* p;
    float num;

    strtol(input.c_str(), &p, 10);
    if (*p == 0) // if it is a integer
    {
        num = stof(input);
        if (num < 0 || num > 1)
        {
            errno = 1;
            throw WrongInput();
            return -1;
        }
    }
    else // if  it isn't
    {
        errno = 2;
        throw WrongInput();
        return -1;
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
            else if (errno == 3) cout << "Option must be between 1 and 3: ";
            cin >> input;
            // resseting the error
            errno = 0;
        }
    }

    return inp;
}

/* Same as in checkInputInt(string input) but for floats */
float checkInputFloat(string input)
{
    float inp = -1;
    while (inp == -1)
    {
        try {
            inp = isFloat(input);
        }
        catch (WrongInput& b)
        {
            if (errno == 1) cout << "Number must be between 0 and 1, please try again: ";
            else cout << "Input must be number, please try again: ";
            cin >> input;
            // resseting the error
            errno = 0;
        }
    }

    return inp;
}
