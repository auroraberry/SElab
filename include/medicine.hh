#ifndef MEDICINE
#define MEDICINE
using namespace std;
#include <string>



class medicine 
{
protected:
    int id;
    string name;
    string description;
    int price;
    int quantity;

public:
    medicine();
    void fillMap();
    void saveMap();
    void addMedicine();
    void printDetails();
    void printDetailsFromHistory(string extraDetails = "");
    void getDetails(int rec = 0);
    void getDetailsFromHistory();
    void removeMedicine();
};
#endif 
