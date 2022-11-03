#ifndef WAVE_CLASS_H
#define WAVE_CLASS_H

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtx/string_cast.hpp>
#include <noise/noise.h>
#include <noise/module/perlin.h>
#include <iostream>
#include <cmath>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>


using namespace noise;

class Wave
{

private:



    module::Perlin perlinModule;
    double SelectWaves(float x, float y, float t, float k, float wavelength, float amplitude, float speed, glm::vec3 direction);
    glm::vec3 CreateTangent(float selectedWave, int x, int y);
    glm::vec3 CreateNormal(float selectedWave, int x, int y);
    glm::vec3 CalculateCircularDirection(glm::vec3 direction, glm::vec3 center);
    float Frequency(float wavelength);




public:

    static constexpr float GRAVITY = 9.8f;
    static constexpr float PI = 3.14159f;

    Wave();
    void test();
    void CreateWave(int image[500][500]);
};

#endif
