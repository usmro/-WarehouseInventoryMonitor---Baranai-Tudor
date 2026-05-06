#ifndef PRODUS_H
#define PRODUS_H

#include <string>

class Produs {
private:
    int id;
    std::string nume;
    int cantitate;
    double pret;
    int pragAlerta;

public:
    Produs();
    Produs(int id, const std::string& nume, int cantitate, double pret, int pragAlerta);

    int getId() const;
    const std::string& getNume() const;
    int getCantitate() const;
    double getPret() const;
    int getPragAlerta() const;

    void setNume(const std::string& nume);
    void setPret(double pret);
    void setPragAlerta(int pragAlerta);

    bool esteSubPrag() const;

    Produs& operator+=(int cantitateAdaugata);
    Produs& operator-=(int cantitateScazuta);
};

#endif
