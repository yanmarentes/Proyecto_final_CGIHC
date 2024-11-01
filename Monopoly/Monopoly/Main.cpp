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

//Camaras a usar
Camera camera;
Camera* currentCamera;
Camera camaraAvatar;
Camera camaraLibre;
Camera camaraAerea;
Camera camaraEstatica;

glm::vec3 camPos(0.0f);
glm::vec4 camoffset(0.0f, 15.0f, 40.0f, 1.0f);
glm::vec4 camoffset1(0.0f, 0.0f, 90.0f, 0.0f);
glm::mat4 camRot(1.0);

Texture piso_texture_dia;
Texture piso_texture_noche;
Texture dadoTexture;
Texture casilla1;
Texture casilla2;
Texture casilla3;
Texture casilla4;
Texture casilla5;
Texture casilla6;
Texture casilla7;
Texture casilla8;
Texture casilla9;
Texture casilla10;
Texture casilla11;
Texture casilla12;
Texture casilla13;
Texture casilla14;
Texture casilla15;
Texture casilla16;
Texture casilla17;
Texture casilla18;
Texture casilla19;
Texture casilla20;
Texture casilla21;
Texture casilla22;
Texture casilla23;
Texture casilla24;
Texture casilla25;
Texture casilla26;
Texture casilla27;
Texture casilla28;
Texture casilla29;
Texture casilla30;
Texture casilla31;
Texture casilla32;
Texture casilla33;
Texture casilla34;
Texture casilla35;
Texture casilla36;
Texture casilla37;
Texture casilla38;
Texture casilla39;


ModelSquareMovement main_character;
Model main_brazo_derecho;
Model main_brazo_izquierdo;
Model main_pierna_derecha;
Model main_pierna_izquierda;
ModelSquareMovement copter;
Model Helices;

//One piece
Model Luffy;
Model ace;
Model ark_maxim;
Model ship;
Model zoro;
Model nami;
Model moby_dick;
Model laboon;
Model brook;
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
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

//void IniciarCamaras();

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
	unsigned int dado_indices[] = {
		0, 1, 2,  // Tri�ngulo 1
		3, 4, 5,  // Tri�ngulo 2
		6, 7, 8,  // Tri�ngulo 3
		9, 10, 11, // Tri�ngulo 4
		12, 13, 14, // Tri�ngulo 5
		15, 16, 17, // Tri�ngulo 6
		18, 19, 20, // Tri�ngulo 7
		21, 22, 23, // Tri�ngulo 8
		24, 25, 26, // Tri�ngulo 9
		27, 28, 29, // Tri�ngulo 10
	};


	GLfloat dado_vertices[] = {
		//x		y		z		S		T			NX		NY		NZ
		0.0f,  1.0f,  0.0f,	    0.45f,  0.3f,		-1.0f,	-1.0f,	-1.0f, //0
		1.0f,  0.0f,  -0.24f,	0.62f,	0.19f,		-1.0f,	-1.0f,	-1.0f, //1 //cara 9
		0.65f, 0.0f,  0.97f,	0.44f,	0.13f,		-1.0f,	-1.0f,	-1.0f, //2

		0.0f,  1.0f,  0.0f,	    0.415f, 0.295f,		0.0f,	-1.0f,	-1.0f, //0
		0.65f, 0.0f,  0.97f,	0.415f,	0.13f,		0.0f,	-1.0f,	-1.0f, //2 // cara 3
	   -0.65f, 0.0f,  0.97f,	0.25f,	0.2f,		0.0f,	-1.0f,	-1.0f, //3

		0.0f,  1.0f,  0.0f,	    0.39f,  0.31f,		1.0f,	-1.0f,	-1.0f, //0
	   -0.65f, 0.0f,  0.97f,	0.201f,	0.22f,		1.0f,	-1.0f,	-1.0f, //3 //cara 1
	   -1.0f,  0.0f, -0.24f,	0.17f,	0.33f,		1.0f,	-1.0f,	-1.0f, //4

		0.0f,  1.0f,  0.0f,	    0.41f,  0.34f,		1.0f,	-1.0f,	1.0f, //0
	   -1.0f,  0.0f, -0.24f,	0.2f,	0.4f,		1.0f,	-1.0f,	1.0f, //4 //cara 7
		0.0f,  0.0f,  -1.0f,	0.3f,	0.48f,		1.0f,	-1.0f,	1.0f, //5

		0.0f,  1.0f,  0.0f,	    0.43f,  0.36f,		-1.0f,	-1.0f,	1.0f, //0
		0.0f,  0.0f,  -1.0f,	0.4f,	0.51f,		-1.0f,	-1.0f,	1.0f, //5 //cara 5
		1.0f,  0.0f,  -0.24f,	0.52f,	0.5f,		-1.0f,	-1.0f,	1.0f, //1

		0.0f, -1.0f,  0.0f,	    0.61f,  0.69f,		-1.0f,	1.0f,	-1.0f, //6
		1.0f,  0.0f,  -0.24f,	0.48f,	0.53f,		-1.0f,	1.0f,	-1.0f, //1 //cara 10
		0.65f, 0.0f,  0.97f,	0.68f,	0.52f,		-1.0f,	1.0f,	-1.0f, //2

		0.0f, -1.0f,  0.0f,	    0.59f,  0.69f,		0.0f,	1.0f,	-1.0f, //6
		0.65f, 0.0f,  0.97f,	0.45f,	0.56f,		0.0f,	1.0f,	-1.0f, //2 //cara 8
	   -0.65f, 0.0f,  0.97f,	0.36f,	0.67f,		0.0f,	1.0f,	-1.0f, // 3

	   0.0f, -1.0f,  0.0f,	    0.58f,  0.72f,		1.0f,	1.0f,	-1.0f, //6
	   -0.65f, 0.0f,  0.97f,	0.44f,	0.84f,		1.0f,	1.0f,	-1.0f, //3 //cara 2
	   -1.0f,  0.0f, -0.24f,	0.36f,	0.71f,		1.0f,	1.0f,	-1.0f, //4

	   0.0f, -1.0f,  0.0f,	    0.6f,  0.723,		1.0f,	1.0f,	1.0f, //6
	   -1.0f,  0.0f, -0.24f,	0.63f,	0.89f,		1.0f,	1.0f,	1.0f, //4 //cara 6
		0.0f,  0.0f,  -1.0f,	0.47f,	0.86f,		1.0f,	1.0f,	1.0f, //5

		0.0f, -1.0f,  0.0f,	    0.62f,  0.72f,		-1.0f,	1.0f,	1.0f, //6 //cara 4
		1.0f,  0.0f,  -0.24f,	0.69f,	0.89f,		-1.0f,	1.0f,	1.0f, //1
		0.0f,  0.0f,  -1.0f,	0.83f,	0.8f,		-1.0f,	1.0f,	1.0f, //5
	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(dado_vertices, dado_indices, sizeof(dado_vertices) / sizeof(dado_vertices[0]), sizeof(dado_indices) / sizeof(dado_indices[0]));

	meshList.push_back(dado);

}

void IniciarCamaras() {

	//camRot = glm::rotate(camRot, glm::radians(main_character.current_rotate), glm::vec3(0.0f, 1.0f, 0.0f));
	//camPos = glm::vec3(main_character.ubi_model.x, main_character.ubi_model.y, main_character.ubi_model.z) + glm::vec3(camRot * camoffset);
	camPos = glm::vec3(main_character.ubi_model.x, main_character.ubi_model.y, main_character.ubi_model.z);

	camaraAvatar = Camera(camPos, glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 1.0f, -1.0f); //
	camaraAerea = Camera(glm::vec3(0.0f, 150.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, -90.0f, 8.5f, 0.0f);
	camaraLibre = Camera(glm::vec3(0.0f, 200.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, -45.0f, 5.5f, 0.5f);
	camaraEstatica = Camera(glm::vec3(-150.0f, 150.0f, 130.0f), glm::vec3(0.0f, 1.0f, 0.0f), -45.0f, -45.0f, 8.5f, 0.0f);


	currentCamera = &camaraLibre;
}



void setCamera(GLint cameraNumber) {

	//camRot = glm::rotate(camRot, glm::radians(main_character.current_rotate), glm::vec3(0.0f, 1.0f, 0.0f));
	//camPos = glm::vec3(main_character.ubi_model.x, main_character.ubi_model.y, main_character.ubi_model.z) + glm::vec3(camRot * camoffset);
	//camRot = glm::rotate(camRot, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//camPos = glm::vec3(main_character.ubi_model.x, main_character.ubi_model.y, main_character.ubi_model.z - main_character.mov_model) + glm::vec3(camRot * camoffset1);
	camPos = glm::vec3(main_character.ubi_model.x, main_character.ubi_model.y, main_character.ubi_model.z - main_character.mov_model);

	switch (cameraNumber) {
	case 1:
		camaraAvatar.setPosicionX(camPos.x + camoffset.x);
		camaraAvatar.setPosicionY(camPos.y + camoffset.y);
		camaraAvatar.setPosicionZ(camPos.z + camoffset.z);
		currentCamera = &camaraAvatar;
		break;
	case 2:
		camaraAerea.keyControlAerea(mainWindow.getsKeys(), deltaTime);
		currentCamera = &camaraAerea;
		break;
	case 3:
		currentCamera = &camaraEstatica;
		break;
	case 4:
		camaraLibre.keyControl(mainWindow.getsKeys(), deltaTime);
		camaraLibre.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		currentCamera = &camaraLibre;
		break;
	}
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
		modelRotation += 45.0f * (deltaTime / 8);
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

		glm::mat4 modelCasilla(1.0);


		switch (current_casilla) {
		case 1:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x - 18.0f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - main_character.mov_model));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			ark_maxim.RenderModel();


			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x, main_character.ubi_model.y, main_character.ubi_model.z - main_character.mov_model));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(1.1f, 1.2f, 0.65f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla1.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 2:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x - 18.0f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - main_character.mov_model));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			hito.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x, main_character.ubi_model.y, main_character.ubi_model.z - main_character.mov_model));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(1.1f, 1.2f, 0.65f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla2.UseTexture();
			meshList[2]->RenderMesh();
			break;
		
		case 3:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x - 18.0f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - main_character.mov_model));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			merry.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x, main_character.ubi_model.y, main_character.ubi_model.z - main_character.mov_model));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(1.1f, 1.2f, 0.65f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla3.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 4:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x - 18.0f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - main_character.mov_model));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			laboon.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x, main_character.ubi_model.y, main_character.ubi_model.z - main_character.mov_model));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(1.1f, 1.2f, 0.65f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla4.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 5:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x - 18.0f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - main_character.mov_model));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			nami.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x, main_character.ubi_model.y, main_character.ubi_model.z - main_character.mov_model));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(1.1f, 1.2f, 0.65f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla5.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 6:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x - 18.0f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - main_character.mov_model));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			mera.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x, main_character.ubi_model.y, main_character.ubi_model.z - main_character.mov_model));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(1.1f, 1.2f, 0.65f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla6.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 7:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x - 18.0f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - main_character.mov_model));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Luffy.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x, main_character.ubi_model.y, main_character.ubi_model.z - main_character.mov_model));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(1.1f, 1.2f, 0.65f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla7.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 8:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x - 18.0f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - main_character.mov_model));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			ope.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x, main_character.ubi_model.y, main_character.ubi_model.z - main_character.mov_model));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(1.1f, 1.2f, 0.65f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla8.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 9:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x - 18.0f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - main_character.mov_model));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			ace.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x, main_character.ubi_model.y, main_character.ubi_model.z - main_character.mov_model));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(1.1f, 1.2f, 0.65f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla9.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 10:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - main_character.mov_model - 18.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			ship.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x, main_character.ubi_model.y, main_character.ubi_model.z - main_character.mov_model));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(1.1f, 1.2f, 1.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla10.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 11:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + main_character.mov_model, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 18.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			zoro.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x + main_character.mov_model, main_character.ubi_model.y, main_character.ubi_model.z));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(0.65f, 1.2f, 1.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla11.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 12:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + main_character.mov_model, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 18.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
			modelCasilla = glm::rotate(modelCasilla, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			gomu.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x + main_character.mov_model, main_character.ubi_model.y, main_character.ubi_model.z));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(0.65f, 1.2f, 1.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla12.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 13:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + main_character.mov_model, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 18.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			moby_dick.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x + main_character.mov_model, main_character.ubi_model.y, main_character.ubi_model.z));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(0.65f, 1.2f, 1.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla3.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 14:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + main_character.mov_model, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 18.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Luffy.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x + main_character.mov_model, main_character.ubi_model.y, main_character.ubi_model.z));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(0.65f, 1.2f, 1.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla14.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 15:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + main_character.mov_model, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 18.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Luffy.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x + main_character.mov_model, main_character.ubi_model.y, main_character.ubi_model.z));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(0.65f, 1.2f, 1.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla15.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 16:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + main_character.mov_model, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 18.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Luffy.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x + main_character.mov_model, main_character.ubi_model.y, main_character.ubi_model.z));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(0.65f, 1.2f, 1.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla16.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 17:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + main_character.mov_model, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 18.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Luffy.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x + main_character.mov_model, main_character.ubi_model.y, main_character.ubi_model.z));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(0.65f, 1.2f, 1.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla17.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 18:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + main_character.mov_model, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 18.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Luffy.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x + main_character.mov_model, main_character.ubi_model.y, main_character.ubi_model.z));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(0.65f, 1.2f, 1.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla18.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 19:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + main_character.mov_model, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - 18.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Luffy.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x + main_character.mov_model, main_character.ubi_model.y, main_character.ubi_model.z));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(0.65f, 1.2f, 1.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla19.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 20:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 18.0f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z - main_character.mov_model));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Luffy.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x + main_character.mov_model, main_character.ubi_model.y, main_character.ubi_model.z));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(1.1f, 1.2f, 1.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla20.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 21:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 18.0f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z + main_character.mov_model));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Luffy.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x, main_character.ubi_model.y, main_character.ubi_model.z + main_character.mov_model));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(1.1f, 1.2f, 0.65f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla21.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 22:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 18.0f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z + main_character.mov_model));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Luffy.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x, main_character.ubi_model.y, main_character.ubi_model.z + main_character.mov_model));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(1.1f, 1.2f, 0.65f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla22.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 23:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 18.0f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z + main_character.mov_model));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Luffy.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x, main_character.ubi_model.y, main_character.ubi_model.z + main_character.mov_model));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(1.1f, 1.2f, 0.65f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla23.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 24:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 18.0f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z + main_character.mov_model));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Luffy.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x, main_character.ubi_model.y, main_character.ubi_model.z + main_character.mov_model));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(1.1f, 1.2f, 0.65f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla24.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 25:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 18.0f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z + main_character.mov_model));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Luffy.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x, main_character.ubi_model.y, main_character.ubi_model.z + main_character.mov_model));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(1.1f, 1.2f, 0.65f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla25.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 26:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 18.0f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z + main_character.mov_model));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			shaymin.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x, main_character.ubi_model.y, main_character.ubi_model.z + main_character.mov_model));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(1.1f, 1.2f, 0.65f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla26.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 27:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 18.0f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z + main_character.mov_model));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			//model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			center.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x, main_character.ubi_model.y, main_character.ubi_model.z + main_character.mov_model));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(1.1f, 1.2f, 0.65f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla27.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 28:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 18.0f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z + main_character.mov_model));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			piplup.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x, main_character.ubi_model.y, main_character.ubi_model.z + main_character.mov_model));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(1.1f, 1.2f, 0.65f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla28.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 29:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x + 18.0f, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z + main_character.mov_model));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			//model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			joy.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x, main_character.ubi_model.y, main_character.ubi_model.z + main_character.mov_model));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(1.1f, 1.2f, 0.65f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla29.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 30:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x - main_character.mov_model, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z + 18.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			lucario.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x, main_character.ubi_model.y, main_character.ubi_model.z + main_character.mov_model));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(1.1f, 1.2f, 1.1f)); 
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla30.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 31:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x - main_character.mov_model, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z + 18.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			//model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			league.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x - main_character.mov_model, main_character.ubi_model.y, main_character.ubi_model.z));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(0.65f, 1.2f, 1.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla31.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 32:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x - main_character.mov_model, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z + 18.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Lectro.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x - main_character.mov_model, main_character.ubi_model.y, main_character.ubi_model.z));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(0.65f, 1.2f, 1.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla32.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 33:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x - main_character.mov_model, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z + 18.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			barry.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x - main_character.mov_model, main_character.ubi_model.y, main_character.ubi_model.z));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(0.65f, 1.2f, 1.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla33.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 34:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x - main_character.mov_model, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z + 18.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			garchomp.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x - main_character.mov_model, main_character.ubi_model.y, main_character.ubi_model.z));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(0.65f, 1.2f, 1.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla34.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 35:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x - main_character.mov_model, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z + 18.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			baya_latano.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x - main_character.mov_model, main_character.ubi_model.y, main_character.ubi_model.z));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(0.65f, 1.2f, 1.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla35.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 36:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x - main_character.mov_model, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z + 18.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			dawn.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x - main_character.mov_model, main_character.ubi_model.y, main_character.ubi_model.z));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(0.65f, 1.2f, 1.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla36.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 37:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x - main_character.mov_model, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z + 18.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			baya_frambu.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x - main_character.mov_model, main_character.ubi_model.y, main_character.ubi_model.z));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(0.65f, 1.2f, 1.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla37.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 38:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x - main_character.mov_model, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z + 18.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			casa_hoja_verde.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x - main_character.mov_model, main_character.ubi_model.y, main_character.ubi_model.z));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(0.65f, 1.2f, 1.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla38.UseTexture();
			meshList[2]->RenderMesh();
			break;
		case 39:
			model = glm::translate(model, glm::vec3(main_character.ubi_model.x - main_character.mov_model, main_character.ubi_model.y + modelPosY, main_character.ubi_model.z + 18.0f));
			model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			oran_berry.RenderModel();

			modelCasilla = glm::translate(modelCasilla, glm::vec3(0.0f, -0.2f, 0.0f));
			modelCasilla = glm::translate(modelCasilla, glm::vec3(main_character.ubi_model.x - main_character.mov_model, main_character.ubi_model.y, main_character.ubi_model.z));
			modelCasilla = glm::scale(modelCasilla, glm::vec3(0.65f, 1.2f, 1.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasilla));
			casilla39.UseTexture();
			meshList[2]->RenderMesh();
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
	IniciarCamaras();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	// +++++++++++++++++++++++++++++++++Texturas+++++++++++++++++++++++++++++++
	piso_texture_dia = Texture("Textures/monopoly.png");
	piso_texture_dia.LoadTextureA();
	piso_texture_noche = Texture("Textures/monopoly_noche.png");
	piso_texture_noche.LoadTextureA();
	dadoTexture = Texture("Textures/dado10caras.png");
	dadoTexture.LoadTextureA();

	
	//Casillas
	casilla1 = Texture("Textures/casilla1.png"); casilla1.LoadTextureA();
	casilla2 = Texture("Textures/casilla2.png"); casilla2.LoadTextureA();
	casilla3 = Texture("Textures/casilla3.png"); casilla3.LoadTextureA();
	casilla4 = Texture("Textures/casilla4.png"); casilla4.LoadTextureA();
	casilla5 = Texture("Textures/casilla5.png"); casilla5.LoadTextureA();
	casilla6 = Texture("Textures/casilla6.png"); casilla6.LoadTextureA();
	casilla7 = Texture("Textures/casilla7.png"); casilla7.LoadTextureA();
	casilla8 = Texture("Textures/casilla8.png"); casilla8.LoadTextureA();
	casilla9 = Texture("Textures/casilla9.png"); casilla9.LoadTextureA();
	casilla10 = Texture("Textures/casilla10.png"); casilla10.LoadTextureA();
	casilla11 = Texture("Textures/casilla11.png"); casilla11.LoadTextureA();
	casilla12 = Texture("Textures/casilla12.png"); casilla12.LoadTextureA();
	casilla13 = Texture("Textures/casilla13.png"); casilla13.LoadTextureA();
	casilla14 = Texture("Textures/casilla14.png"); casilla14.LoadTextureA();
	casilla15 = Texture("Textures/casilla15.png"); casilla15.LoadTextureA();
	casilla16 = Texture("Textures/casilla16.png"); casilla16.LoadTextureA();
	casilla17 = Texture("Textures/casilla17.png"); casilla17.LoadTextureA();
	casilla18 = Texture("Textures/casilla18.png"); casilla18.LoadTextureA();
	casilla19 = Texture("Textures/casilla19.png"); casilla19.LoadTextureA();
	casilla20 = Texture("Textures/casilla20.png"); casilla20.LoadTextureA();
	casilla21 = Texture("Textures/casilla21.png"); casilla21.LoadTextureA();
	casilla22 = Texture("Textures/casilla22.png"); casilla22.LoadTextureA();
	casilla23 = Texture("Textures/casilla23.png"); casilla23.LoadTextureA();
	casilla24 = Texture("Textures/casilla24.png"); casilla24.LoadTextureA();
	casilla25 = Texture("Textures/casilla25.png"); casilla25.LoadTextureA();
	casilla26 = Texture("Textures/casilla26.png"); casilla26.LoadTextureA();
	casilla27 = Texture("Textures/casilla27.png"); casilla27.LoadTextureA();
	casilla28 = Texture("Textures/casilla28.png"); casilla28.LoadTextureA();
	casilla29 = Texture("Textures/casilla29.png"); casilla29.LoadTextureA();
	casilla30 = Texture("Textures/casilla30.png"); casilla30.LoadTextureA();
	casilla31 = Texture("Textures/casilla31.png"); casilla31.LoadTextureA();
	casilla32 = Texture("Textures/casilla32.png"); casilla32.LoadTextureA();
	casilla33 = Texture("Textures/casilla33.png"); casilla33.LoadTextureA();
	casilla34 = Texture("Textures/casilla34.png"); casilla34.LoadTextureA();
	casilla35 = Texture("Textures/casilla35.png"); casilla35.LoadTextureA();
	casilla36 = Texture("Textures/casilla36.png"); casilla36.LoadTextureA();
	casilla37 = Texture("Textures/casilla37.png"); casilla37.LoadTextureA();
	casilla38 = Texture("Textures/casilla38.png"); casilla38.LoadTextureA();
	casilla39 = Texture("Textures/casilla39.png"); casilla39.LoadTextureA();
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
	brook.LoadModel("Models/brook.obj");
	mera.LoadModel("Models/Mera_mera.obj");
	gomu.LoadModel("Models/gomu_gomu_no_mi.obj");
	ope.LoadModel("Models/lawsfruit1.obj");
	sunny.LoadModel("Models/sunny.obj");
	laboon.LoadModel("Models/Laboon.obj");
	merry.LoadModel("Models/Going Merry.obj");
	hito.LoadModel("Models/hito_hito.obj");


	// +++++++++++++++++++++++++skybox+++++++++++++++++++++++++++++++++++++++++
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_rt.png");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_ft.png");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_dn.png");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_up.png");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_bk.png");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_lf.png");


	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
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
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
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
	
	//++++++++++++++Dado++++++++++++++++++++++++++++++++
	Package_Info_Dado info_dado;
	info_dado.movDado = 0.0f;
	info_dado.rotacion_dado = { 0.0f, 0.0f, 0.0f };
	info_dado.map_rotaciones = crear_rotaciones_dado();
	info_dado.altura_dado = 15.0f;

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

		setCamera(mainWindow.getTipoCamara());

		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox((*currentCamera).calculateViewMatrix(), projection);
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
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr((*currentCamera).calculateViewMatrix()));
		glUniform3f(uniformEyePosition, (*currentCamera).getCameraPosition().x, (*currentCamera).getCameraPosition().y, (*currentCamera).getCameraPosition().z);

		// luz ligada a la c�mara de tipo flash
		//sirve para que en tiempo de ejecuci�n (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//informaci�n al shader de fuentes de iluminaci�n
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
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


		manage_ejecutando_tirada(state_main_movement, &main_character, &info_main_character, modelstate);

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


		//Dado
		//Revisar si se presiono la tecla T para tirar el dado
		manage_get_tirada_dado(&mainWindow, state_main_movement, &info_dado);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, info_dado.altura_dado + info_dado.movDado, 0.0f));
		model = glm::rotate(model, glm::radians(info_dado.rotacion_dado.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(info_dado.rotacion_dado.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(info_dado.rotacion_dado.z), glm::vec3(0.0f, 0.0f, 1.0f));
		
		//Aqui se maneja la animacion del dado
		info_dado.pos_y = model[3][1];
		manage_tirando_dado(state_main_movement, &main_character, &info_dado, &info_main_character, movOffset * deltaTime);

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshList[4]->RenderMesh();

		//Revisar casilla y modelar personaje en turno
		render_current_model(state_main_movement, info_main_character.current_casilla, uniformModel, modelstate);

		//Fin
		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}