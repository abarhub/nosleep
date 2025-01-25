
#include <chrono>
#include <ctime>

#include "easylogging++.h"
#include "temps.h"


Temps::Temps() {
    struct tm newtime;
    __time64_t long_time;
    errno_t err;

    // Get time as 64-bit integer.
    _time64(&long_time);
    // Convert to local time.
    err = _localtime64_s(&newtime, &long_time);
    if (err)
    {
        LOG(ERROR) << "Invalid argument to _localtime64_s.";
        exit(1);
    }
    this->heure = newtime.tm_hour;
    this->minutes = newtime.tm_min;
    valeur = this->heure * 100 + this->minutes;
}

int Temps::getHeure() {
    return this->heure;
}

int Temps::getMinute() {
    return this->minutes;
}
int Temps::getValeur() {
    return this->valeur;
}
