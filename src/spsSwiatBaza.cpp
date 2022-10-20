// //////////////////////////////////////////////////////////////////////////////
// Symulator Proces�w Sieciowych/Spolecznych (c) Instytut Studi�w Spo�ecznych
// Uniwersytetu Warszawskiego, ul. Stawki 5/7., 2011 , wborkowski@uw.edu.pl
// //////////////////////////////////////////////////////////////////////////////
// Wersja okrojona dla OPI - Projekt "Transfer technologii 2011"
// //////////////////////////////////////////////////////////////////////////////
//
// G��wne definicje danych i funkcji �wiata
// czyli wszystkie te kt�re nie s� w innych plikach jak np. Init-cjalizacja czy Stat-ystyka
// /////////////////
// U�YWAMY NAJPROSTSZEJ, LINIOWEJ ORGANIZACJI KONTENER�W NA OBIEKTY �WIATA  !!!
// MO�NA J� ZAST�PI� JAK�� INN�, POD WARUNKIEM, �E GWARANTUJE NIEZMIENNE UCHWYTY W POSTACI
// LICZB CA�KOWITYCH.
// //////////////////////////////////////////////////////////////////////////////////////////
#include "spsModel.h"

#include <cassert>
#include <iostream>
using namespace std;

#include "wb_smartlog.hpp"
#include "compatyb.h"
#include "lingo.hpp"

using namespace wbrtm;

///< Gdy dane s� w jednym to wszystkie inne "metody" mog� by� "static"
Swiat Swiat::Ten; ///<  Staramy si� wi�c mie� tylko jeden �wiat :-)


static const int LOGBASE=5; //Na jakim poziomie logowania domy�lnie zapisywa� zdarzenia wewn�trz prywatnych struktur danych

// /////////////////////////////////////////////////////////////////////////
//      ZARZ�DZANIE STRUKTURAMI DANYCH   Swiata
// //////////////////////////////////////////////////////////////////////////

// Wska�niki tablic
static WezelSieci** _Wezly=NULL;
static Powiazanie** _Polaczenia=NULL;
static Komunikat** _Informacje=NULL;

//Zaalokowane rozmiary tablic
static unsigned _WezlyMax=0;
static unsigned _PolaczeniaMax=0;
static unsigned _InformacjeMax=0;
//Ju� uzyte sloty
static unsigned _UzyteWezly=0;
static unsigned _UzytePolaczenia=0;
static unsigned _UzyteInformacje=0;
//Zwolnione sloty (=NULL) do odnalezienia
static unsigned _WolneWezly=0;
static unsigned _WolnePolaczenia=0;
static unsigned _WolneInformacje=0;


// Dost�py do tablic - mog� rzuci� wyj�tkiem albo przerwa� przy b��dzie dost�pu
// albo w "�agodnej" formie mo�e zwr�ci� NULL, tak jak w przypadku pustego
// czyli np. "usmierconego" slotu w tablicy
WezelSieci* Swiat::Wezel(unsigned Ktory)
{            						assert(_Wezly!=NULL);
									assert(Ktory!=Swiat::INVINDEX);
	if(Ktory<_UzyteWezly)
		return _Wezly[Ktory];
		else
		return NULL;
}

Powiazanie* Swiat::Lacze(unsigned Ktory)
{            						assert(_Polaczenia!=NULL);
									assert(Ktory!=Swiat::INVINDEX);
	if(Ktory<_UzytePolaczenia)
		return _Polaczenia[Ktory];
		else
		return NULL;
}

Komunikat* Swiat::Info(unsigned Ktory)
{            						assert(_Informacje!=NULL);
									assert(Ktory!=Swiat::INVINDEX);
	if(Ktory<_UzyteInformacje)
		return _Informacje[Ktory];
		else
		return NULL;
}

unsigned 	Swiat::ZnajdzIndeksWezla(const char* Nazwa)
//Zwraca -1(==UINT_MAX) jak nie znajdzie
{
	if(strlen(Nazwa)==0)
			return -1;
	for(unsigned i=0;i<_UzyteWezly;i++)
		if(_Wezly[i]!=NULL
			&& stricmp(_Wezly[i]->Nazwa(),Nazwa)==0)
				return i;
	return -1;//Jak nie znajdzie
}


// Aktualne rozmiary tablic - ale w ich obr�bie mog� by� puste sloty
unsigned Swiat::IleWezlow()
{
	return _UzyteWezly;
}

unsigned Swiat::IlePowiazan()
{
	return _UzytePolaczenia;
}

unsigned Swiat::IleInformacji()
{
	return _UzyteInformacje;
}

//Liczba jeszcze dost�pnych slotow
unsigned Swiat::IleMoznaWezlow()
{
	return  (_WezlyMax-_UzyteWezly)+_WolneWezly;
}

unsigned Swiat::IleMoznaPowiazan()
{
	return (_PolaczeniaMax-_UzytePolaczenia)+_WolnePolaczenia;
}

unsigned Swiat::IleMoznaInformacji()
{
	return (_InformacjeMax-_UzyteInformacje)+_WolneInformacje;
}

//Funkcje wstawiaj�ce zwracaj� pozycje wstawienia i PRZEJMUJ� ZARZ�D PAMI�CI�
// - wstawiane elementy musz� by� alokowane na stercie (przez "new")
unsigned Swiat::WstawWezel(WezelSieci* Co,unsigned propozycja/*=-1*/)
{
									  if(!Co)  TLOG(0, << "Wezel == NULL!!!" )
												assert(Co!=NULL); //Nie wolno tak kasowa�!!!
	if(! (propozycja<_UzyteWezly
		&& _Wezly[propozycja]!=NULL) )  //Szukamy tylko jesli propozycja nie ma sensu
	{
		if(_UzyteWezly<_WezlyMax) //Jest jeszcze miejsce
		{
			propozycja=_UzyteWezly++; //Bierzemy nast�pny wolny
		}
		else  //Szukamy gdzie s� te zwolnione
		if(_WolneWezly>0)
		{
			propozycja=-1; //Gdyby nie by�o...
			for(unsigned i=0;i<_UzyteWezly;i++)
			if(_Wezly[i]==NULL)
			{
				propozycja=i;
				_WolneWezly--;
				break; //I nie ma co dalej szuka�!
			}
		}
	}

	if(propozycja<_WezlyMax) //a wi�c te� nie -1 bo to naprawd� UINTMAX czy co�
	{
	  _Wezly[propozycja]=Co; //Przestaje by� NULL
	  Co->_MojIndeks=propozycja;
	  ///////////////////////////////////////////
	  Swiat::UwagaZmienionoStrukture();
	  return propozycja;   //TYLKO TU POPRAWNE WYJSCIE
	}
	else
	{
		cerr<<endl<<_LPL("Brak miejsca na wezel!","Table to small. Can't insert a node.")<<endl;
		delete Co;//Usuwa ze sterty, bo przecie� oddano mu ju� w zarz�d!!!
		return -1; //Znaczy b��d!!!
	}
}

unsigned Swiat::WstawLacze(Powiazanie* Co,unsigned propozycja/*=-1*/)
{
													assert(Co!=NULL); //Nie wolno tak kasowa�!!!
	if(! (propozycja<_UzytePolaczenia
		&& _Polaczenia[propozycja]!=NULL) )  //Szukamy tylko jesli propozycja nie ma sensu
	{
		if(_UzytePolaczenia<_PolaczeniaMax) //Jest jeszcze miejsce
		{
			propozycja=_UzytePolaczenia++; //Bierzemy nast�pny wolny
		}
		else  //Szukamy gdzie s� te zwolnione
		if(_WolnePolaczenia>0)
		{
			propozycja=-1; //Gdyby nie by�o...
			for(unsigned i=0;i<_UzytePolaczenia;i++)
			if(_Polaczenia[i]==NULL)
			{
				propozycja=i;
				_WolnePolaczenia--;
				break; //I nie ma co dalej szuka�!
			}
		}
	}

	if(propozycja<_PolaczeniaMax) //a wi�c te� nie -1 bo to naprawd� UINTMAX czy co�
	{
	  _Polaczenia[propozycja]=Co; //Przestaje by� NULL
	  Co->_MojIndeks=propozycja;
	  ///////////////////////////////////////////
	  Swiat::UwagaZmienionoStrukture();
	  return propozycja;   //TYLKO TU POPRAWNE WYJSCIE
	}
	else
	{
		cerr<<endl<<_LPL("Brak miejsca na powiazanie!","Table to small. Can't insert a link.")<<endl;
		delete Co;//Usuwa ze sterty, bo przecie� oddano mu ju� w zarz�d!!!
		return -1; //Znaczy b��d!!!
	}
}

unsigned Swiat::WstawInfo(Komunikat* Co,unsigned propozycja/*=-1*/)
{
													assert(Co!=NULL); //Nie wolno tak kasowa�!!!
	//Musi sprawdzi� czy komunikat jest OK. Nieco redundantne
	//bo nawet je�li ju� takie sprawdzenie wykona� kod zlecaj�cy
	unsigned Kanal=Co->Kanal();
	if(Kanal==INVINDEX)
		  {	goto ERROR2; } //Jakby niezainicjowany - bez informacji na log/cerr
	if(Kanal>=_UzytePolaczenia)
		  {	goto ERROR;  }//Nie ma takiego po�aczenia/kana�u/
	if(_Polaczenia[Kanal]==NULL)
		  {	goto ERROR; }//Kana� znikna�
	if(!(_Polaczenia[Kanal]->Akceptacja(Co)))
		   {  goto ERROR; }//Kana� nie przyjmuje komunikatu
	if(	!Co->Poprawny() )
		   {  goto ERROR; }//Jednak wadliwie skonstruowany
	//Teraz mo�e ju� szuka� miejsca w tablicach
	if(! (propozycja<_UzyteInformacje
		&& _Informacje[propozycja]!=NULL) )  //Szukamy tylko jesli propozycja nie ma sensu
	{
		if(_UzyteInformacje<_InformacjeMax) //Jest jeszcze miejsce
		{
			propozycja=_UzyteInformacje++; //Bierzemy nast�pny wolny
		}
		else  //Szukamy gdzie s� te zwolnione
		if(_WolneInformacje>0)
		{
			propozycja=-1; //Gdyby nie by�o...
			for(unsigned i=0;i<_UzyteInformacje;i++)
			if(_Informacje[i]==NULL)
			{
				propozycja=i;
				_WolneInformacje--;
				break; //I nie ma co dalej szuka�!
			}
		}
	}

	if(propozycja<_InformacjeMax) //a wi�c te� nie -1 bo to naprawd� UINTMAX czy co�
	{
	  _Informacje[propozycja]=Co; //Przestaje by� NULL
	  Co->_MojIndeks=propozycja;
	  ///////////////////////////////////////////
	  //Swiat::UwagaZmienionoStrukture();  - TOPOLOGIA SI� NIE ZMIENIA OD WSTAWIENIA KOMUNIKATU
	  return propozycja;   //TYLKO TU POPRAWNE WYJSCIE
	}
	else
	{
		TLOG(1, <<_LPL("BRAK MIEJSCA NA KOMUNIKAT!","NO SPACE IN MESSG POOL") )
	ERROR:
   /*	{
		clog<<endl<<_LPL("Nieudane wprowadzenie komunikatu!","Can't insert a message.")
				<<Co->WartoscPola(0)<<","<<Co->WartoscPola(1)<<" "<<Co->WartoscPola(2)<<endl;
		clog<<"\tlink: "<<Co->Kanal();
		Powiazanie* Kan=Swiat::Lacze(Co->Kanal());
		if(Kan)
		{
		WezelSieci* Start=Wezel(Kan->Poczatek());
		WezelSieci* End=Wezel(Kan->Koniec());
		clog<<" "<<(Start?Start->Nazwa():"???")<<(Kan->Kierunkowy()?"--->":"<-->")<<(End?End->Nazwa():"??? ");
		}
		clog<<endl;
	}  */
	ERROR2:
		delete Co;//Usuwa ze sterty, bo przecie� oddano mu ju� w zarz�d!!!
		return -1; //Znaczy b��d!!!
	}
	return 0;
}

// UWAGA NA USUWANIE! GENERALNIE DOSY� DROGIE
// * Usuwanie w�z�a usuwa te� wszystkie zwi�zane z nim ��cza i komunikaty
// * Usuwanie ��cza usuwa zwi�zane z nim komunikaty

// Tansze funkcje usuwaj�ce wg. indeksu (lub uchwytu, zale�nie od implementacji
void Swiat::UsunWezel(unsigned Numer)
{                           TLOG(0, << "Jeszcze nie zaiplementowane" )
							assert("Jeszcze nie zaiplementowane"==NULL);
	Swiat::UwagaZmienionoStrukture();
}

void Swiat::UsunLacze(unsigned Numer)
{                               //assert("Jeszcze nie zaiplementowane"==NULL);
	if(_Polaczenia[Numer]==NULL) //ju� skasowany
				return ; //Nic nie robimy
	else
	{
	   delete _Polaczenia[Numer];
	   _Polaczenia[Numer]=NULL;
	   _WolnePolaczenia++; //Jest jeden wolny slot wi�cej

	}

	//Uzywamy komunikaty u�ywaj�ce tego �acza
	for(unsigned i=0;i<_UzyteInformacje;i++)
		if(_Informacje[i]!=NULL
		&& _Informacje[i]->Kanal()==Numer)
			Swiat::UsunInfo(i);

	Swiat::UwagaZmienionoStrukture();
}

void Swiat::UsunInfo(unsigned i)
//Usuwanie komunikatu jest proste bo nikomu nie wolno si� do niego odnosi�
{
	delete (_Informacje[i]);
	_Informacje[i]=NULL;
	_WolneInformacje++; //Jest teraz wolny slot
   //	Swiat::UwagaZmienionoStrukture();  - komunikat nie nalezy do topologii sieci
}

// Funkcja usuwaj�ce wg. wska�nika - kosztowne bo szukaj�
unsigned Swiat::UsunWezel(WezelSieci* Jaki)
{
	for(unsigned i=0;i<_UzyteWezly;i++)
		if(_Wezly[i]==Jaki)
		{
			UsunWezel(i);//Usuwa znaleziony
			return i;
		}
	return -1;
}

unsigned Swiat::UsunLacze(Powiazanie* Jaki)
{
	for(unsigned i=0;i<_UzytePolaczenia;i++)
		if(_Polaczenia[i]==Jaki)
		{
			UsunLacze(i);//Usuwa znaleziony
			return i;
		}
	return -1;
}

unsigned Swiat::UsunInfo(Komunikat* Jaki)
{
	for(unsigned i=0;i<_UzyteInformacje;i++)
		if(_Informacje[i]==Jaki)
		{
			UsunInfo(i);//Usuwa znaleziony
			return i;
		}
	return -1;
}

// KONSTRUKTUR  - W�A�CIWIE NIE MAJ� NIC WA�NEGO DO ZROBIENIA (???)
Swiat::Swiat()
//Konstruktor  prywatny - bo tylko jeden �wiat. Nie ma wiele do roboty
//Prawdziwa robota robi si� gdzie indziej i kiedy indziej
{
#ifndef NDEBUG
	//Konstruktor globalnej zmiennej nie bardzo mo�e pisa� przez cout bo cout mo�e
	//nie by� jeszcze zaincjowane!
 //	fprintf(stderr,"%s\n","DEBUG: Inicjalizacja struktur danych");
#endif
}

typedef WezelSieci* ptrWezelSieci;
typedef Powiazanie* ptrPowiazanie;
typedef Komunikat*  ptrKomunikat;

bool Swiat::_UstalLiczbeWezlow(unsigned IleMax)
//W tym modelu mo�na wywo�ac tylko raz!!!
{
											assert(_Wezly==NULL);
	_Wezly=new ptrWezelSieci[IleMax];
	_WezlyMax=IleMax;
	_UzyteWezly=0;
	Swiat::UwagaZmienionoStrukture();
	return true;
}

bool Swiat::_UstalLiczbeLaczy(unsigned IleMax)
//W tym modelu mo�na wywo�ac tylko raz!!!
{
											assert(_Polaczenia==NULL);
	_Polaczenia=new ptrPowiazanie[IleMax];
	_PolaczeniaMax=IleMax;
	_UzytePolaczenia=0;
	Swiat::UwagaZmienionoStrukture();
	return true;
}

bool Swiat::_UstalLiczbeInfo(unsigned IleMax)
//W tym modelu mo�na wywo�ac tylko raz!!!
{
	if(_UzyteInformacje!=0)
			return false; //Ju� nie mo�na!!!
	if(_Informacje!=NULL)
			delete [] _Informacje;
	_Informacje=new ptrKomunikat[IleMax];
	_InformacjeMax=IleMax;
	_UzyteInformacje=0;
	Swiat::UwagaZmienionoStrukture();
	return true;
}

Swiat::~Swiat()
//Destruktor wirtualny powinien posprz�ta� - ale to czasoch�onne,
//a prawdopodobnie zb�dne - destruktory obiekt�w �wiata i tak by pewnie zawlnia�y
//pami�� i powiadamia�y si� wzajemnie o likwidacji.
//Lepiej jak "�wiat" zostanie oddany w ca�o�ci systemowi na koniec programu
{
#ifndef NDEBUG
 //	fprintf(stderr,"%s\n","DEBUG: Destrukcja struktur danych");
	cout<<endl;
#endif
}


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


