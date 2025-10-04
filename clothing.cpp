#include "clothing.h"
#include <sstream>

Clothing::Clothing(const std::string& name, double price, int qty,
const std::string& size, const std::string& brand)
: Product("clothing", name, price, qty),size_(size),brand_(brand)
{}
Clothing::~Clothing(){}

std::set<std::string> Clothing::keywords() const{
  std::set<std::string> nameKeys = parseStringToWords(name_);
  std::set<std::string> brandKeys = parseStringToWords(brand_);
  std::set<std::string> Ckeys = setUnion(nameKeys,brandKeys);

  return Ckeys;
}

std::string Clothing::displayString() const{
  std::stringstream ss;
  ss << name_ << "\n" << "Size: " << size_ << " Brand: "<< brand_ << "\n"
     << price_ << " " << qty_ << " left.";
  return ss.str();
}

void Clothing::dump(std::ostream& os) const{
  os << category_ << "\n"
     << name_ << "\n"
     << price_ << "\n"
     << qty_ << "\n"
     << size_ << "\n"
     << brand_ << "\n";

}
