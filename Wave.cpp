#include "Wave.h"


Wave::Wave()
{
}


void Wave::test()
{
    float maxValue = 0;
    float minValue = 0;

    for(double t = 0; t < 1; t++)
    {
        //starts at 1, otherwise the first pixel will be 0 always
        for(double y = 1; y <= 10; y++)
        {
            for (double x = 1; x <= 10; x++) {
                // !!!!!!!!!!!! rename ColorValue, ColorValue is gona be used for rgb values later !!!!!!!!!!!!!!!!
                float ColorValue = SelectWaves(x, y, t, 1, 5, 10, 1, glm::vec3 (0.5, -0.5, 0)); 
                glm::vec3 normal = Wave::CreateNormal(ColorValue, x, y);
                if (maxValue < ColorValue) maxValue = ColorValue;
                if (minValue > ColorValue) minValue = ColorValue;
                ///map to rgb color (0-255)
                //value -> (-1, 1) + 1 -> (0 , 2) -> (0, 255)

                /* std::cout << ColorValue << "    "; */
            }
            
        }

    }
    std::cout << "max: " << maxValue << std::endl;
    std::cout << "min: " << minValue << std::endl;
}

// speed * 2/wavelength
double Wave::SelectWaves(float x, float y, float t, float k, float wavelength, float amplitude, float speed, glm::vec3 direction)
{

    float frequency = 2.0f / wavelength;
    float FIspeed   = speed * 2.0f / wavelength;
    double wave = amplitude * std::sin(glm::dot(direction, glm::vec3 (x, y, 0)) * frequency + t * FIspeed);
    std::cout << frequency << " " << FIspeed << " " << wave << "    ";
    return wave;
}



glm::vec3 Wave::CreateNormal(int x, int y)
{
    /* double wave = k * direction.x * x * frequency * amplitude */
    /*             * std::pow((sin(glm::dot(direction, glm::vec3(x, y, 0)) * frequency + t * FIspeed) +1), k-1) */
    /*             * cos(glm::dot(direction, glm::vec3(x, y, 0)) * frequency + t * FIspeed); */
    /* glm::vec3 tangent(0, 1, wave); */
    /* glm::vec3 binormal(1, 0, wave); */
    /* glm::vec3 normal = glm::cross(binormal, tangent); */
    return glm::vec3(0,0,0);
}

glm::vec3 CalculateCircularDirection(glm::vec3 direction, glm::vec3 center)
{
    return((direction - center) / glm::distance(direction, center));
}

float Frequency(float wavelength)
{
    return std::sqrt(Wave::GRAVITY * 2 * Wave::PI / wavelength);
}
