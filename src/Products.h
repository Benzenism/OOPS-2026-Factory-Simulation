#pragma once
#include <string>

class Product {
protected:
    std::string id;
public:
    explicit Product(std::string id);
    virtual ~Product() = default;

    virtual std::string getId() const;
    virtual std::string getType() const = 0;
};

class RawWafer : public Product {
public:
    explicit RawWafer(std::string id);
    std::string getType() const override;
};

class RAMModule : public Product {
public:
    explicit RAMModule(std::string id);
    std::string getType() const override;
};
