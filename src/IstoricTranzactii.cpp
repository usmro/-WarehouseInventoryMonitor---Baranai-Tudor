#include "IstoricTranzactii.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace {
void valideazaTextPentruIstoric(const std::string& text) {
    if (text.find(';') != std::string::npos) {
        throw std::invalid_argument("Textul nu poate contine caracterul ';' in istoric.");
    }
    if (text.find('\n') != std::string::npos || text.find('\r') != std::string::npos) {
        throw std::invalid_argument("Textul nu poate contine sfarsit de linie in istoric.");
    }
}
}

void IstoricTranzactii::inregistreazaIntrare(int produsId, int cantitate, const std::string& observatii) {
    miscari.push_back(std::make_shared<MiscareIntrare>(produsId, cantitate, momentulCurent(), observatii));
}

void IstoricTranzactii::inregistreazaIesire(int produsId, int cantitate, const std::string& observatii) {
    miscari.push_back(std::make_shared<MiscareIesire>(produsId, cantitate, momentulCurent(), observatii));
}

void IstoricTranzactii::adauga(std::shared_ptr<MiscareStoc> miscare) {
    if (!miscare) {
        throw std::invalid_argument("Miscarea nu poate fi null.");
    }
    miscari.push_back(std::move(miscare));
}

std::size_t IstoricTranzactii::numarMiscari() const {
    return miscari.size();
}

std::vector<std::shared_ptr<const MiscareStoc>> IstoricTranzactii::toateMiscarile() const {
    return std::vector<std::shared_ptr<const MiscareStoc>>(miscari.begin(), miscari.end());
}

std::vector<std::shared_ptr<const MiscareStoc>> IstoricTranzactii::miscariPentruProdus(int produsId) const {
    std::vector<std::shared_ptr<const MiscareStoc>> rezultat;
    for (const auto& miscare : miscari) {
        if (miscare->getProdusId() == produsId) {
            rezultat.push_back(miscare);
        }
    }
    return rezultat;
}

std::vector<std::shared_ptr<const MiscareStoc>> IstoricTranzactii::ultimele(std::size_t n) const {
    std::vector<std::shared_ptr<const MiscareStoc>> rezultat;
    if (n == 0 || miscari.empty()) {
        return rezultat;
    }
    std::size_t inceput = miscari.size() > n ? miscari.size() - n : 0;
    for (std::size_t i = inceput; i < miscari.size(); ++i) {
        rezultat.push_back(miscari[i]);
    }
    return rezultat;
}

void IstoricTranzactii::incarcaDinFisier(const std::string& caleFisier) {
    std::ifstream fisier(caleFisier);
    if (!fisier.is_open()) {
        throw std::runtime_error("Fisierul de istoric nu poate fi deschis pentru citire.");
    }

    std::vector<std::shared_ptr<MiscareStoc>> incarcate;
    std::string linie;
    int numarLinie = 0;

    while (std::getline(fisier, linie)) {
        ++numarLinie;
        if (linie.empty() || linie[0] == '#') {
            continue;
        }

        std::stringstream flux(linie);
        std::string tip;
        std::string timestamp;
        std::string idText;
        std::string cantitateText;
        std::string observatii;

        if (!std::getline(flux, tip, ';') ||
            !std::getline(flux, timestamp, ';') ||
            !std::getline(flux, idText, ';') ||
            !std::getline(flux, cantitateText, ';')) {
            throw std::runtime_error("Linie invalida in fisierul de istoric: " + std::to_string(numarLinie));
        }
        std::getline(flux, observatii);

        int produsId = std::stoi(idText);
        int cantitate = std::stoi(cantitateText);

        if (tip == "Intrare") {
            incarcate.push_back(std::make_shared<MiscareIntrare>(produsId, cantitate, timestamp, observatii));
        } else if (tip == "Iesire") {
            incarcate.push_back(std::make_shared<MiscareIesire>(produsId, cantitate, timestamp, observatii));
        } else {
            throw std::runtime_error("Tip de miscare necunoscut in istoric: " + tip);
        }
    }

    miscari = std::move(incarcate);
}

void IstoricTranzactii::salveazaInFisier(const std::string& caleFisier) const {
    std::filesystem::path cale(caleFisier);
    if (cale.has_parent_path()) {
        std::filesystem::create_directories(cale.parent_path());
    }

    std::ofstream fisier(caleFisier);
    if (!fisier.is_open()) {
        throw std::runtime_error("Fisierul de istoric nu poate fi deschis pentru scriere.");
    }

    fisier << "# tip;timestamp;produsId;cantitate;observatii\n";
    for (const auto& miscare : miscari) {
        valideazaTextPentruIstoric(miscare->getTimestamp());
        valideazaTextPentruIstoric(miscare->getObservatii());

        fisier << miscare->getTip() << ';'
               << miscare->getTimestamp() << ';'
               << miscare->getProdusId() << ';'
               << miscare->getCantitate() << ';'
               << miscare->getObservatii() << '\n';
    }
}

void IstoricTranzactii::goleste() {
    miscari.clear();
}
