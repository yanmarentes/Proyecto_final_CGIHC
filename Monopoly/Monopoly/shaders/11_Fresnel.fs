#version 330 core

#extension GL_NV_shadow_samplers_cube : enable

in vec2 TexCoords;
in vec3 ex_N;

in vec3 Position_worldspace;
in vec3 EyeDirection_cameraspace;

out vec4 FragColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

// incoming surface normal and view-space position
in vec3 vs_fs_normal;
in vec3 vs_fs_position;

// incoming Fresnel reflection and refraction parameters
in vec3  vReflect;
in vec3  vRefract[3];
in float vReflectionFactor;

// the cubemap texture
uniform samplerCube cubetex;
uniform sampler2D texture_diffuse1;
uniform float transparency;

void main(void)
{

	// Fresnel
	vec4 reflectedColor = textureCube( cubetex, vec3( -vReflect.x, vReflect.yz ) );
	vec4 refractedColor = vec4( 0.0f );

	refractedColor.r = textureCube( cubetex, vec3( -vRefract[0].x, vRefract[0].yz ) ).r;
	refractedColor.g = textureCube( cubetex, vec3( -vRefract[1].x, vRefract[1].yz ) ).g;
	refractedColor.b = textureCube( cubetex, vec3( -vRefract[2].x, vRefract[2].yz ) ).b;

	vec4 fresnelColor = mix( refractedColor, reflectedColor, clamp( vReflectionFactor, 0.0, 1.0 ) );

	FragColor = fresnelColor;
	FragColor.a = transparency;
}
