#ifndef PRODUS_H
#define PRODUS_H

#include <memory>
#include <string>

class Produs {
private:
    int id;
    std::string nume;
    std::string categorie;
    int cantitate;
    double pret;
    int pragAlerta;

public:
    Produs();
    Produs(int id, const std::string& nume, int cantitate, double pret, int pragAlerta);
    Produs(int id, const std::string& nume, const std::string& categorie, int cantitate, double pret, int pragAlerta);

    int getId() const;
    const std::string& getNume() const;
    const std::string& getCategorie() const;
    int getCantitate() const;
    double getPret() const;
    int getPragAlerta() const;

    void setNume(const std::string& nume);
    void setCategorie(const std::string& categorie);
    void setPret(double pret);
    void setPragAlerta(int pragAlerta);

    bool esteSubPrag() const;
    std::string getStatusStoc() const;
    virtual std::string getTip() const;
    virtual std::string getDetaliiSpecifice() const;
    virtual std::string getDetaliuPersistenta() const;
    virtual std::shared_ptr<Produs> clone() const;

    Produs& operator+=(int cantitateAdaugata);
    Produs& operator-=(int cantitateScazuta);
    virtual ~Produs() = default;
};

#endif
