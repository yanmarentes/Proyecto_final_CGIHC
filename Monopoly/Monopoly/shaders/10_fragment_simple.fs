#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 ex_N; 

uniform sampler2D texture_diffuse1;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{    
    vec4 texel = texture(texture_diffuse1, TexCoords);
    vec4 ambientColor = vec4(1.0,1.0,1.0,1.0);
    FragColor = ambientColor * texel;
}