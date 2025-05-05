#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Discount {
public:
    virtual double apply(double amount) const = 0;
    virtual string getDescription() const = 0;
    virtual ~Discount() = default;
};

class NoDiscount : public Discount {
public:
    double apply(double amount) const override {
        return amount;
    }

    string getDescription() const override {
        return "No discount";
    }
};

class TenPercentDiscount : public Discount {
public:
    double apply(double amount) const override {
        return amount * 0.9;
    }

    string getDescription() const override {
        return "10% discount";
    }
};

class TwentyPercentDiscount : public Discount {
public:
    double apply(double amount) const override {
        return amount * 0.8;
    }

    string getDescription() const override {
        return "20% discount";
    }
};

class DiscountCalculator {
private:
    unique_ptr<Discount> discount;

public:
    void setDiscount(unique_ptr<Discount> d) {
        discount = move(d);
    }

    double calculate(double amount) const {
        return discount ? discount->apply(amount) : amount;
    }

    string getDiscountDescription() const {
        return discount ? discount->getDescription() : "No discount set";
    }
};

string toLower(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

int main() {
    double amount;
    string input;
    DiscountCalculator calculator;

    cout << "Enter Amount: ";
    cin >> amount;
    cin.ignore();

    cout << "Enter discount (No discount, 10%, 20%): ";
    getline(cin, input);

    string normalizedInput = toLower(input);

    unordered_map<string, function<unique_ptr<Discount>()>> discountMap = {
        {"no discount", []() { return make_unique<NoDiscount>(); }},
        {"10%", []() { return make_unique<TenPercentDiscount>(); }},
        {"20%", []() { return make_unique<TwentyPercentDiscount>(); }}
    };

    if (discountMap.count(normalizedInput)) {
        calculator.setDiscount(discountMap[normalizedInput]());
    } else {
        cout << "Invalid discount option. Using no discount by default.\n";
        calculator.setDiscount(make_unique<NoDiscount>());
    }

    double total = calculator.calculate(amount);

    cout << "Discount: " << calculator.getDiscountDescription() << endl;
    cout << "Total: " << total << endl;

    return 0;
}
