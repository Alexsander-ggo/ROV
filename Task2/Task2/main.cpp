#include <iostream>

#include "List.h"

struct Person
{
    int age = 0;
    int weight = 0;
};

std::ostream& operator<<(std::ostream& out, const Person& person) {
    return out << "Person:"
               << " Age = " << person.age
               << " Weight = " << person.weight << "\n";
}

bool operator<(const Person& left, const Person& right) {
    return left.age < right.age;
}

void Test() {
    List<int> listI;
    listI.sortBtoL();
    listI.print();
    int count = 7;
    for (int i = 0; i < count; ++i) {
        listI.pushFront(rand() % count);
        listI.print();
    }
    std::cout << std::endl;
    listI.sortBtoL(1, 5);
    std::cout << "OK" << std::endl;
    listI.print();
    listI.sortLtoB();
    std::cout << "OK2" << std::endl;
    listI.print();
    Person p;
    List<Person> listP;
    //int count = 7;
    for (int i = 0; i < count; ++i) {
        listP.pushFront({rand() % count, rand() % count});
        //listP.print();
    }
    listP.print();
    listP.sortLtoB();
    listP.print();
    listP.sortBtoL();
    listP.print();
}

int main()
{
    Test();
    return 0;
}

//    template <typename Pred>
//    void sortQuick(Node* from, Node* to, Pred pred)
//    {
//        if (from != to) {
//            Node* begin = from;
//            Node* end = shift(to, -1);
//            Node* cur = begin;
//            T value = end->getData();
//            uint64 count = 0;
//            while (begin != end) {
//                if (pred(begin->getData(), value)) {
//                    this->swap(begin, cur);
//                    cur = shift(cur, 1);
//                    ++count;
//                }
//                begin = shift(begin, 1);
//            }
//            this->swap(cur, end);
//            sortQuick(from, cur, pred);
//            sortQuick(shift(cur, 1), to, pred);
//        }
//    }
