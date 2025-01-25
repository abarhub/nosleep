#pragma once

#ifndef TRAITEMENT_H
#define TRAITEMENT_H

#include "config.h"
#include "Temps.h"

class Traitement {
private:
	Config* config;
	Temps* temps = nullptr;
	Temps* dernierAffichageTemps = nullptr;
	int limiteHeure = -1;
	int limiteMinutes = -1;
	int minuteAffichage = -1;
public:
	Traitement(Config* config);
	void initialisation();
	bool traitement();
	void fin();
};

#endif


