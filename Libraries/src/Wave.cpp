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

void Wave::GenWave(int (*waveTexture), float t)
{
    float maxValue = 0;
    float minValue = 0;

        for(int y = 1; y <= height; y++)
        {
            for (int x = 1; x <= width; x++) {

                float amplitude = 100;
                float wavePointValue = SelectWaves(x, y, t, 1, 5, amplitude, 10, glm::vec3(-0.5f, -0.5f, 0)); 

                float colorValue = MapToGrayscale(amplitude, wavePointValue); 
                /* float perlinValue = perlinModule.GetValue(x*100, y*100, 0); */
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


