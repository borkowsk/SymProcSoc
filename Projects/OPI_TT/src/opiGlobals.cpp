// Transfer technologii 2011
// //////////////////////////////////////////////////////////////////////////////////////
// G��wne definicje parametr�w modelu, ewentualnych globalnych funkcji i innych danych
// //////////////////////////////////////////////////////////////////////////////////////

#include "spsModel.h"

#include "compatyb.hpp"
#include "wb_ptr.hpp"
#include "wb_smartlog.hpp" //Bo zmieniamy parametr loglevel
#include "lingo.hpp"
#include <cstring>

//Zdefiniowane w "opiKlasyGeneryczne*.cpp"
extern float GENERYCZNY_POZIOM_PRZECIEKOW; //=0.01;//Jak cz�sto domy�lny komunikat leci dalej ni� do odbiorcy

char PlikWejsciowy[256]=""; //Wej�ciowy plik struktury (dat)
char DelimiterDanych='\t';  //Jaki znak s�u�y do rozdzielania danych w pliku DAT (mo�e by� te� : ; , | \ / itp
int VerboseInput=1;         //Parametr konfiguracyjny VERBOSE
int hor=800; //790?;
int ver=600; //570?;
double   JakieRwProcWidth=5; //=5;//Parametr konfiguracyjny RADIUS
unsigned DefMaxLiczbaWezlow=2;    //Liczby od 0 do 10 oznaczaj� wielokrotno�� liczby pocz�tkowej...
unsigned DefMaxLiczbaLinkow=-1;   //Wi�ksze s� brane literalnie. -1 dla link�w oznacza graf pe�ny
unsigned DefMaxLiczbaKomunikatow=10*1024; //Ile mo�e jednocze�nie istnie� komunikat�w w systemie

bool czytaj_parametry(int argc,const char* argv[])
{
    for(int i=1;i<argc;i++)
    {
        if(argv[i][0]=='-') //Parametr symshell'a do pomini�cia
            continue;
        if(strchr(argv[i],'=')!=NULL) //jest =
        {
            if(!konfiguruj(argv[i]))
            {
                cerr<<endl<<"!!! "<<_LPL("Niepoprawny parametr konfiguracyjny",
                            "Invalid parameter of configuration")<<": "<<argv[i]<<endl;
                return false;
            }
        }
        else  //Ostatni taki na liscie parametr�w jest wa�ny!
        {
            strncpy(PlikWejsciowy,argv[i],sizeof(PlikWejsciowy)-1);
            cout<<endl<<_LPL("Plik wejsciowy ustalony na",
                             "Input file name set to")<<": "<<PlikWejsciowy<<endl;
        }
    }

    if(*PlikWejsciowy=='\0') //Nie ma nazwy
        if(!podaj_nazwe_pliku(PlikWejsciowy,sizeof(PlikWejsciowy)-1,"Podaj nazwe pliku danych") )
        {
            cerr<<endl<<"Problem z nazwa pliku danych"<<endl;
            return false;
        }

    return true; //Na razie wszystko OK
}

bool konfiguruj(const char* parametr,const char* wartosc/*=NULL*/)
//Przetwarza pojedynczy "configpar" o postaci XXX = YYY lub XXX i YYY jako drugi
{
   wbrtm::wb_pchar Pom1,Pom2;

   if(wartosc!=NULL)
   {
       Pom1=parametr; Pom2=wartosc;
   }
   else
   {
       const char* Poz=strchr(parametr,'=');
       if(Poz==NULL)
        { cerr<<endl<<"!!! "<<_LPL("Oczekiwany znak =","Expected character =")<<endl; return false; }//Niepoprawne ca�kiem
       unsigned siz=Poz-parametr;
       Pom1.alloc(siz+1);
       strncpy(Pom1.get_ptr_val(),parametr,siz);
       Pom1[siz]='\0';
       Pom2=(Poz+1);
   }

   //Parametry

   if(strcmp("GENERIC.LEAKS",Pom1.get())==0)
   {
       GENERYCZNY_POZIOM_PRZECIEKOW=atof(Pom2.get());
       cout<<_LPL("Parametr","Parameter")<<" "<<Pom1.get()<<"="<<GENERYCZNY_POZIOM_PRZECIEKOW<<endl;
   }
   else
   if(strcmp("LOG.LEVEL",Pom1.get())==0)
   {
    //extern    int    wbrtm::wb_smartlog::log_level;//=0;   POZIOM LOGOWANIA PLIKU LOG U�YWANEGO W SPS
       wbrtm::wb_smartlog::log_level=atoi(Pom2.get());
       cout<<_LPL("Parametr","Parameter")<<" "<<Pom1.get()<<"="<<wbrtm::wb_smartlog::log_level<<" "<<endl;
   }
   else
   if(strcmp("TOLERANT",Pom1.get())==0)
   {
       Swiat::ForceTolerant=atoi(Pom2.get());
       cout<<_LPL("Parametr","Parameter")<<" "<<Pom1.get()<<"="<<Swiat::ForceTolerant<<" "<<(Swiat::ForceTolerant?"Y E S ! ! !":"NO")<<endl;
   }
   else
   //Parametry wizualizacyjne i logowania danych symulacji
   if(strcmp("VERBOSE",Pom1.get())==0)
   {
       VerboseInput=atoi(Pom2.get());
       cout<<_LPL("Parametr","Parameter")<<" "<<Pom1.get()<<"="<<VerboseInput<<endl;
   }
   else
   if(strcmp("VERTICAL",Pom1.get())==0)
   {
       ver=atoi(Pom2.get());
       cout<<_LPL("Parametr","Parameter")<<" "<<Pom1.get()<<"="<<ver<<endl;
   }
   else
   if(strcmp("HORIZONTAL",Pom1.get())==0)
   {
       hor=atoi(Pom2.get());
       cout<<_LPL("Parametr","Parameter")<<" "<<Pom1.get()<<"="<<hor<<endl;
   }
   else
   if(strcmp("RADIUS",Pom1.get())==0)
   {
       JakieRwProcWidth=atof(Pom2.get());
       cout<<_LPL("Parametr","Parameter")<<" "<<Pom1.get()<<"="<<JakieRwProcWidth<<endl;
   }
   else
   if(strcmp("LANGUAGE",Pom1.get())==0 || strcmp("LANG",Pom1.get())==0)
   {
       wbrtm::_lingo_selector=atoi(Pom2.get());
       cout<<_LPL("Jezyk ustawiony na \"","Language is set to \"")<<_LPL("pl","en")<<'"'<<endl;
   }
   else
   if(strcmp("MAXMESSGAGE",Pom1.get())==0)
   {
       DefMaxLiczbaKomunikatow=atoi(Pom2.get());
       cout<<_LPL("Parametr","Parameter")<<" "<<Pom1.get()<<"="<<DefMaxLiczbaKomunikatow<<endl;
       Swiat::_UstalLiczbeInfo(DefMaxLiczbaKomunikatow);
   }
   else
   {
     cerr<<endl<<"!!! "<<_LPL("Nieznany parametr konfiguracyjny",
                "An unknown parameter of configuration")<<": "<<Pom1.get()<<"="<<endl;
     return false;
   }

   return true;
}


/* *******************************************************************/
/*                     OPI  version 2011                             */
/* *******************************************************************/
/*            THIS CODE IS DESIGNED & COPYRIGHT  BY:                 */
/*             W O J C I E C H   B O R K O W S K I                   */
/*     Instytut Studi�w Spo�ecznych Uniwersytetu Warszawskiego       */
/*         WWW:  http://wwww.iss.uw.edu.pl/borkowski/                */
/*                                                                   */
/*                                (Don't change or remove this note) */
/* *******************************************************************/
