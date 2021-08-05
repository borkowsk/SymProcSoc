////////////////////////////////////////////////////////////////////////////////
// Symulator Proces�w Sieciowych/Spolecznych (c) Instytut Studi�w Spo�ecznych
// Uniwersytetu Warszawskiego, ul. Stawki 5/7., 2011 , wborkowski@uw.edu.pl
////////////////////////////////////////////////////////////////////////////////
// Wersja okrojona dla OPI - Projekt "Transfer technologii 2011"
////////////////////////////////////////////////////////////////////////////////
//
// Deklaracja podstawowego typu realnego w�z�a (nie pure virtual jak WezelSieci)
////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
#ifndef spsGenNodeH
#define spsGenNodeH

#include <string>
#include "spsModel.h" //Tu s� deklaracje potrzebnych typow

class Wielobok; //U�ywa wewnetrznie, ale nie ma powodu tu wnika� w szczegoly

class GenerycznyWezelSieci:public WezelSieci
{
  public:
	//Konstruktor  ustawia domyslne
	GenerycznyWezelSieci();
	static KonstruktorElementowModelu<GenerycznyWezelSieci> WirtualnyKonstruktor;
	ElementModelu::WirtualnyKonstruktor* VKonstruktor() { return &WirtualnyKonstruktor;}
	virtual bool Poprawny(); //true je�li jest dobrze zdefiniowany
	virtual double  Waznosc() { return Waga; }  //Po prostu waga innymi s�owy
	//Metoda pobiera wszystkie potrzebne dane z listy string�w. Jak blad to podaje ktora pozycja
	virtual bool ZrobWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad);
	virtual ~GenerycznyWezelSieci();//Destruktor wirtualny
	virtual void Narysuj();
	virtual void Wymazuj();
	virtual const char* Nazwa();//Musi posiada� jakas nazwe
	//Proste akcesory
	virtual bool Trafiony(float X,float Y); //Np. do inspekcji myszk�
	virtual double ZOrder() {return _Z;} //Do ustalania kolejnosci wyswietlania
	virtual double X() { return _X; }
	virtual double Y() { return _Y; }
	virtual double R(double Angle); //Promie� otaczaj�cego okr�gu lub elipsy
	virtual DziedzKol* Dziedzina() { return &Col;}
	virtual void ChwilaDlaCiebie(); //Daje jakies szanse na endogenne zmiany stan�w - tu koloru
	virtual void InterpretujKomunikat(Komunikat* Co); //Pr�buje obrobi� procesami, a jak si� nie uda to
									//"Brodkastuje" komunikat losowo do jednego lub dwu s�siad�w jak ma
  protected:
	//DziedzKol  		Col;   //Dziedziczony z ElementModelu
	//DaneLokalne		Dane;      //Dane tekstowe
	float    		_X,_Y;
	float			_Z;
	float			Waga;
	Wielobok* 		Ksztalt;
	//Metody pomocnicze dla klas domy�lnej akcji ChwilaDlaCiebie() i dla klas potomnych
	virtual void _RuszProcesPriorytetowy();//Posuni�cie do przodu najpilniejszego z proces�w
	virtual void _RuszKazdyProces();//Wywoluje ka�demu procesowi ChwilaDlaCiebie()
	void          ZarzadcaProcesowPrymitywny() { _RuszKazdyProces(); }
	virtual bool _PrzeniesDaneNaPola(unsigned& Blad);//Interpretuje pola od 1 do 7 w ZrobWgListy
	virtual bool _KomunikatDoProcesow(Komunikat* Co);//Obrobienie komunikatu przez pierwszy ch�tny proces
	virtual void _KomunikatPrzekazLosowo(Komunikat* Co,double P);//Przekazuje komunikat losowo i z prawdopodobie�stwem P namna�a
							//Dosy� to kosztowna funkcja, zw�aszcza puki 'Swiat' nie ma cachowania link�w wez�a
};



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

