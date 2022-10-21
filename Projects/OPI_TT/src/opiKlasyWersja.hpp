// Transfer technologii 2011 - nag��wk implementacji modelu     24-10-2011
////////////////////////////////////////////////////////////////////////////////
// Klasy do kolejnej startowej wersji modelu  (dla danych na poziomie wersji 1.61)
////////////////////////////////////////////////////////////////////////////////
#ifndef _opiKlasy_plik_naglowkowy_hpp_
#define _opiKlasy_plik_naglowkowy_hpp_

#include "spsModel.h"
#include "spsGenNode.h"
#include "spsGenProc.h"
#include "spsGenLink.h"
#include "spsParaLink.h"
#include "spsGenInfo.h"
#include "spsMatrixNode.h"
#include "INCLUDE/platform.hpp"
#include "INCLUDE/wb_ptr.hpp"
#include "INCLUDE/wb_bits.h"
using namespace wbrtm;

#include <stdlib.h>
#include <cassert>
#include <string>
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
using namespace std;

extern float  TOWARZYSKA_NAJMNIEJSZA_WAGA;//=0.01;// dla	bool KontaktTowarzyski::Poprawny();
extern float  JEDNOSTKOWA_WYPLATA;//=100;//Warto�� najmniejszej wp�aty/wyplaty. Spos�b u�ycia niejasny :-)

extern float  WAGA_NA_DZIENDOBRY;//=0.1;//Wsp�czynnik zmniejszania wagi przy pierwszym kontakcie - nie mo�e by� za du�a
extern float  TEMPO_SPADKU_LINKU;// np. =0.01;//O jak� cz�� link socjalny zanika przy nie u�ywaniu
extern float  TEMPO_WZROSTU_LINKU;// np. =0.1;//O jak� cz�� link socjalny ro�nie przy odpowiedzi
extern const char* const KONTO;//="Ruch konta"; //Marker komunikatu finansowego z przep�ywem
extern const char* const DZIENDOBRY;//="Dzien dobry";//Marker otwieraj�cego komunikatu spo�ecznego

//Klasa wewn�trzna skupiaj�ca wsp�lne w�a�ciwo�ci WEZLOW z pracownikami (zespo��w)
//////////////////////////////////////////////////////////////////////////////////
class _ZespolRoboczy:public GenerycznyWezelSieci
{
 protected:
 friend class  ProcesProdukcyjny;
 friend class  ProcesRaportowany;
 friend class  ProcesBadawczy;
 friend class  ProcesSpoleczny;
 friend class  ProcesPoszukiwanTT;
 friend class  ProcesTransferuTech;
//Ile Etat�w	Efektywnosc	Aktywno��	Centralnosc	Probiznesowosc	Prolegalnosc	Czasochlonnosc	Innowacyjno�c	Finans. swobodne	Finanse zaplan	D�ug do nadrz.	Udzial w zysku TT	Ekspercko�� w dziedzinie	Uwagi
 unsigned IleEtatow;//Ile etat�w. Co najmniej 1, 0 - "duch" zespo�u - plan utworzenia
 float    Efektywnosc;//0..2. Czy dzia�a na 50% wydajno�ci czy na 150%. Zale�y od motywacji
 float    Doswiadczenie;//POZIOM EKSPERTYZY? Kolejny mno�nik do liczby etat�w
 float    Centralnosc;//0..1. Jak bardzo wydajno�� i ewentualnie aktywno�� zale�y od szefa/szef�w

 float    Proaktywnosc;//Aktywno��? 0..1. Podtrzymywanie aktywno�ci zewn�trznej, tworzenie link�w  itp.
 float    Czasochlonnosc;//Jaki jest wsp�czynnik czasoch�onno�ci typowej dzia�alno�ci. Wygenerowania pojedynczego wyniku bada�?
 float    Prolegalnosc;//????Uczciwo��??? Jak �atwo podj�� dzia�ania "szarostrefowe"
 //Liczba punkt�w
 unsigned Sukcesy;
 //Bezparametrowy konstruktor z warto�ciami domyslnymi
 _ZespolRoboczy(){IleEtatow=1;Efektywnosc=0.5;Doswiadczenie=0.1;Centralnosc=1;Proaktywnosc=0.5;Czasochlonnosc=31;Prolegalnosc=0.75;Sukcesy=0;}
 //Procedury podzialu czasu pracy na poszczeg�lne procesy - do wywo�ywania w procedurach ChwilaDlaCiebie()
 void     _ZarzadcaProcesowLosowy(); //Wybiera proces do wykonania losowo anga�uj�c si� te� losowo
 void 	  _ZarzadcaProcesowLeniwy(); //Robi tylko najpilniejszy proces albo wcale
 void 	  _ZarzadcaProcesowFantazyjny();//Robi pilny i jak mu co� zostaje si� to jeszcze jaki�
 void 	  _ZarzadcaProcesowFinezyjny();//Sprytnie dzieli czas �eby wszystko szlo do przodu
 void 	  _ZarzadcaProcesowSprawiedliwy();//Dzieli wszystkie si�y proporcjonalnie do priorytet�w proces�w co nie jest dobrym pomys�em
 public:
 virtual double R(double);//Promie� otaczaj�cego okr�gu lub elipsy - ze wzgl�du na du�e wagi musi byc LOGARYTMICZNIE zalezny od wagi
 virtual bool Poprawny();//Sprawdza czy wszystkie wska�niki s� poprawnie wpisane (nie przypadkowe i nie 0)
 //Specjalne dla tego typu
 void ZmienWage(double mnoznik);//Zmienia bezpiecznie wag� powiazania. "mnoznik" mo�e by� wi�kszy lub mniejszy ni� 1
 //�eby mogl rozliczac si� finansowo z procesami.
 virtual void PrzyjmijKase(float Suma)=0;//Suma mo�e by� ujemna. Mo�e t� kas� "przepu�cic" ale proces jest OK
 virtual bool StacGoNaWydatek(float Suma)=0;//Sprawdza czy taki wydatek jest dopuszczalny
 //Do uzycia w r�nych sytuacjach - indeksy typowych aktor�w
 static  unsigned DajRynek();//RYNEK albo MARKET
 static  unsigned DajUrzadPatentowy();//UPATENTOWY | URZ�D PATENTOWY | patent office  | Patent Office
 static  unsigned DajPublikator();//PUBLIKATOR  //unsigned _ZespolRoboczy::DajPublikator();//PUBLIKATOR
 //Inne pomocnicze
 static float _PodobienstwoDziedzin(DziedzinaWKolorze A,DziedzinaWKolorze B);
 static float _KalkulujKosztProduktu(DziedzinaWKolorze D); //Furtka do "realistycznego" kalkulowania ceny produktu
	//Mo�na uzale�ni� od zawarto�ci bitowej Dziedziny "D"!
	// np. KOSZT=D.IleBitow()*4+(D.R*0.5+D.G+D.B*2)/3.5;
	//Kosztuje liczba bitow i niektore bity oraz niektore dziedziny s� dro�sze ni� inne
	//Model ilo�ciowy powinien si� temu przyjrze�.
	//Wa�ny parametr to te�:
	//ILE_RAZY_BADANIA_DROZSZE_NIZ_TOWAR
 };

//Klasa dla badacza, zespo�u badawczego
////////////////////////////////////////////////////////////////////////////////
class JednostkaBadawcza:public _ZespolRoboczy
{
	friend class ProcesBadawczy; //Musi mie� dost�p do w�a�ciwo�ci, w celach kalkulacyjnych
	friend class ProcesTransferuTech;
	//Implementacja tego co musi by� wg. interfejsu typu podstawowego w�z�a
	static KonstruktorElementowModelu<JednostkaBadawcza> WirtKonstr;
	ElementModelu::WirtualnyKonstruktor* VKonstruktor() { return &WirtKonstr;}
  public:
	void ChwilaDlaCiebie(); //Endogenne zmiany stan�w, zarz�dzanie procesami itp.
	void InterpretujKomunikat(Komunikat* Co);//Przyjmowanie komunikat�w
	JednostkaBadawcza(){}//Konstruktor domy�lny

	bool ZrobWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad);
	void AktualizujListeDanych();//Do zapisu i wy�wietlania inspekcyjnego
	//Ruchy na koncie
	void PrzyjmijKase(float Suma) {FinanseSwobodne+=Suma;} //�eby mogl rozliczac si� finansowo z procesami
	bool StacGoNaWydatek(float Suma){ return Suma<FinanseSwobodne;}//Sprawdza czy taki wydatek jest dopuszczalny
  protected://W�a�ciwo�ci obiekt�w klasy  i metody pomocnicze
 //Cechy:
 //////////////////////////////////
 // z klasy "ZespolRoboczy":
 //Ile Etat�w	Efektywnosc	Aktywno��	Centralnosc	Probiznesowosc	Prolegalnosc	Czasochlonnosc	Innowacyjno�c	Finans. swobodne	Finanse zaplan	D�ug do nadrz.	Udzial w zysku TT	Ekspercko�� w dziedzinie	Uwagi
 // inne - w�asne:
//Jedna skala czy dwie Pronaukowosc vs. Probiznesowsc
 float    Probiznesowosc;//0 - czysta nauka, 0.5-badania patentowalne/licenjonowalne, 1 - czysty biznes - szybkie wdra�anie
 float    Innowacyjnosc;//0..1. �atwo�� startowania projekt�w bez inspiracji z zewn�trz, zaciekawienie projektami nieco odleg�ymi od dotychczasowych

 float    FinanseSwobodne;//Rezerwy finansowe - w skali roku LUB UPROSZCZONA "ZAMO�NO��"
 float    FinanseObiecane;//Zaplanowane do wydania, ale by� mo�e jeszcze nie na koncie (odnawiane/odbierane co 365 krok�w)
 float 	  Dlugi;//Ile zesp� "wisi" swojemu wydzia�owi/instytutowi. Mo�liwe tylko chwilowo (30 krok�w?)
 float    UdzialWZysku;//0..1 Jaki procent zysku z innowacji pobiera (reszta idzie dla pracownik�w)

 //JAK IMPLEMENTOWA� MOTYWACJE?????????  --> Zmiany efektywno�ci, ale jaka metoda?
 //Projekty z listy aktywnych projekt�w
 //Kontakty z siatki
 //Co z kontaktami mi�dzynarodowymi?
 //po��czenie z obiektem gridowym a la rynek?
};

//Klasa dla Instytutu, Wydzia�u, Uniwersytetu itp.
////////////////////////////////////////////////////////////////////////////////
class AdministracjaNaukowa:public _ZespolRoboczy
{
	friend class ProcesTransferuTech;
	//Implementacja tego co musi by� wg. interfejsu typu podstawowego w�z�a
	static KonstruktorElementowModelu<AdministracjaNaukowa> WirtKonstr;
	ElementModelu::WirtualnyKonstruktor* VKonstruktor() { return &WirtKonstr;}
  public:
	void ChwilaDlaCiebie(); //Endogenne zmiany stan�w, zarz�dzanie procesami itp.
	void InterpretujKomunikat(Komunikat* Co);//Przyjmowanie komunikat�w
	AdministracjaNaukowa(){}//Konstruktor domyslny
	bool ZrobWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad);
	void AktualizujListeDanych();//Do zapisu i wy�wietlania inspekcyjnego
	//Ruchy na koncie
	void PrzyjmijKase(float Suma) {FinanseSwobodne+=Suma;} //�eby mogl rozliczac si� finansowo z procesami
	bool StacGoNaWydatek(float Suma){ return Suma<FinanseSwobodne;}//Sprawdza czy taki wydatek jest dopuszczalny
  protected://Wlasciwo�ci obiekt�w klasy  i metody pomocnicze
 ///////////////////////////////////////////////////////////////////////////////
 // z KLASY: ZespolRoboczy
 // unsigned  IleEtatow;//Ile etat�w. Co najmniej 1, 0 - "duch" zespo�u - plan utworzenia
 //float     Efektywnosc;//0..2. Czy dzia�a na 50% wydajno�ci czy na 150%. Zale�y od motywacji
 //float     Proaktywnosc;//(Pro)aktywno��. Prawdopodobie�stwo organizowania konferencji i innych event�w umo�liwiaj�cych kontakty mi�dzy naukowcami i go��mi (czyli co ile krok�w nast�puje taki event)
 //float     Centralnosc;//0..1. Jak bardzo aktywno�� (zewn�trzna?) zale�y od szefa/szef�w
 //float     Doswiadczenie;//POZIOM EKSPERTYZY? Jak tego u�ywa�? Mo�e to samo co efektywno��???
 //float     Czasochlonnosc;// Ile czasu (krok�w modelu) si� czeka z podj�ciem decyzji
 //float     Prolegalnosc;//????Elastyczno��??? Jak �atwo podj�� dzia�ania obchodz�ce "bzdurne" przepisy

 float     Probiznesowosc;//0...1 Prawdopodobie�stwo podj�cia inicjatyw z wsp�udzia�em biznesu
 float     ProInwest;//0-1 Sk�onno�� inwestowania wolnych �rodk�w w rozw�j zwi�zany z TT

 float     FinanseSwobodne;//Rezerwy finansowe - w skali roku (odnawiane/odbierane co 365 krok�w) LUB UPROSZCZONA "ZAMO�NO��"
 float     FinanseObiecane;//Zaplanowane do wydania, ale by� mo�e jeszcze nie na koncie
 float 	   Dlugi;//Ile jednostka ju� "wisi". Np. na budow�, albo na pr�d jak dotacja zaleg�a. Nie mo�e wi�cej ni� urz�dowy limit
 float     UdzialWZysku;//0..1 Jaki procent zysku z innowacji pobiera (reszta idzie "w d�" hierarchii)

 //float     UdzialPromo; //0-1 Jakie �rodki s� gotowi przeznaczy� na marketing  (SK�D JE BIOR�?)
 //float     DecyzjaPatentowa;//Ile czasu  jest podejmowana taka decyzja: wdra�amy vs. nie interesuje nas to
};

//Klasa wykonwacy/producenta ostatecznego produktu
////////////////////////////////////////////////////////////////////////////////
class Firma:public _ZespolRoboczy
{
	friend class ProcesTransferuTech;
	//Implementacja tego co musi by� wg. interfejsu typu podstawowego w�z�a
	static KonstruktorElementowModelu<Firma> WirtKonstr;
	ElementModelu::WirtualnyKonstruktor* VKonstruktor() { return &WirtKonstr;}
  public:
	void ChwilaDlaCiebie(); //Endogenne zmiany stan�w, zarz�dzanie procesami itp.
	void InterpretujKomunikat(Komunikat* Co);//Przyjmowanie komunikat�w
	Firma(){}//Konstruktor domyslny
	bool ZrobWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad);
	void AktualizujListeDanych();//Do zapisu i wy�wietlania inspekcyjnego
	//Ruchy na koncie
	void PrzyjmijKase(float Suma) {FinanseBiezace+=Suma;} //�eby mogl rozliczac si� finansowo z procesami
	bool StacGoNaWydatek(float Suma){ return Suma<FinanseBiezace;}//Sprawdza czy taki wydatek jest dopuszczalny
  protected://Wlasciwo�ci obiekt�w klasy i metody pomocnicze
 //////////////////////////////////////////////////////////////////////////////////////
  void _RozeslijNaleznosci(DziedzinaWKolorze Produkt,float& wplyw); //Wg. identyfikatora produktu szuka komu winien dywidende
 //Z KLASY: ZespolRoboczy
 // unsigned IleEtatow;//Ile etat�w. Co najmniej 1, 0 - "duch" zespo�u - plan utworzenia
 //float    Efektywnosc;//0..2. Czy dzia�a na 50% wydajno�ci czy na 150%. Zale�y od motywacji
 //float    Proaktywnosc; //Aktywno��. 0-1 aktywno�� w tworzenia nowych znajomo�ci biznesowych
 //float    Centralnosc;//0..1. Jak bardzo aktywno�� (zewn�trzna?) zale�y od szefa/szef�w
 //float    Doswiadczenie;//POZIOM EKSPERTYZY? Jak tego u�ywa�? Mo�e to samo co efektywno��???
 //float    Czasochlonnosc;//Jaki jest wsp�czynnik czasoch�onno�ci typowej dzia�alno�ci (np. przygotowanie projektu oraz produktu)
 //float    Prolegalnosc;//?"Pragmatyczno��"? Jak �atwo podj�� dzia�ania "szarostrefowe" i omijac "nie�yciowe" przepisy

 float    ProAkademickosc; //0-1 ch�� nawi�zywania kontakt�w z badaczami i uniwersytetami
 float    ProInnowac;//Stosunek do nowo�ci: 0-1 - 0..33% niewiele robi, 33-66% raczej blokuje innych,
					//powy�ej - pr�buje, wchodz�c na coraz wcze�niejszym etapie (prawdopodobienstwo)

 float    FinanseBiezace; //Ile firma ma kasy aktualnie LUB UPROSZCZONA "ZAMO�NO��"
 float    FinanseZaplanowane;//Ile firma ma obiecanych �rodk�w kredytowych
 float 	  Dlugi;//Ile firma ma do sp�aty. Im wi�cej tym mniejsza szansa �e co� dostanie nowego
 float 	  LimitUdzialuInwestTT; //0-1 Jakie �rodki finansowe z zarobionych got�w przeznaczy� na inwestycje TT

 //float 	  UdzialPromo; //0-1 Jakie �rodki sa gotowi przeznaczy� na marketing
 //float Budzet
 //float Wiarygodnosc;//Cecha po��czenia? //0-1 na ile dana firma jest atrakcyjna
			//dla uniwersytetu oraz bank�w w kontek�cie podejmowania z ni� wsp�pracy
 //???float Wspolpraca_Konkurenci; //ch�� podejmowania wsp�pracy z innymi firmami 0-1
 //(te dwie zmienne powinny by� od siebie niezale�ne, relacja o sumie niezerowej
 //...
};

//Klasa organizatora wdrozenia innowacji (BOTT i UOTT)
////////////////////////////////////////////////////////////////////////////////
/*
class OrganizatorWdrozenia:public GenerycznyWezelSieci
{
  //DYLEMAT - CZY TO JEDEN TYP Z "SUWAKAMI" POZWALAJ�CYMI ZMIENI� UOTT w niemal BOTT
  //czy dwa zupe�nie r�ne typy?
  //Ostatecznie przyj�to �e:
  //Czy BOTT=firma+OTT a UOTT=AdmNaukowa+OTT
};
*/

class BOTT:public Firma
{  //Implementacja tego co musi by� wg. interfejsu typu podstawowego w�z�a
	static KonstruktorElementowModelu<BOTT> WirtKonstr;
	ElementModelu::WirtualnyKonstruktor* VKonstruktor() { return &WirtKonstr;}
  public:
	void ChwilaDlaCiebie(); //Endogenne zmiany stan�w, zarz�dzanie procesami itp.
	void InterpretujKomunikat(Komunikat* Co);//Przyjmowanie komunikat�w
	BOTT(){}//Konstruktor domyslny
	bool ZrobWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad);
  protected://Wlasciwo�ci obiekt�w klasy i metody pomocnicze
  //////////////////////////////////////////////////////////////////////////////
  //Ile Etat�w	Efektywnosc	Aktywno��	Centralnosc	Pronaukowosc	Prolegalnosc	Czasochlonnosc	Proinnowacyjnosc	Finans. bie��ce	Finanse zaplan	D�ugi	Limit ryzyk. TT	EksperckoscTT	Uwagi
  //float 	  LimitUdzialuInwestTT; //0-1 Jakie �rodki finansowe na RYZYKOWNE inwestycje TT
  // float EksperckoscTT;=DOSWIADCZENIE//Dodatkowy wsp�czynnik 0..2 skuteczno�ci TT wynikaj�cy z do�wiadczenia
  // float AktywnoscInform;//Aktywne poszukiwanie informacji "bazodanowej" (rynek, bazy, patenty etc)
   // ... po prostu 0.5 normalnej aktywno�ci???
 //Kontakty z siatki
 //Projekty z listy aktywnych projekt�w
 //Nowe znajomo�ci przez polecenie, konferencje, poszukiwanie/nawi�zywanie kontakt�w
 //Ocena projektu?  bezpo�rednia i po�rednia
};

class UOTT:public AdministracjaNaukowa
{  //Implementacja tego co musi by� wg. interfejsu typu podstawowego w�z�a
	static KonstruktorElementowModelu<UOTT> WirtKonstr;
	ElementModelu::WirtualnyKonstruktor* VKonstruktor() { return &WirtKonstr;}
  public:
	void ChwilaDlaCiebie(); //Endogenne zmiany stan�w, zarz�dzanie procesami itp.
	void InterpretujKomunikat(Komunikat* Co);//Przyjmowanie komunikat�w
	UOTT(){}//Konstruktor domyslny
	bool ZrobWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad);
  protected://Wlasciwo�ci obiekt�w klasy i metody pomocnicze
  //////////////////////////////////////////////////////////////////////////////
  //Ile Etat�w	Efektywnosc	Aktywno��	Centralnosc	Probiznesowosc	Prolegalnosc	Czasochlonnosc	ProTT	Finans. swobodne	Finanse zaplan	D�ug do nadrz.	Udzial w zysku TT	EksperckoscTT	Uwagi
   //float    UdzialWZysku;//0..1 Jaki procent zysku z innowacji pobiera i przekszta�ca w motywacj� pracownik�w
   //float EksperckoscTT;==Doswiadczenie//0..2 wsp�czynnik skuteczno�ci TT wynikaj�cy z do�wiadczenia
   //float AktywnoscInform;//Aktywne poszukiwanie informacji "bazodanowej" (rynek, bazy, patenty etc)
   // ... po prostu 0.5 normalnej aktywno�ci???
 //Kontakty z siatki
 //Projekty z listy aktywnych projekt�w
 //Ocena projektu??? - po�rednia przez wewn�trz uczelnianych ekspert�w lub zewn�trzne firmy
 //Trwa to d�u�ej
};


//Klasa powi�zania administracyjnego - przepuszcza w g�r� raporty,
// a w d� wytyczne oraz pieni�dze
////////////////////////////////////////////////////////////////////////////////
class PodlegloscOrganizacyjna:public GenerycznePowiazanie
{  //Implementacja tego co musi by� wg. interfejsu typu podstawowego linku
	static KonstruktorElementowModelu<PodlegloscOrganizacyjna> WirtKonstr;
	//Dla usprawnienia klasa potomna mo�e zwraca� adres swojego wirtualnego konstruktora
	//Cho� mo�na te� nie definiowa�  i polegac na gorszej funkcji z klasy bazowej
	ElementModelu::WirtualnyKonstruktor* VKonstruktor() { return &WirtKonstr;}
  public:
	//void ChwilaDlaCiebie(); //Endogenne zmiany stan�w linku - tu brak ...
	bool  Kierunkowy() { return true;}  //Wizualnie zachowuje si� jak kierunkowy
	bool  Akceptacja(Komunikat* Co);//Ale selekcja komunikat�w jest z�o�ona - mog� i�� pod pr�d
	PodlegloscOrganizacyjna(){}//Konstruktor domyslny

	bool ZrobWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad);
  protected://W�a�ciwo�ci obiekt�w klasy  i metody pomocnicze
  //... ???
};


// Klasa kooperacji sformalizowanej - przepuszcza w raporty - efekty dzia�a�
// oraz pieni�dze
////////////////////////////////////////////////////////////////////////////////
class FormalnaKooperacja:public PowiazaniePaboliczne
{  //Implementacja tego co musi by� wg. interfejsu typu podstawowego linku
	friend class KonstruktorElementowModelu<FormalnaKooperacja>;
	static KonstruktorElementowModelu<FormalnaKooperacja> WirtKonstr;
	ElementModelu::WirtualnyKonstruktor* VKonstruktor() { return &WirtKonstr;}
  public:
	FormalnaKooperacja(unsigned Inicjator,unsigned Kooperator,float Waga/*=Udzial=1*/,unsigned Termin=(10*365));//Konstruktor domy�lny
	void ChwilaDlaCiebie(); //Jak "Wykonany" to z czasem si� coraz bardziej wygina, a jak termi spadnie do 0 to znika
	bool  Akceptacja(Komunikat* Co);//Selekcja komunikat�w
	bool  Poprawny();//Jak po terminie to znika
	bool ZrobWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad);
	void Narysuj();//Przechwycenie rysowania dla debugingu
	//Akcesory
	unsigned DajTermin(){ return Termin;}//Dostep do terminu
	void     UstawTermin(unsigned NowyTermin){Termin = NowyTermin;}
	bool     CzyWykonany(){ return Wykonany;}
	void     UstawWykonano(bool TakNie=true){Wykonany=TakNie;  }
	float 	 JakiUdzial(){ if(Waga<1) return Waga; else return 1;} //Waga linku kooperacyjnego jest mierzona udzialem w zyskach!!!
	//W�a�ciwo�ci obiekt�w klasy  i metody pomocnicze
  protected:
   FormalnaKooperacja(){Wykonany=0;Termin=(10*365);}//Konstruktor domy�lny
   bool   Wykonany;//Czy ju� zosta� zrealizowany i tylko p�yn� dywidendy
   int    Termin;//Maleje z ka�dym krokiem i jak 0 to link znika - koniec umowy
};


//Klasa powi�zania towarzyskiego, zanikaj�cego powoli gdy nie u�ywany
// ASYMETRYCZNA - TAKIE LINKI S� POD DWA BO ZAUFANIE I WAGA POWI�ZANIA
// ZAZWYCZAJ JEST RӯNA Z PUNKTU WIDZENIA OBU PARTNER�W!
//Komentarz na temat negatywno�ci (do rozwa�enia) - hamuj�ca rola w�z�a w przekazie
//informacji. Jak nie lubi� bo zazdroszcz�, to mu nie powiem,
//chocia� jeste�my powi�zani.
////////////////////////////////////////////////////////////////////////////////
class KontaktTowarzyski:public PowiazaniePaboliczneSkierowane
{  //Implementacja tego co musi by� wg. interfejsu typu podstawowego linku
	static KonstruktorElementowModelu<KontaktTowarzyski> WirtKonstr;
	ElementModelu::WirtualnyKonstruktor* VKonstruktor() { return &WirtKonstr;}
  public:
	void ChwilaDlaCiebie(); //Endogenne zmiany stan�w linku - zanikanie...
	bool Poprawny(); //true je�li jest dobrze zdefiniowany. Wci�� istniej� oba konce itp.
					// i WAGA jest wi�ksza ni� TOWARZYSKA_NAJMNIEJSZA_WAGA
	bool  Akceptacja(Komunikat* Co);//Selekcja komunikat�w - podstawowa i wzrost wagi
	KontaktTowarzyski(){}//Konstruktor domyslny
	KontaktTowarzyski(unsigned Kogo,unsigned ZKim,double JakaWaga,double JakieWygiecie);
	bool ZrobWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad);
	//Specjalne dla tego typu
	void ZmienWage(double mnoznik);//Zmienia bezpiecznie wag� powiazania. "mnoznik" mo�e by� wi�kszy lub mniejszy ni� 1
  protected://Wlasciwo�ci obiekt�w klasy  i metody pomocnicze
	//float Zaufanie == Waga linku . Dlatego skierowany bo zaufanie mo�e by� asymetryczne
	//float TempoZanikania; //Czasem szybkie, czasem wolne, r�ne dla r�nych os�b
	//Parametr wygi�cia linku jest te� skorelowane z tempem "parowania" nieu�ywanego
	//linku socjalnego - im bardziej "poboczny" czyli wygi�ty link tym szybciej zanika
};


//Klasa komunikatu oficjalnego - raporty (w g�r�), wytyczne w d� (podleglosci  adm.)
////////////////////////////////////////////////////////////////////////////////
class KomunikacjaTowarzyska:public GeneryczneInfo
{  //Implementacja tego co musi by� wg. interfejsu typu podstawowego linku
	friend class KonstruktorElementowModelu<KomunikacjaTowarzyska>;
	static KonstruktorElementowModelu<KomunikacjaTowarzyska> WirtKonstr;
	ElementModelu::WirtualnyKonstruktor* VKonstruktor() { return &WirtKonstr;}
 public:
	Komunikat* Klonuj(); //Robi na stercie kopie komunikatu do przechowania lub wstawienia
	virtual bool  UstawRodzaj(const char* );
	const string& Rodzaj();//Gdzie� w danych jest rodzaj tego komunikatu

	//Konstruktor wymaga kategori i opcjonalnie numeru wezla o kt�rym mowa
	KomunikacjaTowarzyska(const char* Rodzaj,unsigned OKogoChodzi=-1);
	unsigned&     OKimTaGadka() {return OKim;}  //Na temat kogo jest ten komunikat - mo�e o nadawcy, a mo�e nie!

	bool Zwrotnie(float Szybkosc=0);//Zwrotne adresowanie sluzy do pogrubiania linku na kt�rego kom. odpowiadamy
	bool ZrobWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad); //Jak generyczny

  protected://Wlasciwo�ci obiekt�w klasy  i metody pomocnicze
	void RysujKsztalt(float X,float Y,float Rad,unsigned R,unsigned G,unsigned B); //Rysowanie kszta�tu zale�nego od typu potomnego,
	unsigned OKim;
	KomunikacjaTowarzyska(){OKim=-1;}//Konstruktor domy�lny - nie wiadomo o kim mowa
	KomunikacjaTowarzyska(const KomunikacjaTowarzyska* Wzor):GeneryczneInfo(Wzor)
		{OKim=Wzor->OKim;}//KONSTRUKTOR KOPIUJ�CY przenosi te� numer "obgadywanego" w�z��

};

//Klasa komunikatu oficjalnego - raporty (w g�r�), wytyczne w d� (podleglosci  adm.)
////////////////////////////////////////////////////////////////////////////////
class KomunikacjaOficjalna:public GeneryczneInfo
{  //Implementacja tego co musi by� wg. interfejsu typu podstawowego linku
	friend class KonstruktorElementowModelu<KomunikacjaOficjalna>;
	static KonstruktorElementowModelu<KomunikacjaOficjalna> WirtKonstr;
	ElementModelu::WirtualnyKonstruktor* VKonstruktor() { return &WirtKonstr;}
 public:
	Komunikat* Klonuj(); //Robi na stercie kopie komunikatu do przechowania lub wstawienia
	virtual bool  UstawRodzaj(const char* );
	const string& Rodzaj();//Gdzie� w danych jest rodzaj tego komunikatu

	//Konstruktor z ustalonym rodzajem komunikatu
	KomunikacjaOficjalna(const char* Rodzaj,unsigned Autor);
	unsigned	KtoJestAutorem(){ return Autor;}

	bool ZrobWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad);

  protected://Wlasciwo�ci obiekt�w klasy  i metody pomocnicze
	void RysujKsztalt(float X,float Y,float Rad,unsigned R,unsigned G,unsigned B); //Rysowanie kszta�tu zale�nego od typu potomnego,
	KomunikacjaOficjalna(){Autor=-1;}//Konstruktor domy�lny ukryty, �eby byle kto go nie u�ywa� :-)
	KomunikacjaOficjalna(const KomunikacjaOficjalna* Wzor):GeneryczneInfo(Wzor) //KONSTRUKTOR KOPIUJ�CY
						{Autor=Wzor->Autor;}
   //POLE:
   unsigned Autor; //Kto jest autorem
  //JAKIE RODZAJE:
  //wytyczne w d�, raporty w g�r�,
  //pro�ba o konsultacje prawn�, o zgod� na kontakt/wspolprace
  //odpowiedzi na to...
  //...CO JESZCZE???
};

//Klasa komunikatu-pakietu do przesy�ania realnych produkt�w z bada� lub fabryk
// potrzebna g��wne ze wzgl�du na odr�znienie wizualizacyjne
////////////////////////////////////////////////////////////////////////////////
class PaczkaProduktow:public GeneryczneInfo
{
	//Implementacja tego co musi by� wg. interfejsu typu podstawowego linku
	friend class KonstruktorElementowModelu<PaczkaProduktow>;
	static KonstruktorElementowModelu<PaczkaProduktow> WirtKonstr;
	ElementModelu::WirtualnyKonstruktor* VKonstruktor() { return &WirtKonstr;}
  public:
	virtual bool  UstawRodzaj(const char* );
	const string& Rodzaj();//Gdzie� w danych jest rodzaj tego komunikatu
	Komunikat* Klonuj(); //Robi na stercie kopie komunikatu do przechowania lub wstawienia

	PaczkaProduktow(const char* Nazwa,unsigned IleWPaczce,float CenaZaSztuke,unsigned Producent);
	unsigned IleSztuk() {return Ile;}
	float    CenaZaSzt() {return Cena;}

	bool ZrobWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad);

  protected://Wlasciwo�ci obiekt�w klasy  i metody pomocnicze
	//Wlasciwo�ci obiekt�w klasy  i metody pomocnicze
	void RysujKsztalt(float X,float Y,float Rad,unsigned R,unsigned G,unsigned B); //Rysowanie kszta�tu zale�nego od typu potomnego,

	PaczkaProduktow(){Ile=1;Cena=1;Producent=-1;}//Konstruktor domy�lny ukryty
	PaczkaProduktow(const PaczkaProduktow* Wzor);//KONSTRUKTOR KOPIUJ�CY
  //POLE - LICZBA W PACZCE, CENA, ADRES PRODUCENTA
	unsigned Producent;
	unsigned Ile;
	float    Cena;
  //JAKIE RODZAJE:
  //* Dziedzina to produkt ktorego dotyczy.
  	friend class ProcesProdukcyjny; //Chyba musi miec dost�p do wn�trza paczki
};

//Klasa komunikatu finansowego - obietnice finansowe i kredytowe, umowy, proformy, przelewy
////////////////////////////////////////////////////////////////////////////////
class KomunikacjaFinansowa:public GeneryczneInfo
{  //Implementacja tego co musi by� wg. interfejsu typu podstawowego linku
	friend class KonstruktorElementowModelu<KomunikacjaFinansowa>;
	static KonstruktorElementowModelu<KomunikacjaFinansowa> WirtKonstr;
	ElementModelu::WirtualnyKonstruktor* VKonstruktor() { return &WirtKonstr;}
 public:
	Komunikat* Klonuj(); //Robi na stercie kopie komunikatu do przechowania lub wstawienia
	virtual bool  UstawRodzaj(const char* );
	const string& Rodzaj();//Gdzie� w danych jest rodzaj tego komunikatu

	KomunikacjaFinansowa(const char* Kategoria,float JakaKwota);//Konstruktor domyslny
	float JakaKwota() { return Kwota;}

	bool ZrobWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad);
  protected://Wlasciwo�ci obiekt�w klasy  i metody pomocnicze
	void RysujKsztalt(float X,float Y,float Rad,unsigned R,unsigned G,unsigned B); //Rysowanie kszta�tu zale�nego od typu potomnego,
	KomunikacjaFinansowa(){Kwota=0;}//Sprywatyzowany konstruktor domyslny
	KomunikacjaFinansowa(const KomunikacjaFinansowa* Wzor):GeneryczneInfo(Wzor)
		{}//KONSTRUKTOR KOPIUJ�CY
	//JAKIE RODZAJE:
	float Kwota;
	//obietnice (propozale um�w) finansowe i kredytowe,
	//umowy, proformy,
	//P�ace, przelewy
	//Raporty/wyniki/transfer wiedzy na koniec umowy o finansowanie bada�
	//Partie produkt�w na rynek
	//pieniadze za produkty z powrotem do producenta
};

//Klasa dla �wiata - realizuje odpowiedzi na wolne poszukiwania, zapytania o produkty
// a tak�e wymian� gotowych produkt�w na konkretn� kas� (czyli "zbyt")
////////////////////////////////////////////////////////////////////////////////
class RynekZbytu:public WezelMacierzowy
{  //Implementacja tego co musi by� wg. interfejsu typu podstawowego w�z�a
	static KonstruktorElementowModelu<RynekZbytu> WirtKonstr;
	ElementModelu::WirtualnyKonstruktor* VKonstruktor() { return &WirtKonstr;}
  public:
	RynekZbytu(){}//Konstruktor domy�lny
	void ChwilaDlaCiebie(); //Endogenne zmiany stan�w, zarz�dzanie procesami itp.
	void InterpretujKomunikat(Komunikat* Co);//Przyjmowanie komunikat�w
	bool ZrobWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad);
  protected://Wlasciwo�ci obiekt�w klasy  i metody pomocnicze
	DziedzinaWKolorze _DaSieSprzedac(const DziedzinaWKolorze CoZaProdukt); //Jak 0 to nie...
  //GRID NISZ RYNKOWYCH:
  //CO ROBI:
  //* realizuje odpowiedzi na swobodne poszukiwania inspiracji - losowo z filtrem
  //* zapytania o zbywalno��/zapotrzebowanie na produkty z danym pomys�em
  //	* Czy jest nisza rynkowa? Jak dok�adne jest dopasowanie?
  //	* Czy jest pusta czy zaj�ta i jak dok�adnie zaj�ta?
  //* a tak�e realizuje wymian� gotowych produkt�w
  // na konkretn� kas� (czyli "zbyt")
  // Kolor (DZIEDZINA) rynku okre�la preferencje
  //float SilaPreferencji;//0..1 - jak bardzo preferencja zmienia rozk�ad (?)
  //albo wczytany grid o okre�lonej zawarto�ci
};


//Klasa dla PUBLIKATORA i UPATENTOWEGO - musi przyjmowac tylko komunikaty oficjalne
// a poza tym zachowuje si� podobnie do klasy bazowej
///////////////////////////////////////////////////////////////////////////////////
class SystemInformacyjny:public WezelMacierzowy
{   //Implementacja tego co musi by� wg. interfejsu typu podstawowego w�z�a
	static KonstruktorElementowModelu<SystemInformacyjny> WirtKonstr;
	ElementModelu::WirtualnyKonstruktor* VKonstruktor() { return &WirtKonstr;}
  public:
	SystemInformacyjny(){}//Konstruktor domy�lny
	//void ChwilaDlaCiebie(); //Endogenne zmiany stan�w, zarz�dzanie procesami itp. - jak klasa bazowa?
	void InterpretujKomunikat(Komunikat* Co);//Przyjmowanie komunikat�w
	//bool ZrobWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad);   - jak klasa bazowa?
};

// Proces kreuj�cy nowe linki - losuje w�z�y i wg. podobie�stwa tworzy link i
// wysy�a nim zajawk� o sobie, nadaje wag� proporcjonaln� do wagi w�z�a (albo...)
// Jak nie utworzy linku to to wysy�a przypomnienie do jakiego� z istniej�cych
// powi�za� spo�ecznych zawieraj�ce losowe bity ze swojej dziedziny.
// Co jaki� czas aktualizuje dziedzin� na podstawie dziedziny swojego w�z�a.
// Jego procedura przechwytywania komunikat�w zajmuje si� odpowiadaniem na "zaczepki"
// o ile pochodz� od kogo� znanego, lub wartego poznania. Kieruje si� tu wag� linku
// spo�ecznego.
////////////////////////////////////////////////////////////////////////////////
class ProcesSpoleczny:public GenerycznyProces
{
   //Implementacja tego co musi by� wg. interfejsu typu Proces
	static KonstruktorElementowModelu<ProcesSpoleczny> WirtKonstr;
	ElementModelu::WirtualnyKonstruktor* VKonstruktor() { return &WirtKonstr;}
	public:
		 ProcesSpoleczny(){}//Konstruktor domyslny wykonuje robot� za pomoc� domy�nego GenerycznyProces()

	void ChwilaDlaCiebie(); //Tworzenie nowych kontakt�w i podtrzymywanie starych
							//Co jaki� czas (7 dni?) zaczyna si� od pocz�tku
	bool InterpretujKomunikat(Komunikat* Co);//Ewentualnie odpowiada na komunikat
	private:
	float Aktywnosc; //Pobierana z odpowiedniej warto�ci w�zla
	//Procedury pomocnicze skracajace zapis tego co istotne
	bool _WyslijAutoprezentacje(unsigned kanal,bool calaprawda); //Wysy�ania informacji o sobie
	unsigned _StworzKanal(unsigned IndInny,float Waga,float Parametr=0);//I nowy kanal z ju� obliczona wag�
														//Jak Parametr =0 to jest losowanie ale z dodatnich!
	public:
	//Pomocnicza funkcja statyczna do u�ycia gdzieb�d� - znajduje zadane powi�zanie spoleczne
	static  unsigned _JestPowiazanySocjalnie(unsigned Startowy,unsigned Docelowy);
};

// Proces obci��aj�cy w�ze� i na ko�cu wysy�aj�cy raport do odbiorcy lub w�z�a
// nadrz�dnego (linkiem Adm.) i wzawiaj�cy dzia�anie od poczatku bez nowego procesu
/////////////////////////////////////////////////////////////////////////////////////
class ProcesRaportowany:public GenerycznyProces
{
   //Implementacja tego co musi by� wg. interfejsu typu Proces
	static KonstruktorElementowModelu<ProcesRaportowany> WirtKonstr;
	ElementModelu::WirtualnyKonstruktor* VKonstruktor() { return &WirtKonstr;}
	public:
		 ProcesRaportowany(){}//Konstruktor domyslny wykonuje robot� za pomoc� domy�nego GenerycznyProces()

	void ChwilaDlaCiebie(); //Endogenne zmiany stan�w, popychanie pracy do przodu itp.
							//Na koniec wysy�a raport i zaczyna si� od pocz�tku
	bool InterpretujKomunikat(Komunikat* Co);//Przesyla dalej raporty, wysy�a kase, inkasuje kas�
	private:
	bool _WyslijRaportOdbiorcy(); //Pomocnicza procedura wysy�ania raportu, gdy proces gotowy
};

// Proces zajmuj�cy si� dawaniem grant�w, doatacji lub po�yczek na procent
/////////////////////////////////////////////////////////////////////////////////////
class ProcesGrantowoPozyczkowy:public GenerycznyProces
{
   //Implementacja tego co musi by� wg. interfejsu typu Proces
	static KonstruktorElementowModelu<ProcesGrantowoPozyczkowy> WirtKonstr;
	ElementModelu::WirtualnyKonstruktor* VKonstruktor() { return &WirtKonstr;}
	public:
		 ProcesGrantowoPozyczkowy();//Konstruktor domyslny wykonuje robot� za pomoc� domy�nego GenerycznyProces()

	void ChwilaDlaCiebie(); //Endogenne zmiany stan�w, popychanie pracy do przodu itp.
	bool InterpretujKomunikat(Komunikat* Co);//Odbiera propozycje, a przydziela kas�
	private:
	//Wewnetrzne w�a�ciwo�ci. Mo�e u�ywa� te� cech w�z�a, np. jego sk�onno�ci do ryzyka
	float Fundusz; //Findusze do podzia�u
	float OczekiwanyZwrot;//0 - dotacja, do 100% po�yczki cz�cowo zwrotne, pow 100% pozyczki komercyjne
};

// Proces badawczy produkuje wyniki naukowe do cel�w publikacji, patentowania,
// lub na czyje� zam�wienie, st�d ma w�a�ciwo�� "ZaplanowanyOdbiorca".
// Mo�e si� wznawia� orza generowa� inne procesy badawcze!!!
////////////////////////////////////////////////////////////////////////////////
class ProcesBadawczy:public GenerycznyProces
{
	friend class JednostkaBadawcza; //�eby m�g� sobie sam poustawia� pola jak go tworzy od nowa
   //Implementacja tego co musi by� wg. interfejsu typu Proces
	friend class KonstruktorElementowModelu<ProcesBadawczy>;
	static KonstruktorElementowModelu<ProcesBadawczy> WirtKonstr;
	ElementModelu::WirtualnyKonstruktor* VKonstruktor() { return &WirtKonstr;}
	public:
	//Konstruktor z parametrami do tworzenia procesu w kodzie
	enum EfektNaukowy { NIEWIEM=-1, PUBLIKACJA, PATENT, RAPORT };
	ProcesBadawczy(const char* Nazwa,EfektNaukowy RodzajProd=NIEWIEM,unsigned ZaplanowanyOdbiorca=-1);//Mo�e od razu zna� odbiorce i rodzaj produktu albo nie

	void ChwilaDlaCiebie(); //Endogenne zmiany stan�w, popychanie pracy do przodu itp.
							//Na koniec tworzy swoj� kopie i sibie przeznacza do skasowania
	bool InterpretujKomunikat(Komunikat* Co);//Przyjmowanie komunikat�w
	protected: //Ukryty przed innymi konstruktor domyslny wykonuje robot� za pomoc� domy�nego GenerycznyProces()
	ProcesBadawczy(){ ZaplanowanyEfekt=NIEWIEM; ZaplOdbiorca=-1;}
	virtual float _KalkulujKosztBadan(); //Furtka do "realistycznego" kalkulowania koszt�w bada�
	bool _WyslijInformacje(unsigned link=-1); //Do rozsylania informacji �e si� czyms takim zajmujemy - socjalna lub oficjalna
	bool _WyslijPublikacje(); //Pomocnicza procedura publikowania - domyslnie szuka w�z�a PUBLIKATOR
	bool _WyslijPatent();     //Pomocnicza procedura publikowania - domyslnie szuka w�z�a UPATENTOWY, publikacja wyklucza patent
	bool _WyslijRaport();     //Pomocnicza procedura raportowania - do odbiorcy albo administracji jak brak
	//G��wna specyficzna zmienna
	private:
	EfektNaukowy ZaplanowanyEfekt;//Jakiego rodzaju efekt ma tu by�
	unsigned     ZaplOdbiorca;//Czy znany jest odbiorca i kto to
};

// Proces zajmuj�cy si� produkcj� i wysy��niem towar�w do odbiorcy lub na rynek
// Wysy�a pod koniec dzia�ania, a potem, je�li wys�anie si� powiod�o to tworzy
// swoj� kopi� nieco bardziej efektywn� (szybsz�)
////////////////////////////////////////////////////////////////////////////////
class ProcesProdukcyjny:public GenerycznyProces
{
   //Implementacja tego co musi by� wg. interfejsu typu Proces
	friend class KonstruktorElementowModelu<ProcesProdukcyjny>;
	static KonstruktorElementowModelu<ProcesProdukcyjny> WirtKonstr;
	ElementModelu::WirtualnyKonstruktor* VKonstruktor() { return &WirtKonstr;}
	public:
	//Konstruktor z parametrami do tworzenia procesu w kodzie
	ProcesProdukcyjny(const char* Nazwa, unsigned IleSztuk,float JakaCena,unsigned IleWPaczce=-1);//Dzieli na 10 pak jak -1
	ProcesProdukcyjny(const ProcesProdukcyjny* Wzor);
	void ChwilaDlaCiebie(); //Endogenne zmiany stan�w, popychanie pracy do przodu itp.
							//Na koniec tworzy swoj� kopie i sibie przeznacza do skasowania
	bool InterpretujKomunikat(Komunikat* Co);//Przyjmowanie komunikat�w
	protected: //Ukryty przed innymi konstruktor domyslny wykonuje robot� za pomoc� domy�nego GenerycznyProces()
	ProcesProdukcyjny();
	bool _WyslijProduktyNaRynek(); //Pomocnicza procedura wysy�ania na rynek przy ko�cu procesu produkcji
	//G��wna specyficzna zmienna
	private:
	unsigned Zapotrzebowanie; //Na ile sztuk produktu szacujemy zapotrzebowanie
	float    Cena;	  //Jaka cena producenta za sztuk�
	unsigned WPaczke; //Po ile sztuk si� miesci w paczce
	unsigned Wyslano; //Ile juz posz�o
};

// Proces poszukuj�cy pomys��w na nowe produkty i "odpalajacy" czasem procesy TT
////////////////////////////////////////////////////////////////////////////////
class ProcesPoszukiwanTT:public GenerycznyProces
{
   //Implementacja tego co musi by� wg. interfejsu typu Proces
	static KonstruktorElementowModelu<ProcesPoszukiwanTT> WirtKonstr;
	ElementModelu::WirtualnyKonstruktor* VKonstruktor() { return &WirtKonstr;}
	public:
		 ProcesPoszukiwanTT();//Konstruktor domyslny wykonuje robot� za pomoc� domy�nego GenerycznyProces()
	bool InterpretujKomunikat(Komunikat* Co);//Pobiera r�ne komunikaty i ��czy z nich pomys�y
									//Tak �eby by�y zgodne z profilem firmy. Od czasu do czasu odpala
									//zwyk�y ProcesTT
	void ChwilaDlaCiebie(); //Endogenne zmiany stan�w - nigdy si� nie ko�czy...
	private:
	//Pomocnicze procedury i cechy procesu...
	void _SprobujOdpalicProcesTT(DziedzinaWKolorze Wzorzec,unsigned Wynalazca=-1);
};

// Proces przygotowuj�cy nowy produkt - musi skompletowa� wszystkie sk�adniki
//////////////////////////////////////////////////////////////////////////////////
class ProcesTransferuTech:public GenerycznyProces
{
   //Implementacja tego co musi by� wg. interfejsu typu Proces
	friend class KonstruktorElementowModelu<ProcesTransferuTech>;
	static KonstruktorElementowModelu<ProcesTransferuTech> WirtKonstr;
	ElementModelu::WirtualnyKonstruktor* VKonstruktor() { return &WirtKonstr;}
	public:
	ProcesTransferuTech(const char* Nazwa,DziedzinaWKolorze Pomysl,unsigned AutorPomyslu,float IleCzasu);
	//Endogenne zmiany stan�w - ko�czy si� sukcesem a  lbo pora�k� (z braku czasu lub �rodk�w)
	//Tu wysy�a r�ne zapytania w zalezno�ci od stanu checklisty. Sykces jest wtedy jak odchaczy wszystko
	void ChwilaDlaCiebie();
	//Poszukuje odpowiedzi na swoje pytania. W razie sukcesu odchacza,
	//a w przypadku konieczno�ci ustalenia formalej koopereacji tworzy linki kooperacyjne.
	//W razie pora�ki zamienia linki koop. na s�abe powi�zania socjalne (gdy ich brak)
	//W razie sukcesu przetwarza linki koop. mocne socjalne (lub wzmacnia)
	//I je�li nie dzia�a na bezpo�rednim producencie to sie� kooperacji mo�e przerzuci� na produ
	//na producenta bo jest konieczna do placenia oplat licencyjnych z zysku.
	bool InterpretujKomunikat(Komunikat* Co);
	void Narysuj();//Ma uzupelnione rysowanie
	static double SkalkulujKosztyProjektu(DziedzinaWKolorze D);
	private:
	ProcesTransferuTech(); //Konstruktor domyslny wykonuje robot� za pomoc� domy�nego GenerycznyProces()
	void RemanentPoczatkowy();//Sprawdzenie czego robi� nie trzeba
	bool RemanentCzySukces();//Sprawdzenie czy si� uda�o
	void ObsluzPorazke(); //W razie pora�ki zamienia linki koop. na s�abe powi�zania socjalne (gdy ich brak) lub obni�a wagi
	void ObsluzSukces();  //Przekazuje produkt do produkcji i przeksztalca sie� link�w
	DziedzinaWKolorze WzorzecZapotrzebowania();//Ktorych wynik�w naukowych wci�� brakuje
	void SprawdzajZapotrzebowania();  //Szprawdza czego potrzebuje
	bool ZarejestrujCoSiePrzyda(DziedzinaWKolorze& D,unsigned Badacz); //Co z bada� badacza si� przyda
	bool OdhaczWyniki(DziedzinaWKolorze& D,unsigned Badacz); //Badacz powinien si� zgadza� z zamowieniem, ale jak uprzedzi� to jego zysk (czesciowy?)
	void _RozeslijPoLinkachSocjalnych(Komunikat* Wzor); //Mno�y komunikat po linkach socjalnych
	//Pola chekclisty
	unsigned AutorPomyslu;
	struct ElementListy {
	double Kiedy;unsigned Wykonawca;bool Check;
	ElementListy(){Kiedy=-1;Wykonawca=-1;Check=false;}
	};
	static const unsigned PRODUCENT=24;    //Gdzie zaznaczamy kto wyprodukuje
	static const unsigned FINANSOWANIE=25; //Gdzie zaznaczamy kto to finansuje
	ElementListy Fragmenty[26];//Sk�d s� poszczeg�lne fragmenty/elementy produktu
};

/********************************************************************/
/*			          OPI  version 2011                             */
/********************************************************************/
/*           THIS CODE IS DESIGNED & COPYRIGHT  BY:                 */
/*            W O J C I E C H   B O R K O W S K I                   */
/*    Instytut Studiow Spolecznych Uniwersytetu Warszawskiego       */
/*        WWW:  http://wwww.iss.uw.edu.pl/borkowski/                */
/*                                                                  */
/*                               (Don't change or remove this note) */
/********************************************************************/
#endif