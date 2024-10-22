#version 330 core

in vec2 texCoords;

out vec4 color;

uniform sampler2D sprite;
uniform vec3 spriteColor;

void main()
{
    vec4 texColor = texture(sprite, texCoords);

    float tolerance = 0.01;

    // custom outfit
    if (distance(texColor.xyz, vec3(0.055, 0.29, 0.125)) < tolerance) // uniforma
    {
        color = vec4(spriteColor, 1.0);
    }
    else if (distance(texColor.xyz, vec3(0.039, 0.2, 0.086)) < tolerance || distance(texColor.xyz, vec3(0.016, 0.09, 0.039)) < tolerance) // butoni
    {
        float factor = 0.4;
        color = vec4(spriteColor * factor, 1.0);
    }
    else
    {
        color = texColor;
    }
}

