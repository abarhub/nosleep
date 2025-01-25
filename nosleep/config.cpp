#include <iostream>
#include <fstream>
#include <string>
#include "easylogging++.h"

#include "config.h"

Config* parseConfig(const std::string nomFichier) {

	std::ifstream fichier(nomFichier);

    if (fichier)
    {
        //L'ouverture s'est bien passée, on peut donc lire

        std::string ligne; //Une variable pour stocker les lignes lues

        int heure=0, minute = 0, minuteAffichage=0;
        const std::string heureTexte = "heure=", minuteTexte = "minute=", 
            minuteAffichageText ="minuteAffichage=";

        while (getline(fichier, ligne)) //Tant qu'on n'est pas à la fin, on lit
        {
            //cout << ligne << endl;
            //Et on l'affiche dans la console
            //Ou alors on fait quelque chose avec cette ligne
            //À vous de voir
            if (ligne.empty() || ligne.rfind("#",0) == 0) {
                // ne fait rien
            }
            else {
                if (ligne.rfind(heureTexte, 0) == 0) {
                    std::string s = ligne.substr(heureTexte.length());
                    int param1 = std::stoi(s);
                    if (param1 >= 0 && param1 < 24) {
                        heure = param1;
                    }
                } else if (ligne.rfind(minuteTexte, 0) == 0) {
                    std::string s = ligne.substr(minuteTexte.length());
                    int param1 = std::stoi(s);
                    if (param1 >= 0 && param1 < 60) {
                        minute = param1;
                    }
                } else if (ligne.rfind(minuteAffichageText, 0) == 0) {
                    std::string s = ligne.substr(minuteAffichageText.length());
                    int param1 = std::stoi(s);
                    if (param1 >= 0 && param1 < 60) {
                        minuteAffichage = param1;
                    }
                }
            }
        }
        Config* config = new Config(heure, minute, minuteAffichage);

        return config;
    }
    else
    {
        //cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << endl;
        LOG(ERROR) << "ERREUR: Impossible d'ouvrir le fichier '"<< nomFichier<<"' en lecture.";
        return NULL;
    }
}

