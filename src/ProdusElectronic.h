#ifndef PRODUS_ELECTRONIC_H
#define PRODUS_ELECTRONIC_H

#include "Produs.h"

class ProdusElectronic : public Produs {
private:
    int garantieLuni;

public:
    ProdusElectronic(int id, const std::string& nume, const std::string& categorie,
                     int cantitate, double pret, int pragAlerta, int garantieLuni);

    int getGarantieLuni() const;
    void setGarantieLuni(int garantieLuni);

    std::string getTip() const override;
    std::string getDetaliiSpecifice() const override;
    std::string getDetaliuPersistenta() const override;
    std::shared_ptr<Produs> clone() const override;
};

#endif
