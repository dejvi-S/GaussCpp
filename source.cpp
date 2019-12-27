#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class Gauss {
protected:
    vector<vector<double>> matrixVectors;

public:
    unsigned int matrixSize;
    Gauss();
    Gauss(string fileName);
    bool fileRead(string fileName);

};

int main()
{
    Gauss g1 = Gauss();
    Gauss g2 = Gauss("test.csv");
    return 0;
}
Gauss::Gauss(string fileName) {
    if(!fileRead(fileName)) {
        cout << "Uruchamiam konstruktor domyslny!\n";
        Gauss();
    }
}
Gauss::Gauss() {
    this->matrixSize = 3;
    for( int i = 0; i < matrixSize; i++) {
        vector<double> temp;
        for( int j=0; j <= matrixSize; j++)
                temp.push_back(j);
        this->matrixVectors.push_back(temp);
    }
}
bool Gauss::fileRead(string fileName) {
    ifstream sourceFile;
    sourceFile.open(fileName);
    if ( !sourceFile.is_open() ) {
        cout << "Plik nie zostal otwarty!\n";
        return false;
    }
    sourceFile >> matrixSize;
    char * charThrashCan = new char(0);
    for ( int i = 0; i <= matrixSize; i++ )
        sourceFile >> *charThrashCan;
    for ( int i = 0; i < matrixSize; i++ )
    {
        vector<double> temp;
        double * tempDouble = new double(0);
        for (int j = 0; j < matrixSize; j++)
        {
            sourceFile >> *tempDouble;
            temp.push_back(*tempDouble);
            sourceFile >> *charThrashCan;
        }
        sourceFile >> *charThrashCan;
        sourceFile >> *tempDouble;
        temp.push_back(*tempDouble);
        matrixVectors.push_back(temp);
        delete tempDouble;
    }
    sourceFile.close();
    return true;
}
