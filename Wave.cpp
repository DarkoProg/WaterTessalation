#include "Wave.h"
#include <cmath>
/* #include <iostream> */
/* #include <noise/module/perlin.h> */

using namespace noise;

Wave::Wave()
{
}


void Wave::test()
{
    for(double y = 1; y <= 10; y++)
    {
        for (double x = 1; x <= 10; x++) {
            int ColorValue = round((perlinModule.GetValue((double) (x/y), (double) (y/x), 0) + 1) / 2 * 255); 
            if(ColorValue > 255) ColorValue = 255;
            if(ColorValue < 0) ColorValue = 0;
            std::cout << ColorValue << "    ";
        }
        
        std::cout << std::endl;
    }
}

void Wave::CreateWave(int image[500][500])
{
   for(int j = 0; j < 500; j++)
   {
        for(int i = 0; i < 500; i++)
        {
            image[i][j] = 0;
        }
   }
}
// speed * 2/wavelength
double Wave::SelectWaves(int x, int y, int t, float wavelength, float amplitude, float speed, glm::vec3 direction)
{
    /* cross(direction, direction); */
    float H = amplitude * std::sin(glm::dot(direction, glm::vec3(x, y, 0) * (2/wavelength)) + (t * (speed * 2/wavelength)));
    return 0;
}
