#include "Depozit.h"
#include "ProdusElectronic.h"
#include "ProdusMobilier.h"
#include "Tranzactie.h"

#include <exception>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

const int LATIME_UI = 72;
const std::string FISIER_DATE = "data/produse.csv";

void linie(char caracter = '=') {
    std::cout << std::string(LATIME_UI, caracter) << '\n';
}

void titlu(const std::string& text) {
    std::cout << '\n';
    linie('=');
    std::cout << "| " << std::left << std::setw(LATIME_UI - 4) << text << " |\n";
    linie('=');
}

void subtitlu(const std::string& text) {
    std::cout << '\n' << text << '\n';
    linie('-');
}

void mesajSucces(const std::string& text) {
    std::cout << "\n[OK] " << text << '\n';
}

void mesajInfo(const std::string& text) {
    std::cout << "\n[INFO] " << text << '\n';
}

void mesajEroare(const std::string& text) {
    std::cout << "\n[EROARE] " << text << '\n';
}

std::string textInColoana(const std::string& text, std::size_t latime) {
    if (text.size() <= latime) {
        return text;
    }
    if (latime <= 3) {
        return text.substr(0, latime);
    }
    return text.substr(0, latime - 3) + "...";
}

void salveazaCatalog(Depozit& depozit) {
    depozit.salveazaProduseInFisier(FISIER_DATE);
}

void afiseazaProduse(const std::vector<std::shared_ptr<const Produs>>& produse) {
    if (produse.empty()) {
        mesajInfo("Nu exista produse de afisat.");
        return;
    }

    subtitlu("Catalog produse");
    for (const auto& produs : produse) {
        std::cout << "+----------------------------------------------------------------------+\n"
                  << "| #" << std::left << std::setw(5) << produs->getId()
                  << std::setw(62) << textInColoana(produs->getNume(), 62) << "|\n"
                  << "+----------------------------------------------------------------------+\n"
                  << "| Tip          : " << std::setw(53) << textInColoana(produs->getTip(), 53) << "|\n"
                  << "| Categorie    : " << std::setw(53) << textInColoana(produs->getCategorie(), 53) << "|\n"
                  << "| Pret         : " << std::right << std::setw(10) << std::fixed << std::setprecision(2)
                  << produs->getPret() << " lei" << std::left << std::setw(39) << "" << "|\n"
                  << "| Stoc         : " << std::right << std::setw(10) << produs->getCantitate()
                  << " bucati" << std::left << std::setw(36) << "" << "|\n"
                  << "| Prag alerta  : " << std::right << std::setw(10) << produs->getPragAlerta()
                  << std::left << std::setw(43) << "" << "|\n"
                  << "| Status       : " << std::setw(53) << textInColoana(produs->getStatusStoc(), 53) << "|\n"
                  << "| Detalii      : " << std::setw(53) << textInColoana(produs->getDetaliiSpecifice(), 53) << "|\n"
                  << "+----------------------------------------------------------------------+\n";
    }
}

void curataInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int citesteInt(const std::string& mesaj) {
    int valoare;
    std::cout << "  " << mesaj;
    while (!(std::cin >> valoare)) {
        std::cout << "  Valoare invalida. Incearca din nou: ";
        curataInput();
    }
    return valoare;
}

double citesteDouble(const std::string& mesaj) {
    double valoare;
    std::cout << "  " << mesaj;
    while (!(std::cin >> valoare)) {
        std::cout << "  Valoare invalida. Incearca din nou: ";
        curataInput();
    }
    return valoare;
}

std::string citesteText(const std::string& mesaj) {
    std::string text;
    std::cout << "  " << mesaj;
    std::cin >> std::ws;
    std::getline(std::cin, text);
    return text;
}

void afiseazaMeniu() {
    titlu("Warehouse Inventory Monitor");
    std::cout << "|  1. Adauga produs              |  7. Sugestii reaprovizionare     |\n"
              << "|  2. Elimina produs             |  8. Cauta produs dupa nume       |\n"
              << "|  3. Restock produs             |  9. Filtreaza dupa categorie     |\n"
              << "|  4. Vanzare produs             | 10. Sorteaza pret crescator      |\n"
              << "|  5. Afiseaza toate produsele   | 11. Sorteaza pret descrescator   |\n"
              << "|  6. Raport produse sub prag    | 12. Salveaza catalog             |\n"
              << "| 13. Incarca catalog            |  0. Iesire                       |\n";
    linie('=');
    std::cout << "Alege optiunea: ";
}

int main() {
    Depozit depozit;
    int optiune;

    try {
        if (std::filesystem::exists(FISIER_DATE)) {
            depozit.incarcaProduseDinFisier(FISIER_DATE);
            mesajInfo("Catalog incarcat din " + FISIER_DATE + ".");
        } else {
            mesajInfo("Nu exista fisier de date. Catalogul va fi creat la prima salvare.");
        }
    } catch (const std::exception& eroare) {
        mesajEroare("Catalogul nu a putut fi incarcat: " + std::string(eroare.what()));
    }

    do {
        afiseazaMeniu();
        if (!(std::cin >> optiune)) {
            mesajEroare("Optiune invalida.");
            curataInput();
            continue;
        }

        try {
            switch (optiune) {
            case 1: {
                subtitlu("Adauga produs");
                std::cout << "  Tip produs: 1=Standard, 2=Electronic, 3=Mobilier\n";
                int tipProdus = citesteInt("Tip: ");
                int id = citesteInt("ID: ");
                std::string nume = citesteText("Nume: ");
                std::string categorie = citesteText("Categorie: ");
                int cantitate = citesteInt("Cantitate: ");
                double pret = citesteDouble("Pret: ");
                int prag = citesteInt("Prag alerta: ");

                if (tipProdus == 2) {
                    int garantie = citesteInt("Garantie luni: ");
                    depozit.adaugaProdus(std::make_shared<ProdusElectronic>(id, nume, categorie, cantitate, pret, prag, garantie));
                } else if (tipProdus == 3) {
                    std::string material = citesteText("Material: ");
                    depozit.adaugaProdus(std::make_shared<ProdusMobilier>(id, nume, categorie, cantitate, pret, prag, material));
                } else if (tipProdus == 1) {
                    depozit.adaugaProdus(std::make_shared<Produs>(id, nume, categorie, cantitate, pret, prag));
                } else {
                    mesajEroare("Tip de produs invalid.");
                    break;
                }

                salveazaCatalog(depozit);
                mesajSucces("Produs adaugat.");
                break;
            }
            case 2: {
                subtitlu("Elimina produs");
                int id = citesteInt("ID produs: ");
                depozit.eliminaProdus(id);
                salveazaCatalog(depozit);
                mesajSucces("Produs eliminat.");
                break;
            }
            case 3: {
                subtitlu("Restock produs");
                int id = citesteInt("ID produs: ");
                int cantitate = citesteInt("Cantitate adaugata: ");
                Tranzactie<Intrare> tranzactie(id, cantitate, "restock");
                depozit.restockProdus(tranzactie.getProdusId(), tranzactie.getCantitate());
                salveazaCatalog(depozit);
                mesajSucces("Stoc actualizat prin tranzactie de tip " + tranzactie.getTip() + ".");
                break;
            }
            case 4: {
                subtitlu("Vanzare produs");
                int id = citesteInt("ID produs: ");
                int cantitate = citesteInt("Cantitate vanduta: ");
                Tranzactie<Iesire> tranzactie(id, cantitate, "vanzare");
                depozit.vindeProdus(tranzactie.getProdusId(), tranzactie.getCantitate());
                salveazaCatalog(depozit);
                mesajSucces("Stoc actualizat prin tranzactie de tip " + tranzactie.getTip() + ".");
                break;
            }
            case 5:
                afiseazaProduse(depozit.listaProduse());
                break;
            case 6:
                afiseazaProduse(depozit.raportProduseSubPrag());
                break;
            case 7:
                afiseazaProduse(depozit.sugereazaReaprovizionare());
                break;
            case 8: {
                subtitlu("Cauta produs");
                std::string text = citesteText("Text cautat in numele produsului: ");
                afiseazaProduse(depozit.cautaProduseDupaNume(text));
                break;
            }
            case 9: {
                subtitlu("Filtreaza produse");
                std::string categorie = citesteText("Categorie: ");
                afiseazaProduse(depozit.filtreazaDupaCategorie(categorie));
                break;
            }
            case 10:
                afiseazaProduse(depozit.sorteazaDupaPret(true));
                break;
            case 11:
                afiseazaProduse(depozit.sorteazaDupaPret(false));
                break;
            case 12:
                salveazaCatalog(depozit);
                mesajSucces("Catalog salvat in " + FISIER_DATE + ".");
                break;
            case 13:
                depozit.incarcaProduseDinFisier(FISIER_DATE);
                mesajSucces("Catalog incarcat din " + FISIER_DATE + ".");
                break;
            case 0:
                mesajInfo("Aplicatia se inchide.");
                break;
            default:
                mesajEroare("Optiune inexistenta.");
                break;
            }
        } catch (const std::exception& eroare) {
            mesajEroare(eroare.what());
        }
    } while (optiune != 0);

    return 0;
}
