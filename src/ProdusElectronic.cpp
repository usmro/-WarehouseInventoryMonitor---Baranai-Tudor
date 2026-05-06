#include "ProdusElectronic.h"

#include <memory>
#include <stdexcept>

ProdusElectronic::ProdusElectronic(int id, const std::string& nume, const std::string& categorie,
                                   int cantitate, double pret, int pragAlerta, int garantieLuni)
    : Produs(id, nume, categorie, cantitate, pret, pragAlerta), garantieLuni(garantieLuni) {
    if (garantieLuni < 0) {
        throw std::invalid_argument("Garantia nu poate fi negativa.");
    }
}

int ProdusElectronic::getGarantieLuni() const {
    return garantieLuni;
}

void ProdusElectronic::setGarantieLuni(int garantieNoua) {
    if (garantieNoua < 0) {
        throw std::invalid_argument("Garantia nu poate fi negativa.");
    }
    garantieLuni = garantieNoua;
}

std::string ProdusElectronic::getTip() const {
    return "Electronic";
}

std::string ProdusElectronic::getDetaliiSpecifice() const {
    return "Garantie: " + std::to_string(garantieLuni) + " luni";
}

std::string ProdusElectronic::getDetaliuPersistenta() const {
    return std::to_string(garantieLuni);
}

std::shared_ptr<Produs> ProdusElectronic::clone() const {
    return std::make_shared<ProdusElectronic>(*this);
}
