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

#include <cmath>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <iostream>
#include <math.h>

using namespace noise;

class Wave
{

private:



    module::Perlin perlinModule;


public:

    int height;
    int width;
    static constexpr float GRAVITY = 9.8f;
    static constexpr float PI = 3.14159f;

    Wave();
    Wave(int inputHeight, int inputWidth);

    /* void GenWave(int (*waveTexture), float t); */
    void GenWave(int (*waveTexture), float t, float k, float wavelength, float amplitude, float speed, glm::vec3 direction);
    double SelectWaves(float x, float y, float t, float k, float wavelength, float amplitude, float speed, glm::vec3 direction);
    int MapToGrayscale(float amplitude, double wave);
};

#endif
