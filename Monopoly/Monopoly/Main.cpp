#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <chrono>
#include <map>
#include <cstdlib>  
#include <ctime>    

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include "Model.h"
#include "Skybox.h"
#include "CommonValues.h"
#include "utils.h"

//para iluminaci�n
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animaci�n
float movOffset;
float modelPosY = -15.0f; // Posición vertical del cartel
float modelRotation = 0.05f; // Rotación del cartel
float modelSpeed = 0.1f; // Velocidad del movimiento

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture piso_texture_dia;
Texture piso_texture_noche;
Texture dadoTexture_8;

ModelSquareMovement main_character;
Model main_brazo_derecho;
Model main_brazo_izquierdo;
Model main_pierna_derecha;
Model main_pierna_izquierda;
ModelSquareMovement copter;
Model Helices;
ModelSquareMovement Barco;
Model Rueda_barco;
Model Dado_4_caras;

//One piece
Model Luffy;
Model ace;
Model ark_maxim;
Model ship;
Model zoro;
Model nami;
Model moby_dick;
Model laboon;
Model hito;
Model gomu;
Model ope;
Model mera;
Model sunny;
Model merry;

//Pokemon
Model barry;
Model baya_frambu;
Model baya_latano;
Model casa_hoja_verde;
Model center;
Model dawn;
Model garchomp;
Model Lectro;
Model lucario;
Model joy;
Model oran_berry;
Model piplup;
Model league;
Model shaymin;
Model estadio;

Model casa_heeler;
Model botella_vidrio;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight main_light_noche;
DirectionalLight main_light_dia;

//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//funci�n de calculo de normales por promedio de v�rtices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}

void CrearDado()
{
	unsigned int dado8_id[] = {
		0, 1, 2,  // Tri�ngulo 1
		3, 4, 5,  // Tri�ngulo 2
		6, 7, 8,  // Tri�ngulo 3
		9, 10, 11, // Tri�ngulo 4
		12, 13, 14, // Tri�ngulo 5
		15, 16, 17, // Tri�ngulo 6
		18, 19, 20, // Tri�ngulo 7
		21, 22, 23, // Tri�ngulo 8
	};


	GLfloat dado8_vt[] = {
		// Cara superior 1
		//x		y		z		S		T			NX		NY		NZ
		0.0f, 1.0f, 0.0f, 0.5, 0.75, 1.0f, 1.0f, 1.0f, // 0
		0.0f, 0.0f, -1.0f, 0.75, 1.0, 1.0f, 1.0f, 1.0f, //1
		1.0f, 0.0f, 0.0f, 1.0f, 0.75, 1.0f, 1.0f, 1.0f, // 2

		// Cara superior 3
		0.0f, 1.0f, 0.0f, 0.5, 0.75, 1.0f, 1.0f, 1.0f, // 3
		1.0f, 0.0f, 0.0f, 1.0f, 0.75, 1.0f, 1.0f, 1.0f, // 4
		0.0f, 0.0f, 1.0f, 0.75, 0.50, 1.0f, 1.0f, 1.0f, // 5

		// Cara superior 5
		0.0f, 1.0f, 0.0f, 0.5f, 0.75, 1.0f, 1.0f, 1.0f, // 6
		0.0f, 0.0f, 1.0f, 0.75, 0.50, 1.0f, 1.0f, 1.0f, // 7
		-1.0f, 0.0f, 0.0f, 0.25, 0.5, 1.0f, 1.0f, 1.0f, // 8

		// Cara superior 7
		0.0f, 1.0f, 0.0f, 0.5f, 0.75, 1.0f, 1.0f, 1.0f, // 9 
		-1.0f, 0.0f, 0.0f, 0.25, 0.5, 1.0f, 1.0f, 1.0f, // 10
		0.0f, -0.0f, -1.0f, 0.0f, 0.75f, 1.0f, 1.0f, 1.0f, // 11,

		// Caras inferiores
		// Cara inferior 4
		0.0f, -1.0f, 0.0f, 0.5, 0.25, 1.0f, 1.0f, 1.0f, // 12
		0.0f, 0.0f, -1.0f, 0.75, 0.0, 1.0f, 1.0f, 1.0f, //13
		1.0f, 0.0f, 0.0f, 1.0f, 0.25, 1.0f, 1.0f, 1.0f, // 14

		// Cara superior 6
		0.0f, -1.0f, 0.0f, 0.5, 0.25, 1.0f, 1.0f, 1.0f, // 15
		1.0f, 0.0f, 0.0f, 1.0f, 0.25, 1.0f, 1.0f, 1.0f, // 16
		0.0f, 0.0f, 1.0f, 0.75, 0.50, 1.0f, 1.0f, 1.0f, // 17

		// Cara superior 8
		0.0f, -1.0f, 0.0f, 0.5f, 0.25, 1.0f, 1.0f, 1.0f, // 18
		0.0f, 0.0f, 1.0f, 0.75, 0.50, 1.0f, 1.0f, 1.0f, // 19
		-1.0f, 0.0f, 0.0f, 0.25, 0.5, 1.0f, 1.0f, 1.0f, // 20

		// Cara superior 2
		0.0f, -1.0f, 0.0f, 0.5f, 0.25, 1.0f, 1.0f, 1.0f, // 21
		-1.0f, 0.0f, 0.0f, 0.25, 0.5, 1.0f, 1.0f, 1.0f, // 22
		0.0f, -0.0f, -1.0f, 0.0f, 0.25f, 1.0f, 1.0f, 1.0f, // 23
	};

	// clang-format on
	Mesh* dado8 = new Mesh();
	dado8->CreateMesh(dado8_vt, dado8_id, 192, 36);
	meshList.push_back(dado8);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

/*
** Funcion donde se renderizaran todos los modelos dependiendo de cual sea la casilla current
**
**
*/
void render_current_model(int state_main_movement, int current_casilla, GLuint &uniformModel, int &modelstate) {
	
	//Movimiento del modelo en turno
	switch (modelstate) {
	case 0: //reset
		modelPosY = -10.0f;
		modelstate = 1;
		break;
	case 1: // Subiendo
		modelPosY += modelSpeed * deltaTime;
		if (modelPosY >= 3.0f) {
			modelPosY = 3.0f;
			modelstate = 2; // Cambia a girando hacia arriba
		}
		break;
	case 2: // Girando hacia arriba
		modelRotation += 45.0f * (deltaTime / 8); // 360 grados por segundo
		if (modelRotation >= 360.0f) {
			modelRotation = 0.0f;
			modelstate = 3; // Regresa a bajando
		}
		break;
	case 3: // Bajando
		modelPosY -= modelSpeed * deltaTime;
		if (modelPosY <= -10.0f) {
			modelPosY += 0.1f;
			modelstate = 4; // Cambia a girando hacia abajo
		}
		break;
	case 4: // Reposo
		modelPosY = -10.0f;
		break;
	}

	if (state_main_movement == STATE_REPOSO) {
		glm::mat4 model(1.0);

		switch (current_casilla) {
		case 1:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x - 18.0f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 16.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			ark_maxim.RenderModel();
			break;
		case 2:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x - 18.0f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 32.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			hito.RenderModel();
			break;
		
		case 3:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x - 18.0f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 45.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			merry.RenderModel();
			break;
		case 4:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x - 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 58.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			laboon.RenderModel();
			break;
		case 5:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x - 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 71.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			nami.RenderModel();
			break;
		case 6:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x - 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 82.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			mera.RenderModel();
			break;
		case 7:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x - 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 97.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Luffy.RenderModel();
			break;
		case 8:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x - 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 105.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			ope.RenderModel();
			break;
		case 9:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x - 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 118.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			ace.RenderModel();
			break;
		case 10:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 14.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			ship.RenderModel();
			break;
		case 11:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 32.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			zoro.RenderModel();
			break;
		case 12:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 45.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			gomu.RenderModel();
			break;
		case 13:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 58.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			moby_dick.RenderModel();
			break;
		case 14:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 71.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Luffy.RenderModel();
			break;
		case 15:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 82.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Luffy.RenderModel();
			break;
		case 16:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 97.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Luffy.RenderModel();
			break;
		case 17:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 105.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Luffy.RenderModel();
			break;
		case 18:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 118.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Luffy.RenderModel();
			break;
		case 19:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 14.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Luffy.RenderModel();
			break;
		case 20:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 14.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Luffy.RenderModel();
			break;
		case 21:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 14.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Luffy.RenderModel();
			break;
		case 22:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 14.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Luffy.RenderModel();
			break;
		case 23:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 14.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Luffy.RenderModel();
			break;
		case 24:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 14.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Luffy.RenderModel();
			break;
		case 25:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 14.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Luffy.RenderModel();
			break;
		case 26:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 14.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			shaymin.RenderModel();
			break;
		case 27:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 14.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			//model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			center.RenderModel();
			break;
		case 28:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 14.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			//model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));

			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			piplup.RenderModel();
			break;
		case 29:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 14.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			//model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			joy.RenderModel();
			break;
		case 30:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 14.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			lucario.RenderModel();
			break;
		case 31:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 14.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			//model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			league.RenderModel();
			break;
		case 32:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 14.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Lectro.RenderModel();
			break;
		case 33:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 14.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			barry.RenderModel();
			break;
		case 34:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 14.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			garchomp.RenderModel();
			break;
		case 35:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 14.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			baya_latano.RenderModel();
			break;
		case 36:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 14.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			dawn.RenderModel();
			break;
		case 37:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 14.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			baya_frambu.RenderModel();
			break;
		case 38:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 14.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			casa_hoja_verde.RenderModel();
			break;
		case 39:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 16.8f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 14.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			oran_berry.RenderModel();
			break;
		default:
			break;
		}
	}
}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	CrearDado();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	// +++++++++++++++++++++++++++++++++Texturas+++++++++++++++++++++++++++++++
	piso_texture_dia = Texture("Textures/monopoly.tga");
	piso_texture_dia.LoadTextureA();
	piso_texture_noche = Texture("Textures/monopoly_noche.jpg");
	piso_texture_noche.LoadTextureA();
	dadoTexture_8 = Texture("Textures/dado_8f.tga");
	dadoTexture_8.LoadTextureA();

	// +++++++++++++++++++++++++++++++ Modelos ++++++++++++++++++++++++++++++++
	main_character.LoadModel("Models/chopper_sin_extremidades.obj");
	main_character.load_animation_parameters(MAIN_DISTANCE_CORNER, -0.7f, 180.0f, 0);
	float mov_main_character = 0;

	main_brazo_derecho.LoadModel("Models/chopper_brazo_derecho.obj");
	main_brazo_izquierdo.LoadModel("Models/chopper_brazo_izquierdo.obj");
	main_pierna_derecha.LoadModel("Models/chopper_pata_derecha.obj");
	main_pierna_izquierda.LoadModel("Models/chopper_pata_izquierda.obj");

	copter.LoadModel("Models/cop.obj");
	copter.load_animation_parameters(VEHICLES_DISTANCE_CORNER, 0.0f, 90.0f, 2);
	Helices.LoadModel("Models/helices.obj");

	Barco.LoadModel("Models/barco.obj");
	Barco.load_animation_parameters(VEHICLES_DISTANCE_CORNER, 0.0f, 90.0f, 0);
	Rueda_barco.LoadModel("Models/rueda.obj");

	Dado_4_caras.LoadModel("Models/avr_dado_4.obj");

	Luffy.LoadModel("Models/Luffy.obj");
	ark_maxim.LoadModel("Models/ark_maxim.obj");
	baya_frambu.LoadModel("Models/baya_frambu.obj");
	baya_latano.LoadModel("Models/baya_latano.obj");
	barry.LoadModel("Models/barry.obj");
	casa_heeler.LoadModel("Models/casa_heeler.obj");
	casa_hoja_verde.LoadModel("Models/casa_hoja_verde.obj");
	center.LoadModel("Models/center.obj");
	dawn.LoadModel("Models/dawn.obj");
	estadio.LoadModel("Models/estadio.obj");
	garchomp.LoadModel("Models/garchomp.obj");
	Lectro.LoadModel("Models/Lectro.obj");
	lucario.LoadModel("Models/lucario.obj");
	moby_dick.LoadModel("Models/Moby Dick.obj");
	nami.LoadModel("Models/nami.obj");
	joy.LoadModel("Models/nurse_joy.obj");
	oran_berry.LoadModel("Models/oran berry.obj");
	piplup.LoadModel("Models/piplup.obj");
	league.LoadModel("Models/Pokemon League.obj");
	shaymin.LoadModel("Models/shaymin.obj");
	ship.LoadModel("Models/ship.obj");
	zoro.LoadModel("Models/zoro.obj");
	ace.LoadModel("Models/Ace.obj");
	mera.LoadModel("Models/Mera_mera.obj");
	gomu.LoadModel("Models/gomu_gomu_no_mi.obj");
	ope.LoadModel("Models/lawsfruit1.obj");
	sunny.LoadModel("Models/sunny.obj");
	laboon.LoadModel("Models/Laboon.obj");
	merry.LoadModel("Models/Going Merry.obj");
	hito.LoadModel("Models/hito_hito.obj");


	// +++++++++++++++++++++++++skybox+++++++++++++++++++++++++++++++++++++++++
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, solo 1 y siempre debe de existir
	main_light_noche = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	main_light_dia = DirectionalLight(1.0f, 0.9f, 0.9f,
		0.5f, 0.5f,
		0.0f, -1.0f, -0.5f);

	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaraci�n de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	/*spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;*/

	//luz fija
	spotLights[0] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		0.0f, 22.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	movOffset = 0.3f;

	//++++++++++++++++Info Main Character+++++++++++++++	

	Package_Info_Main_Character info_main_character;
	info_main_character.n_casillas = 0;
	info_main_character.current_casilla = 0;
	info_main_character.real_distance = 0;
	info_main_character.meta_casilla = 0;
	info_main_character.mov_extremidades = 0.0f;
	info_main_character.fordward_extremidad = true;

	//Estado
	int state_main_movement = STATE_REPOSO;
	int modelstate = 4;
	
	//++++++++++++++Dado 8 caras++++++++++++++++++++++++++++++++
	Package_Info_Dado info_dados;
	info_dados.movDado = 0.0f;
	info_dados.mov_dado_side = 0.0f;
	info_dados.rotacion_dado_8 = { 0.0f, 0.0f, 0.0f };
	info_dados.rotacion_dado_4 = { 0.0f, 0.0f, 0.0f };
	info_dados.map_rotaciones_8 = crear_rotaciones_dado_8_caras();
	info_dados.map_rotaciones_4 = crear_rotaciones_dado_4_caras();
	info_dados.altura_dado = 35.0f;
	info_dados.side_limit = 32.0f;

	std::srand(static_cast<unsigned int>(std::time(0)));

	glfwSetTime(0);

	//++++++++++++++++++Dia y Noche+++++++++++++++++++++
	GLfloat change_ambientacion = glfwGetTime() + SEGUNDOS_PARA_CAMBIAR_DIA_NOCHE;
	bool dia = true;

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		if (now >= change_ambientacion) {
			dia = !dia;
			change_ambientacion = now + SEGUNDOS_PARA_CAMBIAR_DIA_NOCHE;
		}

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//informaci�n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la c�mara de tipo flash
		//sirve para que en tiempo de ejecuci�n (dentro del while) se cambien propiedades de la luz
		//glm::vec3 lowerLight = camera.getCameraPosition();
		//lowerLight.y -= 0.3f;
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection()); linterna

		//informacion al shader de fuentes de iluminaci�n
		if (dia)
			shaderList[0].SetDirectionalLight(&main_light_dia);
		else
			shaderList[0].SetDirectionalLight(&main_light_noche);

		//shaderList[0].SetPointLights(pointLights, pointLightCount); linterna
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		//INICIO DE CREACION DE MODELOS
		
		//Piso
		glm::mat4 model(1.0);
		glm::mat4 model_aux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(8.0f, 1.0f, 8.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		if (dia)
			piso_texture_dia.UseTexture();
		else
			piso_texture_noche.UseTexture();

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();


		//Helicoptero Rodrigo
		copter.set_move(movOffset * deltaTime);
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(copter.ubi_model.x, copter.ubi_model.y, copter.ubi_model.z));
		model = glm::rotate(model, glm::radians(copter.current_rotate), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-copter.mov_model, 0.0f, 0.0f));
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		copter.RenderModel();

		float heli = copter.mov_model * 10;
		model = glm::translate(model, glm::vec3(1.0f, 2.6f, 0.0f));
		model = glm::rotate(model, glm::radians(heli), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helices.RenderModel();

		//Barco Emir
		Barco.set_move(movOffset * deltaTime);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(Barco.ubi_model.x, Barco.ubi_model.y, Barco.ubi_model.z));
		model = glm::rotate(model, glm::radians(Barco.current_rotate), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(Barco.mov_model, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Barco.RenderModel();

		model_aux = model;

		float mov_rueda = Barco.mov_model * -10;
		model = glm::translate(model, glm::vec3(-19.0f, 5.0f, 12.0f));
		model = glm::rotate(model, glm::radians(mov_rueda), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rueda_barco.RenderModel();

		model = model_aux;

		model = glm::translate(model, glm::vec3(-19.0f, 5.0f, -12.0f));
		model = glm::rotate(model, glm::radians(mov_rueda), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rueda_barco.RenderModel();


		manage_ejecutando_tirada(state_main_movement, &main_character, &info_main_character, modelstate, deltaTime);

		//Main model Tony Tony Chopper

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(main_character.ubi_model.x, main_character.ubi_model.y, main_character.ubi_model.z));
		model = glm::rotate(model, glm::radians(main_character.current_rotate), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, main_character.mov_model));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		main_character.RenderModel();

		model_aux = model;
		model = glm::translate(model, glm::vec3(-0.6f, 1.3f, 0.0f));
		model = glm::rotate(model, glm::radians(info_main_character.mov_extremidades), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		main_pierna_derecha.RenderModel();

		model = model_aux;
		model = glm::translate(model, glm::vec3(0.6f, 1.3f, 0.0f));
		model = glm::rotate(model, glm::radians(-info_main_character.mov_extremidades), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		main_pierna_izquierda.RenderModel();

		model = model_aux;
		model = glm::translate(model, glm::vec3(-0.9f, 4.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-info_main_character.mov_extremidades), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		main_brazo_derecho.RenderModel();

		model = model_aux;
		model = glm::translate(model, glm::vec3(0.9f, 4.0f, 0.0f));
		model = glm::rotate(model, glm::radians(info_main_character.mov_extremidades), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		main_brazo_izquierdo.RenderModel();


		//Dados
		//Revisar si se presiono la tecla T para tirar el dado
		manage_get_tirada_dado(&mainWindow, state_main_movement, &info_dados);
		// 8 caras
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, info_dados.altura_dado + info_dados.movDado, 4.0f + info_dados.mov_dado_side));
		if (state_main_movement == STATE_TIRANDO_DADO)
			model = glm::rotate(model, glm::radians((info_dados.movDado + info_dados.mov_dado_side) * 20.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(info_dados.rotacion_dado_8.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(info_dados.rotacion_dado_8.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(info_dados.rotacion_dado_8.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		
		//Aqui se maneja la animacion del dado
		info_dados.pos_y = model[3][1];
		info_dados.pos_side = model[3][2];
		manage_tirando_dado(state_main_movement, &main_character, &info_dados, &info_main_character, movOffset * deltaTime);

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture_8.UseTexture();
		meshList[4]->RenderMesh();

		// 4 caras
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f + info_dados.mov_dado_side, info_dados.altura_dado + info_dados.movDado, 0.0f));
		if (state_main_movement == STATE_TIRANDO_DADO)
			model = glm::rotate(model, glm::radians((info_dados.movDado + info_dados.mov_dado_side) * 20.0f), glm::vec3(-1.0f, -1.0f, -1.0f));
		model = glm::rotate(model, glm::radians(info_dados.rotacion_dado_4.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(info_dados.rotacion_dado_4.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(info_dados.rotacion_dado_4.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dado_4_caras.RenderModel();


		//Revisar casilla y modelar personaje en turno
		render_current_model(state_main_movement, info_main_character.current_casilla, uniformModel, modelstate);

		//Fin
		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}