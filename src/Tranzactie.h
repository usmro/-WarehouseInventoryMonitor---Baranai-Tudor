#ifndef TRANZACTIE_H
#define TRANZACTIE_H

#include <stdexcept>
#include <string>

struct Intrare {
    static std::string numeTip() {
        return "Intrare";
    }
};

struct Iesire {
    static std::string numeTip() {
        return "Iesire";
    }
};

template <typename TP>
class Tranzactie {
private:
    int produsId;
    int cantitate;
    std::string observatii;

public:
    Tranzactie(int produsId, int cantitate, const std::string& observatii = "")
        : produsId(produsId), cantitate(cantitate), observatii(observatii) {
        if (produsId <= 0) {
            throw std::invalid_argument("ID-ul produsului din tranzactie trebuie sa fie pozitiv.");
        }
        if (cantitate <= 0) {
            throw std::invalid_argument("Cantitatea din tranzactie trebuie sa fie pozitiva.");
        }
    }

    int getProdusId() const {
        return produsId;
    }

    int getCantitate() const {
        return cantitate;
    }

    const std::string& getObservatii() const {
        return observatii;
    }

    std::string getTip() const {
        return TP::numeTip();
    }
};

#endif
