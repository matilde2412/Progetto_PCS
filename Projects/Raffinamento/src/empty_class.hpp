#ifndef __EMPTY_H
#define __EMPTY_H

#include <iostream>
#include "Eigen/Eigen"
#include <fstream>
#include "list"
#include <ostream>
#include "map"
#include <sstream>
#include <vector>
#include <string>
#include <list>
using namespace Eigen;
using namespace std;

namespace ProjectLibrary
{
    class Punto
    {
    public:
        unsigned int id;
        double x;
        double y;

        //costruttori:
        Punto(const unsigned int& id,const double& x, const double& y):
          id(id), x(x), y(y)
        {
        }

        Punto(const Punto& p):  //costruttore copia
           Punto(p.id,p.x, p.y)
        {
        }

        //operatori
        inline bool operator == (const Punto& other) const
        {
            return (id == other.id);
        }
        inline bool operator != (const Punto& other) const
        {
            return !(id == other.id);
        }
/*
        Punto &operator=(const Punto& p)  //operatore assegnazione (utile in altri possibili casi)
        {
            id = p.id;
            x = p.x;
            y = p.y;
            return *this;
        }*/

    };

    class Lato
    {
    public:
        unsigned int id;
        unsigned int idInizio;
        unsigned int idFine;

        //costruttori:
        Lato(const unsigned int& id, unsigned int& idInizio, unsigned int& idFine):
        id(id), idInizio(idInizio), idFine(idFine)
        {
        }

        Lato(const Lato& l):  //costruttore copia
            id(l.id),idInizio(l.idInizio),idFine(l.idFine)
        {
        }

        //operatori
        inline bool operator  == (const Lato& other) const
        {
            return (id == other.id);
        }

        inline bool operator  != (const Lato& other) const
        {
            return !(id == other.id);
        }
        /*
        Lato &operator=(const Lato& l)  //operatore assegnazione (utile in altri possibili casi)
        {
            id=l.id;
            idInizio = l.idInizio;
            idFine = l.idFine;
            return *this;
        }*/

    };

    class Triangolo
    {
    public:
        unsigned int id;
        bool flag;
        vector<unsigned int> puntiTriangolo; //vettore degli id dei punti e lati
        vector<unsigned int> latiTriangolo;

        //costruttori:
        Triangolo(unsigned int id, Lato& l1, Lato& l2, Lato& l3):
            id(id), flag(true)
        {
            latiTriangolo.push_back(l1.id);
            latiTriangolo.push_back(l2.id);
            latiTriangolo.push_back(l3.id);

            puntiTriangolo.push_back(l1.idInizio);
            puntiTriangolo.push_back(l1.idFine);

            if (l2.idInizio != l1.idInizio && l2.idInizio != l1.idFine)
                puntiTriangolo.push_back(l2.idInizio);
            else
                puntiTriangolo.push_back(l2.idFine);
        }

        Triangolo(const Triangolo& t, const vector<Lato>& vettoreLati)  //costruttore copia
        {
            Lato l1 = vettoreLati[t.latiTriangolo[0]];
            Lato l2 = vettoreLati[t.latiTriangolo[1]];
            Lato l3 = vettoreLati[t.latiTriangolo[2]];
            Triangolo(t.id,l1,l2,l3);
        }

        //operatori
        inline bool operator == (const Triangolo& other) const
        {
            return (id == other.id);
        }
        inline bool operator != (const Triangolo& other) const
        {
            return !(id == other.id);
        }
        /*
        Triangolo &operator=(const Triangolo& t)  //operatore assegnazione (utile in altri possibili casi)
        {
            id=t.id;
            puntiTriangolo = t.puntiTriangolo;
            latiTriangolo = t.latiTriangolo;
            flag = t.flag;
            return *this;
        }*/
    };

    //funzioni inline
    inline double LunghezzaLato(vector<Punto>& vettorePunti,
                        Lato& lato)
    {
        return sqrt(pow(vettorePunti[lato.idInizio].x - vettorePunti[lato.idFine].x,2) + pow(vettorePunti[lato.idInizio].y - vettorePunti[lato.idFine].y,2));
    }

    inline Punto PuntoMedio(vector<Punto>& vettorePunti,
                            Lato& lato)
    {
        return Punto(vettorePunti.size(),(vettorePunti[lato.idInizio].x + vettorePunti[lato.idFine].x)/2, (vettorePunti[lato.idInizio].y + vettorePunti[lato.idFine].y)/2);
    }

    inline double AreaTriangolo(vector<Punto>& vettorePunti,
                        Triangolo& triangolo)
    {
        vector<double> c; //creiamo variabile che potevamo evitare di creare ma sarebbe diventato molto brutto da leggere, quindi la lasciamo
        for (unsigned int i=0;i<3;i++)
        {
            c.push_back(vettorePunti[triangolo.puntiTriangolo[i]].x);
            c.push_back(vettorePunti[triangolo.puntiTriangolo[i]].y);

        }
        return 0.5*abs(c[0]*c[3] + c[2]*c[5] + c[4]*c[1] - c[2]*c[1] - c[4]*c[3] - c[0]*c[5]);
    }

    inline unsigned int IdLatoMaggiore(vector<Lato>& vettoreLati,
                                vector<Punto>& vettorePunti,
                                Triangolo& triangolo)
    {
        unsigned int max = 0;
        for(unsigned int i=1; i<3;i++)
        {
            if(LunghezzaLato(vettorePunti,vettoreLati[triangolo.latiTriangolo[i]]) > LunghezzaLato(vettorePunti,vettoreLati[triangolo.latiTriangolo[max]]))
                max = i;
        }
        return triangolo.latiTriangolo[max];
    }

    inline unsigned int IdPuntoOpposto(Triangolo& t,
                                Lato& latoMaggiore)
    {
        for(unsigned int id : t.puntiTriangolo) //cerco il punto opposto al lato maggiore (se non è ne inizio ne fine del lato maggiore allora è il punto giusto)
        {
            if(id != latoMaggiore.idInizio && id != latoMaggiore.idFine)
                return id;  //se trovo il punto esco dal ciclo for
        }
        throw(runtime_error("Errore ricerca punto opposto"));
    }

    inline int IdAdiacente(Triangolo& t,
                    vector<Triangolo>& vettoreTriangoli,
                    vector<Lato>& vettoreLati,
                    vector<Punto>& vettorePunti)
    {
        unsigned int idLatoMaggiore = IdLatoMaggiore(vettoreLati,vettorePunti,t);
        int tAdiacente = -1;
        for (unsigned int k=0; k<vettoreTriangoli.size(); k++)
        {
            for(unsigned int j = 0; j<3; j++)
            {
                if(vettoreTriangoli[k].latiTriangolo[j] == idLatoMaggiore && vettoreTriangoli[k].id != t.id && vettoreTriangoli[k].flag == true)
                    tAdiacente = k;
            }
        }
        return tAdiacente;
    }

    //definizioni funzioni hpp
    bool ImportPunti(const string& importFile, vector<Punto>& vettorePunti);
    bool ImportLati(const string& importFile, vector<Lato>& vettoreLati);
    bool ImportTriangoli(const string& importFile, vector<Triangolo>& vettoreTriangoli, vector<Lato>& vettoreLati,vector<Punto>& vettorePunti,vector<pair<int,double>>& vettoreIdArea);
    void MergeSort(vector<pair<int,double>>& vettoreIdArea, const unsigned int& sx, const unsigned int& dx);
    void Fusione(vector<pair<int,double>>& vettoreIdArea, const unsigned int& sx, const unsigned int& cx,const unsigned int& dx);
    void DivisioneTriangolo(vector<Punto>& vettorePunti,vector<Lato>& vettoreLati,vector<Triangolo>& vettoreTriangoli,vector<pair<int,double>>& vettoreIdArea, int& idTMaggiore);
    void DivisioneTriangolo2(vector<Punto>& vettorePunti, vector<Lato>& vettoreLati, vector<Triangolo>& vettoreTriangoli, vector<pair<int,double>>& vettoreIdArea, unsigned int& idLatoPrec, int& idt, unsigned int& idpunto1, unsigned int& idpunto2, vector<unsigned int>& latiSpezzati);
    void Raffinamento(vector<Punto>& vettorePunti, vector<Lato>& vettoreLati, vector<Triangolo>& vettoreTriangoli, vector<pair<int,double>>& vettoreIdArea);
    bool ExportPunti(vector<Punto>& vettorePunti);
    bool ExportLati(vector<Lato>& vettoreLati, vector<Punto>& vettorePunti);
}


#endif // __EMPTY_H
