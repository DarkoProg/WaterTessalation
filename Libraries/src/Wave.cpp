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

/* template<typename T, std::size_t N, std::size_t M> */
void Wave::test(int (*waveTexture))
{
    float maxValue = 0;
    float minValue = 0;

    for(double t = 0; t < 1; t++)
    {
        //starts at 1, otherwise the first pixel will be 0 always
        for(int y = 1; y <= height; y++)
        {
            for (int x = 1; x <= width; x++) {
                // !!!!!!!!!!!! rename colorValue, colorValue is gona be used for rgb values later !!!!!!!!!!!!!!!!
                glm::vec3 dir = Wave::CalculateCircularDirection(glm::vec3 (x, y, 0), glm::vec3 (0, 0, 0));

                float amplitude = 5;
                float wavePointValue = SelectWaves(x, y, t, 1, 5, amplitude, 1, glm::vec3(-0.5f, -0.5f, 0)); 

                float colorValue = MapToGrayscale(amplitude, wavePointValue); 
                /* glm::vec3 normal = Wave::CreateNormal(colorValue, x, y); */
                if (maxValue < colorValue) maxValue = colorValue;
                if (minValue > colorValue) minValue = colorValue;
                ///map to rgb color (0-255)
                //value -> (-1, 1) + 1 -> (0 , 2) -> (0, 255)

                /* waveTexture = colorValue; */
                *((waveTexture+(y-1)*width)+x-1) = colorValue; 
                /* std::cout << *((waveTexture+(y-1)*width)+x-1) << "    "; */
            }
            
        }

    }
    /* std::cout << "max: " << maxValue << std::endl; */
    /* std::cout << "min: " << minValue << std::endl; */
}

// speed * 2/wavelength
double Wave::SelectWaves(float x, float y, float t, float k, float wavelength, float amplitude, float speed, glm::vec3 direction)
{

    float frequency = 2.0f / wavelength;
    float FIspeed   = speed * 2.0f / wavelength;
    double wave = amplitude * std::sin(glm::dot(direction, glm::vec3 (x, y, 0)) * frequency + t * FIspeed);
    /* std::cout << frequency << " " << FIspeed << " " << wave << "    "; */
    return wave;
}

double WaveDerivitive(float x, float y, float t, float k, float wavelength, float amplitude, float speed, glm::vec3 direction)
{
    float frequency = 2.0f / wavelength;
    float FIspeed   = speed * 2.0f / wavelength;
    double wave = k * direction.x * x * frequency * amplitude
                * std::pow((sin(glm::dot(direction, glm::vec3(x, y, 0)) * frequency + t * FIspeed) +1), k-1)
                * cos(glm::dot(direction, glm::vec3(x, y, 0)) * frequency + t * FIspeed);


    return wave;
}

glm::mat3 Wave::Surf2World(int x, int y, double waveDerivitive)
{
    glm::vec3 tangent(0, 1, waveDerivitive);
    glm::vec3 binormal(1, 0, waveDerivitive);
    glm::vec3 normal = glm::cross(binormal, tangent);

    return glm::mat3 (binormal, tangent, normal);
}

glm::mat3 Wave::Tex2Surf()
{

    return glm::mat3(1);
}


int Wave::MapToGrayscale(float amplitude, double wave)
{
    return (((wave / amplitude) + 1) * 127.5);
}

glm::vec3 Wave::CalculateCircularDirection(glm::vec3 direction, glm::vec3 center)
{
    return((direction - center) / glm::distance(direction, center));
}

float Wave::Frequency(float wavelength)
{
    return std::sqrt(Wave::GRAVITY * 2 * Wave::PI / wavelength);
}
