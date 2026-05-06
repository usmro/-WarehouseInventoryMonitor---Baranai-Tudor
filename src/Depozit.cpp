#include "Depozit.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>

std::string textMic(const std::string& text) {
    std::string rezultat = text;
    std::transform(rezultat.begin(), rezultat.end(), rezultat.begin(), [](unsigned char caracter) {
        return static_cast<char>(std::tolower(caracter));
    });
    return rezultat;
}

void valideazaTextPentruFisier(const std::string& text) {
    if (text.find(';') != std::string::npos) {
        throw std::invalid_argument("Textul nu poate contine caracterul ';'.");
    }
}

void Depozit::adaugaProdus(const Produs& produs) {
    if (produse.find(produs.getId()) != produse.end()) {
        throw std::runtime_error("Exista deja un produs cu acest ID.");
    }
    produse.emplace(produs.getId(), produs);
}

void Depozit::eliminaProdus(int id) {
    if (produse.erase(id) == 0) {
        throw std::out_of_range("Produsul nu a fost gasit.");
    }
}

void Depozit::restockProdus(int id, int cantitate) {
    auto it = produse.find(id);
    if (it == produse.end()) {
        throw std::out_of_range("Produsul nu a fost gasit.");
    }
    it->second += cantitate;
}

void Depozit::vindeProdus(int id, int cantitate) {
    auto it = produse.find(id);
    if (it == produse.end()) {
        throw std::out_of_range("Produsul nu a fost gasit.");
    }
    it->second -= cantitate;
}

const Produs& Depozit::cautaProdus(int id) const {
    auto it = produse.find(id);
    if (it == produse.end()) {
        throw std::out_of_range("Produsul nu a fost gasit.");
    }
    return it->second;
}

std::vector<Produs> Depozit::listaProduse() const {
    std::vector<Produs> rezultat;
    for (const auto& pereche : produse) {
        rezultat.push_back(pereche.second);
    }
    std::sort(rezultat.begin(), rezultat.end(), [](const Produs& a, const Produs& b) {
        return a.getId() < b.getId();
    });
    return rezultat;
}

std::vector<Produs> Depozit::cautaProduseDupaNume(const std::string& text) const {
    std::vector<Produs> rezultat;
    std::string textCautat = textMic(text);

    for (const auto& pereche : produse) {
        if (textMic(pereche.second.getNume()).find(textCautat) != std::string::npos) {
            rezultat.push_back(pereche.second);
        }
    }

    std::sort(rezultat.begin(), rezultat.end(), [](const Produs& a, const Produs& b) {
        return a.getNume() < b.getNume();
    });
    return rezultat;
}

std::vector<Produs> Depozit::filtreazaDupaCategorie(const std::string& categorie) const {
    std::vector<Produs> rezultat;
    std::string categorieCautata = textMic(categorie);

    for (const auto& pereche : produse) {
        if (textMic(pereche.second.getCategorie()) == categorieCautata) {
            rezultat.push_back(pereche.second);
        }
    }

    std::sort(rezultat.begin(), rezultat.end(), [](const Produs& a, const Produs& b) {
        return a.getNume() < b.getNume();
    });
    return rezultat;
}

std::vector<Produs> Depozit::sorteazaDupaPret(bool crescator) const {
    std::vector<Produs> rezultat = listaProduse();
    std::sort(rezultat.begin(), rezultat.end(), [crescator](const Produs& a, const Produs& b) {
        if (a.getPret() == b.getPret()) {
            return a.getId() < b.getId();
        }
        return crescator ? a.getPret() < b.getPret() : a.getPret() > b.getPret();
    });
    return rezultat;
}

std::vector<Produs> Depozit::raportProduseSubPrag() const {
    std::vector<Produs> rezultat;
    for (const auto& pereche : produse) {
        if (pereche.second.esteSubPrag()) {
            rezultat.push_back(pereche.second);
        }
    }
    std::sort(rezultat.begin(), rezultat.end(), [](const Produs& a, const Produs& b) {
        return a.getId() < b.getId();
    });
    return rezultat;
}

std::vector<Produs> Depozit::sugereazaReaprovizionare() const {
    std::vector<Produs> rezultat = raportProduseSubPrag();
    std::sort(rezultat.begin(), rezultat.end(), [](const Produs& a, const Produs& b) {
        if (a.getCantitate() == b.getCantitate()) {
            return a.getId() < b.getId();
        }
        return a.getCantitate() < b.getCantitate();
    });
    return rezultat;
}

void Depozit::incarcaProduseDinFisier(const std::string& caleFisier) {
    std::ifstream fisier(caleFisier);
    if (!fisier.is_open()) {
        throw std::runtime_error("Fisierul de date nu poate fi deschis pentru citire.");
    }

    std::unordered_map<int, Produs> produseIncarcate;
    std::string linie;
    int numarLinie = 0;

    while (std::getline(fisier, linie)) {
        ++numarLinie;
        if (linie.empty() || linie[0] == '#') {
            continue;
        }

        std::stringstream flux(linie);
        std::string idText;
        std::string nume;
        std::string categorie;
        std::string cantitateText;
        std::string pretText;
        std::string pragText;

        if (!std::getline(flux, idText, ';') ||
            !std::getline(flux, nume, ';') ||
            !std::getline(flux, categorie, ';') ||
            !std::getline(flux, cantitateText, ';') ||
            !std::getline(flux, pretText, ';') ||
            !std::getline(flux, pragText, ';')) {
            throw std::runtime_error("Linie invalida in fisierul de date: " + std::to_string(numarLinie));
        }

        Produs produs(
            std::stoi(idText),
            nume,
            categorie,
            std::stoi(cantitateText),
            std::stod(pretText),
            std::stoi(pragText)
        );

        if (produseIncarcate.find(produs.getId()) != produseIncarcate.end()) {
            throw std::runtime_error("ID duplicat in fisierul de date: " + std::to_string(produs.getId()));
        }
        produseIncarcate.emplace(produs.getId(), produs);
    }

    produse = produseIncarcate;
}

void Depozit::salveazaProduseInFisier(const std::string& caleFisier) const {
    std::filesystem::path cale(caleFisier);
    if (cale.has_parent_path()) {
        std::filesystem::create_directories(cale.parent_path());
    }

    std::ofstream fisier(caleFisier);
    if (!fisier.is_open()) {
        throw std::runtime_error("Fisierul de date nu poate fi deschis pentru scriere.");
    }

    fisier << "# id;nume;categorie;cantitate;pret;pragAlerta\n";
    for (const Produs& produs : listaProduse()) {
        valideazaTextPentruFisier(produs.getNume());
        valideazaTextPentruFisier(produs.getCategorie());

        fisier << produs.getId() << ';'
               << produs.getNume() << ';'
               << produs.getCategorie() << ';'
               << produs.getCantitate() << ';'
               << produs.getPret() << ';'
               << produs.getPragAlerta() << '\n';
    }
}

void Depozit::adaugaFurnizor(const Furnizor& furnizor) {
    if (furnizori.find(furnizor.getId()) != furnizori.end()) {
        throw std::runtime_error("Exista deja un furnizor cu acest ID.");
    }
    furnizori.emplace(furnizor.getId(), furnizor);
}

void Depozit::asociazaProdusCuFurnizor(int furnizorId, int produsId) {
    auto furnizorIt = furnizori.find(furnizorId);
    if (furnizorIt == furnizori.end()) {
        throw std::out_of_range("Furnizorul nu a fost gasit.");
    }
    if (produse.find(produsId) == produse.end()) {
        throw std::out_of_range("Produsul nu a fost gasit.");
    }
    furnizorIt->second.adaugaProdusFurnizat(produsId);
}
