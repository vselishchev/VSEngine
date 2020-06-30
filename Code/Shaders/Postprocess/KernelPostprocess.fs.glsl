#version 430 core
out vec4 FragColor;

in vec2  TexCoords;

uniform sampler2D screenTexture;
uniform float     kernel[9];

const float offset = 1.0f / 300.0f;

void main()
{
    const vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right
    );

    vec3 sampleTex;
    vec3 color = vec3(0.0);
    for(int i = 0; i < 9; ++i)
    {
        sampleTex = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
        color += sampleTex * kernel[i];
    }

    FragColor = vec4(color, 1.0);
}