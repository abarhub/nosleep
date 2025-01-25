
#include <iostream>

#include "easylogging++.h"
#include "config.h"
#include "Temps.h"
#include "traitement.h"

Traitement::Traitement(Config* config) {
    this->temps = nullptr;
    this->dernierAffichageTemps = nullptr;
    this->config = config;
    this->limiteHeure = this->config->getHeure();
    this->limiteMinutes = this->config->getMinute();
    this->minuteAffichage = this->config->getMinuteAffichage();
}

void Traitement::initialisation() {
    SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED);

    //long veille = 1000;
    //long nb = 0;
    //long nbMax = 70;
    //Temps* temps = NULL;
    //Temps* newTemps = NULL;
    //Temps* dernierAffichageTemps = NULL;
    //const int limiteHeure = this->config->getHeure(), limiteMinutes = this->config->getMinute(),
    //    minuteAffichage = this->config->getMinuteAffichage();

    LOG(INFO) << "limite heure: " << this->limiteHeure << "; minutes: " << this->limiteMinutes << "; minuteAffichage: " << this->minuteAffichage;

    this->temps = new Temps();
    LOG(INFO) << "hour: " << this->temps->getHeure() << "; minutes: " << this->temps->getMinute() << ";";
    this->dernierAffichageTemps = new Temps();

    //LOG(INFO) << "Début de mise en veille";
}

bool Traitement::traitement() {
    Temps* newTemps=nullptr;
    newTemps = new Temps();
    if (this->temps->getValeur() != newTemps->getValeur()) {

        delete this->temps;
        this->temps = newTemps;

        if (this->minuteAffichage == 0 || this->temps->getValeur() - this->dernierAffichageTemps->getValeur() > this->minuteAffichage) {
            LOG(INFO) << "hour: " << this->temps->getHeure() << "; minutes: " << this->temps->getMinute() << ";";
            this->dernierAffichageTemps = new Temps();
        }
    }

    if (this->temps->getHeure() > this->limiteHeure || (this->temps->getHeure() == this->limiteHeure && this->temps->getMinute() > this->limiteMinutes)) {
        return true;
    }
    return false;
}

void Traitement::fin() {
    //LOG(INFO) << "Fin de mise en veille";
    LOG(INFO) << "hour: " << this->temps->getHeure() << "; minutes: " << this->temps->getMinute() << ";";
}
