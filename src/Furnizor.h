#ifndef FURNIZOR_H
#define FURNIZOR_H

#include <string>
#include <vector>

class Furnizor {
private:
    int id;
    std::string nume;
    std::string contact;
    std::vector<int> produseFurnizate;

public:
    Furnizor();
    Furnizor(int id, const std::string& nume, const std::string& contact);

    int getId() const;
    const std::string& getNume() const;
    const std::string& getContact() const;
    const std::vector<int>& getProduseFurnizate() const;

    void adaugaProdusFurnizat(int produsId);
    bool eliminaProdusFurnizat(int produsId);
    bool furnizeazaProdusul(int produsId) const;
    void setContact(const std::string& contact);
};

#endif
