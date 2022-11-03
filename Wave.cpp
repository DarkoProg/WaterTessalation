#include "Wave.h"
#include <cmath>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <iostream>
#include <math.h>
/* #include <iostream> */
/* #include <noise/module/perlin.h> */

using namespace noise;

Wave::Wave()
{
}


void Wave::test()
{
    for(double t = 0; t < 10; t++)
    {
        for(double y = 1; y <= 10; y++)
        {
            for (double x = 1; x <= 10; x++) {
                /* int ColorValue = round((perlinModule.GetValue(SelectWaves(x, y, 2, 1, 5, 8, 2, glm::vec3 (0.5, 0.5, 0)), 0, 0) + 1) / 2 * 255); */ 
                int ColorValue = SelectWaves(x, y, t, 1, 5, 8, 1, glm::vec3 (0.5, -0.5, 0)); 
                glm::vec3 normal = Wave::CreateNormal(ColorValue, x, y);
                /* std::cout << glm::to_string(normal) << "    "; */

                /* if(ColorValue > 255) ColorValue = 255; */
                /* if(ColorValue < 0) ColorValue = 0; */
                std::cout << ColorValue << "    ";
            }
            
            std::cout << std::endl;
        }

        std::cout << std::endl;
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
double Wave::SelectWaves(float x, float y, float t, float k, float wavelength, float amplitude, float speed, glm::vec3 direction)
{
    /* cross(direction, direction); */

    float frequency = 2 / wavelength;
    float FIspeed   = speed * 2 / wavelength;
    double wave = k * direction.x * x * frequency * amplitude
                * std::pow((sin(glm::dot(direction, glm::vec3(x, y, 0)) * frequency + t * FIspeed) +1), k-1)
                * cos(glm::dot(direction, glm::vec3(x, y, 0)) * frequency + t * FIspeed);
    return wave;
}


glm::vec3 Wave::CreateTangent(float selectedWave, int x, int y)
{
    return glm::vec3(0, 1, selectedWave);
}

glm::vec3 Wave::CreateNormal(float selectedWave, int x, int y)
{
    glm::vec3 tangent(0, 1, selectedWave);
    glm::vec3 binormal(1, 0, selectedWave);
    glm::vec3 normal = glm::cross(binormal, tangent);
    return normal;
}

glm::vec3 CalculateCircularDirection(glm::vec3 direction, glm::vec3 center)
{
    return((direction - center) / glm::distance(direction, center));
}

float Frequency(float wavelength)
{
    return std::sqrt(Wave::GRAVITY * 2 * Wave::PI / wavelength);
}
