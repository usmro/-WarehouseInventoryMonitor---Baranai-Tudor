#ifndef ISTORIC_TRANZACTII_H
#define ISTORIC_TRANZACTII_H

#include "MiscareStoc.h"

#include <memory>
#include <string>
#include <vector>

class IstoricTranzactii {
private:
    std::vector<std::shared_ptr<MiscareStoc>> miscari;

public:
    void inregistreazaIntrare(int produsId, int cantitate, const std::string& observatii = "");
    void inregistreazaIesire(int produsId, int cantitate, const std::string& observatii = "");
    void adauga(std::shared_ptr<MiscareStoc> miscare);

    std::size_t numarMiscari() const;
    std::vector<std::shared_ptr<const MiscareStoc>> toateMiscarile() const;
    std::vector<std::shared_ptr<const MiscareStoc>> miscariPentruProdus(int produsId) const;
    std::vector<std::shared_ptr<const MiscareStoc>> ultimele(std::size_t n) const;

    void incarcaDinFisier(const std::string& caleFisier);
    void salveazaInFisier(const std::string& caleFisier) const;
    void goleste();
};

#endif
