#ifndef COMMONVALS
#define COMMONVALS
#include "stb_image.h"

const int MAX_POINT_LIGHTS = 2;
const int MAX_SPOT_LIGHTS = 5;

#define MAIN_DISTANCE_CORNER 67.5
#define VEHICLES_DISTANCE_CORNER 100
#define SEGUNDOS_PARA_CAMBIAR_DIA_NOCHE 30

struct Ubicacion {
	float x;
	float y;
	float z;
};

enum State_M_Mov {
	STATE_REPOSO,
	STATE_TIRAR_DADO,
	STATE_TIRANDO_DADO,
	STATE_EJECUTAR_TIRADA_DADO,
	STATE_EJECUTANDO_TIRADA_DADO
};

#endif