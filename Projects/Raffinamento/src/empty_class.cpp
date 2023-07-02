#include "empty_class.hpp"
#include <string>
#include <vector>
#include <utility>
namespace ProjectLibrary
{
    bool ImportPunti(const string& importFile,
                     vector<Punto>& vettorePunti)
    {
        ifstream file; //apro il file
        file.open(importFile);
        if(file.fail())
            return false;
        list<string> listLines; //conto le righe del file per capire il numero di punti per la dimensione del vettore
        string line;
        while (getline(file, line))
            listLines.push_back(line);
        listLines.pop_front();  //elimino prima riga di intestazione
        file.close();

        for (const string& line : listLines) //salvo i punti
        {
            istringstream converter(line);
            unsigned int marker; unsigned int id; double x; double y;
            converter>>id>>marker>>x>>y;
            vettorePunti.push_back(Punto(id,x,y));
        }
        return true;
    }

    bool ImportLati(const string &importFile,
                    vector<Lato>& vettoreLati)
    {
        ifstream file;  //apro il file
        file.open(importFile);
        if(file.fail())
            return false;
        list<string> listLines; //conto le righe del file per capire il numero di punti per la dimensione del vettore
        string line;
        while (getline(file, line))
            listLines.push_back(line);
        listLines.pop_front();
        file.close();

        for (const string& line : listLines) //salvo i lati
        {
            unsigned int inizio; unsigned int fine; unsigned int marker; unsigned int id;
            istringstream converter(line);
            converter>>id>>marker>>inizio>>fine;
            vettoreLati.push_back(Lato(id,inizio,fine)); //aggiungiamo il lato al vettore di lati
        }
        return true;
    }

    bool ImportTriangoli(const string &importFile,
                         vector<Triangolo>& vettoreTriangoli,
                         vector<Lato>& vettoreLati,
                         vector<Punto>& vettorePunti,
                         vector<pair<int,double>>& vettoreIdArea)
    {
        ifstream file; //apro il file
        file.open(importFile);
        if(file.fail())
            return false;
        list<string> listLines; //conto le righe del file per capire il numero di punti per la dimensione del vettore
        string line;
        while (getline(file, line))
            listLines.push_back(line);
        listLines.pop_front();
        file.close();

        for (const string& line : listLines) //salvo i triangoli
        {
            istringstream converter(line);
            unsigned int lato1, lato2, lato3, vertici, id;
            converter>>id>>vertici>>vertici>>vertici>>lato1>>lato2>>lato3;
            vettoreTriangoli.push_back(Triangolo(id,vettoreLati[lato1],vettoreLati[lato2],vettoreLati[lato3]));
            Triangolo& triangolo = vettoreTriangoli[vettoreTriangoli.size()-1];
            vettoreIdArea.push_back(make_pair(id, AreaTriangolo(vettorePunti, triangolo)));
        }
        return true;
    }

    void DivisioneTriangolo(vector<Punto>& vettorePunti,
                            vector<Lato>& vettoreLati,
                            vector<Triangolo>& vettoreTriangoli,
                            vector<pair<int,double>>& vettoreIdArea,
                            int& idTMaggiore)
    {
        unsigned int idLatoMaggiore = IdLatoMaggiore(vettoreLati,vettorePunti,vettoreTriangoli[idTMaggiore]);
        vettorePunti.push_back(PuntoMedio(vettorePunti,vettoreLati[idLatoMaggiore]));
        Punto& puntoMedio = vettorePunti[vettorePunti.size()-1];

        vettoreLati.push_back(Lato(vettoreLati.size(), vettoreLati[idLatoMaggiore].idInizio, puntoMedio.id)); //creo nuovo lato spezzato (1)
        Lato& lato1 = vettoreLati[vettoreLati.size()-1];

        vettoreLati.push_back(Lato(vettoreLati.size(), puntoMedio.id, vettoreLati[idLatoMaggiore].idFine)); //creo nuovo lato spezzato (2)
        Lato& lato2 = vettoreLati[vettoreLati.size()-1];

        unsigned int idpuntoOpposto = IdPuntoOpposto(vettoreTriangoli[idTMaggiore], vettoreLati[idLatoMaggiore]);
        vector <unsigned int> latiSpezzati = {lato1.id,lato2.id};
        DivisioneTriangolo2(vettorePunti,vettoreLati,vettoreTriangoli,vettoreIdArea,idLatoMaggiore,idTMaggiore,idpuntoOpposto,puntoMedio.id,latiSpezzati);
    }

    void DivisioneTriangolo2(vector<Punto>& vettorePunti,
                             vector<Lato>& vettoreLati,
                             vector<Triangolo>& vettoreTriangoli,
                             vector<pair<int,double>>& vettoreIdArea,
                             unsigned int& idLatoPrec,
                             int& idt,  //triangolo da dividere: int perche gli passiamo a volte idadiacente che può essere -1
                             unsigned int& idpunto1,
                             unsigned int& idpunto2,
                             vector<unsigned int>& latiSpezzati)
    {
        vettoreLati.push_back(Lato(vettoreLati.size(),idpunto1 ,idpunto2)); //creo il lato tra i due punti medi, oppure tra il punto opposto e il punto medio
        Lato& latonuovo = vettoreLati[vettoreLati.size()-1];

        //creo i due triangoli
        unsigned int i = 0;
        while (vettoreTriangoli[idt].latiTriangolo[i] != idLatoPrec)
            i++;   //indice latoprec in latitriangolo (da 0 a 2)

        swap(vettoreTriangoli[idt].latiTriangolo[0],vettoreTriangoli[idt].latiTriangolo[i]);
        if(vettoreLati[vettoreTriangoli[idt].latiTriangolo[1]].idInizio == vettoreLati[idLatoPrec].idInizio || vettoreLati[vettoreTriangoli[idt].latiTriangolo[1]].idFine == vettoreLati[idLatoPrec].idInizio)
        {
            vettoreTriangoli.push_back(Triangolo(vettoreTriangoli.size(),latonuovo,vettoreLati[vettoreTriangoli[idt].latiTriangolo[1]],vettoreLati[latiSpezzati[0]]));
            Triangolo& triangolo1 = vettoreTriangoli[vettoreTriangoli.size()-1];
            vettoreIdArea.push_back(make_pair(triangolo1.id, AreaTriangolo(vettorePunti,triangolo1)));

            vettoreTriangoli.push_back(Triangolo(vettoreTriangoli.size(),latonuovo,vettoreLati[vettoreTriangoli[idt].latiTriangolo[2]],vettoreLati[latiSpezzati[1]]));
            Triangolo& triangolo2 = vettoreTriangoli[vettoreTriangoli.size()-1];
            vettoreIdArea.push_back(make_pair(triangolo2.id, AreaTriangolo(vettorePunti,triangolo2)));
        }
        else
        {
            vettoreTriangoli.push_back(Triangolo(vettoreTriangoli.size(),latonuovo,vettoreLati[vettoreTriangoli[idt].latiTriangolo[1]],vettoreLati[latiSpezzati[1]]));
            Triangolo& triangolo1 = vettoreTriangoli[vettoreTriangoli.size()-1];
            vettoreIdArea.push_back(make_pair(triangolo1.id, AreaTriangolo(vettorePunti,triangolo1)));

            vettoreTriangoli.push_back(Triangolo(vettoreTriangoli.size(),latonuovo,vettoreLati[vettoreTriangoli[idt].latiTriangolo[2]],vettoreLati[latiSpezzati[0]]));
            Triangolo& triangolo2 = vettoreTriangoli[vettoreTriangoli.size()-1];
            vettoreIdArea.push_back(make_pair(triangolo2.id, AreaTriangolo(vettorePunti,triangolo2)));
        }
        vettoreTriangoli[idt].flag = false;
    }

    void Fusione(vector<pair<int,double>>& vettoreIdArea,
               const unsigned int& sx,
               const unsigned int& cx,
               const unsigned int& dx)
    {
        unsigned int i = sx;
        unsigned int j = cx+1;
        vector<pair<int,double>> b;   //vettore di appoggio
        unsigned int k =0;
        while (i <= cx && j <= dx) //nei due gruppi che ho diviso devono esserci almeno 1 elem
        {
            if (vettoreIdArea[i].second >= vettoreIdArea[j].second)  //confronto i primi di ogni gruppo (i e j), il minore tra i due lo metto nel vettore di appoggio b
            {
                b.push_back(make_pair(vettoreIdArea[i].first,vettoreIdArea[i].second));
                i++;
            }
            else
            {
                b.push_back(make_pair(vettoreIdArea[j].first,vettoreIdArea[j].second));
                j++;
            }
            k++;
        }
        for(; i <= cx; i++, k++) //capisco se ho "svuotato uno dei due mazzi" ed entro in uno o nell'altro for per copiare i rimanenti elementi (che so gia essere ordinati)
            b.push_back(make_pair(vettoreIdArea[i].first,vettoreIdArea[i].second));
        for(; j <= dx; j++, k++)
            b.push_back(make_pair(vettoreIdArea[j].first,vettoreIdArea[j].second));

        for(i = sx; i <= dx; i++) //copio il vettore di appoggio b in v, visto che non sono gia alla fine (senno v[i]=b[i])
        {                          //devo fare volta per volta in base alla posiz dei sottogruppi considerati
            vettoreIdArea[i].first = b[i-sx].first;
            vettoreIdArea[i].second = b[i-sx].second;
        }
    }


    void MergeSort(vector<pair<int,double>>& vettoreIdArea,
                   const unsigned int& sx,
                   const unsigned int& dx)
    {
        unsigned int cx;
        if(sx < dx)
        {
            cx = (sx + dx)/2;
            MergeSort(vettoreIdArea,sx,cx);
            MergeSort(vettoreIdArea,cx+1,dx);
            Fusione(vettoreIdArea,sx,cx,dx);
        }
    }

    void Raffinamento(vector<Punto>& vettorePunti,
                      vector<Lato>& vettoreLati,
                      vector<Triangolo>& vettoreTriangoli,
                      vector<pair<int,double>>& vettoreIdArea)
    {
        for (unsigned int j = 0; j<500;j++)
        {
            MergeSort(vettoreIdArea,0,vettoreIdArea.size()-1);
            unsigned int z = 0; //trovo triangolo maggiore con flag attiva
            while(vettoreTriangoli[vettoreIdArea[z].first].flag == false)
                z++;
            int idCorrente = vettoreIdArea[z].first; //id del triangolo corrente

            DivisioneTriangolo(vettorePunti,vettoreLati,vettoreTriangoli,vettoreIdArea,idCorrente);

            int idAdiacente = IdAdiacente(vettoreTriangoli[idCorrente],vettoreTriangoli,vettoreLati,vettorePunti);
            bool controllo = false; //serve per fermarci quando i due lati maggiori di tMagg e tAdiac sono coincidenti
            unsigned int idLatoPrec = IdLatoMaggiore(vettoreLati,vettorePunti,vettoreTriangoli[idCorrente]); //qst var la aggiorno ogni iterazione in cui guardo triang adiac
            //aggiorno latiSpezzati perche dopo DivisoneT dentro DivisioneT2 ho aggiunto lato nuovo, scalo indietro di una posiz
            vector<unsigned int> latiSpezzati = {static_cast<unsigned int>(vettoreLati.size()) - 3, static_cast<unsigned int>(vettoreLati.size()) - 2};  //il primo ha inizio coincidente con l maggiore, il secondo con fine di l maggiore

            while (idAdiacente != -1 && controllo == false) //verifico che tMaggiore abbia triangolo adiacente e non sia al bordo
            {
                //caso in cui i lati maggiori di tcorrente e tadiac coincidono, creo lato tra punto medio e punto opp di tadiac 
                //divisione in 2 sottotriangoli
                if (idLatoPrec == IdLatoMaggiore(vettoreLati,vettorePunti,vettoreTriangoli[idAdiacente]))
                {
                    unsigned int idpuntoOpp = IdPuntoOpposto(vettoreTriangoli[idAdiacente],vettoreLati[idLatoPrec]);
                    unsigned int idpuntoM = vettorePunti.size()-1; //è ultimo aggiunto
                    DivisioneTriangolo2(vettorePunti,vettoreLati,vettoreTriangoli,vettoreIdArea,idLatoPrec,idAdiacente,idpuntoOpp, idpuntoM, latiSpezzati);
                    controllo = true;
                }
                else //richiamo funz su triangolo adiacente, prima divisione 'classica' poi l'altra
                     //divisione in 3 sottotriangoli
                {
                    DivisioneTriangolo(vettorePunti,vettoreLati,vettoreTriangoli,vettoreIdArea,idAdiacente);

                    //cerco quale dei due triangoli aggiunti nuovi ha come lato il latomaggiore del triang precedente (usando var idLatoPrec) e gli applico la divisione
                    int t;
                    for (unsigned int v=0; v<3; v++)
                    {
                        if (vettoreTriangoli[vettoreTriangoli.size()-1].latiTriangolo[v] == idLatoPrec)
                            t = vettoreTriangoli.size()-1;
                        if(vettoreTriangoli[vettoreTriangoli.size()-2].latiTriangolo[v] == idLatoPrec)
                            t = vettoreTriangoli.size()-2;
                    }

                    unsigned int idpunto1 = vettorePunti.size()-2, idpunto2 = vettorePunti.size()-1;
                    DivisioneTriangolo2(vettorePunti,vettoreLati,vettoreTriangoli,vettoreIdArea,idLatoPrec,t,idpunto1, idpunto2, latiSpezzati);


                }
                //aggiornamento variabili:
                idCorrente = idAdiacente; //il vecchio triangolo adiacente sarà quello su cui applicare le funzioni al prossimo ciclo
                idAdiacente = IdAdiacente(vettoreTriangoli[idCorrente],vettoreTriangoli,vettoreLati,vettorePunti);
                idLatoPrec = IdLatoMaggiore(vettoreLati,vettorePunti,vettoreTriangoli[idCorrente]);
                //scalo di sue posiz perche nel caso dell'else è come se DivisioneT2 avvenisse due volte, aggiungendo i due lati nuovi
                latiSpezzati = {static_cast<unsigned int>(vettoreLati.size()) - 4, static_cast<unsigned int>(vettoreLati.size()) - 3};

            }
        }
    }

    bool ExportPunti(vector<Punto>& vettorePunti)
    {
        ofstream file("C:/Users/Asus/Desktop/PCS2023_Exercises/Projects/Raffinamento/PuntiRaffinamento.csv");
        if (file.is_open())
        {
            for (unsigned int i=0; i<vettorePunti.size();i++)
                file << vettorePunti[i].x << ";"<< vettorePunti[i].y <<"\n";
            file.close();
            return true;
        }
        else
            return false;
    }

    bool ExportLati(vector<Lato>& vettoreLati,vector<Punto>& vettorePunti)
    {
        ofstream file("C:/Users/Asus/Desktop/PCS2023_Exercises/Projects/Raffinamento/LatiRaffinamento.csv");
        if (file.is_open())
        {
            for (unsigned int i=0; i<vettoreLati.size();i++)
                file << vettorePunti[vettoreLati[i].idInizio].x <<";"<< vettorePunti[vettoreLati[i].idInizio].y<< ";"<< vettorePunti[vettoreLati[i].idFine].x << ";" << vettorePunti[vettoreLati[i].idFine].y <<"\n";
            file.close();
            return true;
        }
        else
            return false;

    }
}
