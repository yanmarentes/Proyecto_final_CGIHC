#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 ex_N; 

in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;
in vec3 Normal_cameraspace;

uniform sampler2D texture_diffuse1;

uniform vec4 LightColor;
uniform vec4 LightPower;
uniform int  alphaIndex;
uniform float distance;

uniform vec4 MaterialAmbientColor;
uniform vec4 MaterialDiffuseColor;
uniform vec4 MaterialSpecularColor;
uniform float transparency;

void main()
{    

    // Cálculo de componente difusa
    vec3 n = normalize( Normal_cameraspace );
    vec3 l = normalize( LightDirection_cameraspace );

    
    
    // Cálculo de componente ambiental
    vec4 K_a = MaterialAmbientColor * LightColor;

    // Cálculo de la componente difusa
    float cosTheta = clamp( dot( n,l ), 0,1 );
    vec4 K_d = MaterialDiffuseColor * LightColor * cosTheta;

    // Cálculo de componente especular
    vec3 E = normalize(EyeDirection_cameraspace);
    vec3 R = reflect(-l,n);
    float cosAlpha = clamp( dot(E,R), 0, 1 );
    vec4 K_s = MaterialSpecularColor * LightColor * pow(cosAlpha,alphaIndex);

    // Cálculo de ilumimación de Phong
    vec4 ex_color = K_a  * LightPower / (distance*distance) +
                    K_d  * LightPower / (distance*distance) +
                    K_s  * LightPower / (distance*distance);
                    
    ex_color.a = transparency;

    vec4 texel = texture(texture_diffuse1, TexCoords);

    FragColor = texel * ex_color;
}