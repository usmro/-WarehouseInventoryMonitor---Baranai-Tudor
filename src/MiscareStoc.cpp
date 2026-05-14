#include "MiscareStoc.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>

MiscareStoc::MiscareStoc(int produsId, int cantitate, const std::string& timestamp, const std::string& observatii)
    : produsId(produsId), cantitate(cantitate), timestamp(timestamp), observatii(observatii) {
    if (produsId <= 0) {
        throw std::invalid_argument("ID-ul produsului din miscare trebuie sa fie pozitiv.");
    }
    if (cantitate <= 0) {
        throw std::invalid_argument("Cantitatea din miscare trebuie sa fie pozitiva.");
    }
    if (timestamp.empty()) {
        throw std::invalid_argument("Timestamp-ul miscarii nu poate fi gol.");
    }
}

int MiscareStoc::getProdusId() const {
    return produsId;
}

int MiscareStoc::getCantitate() const {
    return cantitate;
}

const std::string& MiscareStoc::getTimestamp() const {
    return timestamp;
}

const std::string& MiscareStoc::getObservatii() const {
    return observatii;
}

std::string MiscareIntrare::getTip() const {
    return "Intrare";
}

int MiscareIntrare::getDeltaStoc() const {
    return cantitate;
}

std::shared_ptr<MiscareStoc> MiscareIntrare::clone() const {
    return std::make_shared<MiscareIntrare>(*this);
}

std::string MiscareIesire::getTip() const {
    return "Iesire";
}

int MiscareIesire::getDeltaStoc() const {
    return -cantitate;
}

std::shared_ptr<MiscareStoc> MiscareIesire::clone() const {
    return std::make_shared<MiscareIesire>(*this);
}

std::string momentulCurent() {
    auto acum = std::chrono::system_clock::now();
    std::time_t timpC = std::chrono::system_clock::to_time_t(acum);
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &timpC);
#else
    localtime_r(&timpC, &tm);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}
