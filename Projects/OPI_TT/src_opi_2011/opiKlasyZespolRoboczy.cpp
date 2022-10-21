// Transfer technologii 2011 - w�a�ciwa implementacja modelu     25-10-2011
////////////////////////////////////////////////////////////////////////////////
// Implementacja pomocniczych metod
// z klasy  _ZespolRoboczy
////////////////////////////////////////////////////////////////////////////////

#include "opiKlasyWersja.hpp"
#include "INCLUDE/wb_smartlog.h"
using namespace wbrtm;


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
 float    Doswiadczenie;//POZIOM EKSPERTYZY? Kolejny mno�nik do liczby etat�w
 float    Centralno��;//0..1. Jak bardzo wydajno�� i ewentualnie aktywno�� zale�y od szefa/szef�w

 float    Proaktywnosc;//Aktywno��? 0..1. Podtrzymywanie aktywno�ci zewn�trznej, tworzenie link�w  itp.
 float    Czasochlonnosc;//Jaki jest wsp�czynnik czasoch�onno�ci typowej dzia�alno�ci. Wygenerowania pojedynczego wyniku ?
 float    Prolegalnosc;//????Uczciwo��??? Jak �atwo podj�� dzia�ania "szarostrefowe"
};
*/

float _ZespolRoboczy::_KalkulujKosztProduktu(DziedzinaWKolorze D)
//Furtka do "realistycznego" kalkulowania ceny produktu
{
	//Mo�na uzale�ni� od zawarto�ci bitowej Dziedziny!
	return D.IleBitow()*4+(D.R*0.5+D.G+D.B*2)/3.5;//Nadal trywialnie te koszty na razie, ale ju� ciekawiej
	//Kosztuje liczba bitow i niektore bity i niektore dziedziny s� dro�sze ni� inne
}



float _ZespolRoboczy::_PodobienstwoDziedzin(DziedzinaWKolorze A,DziedzinaWKolorze B)
{
	A.A=0; B.A=0; //Tylko kolor nas tu interesuje
	DziedzinaWKolorze Rozne;
	Rozne.ARGB=A.ARGB^B.ARGB; //XOR czyli r�nica bit�w.
	float res=bits(Rozne.R);
	res+=bits(Rozne.G);
	res+=bits(Rozne.B);
	res/=24.0; //24 maksymalnie mo�e si� r�ni� w 24 pozycjach
	return 1-res;//Zatem zwracane jest podobienstwo
}

bool _ZespolRoboczy::Poprawny()
 //Sprawdza czy wszystkie wska�niki s� poprawnie wpisane (nie przypadkowe i nie 0)
 {
	if(IleEtatow<1)
		   { cerr<<"IleEtatow???"<<endl;	  return false; }
	if(Czasochlonnosc<=0)
		   { cerr<<"Czasochlonnosc???"<<endl; return false; }
	if(Efektywnosc<=0 || 2<Efektywnosc)
		   { cerr<<"Efektywnosc???"<<endl;	 return false; }
	if(Doswiadczenie<=0 || 2<Doswiadczenie)
		   { cerr<<"Doswiadczenie???"<<endl; return false; }
	if(Centralnosc<=0 || 1<Centralnosc)
		   { cerr<<"Centralnosc???"<<endl;	 return false; }
	if(Proaktywnosc<=0 || 2<Proaktywnosc)
		   { cerr<<"Proaktywnosc???"<<endl;	 return false; }
	if(Prolegalnosc<=0 || 1<Prolegalnosc)
		   { cerr<<"Prolegalnosc???"<<endl;	 return false; }
	return true;
 }

void _ZespolRoboczy::ZmienWage(double mnoznik)
//Zmienia  b e z p i e c z n i e  (???) wag� powiazania. "mnoznik" mo�e by� wi�kszy lub mniejszy ni� 1
{
	this->Waga*=mnoznik;
}

unsigned _ZespolRoboczy::DajRynek()
//Do uzycia w r�nych sytuacjach
{
	static unsigned Index=Swiat::INVINDEX;
	//Sprawdza czy dawniej znaleziony nadal jest poprawny
	if(Index!=Swiat::INVINDEX && Swiat::Wezel(Index)!=NULL)
	{
		return Index;
	}
	else
	{  //Znajduje od nowa
		Index=Swiat::ZnajdzIndeksWezla("RYNEK");
		if(Index==Swiat::INVINDEX)
			Index=Swiat::ZnajdzIndeksWezla("MARKET");
		return Index;
	}
}

unsigned _ZespolRoboczy::DajUrzadPatentowy()
//UPATENTOWY | "URZ�D PATENTOWY" | patent office  | Patent Office
{
	static unsigned Index=Swiat::INVINDEX;
	//Sprawdza czy dawniej znaleziony nadal jest poprawny
	if(Index!=Swiat::INVINDEX && Swiat::Wezel(Index)!=NULL)
	{
		return Index;
	}
	else
	{  //Znajduje od nowa
		Index=Swiat::ZnajdzIndeksWezla("UPATENTOWY");
		if(Index==Swiat::INVINDEX)
			Index=Swiat::ZnajdzIndeksWezla("URZ�D PATENTOWY");
		if(Index==Swiat::INVINDEX)
			Index=Swiat::ZnajdzIndeksWezla("patentoffice");
		if(Index==Swiat::INVINDEX)
			Index=Swiat::ZnajdzIndeksWezla("Patent Office");
		return  Index;
	}
}

unsigned _ZespolRoboczy::DajPublikator()
//PUBLIKATOR |  publicator
{
	static unsigned Index=Swiat::INVINDEX;
	//Sprawdza czy dawniej znaleziony nadal jest poprawny
	if(Index!=Swiat::INVINDEX && Swiat::Wezel(Index)!=NULL)
	{
		return Index;
	}
	else
	{  //Znajduje od nowa
		Index=Swiat::ZnajdzIndeksWezla("PUBLIKATOR");
		if(Index==Swiat::INVINDEX)
			Index=Swiat::ZnajdzIndeksWezla("publicator");
		return  Index;
	}
}

//Implementacje r�nych "planer�w" wykonywania proces�w generycznych
//Niekt�re zu�ywaj� deklarowane zasoby zespo�u dosy� rozrzutnie, inne nieco lepiej
//////////////////////////////////////////////////////////////////////////////////////

void     _ZespolRoboczy::_ZarzadcaProcesowLosowy()
 //Wybiera proces do wykonania losowo anga�uj�c si� te� losowo
 {
	//Np. na posuni�cie do przodu najpilniejszego z proces�w
	unsigned ile=Swiat::IleProcesow(MojID());
	unsigned ktory=Swiat::NajpilniejszyProc(MojID(),NULL);
	if(ktory==Swiat::INVINDEX) //Nie ma nic do roboty
			return; //To wychodzimy z pracy :-);

	//Tylko "proces generyczny"	 lub typu potomnego nas tu interesuje
	GenerycznyProces* Roboczy=dynamic_cast<GenerycznyProces*>(Swiat::Proc(ktory,MojID()));
	if(Roboczy)
	{
		float SilaRobocza=IleEtatow*Efektywnosc*Doswiadczenie*(IleEtatow>2?1-Centralnosc:1); //Ile mocy
		double OIle=std::min(SilaRobocza, max(Roboczy->Prior,0.1f) ); //0.1 gdy prior<0.1
		Roboczy->TmpPorcjaPracy=OIle; //Zmienne w�a�ciwie tylko dla DEBUGingu i ewentualnie Logu.
		Roboczy->ChwilaDlaCiebie(); //Popchnij proces
	}
	//else //Proces inny ni� generyczny?  W modelu OPI takich nie ma (?)
	//Swiat::Proc(ktory,MojID())->ChwilaDlaCiebie();//Daje mu szanse zadzia�ania
 }

 void 	  _ZespolRoboczy::_ZarzadcaProcesowLeniwy()
 //Robi tylko najpilniejszy proces albo wcale
 {
	unsigned ile=Swiat::IleProcesow(MojID());
	unsigned ktory=Swiat::NajpilniejszyProc(MojID());

	if(ktory==Swiat::INVINDEX) //Nie ma nic do roboty
			return; //To wychodzimy z pracy :-)

	double   SilaRobocza=IleEtatow*Efektywnosc*Doswiadczenie*(IleEtatow>2?1-Centralnosc:1); //Ile mocy

	GenerycznyProces* Roboczy=dynamic_cast<GenerycznyProces*>(Swiat::Proc(ktory,MojID()));
	if(Roboczy)
	{
		double OIle;
		if(Roboczy->Prior==0)//Duuuu�o czasu do terminu...
			//Mo�na troch� popchn�� ale bez entuzjazmu
			OIle=SilaRobocza*0.75*DRAND();//Nie wi�cej ni� 0.75 normalnej wydolno�ci
		   else
			//Trzeba popchn�� - nie wi�cej ni� o maksymalny dzie� pracy wszystkich, czasem z extra spr�eniem
			OIle=min(SilaRobocza*(1+DRAND()*0.5),double(Roboczy->Prior));
		Roboczy->TmpPorcjaPracy=OIle;
		Roboczy->ChwilaDlaCiebie(); //Popchnij
	}
	//else //Proces inny ni� generyczny?  W modelu OPI takich nie ma (?)
	//Swiat::Proc(ktory,MojID())->ChwilaDlaCiebie();//Daje mu szanse zadzia�ania
 }

 void 	  _ZespolRoboczy::_ZarzadcaProcesowFantazyjny()
 //Robi pilny i jak mu co� zostaje si� to jeszcze jaki�
 {
	unsigned ile_proc_real; //Ile faktycznie ma proces�w do obslu�enia
	//Szukanie najpilniejszego przelicza priorytety
	unsigned ktory=Swiat::NajpilniejszyProc(MojID(),&ile_proc_real);
	if(ktory==Swiat::INVINDEX) //Nie ma w ogole nic do roboty
			return; //To wychodzimy z pracy :-)

	GenerycznyProces* Roboczy=dynamic_cast<GenerycznyProces*>(Swiat::Proc(ktory,MojID()));
															 //assert(Roboczy!=NULL);
	if(Roboczy==NULL) //To nie jest proces generyczny - nie interesuje nas w tym modelu
	{
		//Proces inny ni� generyczny?   W modelu OPI takich nie ma (?)
		//Swiat::Proc(ktory,MojID())->ChwilaDlaCiebie();//Daje mu szanse zadzia�ania
		return;//Miejsce na BRAKE-POINT
	}
											 //	assert(Roboczy->JakZaawansowany()<   ); //Nie zakonczony
	//Trzeba popchn�� - nie wi�cej ni� o maksymalny dzie� pracy wszystkich ale pewnie zazwyczaj mniej???
	double   SilaRobocza=IleEtatow*Efektywnosc*Doswiadczenie*(IleEtatow>2?1-Centralnosc:1);

	double   OIle=SilaRobocza;
	if(Roboczy->Prior==0) //Jak bardzo daleko do deadlineu
				 OIle/=2; //Tak co� tam podlubie, ale bez egzaltacji
	OIle=min(double(Roboczy->Prior),OIle);
	Roboczy->TmpPorcjaPracy=OIle;
	Roboczy->ChwilaDlaCiebie(); //Popchnij

	OIle=SilaRobocza-OIle; //Ile zosta�o mocy przerobowych?

	if(ile_proc_real>1 && OIle>0) //Czasem (z fantazji) robimy te� inny,
	{
		unsigned ile_proc=Swiat::IleProcesow(MojID());
		unsigned ile_prob=0;
		do{
		unsigned nowy=RANDOM(ile_proc); //Losujemy znowu jaki�
		if(ktory==nowy) return; //E... znowu to samo. Nie chcemy ju� priorytetowego
		Roboczy=dynamic_cast<GenerycznyProces*>(Swiat::Proc(nowy,MojID()));
		}while(Roboczy==NULL);    assert(Roboczy!=NULL);

		OIle=OIle*(0.1+0.8*DRAND());//Nie wi�cej ni� 0.9 mo�liwo�ci
		OIle=min(Roboczy->Priorytet(),OIle);//I nie wiecej ni� proces chce
		Roboczy->TmpPorcjaPracy=OIle;
		Roboczy->ChwilaDlaCiebie(); //Popchnij ten drugi
	}
 }

void 	  _ZespolRoboczy::_ZarzadcaProcesowFinezyjny()
 //Sprytnie dzieli czas �eby wszystko szlo do przodu
 {
	unsigned NID=this->MojID();
	unsigned ile_proc=Swiat::IleProcesow(NID);
	if(ile_proc==0)
			return; //NIE MA! NIC DO ZROBIENIA

	//Szukanie najpilniejszego przelicza priorytety
	unsigned najpilniejszy=Swiat::NajpilniejszyProc(NID);
	if(najpilniejszy==Swiat::INVINDEX)
			return;  //NIC DO ZROBIENIA - SAME "TRUPY" PROCES�W

	//Teraz trzeba popchn�� co si� da zwlaszcza te te bliskie konca
	Proces*  Roboczy=Swiat::Proc(najpilniejszy,NID);   							assert(Roboczy);
	GenerycznyProces* Pr=dynamic_cast<GenerycznyProces*>(Roboczy);              assert(Pr);

	double   SilaRobocza=IleEtatow*Efektywnosc*Doswiadczenie*(IleEtatow>2?1-Centralnosc:1);
	double   DlaPriorytu=min(Pr->Priorytet(),SilaRobocza*0.6);

	double   Wykonana=0;
	for(unsigned i=0;i<ile_proc;i++)
	{
		Roboczy=Swiat::Proc(i,NID);
		if(Roboczy==NULL)
				continue;  //Pusty slot procesu
		Pr=dynamic_cast<GenerycznyProces*>(Roboczy);
		if(Pr==NULL) //Nie jest typu konwertowalnego na generic?
		{								assert(Pr); //Raczej niespodziewane w tym modelu
		   //Roboczy->ChwilaDlaCiebie();//Daje mu szanse zadzia�ania  (???NIE POWINNO SI� ZDARZA�! )
		   continue; //Nie wie co wi�cej m�g�by z nim zrobi�
		}
		else //Popychanie proces�w konwertowalnych na "generic"
		{
		  if(Pr->Prior<=0) continue; //Ten proces (chwilowo) nie chce si� zaawansowa�
		  if(ile_proc==1)
		  {
			  Pr->TmpPorcjaPracy=SilaRobocza; //Jedyny dostaje wszystko
		  }
		  else   //Najpilniejszy proces nie jest jedyny - dostaje do 60% czasu pracy
		  {
			  if(i==najpilniejszy)
			  {
				Pr->TmpPorcjaPracy=DlaPriorytu;
			  }
			  else
			  {
				Pr->TmpPorcjaPracy=min(Pr->Priorytet(),(SilaRobocza*0.4)/(ile_proc-1));
			  }
		  }
		  Wykonana+=Pr->TmpPorcjaPracy;
		  Pr->ChwilaDlaCiebie();//Teraz popycha
		}
	}

	if(Wykonana<SilaRobocza) //Co� zosta�o?  Popychamy jeszcze najpilniejszy.
	{                     //Ale ostro�nie bo mo�e by� ju� "za kraw�dzi�" !!!!
	  SilaRobocza-=Wykonana ;
	  Roboczy=Swiat::Proc(najpilniejszy,NID);   							assert(Roboczy);
	  Pr=dynamic_cast<GenerycznyProces*>(Roboczy);              			assert(Pr);
	  if(!Pr->Poprawny() || Pr->JakZaawansowany()>=PROCES_NAD_ZAAWANSOWANY)
				return; //Nie nadaje si� ju� do powt�rnego wywo�ania!
	  Pr->TmpPorcjaPracy=min(Pr->Priorytet(),SilaRobocza);
	  if(Pr->TmpPorcjaPracy>0)
			Pr->ChwilaDlaCiebie();//Teraz znowu popycha
	}

 }

 void 	  _ZespolRoboczy::_ZarzadcaProcesowSprawiedliwy()
 //Dzieli wszystkie si�y proporcjonalnie do priorytet�w proces�w
 //Niestety procesy przeterminowane te� dostaj� tylko u�amek wi�c ci�gle chc� wi�cej!
 //Wi�c scheduler przydziela cora zmniejszy udzia� :-(
 //Trzeba wymy�li� co� innego
 {                                             assert("IMPLEMENTACJA NIE ZAKONCZONA!!!"==NULL);
	unsigned NID=this->MojID();
	unsigned ile_proc=Swiat::IleProcesow(NID);
	if(ile_proc==0)
			return; //NIE MA! NIC DO ZROBIENIA

	//Trzeba policzy� sum� wymaga�
	double SumaPrior=0;
	for(unsigned i=0;i<ile_proc;i++)
	{
		Proces* Roboczy=Swiat::Proc(i,NID);
		if(Roboczy==NULL)
				continue;  //Pusty slot procesu
		SumaPrior+=Roboczy->Priorytet(); //Liczy priorytety
	}

	//Ile mamy si�?
	double   SilaRobocza=IleEtatow*Efektywnosc*Doswiadczenie*(IleEtatow>2?1-Centralnosc:1);
	double   Wspolczynnik=SilaRobocza/SumaPrior;  //???

	if(Wspolczynnik<0.5)
	{
		IFTL(1) clog<<"\""<<Nazwa()<<"\" wykazuje powazny niedobor wydajnosci. Nadaza w "<<Wspolczynnik*100<<"%"<<endl;
		TLOG(2, <<"\""<<Nazwa()<<"\" wykazuje powa�ny niedob�r wydajno�ci. Nad��a w \t"<<Wspolczynnik*100<<"%"    )
	}

	for(unsigned i=0;i<ile_proc;i++)
	{
		Proces* Roboczy=Swiat::Proc(i,NID);
		if(Roboczy==NULL)
				continue;  //Pusty slot procesu
		GenerycznyProces* Pr=dynamic_cast<GenerycznyProces*>(Roboczy);
		if(Pr==NULL) //Jaki� zwykly!?
		{
			Roboczy->ChwilaDlaCiebie();//Te� popchnie...
			continue;
		}

		//Generyczny
		double Prior=Pr->Prior; //Policzone w pierwszej p�tli
		Pr->TmpPorcjaPracy=Prior*Wspolczynnik;    //?????????????????????????????????
		Pr->ChwilaDlaCiebie();//Popycha o odpowiedni udzia�
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
