#include "mydatastore.h"
#include "util.h"

#include <vector>
#include <map>
#include <set>
#include <string>
#include <ostream>

//constructor
MyDataStore::MyDataStore(){}

//destructor
MyDataStore::~MyDataStore(){
  //make sure there are no mem leaks
  std::map<std::string, User*>::iterator userIt = users_.begin();
  for(; userIt != users_.end(); ++userIt){
    delete userIt -> second;
  }
  for(std::set<Product*>::iterator it=products_.begin(); it != products_.end(); ++it){
    Product* p = *it;
    delete p;
  }
}

//product added to store
void MyDataStore::addProduct(Product* p){
  if (p==0) return;
  products_.insert(p); //put product into MASTER product set

  std::set<std::string> keyWrds = p -> keywords();
  for(std::set<std::string>::iterator it = keyWrds.begin(); it != keyWrds.end(); ++it){
    keywordMap_[*it].insert(p);
  }

}

//adds user to store
void MyDataStore::addUser(User* u){
  if(u == 0) return;

  std::string username = convToLower(u -> getName()); //store as lower to make searchable
  users_[username] = u;
}

//searches for terms connected to AND/OR
std::vector<Product*> MyDataStore::search(std::vector<std::string>& terms, int type){
  std::set<Product*> result;

  if(terms.empty()){
    return std::vector<Product*>(); //no terms --> get no result
  }

  std::string term0 = convToLower(terms[0]);
  result = keywordMap_[term0];

  for(size_t i = 1; i<terms.size(); i++){
    std::string currentTerm = convToLower(terms[i]);
    std::set<Product*> withTerm = keywordMap_[currentTerm];

    if(type == 1){ //OR aka UNION
      result = setUnion(result, withTerm);
    }
    else{ //AND aka INTERSECTION 
      result = setIntersection(result, withTerm);
    }
  }

  searchResults_.clear();
  searchResults_.insert(searchResults_.end(), result.begin(), result.end());

  std::vector<Product*> vectorOut(result.begin(), result.end());
  return vectorOut;
}
void MyDataStore::dump(std::ostream& ofile){
  ofile << "<products>\n";
  for(std::set<Product*>::iterator it = products_.begin(); it != products_.end(); ++it)
  {
    (*it) -> dump(ofile);
  }
  ofile << "</products>\n";

  ofile << "<users>\n";
  for(std::map<std::string, User*>::iterator userIt = users_.begin(); userIt != users_.end(); ++userIt){
    userIt -> second -> dump(ofile);
  }
  ofile << "</users>\n";

}

//results from recent search
std::vector<Product*> MyDataStore::getSearchResults() const 
{
  return searchResults_;
}


bool MyDataStore::addToCart(const std::string& username, Product* hit){
  if (hit == 0) return false;

  std::string uname = convToLower(username);
  if(users_.find(uname) == users_.end()){
    return false;
  }

  carts_[uname].push_back(hit);
  return true;
}

bool MyDataStore::viewCart(const std::string& username, std::ostream& os) const
{
  //make username searchable
  std::string uname = convToLower(username);
  if(users_.find(uname) == users_.end()){
    return false; //no such user exists
  }

  std::map<std::string, std::vector<Product*>>::const_iterator cartsFind = carts_.find(uname);
  if(cartsFind == carts_.end()){
    return true; //user exists but NOCART
  }

  //print cart's items --> FIFO
  const std::vector<Product*>& cart = cartsFind -> second;
  for(size_t i = 0; i < cart.size(); i++){
    os << "Item " << (i+1) <<"\n"; //item num
    os << cart[i] -> displayString() << "\n\n"; //print product details
  }
  return true;

}


bool MyDataStore::buyCart(const std::string& username){
  //make username searchable
  std::string uname = convToLower(username);
  std::map<std::string, User*>::iterator userIt = users_.find(uname);
  if(userIt == users_.end()){
    return false; //not valid user
  }

  User* u = userIt -> second; 
  std::vector<Product*>& cart = carts_[uname];

  std::vector<Product*> leftovers; //hold what you cannot buy from cart
  for(size_t i = 0; i < cart.size(); i++){
    Product* currentProduct = cart[i];

    if(currentProduct ->getQty() > 0 && u -> getBalance() >= currentProduct -> getPrice()){
      //enough qty and user has enough moneeyyy
      currentProduct -> subtractQty(1);
      u->deductAmount(currentProduct -> getPrice());
  
    }
    else leftovers.push_back(currentProduct); //leave in cart cause can't pay up
  }

  //make cart hold only non-purchased items
  cart.swap(leftovers); 
  return true;
}




