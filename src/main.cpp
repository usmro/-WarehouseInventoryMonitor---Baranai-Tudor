#include "Depozit.h"
#include "Tranzactie.h"

#include <exception>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>

void afiseazaProduse(const std::vector<Produs>& produse) {
    if (produse.empty()) {
        std::cout << "Nu exista produse de afisat.\n";
        return;
    }

    for (const Produs& produs : produse) {
        std::cout << "\n----------------------------------------\n"
                  << "[" << produs.getId() << "] " << produs.getNume() << '\n'
                  << "Categorie: " << produs.getCategorie() << '\n'
                  << "Pret: " << std::fixed << std::setprecision(2) << produs.getPret() << " lei\n"
                  << "Stoc: " << produs.getCantitate() << " bucati\n"
                  << "Prag alerta: " << produs.getPragAlerta() << '\n'
                  << "Status: " << produs.getStatusStoc() << '\n';
    }
    std::cout << "----------------------------------------\n";
}

void curataInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int citesteInt(const std::string& mesaj) {
    int valoare;
    std::cout << mesaj;
    while (!(std::cin >> valoare)) {
        std::cout << "Valoare invalida. Incearca din nou: ";
        curataInput();
    }
    return valoare;
}

double citesteDouble(const std::string& mesaj) {
    double valoare;
    std::cout << mesaj;
    while (!(std::cin >> valoare)) {
        std::cout << "Valoare invalida. Incearca din nou: ";
        curataInput();
    }
    return valoare;
}

std::string citesteText(const std::string& mesaj) {
    std::string text;
    std::cout << mesaj;
    std::cin >> std::ws;
    std::getline(std::cin, text);
    return text;
}

void afiseazaMeniu() {
    std::cout << "\n=== Warehouse Inventory Monitor ===\n"
              << "1. Adauga produs\n"
              << "2. Elimina produs\n"
              << "3. Restock produs\n"
              << "4. Vanzare produs\n"
              << "5. Afiseaza toate produsele\n"
              << "6. Raport produse sub prag\n"
              << "7. Sugestii reaprovizionare\n"
              << "8. Cauta produs dupa nume\n"
              << "9. Filtreaza dupa categorie\n"
              << "10. Sorteaza dupa pret crescator\n"
              << "11. Sorteaza dupa pret descrescator\n"
              << "0. Iesire\n"
              << "Alege optiunea: ";
}

int main() {
    Depozit depozit;
    int optiune;

    do {
        afiseazaMeniu();
        if (!(std::cin >> optiune)) {
            std::cout << "Optiune invalida.\n";
            curataInput();
            continue;
        }

        try {
            switch (optiune) {
            case 1: {
                int id = citesteInt("ID: ");
                std::string nume = citesteText("Nume: ");
                std::string categorie = citesteText("Categorie: ");
                int cantitate = citesteInt("Cantitate: ");
                double pret = citesteDouble("Pret: ");
                int prag = citesteInt("Prag alerta: ");
                depozit.adaugaProdus(Produs(id, nume, categorie, cantitate, pret, prag));
                std::cout << "Produs adaugat.\n";
                break;
            }
            case 2: {
                int id = citesteInt("ID produs: ");
                depozit.eliminaProdus(id);
                std::cout << "Produs eliminat.\n";
                break;
            }
            case 3: {
                int id = citesteInt("ID produs: ");
                int cantitate = citesteInt("Cantitate adaugata: ");
                Tranzactie<Intrare> tranzactie(id, cantitate, "restock");
                depozit.restockProdus(tranzactie.getProdusId(), tranzactie.getCantitate());
                std::cout << "Stoc actualizat prin tranzactie de tip " << tranzactie.getTip() << ".\n";
                break;
            }
            case 4: {
                int id = citesteInt("ID produs: ");
                int cantitate = citesteInt("Cantitate vanduta: ");
                Tranzactie<Iesire> tranzactie(id, cantitate, "vanzare");
                depozit.vindeProdus(tranzactie.getProdusId(), tranzactie.getCantitate());
                std::cout << "Stoc actualizat prin tranzactie de tip " << tranzactie.getTip() << ".\n";
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
                std::string text = citesteText("Text cautat in numele produsului: ");
                afiseazaProduse(depozit.cautaProduseDupaNume(text));
                break;
            }
            case 9: {
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
            case 0:
                std::cout << "Aplicatia se inchide.\n";
                break;
            default:
                std::cout << "Optiune inexistenta.\n";
                break;
            }
        } catch (const std::exception& eroare) {
            std::cout << "Eroare: " << eroare.what() << '\n';
        }
    } while (optiune != 0);

    return 0;
}
