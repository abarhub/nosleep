#pragma once

#ifndef CONFIG_H
#define CONFIG_H

#include<string>

class Config {
private:
	int heure;
	int minute;
	int minuteAffichage;

public:
	Config(int heure, int minute, int minuteAffichage) {
		this->heure = heure;
		this->minute = minute;
		this->minuteAffichage = minuteAffichage;
	}

	int getHeure() {
		return this->heure;
	}

	int getMinute() {
		return this->minute;
	}

	int getMinuteAffichage() {
		return this->minuteAffichage;
	}

};

Config* parseConfig(const std::string nomFichier);

#endif // !CONFIG_H


