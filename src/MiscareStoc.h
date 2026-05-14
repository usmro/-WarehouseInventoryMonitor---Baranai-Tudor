#ifndef MISCARE_STOC_H
#define MISCARE_STOC_H

#include <memory>
#include <string>

class MiscareStoc {
protected:
    int produsId;
    int cantitate;
    std::string timestamp;
    std::string observatii;

public:
    MiscareStoc(int produsId, int cantitate, const std::string& timestamp, const std::string& observatii);
    virtual ~MiscareStoc() = default;

    int getProdusId() const;
    int getCantitate() const;
    const std::string& getTimestamp() const;
    const std::string& getObservatii() const;

    virtual std::string getTip() const = 0;
    virtual int getDeltaStoc() const = 0;
    virtual std::shared_ptr<MiscareStoc> clone() const = 0;
};

class MiscareIntrare : public MiscareStoc {
public:
    using MiscareStoc::MiscareStoc;
    std::string getTip() const override;
    int getDeltaStoc() const override;
    std::shared_ptr<MiscareStoc> clone() const override;
};

class MiscareIesire : public MiscareStoc {
public:
    using MiscareStoc::MiscareStoc;
    std::string getTip() const override;
    int getDeltaStoc() const override;
    std::shared_ptr<MiscareStoc> clone() const override;
};

std::string momentulCurent();

#endif
