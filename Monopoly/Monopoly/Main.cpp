/*
Semestre 2024-1
Animaci�n:
Sesi�n 1:
Simple o b�sica:Por banderas y condicionales (m�s de 1 transforomaci�n geom�trica se ve modificada
Sesi�n 2
Compleja: Por medio de funciones y algoritmos.
Adicional.- ,Textura Animada
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
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
#include"Model.h"
#include "Skybox.h"

//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animaci�n
float movCoche;
float movOffset;

float movHelicoptero;
float movDado;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture dadoTexture;

Model Kitt_M;
Model Llanta_M;
Model Copter;
Model Helices;

struct Ubicacion {
	float x;
	float y;
	float z;
};


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
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f
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


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


std::map<int, glm::vec3> crear_rotaciones_dado() {
	std::map<int, glm::vec3> mapa;

	mapa[1] = glm::vec3(-14.0f, -8.0f, -49.0f);
	mapa[2] = glm::vec3(132.0f, -107.0f, 1.0f);
	mapa[3] = glm::vec3(45.0f, -182.0f, -3.0f);
	mapa[4] = glm::vec3(-130.0f, -145.0f, -3.0f);
	mapa[5] = glm::vec3(-52.0f, -141.0f, -3.0f);
	mapa[6] = glm::vec3(124.0f, -40.0f, -9.0f);
	mapa[7] = glm::vec3(47.0f, -30.0f, -9.0f);
	mapa[8] = glm::vec3(134.0f, -180.0f, 1.0f);
	mapa[9] = glm::vec3(138.0f, -256.0f, -89.0f);
	mapa[10] = glm::vec3(134.0f, -256.0f, -1.0f);

	return mapa;
}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	CrearDado();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/monopoly.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	dadoTexture = Texture("Textures/dado10caras.png");
	dadoTexture.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Copter = Model();
	Copter.LoadModel("Models/cop.obj");
	Helices = Model();
	Helices.LoadModel("Models/helices.obj");


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

	movHelicoptero = 0.0f;
	movDado = 0.0f;

	float esp_rota = 90.0f;
	float hel_rota = 0.0f;

	float distancia_heli = 100.0f;
	float limit_c = 200.0f; //ToDo: este es el doble que distancia pero hay que ver porque y poner que no sea hardcode, si no calculado

	int n_corner = 0;
	Ubicacion ubi_helicoptero;
	ubi_helicoptero.x = distancia_heli;
	ubi_helicoptero.y = 5.0f;
	ubi_helicoptero.z = distancia_heli;


	bool tirando_dado = false;
	float altura_dado = 15.0f;

	std::map<int, glm::vec3> rot_dado;
	rot_dado = crear_rotaciones_dado();
	glm::vec3 rotacion_dado = {0.0f, 0.0f, 0.0f};

	std::srand(static_cast<unsigned int>(std::time(0)));

	glfwSetTime(0);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

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
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//informaci�n al shader de fuentes de iluminaci�n
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(8.0f, 1.0f, 8.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		movHelicoptero += movOffset * deltaTime;
		
		//Helicoptero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(ubi_helicoptero.x, ubi_helicoptero.y, ubi_helicoptero.z));
		model = glm::rotate(model, glm::radians(hel_rota), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-movHelicoptero, 0.0f, 0.0f));

		glm::vec3 position = glm::vec3(model[3]);
		float posX = position.x;
		
		if (movHelicoptero >= limit_c) {

			movHelicoptero = 0;
			hel_rota = hel_rota - esp_rota;

			switch (n_corner) {
				case 0:
					ubi_helicoptero.x = -ubi_helicoptero.x;
					break;

				case 1:
					ubi_helicoptero.z = -ubi_helicoptero.z;
					break;

				case 2:
					ubi_helicoptero.x = -ubi_helicoptero.x;
					break;

				case 3:
					ubi_helicoptero.z = -ubi_helicoptero.z;
					break;
			}
			n_corner++;

			if (n_corner >= 4)
				n_corner = 0;
		}

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Copter.RenderModel();

		modelaux = model;
		float heli = movHelicoptero * 10;
		model = glm::translate(model, glm::vec3(1.0f, 2.6f, 0.0f));
		model = glm::rotate(model, glm::radians(heli), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helices.RenderModel();

		//movimiento dado
		
		if (mainWindow.get_tirar_dado()) {
			if (!tirando_dado) {
				tirando_dado = true;
				mainWindow.reset_tirar_dado();
				movDado = 0.0f;
				rotacion_dado = { 0.0f, 0.0f, 0.0f };
			}
			else {
				mainWindow.reset_tirar_dado();
			}

		}

		//Dado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, altura_dado + movDado, 0.0f));
		model = glm::rotate(model, glm::radians(rotacion_dado.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotacion_dado.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotacion_dado.z), glm::vec3(0.0f, 0.0f, 1.0f));
		
		float positionY = model[3][1];

		if (tirando_dado) {
			movDado -= movOffset * deltaTime;

			if (positionY <= 0.2f) {
				tirando_dado = false;
				int num_aleat = (std::rand() % 10 + 1);

				rotacion_dado = rot_dado[num_aleat];
			}
		}

		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshList[4]->RenderMesh();

		//Agave �qu� sucede si lo renderizan antes del coche y el helic�ptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, 1.0f, -14.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

