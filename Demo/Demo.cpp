// Demo.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>

#include "../watorsim/watorsim.h"


struct Settings
{
    unsigned Width;
    unsigned Height;

    unsigned InitialFishCount;
    unsigned FishBreed;

    unsigned InitialSharkCount;
    unsigned SharkBreed;
    unsigned SharkStarve;

};


int main()
{
    std::cout << "Hello World!\n";
    
    auto rnd = std::shared_ptr<wator::IRandomNumberProvider>(wator::CreateRandomProvider("C"), [](wator::IRandomNumberProvider* p) { p->Delete(); });
    auto wator = std::shared_ptr<wator::ISimulation>(wator::CreateSimulation("sim1", *rnd), [](wator::ISimulation* p) { p->Delete(); });




    wator->InitDefault();
}

// Programm ausführen: STRG+F5 oder Menüeintrag "Debuggen" > "Starten ohne Debuggen starten"
// Programm debuggen: F5 oder "Debuggen" > Menü "Debuggen starten"

// Tipps für den Einstieg: 
//   1. Verwenden Sie das Projektmappen-Explorer-Fenster zum Hinzufügen/Verwalten von Dateien.
//   2. Verwenden Sie das Team Explorer-Fenster zum Herstellen einer Verbindung mit der Quellcodeverwaltung.
//   3. Verwenden Sie das Ausgabefenster, um die Buildausgabe und andere Nachrichten anzuzeigen.
//   4. Verwenden Sie das Fenster "Fehlerliste", um Fehler anzuzeigen.
//   5. Wechseln Sie zu "Projekt" > "Neues Element hinzufügen", um neue Codedateien zu erstellen, bzw. zu "Projekt" > "Vorhandenes Element hinzufügen", um dem Projekt vorhandene Codedateien hinzuzufügen.
//   6. Um dieses Projekt später erneut zu öffnen, wechseln Sie zu "Datei" > "Öffnen" > "Projekt", und wählen Sie die SLN-Datei aus.
