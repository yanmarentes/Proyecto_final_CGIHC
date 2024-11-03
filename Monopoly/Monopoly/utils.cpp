#include "utils.h"

/**
* Esta funcion es estatica y lo que hace es calcular del punto donde se encuenta el personaje cuanto le falta para llegar a la casilla destino
*
* Return float, distancia a donde tiene que llegar
*/
static float distancia_entre_casillas(int cantidad_dado, int position_initial, ModelSquareMovement* main_character, int& meta_casilla) {
	float real_distance = 0;
	int real_position = position_initial + cantidad_dado;

	float distance_casillas[] = { 0.0f, 17.0f, 30.0f, 42.4, 55.2, 67.9f, 79.9, 92.6, 105.4, 118.2, 135.0f, 152.0f, 164.22, 177.32, 189.6, 202.30, 215, 227.25, 240,
								251.88, 270.5, 286.8, 299.3, 312, 325.11, 337.35, 349, 362, 375, 385.64, 405.16, 421.75, 434.4, 447.12, 459.67, 472.22,
								485.06, 497.35, 509.9, 523.22 };


	if ((position_initial + cantidad_dado) >= 40) {
		real_position = (position_initial + cantidad_dado) - 40;
		main_character->num_turn++;
	}

	meta_casilla = real_position;

	real_distance = distance_casillas[real_position] + (MAIN_DISTANCE_CORNER * 8 * main_character->num_turn);
	printf("Casilla meta :%d\n", real_position);
	printf("Meta en distancia :%lf \n", distance_casillas[real_position]);
	printf("Lo que se le suma a meta distancia :%lf \n", (MAIN_DISTANCE_CORNER * 8 * main_character->num_turn));

	return real_distance;
}

/**
* Aqui se maneja la tecla T de que si esta en estado de reposo se empieza la secuencia de animacion y movimiento, en el caso que no este en reposo
* solamente resetea la variable para poder detectar cuando se vuelva a presionar la tecla T
*
* Return mapa
*/
void manage_get_tirada_dado(Window* mainWindow, int &state_main_movement, Package_Info_Dado* info_dado) {
	if (mainWindow->get_tirar_dado()) {
		if (state_main_movement == STATE_REPOSO) {
			state_main_movement = STATE_TIRANDO_DADO;
			mainWindow->reset_tirar_dado();
			info_dado->movDado = 0.0f;
			info_dado->mov_dado_side = 0.0f;
			info_dado->rotacion_dado_8 = { 0.0f, 0.0f, 0.0f };
			info_dado->rotacion_dado_4 = { 0.0f, 0.0f, 0.0f };
		}
		else {
			mainWindow->reset_tirar_dado();
		}
	}
}

/**
* Aqui maneja la logica de cuando se esta tirando el dado, revisa que llegue a un punto en cuestion "Y" de altura y luego se mueve a la derecha
* Cuando acaba la animacion pone el estado en sacando numero aleatorio
*
* Return void
*/
void manage_tirando_dado(int &state_main_movement, ModelSquareMovement* main_character, Package_Info_Dado* info_dado, Package_Info_Main_Character* info_main_character, float sum_mov_dado) {
	if (state_main_movement == STATE_TIRANDO_DADO) {

		if (info_dado->pos_y > 1.4f) {
			info_dado->movDado -= sum_mov_dado;
		}
		else {
			if (info_dado->pos_side < info_dado->side_limit)
				info_dado->mov_dado_side += sum_mov_dado;
			else
				state_main_movement = STATE_DADO_SACANDO_NUMERO_ALEATORIO;
		}
	} 
	else if (state_main_movement == STATE_DADO_SACANDO_NUMERO_ALEATORIO) {

		int sum_dados = 0;
		int num_aleat = (std::rand() % 8 + 1);
		sum_dados += num_aleat;
		info_dado->rotacion_dado_8 = info_dado->map_rotaciones_8[num_aleat];

		num_aleat = (std::rand() % 4 + 1);
		sum_dados += num_aleat;
		info_dado->rotacion_dado_4 = info_dado->map_rotaciones_4[num_aleat];

		info_main_character->real_distance = distancia_entre_casillas(sum_dados, info_main_character->current_casilla, main_character, info_main_character->meta_casilla);
		state_main_movement = STATE_EJECUTANDO_TIRADA_DADO;
	}
}

/**
* Esta funcion lo que hace es que si esta el juego en el estado de ejecutando tirada y ya llego a la casilla donde le tocaba se detiene y
* resetea todas las variables y pone en estado de resposo, en caso que aun no llegue sigue moviendo el personaje y le de movimiento a 
* piernas y brazos
*
* Return void
*/
void manage_ejecutando_tirada(int &state_main_movement, ModelSquareMovement* main_character, Package_Info_Main_Character* info_main_character, int &modelstate, GLfloat delta_time) {
	if (state_main_movement == STATE_EJECUTANDO_TIRADA_DADO) {
		if (main_character->mov_model_total >= info_main_character->real_distance) {
			main_character->mov_model_since_tirada = 0.0f;
			info_main_character->real_distance = 0;
			info_main_character->current_casilla = info_main_character->meta_casilla;
			state_main_movement = STATE_REPOSO;
			modelstate = 0;
		}
		
		main_character->set_move(0.1 * delta_time);


		if (info_main_character->fordward_extremidad) {
			info_main_character->mov_extremidades += (4.0 * delta_time);

			if (info_main_character->mov_extremidades >= 45.0f)
				info_main_character->fordward_extremidad = false;
		}
		else {
			info_main_character->mov_extremidades -= (4.0 * delta_time);

			if (info_main_character->mov_extremidades <= -45.0f)
				info_main_character->fordward_extremidad = true;
		}
	}
}

/**
* Funcion que regresa un mapa con las rotaciones necesarias para que cada cara quede hacia arriba
*
* Return mapa
*/
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

/**
* Funcion que regresa un mapa con las rotaciones necesarias para que cada cara quede hacia arriba
* 
* Return mapa
*/
std::map<int, glm::vec3> crear_rotaciones_dado_8_caras() {
	std::map<int, glm::vec3> mapa;

	mapa[1] = glm::vec3(36.0f, 5.0f, 132.0f);
	mapa[2] = glm::vec3(53.0f, -46.0f, -2.0f);
	mapa[3] = glm::vec3(80.0f, -127.0f, -148.0f);
	mapa[4] = glm::vec3(36.0f, 7.0f, 40.0f);
	mapa[5] = glm::vec3(-132.0f, 39.0f, 12.0f);
	mapa[6] = glm::vec3(-34.0f, 2.0f, 48.0f);
	mapa[7] = glm::vec3(57.0f, -46.0f, -86.0f);
	mapa[8] = glm::vec3(-48.0f, -42.0f, -82.0f);

	return mapa;
}

std::map<int, glm::vec3> crear_rotaciones_dado_4_caras() {
	std::map<int, glm::vec3> mapa;

	mapa[1] = glm::vec3(0.0f, 0.0f, 0.0f);
	mapa[2] = glm::vec3(-112.0f, 60.0f, 0.0f);
	mapa[3] = glm::vec3(-112.0f, 180.0f, 0.0f);
	mapa[4] = glm::vec3(-112.0f, -60.0f, -2.0f);

	return mapa;
}