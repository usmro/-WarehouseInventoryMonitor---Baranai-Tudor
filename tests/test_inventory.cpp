#include "../src/Depozit.h"
#include "../src/Furnizor.h"
#include "../src/IstoricTranzactii.h"
#include "../src/MiscareStoc.h"
#include "../src/ProdusElectronic.h"
#include "../src/ProdusMobilier.h"
#include "../src/Tranzactie.h"

#include <cassert>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
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

void testMostenireSiPolimorfism() {
    Depozit depozit;
    depozit.adaugaProdus(std::make_shared<ProdusElectronic>(1, "Laptop Lenovo", "Laptopuri", 4, 2500.0, 2, 24));
    depozit.adaugaProdus(std::make_shared<ProdusMobilier>(2, "Dulap alb", "Dulapuri", 2, 700.0, 1, "PAL"));

    std::vector<std::shared_ptr<const Produs>> produse = depozit.listaProduse();
    assert(produse.size() == 2);
    assert(produse[0]->getTip() == "Electronic");
    assert(produse[0]->getDetaliiSpecifice() == "Garantie: 24 luni");
    assert(produse[1]->getTip() == "Mobilier");
    assert(produse[1]->getDetaliiSpecifice() == "Material: PAL");
}

void testRaportSubPrag() {
    Depozit depozit;
    depozit.adaugaProdus(Produs(1, "Laptop", 5, 2500.0, 2));
    depozit.adaugaProdus(Produs(2, "Mouse", 3, 50.0, 10));
    depozit.adaugaProdus(Produs(3, "Monitor", 1, 800.0, 5));

    std::vector<std::shared_ptr<const Produs>> raport = depozit.raportProduseSubPrag();
    assert(raport.size() == 2);
    assert(raport[0]->getId() == 2);
    assert(raport[1]->getId() == 3);
}

void testCautareFiltrareSortareCatalog() {
    Depozit depozit;
    depozit.adaugaProdus(Produs(1, "Laptop Lenovo", "Electronice", 5, 2500.0, 2));
    depozit.adaugaProdus(Produs(2, "Dulap alb", "Mobilier", 3, 700.0, 1));
    depozit.adaugaProdus(Produs(3, "Mouse wireless", "Electronice", 20, 80.0, 5));

    std::vector<std::shared_ptr<const Produs>> cautare = depozit.cautaProduseDupaNume("lenovo");
    assert(cautare.size() == 1);
    assert(cautare[0]->getId() == 1);

    std::vector<std::shared_ptr<const Produs>> electronice = depozit.filtreazaDupaCategorie("electronice");
    assert(electronice.size() == 2);

    std::vector<std::shared_ptr<const Produs>> sortate = depozit.sorteazaDupaPret(true);
    assert(sortate[0]->getId() == 3);
    assert(sortate[1]->getId() == 2);
    assert(sortate[2]->getId() == 1);
}

void testSugestiiSortate() {
    Depozit depozit;
    depozit.adaugaProdus(Produs(1, "Laptop", 5, 2500.0, 10));
    depozit.adaugaProdus(Produs(2, "Mouse", 1, 50.0, 10));
    depozit.adaugaProdus(Produs(3, "Monitor", 3, 800.0, 5));

    std::vector<std::shared_ptr<const Produs>> sugestii = depozit.sugereazaReaprovizionare();
    assert(sugestii.size() == 3);
    assert(sugestii[0]->getId() == 2);
    assert(sugestii[1]->getId() == 3);
    assert(sugestii[2]->getId() == 1);
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

void testPersistentaProduse() {
    const char* caleFisier = "build/test_produse.csv";

    Depozit depozit;
    depozit.adaugaProdus(std::make_shared<ProdusMobilier>(10, "Raft metalic", "Depozitare", 6, 350.0, 2, "Otel"));
    depozit.adaugaProdus(std::make_shared<ProdusElectronic>(11, "Scanner cod bare", "Electronice", 3, 420.0, 1, 24));
    depozit.salveazaProduseInFisier(caleFisier);

    Depozit depozitIncarcat;
    depozitIncarcat.incarcaProduseDinFisier(caleFisier);

    std::vector<std::shared_ptr<const Produs>> produse = depozitIncarcat.listaProduse();
    assert(produse.size() == 2);
    assert(produse[0]->getId() == 10);
    assert(produse[0]->getCategorie() == "Depozitare");
    assert(produse[0]->getTip() == "Mobilier");
    assert(produse[0]->getDetaliiSpecifice() == "Material: Otel");
    assert(produse[1]->getId() == 11);
    assert(produse[1]->getNume() == "Scanner cod bare");
    assert(produse[1]->getTip() == "Electronic");

    std::remove(caleFisier);
}

void testIstoricInregistreazaRestockSiVanzare() {
    Depozit depozit;
    depozit.adaugaProdus(Produs(1, "Laptop", "Electronice", 5, 2500.0, 2));

    depozit.restockProdus(1, 3, "comanda noua");
    depozit.vindeProdus(1, 4, "client local");

    const IstoricTranzactii& istoric = depozit.getIstoric();
    assert(istoric.numarMiscari() == 2);

    auto miscari = istoric.toateMiscarile();
    assert(miscari[0]->getTip() == "Intrare");
    assert(miscari[0]->getCantitate() == 3);
    assert(miscari[0]->getDeltaStoc() == 3);
    assert(miscari[0]->getObservatii() == "comanda noua");

    assert(miscari[1]->getTip() == "Iesire");
    assert(miscari[1]->getDeltaStoc() == -4);

    assert(depozit.cautaProdus(1).getCantitate() == 4);
}

void testIstoricPolimorfism() {
    IstoricTranzactii istoric;
    istoric.inregistreazaIntrare(7, 10, "");
    istoric.inregistreazaIesire(7, 2, "");

    auto miscari = istoric.toateMiscarile();
    assert(miscari.size() == 2);

    int suma = 0;
    for (const auto& miscare : miscari) {
        suma += miscare->getDeltaStoc();
    }
    assert(suma == 8);

    auto copie = miscari[0]->clone();
    assert(copie->getTip() == "Intrare");
    assert(copie->getProdusId() == 7);
}

void testIstoricFiltrarePerProdus() {
    Depozit depozit;
    depozit.adaugaProdus(Produs(1, "Laptop", "Electronice", 10, 2500.0, 2));
    depozit.adaugaProdus(Produs(2, "Mouse", "Periferice", 20, 80.0, 5));

    depozit.restockProdus(1, 5);
    depozit.restockProdus(2, 10);
    depozit.vindeProdus(1, 3);

    auto miscariProdus1 = depozit.getIstoric().miscariPentruProdus(1);
    assert(miscariProdus1.size() == 2);
    assert(miscariProdus1[0]->getTip() == "Intrare");
    assert(miscariProdus1[1]->getTip() == "Iesire");

    auto ultimele = depozit.getIstoric().ultimele(2);
    assert(ultimele.size() == 2);
    assert(ultimele[1]->getProdusId() == 1);
}

void testIstoricPersistenta() {
    const char* caleFisier = "build/test_istoric.csv";

    Depozit depozit;
    depozit.adaugaProdus(Produs(1, "Laptop", "Electronice", 5, 2500.0, 2));
    depozit.restockProdus(1, 4, "comanda lunara");
    depozit.vindeProdus(1, 2, "vanzare online");
    depozit.salveazaIstoricInFisier(caleFisier);

    Depozit incarcat;
    incarcat.incarcaIstoricDinFisier(caleFisier);

    auto miscari = incarcat.getIstoric().toateMiscarile();
    assert(miscari.size() == 2);
    assert(miscari[0]->getTip() == "Intrare");
    assert(miscari[0]->getCantitate() == 4);
    assert(miscari[0]->getObservatii() == "comanda lunara");
    assert(miscari[1]->getTip() == "Iesire");
    assert(miscari[1]->getCantitate() == 2);

    std::remove(caleFisier);
}

void testFurnizorAsociereSiCautare() {
    Depozit depozit;
    depozit.adaugaProdus(Produs(1, "Laptop", "Electronice", 5, 2500.0, 2));
    depozit.adaugaProdus(Produs(2, "Mouse", "Periferice", 20, 80.0, 5));
    depozit.adaugaFurnizor(Furnizor(10, "ACME SRL", "acme@example.ro"));
    depozit.asociazaProdusCuFurnizor(10, 1);
    depozit.asociazaProdusCuFurnizor(10, 2);

    auto produseFurnizor = depozit.produsePentruFurnizor(10);
    assert(produseFurnizor.size() == 2);

    const Furnizor& f = depozit.cautaFurnizor(10);
    assert(f.furnizeazaProdusul(1));
    assert(f.furnizeazaProdusul(2));
}

void testEliminareProdusScoateFurnizor() {
    Depozit depozit;
    depozit.adaugaProdus(Produs(1, "Laptop", "Electronice", 5, 2500.0, 2));
    depozit.adaugaFurnizor(Furnizor(10, "ACME", "acme@example.ro"));
    depozit.asociazaProdusCuFurnizor(10, 1);

    depozit.eliminaProdus(1);

    assert(!depozit.cautaFurnizor(10).furnizeazaProdusul(1));
}

void testReaprovizionarePeFurnizor() {
    Depozit depozit;
    depozit.adaugaProdus(Produs(1, "Laptop", "Electronice", 1, 2500.0, 5));
    depozit.adaugaProdus(Produs(2, "Mouse", "Periferice", 20, 80.0, 5));
    depozit.adaugaProdus(Produs(3, "Tastatura", "Periferice", 0, 120.0, 4));

    depozit.adaugaFurnizor(Furnizor(10, "ACME", "acme@example.ro"));
    depozit.adaugaFurnizor(Furnizor(20, "TechDist", "tech@example.ro"));

    depozit.asociazaProdusCuFurnizor(10, 1);
    depozit.asociazaProdusCuFurnizor(10, 2);
    depozit.asociazaProdusCuFurnizor(20, 3);

    auto sugestii = depozit.sugestiiReaprovizionarePeFurnizor();
    assert(sugestii.size() == 2);

    assert(sugestii[0].first.getId() == 10);
    assert(sugestii[0].second.size() == 1);
    assert(sugestii[0].second[0]->getId() == 1);

    assert(sugestii[1].first.getId() == 20);
    assert(sugestii[1].second.size() == 1);
    assert(sugestii[1].second[0]->getId() == 3);
}

void testFurnizorPersistenta() {
    const char* caleFisier = "build/test_furnizori.csv";

    Depozit depozit;
    depozit.adaugaProdus(Produs(1, "Laptop", "Electronice", 5, 2500.0, 2));
    depozit.adaugaProdus(Produs(2, "Mouse", "Periferice", 20, 80.0, 5));
    depozit.adaugaFurnizor(Furnizor(10, "ACME SRL", "acme@example.ro"));
    depozit.adaugaFurnizor(Furnizor(20, "TechDist", ""));
    depozit.asociazaProdusCuFurnizor(10, 1);
    depozit.asociazaProdusCuFurnizor(10, 2);

    depozit.salveazaFurnizoriInFisier(caleFisier);

    Depozit incarcat;
    incarcat.adaugaProdus(Produs(1, "Laptop", "Electronice", 5, 2500.0, 2));
    incarcat.adaugaProdus(Produs(2, "Mouse", "Periferice", 20, 80.0, 5));
    incarcat.incarcaFurnizoriDinFisier(caleFisier);

    auto furnizori = incarcat.listaFurnizori();
    assert(furnizori.size() == 2);
    assert(furnizori[0].getId() == 10);
    assert(furnizori[0].getNume() == "ACME SRL");
    assert(furnizori[0].getProduseFurnizate().size() == 2);
    assert(furnizori[1].getId() == 20);
    assert(furnizori[1].getProduseFurnizate().empty());

    auto produseAcme = incarcat.produsePentruFurnizor(10);
    assert(produseAcme.size() == 2);
    assert(produseAcme[0]->getId() == 1);

    std::remove(caleFisier);
}

int main() {
    testAdaugareProdus();
    testIdDuplicat();
    testOperatoriCantitate();
    testStocInsuficient();
    testStatusStoc();
    testMostenireSiPolimorfism();
    testRaportSubPrag();
    testCautareFiltrareSortareCatalog();
    testSugestiiSortate();
    testTranzactieTemplate();
    testTranzactieInvalida();
    testPersistentaProduse();
    testIstoricInregistreazaRestockSiVanzare();
    testIstoricPolimorfism();
    testIstoricFiltrarePerProdus();
    testIstoricPersistenta();
    testFurnizorAsociereSiCautare();
    testEliminareProdusScoateFurnizor();
    testReaprovizionarePeFurnizor();
    testFurnizorPersistenta();

    return 0;
}
