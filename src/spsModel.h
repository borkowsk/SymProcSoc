/// \file
/// \brief Główne deklaracje parametrów modelu, globalnych funkcji i struktur danych
///        -----------------------------------------------------------------------------
///
/// \details
///              ...
///     ## (c)
///     Symulator Procesów Sieciowych/Społecznych (c) Instytut Studiów Społecznych
///     Uniwersytetu Warszawskiego, ul. Stawki 5/7., 2011 , wborkowski@uw.edu.pl
/// \date
///     2022.11.03 (last updated)
//*////////////////////////////////////////////////////////////////////////////////

//
//
// //////////////////////////////////////////////////////////////////////////////
#ifndef __SPS_MODEL_MAIN_HEADER_H__
#define __SPS_MODEL_MAIN_HEADER_H__

/// \brief Parametry konfiguracyjne SPS
/// \details mogą być zmienione parametrami wywołania programu,
///          a używane są od inicjalizacji obiektu klasy `Swiat`.
///          TODO: Powinny być raczej wewnątrz klasy `Swiat` (!?!?!)
const unsigned 	NumerKoloruTla=255+200; ///< const\y nie do zmieniania!

extern int 		VerboseInput; ///< default=1;   \brief Parametr konfiguracyjny  VERBOSE
extern int 		   hor;       ///< default=790; \brief Parametr konfiguracyjny  HORIZONTAL
extern int 		   ver;       ///< default=570; \brief Parametr konfiguracyjny  VERTICAL
extern double   JakieRwProcWidth;      ///< default=5;  \brief Parametr konfiguracyjny RADIUS
extern unsigned DefMaxLiczbaWezlow;    ///< default=2;  \brief Ograniczenie na liczbę węzłów. Liczby od 0 do 10 oznaczają wielokrotność liczby początkowej...
extern unsigned DefMaxLiczbaLinkow;    ///< default=-1; \brief Ograniczenie na liczbę linków. -1 dla linków oznacza graf pełny
extern unsigned DefMaxLiczbaKomunikatow;    ///< default=10*1024; \brief Ograniczenie na liczbę komunikatów (połączenia ???)
extern unsigned DefLiczbaProcesowNaWezel;   ///< default=10*1024; \brief Ograniczenie na liczbę procesów węzła

/// Wartośc zaawansowania wymuszająca skasowanie procesu przy najbliższej okazji.
///          TODO: Powinno być raczej wewnątrz klasy `Proces` (!?!?!)
const double PROCES_NAD_ZAAWANSOWANY=2.0;

#include "spsElement.h"  //Klasa bazowa dla wszystkich elementów modelu SPS

// Klasy bazowe dla bardziej konkretnych elementów modelu, ale nadal
// "pure virtual" - potrzebne tylko jako typy elementów tablic "Świata"
// ////////////////////////////////////////////////////////////////////
class Komunikat; //Deklaracja zapowiadająca komunikat
class Proces;    //Deklaracja zapowiadająca proces

class WezelSieci:public ElementModelu
{
  public:
    // Z interfejsu wszelkich elementów
    // ////////////////////////////////
	virtual ~WezelSieci(){}    ///< \brief Destruktor wirtualny, bo są metody wirtualne
	virtual bool Poprawny()=0; ///< \brief Czy węzeł poprawny. Jak nie to może zostać usunięty! \return true jeśli jest dobrze zdefiniowany.
	virtual void Narysuj()=0;  ///< \brief Metoda rysująca
	virtual void Wymazuj()=0;  ///< \brief Metoda czyszcząca z wizualizacji
	virtual void ChwilaDlaCiebie(){}; ///< \brief Metoda zmiany stanu. Daje mu szanse na endogenne zmiany stan�w

    // Specyficzne tylko dla węzłów
    // ////////////////////////////
	virtual const char* Nazwa()=0;   ///< \brief Akcesor nazwy. Obiekt musi posiadać jakąś nazwę
	virtual double ZOrder()=0;       ///< \brief Akcesor kolejności wyświetlania
	virtual double X()=0;            ///< \brief Akcesor X środka węzła
	virtual double Y()=0;            ///< \brief Akcesor Y środka węzła
	virtual double R(double angle)=0;  ///< \brief Akcesor promienia otaczającego okręgu lub elipsy
	virtual DziedzKol* Dziedzina()=0;  ///< \brief Akcesor "dziedziny" kodowanej kolorystycznie

    ///  \brief Metoda interpretacji komunikatu
    /// \details Może zareagować, może coś zrobić z komunikatem, ale przechowywać może tylko kopie
    ///          komunikatu bo oryginał jest zarządzany przez świat i może zniknąć po powrocie z tej metody.
    /// \note    Wersja domyślna nie robi w ogóle nic.
	virtual void InterpretujKomunikat(Komunikat* Co){}

};


/// \brief Klasa procesu wykonywanego na węźle
/// \details Może to być tylko atrapa procesu, ale klasy potomne mogą wykonywać jakieś realne działania.
class Proces:public ElementModelu
{
  public:
    // To co byc musi
    // ///////////////
	Proces():_MojWezel(-1),_IleKrokowDoKonca(0){} ///< Konstruktor musi zapewnić domyślną NIEPOPRAWNOŚĆ indeksu węzła
	virtual ~Proces(){}                           ///< Destruktor wirtualny, bo są metody wirtualne
	virtual bool Poprawny()=0;        ///< \brief test poprawności \return `true` jeśli jest dobrze zdefiniowany
    virtual void Narysuj()=0;         ///< \brief Metoda rysująca
    virtual void Wymazuj()=0;         ///< \brief Metoda czyszcząca z wizualizacji
	virtual void ChwilaDlaCiebie(){}; ///< \brief Możliwość zmiany stanu procesu albo zrobienia czegoś realnego

    // Specyficzne dla Procesu-projektu
    // ////////////////////////////////
	virtual double   JakZaawansowany()=0;  ///< \brief Stopień zaawansowania procesu. \return 0 po starcie, a jak 1 to zakończony
	virtual double   Priorytet()=0;        ///< \brief Priorytet procesu. Im wyższa wartość tym proces więcej działa.
	virtual unsigned Procesor(){return _MojWezel;} ///< \brief Indeks węzła który realizuje proces
			float    DoKonca(){ return _IleKrokowDoKonca;} ///< \brief Ile (kroków) do oczekiwanego konca.

    /// \brief Przetwarzanie komunikatu skierowanego do tego procesu
    /// \details Tak jak węzeł, może coś zrobić z komunikatem, ale przechowywać może tylko kopie
    ///          bo oryginał komunikatu może zniknąć po powrocie z tej metody.
    /// \return  Zwraca `true`, jeśli przetworzył komunikat
    virtual bool InterpretujKomunikat(Komunikat* Co){return false;}

  private: // POLA
	friend class Swiat;
	unsigned _MojWezel; //To pole MUSI być jakoś dostępne dla klasy `SWIAT`

  protected:
	float _IleKrokowDoKonca; //Ile kroków czasu zostało do deadlinu. `Swiat::` odejmuje co krok.

	void __ChceWezelNr(unsigned indeks) // Tylko do ustawiania przy wczytaniu -
	{  _MojWezel=indeks; }              // jedynie procedura inicjująca świat robi z tego użytek!!!
};

/// \brief Interface powiązania elementów
class Powiazanie:public ElementModelu
{
  public:
	virtual ~Powiazanie(){}           ///< \brief Destruktor wirtualny
	virtual bool Poprawny()=0;        ///< \brief Test poprawności. \return true je�li jest dobrze zdefiniowany.
    virtual double ZOrder()=0;        ///< \brief Do ustalania kolejnosci wyswietlania
    virtual void Narysuj()=0;         ///< \brief Metoda rysująca
    virtual void Wymazuj()=0;         ///< \brief Metoda czyszcząca z wizualizacji
	virtual void ChwilaDlaCiebie(){}; ///< \brief Daje mu szanse na endogenne zmiany stan�w

    // Specyficzne dla Powiązania (linku)
    // //////////////////////////////////
	virtual unsigned Poczatek()=0;    ///< \brief Co jest początkiem połączenia \return indeks węzła
	virtual unsigned Koniec()=0;      ///< \brief Co jest koncem  połączenia \return indeks węzła
	virtual bool  Kierunkowy()=0;     ///< \brief Czy połączenie symetryczne? Wtedy niema znaczenia co jest końcem co początkiem.

    /// \brief Ustala czy dany komunikat może przejść tym łączem
    /// \details Przelicza położenia wzdłuż linku
    ///          Może ponadto uwzględniać specyfikę komunikatu
	virtual bool  Akceptacja(Komunikat* Co)=0;

	virtual void  PodajPozycje(double D,bool KierunekZwykly,
							   double& X,double& Y,Komunikat* K=NULL)=0;
};

/// \brief Interface komunikatu
class Komunikat:public ElementModelu
{
  public:
	virtual ~Komunikat(){}  ///< Destruktor wirtualny, bo są metody wirtualne

	virtual bool Poprawny()=0; //true je�li jest dobrze zdefiniowany (wci�� istnieje link, nadawca i odbiorca etc...)
	virtual void Narysuj()=0;
	virtual void Wymazuj()=0;
	virtual void ChwilaDlaCiebie(){}; //Daje mu szanse na endogenne zmiany stan�w
    // Specyficzne dla Komunikatu (Message)
    // ////////////////////////////////////
    virtual const std::string& Rodzaj()=0;  //Jak nic nie ma to zwraca "???"
    virtual bool  UstawRodzaj(const char* )=0;//A jak typ nie udost�pnia zmiany to zwraca false
	virtual float  JakDostawa()=0;//0 po starcie, jak 1 to ju� dotar�=0
	virtual float  JakiTermin()=0;//W ile krok�w powinien zosta� dostarczony?
	virtual unsigned Kanal()=0; //Po ktorym po��czeniu czyli linku
	virtual bool 	 KierunekZgodny()=0;//Czy idzie zgodnie z nominalnym kierunkiem linku czy pod pr�d
	virtual unsigned Nadawca()=0; //Od kogo ...
	virtual unsigned Odbiorca()=0;//... do kogo.
	virtual Komunikat* Klonuj()=0; //Robi na stercie kopie komunikatu do przechowania lub wstawienia
	//Adresowanie w r�zny spos�bw
	virtual bool 	Zwrotnie(float _Szybkosc=0)=0;//Mo�na odwr�ci� komunikat - zw�aszcza jak jest to klon
	virtual bool  	Zaadresuj(unsigned _Nadawca,unsigned _Odbiorca,float _Szybkosc=0)=0; //Zwracaj� true jak odnajdzie
	virtual bool  	Zaadresuj(unsigned _Powiazanie,bool KierunekZwykly,float _Szybkosc=0)=0;//...poprawny kana� komunikacji
};

//Klasa interface'u Swiata - zarz�dzaj�ca danymi calej symulacji
///////////////////////////////////////////////////////////////////
//UWAGA - de facto SINGLETON!!! Dane s� tylko w jednej kopii - ka�dy �wiat ma do
//nich dost�p
//Chodzi o definicje interface'u, kt�rego implementacje mo�na �atwo zmieni�

class Swiat//:public ElementModelu - PO CO?
{
	//!!!
	//Wszystkie dane s� w jednej kopii poza deklaracj� klasy !!!
	//!!!
	Swiat();         //Konstruktor  prywatny - bo tylko jeden �wiat
	public:
	static double SkalujX(); //Bardzo cz�sto uzywane
	static double SkalujY(); //Mo�e inliny kiedy� zrobie?
	//Staramy sie miec tylko jeden Swiat :-)
	static Swiat Ten; //!!!
	static const unsigned INVINDEX=unsigned(-1); //Marker wadliwych indeks�w

	//Cz�� wirtualna ze wzgl�du na dziedziczenie - mo�e niepotrzebnie tak
	virtual ~Swiat();//Destruktor wirtualny
	//Wizualizacyjne - Co wyswietla� w ka�dym kroku i definicja procedur
	static  bool WyswietlajWezly;
	static  bool WyswietlajProcesy;
	static  bool WyswietlajLinki;
	static  bool WyswietlajInformacje;
	//Kt�re pole s�u�y do opisu link�w a kt�re w�z��w
	static  unsigned OpisWezlow;
	static  unsigned OpisLinkow;
	virtual void Narysuj(); //Odrysowanie ca�o�ci
	virtual void Wymazuj(); //Wyczyszczenie calo�ci - mo�e by� "na skr�ty"
	virtual void ChwilaDlaCiebie(){Krok_modelu();}; //Daje mu szanse na endogenne zmiany stan�w

	//Poniewa� dane s� w jednej to wszystkie inne "metody" mog� by� "static"
	///////////////////////////////////////////////////////////////////////////
	static bool ForceTolerant;//Przy wczytywaniu danych tylko ostrzega o niepoprawnych linkach
	static bool Inicjalizacja(const char* PlikWejsciowy,const char DelimiterDanych);   //Wczytanie i/lub wylosowanie struktury modelu
	static bool Krok_modelu();     //Wykonanie kroku modelu - mo�e by� wizualizacja w trakcie
	static bool Wizualizacja_po_kroku(); //Wykonanie pe�nej wizualizacji modelu w danej chwili
	static bool Statystyka_po_kroku(); //Wyliczenie pe�nej statystyki modelu w danej chwili
	static bool Status_po_kroku();  //Na zakonczenie kroku trzeba odnowi� graficzny wiersz statusu

	static double NumerKroku(); //Mo�e kroki b�d� te� u�amkowe? A w og�le double ma wi�cej cyfr znacz�cych
	static ElementModelu* Inspekcja(int x,int y,bool Wizual=false);//Odnajduje element wg. pozycji pikselowej
							//i opcjonalnie od razu wy�wietla w oknie wizualizacji pe�ne informacje o nim

	// Dost�py do tablic - mog� rzuci� wyj�tkiem albo przerwa� przy b��dzie dost�pu
	// albo w "�agodnej" formie mo�e zwr�ci� NULL, tak jak w przypadku pustego
	// czyli np. "usmierconego" slotu w tablicy
	static WezelSieci*  Wezel(unsigned Ktory);
	static Powiazanie*  Lacze(unsigned Ktory);
	static Komunikat*   Info(unsigned Ktory);
	static Proces*		Proc(unsigned Ktory,unsigned KtoryWezel=INVINDEX);//Zwi�zane z wez�ami
						//!!!Indeksy proces�w s� poprawne tylko tymczasowo

	//Pomocnicze funkcje wyszukiwawcze
	static unsigned 	ZnajdzIndeksWezla(const char* Nazwa); //Zwraca INVINDEX) jak nie znajdzie
	static unsigned     NajpilniejszyProc(unsigned KtoryWezel,unsigned* IleRealnie=NULL,bool Posortuj=false);//Daje indeks procesu o najwy�szym priorytecie
															//Mo�e zwr�cic -1 jak nie ma proces�w, a opcjonalnie ich liczb� na *IleRealnie
	//Aktualne rozmiary tablic - ale w ich obr�bie mog� by� puste sloty
	static unsigned IleWezlow();
	static unsigned IlePowiazan();
	static unsigned IleInformacji();
	static unsigned IleProcesow(unsigned KtoryWezel=INVINDEX);//jak INVINDEX to ogolnie, jak numer w�z�a to lokalnie

	//Liczba jeszcze dost�pnych slotow
	static 	unsigned IleMoznaWezlow();
	static 	unsigned IleMoznaPowiazan();
	static 	unsigned IleMoznaInformacji();
	static  unsigned IleMoznaProcesow(unsigned KtoryWezel=INVINDEX);//jak INVINDEX to ogolnie, jak numer w�z�a to lokalnie

	//UWAGA NA ZMIANY STRUKTURY!
	//Niekt�re dane do wizualizacji mog� by� liczone rzadziej ni� co krok/klatk� obrazu -
	//np. usuni�cie/dodanie wez�a, usuniecie/dodanie linku
	//ale czasem te zmiany s� w obr�bie obiektow podrz�dnych i wtedy trzeba poinformowa�
	//klas� "�wiata" �e wszystko trzeba policzy� od nowa.
	//Dotyczy to na pewno zmian warto�ci "ZOrder"
	static void UwagaZmienionoStrukture(); //Wstawianie i usuwanie wezlow jest ...

	//Funkcje wstawiaj�ce zwracaj� pozycje wstawienia i PRZEJMUJ� ZARZ�D PAMI�CI�
	// - wstawiane elementy musz� by� alokowane na stercie (przez "new")
	//Jak si� nie uda z jaki� powod�w wstawi� to wstawiany obiekt jest deletowany!
	//Nie jest wi�c bezpiecznie u�ywa� roboczego wska�nika po wstawieniu
	//Pole "propozycja" pozwala zaoszcz�szcz�dzi� szukania miejsca je�li trafia
	static unsigned WstawWezel(WezelSieci* Jaki,unsigned propozycja=INVINDEX);
	static unsigned WstawLacze(Powiazanie* Jaki,unsigned propozycja=INVINDEX);
	static unsigned WstawInfo(Komunikat* Jaki,unsigned propozycja=INVINDEX);
	static unsigned WstawProc(Proces* Jaki,unsigned KtoryWezel);//Musi by� jasne kt�ry w�ze�

	//UWAGA NA USUWANIE!  GENERALNIE DOSY� DROGIE
	//* Usuwanie w�z�a usuwa te� wszystkie zwi�zane z nim ��cza, komunikaty i procesy
	//* Usuwanie ��cza usuwa zwi�zane z nim komunikaty

	//Tansze funkcje usuwaj�ce wg. indeksu (lub uchwytu, zale�nie od implementacji
	static void UsunWezel(unsigned Ktory);
	static void UsunLacze(unsigned Ktory);
	static void UsunInfo(unsigned Ktory);
	static void UsunProc(unsigned Ktory,unsigned KtoryWezel=INVINDEX);

	//Funkcja usuwaj�ce wg. wska�nika - kosztowne bo szukaj�
	static unsigned UsunWezel(WezelSieci* Jaki);
	static unsigned UsunLacze(Powiazanie* Jaki);
	static unsigned UsunInfo(Komunikat* Jaki);
	static unsigned UsunProc(Proces* Jaki,unsigned KtoryWezel=INVINDEX);

	static bool _UstalLiczbeInfo(unsigned IleMax);  //W celu zmiany w konfiguruj
	private:  //Zmiany rozmiar�w struktur potrzebne s� tylko wewn�trznie (na razie?)
	static bool _UstalLiczbeLaczy(unsigned IleMax);
	static bool _UstalLiczbeWezlow(unsigned IleMax);

	static bool _UstalPoczLiczbeProc(unsigned IleMaxDlaWezla);
};


//Inne Globalne funkcje - je�li zwracaj� false to znaczy �e si� co� nie uda�o
bool konfiguruj(const char* parametr,const char* wartosc=0); //Przetwarza pojedynczy "configpar" o postaci XXX=YYY lub XXX i YYY jako drugi
bool czytaj_parametry(int argc,const char* argv[]); //Przetwarza ca�� liste parametrow
bool podaj_nazwe_pliku(char* Bufor,unsigned size,const char* Monit="Nazwa pliku");

//RANDOMIZER MUSI BY� WYBRANY RAZ NA APLIKACJE!
#define USES_STDC_RAND
#include "random.h"

/* *******************************************************************/
/*			            SPS  version 2022                            */
/* *******************************************************************/
/*             THIS CODE IS DESIGNED & COPYRIGHT  BY:                */
/*              W O J C I E C H   B O R K O W S K I                  */
/*     Instytut Studiów Społecznych Uniwersytetu Warszawskiego       */
/*        RG:https://www.researchgate.net/profile/Wojciech-Borkowski */
/*        GitHub: https://github.com/borkowsk                        */
/*                                                                   */
/*                               (Don't change or remove this note)  */
/* *******************************************************************/
#endif //__SPS_MODEL_MAIN_HEADER_H__ - KONIEC NAGŁÓWKA

