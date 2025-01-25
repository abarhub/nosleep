#pragma once

#ifndef TEMPS_H
#define TEMPS_H


class Temps {
private:
    int heure;
    int minutes;
    int valeur;
public:
    Temps();
    int getHeure();
    int getMinute();
    int getValeur();
};

typedef class Temps Temps;


#endif

