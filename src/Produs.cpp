#include "Produs.h"

#include <stdexcept>

Produs::Produs() : id(0), cantitate(0), pret(0.0), pragAlerta(0) {
}

Produs::Produs(int id, const std::string& nume, int cantitate, double pret, int pragAlerta)
    : id(id), nume(nume), cantitate(cantitate), pret(pret), pragAlerta(pragAlerta) {
    if (id <= 0) {
        throw std::invalid_argument("ID-ul produsului trebuie sa fie pozitiv.");
    }
    if (nume.empty()) {
        throw std::invalid_argument("Numele produsului nu poate fi gol.");
    }
    if (cantitate < 0) {
        throw std::invalid_argument("Cantitatea nu poate fi negativa.");
    }
    if (pret < 0) {
        throw std::invalid_argument("Pretul nu poate fi negativ.");
    }
    if (pragAlerta < 0) {
        throw std::invalid_argument("Pragul de alerta nu poate fi negativ.");
    }
}

int Produs::getId() const {
    return id;
}

const std::string& Produs::getNume() const {
    return nume;
}

int Produs::getCantitate() const {
    return cantitate;
}

double Produs::getPret() const {
    return pret;
}

int Produs::getPragAlerta() const {
    return pragAlerta;
}

void Produs::setNume(const std::string& numeNou) {
    if (numeNou.empty()) {
        throw std::invalid_argument("Numele produsului nu poate fi gol.");
    }
    nume = numeNou;
}

void Produs::setPret(double pretNou) {
    if (pretNou < 0) {
        throw std::invalid_argument("Pretul nu poate fi negativ.");
    }
    pret = pretNou;
}

void Produs::setPragAlerta(int pragNou) {
    if (pragNou < 0) {
        throw std::invalid_argument("Pragul de alerta nu poate fi negativ.");
    }
    pragAlerta = pragNou;
}

bool Produs::esteSubPrag() const {
    return cantitate < pragAlerta;
}

Produs& Produs::operator+=(int cantitateAdaugata) {
    if (cantitateAdaugata < 0) {
        throw std::invalid_argument("Cantitatea adaugata nu poate fi negativa.");
    }
    cantitate += cantitateAdaugata;
    return *this;
}

Produs& Produs::operator-=(int cantitateScazuta) {
    if (cantitateScazuta < 0) {
        throw std::invalid_argument("Cantitatea scazuta nu poate fi negativa.");
    }
    if (cantitateScazuta > cantitate) {
        throw std::runtime_error("Stoc insuficient pentru aceasta operatie.");
    }
    cantitate -= cantitateScazuta;
    return *this;
}
