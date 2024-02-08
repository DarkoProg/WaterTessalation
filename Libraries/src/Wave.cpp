#include "../include/Wave.h"
#include <glm/ext/vector_float3.hpp>

Wave::Wave()
{
    height = 1000;
    width  = 1000;
}

Wave::Wave(int inputHeight, int inputWidth)
{
    height = inputHeight;
    width  = inputWidth;
}

void Wave::GenWave(int (*waveTexture), float t, float k, float wavelength, float amplitude, float speed, glm::vec3 direction)
{
    float maxValue = 0;
    float minValue = 0;

        for(int y = 1; y <= height; y++)
        {
            for (int x = 1; x <= width; x++) {

                /* float amplitude = 100; */
                /* float wavePointValue = SelectWaves(x, y, t, 1, 5, amplitude, 10, glm::vec3(-0.5f, -0.5f, 0)); */ 
                float wavePointValue = SelectWaves(x, y, t, k, wavelength, amplitude, speed, direction); 

                float perlinValue = (perlinModule.GetValue((float)x/width, (float)y/height, 0)+1)/2;
                /* std::cout << "Perlin: " << perlinValue << std::endl; */
                /* float colorValue = MapToGrayscale(amplitude, wavePointValue)* perlinValue; */ 
                float colorValue = MapToGrayscale(amplitude, wavePointValue); 
                /* float colorValue = (perlinValue)*255; */ 
                if (maxValue < colorValue) maxValue = colorValue;
                if (minValue > colorValue) minValue = colorValue;

                *((waveTexture+(y-1)*width)+x-1) = colorValue; 
            }
        }
}

// speed * 2/wavelength
double Wave::SelectWaves(float x, float y, float t, float k, float wavelength, float amplitude, float speed, glm::vec3 direction)
{

    float frequency = 2.0f / wavelength;
    float FIspeed   = speed * 2.0f / wavelength;
    double wave = amplitude * std::sin(glm::dot(direction, glm::vec3 (x, y, 0)) * frequency + t * FIspeed);
    return wave;
}


int Wave::MapToGrayscale(float amplitude, double wave)
{
    return (((wave / amplitude) + 1) * 127.5);
}


