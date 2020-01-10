#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;
const double ESPe = 1e-12;

class Gauss {
protected:
    vector<vector<double>> matrixVectors;
    vector<unsigned int> swapStack;
    vector<double> result;
public:
    enum N {ROW, COL, FULL, BASIC};
    unsigned int matrixSize;
    Gauss();
    Gauss(string fileName);
    Gauss(const Gauss &objectToClone);
    bool fileRead(string fileName);
    bool printMatrix();
    bool diagonalCheckZero(unsigned int index);
    double matrixFactor(unsigned int index, unsigned int offsetIndex);
    void rowCalculation(unsigned int index);
    bool gauss(N e);
    void printResult(bool successSwitch);
    void maxElementInRow(unsigned int index);
    void maxElementInColumn(unsigned int index);
    void maxElementInMatrix(unsigned int index);
    void printSolve(N e);
};

int main()
{
    Gauss g1 = Gauss("test5.csv");
    Gauss g2 = Gauss(g1),g3 = Gauss(g1),g4 = Gauss(g1);
    g1.printSolve(Gauss::BASIC);
    g2.printSolve(Gauss::ROW);
    g3.printSolve(Gauss::COL);
    g4.printSolve(Gauss::FULL);
    return 0;
}

Gauss::Gauss(string fileName) {
    if(!fileRead(fileName)) {
        cout << "Uruchamiam konstruktor domyslny!\n";
        *this = Gauss();
    }
}
Gauss::Gauss() {
    this->matrixSize = 3;
    for( int i = 0; i < matrixSize; i++) {
        vector<double> temp;
        for( int j=0; j <= matrixSize; j++)
                temp.push_back(j);
        matrixVectors.push_back(temp);
        result.assign(matrixSize,0);
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
    for ( int i = 0; i < matrixSize; i++ )  {
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
    result.assign(matrixSize,0);
    return true;
}
bool Gauss::printMatrix() {
    if( this->matrixVectors.empty() ) {
        cout << "Macierz jest pusta! Nie mam co wypisac!\n";
        return false;
    }
    for(int i=0; i<matrixSize; i++) {
        for(int j=0; j<matrixSize; j++) {
            cout << left << setw(15) << setfill(' ') << matrixVectors[i][j];
        }
        cout << left << setw(7) << setfill(' ') <<"|"<< left << setw(7) << setfill(' ')<<matrixVectors[i][matrixSize]<<"\n";
    }
    return true;
}
Gauss::Gauss(const Gauss &objectToClone) {
    this->matrixSize = objectToClone.matrixSize;
    this->matrixVectors = objectToClone.matrixVectors;
    this->result = objectToClone.result;
}
bool Gauss::diagonalCheckZero(unsigned int index) {
    return (fabs(matrixVectors[index][index]) < ESPe) ? true : false;
}
double Gauss::matrixFactor(unsigned int index, unsigned int offsetIndex) {
    return matrixVectors[index+1+offsetIndex][index]/matrixVectors[index][index];
}
void Gauss::rowCalculation(unsigned int index) {
    for( int i=index; i<matrixSize-1 ;i++ ) {
            double tempFactor = matrixFactor(index,i-index);
            for(int j=index+1; j<=matrixSize; j++)
                matrixVectors[i+1][j] = matrixVectors[i+1][j] - tempFactor * matrixVectors[index][j];
            for( int j=0; j<=index; j++)
                matrixVectors[i+1][j] = 0;
    }
}
void Gauss::printResult(bool successSwitch){
    if(successSwitch) {
        for(int i=matrixSize-1; i>=0; i--)  {
            double sum = 0;
            for(int j=matrixSize-1; j>i; j--)
                sum = sum+result[j] * matrixVectors[i][j];
            result[i] = (-sum + matrixVectors[i][matrixSize]) / matrixVectors[i][i];
            if(fabs(result[i])<ESPe)
                result[i] = 0;
        }
        if(result.empty())
            cout<<"Zbior wynikow jest pusty!\n";
        else {
            while(!swapStack.empty()) {
                swap(result[swapStack.back()],result[swapStack.back()-1]);
                swapStack.pop_back(); swapStack.pop_back();
            }
            cout<<"\nWynik:\n";
            for(int i=0; i<result.size(); i++) {
                cout<<"X"<< left << setw(6) << setfill(' ')<<i<<result[i]<<"\n";
            }   cout<<"\n";
        }
    } else cout<<"Macierz osobliwa!\n";
}
void Gauss::maxElementInRow(unsigned int index)
{
    double tempMaxElement = fabs(matrixVectors[index][index]);
    unsigned int swapIndex = index;
    for(int i=index+1;i<matrixSize;i++)
        if(tempMaxElement < fabs(matrixVectors[index][i]))    {
            tempMaxElement = fabs(matrixVectors[index][i]);
            swapIndex = i;
        }
    if(index != swapIndex) {
        swapStack.push_back(index);
        swapStack.push_back(swapIndex);
        for(int i=0; i<matrixSize; i++)
            swap(matrixVectors[i][swapIndex],matrixVectors[i][index]);
    }
}
void Gauss::maxElementInColumn(unsigned int index)
{
    double tempMaxElement = fabs(matrixVectors[index][index]);
    unsigned int swapIndex = index;
    for(int i=index+1;i<matrixSize;i++)
        if(tempMaxElement<fabs(matrixVectors[i][index])) {
            tempMaxElement = fabs(matrixVectors[i][index]);
            swapIndex = i;
        }
    if(index != swapIndex)
        for(int i=index; i<=matrixSize; i++)
            swap(matrixVectors[swapIndex][i],matrixVectors[index][i]);
}
void Gauss::maxElementInMatrix(unsigned int index)
{
    double tempMaxElement = fabs(matrixVectors[index][index]);
    unsigned int swapIndexJ = index;
    unsigned int swapIndexI = index;
    for(int i=index;i<matrixSize;i++)
        for(int j=index;j<matrixSize;j++)
            if(tempMaxElement<fabs(matrixVectors[i][j])) {
                tempMaxElement = fabs(matrixVectors[i][j]);
                swapIndexJ = j;
                swapIndexI = i;
            }
    if(index != swapIndexI)
        for(int i=index; i<=matrixSize; i++)
            swap(matrixVectors[swapIndexI][i],matrixVectors[index][i]);
    if(index != swapIndexJ) {
        swapStack.push_back(index);
        swapStack.push_back(swapIndexJ);
        for(int i=0; i<matrixSize; i++)
            swap(matrixVectors[i][swapIndexJ],matrixVectors[i][index]);
    }
}
bool Gauss::gauss(N e) {
    typedef void( Gauss::*functor)(unsigned int);
    functor sortFunction;
    switch(e)   {
        case ROW:
            sortFunction = &Gauss::maxElementInRow;
            break;
        case COL:
            sortFunction = &Gauss::maxElementInColumn;
            break;
        case FULL:
            sortFunction = &Gauss::maxElementInMatrix;
            break;
        default:
            for(int i=0; i<matrixSize-1; i++)
                if(!diagonalCheckZero(i))
                    rowCalculation(i);
                else
                    return false;
            return true;
            break;
    }
    for(int i=0; i<matrixSize-1; i++) {
        (this->*sortFunction)(i);
        if(!diagonalCheckZero(i))
            rowCalculation(i);
        else
            return false;
    }
    return true;
}
void Gauss::printSolve(N e) {
    string temp = "";
    switch(e)   {
        case ROW:
            temp = "Eliminacja Gaussa z wyborem elementu podstawowego w wierszu:\n";
            break;
        case COL:
            temp = "Eliminacja Gaussa z wyborem elementu podstawowego w kolumnie:\n";
            break;
        case FULL:
            temp = "Eliminacja Gaussa z pelnym wyborem elementu podstawowego:";
            break;
        default:
            temp = "Podstawowa eliminacja Gaussa:";
            break;
    }
    cout<<temp;
    cout<<"\nMacierz wczytana:\n";
    printMatrix();
    printResult(gauss(e));
    cout<<"Macierz przeksztalcona:\n";
    printMatrix();
    for(int i=0;i<(matrixSize+1)*15;i++)
        cout<<'-';
    cout<<'\n';
}


