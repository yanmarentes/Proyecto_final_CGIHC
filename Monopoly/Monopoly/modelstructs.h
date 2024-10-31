#ifndef MODELSTRUCTS_H
#define MODELSTRUCTS_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <mesh.h>
#include <shader.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <stdlib.h>
using namespace std;

#include <glm/gtx/string_cast.hpp>

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

struct BoneInfo
{
	aiMatrix4x4 BoneOffset;
	aiMatrix4x4 FinalTransformation;

	BoneInfo()
	{
		aiMatrix4x4::Scaling(aiVector3D(0.0),BoneOffset);
		aiMatrix4x4::Scaling(aiVector3D(0.0), FinalTransformation);
	}
};

struct VertexBoneData
{
	unsigned int IDs[2*MAX_NUM_BONES];
	float Weights[2*MAX_NUM_BONES];
	int numBones;

	VertexBoneData()
	{
		Reset();
	};

	void Reset()
	{
		numBones = 0;
		for (int i = 0; i < 2 * MAX_NUM_BONES; i++) // ...initialize it
		{
			IDs[i] = 0;
			Weights[i] = 0.0f;
		}
	}

	void AddBoneData(unsigned int BoneID, float Weight) {
		if (numBones < 2 * MAX_NUM_BONES) {
			IDs[numBones] = BoneID;
			Weights[numBones] = Weight;
			numBones++;
		}
	}
};

struct Bone {
	aiString name;
	vector<unsigned int>    IDs;
	vector<float>           weights;
	glm::mat4               transformation;
	glm::mat4               offsetMatrix;

	Bone() {
		name = "";
		transformation = glm::mat4(1.0f);
		offsetMatrix   = glm::mat4(1.0f);
	};

	void push(unsigned int ID, float w) {
		IDs.push_back(ID);
		weights.push_back(w);
	}

};

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
#endif