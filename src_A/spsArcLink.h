
////////////////////////////////////////////////////////////////////////////////
// Symulator Proces�w Sieciowych/Spolecznych (c) Instytut Studi�w Spo�ecznych
// Uniwersytetu Warszawskiego, ul. Stawki 5/7., 2011 , wborkowski@uw.edu.pl
////////////////////////////////////////////////////////////////////////////////
// Wersja okrojona dla OPI - Projekt "Transfer technologii 2011"
////////////////////////////////////////////////////////////////////////////////
//
// Deklaracja/Definicja linku og�lnego, ale rysowanego jako �uk
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
#ifndef spsArcLinkH
#define spsArcLinkH

#include "spsGenLink.h"

class PowiazanieZagiete:public GenerycznePowiazanie
{
  public:
	PowiazanieZagiete(); //Domyslny konstruktor ustawiaj�cy pusty link
	PowiazanieZagiete(double par);//Konstruktor z ustawieniem parametru
	static KonstruktorElementowModelu<PowiazanieZagiete> WirtualnyKonstruktor;
	//Metoda pobiera wszystkie potrzebne dane z listy string�w. Jak blad to podaje ktora pozycja
	virtual bool ZrobWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad);
	virtual ~PowiazanieZagiete();
	//Tak naprawd� to r�ni si� tylko sposobem rysowania
	virtual void Narysuj();
	virtual void Wymazuj();
	virtual void  PodajPozycje(double D,bool KierunekZwykly,
							   double& X,double& Y,Komunikat* K=NULL);//Przelicza polozenia wdluz linku
							   //Mo�e uwzgl�dnia� te� specyfik� komunikatu
	void UstawParametr(double par);//Zmienia parametr wi�c i pomocnicze zmienne si� zmieniaja
	protected:
	void _PoliczParametryLuku();//Liczy parametry �uku dla danej wartosci parametru
	double parametr;//Warto�� decyduj�ca o sposobie zagi�cia
	private:
	//Pomocnicze pola zawieraj�ce niezb�dne parametry �uku
	double Xa,Ya,Xb,Yb,Xo,Yo,Promien,alfa,beta;
	unsigned krokow;//Ile odcink�w �uku przy rysowaniu ~ odleglo�� PK/10 pix
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
