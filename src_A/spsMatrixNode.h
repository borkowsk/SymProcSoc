////////////////////////////////////////////////////////////////////////////////
// Symulator Proces�w Sieciowych/Spolecznych (c) Instytut Studi�w Spo�ecznych
// Uniwersytetu Warszawskiego, ul. Stawki 5/7., 2011 , wborkowski@uw.edu.pl
////////////////////////////////////////////////////////////////////////////////
// Wersja okrojona dla OPI - Projekt "Transfer technologii 2011"
////////////////////////////////////////////////////////////////////////////////
//
// Definicje matrycowego typu w�z�a - czyli zawieraj�cego tablic� "dziedin"
// kt�ra jest wizualizowana jako obrazek i mo�e by� obrazkiem zainicjowana
////////////////////////////////////////////////////////////////////////////////
#ifndef spsMatrixNodeH
#define spsMatrixNodeH

#include "spsGenNode.h"

class WezelMacierzowy:public GenerycznyWezelSieci
{
   public:
	//Konstruktor  ustawia domyslne
	WezelMacierzowy();
	static KonstruktorElementowModelu<WezelMacierzowy> WirtualnyKonstruktor;
	//Metoda pobiera wszystkie potrzebne dane z listy string�w. Jak blad to podaje ktora pozycja
	virtual bool ZrobWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad);
	virtual ~WezelMacierzowy();//Destruktor wirtualny
	virtual void Narysuj();
	virtual void Wymazuj();
	//Proste akcesory
	virtual bool Trafiony(float X,float Y);//Np. do inspekcji myszk�
	virtual double R(double Angle); //Promie� otaczaj�cego okr�gu lub elipsy itp...
	virtual void InterpretujKomunikat(Komunikat* Co);//Interpretuje komunikat
	virtual void ChwilaDlaCiebie(); //Daje jakies szanse na endogenne zmiany stan�w - tu koloru
	virtual DziedzKol& operator() (unsigned x,unsigned y); //Dost�p do danych macierzy
  protected:
	//DziedzKol  		Col;   //Dziedziczony z ElementModelu
	//Dane;				//Dziedziczony z  GenerycznyWezelSieci
	//double    		W,_X,_Y,_Z; //j.w.
	//Wielobok* 		Ksztalt; //U�ywany do wy�wietlania pojedynczego "piksela"
	DziedzinaWKolorze*  Tablica;
	unsigned 			Wysokosc;
	unsigned 			Szerokosc;
	//Procedury pomocnicze przydatne te� dla klas potomnych
	//////////////////////////////////////////////////////////////////////////
	virtual bool _ZaladujKoloryZPliku(const char* Nazwa,const char* TekstPomocniczy=NULL);//Procedura �aduj�ca dane
				//zwraca "true" je�li si� uda, false jak nie (komunikat mo�e by� na perror)
				//z plik�w DAT (tab delimited), i ewentualnie graficznym PBM
				// - jest w osobnym �r�dle wi�c mo�na przeimplementowa�
				//TekstPomocniczy to dane po gwiazdce (*) w kom�rce poprzedzajacej nazw�
	//Funkcja przeszukiwania "bazy danych". Mo�e losowo, albo liniowo, ale potomne mog� zaiplementowac co� lepszego
	virtual DziedzinaWKolorze _ZnajdzNajpodobniejszy(DziedzinaWKolorze D,unsigned& Indeks,double& WzglednePobienstwo,unsigned IleProb=Swiat::INVINDEX);
	//...Element b�d�cy podstw� mo�e by� wskazany lub losowy
	virtual bool _OdpowiedzLosowymBitem(Komunikat* Pyt,unsigned Ktory=Swiat::INVINDEX,bool AND_OR=true);
	//MOZE BY� KOSZTOWNA! Jak nie podano liczby pr�b to przegl�da ca�� zawarto��!!!
	virtual bool _OdpowiedzNajpodobniejszym(Komunikat* Pyt,unsigned IleProb=-1);
};


//---------------------------------------------------------------------------

/********************************************************************/
/*			          SPS  version 2011                             */
/********************************************************************/
/*           THIS CODE IS DESIGNED & COPYRIGHT  BY:                 */
/*            W O J C I E C H   B O R K O W S K I                   */
/*    Instytut Studiow Spolecznych Uniwersytetu Warszawskiego       */
/*        WWW:  http://wwww.iss.uw.edu.pl/borkowski/                */
/*                                                                  */
/*                               (Don't change or remove this note) */
/********************************************************************/
#endif
