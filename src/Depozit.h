#ifndef DEPOZIT_H
#define DEPOZIT_H

#include "Furnizor.h"
#include "Produs.h"

#include <unordered_map>
#include <vector>

class Depozit {
private:
    std::unordered_map<int, Produs> produse;
    std::unordered_map<int, Furnizor> furnizori;

public:
    void adaugaProdus(const Produs& produs);
    void eliminaProdus(int id);
    void restockProdus(int id, int cantitate);
    void vindeProdus(int id, int cantitate);

    const Produs& cautaProdus(int id) const;
    std::vector<Produs> listaProduse() const;
    std::vector<Produs> raportProduseSubPrag() const;
    std::vector<Produs> sugereazaReaprovizionare() const;

    void adaugaFurnizor(const Furnizor& furnizor);
    void asociazaProdusCuFurnizor(int furnizorId, int produsId);
};

#endif
