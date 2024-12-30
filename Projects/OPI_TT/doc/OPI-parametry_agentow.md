1. Jednostka Naukowa,

* **IleEtatow** (unsigned) – parametr określa jaką ilością osobomiesięcy dysponuje agent. Jest to związane z ilością działań jakie agent może podjąć w każdym kroku.  
* **Efektywnosc** (float) – parametr mówiący o wydajności pracy. Zakres parametru definiuje 0-brak działań i 2- działanie z wydajnością 200% „normy”. Ten parametr mówi o motywacji agenta do działania.  
* **Proaktywnosc** (float) – parametr określający aktywność w zakresie linków społecznych agenta. Zakres parametru: 0-brak proaktywności, brak działań związanych z wysyłaniem informacji do innych, tworzeniem nowych linków; do 1- wysoka proaktyność, częste nawiązywanie nowych linków, podtrzymywanie utworzonych linków.  
* **Centralnosc** (float) – parametr określający styl podejmowania decyzji przez agenta. 0 oznacza agentów podejmujących decyzje demokratycznie (w zależności od ilości członków w zespole, gdy agentem jest zespół badawczy); 1 oznacza decyzyjność autokratyczną, zależącą od osoby kierującej zespołem.  
* **Probiznesowosc** (float) – orientacja motywacyjna związana z podejmowanymi działaniami. Parametr w zakresie od 0 do 1\. 0 oznacza orientacja na osiągnięcia naukowe, oraz prestiż naukowy, z czym się wiążą wybory typu publikacja wyników, badania podstawowe. 0.5 to sytuacja, gdy agent woli patentować, licencjonować wyniki. 1 to orientacja biznesowa, gdzie agent podejmując decyzje kieruje się korzyściami finansowymi i jest nastawiony na komercjalizację wyników oraz samodzielne wdrażanie.  
* **Prolegalnosc** (float) – parametr określający na ile agent podejmuje decyzje w oparciu o obowiązujące przepisy, w szczególności jeżeli chodzi o przepisy dotyczące własności intelektualnej (1), a na ile jest skłonny działać w „szarej strefie”, czyli współpracować z biznesem z wykluczeniem Administracji, np. Uniwersytetu, w ramach którego pracuje (0).   
* **Czasochlonnosc** (float)- parametr określający ile kroków symulacji wymaga przeciętnie wygenerowanie pojedynczego wyniku badań. Mówi o średniej długości trwania projektów realizowanych przez danego agenta.  
* **Innowacyjnosc** (float) \- Łatwość tworzenia nowych projektów bez inspiracji z zewnątrz, zaciekawienie projektami nieco odległymi od dotychczasowych zainteresowań. Parametr opisuje na ile dany agent realizuje nowatorskie projekty.  
* **UdzialWZysku** (float) \-Jaki procent zysku z innowacji pobiera agent. Parametr związany z motywacją. Zakres od 0 – 0% do 1 \-100%.  
* **FinanseSwobodne** (float) \- Rezerwy finansowe agenta w skali roku.    
* **FinanseObiecane** (float) – Finanse zaplanowane do wydania w danym roku, ale być może jeszcze nie dostępne na koncie agenta.  
* **Dlugi**(float) – Finanse zaległe, zobowiązania w stosunku do Administracji, bądź innych agentów. Możliwe jest posiadanie długów przez agenta tylko tymczasowo, do 30 kroków.   
* **Eksperckosc** (float) \- 




2. Administracja  
     
* **IleEtatow** (unsigned) – parametr określa jaką ilością osobomiesięcy dysponuje agent. Jest to związane z ilością działań jakie agent może podjąć w każdym kroku.  
* **Efektywnosc** (float) – parametr mówiący o wydajności pracy. Zakres parametru definiuje 0-brak działań i 2- działanie z wydajnością 200% „normy”. Ten parametr mówi o motywacji agenta do działania.  
* **AktywnoscKonf** (float) – parametr określający jak często (co ile kroków) agent organizuje konferencje i inne eventy umożliwiające kontakty między naukowcami(jednostkami naukowymi) i gośćmi(np. firmami). (czyli co ile kroków następuje taki event)  
* **Centralnosc** (float) – parametr określający styl podejmowania decyzji przez agenta. 0 oznacza agentów podejmujących decyzje demokratycznie (w zależności od ilości członków w zespole, gdy agentem jest zespół badawczy); 1 oznacza decyzyjność autokratyczną, zależącą od osoby kierującej zespołem,  
* **Probiznesowosc** (float) – orientacja motywacyjna związana z podejmowanymi działaniami i stosunkiem względem środowiska biznesowego. Parametr  mówi o prawdopodobieństwie podjęcia inicjatyw z współudziałem biznesu.   
* **Prolegalnosc** (float) – parametr określający na ile agent podejmuje decyzje w oparciu o obowiązujące przepisy (1), a na ile jest skłonny działać elastycznie, omijając bariery biurokratyczne (0).   
* **Czasochlonnosc** (float)- parametr określający ile kroków symulacji wymaga przeciętnie wygenerowanie pojedynczego działania. Mówi o średniej długości trwania projektów, podejmowania decyzji przez danego agenta.  
* **ProInwest** (float) – parametr opisujący skłonność inwestowania wolnych środków w rozwój związany z Transferem technologii. 0 – brak takich decyzji. 1- inwestowanie wszystkich wolnych środków w działania związane z transferem technologii.  
* **UdzialWZysku** (float) \-Jaki procent zysku z innowacji pobiera agent. Parametr związany z motywacją. Zakres od 0 – 0% do 1 \-100%. Reszta zysków jest kierowana do agentów niżej w hierarchii (do jednostki naukowej).  
* **FinanseSwobodne**(float) \- Rezerwy finansowe agenta w skali roku.    
* **FinanseObiecane** (float) – Finanse zaplanowane do wydania w danym roku, ale być może jeszcze nie dostępne na koncie agenta.  
* **Dlugi**(float) – Finanse zaległe, zobowiązania w stosunku do Administracji, bądź innych agentów. Możliwe jest posiadanie długów przez agenta tylko w granicach prawnie określonych limitów. 

3\. Firma, 

* **IleEtatow** (unsigned) – parametr określa jaką ilością osobomiesięcy dysponuje agent. Jest to związane z ilością działań jakie agent może podjąć w każdym kroku.  
* **Efektywnosc** (float) – parametr mówiący o wydajności pracy. Zakres parametru definiuje 0-brak działań i 2- działanie z wydajnością 200% „normy”. Ten parametr mówi o motywacji agenta do działania.  
* **Proaktywnosc** (float) – parametr określający aktywność w zakresie linków społecznych agenta. Zakres parametru: 0-brak proaktywności, brak działań związanych z wysyłaniem informacji do innych, tworzeniem nowych linków; do 1- wysoka proaktyność, częste nawiązywanie nowych linków, podtrzymywanie utworzonych linków.  
* **Centralnosc** (float) – parametr określający styl podejmowania decyzji przez agenta. 0 oznacza agentów podejmujących decyzje demokratycznie (w zależności od ilości członków w zespole, gdy agentem jest zespół badawczy); 1 oznacza decyzyjność autokratyczną, zależną od osoby kierującej zespołem.  
* **ProAkademickosc** (float) – orientacja motywacyjna związana ze współpracą ze środowiskiem naukowym. Zakres od 0 – brak zainteresowania kontaktami z uczelniami i naukowcami do 1 – chęć nawiązywania kontaktów z badaczami i uniwersytetami.  
* **Prolegalnosc** (float) – parametr określający „pragmatyczność” agenta. Na ile jest on skłonny stosować kreatywne rozwiązania, oraz omijać biurokratyczne bariery i działać poza prawnymi ramami współpracy a na ile ściśle się trzyma przepisów i ścieżki oficjalnej. Zakres od 0 do 1\.  
* **Czasochlonnosc** (float)- parametr określający ile kroków symulacji wymaga przeciętnie wygenerowanie pojedynczego wyniku projektu. Mówi o średniej długości trwania projektów realizowanych przez danego agenta.  
* **ProInnowac** (float) – parametr opisujący stosunek do nowości, innowacyjnych rozwiązań. Zakres od 0 do 1\. Jeśli wartość jest niska agent niewiele robi. Przy wartościach średnich agent raczej blokuje innych, a przy wysokich próbuje, wchodząc na coraz wcześniejszym etapie.  
* **FinanseBiezace** (float) – aktualny budżet agenta**.**  
* **FinanseZaplanowane** (float) – finanse zaplanowane do wydania w danym roku, ale być może jeszcze nie dostępne na koncie agenta, np. obiecane środki kredytowe, albo granty.  
* **Dlugi** (float) – Finanse zaległe, zobowiązania w stosunku do innych agentów. Możliwe jest posiadanie długów przez agenta tylko w granicach prawnie określonych limitów. Im większe długi tym mniejsza szansa na pozyskanie nowych funduszy zewnętrznych.  
*  **LimitUdzialuInwestTT** (float)- Parametr opisujący jaki procent środków finansowych z posiadanych, dany agent jest gotów przeznaczyć na inwestycje w zakresie transferu technologii.  
* **Eksperckosc** (float) \-   
  


3. BOTT

* **IleEtatow** (unsigned) – parametr określa jaką ilością osobomiesięcy dysponuje agent. Jest to związane z ilością działań jakie agent może podjąć w każdym kroku.  
* **Efektywnosc** (float) – parametr mówiący o wydajności pracy. Zakres parametru definiuje 0-brak działań i 2- działanie z wydajnością 200% „normy”. Ten parametr mówi o motywacji agenta do działania.  
* **Proaktywnosc** (float) – parametr określający aktywność w zakresie tworzenia linków przez agenta. Zakres parametru: 0-brak proaktywności, brak działań związanych z wysyłaniem informacji do innych, tworzeniem nowych linków; do 1- wysoka proaktyność, częste nawiązywanie nowych linków, podtrzymywanie utworzonych linków.  
* **Centralnosc** (float) – parametr określający styl podejmowania decyzji przez agenta. 0 oznacza agentów podejmujących decyzje demokratycznie (w zależności od ilości członków w zespole, gdy agentem jest zespół badawczy); 1 oznacza decyzyjność autokratyczną, zależącą od osoby kierującej zespołem.  
* **ProAkademickosc** (float) – orientacja motywacyjna związana ze współpracą ze środowiskiem naukowym. Zakres od 0 – brak zainteresowania kontaktami z uczelniami i naukowcami do 1 – chęć nawiązywania kontaktów z badaczami i uniwersytetami.  
* **Prolegalnosc** (float) – parametr określający „pragmatyczność” agenta. Na ile jest on skłonny stosować kreatywne rozwiązania, oraz omijać biurokratyczne bariery i działać poza prawnymi ramami współpracy a na ile ściśle się trzyma przepisów i ścieżki oficjalnej. Zakres od 0 do 1\.  
* **Czasochlonnosc** (float)- parametr określający ile kroków symulacji wymaga przeciętnie wygenerowanie pojedynczego wyniku projektu. Mówi o średniej długości trwania projektów realizowanych przez danego agenta.  
* **ProInnowac** (float) – parametr opisujący stosunek do nowości, innowacyjnych rozwiązań. Zakres od 0 do 1\. Jeśli wartość jest niska agent niewiele robi. Przy wartościach średnich agent raczej blokuje innych, a przy wysokich próbuje, wchodząc na coraz wcześniejszym etapie.  
*  **FinanseBiezace** (float) – aktualny budżet agenta**.**  
*  **FinanseZaplanowane** (float) – finanse zaplanowane do wydania w danym roku, ale być może jeszcze nie dostępne na koncie agenta, np. obiecane środki kredytowe, albo granty.  
* **Dlugi** (float) – Finanse zaległe, zobowiązania w stosunku do innych agentów. Możliwe jest posiadanie długów przez agenta tylko w granicach prawnie określonych limitów. Im większe długi tym mniejsza szansa na pozyskanie nowych funduszy zewnętrznych.  
* **LimitRyzyka** (float) \-   
* **Eksperckosc** (float) \- 


4. UOTT,

* **IleEtatow** (unsigned) – parametr określa jaką ilością osobomiesięcy dysponuje agent. Jest to związane z ilością działań jakie agent może podjąć w każdym kroku.  
* **Efektywnosc** (float) – parametr mówiący o wydajności pracy. Zakres parametru definiuje 0-brak działań i 2- działanie z wydajnością 200% „normy”. Ten parametr mówi o motywacji agenta do działania.  
* **Proaktywnosc** (float) – parametr określający aktywność w zakresie tworzenia linków przez agenta. Zakres parametru: 0-brak proaktywności, brak działań związanych z wysyłaniem informacji do innych, tworzeniem nowych linków; do 1- wysoka proaktyność, częste nawiązywanie nowych linków, podtrzymywanie utworzonych linków.  
* **Centralnosc** (float) – parametr określający styl podejmowania decyzji przez agenta. 0 oznacza agentów podejmujących decyzje demokratycznie (w zależności od ilości członków w zespole, gdy agentem jest zespół badawczy); `1` oznacza decyzyjność autokratyczną, zależną od osoby kierującej zespołem.  
* **Probiznesowosc** (float) – orientacja motywacyjna związana z podejmowanymi działaniami. Parametr w zakresie od 0 do 1\. 0 oznacza ogólne niskie zainteresowanie współpracą z biznesem. 0.3 \- 0.7  to sytuacja, gdy agent woli patentować wyniki. 1 to orientacja biznesowa, gdzie agent podejmując decyzje kieruje się korzyściami finansowymi i jest nastawiony na komercjalizację wyników oraz ich wdrażanie.  
* **Prolegalnosc** (float) – parametr określający na ile agent podejmuje decyzje w oparciu o obowiązujące przepisy, w szczególności jeżeli chodzi o przepisy dotyczące własności intelektualnej (1), a na ile jest skłonny działać w „szarej strefie”, czyli współpracować z biznesem z pominięciem uciążliwej biurokracji (0).   
* **Czasochlonnosc** (float)- parametr określający ile kroków symulacji wymaga przeciętnie wygenerowanie pojedynczego działania. Mówi o średniej długości trwania projektów realizowanych przez danego agenta.  
* **FinanseBiezace** (float) – aktualny budżet agenta**.**  
* **FinanseZaplanowane** (float) – finanse zaplanowane do wydania w danym roku, ale być może jeszcze nie dostępne na koncie agenta, np. obiecane środki kredytowe, albo granty.  
* **Dlugi** (float) – Finanse zaległe, zobowiązania w stosunku do innych agentów. Możliwe jest posiadanie długów przez agenta tylko w granicach prawnie określonych limitów. Im większe długi tym mniejsza szansa na pozyskanie nowych funduszy zewnętrznych.  
* **Eksperckość** (float) \- ???

6\. Urząd Patentowy

Brak parametrów????

* **Czasochłonność** (float)- parametr określający ile kroków symulacji wymaga przeciętnie wygenerowanie pojedynczego działania.. Mówi o średniej długości trwania projektów realizowanych przez danego agenta.  
* **Eksperckość** (float) \- ???