#ifndef WAVE_CLASS_H
#define WAVE_CLASS_H

#include <glm/ext/vector_float2.hpp>
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
    double SelectWaves(int x, int y, int t, float wavelength, float amplitude, float speed, glm::vec3 direction);

public:


    Wave();
    void test();
    void CreateWave(int image[500][500]);
};

#endif
