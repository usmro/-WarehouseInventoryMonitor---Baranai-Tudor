#include "Furnizor.h"

#include <algorithm>
#include <stdexcept>

Furnizor::Furnizor() : id(0) {
}

Furnizor::Furnizor(int id, const std::string& nume, const std::string& contact)
    : id(id), nume(nume), contact(contact) {
    if (id <= 0) {
        throw std::invalid_argument("ID-ul furnizorului trebuie sa fie pozitiv.");
    }
    if (nume.empty()) {
        throw std::invalid_argument("Numele furnizorului nu poate fi gol.");
    }
}

int Furnizor::getId() const {
    return id;
}

const std::string& Furnizor::getNume() const {
    return nume;
}

const std::string& Furnizor::getContact() const {
    return contact;
}

const std::vector<int>& Furnizor::getProduseFurnizate() const {
    return produseFurnizate;
}

void Furnizor::adaugaProdusFurnizat(int produsId) {
    if (produsId <= 0) {
        throw std::invalid_argument("ID-ul produsului trebuie sa fie pozitiv.");
    }
    if (std::find(produseFurnizate.begin(), produseFurnizate.end(), produsId) == produseFurnizate.end()) {
        produseFurnizate.push_back(produsId);
    }
}
