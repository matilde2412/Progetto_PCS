#ifndef __TEST_EMPTY_H
#define __TEST_EMPTY_H

#include <gtest/gtest.h>

#include "empty_class.hpp"

using namespace testing;

TEST(Test, ImportPuntiTest)
{
    std::string importFile = "C:/PCS2023_Exercises/Projects/Raffinamento/Dataset/Test1/Cell0Ds.csv";

    vector<ProjectLibrary::Punto> vettorePunti;
    bool importSuccess = ProjectLibrary::ImportPunti(importFile,vettorePunti);

    ASSERT_TRUE(importSuccess);
}

TEST(Test, ImportLatiTest)
{
    std::string importFileLati = "C:/PCS2023_Exercises/Projects/Raffinamento/Dataset/Test1/Cell1Ds.csv";
    vector<ProjectLibrary::Lato> vettoreLati;
    bool importSuccess = ProjectLibrary::ImportLati(importFileLati,vettoreLati);

    ASSERT_TRUE(importSuccess);
}

TEST(Test, ImportTriangoliTest)
{
    std::string importFilePunti = "C:/PCS2023_Exercises/Projects/Raffinamento/Dataset/Test1/Cell0Ds.csv";
    vector<ProjectLibrary::Punto> vettorePunti;
    ProjectLibrary::ImportPunti(importFilePunti,vettorePunti);

    std::string importFileLati = "C:/PCS2023_Exercises/Projects/Raffinamento/Dataset/Test1/Cell1Ds.csv";
    vector<ProjectLibrary::Lato> vettoreLati;
    ProjectLibrary::ImportLati(importFileLati,vettoreLati);

    std::string importFileTriangoli = "C:/PCS2023_Exercises/Projects/Raffinamento/Dataset/Test1/Cell2Ds.csv";
    vector<ProjectLibrary::Triangolo> vettoreTriangoli;
    vector<pair<int,double>> vettoreIdArea;
    bool importSuccess = ProjectLibrary::ImportTriangoli(importFileTriangoli,vettoreTriangoli,vettoreLati,vettorePunti,vettoreIdArea);

    ASSERT_TRUE(importSuccess);
}

TEST (Test, TestPuntoMedio)
{
    ProjectLibrary::Punto p1(0,0,1);
    ProjectLibrary::Punto p2(1,0,0);
    vector<ProjectLibrary::Punto> vettorePunti={p1,p2};
    unsigned int zero = 0, uno = 1, due = 2;
    ProjectLibrary::Lato l(zero,zero,uno);
    ProjectLibrary::Punto pMedio = ProjectLibrary::PuntoMedio(vettorePunti, l);
    ProjectLibrary::Punto expectpMedio(vettorePunti.size(),0.5,1);
    EXPECT_EQ(pMedio,expectpMedio);
}

TEST(Test, TestLunghezzaLato)
{
    ProjectLibrary::Punto p1(0,0,1);
    ProjectLibrary::Punto p2(1,0,0);
    vector<ProjectLibrary::Punto> vettorePunti={p1,p2};
    unsigned int zero = 0, uno = 1, due = 2;
    ProjectLibrary::Lato l(zero,zero,uno);
    double lunghezza = ProjectLibrary::LunghezzaLato(vettorePunti,l);
    double expectLunghezza = 1;
    EXPECT_EQ(lunghezza,expectLunghezza);
}

TEST (Test, TestLatoMaggiore)
{
    ProjectLibrary::Punto p1(0,0,1);
    ProjectLibrary::Punto p2(1,0,0);
    ProjectLibrary::Punto p3(2,1,0);
    vector<ProjectLibrary::Punto> vettorePunti={p1,p2,p3};
    unsigned int zero = 0, uno = 1, due = 2;
    ProjectLibrary::Lato l1(zero,zero,uno);
    ProjectLibrary::Lato l2(uno,uno,due);
    ProjectLibrary::Lato l3(due,due,zero);
    vector<ProjectLibrary::Lato> vettoreLati={l1,l2,l3};
    ProjectLibrary::Triangolo t(0,l1,l2,l3);
    unsigned int latoMaggiore = ProjectLibrary::IdLatoMaggiore(vettoreLati,vettorePunti,t);
    unsigned int expected = 2;
    EXPECT_EQ(latoMaggiore,expected);
}

TEST(Test, TestAreaTriangolo)
{
    ProjectLibrary::Punto a(0,0,1);
    ProjectLibrary::Punto b(1,0,0);
    ProjectLibrary::Punto c(2,1,0);
    vector<ProjectLibrary::Punto> vettorePunti={a,b,c};
    unsigned int zero = 0, uno = 1, due = 2;
    ProjectLibrary::Lato l1(zero,zero,uno);
    ProjectLibrary::Lato l2(uno,uno,due);
    ProjectLibrary::Lato l3(due,due,zero);
    ProjectLibrary::Triangolo t(0,l1,l2,l3);
    double area = ProjectLibrary::AreaTriangolo(vettorePunti,t);
    double expected = 0.5;
    EXPECT_EQ(area,expected);
}
TEST(Test,TestPuntoOpposto)
{
    ProjectLibrary::Punto p1(0,0,1);
    ProjectLibrary::Punto p2(1,0,0);
    ProjectLibrary::Punto p3(2,1,0);
    vector<ProjectLibrary::Punto> vettorePunti={p1,p2,p3};
    unsigned int zero = 0, uno = 1, due = 2;
    ProjectLibrary::Lato l1(zero,zero,uno);
    ProjectLibrary::Lato l2(uno,uno,due);
    ProjectLibrary::Lato l3(due,due,zero);
    vector<ProjectLibrary::Lato> vettoreLati={l1,l2,l3};
    ProjectLibrary::Triangolo t(0,l1,l2,l3);
    unsigned int latoMaggiore = ProjectLibrary::IdLatoMaggiore(vettoreLati,vettorePunti,t);
    unsigned int pOpposto = ProjectLibrary::IdPuntoOpposto(t,vettoreLati[latoMaggiore]);
    unsigned int expected = 1;
    EXPECT_EQ(pOpposto,expected);
}
TEST(Test,TestIdAdiacente)
{
    ProjectLibrary::Punto p1(0,0,1);
    ProjectLibrary::Punto p2(1,0,0);
    ProjectLibrary::Punto p3(2,1,0);
    ProjectLibrary::Punto p4(3,1,1);
    ProjectLibrary::Punto p5(4,1,-1);
    vector<ProjectLibrary::Punto> vettorePunti={p1,p2,p3,p4,p5};
    unsigned int zero = 0, uno = 1, due = 2, tre = 3, quattro = 4, cinque = 5, sei = 6;
    ProjectLibrary::Lato l1(zero,zero,uno);
    ProjectLibrary::Lato l2(uno,uno,due);
    ProjectLibrary::Lato l3(due,due,zero);
    ProjectLibrary::Lato l4(tre,zero,tre);
    ProjectLibrary::Lato l5(quattro,due,tre);
    ProjectLibrary::Lato l6(cinque,uno,quattro);
    ProjectLibrary::Lato l7(sei,due,quattro);
    vector<ProjectLibrary::Lato> vettoreLati={l1,l2,l3,l4,l5,l6,l7};
    ProjectLibrary::Triangolo t1(0,l1,l2,l3);
    ProjectLibrary::Triangolo t2(1,l3,l4,l5);
    ProjectLibrary::Triangolo t3(2,l2,l6,l7);
    vector<ProjectLibrary::Triangolo> vettoreTriangoli={t1,t2,t3};
    unsigned int tAdiacente = ProjectLibrary::IdAdiacente(t1,vettoreTriangoli, vettoreLati,vettorePunti);
    unsigned int expected = 1;
    EXPECT_EQ(tAdiacente,expected);

}
TEST(Test, TestMergeSort)
{
    vector<pair<int, double>> v = {{0,0.75},{1,0.03},{2,0.27},{3,0.54}};
    ProjectLibrary::MergeSort(v, 0, v.size()-1);
    vector<pair<int, double>> expected = {{0,0.75},{3,0.54},{2,0.27},{1,0.03}};
    EXPECT_EQ(v, expected);
}

TEST(Test, TestDivisioneTriangolo)
{
    ProjectLibrary::Punto p1(0,0,1);
    ProjectLibrary::Punto p2(1,0,0);
    ProjectLibrary::Punto p3(2,1,0);
    ProjectLibrary::Punto p4(3,1,1);
    ProjectLibrary::Punto p5(4,1,-1);
    vector<ProjectLibrary::Punto> vettorePunti={p1,p2,p3,p4,p5};
    int t0 = 0;
    unsigned int zero = 0, uno = 1, due = 2, tre = 3, quattro = 4, cinque = 5, sei = 6;
    ProjectLibrary::Lato l1(0,zero,uno);
    ProjectLibrary::Lato l2(1,uno,due);
    ProjectLibrary::Lato l3(2,due,zero);
    ProjectLibrary::Lato l4(3,zero,tre);
    ProjectLibrary::Lato l5(4,due,tre);
    ProjectLibrary::Lato l6(5,uno,quattro);
    ProjectLibrary::Lato l7(6,due,quattro);
    vector<ProjectLibrary::Lato> vettoreLati={l1,l2,l3,l4,l5,l6,l7};
    ProjectLibrary::Triangolo t1(0,l1,l2,l3);
    ProjectLibrary::Triangolo t2(1,l3,l4,l5);
    ProjectLibrary::Triangolo t3(2,l2,l6,l7);
    vector<ProjectLibrary::Triangolo> vettoreTriangoli={t1,t2,t3};
    vector<pair<int, double>> vettoreIdArea = {{0,0.5},{1,0.5},{2,0.5}};
    ProjectLibrary::DivisioneTriangolo(vettorePunti,vettoreLati,vettoreTriangoli,vettoreIdArea,t0);
    ProjectLibrary::Punto p6(5,0.5,0.5);
    vector<ProjectLibrary::Punto> expectedPunti={p1,p2,p3,p4,p5,p6};
    ProjectLibrary::Lato l8(7,due,cinque);
    ProjectLibrary::Lato l9(8,cinque,zero);
    ProjectLibrary::Lato l10(9,uno,cinque);
    vector<ProjectLibrary::Lato> expectedLati={l1,l2,l3,l4,l5,l6,l7,l8,l9,l10};
    ProjectLibrary::Triangolo t4(3,l1,l10,l9);
    ProjectLibrary::Triangolo t5(4,l2,l10,l8);
    vector<ProjectLibrary::Triangolo> expectedTriangoli={t1,t2,t3,t4,t5};
    EXPECT_EQ(vettorePunti, expectedPunti);
    EXPECT_EQ(vettoreLati, expectedLati);
    EXPECT_EQ(vettoreTriangoli, expectedTriangoli);
}

TEST(Test, TestDivisioneTriangolo2)
{
    ProjectLibrary::Punto p1(0,0,1);
    ProjectLibrary::Punto p2(1,0,0);
    ProjectLibrary::Punto p3(2,1,0);
    ProjectLibrary::Punto p4(3,0.5,0.5);
    vector<ProjectLibrary::Punto> vettorePunti={p1,p2,p3,p4};
    int t0 = 0;
    unsigned int zero = 0, uno = 1, due = 2, tre = 3, quattro = 4, cinque = 5, sei = 6;
    ProjectLibrary::Lato l1(0,zero,uno);
    ProjectLibrary::Lato l2(1,uno,due);
    ProjectLibrary::Lato l3(2,due,zero);
    ProjectLibrary::Lato l4(3,due,tre);
    ProjectLibrary::Lato l5(4,tre,zero);
    vector<ProjectLibrary::Lato> vettoreLati={l1,l2,l3,l4,l5};
    vector<unsigned int> latispezzati = {tre,quattro};
    ProjectLibrary::Triangolo t1(0,l1,l2,l3);
    vector<ProjectLibrary::Triangolo> vettoreTriangoli={t1};
    vector<pair<int, double>> vettoreIdArea = {{0,0.5}};
    ProjectLibrary::DivisioneTriangolo2(vettorePunti,vettoreLati,vettoreTriangoli,vettoreIdArea,due,t0,uno,tre,latispezzati);
    ProjectLibrary::Lato l6(5,uno,tre);
    vector<ProjectLibrary::Lato> expectedLati={l1,l2,l3,l4,l5,l6};
    ProjectLibrary::Triangolo t2(1,l2,l6,l4);
    ProjectLibrary::Triangolo t3(2,l1,l6,l5);
    vector<ProjectLibrary::Triangolo> expectedTriangoli={t1,t2,t3};
    EXPECT_EQ(vettoreLati, expectedLati);
    EXPECT_EQ(vettoreTriangoli, expectedTriangoli);
}

#endif // __TEST_EMPTY_H

