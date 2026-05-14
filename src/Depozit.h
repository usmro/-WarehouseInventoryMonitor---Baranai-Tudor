#ifndef DEPOZIT_H
#define DEPOZIT_H

#include "Furnizor.h"
#include "IstoricTranzactii.h"
#include "Produs.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class Depozit {
private:
    std::unordered_map<int, std::shared_ptr<Produs>> produse;
    std::unordered_map<int, Furnizor> furnizori;
    IstoricTranzactii istoric;

public:
    void adaugaProdus(const Produs& produs);
    void adaugaProdus(std::shared_ptr<Produs> produs);
    void eliminaProdus(int id);
    void restockProdus(int id, int cantitate, const std::string& observatii = "");
    void vindeProdus(int id, int cantitate, const std::string& observatii = "");

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
    void eliminaFurnizor(int id);
    void asociazaProdusCuFurnizor(int furnizorId, int produsId);
    void deasociazaProdusDeFurnizor(int furnizorId, int produsId);
    const Furnizor& cautaFurnizor(int id) const;
    std::vector<Furnizor> listaFurnizori() const;
    std::vector<std::shared_ptr<const Produs>> produsePentruFurnizor(int furnizorId) const;
    std::vector<std::pair<Furnizor, std::vector<std::shared_ptr<const Produs>>>>
        sugestiiReaprovizionarePeFurnizor() const;

    void incarcaFurnizoriDinFisier(const std::string& caleFisier);
    void salveazaFurnizoriInFisier(const std::string& caleFisier) const;

    const IstoricTranzactii& getIstoric() const;
    IstoricTranzactii& getIstoric();
    void incarcaIstoricDinFisier(const std::string& caleFisier);
    void salveazaIstoricInFisier(const std::string& caleFisier) const;
};

#endif
