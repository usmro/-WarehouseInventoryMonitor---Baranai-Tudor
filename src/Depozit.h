#ifndef DEPOZIT_H
#define DEPOZIT_H

#include "Furnizor.h"
#include "Produs.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Depozit {
private:
    std::unordered_map<int, std::shared_ptr<Produs>> produse;
    std::unordered_map<int, Furnizor> furnizori;

public:
    void adaugaProdus(const Produs& produs);
    void adaugaProdus(std::shared_ptr<Produs> produs);
    void eliminaProdus(int id);
    void restockProdus(int id, int cantitate);
    void vindeProdus(int id, int cantitate);

    const Produs& cautaProdus(int id) const;
    std::vector<std::shared_ptr<const Produs>> listaProduse() const;
    std::vector<std::shared_ptr<const Produs>> cautaProduseDupaNume(const std::string& text) const;
    std::vector<std::shared_ptr<const Produs>> filtreazaDupaCategorie(const std::string& categorie) const;
    std::vector<std::shared_ptr<const Produs>> sorteazaDupaPret(bool crescator = true) const;
    std::vector<std::shared_ptr<const Produs>> raportProduseSubPrag() const;
    std::vector<std::shared_ptr<const Produs>> sugereazaReaprovizionare() const;
    void incarcaProduseDinFisier(const std::string& caleFisier);
    void salveazaProduseInFisier(const std::string& caleFisier) const;

    void adaugaFurnizor(const Furnizor& furnizor);
    void asociazaProdusCuFurnizor(int furnizorId, int produsId);
};

#endif
