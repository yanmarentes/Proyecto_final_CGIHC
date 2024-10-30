#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform float time;

void main()
{   

    vec2 coordsT = TexCoords;
    // coordsT.x += 0.1f * time;
    // coordsT.y += 0.1f * time;

    vec4 texel = texture(texture_diffuse1, coordsT);

    FragColor = texel;
}