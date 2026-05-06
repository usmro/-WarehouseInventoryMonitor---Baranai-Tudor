#include "ProdusMobilier.h"

#include <memory>
#include <stdexcept>

ProdusMobilier::ProdusMobilier(int id, const std::string& nume, const std::string& categorie,
                               int cantitate, double pret, int pragAlerta, const std::string& material)
    : Produs(id, nume, categorie, cantitate, pret, pragAlerta), material(material) {
    if (material.empty()) {
        throw std::invalid_argument("Materialul nu poate fi gol.");
    }
}

const std::string& ProdusMobilier::getMaterial() const {
    return material;
}

void ProdusMobilier::setMaterial(const std::string& materialNou) {
    if (materialNou.empty()) {
        throw std::invalid_argument("Materialul nu poate fi gol.");
    }
    material = materialNou;
}

std::string ProdusMobilier::getTip() const {
    return "Mobilier";
}

std::string ProdusMobilier::getDetaliiSpecifice() const {
    return "Material: " + material;
}

std::string ProdusMobilier::getDetaliuPersistenta() const {
    return material;
}

std::shared_ptr<Produs> ProdusMobilier::clone() const {
    return std::make_shared<ProdusMobilier>(*this);
}
