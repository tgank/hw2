#include "book.h"
#include <sstream>


//constructor
Book::Book(const std::string& name, double price, int qty,
const std::string& isbn, const std::string& author)
: Product("book", name, price, qty),isbn_(isbn),author_(author)
{}
//destructor
Book::~Book(){}


std::set<std::string> Book::keywords() const{

  //parse through name
  std::set<std::string> nameKeys = parseStringToWords(name_);
  //parse through author
  std::set<std::string> authorKeys = parseStringToWords(author_);
  //
  std::set<std::string> Bkeys = setUnion(nameKeys,authorKeys);
  Bkeys.insert(convToLower(isbn_));

  return Bkeys;
}

std::string Book::displayString() const{
  std::stringstream ss;
  ss << name_ << "\n" << "Author: " << author_ << " ISBN: "<< isbn_ << "\n"
     << price_ << " " << qty_ << " left.";
  return ss.str();
}

void Book::dump(std::ostream& os) const{
  os << category_ << "\n"
     << name_ << "\n"
     << price_ << "\n"
     << qty_ << "\n"
     << isbn_ << "\n"
     << author_ << "\n";

}
