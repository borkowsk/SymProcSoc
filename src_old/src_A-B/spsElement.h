////////////////////////////////////////////////////////////////////////////////
// Symulator Proces�w Sieciowych/Spolecznych (c) Instytut Studi�w Spo�ecznych
// Uniwersytetu Warszawskiego, ul. Stawki 5/7., 2011 , wborkowski@uw.edu.pl
////////////////////////////////////////////////////////////////////////////////
// Wersja okrojona dla OPI - Projekt "Transfer technologii 2011"
////////////////////////////////////////////////////////////////////////////////
//
//Klasa bazowa definiuj�ca najbardziej podstawowy interfejs do element�w modelu
// G��wnie chodzi o wizualizacje i inicjalizacje z pliku
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
#ifndef spsElementH
#define spsElementH

#include <string> //Musi by� bo u�ywany w deklaracjach

//Pomocnicza unia do manipulacji kolorami/dziedzinami elementow
union DziedzinaWKolorze
{
	unsigned ARGB;
	struct{     //Naiwna kolejno�� ARGB nie koniecznie musi dzia�ac
		unsigned B:8;
		unsigned G:8;
		unsigned R:8;
		unsigned A:8;
	};
	float F; //Jakby kto chcia� widzie� to jako Float
	bool operator == (const DziedzinaWKolorze& s) { return this->ARGB==s.ARGB;}
};
typedef DziedzinaWKolorze DziedzKol;

class Swiat; //Zapowiedz Swiata

class ElementModelu
{
	void operator = (const ElementModelu&); //Blokada bezpo�redniego przypisywania
 public:
	unsigned MojID(); //Zwraca Indeks ustawiony przez �wiat albo jaki� innych identyfikator wobec swiata

	ElementModelu();//Konstruktor domy�lny

	virtual ~ElementModelu();//Destruktor wirtualny, bo b�d� metody wirtualne
	virtual bool   Poprawny()=0; //true je�li jest dobrze zdefiniowany. Jak nie to mo�e zosta� usuni�ty!
	virtual double Waznosc() { return 1; } //Zwraca wsp�lczynnik wa�no�ci obiektu. Np. wag� w�z�a lub linku, priorytet procesu, etc.

	//WSPARCIE WEJSCIA WYJSCIA i dost�pu do danych tekstowych
	//Metoda pobiera wszystkie potrzebne dane z listy string�w. Jak blad to IlePol podaje ktora pozycja listy
	//Jak bez b�edu to IlePol podaje pierwsze pole za list�
	virtual bool ZrobWgListy(const std::string* Lista,unsigned Ile,unsigned& IlePol)=0;
	//Jak ma jakie� dane to mo�na przeczyta� - i np. zwizualizowa�, oraz zmieni�
	virtual unsigned 	IlePolDanych();  //Ile jest w tej chwili p�l
	virtual const char* NazwaPola(unsigned Ktore);
	virtual const char* WartoscPola(unsigned Ktore);
	virtual bool 		UstawPole(unsigned Ktore,const char* NowaWartosc);
	//Metoda aktualizuj�ca zapis stanu obiektu na jego li�cie danych. Po jej wykonaniu
	//pobierane pola musz� by� ju� poprawne i mo�na si� nimi pos�u�y� do zapisu do pliku
	//W szczeg�lno�ci mo�e realokowa� list� "Dane", je�li nie mia�a wcze�niej poprawnego rozmiaru
	virtual void AktualizujListeDanych();

	//Wsp�lny interfejs wizualizacji
	virtual void Narysuj()=0;
	virtual void Wymazuj()=0;
	virtual bool Trafiony(float X,float Y) { return false;} //Np. do inspekcji myszk�


	virtual void ChwilaDlaCiebie()=0; //Daje mu szanse na endogenne zmiany stan�w

	//Kolor czyli "dziedzin�" musi miec kazdy obiekt w u�ywany w SPS
	//Mo�e on znaczy� r�ne rzeczy, ale przede wszystkim jest potrzebny w ka�dej wizualizacji
	//Jego czytanie i pisanie mo�na obudowa� w klasach potomnych w razie potrzeby
	virtual DziedzKol	PodajDziedzine();
	virtual void 	UstawDziedzine(const DziedzinaWKolorze& D);
	virtual void 	UstawDziedzine(unsigned R,unsigned G, unsigned B,unsigned A=0);

 public:
 //Klasa pomocnicza obslugujaca zarz�dzanie danymi tekstowymi obiekt�w modelu
 //a mog�ca te� przej�c zarz�dzanie ich konwersjami i przekszta�cenie w warianty
 class DaneLokalne
 {
	std::string* Dane;
	unsigned 	IleDanych;
	const DaneLokalne& operator = (const DaneLokalne& Wzor); //Prywatne przypisanie - BRAK!
 public:
	DaneLokalne():Dane(0),IleDanych(0){}
	~DaneLokalne();

	void InicjujWgListy(const std::string* Lista,unsigned Ile);
	void InicjujZDanych(const DaneLokalne& Wzor);
	void Realokuj(unsigned Ile);//Rozbudowuje lub skraca list�
	//Akcesory
	unsigned Ile() const { return IleDanych;}
	std::string& operator [] (unsigned index) const ;
	//Przypisania i konwersje na elementy listy
	//Zwracaj� true jak si� uda�o
	bool PrzypiszZ(unsigned gdzie,double co);
	bool PrzypiszZ(unsigned gdzie,long   co);
	//Zwracaj� indeks znaku kt�ry nie pasowa� lub -1 jako sukces
	int KonwertujDo(unsigned zkad,float& naco);
	int KonwertujDo(unsigned zkad,double& naco);
	int KonwertujDo(unsigned zkad,int& naco);
	int KonwertujDo(unsigned zkad,unsigned& naco);
 };

 public:
// Interface konieczny do elastycznego wczytywania z tablicy string�w lub pliku
// !!! Ale nie r�bcie tego sami w domu :-)
 class WirtualnyKonstruktor   //Klasa bazowa wirtualnych konstruktor�w
 {
  protected:
	static void Zarejestruj(const char* NazwaKlasyKonstruowanej,WirtualnyKonstruktor* ); //Obiekt musi istnie� puki jest potrzebny
	static void Wyrejestruj(const char* NazwaKlasyKonstruowanej,WirtualnyKonstruktor* ); //A potem mo�na wyrejestrowa�

  public:
	static const char* 			 ListaNazwTypow(void); //Generuje przegl�dow� list� nazw zarejestrowanych typ�w element�w
	static WirtualnyKonstruktor* DajWirtualnyKonstruktor(const char* NazwaKlasy); //Znajduje wg. zarejestrowanej nazwy typu

	virtual  const char* NazwaTypu()=0; //Nazwa typu dla tego obiektu konstruuj�cego
	virtual  unsigned 	 IleNazwPol();//Dwa pola s� obslugiwane nawet jak nie ma nazw!
	virtual  const char* NazwaPola(unsigned i);//Bezpieczniejsze w u�yciu, a najlepiej uzywac "nazwy pola" z ElementModelu

	virtual  unsigned&    Flagi()=0; //32 bity na jakie� nieprzewidziane wcze�niej informacje konfiguracyjne
	static const unsigned VFLAG=0x3; //Najmlodsze bity zarezerwowany na tryb wizualizacji
									 //* 0 nie powinien pokazywa� obiekt�w danej klasy!
									 //* 1 tylko kropki
									 //* 2 domyslny spos�b uproszczony
									 //* 3 sposob wirtualny, zwykle graficznie rozbudowany

	//NAJWA�NIEJSZY ELEMENT -  TWORZENIE OBIEKTU ODPOWIEDNIEGO TYPU POCHODNEGO OD Element
	virtual  ElementModelu*  KonstruktorWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad)=0;

  public:
	DaneLokalne Nazwy; //Nazwy p�l, s� te� dost�pne bezpo�rednio, ale mog� by� puste!!!
 };

//Kazda klasa potomna musi mie� klas� pe�ni�ca rol� wirtualnego konstruktora tworz�cego takie obiekty na stercie
//Obiekt takiego szablonu trzeba zadeklarowa� i zainicjowac jako pole static ka�dej klasy  potomnej dla "Element"
 template<class JakiElement>
 class KonstruktorElementowModelu:public  WirtualnyKonstruktor
 {
	std::string MojaNazwa;
	unsigned    FLAGI;
  public:
	KonstruktorElementowModelu(const char* NazwaKlasyKonstruowanej):MojaNazwa(NazwaKlasyKonstruowanej),FLAGI(VFLAG)
		{ Zarejestruj(MojaNazwa.c_str(),this); }

	~KonstruktorElementowModelu()  { Wyrejestruj(MojaNazwa.c_str(),this); }

	const char* NazwaTypu()	{ return MojaNazwa.c_str(); }
	unsigned&   Flagi()     { return FLAGI; }

	ElementModelu*  KonstruktorWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad)
	{
	   JakiElement* Tworzony=new JakiElement();
	   if(Tworzony->ZrobWgListy(Lista,Ile,Blad))
				 return Tworzony;
		  else  { delete Tworzony; return NULL;}
	}
 };

 // POLA chronione i publiczne stanowi�ce dane klasy Element
 ///////////////////////////////////////////////////////////////////////////////
 protected:
	friend class Swiat; //Ma dost�p do p�l prywatnych - zw�aszcza do zapisu indeksu

	unsigned _MojIndeks;//Pole jest ustawiane przez Swiat(!!!)
				//Pozwala si� obiektowi zlokalizowa� w strukturach �wiata

	DziedzKol Col; //Ka�dy "Element" SPS musi mie� sw�j g��wny kolor, kt�ry mo�e znaczy� r�ne rzeczy
				// ale przede wszystkim jest potrzebny w ka�dej wizualizacji
 public:
	//Obsluga danych tekstowych - jako udost�pniony obiekt, co nie jest zbyt bezpieczne (!)
	DaneLokalne Dane; //I kazdy ElementModelu ma taki jeden obiekt  danych tekstowych

	//Obsluga wirtualnego kreowania
	//Dla usprawnienia klasa potomna mo�e zwraca� adres swojego wirtualnego konstruktora
	//albo NULL oznaczaj�cy �e trzeba go szuka� wg. zarejestrowanej nazwy
	//lub �e go nie ma ... (co mo�e stanowi� problem)
	virtual WirtualnyKonstruktor* VKonstruktor() { return NULL; }
 	virtual unsigned              VWidocznosc(); //Bazuj�ca na FLAGS informacja o widocznosci
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
#endif    // opiElementH

