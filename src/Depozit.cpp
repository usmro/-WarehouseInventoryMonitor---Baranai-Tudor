#include "Depozit.h"

#include <algorithm>
#include <cctype>
#include <stdexcept>

std::string textMic(const std::string& text) {
    std::string rezultat = text;
    std::transform(rezultat.begin(), rezultat.end(), rezultat.begin(), [](unsigned char caracter) {
        return static_cast<char>(std::tolower(caracter));
    });
    return rezultat;
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
