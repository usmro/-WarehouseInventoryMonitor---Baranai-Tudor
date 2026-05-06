#include "../src/Depozit.h"
#include "../src/Tranzactie.h"

#include <cassert>
#include <stdexcept>
#include <vector>

void testAdaugareProdus() {
    Depozit depozit;
    depozit.adaugaProdus(Produs(1, "Laptop", "Electronice", 5, 2500.0, 2));

    const Produs& produs = depozit.cautaProdus(1);
    assert(produs.getId() == 1);
    assert(produs.getNume() == "Laptop");
    assert(produs.getCategorie() == "Electronice");
    assert(produs.getCantitate() == 5);
}

void testIdDuplicat() {
    Depozit depozit;
    depozit.adaugaProdus(Produs(1, "Laptop", 5, 2500.0, 2));

    bool exceptiePrinsa = false;
    try {
        depozit.adaugaProdus(Produs(1, "Mouse", 20, 50.0, 5));
    } catch (const std::runtime_error&) {
        exceptiePrinsa = true;
    }

    assert(exceptiePrinsa);
}

void testOperatoriCantitate() {
    Produs produs(2, "Monitor", 10, 800.0, 3);
    produs += 5;
    assert(produs.getCantitate() == 15);

    produs -= 4;
    assert(produs.getCantitate() == 11);
}

void testStocInsuficient() {
    Produs produs(3, "Tastatura", 2, 120.0, 5);

    bool exceptiePrinsa = false;
    try {
        produs -= 3;
    } catch (const std::runtime_error&) {
        exceptiePrinsa = true;
    }

    assert(exceptiePrinsa);
}

void testStatusStoc() {
    Produs inStoc(1, "Dulap", "Mobilier", 8, 900.0, 3);
    Produs stocRedus(2, "Bec LED", "Accesorii", 2, 15.0, 5);
    Produs indisponibil(3, "Frigider", "Electrocasnice", 0, 1800.0, 2);

    assert(inStoc.getStatusStoc() == "In stoc");
    assert(stocRedus.getStatusStoc() == "Stoc redus");
    assert(indisponibil.getStatusStoc() == "Indisponibil");
}

void testRaportSubPrag() {
    Depozit depozit;
    depozit.adaugaProdus(Produs(1, "Laptop", 5, 2500.0, 2));
    depozit.adaugaProdus(Produs(2, "Mouse", 3, 50.0, 10));
    depozit.adaugaProdus(Produs(3, "Monitor", 1, 800.0, 5));

    std::vector<Produs> raport = depozit.raportProduseSubPrag();
    assert(raport.size() == 2);
    assert(raport[0].getId() == 2);
    assert(raport[1].getId() == 3);
}

void testCautareFiltrareSortareCatalog() {
    Depozit depozit;
    depozit.adaugaProdus(Produs(1, "Laptop Lenovo", "Electronice", 5, 2500.0, 2));
    depozit.adaugaProdus(Produs(2, "Dulap alb", "Mobilier", 3, 700.0, 1));
    depozit.adaugaProdus(Produs(3, "Mouse wireless", "Electronice", 20, 80.0, 5));

    std::vector<Produs> cautare = depozit.cautaProduseDupaNume("lap");
    assert(cautare.size() == 1);
    assert(cautare[0].getId() == 1);

    std::vector<Produs> electronice = depozit.filtreazaDupaCategorie("electronice");
    assert(electronice.size() == 2);

    std::vector<Produs> sortate = depozit.sorteazaDupaPret(true);
    assert(sortate[0].getId() == 3);
    assert(sortate[1].getId() == 2);
    assert(sortate[2].getId() == 1);
}

void testSugestiiSortate() {
    Depozit depozit;
    depozit.adaugaProdus(Produs(1, "Laptop", 5, 2500.0, 10));
    depozit.adaugaProdus(Produs(2, "Mouse", 1, 50.0, 10));
    depozit.adaugaProdus(Produs(3, "Monitor", 3, 800.0, 5));

    std::vector<Produs> sugestii = depozit.sugereazaReaprovizionare();
    assert(sugestii.size() == 3);
    assert(sugestii[0].getId() == 2);
    assert(sugestii[1].getId() == 3);
    assert(sugestii[2].getId() == 1);
}

void testTranzactieTemplate() {
    Tranzactie<Intrare> intrare(1, 10, "restock");
    Tranzactie<Iesire> iesire(1, 2, "vanzare");

    assert(intrare.getTip() == "Intrare");
    assert(iesire.getTip() == "Iesire");
    assert(intrare.getCantitate() == 10);
    assert(iesire.getProdusId() == 1);
}

void testTranzactieInvalida() {
    bool exceptiePrinsa = false;
    try {
        Tranzactie<Intrare> tranzactie(1, 0, "cantitate invalida");
    } catch (const std::invalid_argument&) {
        exceptiePrinsa = true;
    }

    assert(exceptiePrinsa);
}

int main() {
    testAdaugareProdus();
    testIdDuplicat();
    testOperatoriCantitate();
    testStocInsuficient();
    testStatusStoc();
    testRaportSubPrag();
    testCautareFiltrareSortareCatalog();
    testSugestiiSortate();
    testTranzactieTemplate();
    testTranzactieInvalida();

    return 0;
}
