#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include "CommonValues.h"
#include "Model.h"
#include "Window.h"
#include <map>

struct Package_Info_Dado {
	float movDado;
	float pos_y;
	glm::vec3 rotacion_dado;
	std::map<int, glm::vec3> map_rotaciones;
	float altura_dado;
};

struct Package_Info_Main_Character {
	int n_casillas;
	int current_casilla;
	float real_distance;
	int meta_casilla;
	float mov_extremidades;
	bool fordward_extremidad;
};

//float distancia_entre_casillas(int cantidad_dado, int& position_initial, ModelSquareMovement* main_character, int& meta_casilla);
void manage_get_tirada_dado(Window* mainWindow, int& state_main_movement, Package_Info_Dado* info_dado);
void manage_tirando_dado(int& state_main_movement, ModelSquareMovement* main_character, Package_Info_Dado* info_dado, Package_Info_Main_Character* info_main_character, float sum_mov_dado);
void manage_ejecutando_tirada(int& state_main_movement, ModelSquareMovement* main_character, Package_Info_Main_Character* info_main_character);
std::map<int, glm::vec3> crear_rotaciones_dado();

#endif