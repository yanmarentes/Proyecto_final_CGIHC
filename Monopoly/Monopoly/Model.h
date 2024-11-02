#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "Mesh.h"
#include "Texture.h"
#include "CommonValues.h"

class Model
{
public:
	Model();

	void LoadModel(const std::string& fileName);
	void RenderModel();
	void ClearModel();

	~Model();

private:
	void LoadNode(aiNode* node, const aiScene* scene);
	void LoadMesh(aiMesh* mesh, const aiScene* scene);
	void LoadMaterials(const aiScene* scene);
	std::vector<Mesh*>MeshList;
	std::vector<Texture*>TextureList;
	std::vector<unsigned int>meshTotex;
};

/**
* Cree esta funcion para crear modelos que vayan a utilizar un movimiento cuadrado asi como el personaje principal y los modelos externos
* como el helicoptero y el barco
*/
class ModelSquareMovement : public Model {

public:
	void load_animation_parameters(float distance_corner, float altura, float initial_rotate, int initial_corner);
	void set_move(float mov);

	Ubicacion ubi_model;
	float current_rotate;
	float mov_model_side;
	float mov_model_since_tirada;
	float mov_model_total;
	int num_turn;
	int side;

private:
	float distance;
	float addition_rotate;
	float movement_limit;
};

#endif