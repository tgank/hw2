#ifndef MYDATASTORE_H
#define MYDATASTORE_H

#include "datastore.h"
#include "product.h"
#include "user.h"

#include <vector>
#include <set>
#include <map>

#include <string>
#include <ostream>

class MyDataStore : public DataStore {
  public:
    MyDataStore();
    ~MyDataStore() override;
    void addProduct(Product* p) override;
    void addUser(User* u) override;

    //0=AND, 1=OR
    std::vector<Product*> search(std::vector<std::string>& terms, int type) override;
    void dump(std::ostream& ofile) override;

    //for amazon.cpp
    bool addToCart(const std::string& username, Product* hit);
    bool viewCart(const std::string& username, std::ostream& os) const;
    bool buyCart(const std::string& username);

    std::vector<Product*> getSearchResults() const;

    private:

    std::set<Product*> products_;
    std::map<std::string, User*> users_;
    std::map<std::string, std::set<Product*>> keywordMap_; // keywordn --> products
    std::map<std::string, std::vector<Product*>> carts_; //FIFO
    std::vector<Product*> searchResults_;

};



#endif