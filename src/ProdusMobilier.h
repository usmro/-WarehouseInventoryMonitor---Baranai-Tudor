#ifndef PRODUS_MOBILIER_H
#define PRODUS_MOBILIER_H

#include "Produs.h"

class ProdusMobilier : public Produs {
private:
    std::string material;

public:
    ProdusMobilier(int id, const std::string& nume, const std::string& categorie,
                   int cantitate, double pret, int pragAlerta, const std::string& material);

    const std::string& getMaterial() const;
    void setMaterial(const std::string& material);

    std::string getTip() const override;
    std::string getDetaliiSpecifice() const override;
    std::string getDetaliuPersistenta() const override;
    std::shared_ptr<Produs> clone() const override;
};

#endif
