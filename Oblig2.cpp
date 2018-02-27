#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

//  INCLUDE:
#include  <iostream>         //  cin, cout
#include  <fstream>          //  ifstream, ofstream
#include  <cstring>          //  strcpy, strlen, strncmp
#include  <cctype>           //  toupper
#include  <cstdlib>          //  itoa

using namespace std;

//  CONST:
const int MAXANS  = 100;     //  Max.antall ansatte.
const int MAXBARN =  20;     //  Max.antall barn EN ansatt kan ha.
const int MAXTXT =   80;     //  Max.lengde for en tekst/streng.
const int DATOLEN =   7;     //  Fast lengde for en dato (inkl. '\0').

//  ENUM:
enum Kjonn { jente, gutt };

//  DEKLARASJON AV FUNKSJONER:
void skrivMeny();
char les();
int  les(char* t, int min, int max);
int  finnesAllerede(int ansNr);
void smettInn();
void lesFraFil();
void skrivTilFil();
void nyAnsatt();
void partnerEndring();
void nyttBarn();
void dataOmAnsatt();
void alleEttAar();
void fjernAnsatt();

//  KLASSER:
class Person  {                   //  Abstrakt baseklasse.
protected:
    char* fornavn;                //  Personens fornavn.
    char  fodselsdato[DATOLEN];   //  Dens f›dselsadato p† formen: ††mmdd
    
public:
    Person()  {
        char temp[MAXTXT + 1];	//	Lager temp for gitt tekst
        cout << "Skriv inn fornavn:\t";
        cin.getline(temp, MAXTXT);	// Leser inn i tempen
        fornavn = new char[strlen(temp) + 1];	//	Lager et fornavn med
        strcpy(fornavn, temp);						// noyaktig lengde
        cout << "Skriv inn din fodeslesdato:\t";
        cin.getline(fodselsdato, DATOLEN);
    }
    
    Person(ifstream & inn)  {
        char temp[MAXTXT + 1];					///<> // Leser /n i getline
        inn.getline(temp, MAXTXT);			///<>
        fornavn = new char[strlen(temp) + 1];
        strcpy(fornavn, temp);
        for(int i = 0; i < DATOLEN; i++)
            inn.getline(fodselsdato, DATOLEN);
    }
    
    ~Person()  {
        delete[] fornavn;
    }
    
    void skrivTilFil(ofstream & ut)  {
        ut << fornavn << '\n'
        << fodselsdato;
    }
    
    void display()	{
        cout << "fornavn:\t"<< fornavn;
        cout << "\nfodselsdato:\t" << fodselsdato;
    }
    
    //  ANDRE MEDLEMSFUNKSJONER .?
    
};


class Barn : public Person  {          //  Konkret klasse.
private:
    Kjonn kjonn;                       //  Barnets kj›nn (jente eller gutt).
    
public:
    Barn()  {
        char kj;
        cout << "Hvilket kjonn har barnet? J for jente G for gutt\t";
        kj = les();			//	Setter kjonnverdi
        kj == 'J' ? (kjonn = jente) : (kjonn = gutt);
    }
    
    Barn(ifstream & inn) : Person(inn)  {
        char kj;
        inn >> kj;			//Leser kjonnverdi fra fil
        kj == 'J' ? (kjonn = jente) : (kjonn = gutt);
    }
    
    void skrivTilFil(ofstream & ut)  {
                                                    //	Skriver kjonnverdi til fil
        (kjonn == jente) ? (ut << " J") : (ut << " G");
    }
    
    void display()  {
        Person::display();
        cout << "\nKjonnet er:\t" << kjonn;
    }
    
    bool likAar(char aaaa[])
    {						//	Sjekker om barnets fodselsdato sine 2 forste
        //	Siffre er lik de to siste i gitt aar
        return ((fodselsdato[0] == aaaa[2]) && (fodselsdato[1] == aaaa[3]));
    }
};


class Voksen : public Person  {   //  Abstrakt baseklasse.
protected:
    char* etternavn;              //  Den voksnes etternavn.
    
public:
    Voksen()  {
        char temp[MAXTXT + 1];
        cout << "Skriv inn etternavn:\t";
        cin.getline(temp, MAXTXT);
        etternavn = new char[strlen(temp) + 1];
        strcpy(etternavn, temp);
    }
    
    Voksen(ifstream & inn) : Person(inn)  {
        inn >> etternavn;
    }
    
    ~Voksen()  {
        delete[] etternavn;
    }
    
    void skrivTilFil(ofstream & ut)  {
        ut << etternavn;
    }
    
    void display()  {
        Person::display();
        cout <<	"\nEtternavnet er:\t" << etternavn;
    }
    
    //  ANDRE MEDLEMSFUNKSJONER ?
    
};


class Partner : public Voksen  {  //  Konkret klasse.
private:
    int telefon1;                 //  Partnerens 1.treff-telefon (f.eks. jobb)
    int telefon2;                 //  Partnerens 2.treff-telefon (f.eks. mobil)
    
public:
    Partner()  {
        telefon1 = les("Telefonummer1", 00000000, 99999999);
        telefon2 = les("Telefonummer2", 00000000, 99999999);
    }
    
    Partner(ifstream & inn) : Voksen(inn)  {
        inn >> telefon1 >> telefon2;
    }
    
    void skrivTilFil(ofstream & ut)  {
        ut << telefon1 << ' ' << telefon2;
    }
    
    void display()  {
        Voksen::display();
        cout << "\nTelefonummer1:\t" << telefon1 << '\n';
        cout << "Telefonummer2:\t" << telefon2 << '\n';
    }
    
    //  ANDRE MEDLEMSFUNKSJONER ?
    
};


class Ansatt : public Voksen  {        //  Konkret klasse.
private:
    int      nr;                       //  Unikt/entydig ansatt-nummer.
    int      antBarn;                  //  Antall barn vedkommende har.
    char*    adresse;                  //  Gate- og postadresse.
    Partner* partner;                  //  Peker til partnere.
    Barn*    barn[MAXBARN+1];          //  Pekere til alle barna.
    
public:
    Ansatt()  {
        cout << "\n\nWARNING 1: En Ansatt opprettes ALLTID med et nummer!\n\n";
    }
    
    Ansatt(int n)  {
        nr = n;
        antBarn = 0;		// Nullstiller antBar og partner peker
        partner = nullptr;
        char temp[MAXTXT + 1];
        cout << "Skriv inn adresse:\t";
        cin.getline(temp, MAXTXT);
        adresse = new char[strlen(temp) + 1];
        strcpy(adresse, temp);
    }
    
    Ansatt(int n, ifstream & inn) : Voksen(inn)  {
        nr = n;
        int harPart, harBarn;
        char temp[MAXTXT + 1];
        inn >> nr; inn.ignore(2);
        inn >> antBarn; inn.ignore(2);
        inn.getline(temp, MAXTXT);
        adresse = new char[strlen(temp) + 1];
        strcpy(adresse, temp);
        inn.ignore(2);
        inn >> harPart; inn.ignore(2);
        if(harPart == 1)	{
            partner = new Partner(inn);
        }
        inn.ignore(2);
        inn >> harBarn;
        if(harBarn != 0)	{
            for(int i = 0; i <= antBarn; i++)	{
                barn[i] = new Barn(inn);
            }
        }
    }
    
    ~Ansatt()  {
        delete[] adresse;
        delete partner;
        for(int i = 0; i < antBarn; i++)
            delete[] barn[i];
    }
    
    void skrivTilFil(ofstream & ut)  {
        int harPartner;
        ut << nr << '\n';
        ut << antBarn << '\n';
        ut << adresse << '\n';
        harPartner = les("Har du en partner? 0/1\t", 0, 1);
        ut << harPartner;
        
        if(harPartner == 1)	{
            partner->skrivTilFil(ut);
        }
        
        if(antBarn != 0)	{
            ut << antBarn << '\n';
            for(int i = 0; i <= antBarn; i++)	{
                barn[i]->skrivTilFil(ut);
            }
        
        }	else {
                ut << "0" << '\n';
        }
}
    
    void display()  {
        Voksen::display();
        cout << "\nansattnummer:\t" << nr;
        cout << "\nantall barn:\t" << antBarn;
        cout << "\nadresse:\t" << adresse;
    }
    
    int RetAnsNr()  {   return nr;  }
    int retAntBarn()    {   return antBarn; }
    bool harPartner()	{
        if(partner) {
            return true;
        } else  {
            return false;
        }
    }
    void nyPartner()	{	partner = new Partner;	}
    void slettPartner()	{	delete partner;	}
    void nyttBarn()		{	barn[++antBarn] = new Barn;	}
    
    bool finnes(int ansNr)   {
        if (ansNr == nr) {
            return 1;
        }   else    {
            return 0;
        }
    }
    
    int barnFodt(char aaa[])    {
        if (antBarn > 0)   {
            for (int i = 1; i <= antBarn; i++)  {
                if(barn[i]->likAar(aaa))    {
                    return nr;
                }   else {
                        return 0;
                }
            }
        }   else    {
                return 0;
        }
        return 0;
    }
    //  ANDRE MEDLEMSFUNKSJONER ......?
    
};

//  GLOBALE VARIABLE:
Ansatt* ansatte[MAXANS+1];     //  Array med peker til alle de ansatte.
int sisteBrukt;                //  Siste 'skuff' brukt av "ansatte"-arrayen.

//  HOVEDPROGRAMMET:
int main()  {
    char kommando;                //  Brukerens ›nske/valg.
    
    lesFraFil();                  //  Leser inn hele datastrukturen fra fil.
    skrivMeny();                  //  Meny av brukerens valg.
    
    kommando = les();             //  Leser brukerens ›nske/valg.
    while (kommando != 'Q')  {
        switch(kommando)  {
            case 'N': nyAnsatt();       break;   //  Legg inn (om mulig) ny ansatt.
            case 'P': partnerEndring(); break;   //  Endre data om partner.
            case 'B': nyttBarn();       break;   //  Legg inn (om mulig) nytt barn.
            case 'D': dataOmAnsatt();   break;   //  Skriv alle data om en ansatt.
            case 'A': alleEttAar();     break;   //  Ansnr med barn f›dt et gitt †r.
            case 'F': fjernAnsatt();    break;   //  Fjern/slett en ansatt.
            default:  skrivMeny();      break;   //  Meny av brukerens valg.
        }
        kommando = les();           //  Leser brukerens ›nske/valg.
    }
    return 0;
}

//  DEFINISJON AV FUNKSJONER:
void skrivMeny()  {
    cout << "\n\nFLGENDE KOMMANDOER ER TILGJENGELIGE:";
    cout << "\n\tN - Ny ansatt";
    cout << "\n\tP - Partner-endring";
    cout << "\n\tB - nytt Barn";
    cout << "\n\tD - alle Data om en ansatt";
    cout << "\n\tA - Alle barn f›dt ett gitt †r";
    cout << "\n\tF - Fjern en ansatt";
    cout << "\n\tQ - Quit / avslutt";
}


char les()  {                     //  Leser og returnerer ETT upcaset tegn.
    char ch;
    cout << "\n\nKommando:  ";
    cin >> ch;  cin.ignore();
    return (toupper(ch));
}
//  Skriver ledetekst (t), leser og
//    returnerer et tall mellom min og max:
int les(char* t, int min, int max)  {
    int tall;
    do  {
        cout << '\t' << t << " (" << min << '-' << max <<  "):  ";
        cin >> tall;  cin.ignore();
    } while (tall < min  ||  tall > max);
    return tall;
}

//  Leter etter en ansatt med et gitt nummer
int finnesAllerede(int ansNr)  {   //    ("ansNr"). Retur: indeks eller 0.
    int i = 1, temp = 0;
    for(i; i <= sisteBrukt; i++)	{
        if(ansatte[i]->finnes(ansNr) == true)	{
            temp = i;
        }
    }
    if(i == sisteBrukt) {
        return temp;
    }
    return temp;
}


void smettInn()  {                    //  Innstikkssortering av ETT objekt:
    int ansNr = ansatte[sisteBrukt]->RetAnsNr();
    for (int i = 1; i <= sisteBrukt; i++) {
        if ((ansatte[i]->RetAnsNr() < ansNr) && (ansatte[i+1]->RetAnsNr() > ansNr)) {
            Ansatt* temp = ansatte[sisteBrukt];
            for (int j = (sisteBrukt - 1); j != i; j--) {
                ansatte[j + 1] = ansatte[j];
            }
            ansatte[i + 1] = temp;
        }
    }
}


void lesFraFil()  {                   //  Leser HELE datastrukturen fra fil:
    int nr;
    ifstream inn("FilTilOblig2.dta");
    if(inn)	{
        inn >> sisteBrukt; inn.ignore(2);	//ignore(2) fordi LINUX
        
        for(int i = 1; i <= sisteBrukt; i++)	{
            ansatte[i] = new Ansatt(nr, inn);
        }
    }	else	{
        cout << "fant ikke filen \"FilTilOblig2.dta\".........";
    }
}


void skrivTilFil()  {                 //  Skriver HELE datastrukturen til fil:
    cout << "Skriver til fil \"FilTilOblig2.dta\"......";
    ofstream ut("FilTilOblig2.dta");
    ut << sisteBrukt << '\n';
    for(int i = 0; i <= sisteBrukt; i++)    {
        ansatte[i]->skrivTilFil(ut);
    }
}


void nyAnsatt()  {                    //  N - NY ANSATT:
    int nr;
    if(sisteBrukt < MAXANS)	{
        nr = les("Hva er ansattnummeret til den ansatte?:\t",0, 1000);
        ansatte[++sisteBrukt] = new Ansatt(nr);
        smettInn();
        //skrivTilFil();
    }	else	{
        cout << "\nIkke plass til flere ansatte\t\n\n";
    }
}


void partnerEndring()  {              //  P - ENDRE DATA OM PARTNER:
    char ans;
    bool hp;
    int ansNr = les("Hvilken ansatt vil du endre info til?", 0, 1000);
    if(finnesAllerede(ansNr) != 0)	{
        hp = ansatte[ansNr]->harPartner();
        if(hp == false)	{
            cout << "du har ikke en partner. Vil du ha en? J/N:\n";
            ans = les();
            switch (ans) {
                case 'J':	{	ansatte[ansNr]->nyPartner(); break; }
                case 'N':	{	cout << "greit.\n"; break; }
            }
        } else	{
            cout << "du har en partner. Vil du slette den? J/N:\n";
            ans = les();
            switch (ans) {
                case 'J':	{	ansatte[ansNr]->slettPartner(); break; }
                case 'N':	{	cout << "greit.\n"; break; }
            }
        }
    }
}

void nyttBarn()  {                    //  B - NYTT BARN:
    int ansNr = les("Skriv inn et ansattnummer:\t", 0, 1000);
    if(finnesAllerede(ansNr) != 0)	{
        ansatte[ansNr]->nyttBarn();
    }
}


void dataOmAnsatt()  {                //  D - SKRIVER ALLE DATA OM EN ANSATT:
    int ansNr;
    ansNr = les("Hvilken ansatt vil du se info til?", 0, 1000);
    if(finnesAllerede(ansNr) != 0)	{
        for(int i = 1; i <= sisteBrukt; i++)	{
            if(ansatte[i]->RetAnsNr() == ansNr)	{
                ansatte[i]->display();
            }
        }
    }	else	{
        cout << "Gitt ansatt finnes ikke!\n\n";
    }
    
}

//  A - SKRIVER ALLE ANSATT MED BARN
void alleEttAar()  {                  //      FDT ET GITT R:
    char aa[4+1];
    cout << "\nAarstall (aaaa): "; cin.getline(aa, 4+1);
    
    for (int i = 1; i <= sisteBrukt; i++)	{
        int ansatt = ansatte[i]->barnFodt(aa);
        if (ansatt != 0)
            cout << "\nAnsatt nr " << ansatt;
    }
}


void fjernAnsatt()  {                 //  F - FJERN/SLETT EN ANSATT
    int ansNr = les("Hvilken ansatt vil du slette?:\t", 0, 1000);
    if(finnesAllerede(ansNr != 0))	{
        for(int i = 0; i <= sisteBrukt; i++)	{
            if(ansatte[i]->RetAnsNr() == ansNr)	{
                delete ansatte[i];
                ansatte[i] = nullptr;
            }
        }
    }
}
