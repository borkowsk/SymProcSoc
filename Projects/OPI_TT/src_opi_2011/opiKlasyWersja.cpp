// Transfer technologii 2011 - w�a�ciwa implementacja modelu     24-10-2011
////////////////////////////////////////////////////////////////////////////////
// Klasy modelu OPI - IMPLEMENTACJA TEGO CO BY� W NICH MUSI
// ze wzgl�du na wymagania interfac�w pakietu SPS
////////////////////////////////////////////////////////////////////////////////

#include "opiKlasyWersja.hpp"
#include "INCLUDE/wb_smartlog.h"
using namespace wbrtm;

// OBIEKTY WIRTUALNYCH KONSTRUKTOR�W DLA KA�DEJ Z KLAS MODELU
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Proces zajmuj�cy si� produkcj� i wysy��niem towar�w do odbiorcy lub na rynek
// Wysy�a pod koniec dzia�ania, a potem, je�li wys�anie si� powiod�o to tworzy
// swoj� kopi� nieco bardziej efektywn� (szybsz�)
////////////////////////////////////////////////////////////////////////////////
ProcesProdukcyjny::KonstruktorElementowModelu<ProcesProdukcyjny> ProcesProdukcyjny::WirtKonstr("ProductProc");

// Proces obci��aj�cy w�ze� i na ko�cu wysy�aj�cy raport do odbiorcy lub w�z�a
// nadrz�dnego (linkiem Adm.) i wzawiaj�cy dzia�anie od poczatku bez nowego procesu
/////////////////////////////////////////////////////////////////////////////////////
ProcesRaportowany::KonstruktorElementowModelu<ProcesRaportowany> ProcesRaportowany::WirtKonstr("ReportProc");

// Proces badawczy produkuje wyniki naukowe do cel�w publikacji, patentowania,
// lub na czyje� zam�wienie, st�d ma w�a�ciwo�� "ZaplanowanyOdbiorca"
///////////////////////////////////////////////////////////////////////////////////////////////
ProcesBadawczy::KonstruktorElementowModelu<ProcesBadawczy> ProcesBadawczy::WirtKonstr("ResearchProc");

// Proces kreuj�cy nowe linki - losuje w�z�y i wg. podobie�stwa tworzy link i
// wysy�a nim zajawk� o sobie, nadaje wag� proporcjonaln� do wagi w�z�a (albo...)
// Jak nie utworzy linku to to wysy�a przypomnienie do jakiego� z istniej�cych
// powi�za� spo�ecznych zawieraj�ce losowe bity ze swojej dziedziny.
////////////////////////////////////////////////////////////////////////////////
ProcesSpoleczny::KonstruktorElementowModelu<ProcesSpoleczny> ProcesSpoleczny::WirtKonstr("SocialProc");

// Proces poszukuj�cy pomys��w na nowe produkty i "odpalajacy" czasem procesy TT
//////////////////////////////////////////////////////////////////////////////////
ProcesPoszukiwanTT::KonstruktorElementowModelu<ProcesPoszukiwanTT>
								ProcesPoszukiwanTT::WirtKonstr("TTSearchProc");

// Proces przygotowuj�cy nowy produkt - musi skompletowa� wszystkie sk�adniki
//////////////////////////////////////////////////////////////////////////////////
ProcesTransferuTech::KonstruktorElementowModelu<ProcesTransferuTech>
								ProcesTransferuTech::WirtKonstr("TTransferProc");

// Proces zajmuj�cy si� dawaniem grant�w, dotacji lub po�yczek na procent
/////////////////////////////////////////////////////////////////////////////////////
ProcesGrantowoPozyczkowy::KonstruktorElementowModelu<ProcesGrantowoPozyczkowy>
							ProcesGrantowoPozyczkowy::WirtKonstr("FinSourceProc");

//Klasa powi�zania administracyjnego - przepuszcza w g�r� raporty,
// a w d� wytyczne oraz pieni�dze
////////////////////////////////////////////////////////////////////////////////
PodlegloscOrganizacyjna::KonstruktorElementowModelu<PodlegloscOrganizacyjna>
	PodlegloscOrganizacyjna::WirtKonstr("AdminLink");

// Klasa kooperacji sformalizowanej - przepuszcza w raporty - efekty dzia�a�
// oraz pieni�dze
////////////////////////////////////////////////////////////////////////////////
FormalnaKooperacja::KonstruktorElementowModelu<FormalnaKooperacja>
	FormalnaKooperacja::WirtKonstr("CoopLink");

//Klasa powi�zania towarzyskiego, zanikaj�cego powoli gdy nie u�ywany
// ASYMETRYCZNA - TAKIE LINKI S� POD DWA BO ZAUFANIE I WAGA POWI�ZANIA
// ZAZWYCZAJ JEST RӯNA Z PUNKTU WIDZENIA OBU PARTNER�W!
////////////////////////////////////////////////////////////////////////////////
KontaktTowarzyski::KonstruktorElementowModelu<KontaktTowarzyski>
	KontaktTowarzyski::WirtKonstr("SocialLink");

//Klasa komunikatu oficjalnego - raporty (w g�r�), wytyczne w d� (podleglosci  adm.)
////////////////////////////////////////////////////////////////////////////////
KomunikacjaTowarzyska::KonstruktorElementowModelu<KomunikacjaTowarzyska>
	KomunikacjaTowarzyska::WirtKonstr("SocialInfo");

//Klasa komunikatu oficjalnego - raporty (w g�r�), wytyczne w d� (podleglosci  adm.)
////////////////////////////////////////////////////////////////////////////////
KomunikacjaOficjalna::KonstruktorElementowModelu<KomunikacjaOficjalna>
	KomunikacjaOficjalna::WirtKonstr("OficialInfo");

//Klasa komunikatu-pakietu do przesy�ania realnych produkt�w z bada� lub fabryk
// potrzebna g��wne ze wzgl�du na odr�znienie wizualizacyjne
////////////////////////////////////////////////////////////////////////////////
PaczkaProduktow::KonstruktorElementowModelu<PaczkaProduktow>
	PaczkaProduktow::WirtKonstr("PackInfo");

//Klasa komunikatu finansowego - obietnice finansowe i kredytowe, umowy, proformy, przelewy
////////////////////////////////////////////////////////////////////////////////
KomunikacjaFinansowa::KonstruktorElementowModelu<KomunikacjaFinansowa>
	KomunikacjaFinansowa::WirtKonstr("FinancInfo");

//Klasa dla �wiata - realizuje odpowiedzi na wolne poszukiwania, zapytania o produkty
// a tak�e wymian� gotowych produkt�w na konkretn� kas� (czyli "zbyt")
////////////////////////////////////////////////////////////////////////////////
RynekZbytu::KonstruktorElementowModelu<RynekZbytu>
	RynekZbytu::WirtKonstr("MarketNode");

//Klasa dla PUBLIKATORA i UPATENTOWEGO - musi przyjmowac tylko komunikaty oficjalne
// a poza tym zachowuje si� podobnie do klasy bazowej
///////////////////////////////////////////////////////////////////////////////////
SystemInformacyjny::KonstruktorElementowModelu<SystemInformacyjny>
	SystemInformacyjny::WirtKonstr("InforNode");

//Klasa dla badacza i/lub zespo�u badawczego
////////////////////////////////////////////////////////////////////////////////
JednostkaBadawcza::KonstruktorElementowModelu<JednostkaBadawcza>
	JednostkaBadawcza::WirtKonstr("ResearchUNode");

//Klasa dla Instytutu, Wydzia�u, Uniwersytetu itp.
////////////////////////////////////////////////////////////////////////////////
AdministracjaNaukowa::KonstruktorElementowModelu<AdministracjaNaukowa>
	AdministracjaNaukowa::WirtKonstr("ResAdminNode");

//Klasa wykonwacy/producenta ostatecznego produktu
////////////////////////////////////////////////////////////////////////////////
Firma::KonstruktorElementowModelu<Firma>
	Firma::WirtKonstr("BusinessNode");

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
BOTT::KonstruktorElementowModelu<BOTT> BOTT::WirtKonstr("BOTTNode");
UOTT::KonstruktorElementowModelu<UOTT> UOTT::WirtKonstr("UOTTNode");


// Metody na potrzeby wczytywania i zapisywania plik�w danych
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
Komunikat* KomunikacjaTowarzyska::Klonuj(){return new KomunikacjaTowarzyska(this);}
Komunikat* KomunikacjaOficjalna::Klonuj(){return new KomunikacjaOficjalna(this);}
Komunikat* KomunikacjaFinansowa::Klonuj(){return new KomunikacjaFinansowa(this);}
Komunikat* PaczkaProduktow::Klonuj(){return new PaczkaProduktow(this);}

void JednostkaBadawcza::AktualizujListeDanych()
//Do zapisu i wy�wietlania inspekcyjnego
{
	unsigned Poz=8;           //PROWIZORKA!!! Na razie na "sile"
	//Finans. bie��ce  = u float    FinanseBiezace; //Ile firma ma kasy aktualnie LUB UPROSZCZONA "ZAMO�NO��"
	Dane.PrzypiszZ(Poz+8,FinanseSwobodne);
	//Finanse zaplan  = u float    FinanseZaplanowane;//Ile firma ma obiecanych �rodk�w kredytowych
	Dane.PrzypiszZ(Poz+9,FinanseObiecane );
	//D�ugi  = u float 	 Dlugi ;//Ile firma ma do sp�aty. Im wi�cej tym mniejsza szansa �e co� dostanie nowego
	Dane.PrzypiszZ(Poz+10,Dlugi );
}

bool JednostkaBadawcza::ZrobWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad)
{
	unsigned Poz;
	if(!GenerycznyWezelSieci::ZrobWgListy(Lista,Ile,Poz)) {Blad=Poz; return false;} //Nie uda�o si�
 //Ile Etat�w = unsigned IleEtatow;//Ile etat�w. Co najmniej 1, 0 - "duch" zespo�u - plan utworzenia
	if(Dane.KonwertujDo(Poz+0,IleEtatow)!=-1) {Blad=Poz+0; return false;}
 //Efektywnosc = float    Efektywnosc;//0..2. Czy dzia�a na 50% wydajno�ci czy na 150%. Zale�y od motywacji
	if(Dane.KonwertujDo(Poz+1,Efektywnosc)!=-1) {Blad=Poz+1; return false;}
 //Aktywno�� = float    Proaktywnosc;//Aktywno��? 0..1. Podtrzymywanie aktywno�ci zewn�trznej, tworzenie link�w
	if(Dane.KonwertujDo(Poz+2,Proaktywnosc)!=-1) {Blad=Poz+2; return false;}
 //Centralno�� = float    Centralno��;//0..1. Jak bardzo aktywno�� (zewn�trzna?) zale�y od szefa/szef�w
	if(Dane.KonwertujDo(Poz+3,Centralnosc)!=-1) {Blad=Poz+3; return false;}
 //Probiznesowosc = float    Probiznesowosc;//0 - czysta nauka, 0.5-badania patentowalne/licenjonowalne, 1 - czysty biznes - szybkie wdra�anie
	if(Dane.KonwertujDo(Poz+4,Probiznesowosc)!=-1) {Blad=Poz+4; return false;}
 //Prolegalnosc = float    Prolegalnosc;//????Uczciwo��??? Jak �atwo podj�� dzia�ania "szarostrefowe"
	if(Dane.KonwertujDo(Poz+5,Prolegalnosc)!=-1) {Blad=Poz+5; return false;}
 //Czasochlonnosc = float    Czasochlonnosc;//Jaki jest wsp�czynnik czasoch�onno�ci typowej dzia�alno�ci. Wygenerowania pojedynczego wyniku bada�?
	if(Dane.KonwertujDo(Poz+6,Czasochlonnosc)!=-1) {Blad=Poz+6; return false;}
 //Innowacyjno�c = float    Innowacyjnosc;//0..1. �atwo�� startowania projekt�w bez inspiracji z zewn�trz, zaciekawienie projektami nieco odleg�ymi od dotychczasowych
	if(Dane.KonwertujDo(Poz+7,Innowacyjnosc)!=-1) {Blad=Poz+7; return false;}
 //Finans. swobodne = float    FinanseSwobodne;//Rezerwy finansowe - w skali roku LUB UPROSZCZONA "ZAMO�NO��"
	if(Dane.KonwertujDo(Poz+8,FinanseSwobodne)!=-1) {Blad=Poz+8; return false;}
 //Finanse zaplan = float    FinanseObiecane;//Zaplanowane do wydania, ale by� mo�e jeszcze nie na koncie (odnawiane/odbierane co 365 krok�w)
	if(Dane.KonwertujDo(Poz+9,FinanseObiecane)!=-1) {Blad=Poz+9; return false;}
 //D�ug do nadrz. = float 	  Dlugi;//Ile zesp� "wisi" swojemu wydzia�owi/instytutowi. Mo�liwe tylko chwilowo (30 krok�w?)
	if(Dane.KonwertujDo(Poz+10,Dlugi)!=-1) {Blad=Poz+10; return false;}
 //Udzial w zysku TT = float    UdzialWZysku;//0..1 Jaki procent zysku z innowacji pobiera (reszta idzie dla pracownik�w)
	if(Dane.KonwertujDo(Poz+11,UdzialWZysku)!=-1) {Blad=Poz+11; return false;}
 //Eksperckosc w dziedzinie = float    Doswiadczenie;//POZIOM EKSPERTYZY
	if(Dane.KonwertujDo(Poz+12,Doswiadczenie)!=-1) {Blad=Poz+12; return false;}  //Powinno byc 12!!!
   Blad=Poz+13;//OK
   return true;
}

void AdministracjaNaukowa::AktualizujListeDanych()
//Do zapisu i wy�wietlania inspekcyjnego
{
	unsigned Poz=8;           //PROWIZORKA!!! Na razie na "sile"
	//Finans. bie��ce  = u float    FinanseBiezace; //Ile firma ma kasy aktualnie LUB UPROSZCZONA "ZAMO�NO��"
	Dane.PrzypiszZ(Poz+8,FinanseSwobodne);
	//Finanse zaplan  = u float    FinanseZaplanowane;//Ile firma ma obiecanych �rodk�w kredytowych
	Dane.PrzypiszZ(Poz+9,FinanseObiecane );
	//D�ugi  = u float 	 Dlugi ;//Ile firma ma do sp�aty. Im wi�cej tym mniejsza szansa �e co� dostanie nowego
	Dane.PrzypiszZ(Poz+10,Dlugi );
}

bool AdministracjaNaukowa::ZrobWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad)
{
	unsigned Poz;
	if(!GenerycznyWezelSieci::ZrobWgListy(Lista,Ile,Poz)) {Blad=Poz; return false;} //Nie uda�o si�
	//Ile Etat�w = unsigned  IleEtatow;//Ile etat�w. Co najmniej 1, 0 - "duch" zespo�u - plan utworzenia
	if(Dane.KonwertujDo(Poz+0,IleEtatow)!=-1) {Blad=Poz+0; return false;}
	//Efektywnosc =  float     Efektywnosc;//0..2. Czy dzia�a na 50% wydajno�ci czy na 150%. Zale�y od motywacji
	if(Dane.KonwertujDo(Poz+1,Efektywnosc)!=-1) {Blad=Poz+1; return false;}
	//Aktywno�� =  float     AktywnoscKonf;//(Pro)aktywno��. Prawdopodobie�stwo organizowania konferencji i innych event�w umo�liwiaj�cych kontakty mi�dzy naukowcami i go��mi (czyli co ile krok�w nast�puje taki event)
	if(Dane.KonwertujDo(Poz+2,Proaktywnosc)!=-1) {Blad=Poz+2; return false;}
	//Centralno�� =  float     Centralno��;//0..1. Jak bardzo aktywno�� (zewn�trzna?) zale�y od szefa/szef�w
	if(Dane.KonwertujDo(Poz+3,Centralnosc)!=-1) {Blad=Poz+3; return false;}
	//Probiznesowosc =  float     Probiznesowosc;//0...1 Prawdopodobie�stwo podj�cia inicjatyw z wsp�udzia�em biznesu
	if(Dane.KonwertujDo(Poz+4,Probiznesowosc)!=-1) {Blad=Poz+4; return false;}
	//Prolegalnosc =  float     Prolegalnosc;//????Elastyczno��??? Jak �atwo podj�� dzia�ania obchodz�ce "bzdurne" przepisy
	if(Dane.KonwertujDo(Poz+5,Prolegalnosc)!=-1) {Blad=Poz+5; return false;}
	//Czasochlonnosc =  float     Czasochlonnosc;// Ile czasu (krok�w modelu) si� czeka z podj�ciem decyzji
	if(Dane.KonwertujDo(Poz+6,Czasochlonnosc)!=-1) {Blad=Poz+6; return false;}
	//ProTT =  float     ProInwest;//0-1 Sk�onno�� inwestowania wolnych �rodk�w w rozw�j zwi�zany z TT
	if(Dane.KonwertujDo(Poz+7,ProInwest)!=-1) {Blad=Poz+7; return false;}
	//Finans. swobodne =  float     FinanseSwobodne;//Rezerwy finansowe - w skali roku (odnawiane/odbierane co 365 krok�w) LUB UPROSZCZONA "ZAMO�NO��"
	if(Dane.KonwertujDo(Poz+8,FinanseSwobodne)!=-1) {Blad=Poz+8; return false;}
	//Finanse zaplan  = float     FinanseObiecane;//Zaplanowane do wydania, ale by� mo�e jeszcze nie na koncie
	if(Dane.KonwertujDo(Poz+9,FinanseObiecane)!=-1) {Blad=Poz+9; return false;}
	//D�ugi =  float 	   Dlugi;//Ile jednostka ju� "wisi". Np. na budow�, albo na pr�d jak dotacja zaleg�a. Nie mo�e wi�cej ni� urz�dowy limit
	if(Dane.KonwertujDo(Poz+10,Dlugi)!=-1) {Blad=Poz+10; return false;}
	//Udzial w zysku TT =  float     UdzialWZysku;//0..1 Jaki procent zysku z innowacji pobiera (reszta idzie "w d�" hierarchii)
	if(Dane.KonwertujDo(Poz+11,UdzialWZysku)!=-1) {Blad=Poz+11; return false;}
	//Do�wiadczenie =  float     Doswiadczenie;//POZIOM EKSPERTYZY? Jak tego u�ywa�? Mo�e to samo co efektywno��???
	if(Dane.KonwertujDo(Poz+12,Doswiadczenie)!=-1) {Blad=Poz+12; return false;}
	Blad=Poz+13;//OK
	return true;
}

void Firma::AktualizujListeDanych()
//Do zapisu i wy�wietlania inspekcyjnego
{
	unsigned Poz=8;           //PROWIZORKA!!! Na razie na "sile"
	//Finans. bie��ce  = u float    FinanseBiezace; //Ile firma ma kasy aktualnie LUB UPROSZCZONA "ZAMO�NO��"
	Dane.PrzypiszZ(Poz+8,FinanseBiezace );
	//Finanse zaplan  = u float    FinanseZaplanowane;//Ile firma ma obiecanych �rodk�w kredytowych
	Dane.PrzypiszZ(Poz+9,FinanseZaplanowane );
	//D�ugi  = u float 	 Dlugi ;//Ile firma ma do sp�aty. Im wi�cej tym mniejsza szansa �e co� dostanie nowego
	Dane.PrzypiszZ(Poz+10,Dlugi );
}

bool Firma::ZrobWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad)
{
	unsigned Poz;
	if(!GenerycznyWezelSieci::ZrobWgListy(Lista,Ile,Poz))  {Blad=Poz; return false;} //Nie uda�o si�
	//Ile Etat�w  = unsigned IleEtatow;//Ile etat�w. Co najmniej 1, 0 - "duch" zespo�u - plan utworzenia
	if(Dane.KonwertujDo(Poz+0,IleEtatow)!=-1) {Blad=Poz+0; return false;}
	//Efektywnosc  = u float    Efektywnosc;//0..2. Czy dzia�a na 50% wydajno�ci czy na 150%. Zale�y od motywacji
	if(Dane.KonwertujDo(Poz+1,Efektywnosc )!=-1) {Blad=Poz+1; return false;}
	//Aktywno��  = u float    Proaktywnosc; //Aktywno��. 0-1 aktywno�� w tworzenia nowych znajomo�ci biznesowych
	if(Dane.KonwertujDo(Poz+2,Proaktywnosc )!=-1) {Blad=Poz+2; return false;}
	//Centralno��  = u float    Centralno��;//0..1. Jak bardzo aktywno�� (zewn�trzna?) zale�y od szefa/szef�w
	if(Dane.KonwertujDo(Poz+3,Centralnosc )!=-1) {Blad=Poz+3; return false;}
	//Pronaukowosc  = u float    ProAkademickosc; //0-1 ch�� nawi�zywania kontakt�w z badaczami i uniwersytetami
	if(Dane.KonwertujDo(Poz+4,ProAkademickosc )!=-1) {Blad=Poz+4; return false;}
	//Prolegalnosc  = u float    Prolegalnosc;//?"Pragmatyczno��"? Jak �atwo podj�� dzia�ania "szarostrefowe" i omijac "nie�yciowe" przepisy
	if(Dane.KonwertujDo(Poz+5,Prolegalnosc )!=-1) {Blad=Poz+5; return false;}
	//Czasochlonnosc  = u float    Czasochlonnosc;//Jaki jest wsp�czynnik czasoch�onno�ci typowej dzia�alno�ci (np. przygotowanie projektu oraz produktu)
	if(Dane.KonwertujDo(Poz+6,Czasochlonnosc )!=-1) {Blad=Poz+6; return false;}
	//Proinnowacyjnosc  = u float    ProInnowac;//Stosunek do nowo�ci: 0-1 - 0..33% niewiele robi, 33-66% raczej blokuje innych, powy�ej - pr�buje, wchodz�c na coraz wcze�niejszym etapie (prawdopodobienstwo)
	if(Dane.KonwertujDo(Poz+7,ProInnowac )!=-1) {Blad=Poz+7; return false;}
	//Finans. bie��ce  = u float    FinanseBiezace; //Ile firma ma kasy aktualnie LUB UPROSZCZONA "ZAMO�NO��"
	if(Dane.KonwertujDo(Poz+8,FinanseBiezace )!=-1) {Blad=Poz+8; return false;}
	//Finanse zaplan  = u float    FinanseZaplanowane;//Ile firma ma obiecanych �rodk�w kredytowych
	if(Dane.KonwertujDo(Poz+9,FinanseZaplanowane )!=-1) {Blad=Poz+9; return false;}
	//D�ugi  = u float 	 Dlugi ;//Ile firma ma do sp�aty. Im wi�cej tym mniejsza szansa �e co� dostanie nowego
	if(Dane.KonwertujDo(Poz+10,Dlugi )!=-1) {Blad=Poz+10; return false;}
	//Limit TT  = u float 	  LimitUdzialuInwestTT; //0-1 Jakie �rodki finansowe z zarobionych got�w przeznaczy� na inwestycje TT
	if(Dane.KonwertujDo(Poz+11,LimitUdzialuInwestTT )!=-1) {Blad=Poz+11; return false;}
	//Znajomo�� bran�y  = u float    Doswiadczenie;//POZIOM EKSPERTYZY? Jak tego u�ywa�? Mo�e to samo co efektywno��???
	if(Dane.KonwertujDo(Poz+12,Doswiadczenie )!=-1) {Blad=Poz+12; return false;}
	Blad=Poz+13;//OK
	return true;
}

bool BOTT::ZrobWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad)
{
	return Firma::ZrobWgListy(Lista,Ile,Blad);
}
bool UOTT::ZrobWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad)
{
	return AdministracjaNaukowa::ZrobWgListy(Lista,Ile,Blad);
}
bool PodlegloscOrganizacyjna::ZrobWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad)
{
	return GenerycznePowiazanie::ZrobWgListy(Lista,Ile,Blad);
}
bool FormalnaKooperacja::ZrobWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad)
{
	return PowiazaniePaboliczne::ZrobWgListy(Lista,Ile,Blad);
}
bool KontaktTowarzyski::ZrobWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad)
{
	return PowiazaniePaboliczneSkierowane::ZrobWgListy(Lista,Ile,Blad);
}
bool KomunikacjaTowarzyska::ZrobWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad)
{
	return GeneryczneInfo::ZrobWgListy(Lista,Ile,Blad);
}
bool KomunikacjaOficjalna::ZrobWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad)
{
	return GeneryczneInfo::ZrobWgListy(Lista,Ile,Blad);
}
bool KomunikacjaFinansowa::ZrobWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad)
{
	return GeneryczneInfo::ZrobWgListy(Lista,Ile,Blad);
}
bool PaczkaProduktow::ZrobWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad)
{
	return GeneryczneInfo::ZrobWgListy(Lista,Ile,Blad);
}
bool RynekZbytu::ZrobWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad)
{
	return WezelMacierzowy::ZrobWgListy(Lista,Ile,Blad);
}

// DODATKOWE IMPLEMENTACJE WIZUALIZACJI!!!
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//Chwilowo tutaj r�ne wizualizacje komunikat�w zrobione proceduralnie.
//Na razie SPS tylko to umo�liwia, ale docelowo powinny by� obiekty wizualizacyjne
//u�ywaj�ce po prostu obrazk�w lub animacji - jak ��wie w Logomocji czy StarLogo

#include "SYMSHELL/symshell.h"

double _ZespolRoboczy::R(double KAT_NIEISTOTNY)
//Promie� otaczaj�cego zesp�l roboczy okr�gu lub elipsy musi by� bardziej zmy�lnie
//zalezny od wagi, poniewa� te wagi bywaj� bardzo du�e w trakcie symulacji
{
	double JakWazny=this->Waznosc();
	if(JakWazny<=1)
	{
	   JakWazny=1+((JakieRwProcWidth/100)*ver*JakWazny);
	   return JakWazny;
	}
	else
	{
	   double LogWazny=log10(JakWazny);
	   JakWazny=1+((JakieRwProcWidth/100)*ver*(1+LogWazny));
	   return JakWazny;
	}
}

void FormalnaKooperacja::Narysuj()
{
	PowiazaniePaboliczne::Narysuj();//Przechwycenie rysowania dla debugingu
}

void KomunikacjaTowarzyska::RysujKsztalt(float X,float Y,float Rad,unsigned R,unsigned G,unsigned B)
{
	set_brush_rgb(R,G,B);//Podstawa
	set_pen_rgb(R/2,G/2,B/2,0,SSH_LINE_SOLID); //Ciemne  //Free_style_pen
	//set_pen_rgb(R*2%256,G*2%256,B*2%256,0,SSH_LINE_SOLID); //Kontur
	//fill_rect_d(X-Rad,Y-Rad,X+Rad,Y+Rad);
	//set_brush_rgb(R,G,B);
	fill_circle_d(X,Y,Rad);
	set_pen_rgb(min(R*2,255u),min(G*2,255u),min(B*2,255u),0,SSH_LINE_SOLID); //JASNE - Free_style_pen
	int Left=X-Rad+1;
	int Right=X+Rad-1;
	if(this->Rodzaj().find('?'))
	{
	   line_d(Left,Y-Rad/2,Right,Y+Rad/2);
	   line_d(Left,Y+Rad/2,Right,Y-Rad/2);
	}
	else
	{
	for(unsigned i=0;i<unsigned(Rad);i++)
		if(i%2==0)
			//line_d(X-Rad+1,Y-Rad+i,X+Rad-1,Y-Rad+i);
			line_d(Left,Y-Rad/2+i,Right,Y-Rad/2+i);
	}
}

void KomunikacjaOficjalna::RysujKsztalt(float X,float Y,float Rad,unsigned R,unsigned G,unsigned B)
{
	if(this->Rodzaj()=="RAPORT")
		set_brush_rgb(255,255,255);//Bia�a kartka
		else
        set_brush_rgb(R,G,B);//Kartka w kolorze tematycznym
	fill_rect_d(X-Rad,Y-Rad,X+Rad,Y+Rad);
	set_pen_rgb(R/2,G/2,B/2,0,SSH_LINE_SOLID); //Ciemne  //Free_style_pen
	for(unsigned i=0;i<unsigned(2*Rad);i++)
		if(i%2==0)
			line_d(X-Rad+1,Y-Rad+i,X+Rad-1,Y-Rad+i);
	//set_brush_rgb(R,G,B);
	//fill_circle_d(X,Y,Rad);
}

void KomunikacjaFinansowa::RysujKsztalt(float X,float Y,float Rad,unsigned R,unsigned G,unsigned B)
{
	float L;
	if(Kwota>1)
		L=log10(double(this->Kwota));  //Uzaleznienie wielko�ci od kwoty
		else
		L=0.5;//Czasem tu si� co� dzieje cho� nie powinno
	if(L<2) Rad/=2; //Drobne kwoty
		else
		Rad*=L-1;//Typowa kwota to 100!
	set_brush_rgb(255,255,255);//Bia�a kartka
	fill_rect_d(X-Rad,Y-Rad,X+Rad,Y+Rad);
	set_pen_rgb(R/2,G/2,B/2,0,SSH_LINE_SOLID); //Ciemne  //Free_style_pen
	for(unsigned i=Rad;i<unsigned(2*Rad);i++) //Rzut �eby nie "parska�"
		if(i%2==0)
			line_d(X-Rad+1,Y-Rad+i,X+Rad-1,Y-Rad+i);
	set_pen_rgb(R/2,G/2,B/2,0,SSH_LINE_SOLID);  //Free_style_pen
	set_brush_rgb(255,255,0); //Z�oto
	fill_circle_d(X,Y,Rad/2);
}

void PaczkaProduktow::RysujKsztalt(float X,float Y,float Rad,unsigned R,unsigned G,unsigned B)
{                            						assert(this->Ile>=1);
    float L;
	if(Ile>1)
		L=log10(double(this->Ile));  //Uzaleznienie wielko�ci od kwoty
		else
		L=0.5;//Czasem tu si� co� dzieje cho� nie powinno
	if(L<1) Rad/=2;   //Typowy rozmiar to 10
		else
		Rad*=L;
	set_brush_rgb(R,G,B);
	//Paczka
	fill_rect_d(X-Rad,Y-Rad,X+Rad,Y+Rad);
	//fill_circle_d(X,Y,Rad);
	set_pen_rgb(R/3,G/3,B/3,0,SSH_LINE_SOLID);  //Free_style_pen
	//Sznurek
	line_d(X-Rad,Y,X+Rad,Y);
	line_d(X,Y-Rad,X,Y+Rad);
}


void ProcesTransferuTech::Narysuj()
//Ma uzupelnione rysowanie
{
	double SkalaX=Swiat::SkalujX();
	double SkalaY=Swiat::SkalujY();

	GenerycznyProces::Narysuj();
	WezelSieci* Mw=Swiat::Wezel(Procesor());
	if(Mw==NULL)
		cerr<<endl<<"Proces na nieistniej�cym wezle"<<endl;
	unsigned ile=Swiat::IleProcesow(Procesor());

	double R=Mw->R(1.5*M_PI);
	double Y=Mw->Y()-0.1*R+this->MojID();//Indeks procesu na w�le
	double X1=Mw->X()-R;
	double X2=X1+2*R;

	for(unsigned i=0;i<sizeof(Fragmenty)/sizeof(Fragmenty[0]);i++)
	{
		if(Fragmenty[i].Check)
		{        //Fragmenty[i].Check=true;//oznacza �e bit niepotrzebny!
				 //Fragmenty[i].Wykonawca=-1;//Zaden wykonawca a odhaczone
			if(Fragmenty[i].Wykonawca!=-1)
			   set_pen_rgb(255,255,255,1,SSH_LINE_SOLID);//Gotowe - bia�y!
			else
			   set_pen_rgb(0,255,255,1,SSH_LINE_SOLID);  //Niepotrzebne - Cyjan
		}
		else
			set_pen_rgb(0,0,0,1,SSH_LINE_SOLID);  //Czarny
		line_d(SkalaX*X1-i-1,SkalaY*Y-1,SkalaX*X1-i-1,SkalaY*Y+1); //Linia bitowa na check
	}

	if( Fragmenty[PRODUCENT].Check )    //Gdzie zaznaczamy kto wyprodukuje
	{
		set_pen_rgb(255,255,255,1,SSH_LINE_SOLID);
		line_d(SkalaX*X2+2,SkalaY*Y-1,SkalaX*X2+4,SkalaY*Y+2); //Linia bitowa na check
	}
	if( Fragmenty[FINANSOWANIE].Check ) //Gdzie zaznaczamy kto to finansuje
	{
		set_pen_rgb(255,0,255,1,SSH_LINE_SOLID);
		line_d(SkalaX*X2+0,SkalaY*Y-1,SkalaX*X2+2,SkalaY*Y+2); //Linia bitowa na check
	}
}

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

/*
#include "spsModel.h"
#include "spsGenNode.h"
#include "spsGenProc.h"
#include "spsGenLink.h"
#include "spsParaLink.h"
#include "spsGenInfo.h"
#include "spsMatrixNode.h"
#include "INCLUDE/platform.hpp"
#include "INCLUDE/wb_bits.h"
#include "INCLUDE/wb_ptr.hpp"
using namespace wbrtm;

#include <stdlib.h>
#include <cassert>
#include <string>
#include <iostream>
using namespace std;
*/

/*  STARY KOD Z "JEDNOSTKI NAUKOWEJ"
		else  //Obsluga konca procesu sprawozdawczego
		{
			DziedzinaWKolorze D=Roboczy->PodajDziedzine();
			std::string Nazwa=Roboczy->Dane[1];

			//Konczymy i ewentualnie tworzymy jaki� nowy proces gdy obici��eniowo-sprawozdawczy
			c----<<endl<<this->Nazwa()<<' '<<_LPL("zakonczyl proces","fin.proc.")<<' '<<Nazwa<<' '<<Roboczy->DoKonca()<<' ';

			if(Nazwa=="OBCI��ENIE")
			{
				double Zaangazowanie=Roboczy->JakPracochlonny();//Jak pracoch�onne to dzia�anie
				//Delikatne obni�anie obci��enia jak si� nie wyrabiaj�
				if(Roboczy->DoKonca()<-7)
				{
					Zaangazowanie*=0.99;
					c----<<" z mocna OBSUWA!!!"<<endl;
				}
				else c----<<endl;
				//Usuwamy bo si� sko�czyl
				Roboczy->Wymazuj();
				Roboczy->UznajZaZakonczony();

				//Tworzymy nowy
				Roboczy=new GenerycznyProces(Nazwa.c_str(),Zaangazowanie,365);  //356 dni oczywiscie
				Roboczy->UstawDziedzine(D); //Dziedzina poprzedniego procesu
				Swiat::WstawProc(Roboczy,MojID());
				c----<<this->Nazwa()<<' '<<_LPL("ustalil nowy proces","set new proc.")<<' '<<Nazwa<<' ';
				c----<<"Zaang.:"<<Zaangazowanie<<"=="<<(Zaangazowanie/(IleEtatow*365))*100<<"% wydolnosci"<<endl;
			}
			else
			{
				//Usuwamy proces inny ni� OBCIAZENIE bo si� skonczyl
				Roboczy->Wymazuj();
				Roboczy->UznajZaZakonczony();
				//Swiat::UsunProc(ktory,MojID());
			}

			//Generuje raport dla administracji
			KomunikacjaOficjalna* KO=new KomunikacjaOficjalna();
			KO->UstawDziedzine(D);
			//Dosy� skomplikowane szukanie adresata. Trzeba by mo�e zrobi� lepsz� obslug� link�w w�asnych w�z�a
			unsigned Kanal=-1;
			for(unsigned i=0;i<Swiat::IlePowiazan();i++)
			{
				PodlegloscOrganizacyjna* P=dynamic_cast<PodlegloscOrganizacyjna*>(Swiat::Lacze(i));
				if(P!=NULL && P->Poczatek()==MojID())
					{Kanal=i;break;}
			}
			if(Kanal!=Swiat::INVINDEX
				&& KO->Zaadresuj(Kanal,true,0.05)
				&& (Swiat::WstawInfo(KO)!=Swiat::INVINDEX) )
					;//OK
		}
*/


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
