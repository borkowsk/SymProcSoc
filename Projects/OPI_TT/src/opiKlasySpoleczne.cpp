// Transfer technologii 2011 - w�a�ciwa implementacja modelu     24-10-2011
//*//////////////////////////////////////////////////////////////////////////////
// Implementacja klas obs�uguj�cych proces spo�eczny: Procesu, Linku i Komunikatu
//*//////////////////////////////////////////////////////////////////////////////

#include "opiKlasyWersja.hpp"
#include "wb_smartlog.hpp"
#include "lingo.hpp"
using namespace wbrtm;

//char* const DZIENDOBRY="AUTOPREZENTACJA";//Marker otwieraj�cego komunikatu spo�ecznego
float  WAGA_NA_DZIENDOBRY=0.1;//Wsp�czynnik zmniejszania wagi przy pierwszym kontakcie - nie mo�e by� za du�a
float  TOWARZYSKA_NAJMNIEJSZA_WAGA=0.01;// dla	bool KontaktTowarzyski::Poprawny();
float  TEMPO_SPADKU_LINKU=0.025;//O jak� cz�� link socjalny zanika przy nie u�ywaniu
float  TEMPO_WZROSTU_LINKU=0.25;//O jak� cz�� link socjalny ro�nie przy odpowiedzi

static const int LOGSOC=3; //Na jakim poziomie logowania domy�lnie zapisywa� procesy spo�eczne

// Proces kreuj�cy nowe linki - losuje w�z�y i wg. podobie�stwa tworzy link i
// wysy�a nim zajawk� o sobie, nadaje wag� proporcjonaln� do wagi w�z�a (albo...)
// Jak nie utworzy linku to to wysy�a przypomnienie do jakiego� z istniej�cych
// powi�za� spo�ecznych zawieraj�ce losowe bity ze swojej dziedziny.
// Co jaki� czas aktualizuje dziedzin� na podstawie dziedziny swojego w�z�a.
// Jego procedura przechwytywania komunikat�w zajmuje si� odpowiadaniem na "zaczepki"
// o ile pochodz� od kogo� znanego, lub wartego poznania. Kieruje si� tu wag� linku
// spo�ecznego.
////////////////////////////////////////////////////////////////////////////////
//	private:
//	float Aktywnosc; //Pobierana z odpowiedniej warto�ci w�zla
static unsigned _JestPowiazanySocjalnie(unsigned Startowy,unsigned Docelowy);

KomunikacjaTowarzyska::KomunikacjaTowarzyska(const char * Kateg, unsigned int Temat)
{
	Dane.Realokuj(11); Dane[9]=Kateg; OKim=Temat;
}

const string& KomunikacjaTowarzyska::Rodzaj()
//Gdzie� w danych jest rodzaj tego komunikatu
{
    return Dane[9];
}

bool  KomunikacjaTowarzyska::UstawRodzaj(const char* Nowy)
//A jak typ nie udost�pnia zmiany to zwraca false
{
	Dane[9]=Nowy;
	return true;
}

void ProcesSpoleczny::ChwilaDlaCiebie()
//Tworzenie nowych kontakt�w i podtrzymywanie starych

{
	unsigned    MPRNID=this->Procesor();     					assert(MPRNID!=Swiat::INVINDEX);
	WezelSieci* MojProcesor=Swiat::Wezel(MPRNID);
	if(this->PodajDziedzine().ARGB==0) //Skopiowanie dziedziny gdy jej nie ma
				this->UstawDziedzine(MojProcesor->PodajDziedzine());

	//To co jest naprawd� do zrobienia to czasem nowe linki, a czasem podtrzymanie starych
	for(float i=-0.01;i<this->TmpPorcjaPracy;i+=1.0)//Zaczyna troch� poni�ej 0 �eby zawsze cho� jedn� pr�b� zrobi�
	{
	  //Im wi�ksza aktywno�� tym �atwiej wylosowa� dzia�anie
	  if(DRAND()<Aktywnosc)//Jedna pr�ba nawi�zania nowego po��czenia
	  {

		unsigned IndInny=RANDOM(Swiat::IleWezlow());
														assert(IndInny<Swiat::IleWezlow());
		if(IndInny==MPRNID) continue; //Sam do siebie nie kontaktuje

		GenerycznyWezelSieci* PtrInny=dynamic_cast<GenerycznyWezelSieci*>(Swiat::Wezel(IndInny));

		if(PtrInny==NULL) continue; //Jak slot pusty lub nie pochodzi od "GenerycznyWezelSieci"

		float Podob;
		if( (Podob=_ZespolRoboczy::_PodobienstwoDziedzin( PodajDziedzine() , PtrInny->PodajDziedzine() )) > DRAND()
			&& _JestPowiazanySocjalnie(MPRNID,IndInny)==Swiat::INVINDEX
			)
			{
			   double Waznosc=PtrInny->Waznosc();
			   unsigned Kanal=_StworzKanal(IndInny,Podob*pow(Waznosc,0.5)*WAGA_NA_DZIENDOBRY,0);//Parametr==0 czyli losowana krzywizna
			   if(Kanal!=Swiat::INVINDEX)
					_WyslijAutoprezentacje(Kanal,true);//Pierwsza zawiera ca�� informacje o dziedzinie wez�a
			}
	   }
	   else
	   if(DRAND()<Aktywnosc)// Jedna pr�ba podtrzymania kontaktu
	   {  //LOSOWANIE WG. WAZNOSCI LINK�W!!!
		  wb_dynarray<unsigned> ListaLinkow;   //Chyba powinien by� jaki� spos�b dostarczany przez interface
		  wb_dynarray<float>	ListaWag;
		  double 				suma_wag=0;
		  unsigned ile_naprawde=0;
		  unsigned ile=Swiat::IlePowiazan();
		  ListaLinkow.alloc(ile);
			 ListaWag.alloc(ile);
		  Powiazanie* P=NULL;
		  for(unsigned i = 0; i < ile; i++)
		  {
			P=Swiat::Lacze(i);
			if( P!=NULL
			&& P->Poczatek()==Procesor()
			&& dynamic_cast<KontaktTowarzyski*>(P)!=NULL
				)
				{
					ListaLinkow[ile_naprawde]=i;
					ListaWag[ile_naprawde]=P->Waznosc();
					suma_wag+=ListaWag[ile_naprawde];
					ile_naprawde++;
				}
		  }

		  double strzal=DRAND()*suma_wag;
		  suma_wag=0;
		  for(unsigned i = 0; i < ile_naprawde; i++)
		  {
			 suma_wag+=ListaWag[i];
			 if(strzal<suma_wag)//TO TU!
			 {
				_WyslijAutoprezentacje(ListaLinkow[i],false);
				break; // i ju�!!!
			 }
		  }
	   }
	}

	GenerycznyProces::ChwilaDlaCiebie();//Tu robimy to co wynika z dziedziczenia po prostrzym procesie

	//Buchalteria - Co jaki� czas (7 dni?) zaczyna si� od pocz�tku
	if(JakZaawansowany()>=1) //W�a�nie zakonczony
	{
		//to go restartujemy po prostu!!!
		Wymazuj();  //Zaznaczamy koniec. Ale nie koniecznie b�dzie to widoczne na ekranie

		if(fabs(this->_IleKrokowDoKonca)>7) //Op�znienia/przedterminy nie s� bardzo istotne chyba �e du�e
		{
		   IFTL(LOGSOC+1)clog<<endl<<MojProcesor->Nazwa()<<' '<<_LPL("wznowil proces","reactivated proc.")<<' '<<Dane[1]<<':'<<hex<<PodajDziedzine().ARGB<<_LPL(" przed/po terminie: "," befor/after deadline: ")<<dec<<_IleKrokowDoKonca<<_LPL(" krok(ow)"," step(s)")<<endl;
		   TLOG(LOGSOC+1, <<MojProcesor->Nazwa()<<' '<<_LPL("wznowil proces","reactivated proc.")<<' '<<Dane[1]<<':'<<hex<<PodajDziedzine().ARGB<<_LPL(" przed/po terminie: "," befor/after deadline: ")<<dec<<_IleKrokowDoKonca<<_LPL(" krok(ow)"," step(s)")          )
		}

		PracaDotychczasowa=0;  //Uznajemy �e mamy od nowa prac� do wykonania
		if(_IleKrokowDoKonca<0) _IleKrokowDoKonca=7;//Procesy spo�eczne maj� cykl tygodniowy,
		   else	_IleKrokowDoKonca=14;// ...ale jak si� sko�cz� za wcze�nie, to robimy przerw�

		//Tu trzeba ustali� sensowny limit pracy do wykonania
		_ZespolRoboczy* ProcesorZR=dynamic_cast<_ZespolRoboczy*>(MojProcesor);
		if(ProcesorZR) //Je�li uda�o si� sprowadzi� do tego "mianownika"
		{
			if(Dane.KonwertujDo(8,Aktywnosc)!=-1)
					  Aktywnosc=0.1;//Domy�lne aktywno�� nie mo�e by� za du�a
			WymaganaIloscPracy=7*ProcesorZR->IleEtatow*Aktywnosc*ProcesorZR->Proaktywnosc;
			if(WymaganaIloscPracy<1) WymaganaIloscPracy=1; //Zeby na pewno raz si� wywo�a�
		}
		else
		{ 	WymaganaIloscPracy=7; }//Tu na wypadek (?) gdyby w�ze� nie by� z rodziny _ZespolRoboczy

		Prior=1; //Na wszelki wypadek. Chyba i tak si� zaraz zmieni
	}
}

bool ProcesSpoleczny::InterpretujKomunikat(Komunikat* Co)
//Ewentualnie odpowiada na komunikat. Ale nie tak czy inaczej  (czasem???)
//nie uznaje za obrobiony wi�c mo�e zosta� jeszcze jako� przetworzony
//przez inne procesy. Np. zapami�tany.
{                                                                       assert(Co->Kanal()!=Swiat::INVINDEX);
	KomunikacjaTowarzyska* Info=dynamic_cast<KomunikacjaTowarzyska*>(Co);
	if(Info!=NULL)
	{   //Je�li uznamy za warty odpowiedzi to pr�bujemy odpowiedzie�
		float Podob;
		if( (Podob=_ZespolRoboczy::_PodobienstwoDziedzin( PodajDziedzine() , Info->PodajDziedzine() )) > DRAND()  )
		{    //TODO!: TA GALA� MOZNA JUZ ZROBIC BEZPIECZNIEJ, ALE TO KIEDYS!
		   unsigned IndInny=Co->Nadawca();//Ustalenie nadawcy komunikatu, bo zaraz mo�e znikn��
											assert(Co->Odbiorca()==Procesor());//!!!
		   unsigned IndKanal=Co->Kanal(); //Podobnie informacja o kanale

		   //Probujemy znale�� kanal zwrotny
		   wb_ptr<Komunikat> Tmp( Co->Klonuj() );   //Troch� na okolo ale ...
		   Tmp->Zwrotnie();    //W zwrotnie dla Kom. spoelcznych jest to zaimlementowane
		   unsigned Kanal=Tmp->Kanal();//... odczytac go ...

		   if(Kanal==Swiat::INVINDEX)//I je�li nie uda sie adresowanie zwrotne
		   {
			 //To trzeba stworzy� nowy link. Info jest ju� niepoprawne...
			 WezelSieci* PtrInny=Swiat::Wezel(IndInny);
			 KontaktTowarzyski* KT=dynamic_cast<KontaktTowarzyski*>(Swiat::Lacze(IndKanal));
			 double Parametr=0;//W razie czego losowanie
			 if(KT) Parametr=KT->DajWygiecie(); //Takie samo wygiecie, ale link w drug� stron�
			 double Waznosc=PtrInny->Waznosc();
			 Kanal=_StworzKanal(IndInny,Podob*pow(Waznosc,0.5)*WAGA_NA_DZIENDOBRY,Parametr);
			 if(Kanal!=Swiat::INVINDEX)
					_WyslijAutoprezentacje(Kanal,true);//Otwieraj�ca zawiera ca�� informacje o dziedzinie wez�a

			 //GDY ZNISZCZYL KOMUNIKAT TO GO NIKT INNY NIE MOZE OBRABIA�!
			 if(Co->Kanal()==Swiat::INVINDEX)//Zabezpieczenie po b��dzie
					return true;
		   }
		   else //Adresowanie zwrotne si� uda�o - kto� kogo ju� znamy
		   {
			if(DRAND()>0.5) //Nawet ciekawe konwersacje w koncu wygasaj�. Inaczej zrobi nat�ok informacji!!!
			  _WyslijAutoprezentacje(Kanal,false); //Wysy�amy jeden bit ze swojej dziedziny - co i tak jest du�� ilo�ci� informacji
		   }
		}																assert(Co->Kanal()!=Swiat::INVINDEX);
	}
	else
	{   //Podgladanie o autorstwa i proba nawiazania kontaktu
		KomunikacjaOficjalna* Info=dynamic_cast<KomunikacjaOficjalna*>(Co);
		unsigned IndKanal=Co->Kanal(); //Podobnie informacja o kanale
		float Podob;
		if(Info                       //Moze nie za cz�sto, bo raporty i prezezentacje s� zbyt powszechne w bliskich relacjach?
		&& pow(double(Podob=_ZespolRoboczy::_PodobienstwoDziedzin(PodajDziedzine() , Info->PodajDziedzine() )),1.2) > DRAND() )
		{                         //!!!!!!!!!!!!!!!
			unsigned IndInny=Info->KtoJestAutorem(); //Bardziej nas interesuje autor ni� nadawca!
			if(IndInny == Procesor() )
					return false; //...ale jak dostaniemy po�rednio co� o sobie to nic nam z tego
			if(IndInny==Info->Nadawca())
					return false;//Interesuja nas tylko sytuacje po�rednie
			unsigned Zwrotny;
			if((Zwrotny=_JestPowiazanySocjalnie(Procesor(),IndInny))==Swiat::INVINDEX) //Nie ma takiego polaczenia
			{
			 //Trzena stworzy� nowy kana� spo�eczny. Komunikacja oficjalna mog�a nie przyj�� po linku spo�ecznym
			 WezelSieci* PtrInny=Swiat::Wezel(IndInny);
			 double Parametr=0;//W razie czego losowanie
			 KontaktTowarzyski* KT=dynamic_cast<KontaktTowarzyski*>(Swiat::Lacze(IndKanal));
			 if(KT) Parametr=KT->DajWygiecie(); //Dziedziczy wygiecie po przekazicielu
			 double Waznosc=PtrInny->Waznosc();                   //Waga na "DZIEN DOBRY" zale�y od wagi linku ktorym przysz�o
			 unsigned NowyKanal=_StworzKanal(IndInny,Podob*pow(Waznosc,0.5)*(KT?KT->Waznosc():WAGA_NA_DZIENDOBRY),Parametr);
			 if(NowyKanal!=Swiat::INVINDEX)
					_WyslijAutoprezentacje(NowyKanal,true);//Otwieraj�ca zawiera ca�� informacje o dziedzinie wez�a
			}
			else //Ju� si� znamy...
			{                                         assert(Zwrotny!=Swiat::INVINDEX);
				//Mo�na by przytakn��....
			  //	if(DRAND()>0.3) //Ale czy zawsze? Bo od podtrzymywania link�w jest normalny proces spoleczny
					_WyslijAutoprezentacje(Zwrotny,false);
			}
		}                                                           assert(Co->Kanal()!=Swiat::INVINDEX);
	}
																	assert(Co->Kanal()!=Swiat::INVINDEX);
	return false; // mo�e zosta� jeszcze jako� przetworzony dalej
}

unsigned ProcesSpoleczny::_JestPowiazanySocjalnie(unsigned Startowy,unsigned Docelowy)
{
  unsigned ile=Swiat::IlePowiazan();
  Powiazanie* Lacze=NULL;
  KontaktTowarzyski* Debugowy=NULL;
  for(unsigned i=0;i<ile;i++)
  {
   if((Lacze=Swiat::Lacze(i))!=NULL)
   {
	if( Lacze->Poczatek()==Startowy
	&&  Lacze->Koniec()==Docelowy
	&& ((Debugowy=dynamic_cast<KontaktTowarzyski*>(Lacze))!=NULL)
	  )
	  {
		IFTL(LOGSOC)clog<<endl<<"Jest powiazanie "<<Startowy<<"---->"<<Docelowy<<" "<<Debugowy->Poczatek()<<' '<<Debugowy->Koniec()<<endl;
		TLOG(LOGSOC, <<"Jest powiazanie "<<Startowy<<"---->"<<Docelowy<<" "<<Debugowy->Poczatek()<<' '<<Debugowy->Koniec() )
		return i;
	  }
   }
  }
  //Zadnego spe�niaj�cego wymaganie nie znaleziono
  IFTL(LOGSOC)clog<<endl<<"Brak  powiazania "<<Startowy<<"-//->"<<Docelowy<<endl;
  TLOG(LOGSOC, <<"Brak  powiazania "<<Startowy<<"-//->"<<Docelowy      )
  return Swiat::INVINDEX;
}

bool ProcesSpoleczny::_WyslijAutoprezentacje(unsigned kanal,bool wszystko)
//Pomocnicza procedura wysy�ania jakiej� informacji o sobie (lub jakiejs z bazy?)
{
	Powiazanie* Lacze=Swiat::Lacze(kanal);

	wb_ptr<KomunikacjaTowarzyska> Info( new  KomunikacjaTowarzyska("AUTOPREZENTACJA",this->Procesor()) );
	if(Lacze->Poczatek()==this->Procesor()) //Typowa sytuacja z linkiem socjalnym lub symetrycznym w dobra stron�
		Info->Zaadresuj(kanal,true,0.1+DRAND()*0.1);
		else //Jaki� link symetryczny prawdopodobnie
		{                                   //W tej sytuacji Koniec jest poczatkiem
											assert(Lacze->Koniec()==this->Procesor());
		  if(!Info->Zaadresuj(kanal,false,0.1+DRAND()*0.1) )
						return false;
		}

	if(wszystko)
		Info->UstawDziedzine(PodajDziedzine());
		else
		if(PodajDziedzine().ARGB==0)//Rzadka ale niebezpieczna sytuacja - zadnych bit�w!!!
		{
			Info->UstawDziedzine(PodajDziedzine());//??? KTO Z NIM POGADA O NICZYM!
			//mo�e lepiej tu zrezygnowa�?
		}
		else
		{ // przygotowanie zawarto�ci komunikatu
		  DziedzinaWKolorze Nowa;
		  do{
		   //unsigned maska=0x1 << RANDOM(32);//Jaki� bit z dziedziny. Jak wyjsdzie w Alfie to trudno  - ALE ZBYT CZARNE!
			unsigned maska= (1+RANDOM(255)) << (RANDOM(32));//Pare bit�w z dziedziny. Wi�ksza szansa na kolor
			Nowa.ARGB=PodajDziedzine().ARGB & maska;
		  }while(Nowa.ARGB==0);//Powtarza a� co� trafi
		  Info->UstawDziedzine(Nowa);
		}

	IFTL(LOGSOC)clog<<endl<<"Wysylam info "<<unsigned(Info->Nadawca())<<"====>"<<unsigned(Info->Odbiorca())<<" "<<hex<<Info->PodajDziedzine().ARGB<<dec<<' ';
	TLOG(LOGSOC, <<"Wysylam info "<<unsigned(Info->Nadawca())<<"====>"<<unsigned(Info->Odbiorca())<<" "<<hex<<Info->PodajDziedzine().ARGB<<dec<<' ' )

	bool Result=Swiat::WstawInfo(Info.give())!=Swiat::INVINDEX; //Zwroci true jak wys�a�

	IFTL(LOGSOC)clog<<(Result?"OK":"Upsss!!!")<<endl;
	TLOG(LOGSOC, <<(Result?"OK":"Upsss!!!")     )
	return Result;
}

unsigned ProcesSpoleczny::_StworzKanal(unsigned IndInny,float Waga,float Parametr)//I nowy kanal z ju� obliczona wag�
														//Jak Parametr =0 to jest losowanie ale z dodatnich!
{
	WezelSieci* PtrInny=Swiat::Wezel(IndInny);
	if(PtrInny==NULL) //Przestal istnie�?!?!
			return Swiat::INVINDEX;

	DziedzinaWKolorze Nowa;
	Nowa.ARGB=PtrInny->PodajDziedzine().ARGB & PodajDziedzine().ARGB;
	if(Nowa.A==0 && Nowa.R==0 && Nowa.G==0 && Nowa.B==0)
			return Swiat::INVINDEX;  //0 wsp�lnych temat�w, o czym mieliby gada�????

	//Nowa.A=0; //Alfa nieistotna ???
	if(Waga>1) Waga=1;
	if(Parametr==0)
			Parametr=0.1+0.9*DRAND(); //A wygiecie mo�e by� losowe
	KontaktTowarzyski* KT=new KontaktTowarzyski(Procesor(),IndInny,
								Waga,    //Polaczenia powstale losowo na "dzien dobry" nie powinny by� silne
								Parametr
								);
	KT->UstawDziedzine(Nowa);

	IFTL(LOGSOC)clog<<endl<<_LPL("Nowy link towarzyski","New social connection")<<" "<<dec<<unsigned(KT->Poczatek())<<"-->"<<unsigned(KT->Koniec())<<" W:"<<KT->Waznosc()<<endl;
	TLOG(LOGSOC, <<_LPL("Nowy link towarzyski","New social connection")<<" "<<dec<<unsigned(KT->Poczatek())<<"-->"<<unsigned(KT->Koniec())<<" W:"<<KT->Waznosc()  )
	unsigned Kanal=Swiat::WstawLacze(KT);//Tworzy nowy kanal. Zapami�tany �eby mo�na by�o podejrze� w debugerze
	return Kanal;
}

KontaktTowarzyski::KontaktTowarzyski(unsigned Kogo,unsigned ZKim,double JakaWaga,double JakieWygiecie)
{
	_S=Kogo;  _T=ZKim;  Waga=JakaWaga;
	_Z=RANDOM(10);
	parametr=JakieWygiecie;
}

void KontaktTowarzyski::ZmienWage(double mnoznik)
//Zmienia bezpiecznie wag� powiazania. "mnoznik" mo�e by� wi�kszy lub mniejszy ni� 1
{
	this->Waga*=mnoznik;  //Ro�nie waga lub d��y do 0
	if(Waga>1) Waga=1;  //Ale nie ro�nie powy�ej 1 !!!
}

bool KontaktTowarzyski::Poprawny()
{
	if(this->Waga<TOWARZYSKA_NAJMNIEJSZA_WAGA)//np. =0.001;
	{  //Najprawdopodobniej za chwil� zostanie usuni�te
	   IFTL(LOGSOC) clog<<_LPL("Usuwam powiazanie","Deleting connection")<<" "<<dec<<unsigned(Poczatek())<<(Kierunkowy()?"-->":"<-->")<<unsigned(Koniec())<<" D:"<<hex<<PodajDziedzine().ARGB<<dec<<" W:"<<Waznosc()<<endl;
	   TLOG(LOGSOC, <<_LPL("Usuwam powiazanie","Deleting connection")<<" "<<dec<<unsigned(Poczatek())<<(Kierunkowy()?"-->":"<-->")<<unsigned(Koniec())<<" D:"<<hex<<PodajDziedzine().ARGB<<dec<<" W:"<<Waznosc() )
	   return false;
	}
	else
	   return PowiazaniePaboliczneSkierowane::Poprawny(); //Mo�e by� niepoprawne z innych powod�w
}

void KontaktTowarzyski::ChwilaDlaCiebie()
//Link "paruje" z czasem - jego waga spada
{                    assert(0<TEMPO_SPADKU_LINKU && TEMPO_SPADKU_LINKU<1);
	ZmienWage(1.0-TEMPO_SPADKU_LINKU);//Mno�y przez taki u�amek
}

bool KontaktTowarzyski::Akceptacja(Komunikat * Co)
//Przyj�cie komunikatu zwi�ksza wag� ��cza
{
	 bool Wynik=PowiazaniePaboliczneSkierowane::Akceptacja(Co);

	 if(Wynik)//Komunikat przyj�ty
		ZmienWage(1+(TEMPO_WZROSTU_LINKU/10.0));  //Ro�nie waga o troch�, 10 x wi�cej gdy zwrotny

	 return Wynik;
}

bool KomunikacjaTowarzyska::Zwrotnie(float _Szybkosc)
{                                          							assert(Link!=Swiat::INVINDEX);
	Powiazanie* P=Swiat::Lacze(this->Link);
	if(P==NULL)//��cze w mi�dzyczasie przesta�o istnie� wi�c nie ma sk�d wzi�� informacji o nim
		   {  this->Link=-1; return false; }

	KontaktTowarzyski* Zrodlowy=NULL;//Potrzebny
	if((Zrodlowy=dynamic_cast<KontaktTowarzyski*>(P))==NULL )
			return  GeneryczneInfo::Zwrotnie(_Szybkosc); //Jak nie kontakt towarzyski to domyslnie obslugiwany

	//Kierunek=!Kierunek; //NIE MA ZMIANY KIERUNKU BO PO JEDNOKIERUNKOWYCH ZAWSZE CHODZI "Z PR�DEM"
	//A �e nie mo�na "pod pr�d" linkiem kierunkowym to trzeba znale�� symetryczny
	unsigned ile=Swiat::IlePowiazan(); KontaktTowarzyski* Debugowy=NULL;
	unsigned Startowy=P->Koniec();  //Szukamy wi�c takiego kt�rego poczatek bedzie koncem
	unsigned Docelowy=P->Poczatek();//a koniec pocz�tkiem
	unsigned indx=Swiat::INVINDEX; //Na indx wychodzi informacja gdy si� znajdzie
	for(unsigned i=0;i<ile;i++)
	{
	 Powiazanie* Lacze;
	 if((Lacze=Swiat::Lacze(i))!=NULL)
	 {
	  if(  Lacze->Poczatek()==Startowy
	   && Lacze->Koniec()==Docelowy
	   && ((Debugowy=dynamic_cast<KontaktTowarzyski*>(Lacze))!=NULL)
	   )
	  {
		indx=i;
		break;
	  }
	 }
	}
	if(indx==Swiat::INVINDEX) //Nie uda�o si� znale��
		   {  this->Link=-1; return false; }
	//UDALO SIE
	Zrodlowy->ZmienWage(1+TEMPO_WZROSTU_LINKU);//Jak udana adresowanie zwrotne to podwy�szamy wag� linku
	this->Link=indx;
	if(_Szybkosc!=0)
		Predkosc=_Szybkosc;//A je�li jest r�wne 0 to zostawia to co by�o
	if(Predkosc<=0) //A jak wysz�o mniejsze bad� r�wne 0 to nadaje predko�� awaryjn�
		Predkosc=0.000000001;//Awaryjna pr�dko�c komunikatu
	LicznikZycia=0;
	return true;
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
	   IFTL(LOGBASE+1) clog<<_LPL("Usuniete powiazanie","Deleted connection")<<" "<<dec<<unsigned(_Polaczenia[Numer]->Poczatek())<<(_Polaczenia[Numer]->Kierunkowy()?"-->":"<-->")<<unsigned(_Polaczenia[Numer]->Koniec())<<" D:"<<hex<<_Polaczenia[Numer]->PodajDziedzine().ARGB<<dec<<"W:"<<_Polaczenia[Numer]->Waznosc()<<endl;
	   TLOG(LOGBASE, <<_LPL("Usuniete powiazanie","Deleted connection")<<" "<<dec<<unsigned(_Polaczenia[Numer]->Poczatek())<<(_Polaczenia[Numer]->Kierunkowy()?"-->":"<-->")<<unsigned(_Polaczenia[Numer]->Koniec())<<" D:"<<hex<<_Polaczenia[Numer]->PodajDziedzine().ARGB<<dec<<"W:"<<_Polaczenia[Numer]->Waznosc() )
 */
