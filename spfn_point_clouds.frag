#version 330 core

in vec3 outColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(outColor, 1.0);
    //FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}