#include "Depozit.h"
#include "ProdusElectronic.h"
#include "ProdusMobilier.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace {

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

void valideazaTextFurnizor(const std::string& text) {
    if (text.find(';') != std::string::npos || text.find(',') != std::string::npos) {
        throw std::invalid_argument("Textul nu poate contine ';' sau ',' in fisierul de furnizori.");
    }
}

}

void Depozit::adaugaProdus(const Produs& produs) {
    adaugaProdus(produs.clone());
}

void Depozit::adaugaProdus(std::shared_ptr<Produs> produs) {
    if (!produs) {
        throw std::invalid_argument("Produsul nu poate fi null.");
    }
    if (produse.find(produs->getId()) != produse.end()) {
        throw std::runtime_error("Exista deja un produs cu acest ID.");
    }
    produse.emplace(produs->getId(), produs);
}

void Depozit::eliminaProdus(int id) {
    if (produse.erase(id) == 0) {
        throw std::out_of_range("Produsul nu a fost gasit.");
    }
    for (auto& pereche : furnizori) {
        pereche.second.eliminaProdusFurnizat(id);
    }
}

void Depozit::restockProdus(int id, int cantitate, const std::string& observatii) {
    auto it = produse.find(id);
    if (it == produse.end()) {
        throw std::out_of_range("Produsul nu a fost gasit.");
    }
    *(it->second) += cantitate;
    istoric.inregistreazaIntrare(id, cantitate, observatii);
}

void Depozit::vindeProdus(int id, int cantitate, const std::string& observatii) {
    auto it = produse.find(id);
    if (it == produse.end()) {
        throw std::out_of_range("Produsul nu a fost gasit.");
    }
    *(it->second) -= cantitate;
    istoric.inregistreazaIesire(id, cantitate, observatii);
}

const Produs& Depozit::cautaProdus(int id) const {
    auto it = produse.find(id);
    if (it == produse.end()) {
        throw std::out_of_range("Produsul nu a fost gasit.");
    }
    return *(it->second);
}

std::vector<std::shared_ptr<const Produs>> Depozit::listaProduse() const {
    std::vector<std::shared_ptr<const Produs>> rezultat;
    for (const auto& pereche : produse) {
        rezultat.push_back(pereche.second);
    }
    std::sort(rezultat.begin(), rezultat.end(), [](const auto& a, const auto& b) {
        return a->getId() < b->getId();
    });
    return rezultat;
}

std::vector<std::shared_ptr<const Produs>> Depozit::cautaProduseDupaNume(const std::string& text) const {
    std::vector<std::shared_ptr<const Produs>> rezultat;
    std::string textCautat = textMic(text);

    for (const auto& pereche : produse) {
        if (textMic(pereche.second->getNume()).find(textCautat) != std::string::npos) {
            rezultat.push_back(pereche.second);
        }
    }

    std::sort(rezultat.begin(), rezultat.end(), [](const auto& a, const auto& b) {
        return a->getNume() < b->getNume();
    });
    return rezultat;
}

std::vector<std::shared_ptr<const Produs>> Depozit::filtreazaDupaCategorie(const std::string& categorie) const {
    std::vector<std::shared_ptr<const Produs>> rezultat;
    std::string categorieCautata = textMic(categorie);

    for (const auto& pereche : produse) {
        if (textMic(pereche.second->getCategorie()) == categorieCautata) {
            rezultat.push_back(pereche.second);
        }
    }

    std::sort(rezultat.begin(), rezultat.end(), [](const auto& a, const auto& b) {
        return a->getNume() < b->getNume();
    });
    return rezultat;
}

std::vector<std::shared_ptr<const Produs>> Depozit::sorteazaDupaPret(bool crescator) const {
    auto rezultat = listaProduse();
    std::sort(rezultat.begin(), rezultat.end(), [crescator](const auto& a, const auto& b) {
        if (a->getPret() == b->getPret()) {
            return a->getId() < b->getId();
        }
        return crescator ? a->getPret() < b->getPret() : a->getPret() > b->getPret();
    });
    return rezultat;
}

std::vector<std::shared_ptr<const Produs>> Depozit::raportProduseSubPrag() const {
    std::vector<std::shared_ptr<const Produs>> rezultat;
    for (const auto& pereche : produse) {
        if (pereche.second->esteSubPrag()) {
            rezultat.push_back(pereche.second);
        }
    }
    std::sort(rezultat.begin(), rezultat.end(), [](const auto& a, const auto& b) {
        return a->getId() < b->getId();
    });
    return rezultat;
}

std::vector<std::shared_ptr<const Produs>> Depozit::sugereazaReaprovizionare() const {
    auto rezultat = raportProduseSubPrag();
    std::sort(rezultat.begin(), rezultat.end(), [](const auto& a, const auto& b) {
        if (a->getCantitate() == b->getCantitate()) {
            return a->getId() < b->getId();
        }
        return a->getCantitate() < b->getCantitate();
    });
    return rezultat;
}

void Depozit::incarcaProduseDinFisier(const std::string& caleFisier) {
    std::ifstream fisier(caleFisier);
    if (!fisier.is_open()) {
        throw std::runtime_error("Fisierul de date nu poate fi deschis pentru citire.");
    }

    std::unordered_map<int, std::shared_ptr<Produs>> produseIncarcate;
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
        std::string tip;
        std::string detaliuExtra;

        if (!std::getline(flux, tip, ';') ||
            !std::getline(flux, idText, ';') ||
            !std::getline(flux, nume, ';') ||
            !std::getline(flux, categorie, ';') ||
            !std::getline(flux, cantitateText, ';') ||
            !std::getline(flux, pretText, ';') ||
            !std::getline(flux, pragText, ';')) {
            throw std::runtime_error("Linie invalida in fisierul de date: " + std::to_string(numarLinie));
        }
        std::getline(flux, detaliuExtra, ';');

        int id = std::stoi(idText);
        int cantitate = std::stoi(cantitateText);
        double pret = std::stod(pretText);
        int prag = std::stoi(pragText);
        std::shared_ptr<Produs> produs;

        if (tip == "Electronic") {
            produs = std::make_shared<ProdusElectronic>(id, nume, categorie, cantitate, pret, prag, std::stoi(detaliuExtra));
        } else if (tip == "Mobilier") {
            produs = std::make_shared<ProdusMobilier>(id, nume, categorie, cantitate, pret, prag, detaliuExtra);
        } else if (tip == "Standard") {
            produs = std::make_shared<Produs>(id, nume, categorie, cantitate, pret, prag);
        } else {
            throw std::runtime_error("Tip necunoscut in fisierul de date: " + tip);
        }

        if (produseIncarcate.find(produs->getId()) != produseIncarcate.end()) {
            throw std::runtime_error("ID duplicat in fisierul de date: " + std::to_string(produs->getId()));
        }
        produseIncarcate.emplace(produs->getId(), produs);
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

    fisier << "# tip;id;nume;categorie;cantitate;pret;pragAlerta;detaliuExtra\n";
    for (const auto& produs : listaProduse()) {
        valideazaTextPentruFisier(produs->getNume());
        valideazaTextPentruFisier(produs->getCategorie());
        valideazaTextPentruFisier(produs->getDetaliuPersistenta());

        fisier << produs->getTip() << ';'
               << produs->getId() << ';'
               << produs->getNume() << ';'
               << produs->getCategorie() << ';'
               << produs->getCantitate() << ';'
               << produs->getPret() << ';'
               << produs->getPragAlerta() << ';'
               << produs->getDetaliuPersistenta() << '\n';
    }
}

void Depozit::adaugaFurnizor(const Furnizor& furnizor) {
    if (furnizori.find(furnizor.getId()) != furnizori.end()) {
        throw std::runtime_error("Exista deja un furnizor cu acest ID.");
    }
    furnizori.emplace(furnizor.getId(), furnizor);
}

void Depozit::eliminaFurnizor(int id) {
    if (furnizori.erase(id) == 0) {
        throw std::out_of_range("Furnizorul nu a fost gasit.");
    }
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

void Depozit::deasociazaProdusDeFurnizor(int furnizorId, int produsId) {
    auto furnizorIt = furnizori.find(furnizorId);
    if (furnizorIt == furnizori.end()) {
        throw std::out_of_range("Furnizorul nu a fost gasit.");
    }
    if (!furnizorIt->second.eliminaProdusFurnizat(produsId)) {
        throw std::out_of_range("Produsul nu este asociat cu acest furnizor.");
    }
}

const Furnizor& Depozit::cautaFurnizor(int id) const {
    auto it = furnizori.find(id);
    if (it == furnizori.end()) {
        throw std::out_of_range("Furnizorul nu a fost gasit.");
    }
    return it->second;
}

std::vector<Furnizor> Depozit::listaFurnizori() const {
    std::vector<Furnizor> rezultat;
    rezultat.reserve(furnizori.size());
    for (const auto& pereche : furnizori) {
        rezultat.push_back(pereche.second);
    }
    std::sort(rezultat.begin(), rezultat.end(), [](const Furnizor& a, const Furnizor& b) {
        return a.getId() < b.getId();
    });
    return rezultat;
}

std::vector<std::shared_ptr<const Produs>> Depozit::produsePentruFurnizor(int furnizorId) const {
    auto it = furnizori.find(furnizorId);
    if (it == furnizori.end()) {
        throw std::out_of_range("Furnizorul nu a fost gasit.");
    }
    std::vector<std::shared_ptr<const Produs>> rezultat;
    for (int produsId : it->second.getProduseFurnizate()) {
        auto produsIt = produse.find(produsId);
        if (produsIt != produse.end()) {
            rezultat.push_back(produsIt->second);
        }
    }
    std::sort(rezultat.begin(), rezultat.end(), [](const auto& a, const auto& b) {
        return a->getId() < b->getId();
    });
    return rezultat;
}

std::vector<std::pair<Furnizor, std::vector<std::shared_ptr<const Produs>>>>
Depozit::sugestiiReaprovizionarePeFurnizor() const {
    std::vector<std::pair<Furnizor, std::vector<std::shared_ptr<const Produs>>>> rezultat;
    for (const auto& furnizor : listaFurnizori()) {
        std::vector<std::shared_ptr<const Produs>> deReaprovizionat;
        for (int produsId : furnizor.getProduseFurnizate()) {
            auto produsIt = produse.find(produsId);
            if (produsIt != produse.end() && produsIt->second->esteSubPrag()) {
                deReaprovizionat.push_back(produsIt->second);
            }
        }
        if (deReaprovizionat.empty()) {
            continue;
        }
        std::sort(deReaprovizionat.begin(), deReaprovizionat.end(), [](const auto& a, const auto& b) {
            if (a->getCantitate() == b->getCantitate()) {
                return a->getId() < b->getId();
            }
            return a->getCantitate() < b->getCantitate();
        });
        rezultat.emplace_back(furnizor, std::move(deReaprovizionat));
    }
    return rezultat;
}

void Depozit::incarcaFurnizoriDinFisier(const std::string& caleFisier) {
    std::ifstream fisier(caleFisier);
    if (!fisier.is_open()) {
        throw std::runtime_error("Fisierul de furnizori nu poate fi deschis pentru citire.");
    }

    std::unordered_map<int, Furnizor> furnizoriIncarcati;
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
        std::string contact;
        std::string produseText;

        if (!std::getline(flux, idText, ';') ||
            !std::getline(flux, nume, ';') ||
            !std::getline(flux, contact, ';')) {
            throw std::runtime_error("Linie invalida in fisierul de furnizori: " + std::to_string(numarLinie));
        }
        std::getline(flux, produseText, ';');

        int id = std::stoi(idText);
        Furnizor furnizor(id, nume, contact);

        if (!produseText.empty()) {
            std::stringstream produseFlux(produseText);
            std::string produsIdText;
            while (std::getline(produseFlux, produsIdText, ',')) {
                if (produsIdText.empty()) {
                    continue;
                }
                furnizor.adaugaProdusFurnizat(std::stoi(produsIdText));
            }
        }

        if (furnizoriIncarcati.find(id) != furnizoriIncarcati.end()) {
            throw std::runtime_error("ID duplicat in fisierul de furnizori: " + std::to_string(id));
        }
        furnizoriIncarcati.emplace(id, furnizor);
    }

    furnizori = std::move(furnizoriIncarcati);
}

void Depozit::salveazaFurnizoriInFisier(const std::string& caleFisier) const {
    std::filesystem::path cale(caleFisier);
    if (cale.has_parent_path()) {
        std::filesystem::create_directories(cale.parent_path());
    }

    std::ofstream fisier(caleFisier);
    if (!fisier.is_open()) {
        throw std::runtime_error("Fisierul de furnizori nu poate fi deschis pentru scriere.");
    }

    fisier << "# id;nume;contact;produseIds (separate cu virgula)\n";
    for (const auto& furnizor : listaFurnizori()) {
        valideazaTextFurnizor(furnizor.getNume());
        valideazaTextFurnizor(furnizor.getContact());

        fisier << furnizor.getId() << ';'
               << furnizor.getNume() << ';'
               << furnizor.getContact() << ';';

        const auto& produseIds = furnizor.getProduseFurnizate();
        for (std::size_t i = 0; i < produseIds.size(); ++i) {
            if (i > 0) {
                fisier << ',';
            }
            fisier << produseIds[i];
        }
        fisier << '\n';
    }
}

const IstoricTranzactii& Depozit::getIstoric() const {
    return istoric;
}

IstoricTranzactii& Depozit::getIstoric() {
    return istoric;
}

void Depozit::incarcaIstoricDinFisier(const std::string& caleFisier) {
    istoric.incarcaDinFisier(caleFisier);
}

void Depozit::salveazaIstoricInFisier(const std::string& caleFisier) const {
    istoric.salveazaInFisier(caleFisier);
}
