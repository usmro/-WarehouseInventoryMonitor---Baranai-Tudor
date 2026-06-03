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
const std::string FISIER_PRODUSE = "data/produse.csv";
const std::string FISIER_FURNIZORI = "data/furnizori.csv";
const std::string FISIER_ISTORIC = "data/istoric.csv";

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

void salveazaToate(Depozit& depozit) {
    depozit.salveazaProduseInFisier(FISIER_PRODUSE);
    depozit.salveazaFurnizoriInFisier(FISIER_FURNIZORI);
    depozit.salveazaIstoricInFisier(FISIER_ISTORIC);
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

void afiseazaFurnizor(const Furnizor& furnizor) {
    std::cout << "+----------------------------------------------------------------------+\n"
              << "| #" << std::left << std::setw(5) << furnizor.getId()
              << std::setw(62) << textInColoana(furnizor.getNume(), 62) << "|\n"
              << "+----------------------------------------------------------------------+\n"
              << "| Contact      : " << std::setw(53) << textInColoana(furnizor.getContact(), 53) << "|\n"
              << "| Produse      : " << std::right << std::setw(10) << furnizor.getProduseFurnizate().size()
              << std::left << std::setw(43) << "" << "|\n"
              << "+----------------------------------------------------------------------+\n";
}

void afiseazaMiscari(const std::vector<std::shared_ptr<const MiscareStoc>>& miscari) {
    if (miscari.empty()) {
        mesajInfo("Nu exista miscari de afisat.");
        return;
    }

    subtitlu("Istoric tranzactii");
    std::cout << std::left
              << std::setw(20) << "Timestamp"
              << std::setw(10) << "Tip"
              << std::setw(10) << "ProdusID"
              << std::setw(10) << "Cantitate"
              << "Observatii\n";
    linie('-');
    for (const auto& miscare : miscari) {
        std::cout << std::left
                  << std::setw(20) << miscare->getTimestamp()
                  << std::setw(10) << miscare->getTip()
                  << std::setw(10) << miscare->getProdusId()
                  << std::setw(10) << miscare->getCantitate()
                  << miscare->getObservatii() << '\n';
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
    // Consumam restul liniei pentru ca o citire de text ulterioara sa nu
    // primeasca newline-ul ramas in buffer.
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return valoare;
}

double citesteDouble(const std::string& mesaj) {
    double valoare;
    std::cout << "  " << mesaj;
    while (!(std::cin >> valoare)) {
        std::cout << "  Valoare invalida. Incearca din nou: ";
        curataInput();
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return valoare;
}

std::string citesteText(const std::string& mesaj) {
    std::string text;
    std::cout << "  " << mesaj;
    std::cin >> std::ws;
    std::getline(std::cin, text);
    return text;
}

std::string citesteTextOptional(const std::string& mesaj) {
    // Fara `>> std::ws`: altfel un Enter gol ar fi consumat ca whitespace si
    // campul optional nu ar putea fi lasat gol.
    std::string text;
    std::cout << "  " << mesaj;
    std::getline(std::cin, text);
    return text;
}

bool contineCaractereInterzise(const std::string& text, const std::string& interzise) {
    return text.find_first_of(interzise) != std::string::npos;
}

std::string citesteTextFiltrat(const std::string& mesaj, const std::string& interzise) {
    while (true) {
        std::string text = citesteText(mesaj);
        if (contineCaractereInterzise(text, interzise)) {
            std::cout << "  Caractere interzise (" << interzise << "). Incearca din nou.\n";
            continue;
        }
        return text;
    }
}

std::string citesteTextOptionalFiltrat(const std::string& mesaj, const std::string& interzise) {
    while (true) {
        std::string text = citesteTextOptional(mesaj);
        if (contineCaractereInterzise(text, interzise)) {
            std::cout << "  Caractere interzise (" << interzise << "). Incearca din nou.\n";
            continue;
        }
        return text;
    }
}

void afiseazaMeniu() {
    titlu("Warehouse Inventory Monitor");
    std::cout << "[Produse]\n"
              << "   1. Adauga produs              5. Afiseaza toate produsele\n"
              << "   2. Elimina produs             6. Raport produse sub prag\n"
              << "   3. Restock produs             7. Sugestii reaprovizionare\n"
              << "   4. Vanzare produs\n\n"
              << "[Catalog]\n"
              << "   8. Cauta produs dupa nume   11. Sorteaza pret descrescator\n"
              << "   9. Filtreaza dupa categorie 12. Salveaza tot\n"
              << "  10. Sorteaza pret crescator  13. Reincarca tot din fisiere\n\n"
              << "[Furnizori]\n"
              << "  14. Adauga furnizor          17. Produse pentru un furnizor\n"
              << "  15. Listeaza furnizori       18. Reaprovizionare pe furnizor\n"
              << "  16. Asociaza produs furnizor 19. Elimina furnizor\n\n"
              << "[Istoric tranzactii]\n"
              << "  20. Istoric complet          22. Ultimele N miscari\n"
              << "  21. Istoric pentru un produs 23. Sterge istoric\n\n"
              << "   0. Iesire\n";
    linie('=');
    std::cout << "Alege optiunea: ";
}

int main() {
    Depozit depozit;
    int optiune;

    try {
        if (std::filesystem::exists(FISIER_PRODUSE)) {
            depozit.incarcaProduseDinFisier(FISIER_PRODUSE);
            mesajInfo("Catalog incarcat din " + FISIER_PRODUSE + ".");
        } else {
            mesajInfo("Nu exista fisier de produse. Catalogul va fi creat la prima salvare.");
        }
    } catch (const std::exception& eroare) {
        mesajEroare("Catalogul nu a putut fi incarcat: " + std::string(eroare.what()));
    }

    try {
        if (std::filesystem::exists(FISIER_FURNIZORI)) {
            depozit.incarcaFurnizoriDinFisier(FISIER_FURNIZORI);
            mesajInfo("Furnizori incarcati din " + FISIER_FURNIZORI + ".");
        }
    } catch (const std::exception& eroare) {
        mesajEroare("Furnizorii nu au putut fi incarcati: " + std::string(eroare.what()));
    }

    try {
        if (std::filesystem::exists(FISIER_ISTORIC)) {
            depozit.incarcaIstoricDinFisier(FISIER_ISTORIC);
            mesajInfo("Istoric incarcat din " + FISIER_ISTORIC + ".");
        }
    } catch (const std::exception& eroare) {
        mesajEroare("Istoricul nu a putut fi incarcat: " + std::string(eroare.what()));
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
                std::string nume = citesteTextFiltrat("Nume: ", ";");
                std::string categorie = citesteTextFiltrat("Categorie: ", ";");
                int cantitate = citesteInt("Cantitate: ");
                double pret = citesteDouble("Pret: ");
                int prag = citesteInt("Prag alerta: ");

                if (tipProdus == 2) {
                    int garantie = citesteInt("Garantie luni: ");
                    depozit.adaugaProdus(std::make_shared<ProdusElectronic>(id, nume, categorie, cantitate, pret, prag, garantie));
                } else if (tipProdus == 3) {
                    std::string material = citesteTextFiltrat("Material: ", ";");
                    depozit.adaugaProdus(std::make_shared<ProdusMobilier>(id, nume, categorie, cantitate, pret, prag, material));
                } else if (tipProdus == 1) {
                    depozit.adaugaProdus(std::make_shared<Produs>(id, nume, categorie, cantitate, pret, prag));
                } else {
                    mesajEroare("Tip de produs invalid.");
                    break;
                }

                salveazaToate(depozit);
                mesajSucces("Produs adaugat.");
                break;
            }
            case 2: {
                subtitlu("Elimina produs");
                int id = citesteInt("ID produs: ");
                depozit.eliminaProdus(id);
                salveazaToate(depozit);
                mesajSucces("Produs eliminat (si dezasociat de furnizori).");
                break;
            }
            case 3: {
                subtitlu("Restock produs");
                int id = citesteInt("ID produs: ");
                int cantitate = citesteInt("Cantitate adaugata: ");
                std::string observatii = citesteTextOptional("Observatii (optional, Enter pentru gol): ");
                Tranzactie<Intrare> tranzactie(id, cantitate, observatii);
                depozit.restockProdus(tranzactie.getProdusId(), tranzactie.getCantitate(), tranzactie.getObservatii());
                salveazaToate(depozit);
                mesajSucces("Stoc actualizat prin tranzactie de tip " + tranzactie.getTip() + ".");
                break;
            }
            case 4: {
                subtitlu("Vanzare produs");
                int id = citesteInt("ID produs: ");
                int cantitate = citesteInt("Cantitate vanduta: ");
                std::string observatii = citesteTextOptional("Observatii (optional, Enter pentru gol): ");
                Tranzactie<Iesire> tranzactie(id, cantitate, observatii);
                depozit.vindeProdus(tranzactie.getProdusId(), tranzactie.getCantitate(), tranzactie.getObservatii());
                salveazaToate(depozit);
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
                salveazaToate(depozit);
                mesajSucces("Catalog, furnizori si istoric salvate.");
                break;
            case 13: {
                if (std::filesystem::exists(FISIER_PRODUSE)) {
                    depozit.incarcaProduseDinFisier(FISIER_PRODUSE);
                }
                if (std::filesystem::exists(FISIER_FURNIZORI)) {
                    depozit.incarcaFurnizoriDinFisier(FISIER_FURNIZORI);
                }
                if (std::filesystem::exists(FISIER_ISTORIC)) {
                    depozit.incarcaIstoricDinFisier(FISIER_ISTORIC);
                }
                mesajSucces("Date reincarcate din fisiere.");
                break;
            }
            case 14: {
                subtitlu("Adauga furnizor");
                int id = citesteInt("ID furnizor: ");
                std::string nume = citesteTextFiltrat("Nume: ", ";,");
                std::string contact = citesteTextOptionalFiltrat("Contact (email/telefon, optional): ", ";,");
                depozit.adaugaFurnizor(Furnizor(id, nume, contact));
                salveazaToate(depozit);
                mesajSucces("Furnizor adaugat.");
                break;
            }
            case 15: {
                auto furnizori = depozit.listaFurnizori();
                if (furnizori.empty()) {
                    mesajInfo("Nu exista furnizori inregistrati.");
                } else {
                    subtitlu("Furnizori");
                    for (const auto& furnizor : furnizori) {
                        afiseazaFurnizor(furnizor);
                    }
                }
                break;
            }
            case 16: {
                subtitlu("Asociaza produs cu furnizor");
                int furnizorId = citesteInt("ID furnizor: ");
                int produsId = citesteInt("ID produs: ");
                depozit.asociazaProdusCuFurnizor(furnizorId, produsId);
                salveazaToate(depozit);
                mesajSucces("Produs asociat cu furnizorul.");
                break;
            }
            case 17: {
                subtitlu("Produse pentru un furnizor");
                int furnizorId = citesteInt("ID furnizor: ");
                afiseazaFurnizor(depozit.cautaFurnizor(furnizorId));
                afiseazaProduse(depozit.produsePentruFurnizor(furnizorId));
                break;
            }
            case 18: {
                auto sugestii = depozit.sugestiiReaprovizionarePeFurnizor();
                if (sugestii.empty()) {
                    mesajInfo("Niciun furnizor nu are produse sub prag.");
                } else {
                    titlu("Reaprovizionare pe furnizor");
                    for (const auto& grup : sugestii) {
                        subtitlu("Furnizor #" + std::to_string(grup.first.getId()) + " - " + grup.first.getNume());
                        std::cout << "Contact: " << grup.first.getContact() << '\n';
                        afiseazaProduse(grup.second);
                    }
                }
                break;
            }
            case 19: {
                subtitlu("Elimina furnizor");
                int furnizorId = citesteInt("ID furnizor: ");
                depozit.eliminaFurnizor(furnizorId);
                salveazaToate(depozit);
                mesajSucces("Furnizor eliminat.");
                break;
            }
            case 20:
                afiseazaMiscari(depozit.getIstoric().toateMiscarile());
                break;
            case 21: {
                subtitlu("Istoric pentru un produs");
                int produsId = citesteInt("ID produs: ");
                afiseazaMiscari(depozit.getIstoric().miscariPentruProdus(produsId));
                break;
            }
            case 22: {
                subtitlu("Ultimele N miscari");
                int n = citesteInt("N: ");
                if (n < 0) {
                    mesajEroare("N nu poate fi negativ.");
                    break;
                }
                afiseazaMiscari(depozit.getIstoric().ultimele(static_cast<std::size_t>(n)));
                break;
            }
            case 23: {
                depozit.getIstoric().goleste();
                depozit.salveazaIstoricInFisier(FISIER_ISTORIC);
                mesajSucces("Istoricul tranzactiilor a fost sters.");
                break;
            }
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
