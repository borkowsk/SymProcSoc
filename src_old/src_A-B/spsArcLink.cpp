////////////////////////////////////////////////////////////////////////////////
// Symulator Proces�w Sieciowych/Spolecznych (c) Instytut Studi�w Spo�ecznych
// Uniwersytetu Warszawskiego, ul. Stawki 5/7., 2011 , wborkowski@uw.edu.pl
///////////////////////////////////////////////////////////////////////////////
// Wersja okrojona dla OPI - Projekt "Transfer technologii 2011"
////////////////////////////////////////////////////////////////////////////////
//
// Deklaracja/Definicja linku og�lnego, ale rysowanego jako �uk
////////////////////////////////////////////////////////////////////////////////

#pragma hdrstop

#include "spsArcLink.h"
#include <cassert>
#include <cmath>
using namespace std;
//---------------------------------------------------------------------------
PowiazanieZagiete::KonstruktorElementowModelu<PowiazanieZagiete> PowiazanieZagiete::WirtualnyKonstruktor("arclink");

PowiazanieZagiete::~PowiazanieZagiete()
{
   //Nie musi robi� nic - robot� wykonuje destruktor klasy bazowej
}

PowiazanieZagiete::PowiazanieZagiete()
//Domyslny konstruktor ustawiaj�cy pusty link
{
	parametr=1;		krokow=0;
	Xa=Ya=Xb=Yb=Xo=Yo=Promien=alfa=beta=0;
}

PowiazanieZagiete::PowiazanieZagiete(double par)
//Domyslny konstruktor ustawiaj�cy pusty link Z PARAMETREM
{
	parametr=par;	krokow=0;
	Xa=Ya=Xb=Yb=Xo=Yo=Promien=alfa=beta=0;
}

void PowiazanieZagiete::UstawParametr(double par)
//Zmienia parametr wi�c i pomocnicze zmienne si� zmieniaja
{
	parametr=par;	krokow=0;
	Xa=Ya=Xb=Yb=Xo=Yo=Promien=alfa=beta=0;
}

//Metoda pobiera wszystkie potrzebne dane z listy string�w. Jak blad to podaje ktora pozycja
bool PowiazanieZagiete::ZrobWgListy(const std::string* Lista,unsigned Ile,unsigned& Blad)
{
   GenerycznePowiazanie::ZrobWgListy(Lista,Ile,Blad);
   //Teraz zostaje ustalenie parametru - jak jest zdefiniowany
   if(!Dane.KonwertujDo(6,parametr)) { Blad=6 ;return false;}
   return true;
}

const double Eps=1;

void PowiazanieZagiete::_PoliczParametryLuku()//Liczy parametry �uku dla danej wartosci parametru
//Pomocnicze pola zawieraj�ce niezb�dne parametry �uku
//	double Xa,Ya,Xb,Yb,Xo,Yo,Promien,alfa,beta;
{
   WezelSieci* Pocz=Swiat::Wezel(_S);          assert(Pocz!=NULL);
   WezelSieci* Koni=Swiat::Wezel(_T);          assert(Koni!=NULL);
   Xa=Pocz->X();
   Ya=Pocz->Y();
   Xb=Koni->X();
   Yb=Koni->Y();

   if(Xa==Xb && Ya==Yb) //Bardzo z�o�liwa sytuacja
		{Promien=0;krokow=0; return;} //NIC SIE NIE DA ZROBIC

   //Liczenie wsp�rz�dnych �rodka okr�gu z kt�rego bierzemy �uk
   if(fabs(Ya-Yb)<Eps) //Mniej ni� minimalne czyli pixel - pewnie i tak nie bedzie wida� r�znicy
   {
	  Xo=(Xa+Xb)/2;
	  Yo=(Ya+Yb)/2+parametr;
   }
   else
   if(fabs(Xa-Xb)<Eps)// j.w tylko �e w pionie
   {
	   Yo=(Ya+Yb)/2;
	   Xo=(Xa+Xb)/2+parametr;
   }
   else //Jednak trzeba troch� bardziej policzy�
   {
															assert(Yb-Ya!=0);
															assert(Xb-Xa!=0);
	 double t=-(Xb-Xa)/(Yb-Ya); 							assert(t!=0);
	 double Xs=(Xa+Xb)/2;
	 double Ys=(Ya+Yb)/2;
	 double b=Ys-t*Xs;
	 double c=-t*Xa-Ya;
	 if( 2*t*Xo+b+c >0 )
	 {
		Xo=(parametr*sqrt(t*t+1)-b-c)/(2*t);
		Yo=t*Xo+b;
	 }
	 else
	 {
		Xo=(-parametr*sqrt(t*t+1)-b-c)/(2*t);
		Yo=t*Xo+b;
	 }
   }
														assert(Xa!=Xo || Ya!=Yo);
   Promien=sqrt((Xa-Xo)*(Xa-Xo)+(Ya-Yo)*(Ya-Yo));   	assert(Promien>0);
   alfa=atan2( (Xa-Xo),(Ya-Yo) );
   beta=atan2( (Xb-Xo),(Yb-Yo) );
 //											assert(Xa==Xo+Promien*cos(alfa));    //NIE HALO :-(
 //											assert(Ya==Yo+Promien*sin(alfa));
   krokow=Promien/10;//Na razie tak zgrubnie
}

//Tak naprawd� to r�ni si� tylko sposobem rysowania
//Kt�ry zalezy od funkcji licz�cej punkty na �uku
void PowiazanieZagiete::PodajPozycje(double D,bool KierunekZwykly,double& X,double& Y,Komunikat* Messg)
//Przelicza polozenia wdluz linku
{
   WezelSieci* Pocz=Swiat::Wezel(_S);
   WezelSieci* Koni=Swiat::Wezel(_T);
   if(Pocz->X()!=Xa || Pocz->Y()!=Ya || Koni->X()!=Xb || Koni->Y()!=Yb)
		   _PoliczParametryLuku();//Kt�rys si� przesuna� albo zmienil si� parametr
   if(krokow==0) //Co� si� chyba nie uda�o
   {
	GenerycznePowiazanie::PodajPozycje(D,KierunekZwykly,X,Y,Messg);//RACZEJ TYMCZASOWO
	return;
   }
   //Wlasciwe obliczenie
   double angle;
   if(KierunekZwykly)
   {
   if(alfa>beta)
	angle=alfa-D*fabs(alfa-beta);
	else
	angle=alfa+D*fabs(beta-alfa);
   }
	else
   {
	angle=alfa+(1-D)*fabs(beta-alfa);
   }
   X=Xo+Promien*cos(angle);
   Y=Yo+Promien*sin(angle);
}

