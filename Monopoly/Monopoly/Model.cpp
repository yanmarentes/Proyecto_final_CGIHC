#include "Model.h"

Model::Model()
{
}

void Model::LoadModel(const std::string & fileName)
{
	Assimp::Importer importer;//					Pasa de Polygons y Quads a triangulos, modifica orden para el origen, generar normales si el  objeto no tiene, trata v�rtices iguales como 1 solo
	//const aiScene *scene=importer.ReadFile(fileName,aiProcess_Triangulate |aiProcess_FlipUVs|aiProcess_GenSmoothNormals|aiProcess_JoinIdenticalVertices);
	const aiScene *scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
	if (!scene)
	{	
		printf("Fall� en cargar el modelo: %s \n", fileName, importer.GetErrorString());
		return;
	}
	LoadNode(scene->mRootNode, scene);
	LoadMaterials(scene);
	}

void Model::ClearModel()
{
	for (unsigned int i = 0; i < MeshList.size(); i++)
	{
		if (MeshList[i])
		{
			delete MeshList[i];
			MeshList[i] = nullptr;

		}
	}

	for (unsigned int i = 0; i < TextureList.size(); i++)
	{
		if (TextureList[i])
		{
			delete TextureList[i];
			TextureList[i] = nullptr;
		}
	}

}

void Model::RenderModel()
{
	for (unsigned int i = 0; i < MeshList.size(); i++)
	{
		unsigned int materialIndex = meshTotex[i];
		if (!materialIndex< TextureList.size()&& TextureList[materialIndex])
		{
			TextureList[materialIndex]->UseTexture();
		}
		MeshList[i]->RenderMesh();

	}


}


Model::~Model()
{
}

void Model::LoadNode(aiNode * node, const aiScene * scene)
{
	for (unsigned int i = 0; i <node->mNumMeshes; i++)
	{
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene);
	}
}

void Model::LoadMesh(aiMesh * mesh, const aiScene * scene)
{

	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.insert(vertices.end(), { mesh->mVertices[i].x,mesh->mVertices[i].y ,mesh->mVertices[i].z });
		//UV
		if (mesh->mTextureCoords[0])//si tiene coordenadas de texturizado
		{
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y});
		}
		else
		{
			vertices.insert(vertices.end(), { 0.0f,0.0f });
		}
		//Normals importante, las normales son negativas porque la luz interact�a con ellas de esa forma, c�mo se vio con el dado/cubo
		vertices.insert(vertices.end(), { -mesh->mNormals[i].x,-mesh->mNormals[i].y ,-mesh->mNormals[i].z });
	}
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	Mesh* newMesh = new Mesh();
	newMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	MeshList.push_back(newMesh);
	meshTotex.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene * scene)
{
	TextureList.resize(scene->mNumMaterials);
	for (unsigned int i = 0; i < scene ->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];
		TextureList[i] = nullptr;
		if (material->GetTextureCount(aiTextureType_DIFFUSE	))
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE,0,&path)==AI_SUCCESS)
			{
				int idx = std::string(path.data).rfind("\\");//para quitar del path del modelo todo lo que este antes del \ de ubicaci�n de directorio
				std::string filename = std::string(path.data).substr(idx + 1);
				std::string tga ="tga";
				std::string png = "png";
				std::size_t existetga = filename.find(tga);
				std::size_t existepng= filename.find(png);
				std::string texPath = std::string("Textures/") + filename;
				TextureList[i] = new Texture(texPath.c_str());
				if (existetga != std::string::npos || existepng != std::string::npos)
				{
					if (!TextureList[i]->LoadTextureA())
					{
						printf("Fall� en cargar la Textura :%s\n", texPath);
						delete TextureList[i];
						TextureList[i] = nullptr;
					}
				}
				else
				{
					if (!TextureList[i]->LoadTexture())
					{
						printf("Fall� en cargar la Textura :%s\n", texPath);
						delete TextureList[i];
						TextureList[i] = nullptr;
					}
				}
			}
		}
		if (!TextureList[i])
		{
			TextureList[i] = new Texture("Textures/plain.png"); //textura que se aplicar� a los modelos si no tienen textura o la textura no se puede cargar
			TextureList[i]->LoadTextureA();
		}

	}
}


/**
* Esta es la funcion para darle los parametros del movimeindo al modelo
*/
void ModelSquareMovement::load_animation_parameters(float distance_corner, float altura = 0.0f, float initial_rotate = 0.0f, int initial_corner = 0) {
	addition_rotate = 90.0f;
	current_rotate = 0.0f + initial_rotate;

	distance = distance_corner;
	movement_limit = distance_corner * 2;
	mov_model_since_tirada = 0.0f;

	switch (initial_corner) {
		case 0:
			ubi_model.x = -distance;
			ubi_model.y = altura;
			ubi_model.z = distance;
			side = 0;
			break;
		case 1:
			ubi_model.x = -distance;
			ubi_model.y = altura;
			ubi_model.z = -distance;
			side = 1;
			break;
		case 2:
			ubi_model.x = distance;
			ubi_model.y = altura;
			ubi_model.z = -distance;
			side = 2;
			break;
		case 3:
			ubi_model.x = distance;
			ubi_model.y = altura;
			ubi_model.z = distance;
			side = 3;
			break;
		default:
			ubi_model.x = -distance;
			ubi_model.y = altura;
			ubi_model.z = distance;
			side = 0;
	}
	
	
	mov_model_side = 0.0f;
	mov_model_total = 0.0f;

	num_turn = 0;
}

/**
* Aqui voy moviendo al personaje y maneja las 4 esquinas, si llega a 1 da vuelta
* 
*/
void ModelSquareMovement::set_move(float mov) {

	if (mov_model_side >= movement_limit) {

		mov_model_side = 0;
		current_rotate = current_rotate - addition_rotate;

		switch (side) {
		case 0:
			ubi_model.z = -ubi_model.z;
			break;

		case 1:
			ubi_model.x = -ubi_model.x;
			break;

		case 2:
			ubi_model.z = -ubi_model.z;
			break;

		case 3:
			ubi_model.x = -ubi_model.x;
			break;
		}
		side++;

		if (side >= 4) {
			side = 0;
		}
	}

	mov_model_side += mov;
	mov_model_since_tirada += mov;
	mov_model_total += mov;
}
