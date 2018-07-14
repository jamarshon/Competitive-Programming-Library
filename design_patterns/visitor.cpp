#include <iostream>
#include <vector>

using namespace std;

class IVisitor;

class IVisitable {
public:
    virtual double accept(IVisitor& visitor) = 0;
};

class Liquor;
class Tobacco;
class Necessity;

class IVisitor {
public:
    virtual double visit(Liquor& liquor_item) = 0;
    virtual double visit(Tobacco& tobacco_item) = 0;
    virtual double visit(Necessity& necessity_item) = 0;
};

class Liquor : public IVisitable {
public:
    double accept(IVisitor& visitor) override { return visitor.visit(*this); }
    double price = 0.01;
};

class Tobacco : public IVisitable {
public:
    double accept(IVisitor& visitor) override { return visitor.visit(*this); }
    double price = 0.02;
};

class Necessity : public IVisitable {
public:
    double accept(IVisitor& visitor) override { return visitor.visit(*this); }
    double price = 0.03;
};

class TaxVisitor : public IVisitor {
public:
    double visit(Liquor& liquor_item) override { return liquor_item.price * kLiquorTax; }
    double visit(Tobacco& tobacco_item) override { return tobacco_item.price * kTobaccoTax; }
    double visit(Necessity& necessity_item)  override { return necessity_item.price * kNecessityTax; }
private:
    const double kLiquorTax = 0.01;
    const double kTobaccoTax = 0.02;
    const double kNecessityTax = 0.03;
};

class TaxHolidayVisitor : public IVisitor {
public:
    double visit(Liquor& liquor_item) override { return liquor_item.price * kLiquorTax; }
    double visit(Tobacco& tobacco_item) override { return tobacco_item.price * kTobaccoTax; }
    double visit(Necessity& necessity_item)  override { return necessity_item.price * kNecessityTax; }
private:
    const double kLiquorTax = 0.01 * 0.01;
    const double kTobaccoTax = 0.02 * 0.02;
    const double kNecessityTax = 0.03 * 0.03;
};

int main() {
    TaxVisitor tax_visitor;
    TaxHolidayVisitor tax_holiday_visitor;

    vector<IVisitable*> arr{new Liquor(), new Tobacco(), new Necessity()};

    for(const auto& e: arr) {
        cout << (e->accept(tax_visitor)) << endl;
        cout << (e->accept(tax_holiday_visitor)) << endl;
    }
    return 0;
}

