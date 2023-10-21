#version 400 core

out vec4 FragColor;

in vec3 Position_FS_in;

void main() {
    float height = Position_FS_in.y / 6;
    FragColor = vec4(height, height, height, 1.0);
}
