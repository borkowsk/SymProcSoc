// Transfer technologii 2011 - w�a�ciwa implementacja modelu     24-10-2011
////////////////////////////////////////////////////////////////////////////////
// Implementacja akcji klas w�a�ciwego modelu
// Rynek, firma, kooperacja, produkty i pieni�dze
//(bez technikali�w obslugi interfaceu )
////////////////////////////////////////////////////////////////////////////////

#include "opiKlasyWersja.hpp"
#include "INCLUDE/wb_smartlog.h"
using namespace wbrtm;

char* const  KONTO="Ruch konta"; //Marker komunikatu finansowego z przep�ywem
float  JEDNOSTKOWA_WYPLATA=100;  //Warto�� najmniejszej wp�aty/wyplaty. Spos�b u�ycia niejasny :-)
static const int LOCLOG=2; //Na jakim poziomie logowania domy�lnie zapisywa� problemy z tego pliku

//Klasa wewn�trzna skupiaj�ca wsp�lne w�a�ciwo�ci w�z��w z pracownikami (zespo��w)
//////////////////////////////////////////////////////////////////////////////////
/*
class _ZespolRoboczy:public GenerycznyWezelSieci
{
 protected:
 friend class  ProcesProdukcyjny;
 friend class  ProcesRaportowany;
 friend class  ProcesSpoleczny;
//Ile Etat�w	Efektywnosc	Aktywno��	Centralno��	Probiznesowosc	Prolegalnosc	Czasochlonnosc	Innowacyjno�c	Finans. swobodne	Finanse zaplan	D�ug do nadrz.	Udzial w zysku TT	Ekspercko�� w dziedzinie	Uwagi
 unsigned IleEtatow;//Ile etat�w. Co najmniej 1, 0 - "duch" zespo�u - plan utworzenia
 float    Efektywnosc;//0..2. Czy dzia�a na 50% wydajno�ci czy na 150%. Zale�y od motywacji
 float    Proaktywnosc;//Aktywno��? 0..1. Podtrzymywanie aktywno�ci zewn�trznej, tworzenie link�w
 float    Centralno��;//0..1. Jak bardzo aktywno�� (zewn�trzna?) zale�y od szefa/szef�w
 float    Doswiadczenie;//POZIOM EKSPERTYZY? Jak tego u�ywa�? Mo�e to samo co efektywno��???
 float    Czasochlonnosc;//Jaki jest wsp�czynnik czasoch�onno�ci typowej dzia�alno�ci. Wygenerowania pojedynczego wyniku bada�?
 float    Prolegalnosc;//????Uczciwo��??? Jak �atwo podj�� dzia�ania "szarostrefowe"
};
*/

PaczkaProduktow::PaczkaProduktow(const char* Nazwa,unsigned IleWPaczce,float CenaZaSztuke,unsigned KtoProducentem)
{
	Dane.Realokuj(12); Dane[9]=Nazwa;
	Cena=CenaZaSztuke;
	Ile=IleWPaczce;
	this->Producent=KtoProducentem;
}

PaczkaProduktow::PaczkaProduktow(const PaczkaProduktow* Wzor) //KONSTRUKTOR KOPIUJ�CY
			:GeneryczneInfo(Wzor)
{
	Cena=Wzor->Cena;
	Ile=Wzor->Ile;
	Producent=Wzor->Producent;
}

const string& PaczkaProduktow::Rodzaj()
//Tu mo�e by� mark� czy nazw� handlow� - bo mo�e by� przesylany dalej!
{
	return Dane[9];
}

bool  PaczkaProduktow::UstawRodzaj(const char* Nowy)
//A jak typ nie udost�pnia zmiany to zwraca false
{
	Dane[9]=Nowy;
	return true;
}

KomunikacjaFinansowa::KomunikacjaFinansowa(const char* Kategoria,float JakaKwota)
//Konstruktor domyslny dla symulacyjnego tworzenia komunikatu finansowego
{
	Dane.Realokuj(12); Dane[9]=Kategoria; Kwota=JakaKwota;
}

const string& KomunikacjaFinansowa::Rodzaj()//Gdzie� w danych jest rodzaj tego komunikatu
{
	return Dane[9];
}

bool  KomunikacjaFinansowa::UstawRodzaj(const char* Nowy)
//A jak typ nie udost�pnia zmiany to zwraca false
{
	Dane[9]=Nowy;
	return true;
}


ProcesProdukcyjny:://ProcesProdukcyjny(unsigned IleSztuk,float JakaCena,unsigned IleWPaczce) // domyslnie JakaCena=1 , IleWPaczce=-1
ProcesProdukcyjny(const char* Nazwa, unsigned IleSztuk,float JakaCena,unsigned IleWPaczce):
			GenerycznyProces(Nazwa,365*10+DRAND()*10,365)
//Konstruktor z parametrami do tworzenia procesu w kodzie
{
	Zapotrzebowanie=IleSztuk; Cena=JakaCena; Wyslano=0;
	if(IleWPaczce!=-1)  WPaczke=IleWPaczce;
		   else 		WPaczke=IleSztuk/10; //Dzieli na 10 pak jak -1
}

ProcesProdukcyjny::ProcesProdukcyjny(const ProcesProdukcyjny* Wzor):
				GenerycznyProces(Wzor)
{
	Zapotrzebowanie=Wzor->Zapotrzebowanie;
	WPaczke=Wzor->WPaczke;
	Cena=Wzor->Cena;
	Wyslano=0;
}

ProcesProdukcyjny::ProcesProdukcyjny()
{
	Zapotrzebowanie=100;
	WPaczke=10;
	Cena=10;
	Wyslano=0;
}
	   //	unsigned Blad=0; //Dziedzina ju� jest taka jak macierzystego procesu
	   //	bool Result=Nowy->ZrobWgListy(&Dane[0],Dane.Ile(),Blad); //chodzi tez o nadanie warto�ci jak w macierzystym procesie
	   //																			assert(Result && Blad==8);//By�o poprawne kiedy�, musi by� i teraz


// WLASCIWA IMPLEMENTACJA TEGO CO ROBI� KLASY
////////////////////////////////////////////////////////////////////////////////

void ProcesProdukcyjny::ChwilaDlaCiebie()
{
	if(!this->Poprawny() || this->JakZaawansowany()>=PROCES_NAD_ZAAWANSOWANY)
					return; //Bo mo�e ju� zako�czony!!! A to by mno�y� procesy
						//Sheduler nie powinien do tego dopu�ci�, ale...

	GenerycznyProces::ChwilaDlaCiebie(); //Popchni�te liczniki

	double Zaaw=JakZaawansowany(); //Musi wiedzie� jak jest teraz zaawansowany

	if(0.985<Zaaw) //Finalizowanie procesu - wysy�anie produktu na rynek
		_WyslijProduktyNaRynek();

	if(Zaaw>=1) //Jest ju� zakonczony - samowzbudza reprodukcj�
	{
		WezelSieci* MojProcesor=Swiat::Wezel(this->Procesor());

		if(DoKonca()<-7)//Sp�ni� si� wi�cej ni� tydzie�  - KOMUNIKAT
		{
		   IFTL(LOCLOG+1)	clog<<endl<<"\""<<MojProcesor->Nazwa()<<"\" "<<_LPL("z poslizgiem zakonczyl proces","fin.proc.")<<' '<<Dane[1]<<':'<<hex<<PodajDziedzine().ARGB<<" ("<<-DoKonca()<<_LPL(" krokow po terminie"," steps after deadline")<<")"<<endl;
		   TLOG(LOCLOG, <<"\""<<MojProcesor->Nazwa()<<"\" "<<_LPL("z poslizgiem zakonczyl proces","fin.proc.")<<'\t'<<Dane[1]<<":\t"<<hex<<PodajDziedzine().ARGB<<"\t ("<<-DoKonca()<<_LPL(" krokow po terminie"," steps after deadline")<<")" )
		}

		//Tworzymy nowy proces produkcji b�d�cy niemal kopi�, tylko mo�e troch� mniej czasoch�onny i kosztowny
		//==========================================================================================================
		wb_ptr<ProcesProdukcyjny> Nowy( new ProcesProdukcyjny(this) );	//(IleSztuk,JakaCena,IleWPaczce);//Nowa wersja, ale parametry si� i tak zama�� zaraz

		//Ale niekt�re trzeba zmieni�, �eby by� post�p
		double Zaangazowanie=JakPracochlonny()*0.999;//IleEtatow/ile_proc? //  Czasochlonnosc ukryta w pierwotnym procesie
		double IleCzasu=365;//Domy�lny czas przygotowania produktu - raczej za du�y
		Nowy->WPaczke=WPaczke; //Tego akurat nie, ale warto�� domy�lna nie musi by� poprawna
		Nowy->Cena=_ZespolRoboczy::_KalkulujKosztProduktu(this->PodajDziedzine())*1.20; //Nowa kalkulacja ceny z 20% zyskiem
		if(Nowy->Cena>=Cena)//Starszy produkt mo�na i trzeba by sprzedawa� taniej
			Nowy->Cena*=0.99;
		Nowy->Zapotrzebowanie=Zapotrzebowanie*1.1; //Jaka� szansa na wzrost, bo wracaj�ce paczki zawsze obni�aj� zapotrzebowanie
		if(Nowy->Zapotrzebowanie<WPaczke) //Ju� si� bardzo s�abo sprzedaje
		{
			if(DRAND()<0.5)
				Nowy->Zapotrzebowanie=WPaczke; //Produkt kolekcjonerski?
				else
				{//A mo�e ca�kiem zrezygnowa�? Mo�na je�li s� jakie� inne procesy produkcji na tym w�le (Firmie)
					IFTL(LOCLOG) clog<<endl<<"\""<<MojProcesor->Nazwa()<<"\" "<<_LPL("PORZUCIL proces","abandon the proc.")<<' '<<Nowy->Dane[1]<<':'<<hex<<Nowy->PodajDziedzine().ARGB<<dec<<endl;
					TLOG(LOCLOG, <<"\""<<MojProcesor->Nazwa()<<"\" "<<_LPL("PORZUCIL proces","abandon the proc.")<<'\t'<<Nowy->Dane[1]<<":\t"<<hex<<Nowy->PodajDziedzine().ARGB<<dec )
					Wymazuj();  //Informacja na wy�wietlaniu
					UznajZaZakonczony();//Jawnie nie usuwamy bo to grozi katastrof� gdy� wywo�uje "delete"
				}
		}

		//Trzeba sprawdzi� by wydolno�� firmy i z tego policzy� czas
		_ZespolRoboczy* ProcesorZR=dynamic_cast<_ZespolRoboczy*>(MojProcesor);
		if(ProcesorZR) //Uda�o si� sprowadzi� do tego "mianownika"
		{
			//Mo�na lepiej zaplanowa� czas wykonania nowej wersji procesu //=(Zaangazowanie/IleEtatow)*ile_proc*1.1;//10% rezerwy
			unsigned ile_proc=Swiat::IleProcesow(Procesor()); //S� tam te� puste!
			IleCzasu=(Zaangazowanie/ProcesorZR->IleEtatow)*ile_proc*1.1;//10% rezerwy
			ProcesorZR->Doswiadczenie*=1.01;//Wzrost do�wiadczenia
			if(ProcesorZR->Doswiadczenie>1) ProcesorZR->Doswiadczenie=1; //Ale w tym modelu nie wi�cej ni� do 1

			Nowy->WymaganaIloscPracy=Zaangazowanie;
			Nowy->_IleKrokowDoKonca=IleCzasu;
			Nowy->TmpPorcjaPracy=1;
			Nowy->PracaDotychczasowa=0;

			IFTL(LOCLOG+1) clog<<endl<<"\""<<MojProcesor->Nazwa()<<"\" "<<_LPL("ustalil nowy proces","set new proc.")<<' '<<Nowy->Dane[1]<<':'<<hex<<Nowy->PodajDziedzine().ARGB<<" Zapotrz.:"<<dec<<Nowy->Zapotrzebowanie<<" Zaang.:"<<Zaangazowanie<<endl;
			TLOG(LOCLOG, <<"\""<<MojProcesor->Nazwa()<<"\" "<<_LPL("ustalil nowy proces","set new proc.")<<'\t'<<Nowy->Dane[1]<<":\t"<<hex<<Nowy->PodajDziedzine().ARGB<<"\tZapotrz.:\t"<<dec<<Nowy->Zapotrzebowanie<<"\tZaang.:\t"<<Zaangazowanie )

			if(Swiat::WstawProc(Nowy.give(),Procesor())!=Swiat::INVINDEX) //I procesor te�...
							;//Uda�o sie
				else
				TLOG(LOCLOG, <<"Nie udane odpalenie procesu na "<<"\""<<MojProcesor->Nazwa()<<"\" " )
		}
		else
		{
			//Jaka� wtopa - PROCES PRODUKCJI na NIEW�A�CIWYM TYPE W�Z�A  - nie ponawiamy!!!
			cerr<<endl<<"PROCES PRODUKCJI na NIEWLASCIWYM TYPE WEZ�A "<<"\""<<MojProcesor->Nazwa()<<"\" TYP: "<<MojProcesor->WartoscPola(0)<<endl;
			TLOG(0, <<"PROCES PRODUKCJI na NIEW�A�CIWYM TYPE W�Z�A "<<"\t\""<<MojProcesor->Nazwa()<<"\"\tTYP:\t"<<MojProcesor->WartoscPola(0)  )
		}

		//Teraz uznajemy aktualny za gotowy do usuniecia
		Wymazuj();  //Informacja na wy�wietlaniu
		UznajZaZakonczony();//Jawnie nie usuwamy bo to grozi katastrof� gdy� wywo�uje "delete"

	}
}

bool ProcesProdukcyjny::InterpretujKomunikat(Komunikat * Co)
//Gdy paczki wracaj� mo�e je odes�ac ze zmniejszon� cen�
//a jak to nie pomo�e to zmniejszy� "Zapotrzebowanie" na przysz�o��
{
	if(Co->PodajDziedzine()==PodajDziedzine()) //Interesuj� go komunikatyz jego dziedziny
	{
	  PaczkaProduktow* Zwrot=dynamic_cast<PaczkaProduktow*>(Co);
	  if(Zwrot!=NULL) //I na razie jedynie wtedy gdy to zwrot jego produktu
	  {
		float WJakiejBylCenie=Zwrot->CenaZaSzt();
		if(WJakiejBylCenie<Cena/3) //Je�li nie uda�o si� sprzeda� za 1/3 aktualnej ceny to rezygnujemy
		{
			Zapotrzebowanie-=Zwrot->IleSztuk();  //Zmniejszamy troch� zapotrzebowanie (o 1 czy o wielko�� paczki?)
			if(Zapotrzebowanie<=0)
								Zapotrzebowanie=1; //Bo nie wiem co si� stanie w procesie jak to spadnie do 0!!!
			return true; //Ale komunikat obsluzony
		}
		//Mo�na jeszcze spr�bowa� sprzeda� taniej w mniejszych partiach
		Zwrot->Cena*=0.75;//Za 3/4 poprzedniej ceny
		Zwrot->Ile/=2;//I dwie mniejsze paczki
		if(Zwrot->Ile<1)
			 Zwrot->Ile=1; //Nie da�o si� podzieli� zawarto�ci
		Komunikat* PaP=Zwrot->Klonuj(); //Dziedzina z klonowania  paczki
		PaP->Zwrotnie(0.012);//Powinno zadzia�a�
		if(Swiat::WstawInfo(PaP)!=Swiat::INVINDEX && Zwrot->Ile>1) //Jak zosta�a druga porcja
		{
		  PaP=Zwrot->Klonuj(); //Dziedzina z klonowania  paczki
		  PaP->Zwrotnie(0.015);
		  Swiat::WstawInfo(PaP);//Ale juz nie sprawdzamy (dla skr�cenia kodu)
		}
		return true;   // komunikat obsluzony
	  }
	}

	return false;
}

bool ProcesProdukcyjny::_WyslijProduktyNaRynek()
//Pomocnicza procedura wysy�ania na rynek przy ko�cu procesu produkcji
{
	if(Wyslano>=Zapotrzebowanie)
						return true; //Ju� wszystko wys�ano!

	unsigned NID=Procesor();//Nadawc� jest w�ze� "procesor" tego procesu
	WezelSieci* We=Swiat::Wezel(NID);

	unsigned ile_wyslac=this->WPaczke;
	double Zaaw=JakZaawansowany(); //Musi wiedzie� jak jest teraz zaawansowany
	if(Zaaw>=1)//Chyba ostatnia szansa - ca�a reszta
			ile_wyslac=Zapotrzebowanie-Wyslano;
	Wyslano+=ile_wyslac;

	//Produkcja kosztuje. Odliczamy to w tym mniejscu
	_ZespolRoboczy* ProcesorZR=dynamic_cast<_ZespolRoboczy*>(We);
	if( ProcesorZR!=NULL ) //O ile w�zel bierze pod uwag� koszty
	{
		float koszt=_ZespolRoboczy::_KalkulujKosztProduktu(this->PodajDziedzine())*ile_wyslac;
		if(!ProcesorZR->StacGoNaWydatek(koszt))
						return false;//Przerywamy
		ProcesorZR->PrzyjmijKase( - koszt);//Tu to proces przyjmuje kas� a nie w�ze�
	}

	//Pakujemy paczke
	wb_ptr<PaczkaProduktow> PaP( new PaczkaProduktow(We->Nazwa(),ile_wyslac,Cena,NID) );
	PaP->UstawDziedzine(this->PodajDziedzine()); //Zawartosc z dziedziny konkretnego procesu produkcji

	//Znajdujemy wla�ciwego odbiorce
	std::string NazwaOdbiorcy;
	if(IlePolDanych()<9 || strlen(WartoscPola(8))==0)
		NazwaOdbiorcy="Market";
		else
		NazwaOdbiorcy=WartoscPola(8);

	unsigned Odbiorca=Swiat::ZnajdzIndeksWezla(NazwaOdbiorcy.c_str());
	if(Odbiorca==Swiat::INVINDEX)
	{
		//Nie ma ju� dost�pnego odbiorcy!!!   PaP usunie paczk� na koncu metody
		IFTL(LOCLOG) clog<<endl<<"Proces \""<<Nazwa()<<"\" utracil odbiorce \""<<NazwaOdbiorcy.c_str()<<"\""<<endl;
		TLOG(1,      <<"Proces \""<<Nazwa()<<"\" utraci� odbiorc� \""<<NazwaOdbiorcy.c_str()<<"\""  )
		if(NazwaOdbiorcy=="Market")//Jak odbiorc� by� og�lny rynek
			 UznajZaZakonczony(); //To proces ju� niepotrzebny wi�c do USUNI�CIA
			 else //W przeciwnym razie mo�na jeszcze sprobowa� sprzedawa� na rynek
			 Dane[8]="Market";
		return false; //Nie uda�o si�...
	}

	if(PaP->Zaadresuj(NID,Odbiorca,0.02) //Jak si� uda poprawnie zaadresowa�
	   &&  (Swiat::WstawInfo(PaP.give())!=Swiat::INVINDEX) )    //To wysy�amy...
					;//OK
			else //Mo�e si� np. zdarzy� gdy system uleg� zawa�owi komunikat�w
			{
			Zapotrzebowanie-=ile_wyslac;//Wi�c trzeba si� stabilizowa�
			if(Zapotrzebowanie<5) Zapotrzebowanie=5;
			TLOG( 1, <<"Nie uda�o si� wys�a� paczki do \""<<NazwaOdbiorcy.c_str()<<"\". Mo�e za du�o komunikatow?" )
			}

	return true; //Chyba uda�o si�
}

void Firma::InterpretujKomunikat(Komunikat* Co)
//Przyjmowanie komunikat�w
{                                                                         assert(Co->Kanal()!=Swiat::INVINDEX);
	KomunikacjaFinansowa* KFI=dynamic_cast<KomunikacjaFinansowa*>(Co);
	if(KFI!=NULL)
	{
		if(KFI->Rodzaj()==KONTO)
		{
		   float wplyw=KFI->JakaKwota(); //Tu mo�e zwi�kszy� sw�j fundusz
		   _RozeslijNaleznosci(KFI->PodajDziedzine(),wplyw); //Ale mo�e jest winien oplaty licencyjne
		   this->FinanseBiezace+=wplyw; //To co zosta�o mo�na wsadzi� an w�asne konto
		   if(Dlugi>0)//Jak trzeba jako� wyr�wna� d�ug
		   {
				double kwota=0.01*Dlugi; //Nie wi�cej ni� 1% na raz �eby nie utraci� p�ynno�ci finansowej
				if(kwota<FinanseBiezace) //I o ile si� w og�le uda
				{
					Dlugi-=kwota;
					FinanseBiezace-=kwota;
				}
		   }
		}
		else //Jaki� inny komunikat finansowy
		{
			//MO�E JAKI� MOJ PROCES BEDZIE ZAINTERESOWANY? (na dole generyczne przegladanie)
		}
	}
	else
	if(Co->Rodzaj()=="?SZUKAM.PRODUCENTA")
	{
		DziedzinaWKolorze D=PodajDziedzine(); //Czy si� to miesci w naszej branzy
		D=D.ARGB & Co->PodajDziedzine().ARGB;
		D.A=0;//To nas nie interesuje. Chwilowo?
		if(D.IleBitow()>0            //Jakies wspolne bity
		&& _ZespolRoboczy::_PodobienstwoDziedzin(D,PodajDziedzine())>DRAND())    //I ewentualnie prawdopodobienstwo
		{
			Komunikat* Klon=Co->Klonuj();
			Klon->UstawRodzaj("WYPRODUKUJE!");

			if(  ! Klon->Zwrotnie()  )//Jak towarzyski to musialby istnie� zwrotny link
			{   //A jak nie to tworzymy towarzyski do jednostki wskazanej w Co
				unsigned IndInny=Co->Nadawca();
				KomunikacjaTowarzyska* KT=dynamic_cast<KomunikacjaTowarzyska*>(Co);
				if(KT) IndInny=KT->OKimTaGadka();

				wb_ptr<KontaktTowarzyski>  Nowy( new KontaktTowarzyski(MojID(),IndInny,WAGA_NA_DZIENDOBRY,DRAND())  );
				Nowy->UstawDziedzine(D);
				unsigned Kanal=Swiat::WstawLacze(Nowy.give());       		assert(Kanal!=Swiat::INVINDEX);
				Klon->Zaadresuj(Kanal,true,0.1);		//Tu si� b��du juz nie spodziewam...
			}

			KomunikacjaTowarzyska* KlonT=dynamic_cast<KomunikacjaTowarzyska*>(Klon);
			if(KlonT) KlonT->OKimTaGadka()=MojID();
			if(Swiat::WstawInfo(Klon)==Swiat::INVINDEX)
					TLOG(0,<<"ODPOWIEDZ NIE WYSLANA" )
					else
					return ;  //Czyli obsluzyl?
		}
	}
	else
	{                             										assert(Co->Kanal()!=Swiat::INVINDEX);
	//Obr�bka domy�lna
	GenerycznyWezelSieci::InterpretujKomunikat(Co);
	}
}

void Firma::ChwilaDlaCiebie()
{
	if(Doswiadczenie<0.33)
		_ZarzadcaProcesowFantazyjny();//Mocno niedo�wiadczona firma
	else
		_ZarzadcaProcesowFinezyjny();
		//_ZarzadcaProcesowSprawiedliwy(); //�le dzia�a :-(

	//Mo�e przeliczy� swoj� wag�
	//float    FinanseBiezace; //Ile firma ma kasy aktualnie LUB UPROSZCZONA "ZAMO�NO��"
	//float    ;//Ile firma ma obiecanych �rodk�w kredytowych
	//float 	  Dlugi;//Ile firma ma do sp�aty. Im wi�cej tym mniejsza szansa �e co� dostanie nowego
	double bilans=(FinanseBiezace+FinanseZaplanowane)-Dlugi;
	bilans*=this->Doswiadczenie;
	if(bilans>1)
		bilans=log10(bilans);
		else
		bilans=0.9;
	bilans/=3;//Typowe sumy s� 10^6 wi�c by�oby za du�o
	Waga=(0.99*Waga)+(0.01*bilans);  //W�drowna �rednia
				//Na razie dzia�a tylko na d�ugo�� proces�w bo rozmiar wielok�t�w zosta�
				//zaplanowany niestety jako sta�y
}

void RynekZbytu::ChwilaDlaCiebie()
//Proces dystrybucji produkt�w,
//oraz(?) zmiany potrzeb w mechanizmie wp�ywu i epidemi
{
	GenerycznyWezelSieci::ChwilaDlaCiebie();
}

DziedzinaWKolorze RynekZbytu::_DaSieSprzedac(const DziedzinaWKolorze CoZaProdukt)
{
	 DziedzinaWKolorze D;
	 unsigned Indeks; double WzglednePobienstwo;
	 D=_ZnajdzNajpodobniejszy(CoZaProdukt,Indeks,WzglednePobienstwo);//unsigned IleProb)<==oznacza przeszukanie liniowe

	 if( pow(WzglednePobienstwo,3) > DRAND() ) //Musi by� nieliniow, �eby si� naprawd� liczy�o podobienstwo do niszy
		return D;     //Da si�
		else
		{
			if(DRAND()<0.1)//Z rzadka nisza si� blokuje, zwlaszcza gdy produkt nietrafiony
			{
				Tablica[Indeks].A=RANDOM(256); Tablica[Indeks].R=RANDOM(256);
				Tablica[Indeks].G=RANDOM(256); Tablica[Indeks].B=RANDOM(256);
			}
			D.ARGB=0;
			return D;  //Tym razdem nie
		}
}

void RynekZbytu::InterpretujKomunikat(Komunikat* Co)
//Przyjmowanie komunikat�w typu PACZKA i zwracanie zap�aty za nie
{
	PaczkaProduktow* Paka=dynamic_cast<PaczkaProduktow*>(Co);
	if(Paka!=NULL)
	{
		if(_DaSieSprzedac(Paka->PodajDziedzine()).ARGB>0 )        //W _DaSie mo�e by� losowo��. Cz�� paczek idzie wtedy na przemia�!
		{
	   //Trzeba za to zaplaci�
		float WartoscZaplaty=Paka->IleSztuk()*Paka->CenaZaSzt();
		KomunikacjaFinansowa* Zaplata=new KomunikacjaFinansowa(KONTO,WartoscZaplaty);
		Zaplata->UstawDziedzine(Paka->PodajDziedzine()); //Za jaki produkt ta faktura

		if(Zaplata->Zaadresuj(Co->Kanal(),!Co->KierunekZgodny(),0.03)
			&& (Swiat::WstawInfo(Zaplata)!=Swiat::INVINDEX) )
				;//OK
				else
				{
				TLOG( 1, <<"Nie uda�o si� wys�a� zap�aty za paczk� "<<Paka->Rodzaj()<<"do w�z�a "<<Paka->Nadawca()  )
				}
		}
		else //Nie uda�o si� sprzeda�, zwracamy
		{
			Komunikat* Zwrot=Paka->Klonuj();//Mog�by jako� zaznacza� �e t zwrot?
			if(Zwrot->Zwrotnie()
			 && Swiat::WstawInfo(Zwrot)!=Swiat::INVINDEX)
				;//OK
				else
				TLOG(1, <<"Nie uda�o si� wys�a� zap�aty za paczk� "<<hex<<Co->PodajDziedzine().ARGB<<" Do "<<Co->Nadawca()  )
		}
	}
	else
	if(Co->Rodzaj()=="TEST")   //Niezale�nie od typu...
	{
		DziedzinaWKolorze D=Co->PodajDziedzine();
		unsigned Indeks; double WzglednePobienstwo;
		D=_ZnajdzNajpodobniejszy(D,Indeks,WzglednePobienstwo);//unsigned IleProb)<==oznacza przeszukanie liniowe
		if(WzglednePobienstwo>0.75)//Tylko naprawd� podobne i przy uproszczonej metryce nie jednobitowe
		{
			Komunikat* ODP=Co->Klonuj();
			ODP->UstawRodzaj("TEST.OK");
			ODP->UstawDziedzine(D);
			if(ODP->Zwrotnie()
			 && Swiat::WstawInfo(ODP)!=Swiat::INVINDEX)
			 ;
			 else
			 TLOG(1, <<"Nie uda�o si� wys�a� odpowiedzi na TEST rynkowy D:"<<hex<<Co->PodajDziedzine().ARGB<<" Do "<<Co->Nadawca()  )
		}
	}
	else
	//Inny rodzaj komunikatu lub nie sprzedana paczka
	WezelMacierzowy::InterpretujKomunikat(Co);
}


void Firma::_RozeslijNaleznosci(DziedzinaWKolorze Produkt,float& wplyw)
//Wg. identyfikatora produktu szuka komu winien dywidende
{
   float IleWydane=wplyw;//Wydane
   if(wplyw<1) //No grosze to ju� ignorujemy!!!!
			return;
   unsigned ile_linkow=Swiat::IlePowiazan();
   Powiazanie* Link;
   FormalnaKooperacja* FK=NULL;
   for(unsigned i=0;i<ile_linkow;i++)
   if((Link=Swiat::Lacze(i))!=NULL
   &&  Link->PodajDziedzine()==Produkt  //Jest zwi�zany z tym produktem
   && (Link->Poczatek()==MojID() ) //  || Link->Koniec()==MojID()  ZAP�TLA!!!
   && (FK=dynamic_cast<FormalnaKooperacja*>(Link))!=NULL
   )
   {
		//Trzeba b�dzie za to zaplaci�
		float WartoscZaplaty=wplyw*FK->JakiUdzial();
		KomunikacjaFinansowa* Zaplata=new KomunikacjaFinansowa(KONTO,WartoscZaplaty);
		Zaplata->UstawDziedzine(Produkt); //Za jaki produkt ta kasiora
		bool Kierunek=FK->Poczatek()==MojID();
		if(Zaplata->Zaadresuj(i,Kierunek,0.03)
		&& (Swiat::WstawInfo(Zaplata)!=Swiat::INVINDEX) )
			{    //Posz�o chyba?
				if(IleWydane>WartoscZaplaty)//Placi� trzeba, nawet jak zabraknie bo umowa g�upia
					IleWydane-=WartoscZaplaty;
				else
					Dlugi+=WartoscZaplaty;//Powieksza d�ug, mo�e potem si� wyr�wna
			}
			else
			TLOG(1, <<"\""<<Nazwa()<<"\" nie uda�o si� wys�a� dywidendy za produkt D:"<<hex<<Produkt.ARGB<<dec<<" "<<WartoscZaplaty )
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
DziedzinaWKolorze RynekZbytu::_DaSieSprzedac(const DziedzinaWKolorze CoZaProdukt)
{
	 DziedzinaWKolorze D;
	 D=CoZaProdukt;
	 unsigned ile_bitow=bits(D.A)+bits(D.R)+bits(D.G)+bits(D.B);
	 //Wersja uproszczona - im wi�cej bit�w tym wi�ksza szansa - losowe testowanie da�oby co� zbli�onego
	 //Chodzi o to �e nie zawsze uda si� sprzeda� i im bardziej wyspecjalizowany produkt tym trudniej
	 if(double(ile_bitow)/32.0 > DRAND() )
		return D;
		else
		{D.ARGB=0;return D;}
}
*/
