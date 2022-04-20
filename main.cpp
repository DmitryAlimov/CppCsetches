#include <iostream>
#include <map>
#include <iostream>
#include <string>
using namespace std;

//инициализируем фабрику: (сlass ID??) - тип данных-идентификатора (в данном случае - строка),
//                         class Base - класс-предок с наследниками которого работает фабрика
//                         class ... Args - типы данных, используемых для инициализации наследников
template<class ID,class Base,class ... Args> class GenericObjectFactory{
private:
    //Объявляем тип данных = функция fInstantiator, которая возвращает указатель на new объект типа Base
    //Все созданные по шаблону функции instantiator будут являться одновременно и fInstantiator (т. е. это что-то вроде класса-предка)
    typedef Base* (*fInstantiator)(Args ...);

    //Объявляем шаблонную функцию instantiator, которая возвращает указатель на new объект класса Base (или его наследника)
    //Поскольку функция возвращает *Base, то ее тип это одновременно и fInstantiator
    //static - чтобы инициализировать функцию и не потерять
    template<class Derived>
    static Base* instantiator(Args ... args){
        return new Derived(args ...);
    }

    //хранит инстантиаторы, к которым можно доступиться по ключу
    map<ID, fInstantiator> classes;

public:
    GenericObjectFactory(){}
    //Добавляем в map указатель на объект шаблонной функции instantiator, который будет доступен по ключу-строке id
    template<class Derived> void add(ID id){
        classes[id]=&instantiator<Derived>;
    }
    //возвращаем указатель на instantiator, т е функцию, которая возвращает указатель на new объект
    fInstantiator get(ID id){
        return classes[id];
    }

};


class Animal{
public:
    Animal(bool isAlive,string name) : isAlive(isAlive),name(name){};
    bool isAlive;
    string name;
    virtual string voice() const=0;
};
class Dog : public Animal{
public:
    using Animal::Animal;
    string voice() const{
        return this->isAlive?
            "Woof! I'm "+this->name+"n":
            "";
    }
};
class Cat : public Animal{
public:
    using Animal::Animal;
    string voice() const{
    	return this->isAlive?
            "Meow, I'm "+this->name+"n":
            "";
    }
};

int a(int val){
    return val;
}

int main(){
    GenericObjectFactory<string,Animal,bool,string> animalFactory;

    animalFactory.add<Dog>("man's friend");
    animalFactory.add<Cat>("^_^");

    Animal *dog1=animalFactory.get("man's friend")(true,"charlie");
    Animal *dog2=animalFactory.get("man's friend")(false,"fido");
    Animal *cat =animalFactory.get("^_^")(true,"begemoth");

    cout << dog1->voice()
         << dog2->voice()
         << cat ->voice();

    return 0;
}
