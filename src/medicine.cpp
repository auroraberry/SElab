using namespace std;
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "./../include/global.hh"
#include "./../include/medicine.hh"
#include "./../include/hospital.hh"

medicine::medicine()
{
    id = -1;
    name = "";
    description = "";
    price = 0;
    quantity = 0;
}

void medicine::fillMap()
{
    fstream f;
    f.open("./data/medicines.csv", ios::in);
    string temp;
    //skipping the first row containing column headers;
    getline(f >> ws, temp);
    //analyzing each entry afterwards;
    while (getline(f >> ws, temp))
    {
        medicine m;
        //creating a string stream object to read from string 'temp';
        stringstream s(temp);
        string s1, s4, s5;
        //reading from the string stream object 's';
        getline(s, s1, ',');
        getline(s, m.name, ',');
        getline(s, m.description, ',');
        getline(s, s4, ',');
        getline(s, s5, ',');
        m.id = strToNum(s1);
        m.price = strToNum(s4);
        m.quantity = strToNum(s5);
        hospital::medicinesList[m.id] = m;
    }
    f.close();
    return;
}
void medicine::saveMap()
{
    fstream f;
    f.open("./data/temp.csv", ios::out);
    // `le first line conataining column headers:
    f << "medicineId,name,description,price,quantity\n";
    for (auto i : hospital::medicinesList)
        f << i.second.id << "," << i.second.name << "," << i.second.description << "," << i.second.price << "," << i.second.quantity << endl;
    f.close();
    remove("./data/medicines.csv");
    rename("./data/temp.csv", "./data/medicines.csv");
    return;
}
void medicine::addMedicine()
{
    if (hospital::medicinesList.size() == hospital::medicinesLimit)
    {
        cout << "\n\nMedicines limit reached, can't add more!\n\n";
        return;
    }
    cout << "\nEnter the name of the medicine: \n";
    getline(cin >> ws, name);
    cout << "\nEnter the description of the medicine: \n";
    getline(cin, description);
    cout << "\nEnter the price of the medicine: \n";
    cin >> price;
    cout << "\nEnter the quantity of the medicine: \n";
    cin >> quantity;
    if (hospital::medicinesList.rbegin() != hospital::medicinesList.rend())
        id = ((hospital::medicinesList.rbegin())->first) + 1;
    else
        id = 1;
    hospital::medicinesList[id] = *this;

    //creating a fstream object to read/write from/to files;
    fstream f;
    //creating a record in medicines.csv;
    f.open("./data/medicines.csv", ios::app);
    f << id << "," << name << "," << description << "," << price << "," << quantity << endl;
    f.close();

    cout << "\n"
         << name << " added successfully!\n";
    cout << "Their ID is: " << id << "\n";

    return;
}
void medicine::printDetails()
{
    if (id == -1)
        return;
    cout << "Medicine ID     : " << id << "\n";
    cout << "Name            : " << name << "\n";
    cout << "Description     : " << description << "\n";
    cout << "Price           : " << price << "\n";
    cout << "Quantity        : " << quantity << "\n";
    return;
}
void medicine::printDetailsFromHistory(string extraDetails)
{
    if (id == -1)
        return;
    if (extraDetails == "")
    {
        fstream f;
        f.open("./data/medicinesHistory.csv", ios::in);
        string temp;
        //skipping the first row containing column headers;
        getline(f >> ws, temp);
        //analyzing each entry afterwards;
        while (getline(f >> ws, temp))
        {
            medicine m;
            //creating a string stream object to read from string 'temp';
            stringstream s(temp);
            string s1, s4, s5;
            //reading from the string stream object 's';
            getline(s, s1, ',');
            getline(s, m.name, ',');
            getline(s, m.description, ',');
            getline(s, s4, ',');
            getline(s, s5, ',');
            if (strToNum(s1) == id)
            {
                getline(s, extraDetails);
                price = strToNum(s4);
                quantity = strToNum(s5);
            }
        }
        f.close();
    }
}
void medicine::getDetails(int rec)
{
    int opt = 0;
    cout << "\nOPTIONS:\n[1]: Filter by ID\n[2]: Filter by Name\n\n";
    cin >> opt;
    while (opt != 1 && opt != 2)
        cout << "option 1 or 2?\n", cin >> opt;
    //1: Filter by ID;
    if (opt == 1)
    {
        int reqId;
        cout << "\nEnter ID:\n";
        cin >> reqId;
        if (hospital::medicinesList.find(reqId) != hospital::medicinesList.end())
            *this = hospital::medicinesList[reqId];
        else
            cout << "\nNo matching record found!\n";
    }
    //2: Filter by name;
    else if (opt == 2)
    {
        string reqName;
        cout << "Enter the name of medicine required:\n";
        getline(cin >> ws, reqName);
        for (auto i : hospital::medicinesList)
        {
            if (i.second.name == reqName)
            {
                *this = i.second;
                return;
            }
        }
        //if a record is found, it's details will be stored in the medicine class object that called this function,
        //and the control is returned;
        //else:
        cout << "\nNo matching record found!\n";
    }
    return;
}
void medicine::getDetailsFromHistory()
{
    int opt = 0;
    cout << "\nOPTIONS:\n[1]: Filter by Name\n[2]: Filter by ID\n\n";
    cin >> opt;
    while (opt != 1 && opt != 2)
        cout << "option 1 or 2?\n", cin >> opt;

    //1: Filter by name;
    if (opt == 1)
    {
        string reqName;
        cout << "Enter the name of medicine required:\n";
        getline(cin >> ws, reqName);
        vector<medicine> matchingRecords;
        vector<string> extraDetails;
        fstream f;
        f.open("./data/medicinesHistory.csv", ios::in);
        string temp;
        //skipping the first row containing column headers;
        getline(f >> ws, temp);
        //analyzing each entry afterwards;
        while (getline(f >> ws, temp))
        {
            medicine m;
            //creating a string stream object to read from string 'temp';
            stringstream s(temp);
            string s1, s4, s5;
            //reading from the string stream object 's';
            getline(s, s1, ',');
            getline(s, m.name, ',');
            getline(s, m.description, ',');
            getline(s, s4, ',');
            getline(s, s5, ',');
            if (m.name == reqName)
            {
                getline(s, extraDetails[matchingRecords.size()]);
                m.id = strToNum(s1);
                m.price = strToNum(s4);
                m.quantity = strToNum(s5);
                matchingRecords.push_back(m);
            }
        }
        f.close();
        cout << "\n";
        cout << matchingRecords.size() << " matching record(s) found!\n";
        for (int i = 0; i < matchingRecords.size(); i++)
            matchingRecords[i].printDetailsFromHistory(extraDetails[i]);
    }
    //2: Filter by ID;
    else if (opt == 2)
    {
        int reqId;
        cout << "\nEnter ID:\n";
        cin >> reqId;
        vector<medicine> matchingRecords;
        vector<string> extraDetails;
        fstream f;
        f.open("./data/medicinesHistory.csv", ios::in);
        string temp;
        //skipping the first row containing column
        getline(f >> ws, temp);
        //analyzing each entry afterwards;
        while (getline(f >> ws, temp))
        {
            medicine m;
            //creating a string stream object to read from string 'temp';
            stringstream s(temp);
            string s1, s4, s5;
            //reading from the string stream object 's';
            getline(s, s1, ',');
            getline(s, m.name, ',');
            getline(s, m.description, ',');
            getline(s, s4, ',');
            getline(s, s5, ',');
            if (strToNum(s1) == reqId)
            {
                getline(s, extraDetails[matchingRecords.size()]);
                m.id = strToNum(s1);
                m.price = strToNum(s4);
                m.quantity = strToNum(s5);
                matchingRecords.push_back(m);
            }
        }
        f.close();
        cout << "\n";
        cout << matchingRecords.size() << " matching record(s) found!\n";
        for (int i = 0; i < matchingRecords.size(); i++)
            matchingRecords[i].printDetailsFromHistory(extraDetails[i]);
    }
    return;
}
void medicine::removeMedicine()
{
    cout << "\nSearch for the medicine you want to remove.\n";
    getDetails();
    if (id == -1)
        return;
    hospital::medicinesList.erase(id);

    string s, temp;
    stringstream str;
    fstream f, fout;
    string reason;
    cout << "\nReason?\n";
    getline(cin >> ws, reason);
    str << name << "," << description << "," << price << "," << quantity << ",N,NA\n";
    getline(str, s);
    f.open("./data/medicinesHistory.csv", ios::in);
    fout.open("./data/temp.csv", ios::out);
    while (getline(f, temp))
    {
        if (temp == s)
        {
            fout << name << "," << description << "," << price << "," << quantity << ",Y," << reason << "\n";
        }
        else
            fout << temp << "\n";
    }
    f.close();
    fout.close();
    s.erase();
    temp.erase();
    remove("./data/medicinesHistory.csv");
    rename("./data/temp.csv", "./data/medicinesHistory.csv");
    cout << name << " removed successfully!\n";
    return;
}

// Path: SElab/src/ambulance.cpp