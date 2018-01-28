#include <iostream>

class Base {
public:
  Base() { std::cout << "Base()" << std::endl; }
  virtual ~Base() { std::cout << "~Base()" << std::endl; }

  virtual void virfunc() { 
    std::cout << "base virfunc" << std::endl; 
  }

  virtual void purevirfunc() = 0;
};

class Derived : public Base {
public:
  Derived() : Base() { std::cout << "Derived()" << std::endl; }
  virtual ~Derived() { std::cout << "~Derived()" << std::endl; }

  //void virfunc() override { std::cout << "derived virtual" << std::endl; }

  void purevirfunc() override { std::cout << "derived purevirfunc" << std::endl; }
}; 

int main(int argc, char ** argv) {
  Base * c = new Derived();
  c->virfunc();
  c->purevirfunc();
  delete c;
  return 0;
}
