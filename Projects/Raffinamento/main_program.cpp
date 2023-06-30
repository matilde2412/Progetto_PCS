#include "empty_class.hpp"
#include <chrono>

int main()
{
    auto start = std::chrono::high_resolution_clock::now();

    vector<ProjectLibrary::Punto> vettorePunti;
    vector<ProjectLibrary::Lato> vettoreLati;
    vector<ProjectLibrary::Triangolo> vettoreTriangoli;
    vector<pair<int, double>> vettoreIdArea;

    ProjectLibrary::ImportPunti("C:/PCS2023_Exercises/Projects/Raffinamento/Dataset/Test1/Cell0Ds.csv", vettorePunti);
    ProjectLibrary::ImportLati("C:/PCS2023_Exercises/Projects/Raffinamento/Dataset/Test1/Cell1Ds.csv", vettoreLati);
    ProjectLibrary::ImportTriangoli("C:/PCS2023_Exercises/Projects/Raffinamento/Dataset/Test1/Cell2Ds.csv", vettoreTriangoli, vettoreLati,vettorePunti,vettoreIdArea);
    ProjectLibrary::Raffinamento(vettorePunti, vettoreLati, vettoreTriangoli, vettoreIdArea);
    ProjectLibrary::ExportPunti(vettorePunti);
    ProjectLibrary::ExportLati(vettoreLati,vettorePunti);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Tempo impiegato: " << duration.count() << " secondi" << std::endl;
    return 0;
}
