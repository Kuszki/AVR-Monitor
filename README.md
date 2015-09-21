# AVR-Monitor

Zbiór programów i bibliotek oraz projekt urządzenia elektronicznego do badania analogowych sygnałów wolnozmiennych i sterowania urządzeń sygnałami cyfrowymi.

## Programy

### Interfejs

AVR-Interface umożliwia obsługę urządzenia przy pomocy graficznego interfejsu użytkownika. Program pozwala na monitorowanie przebiegów w czasie rzeczywistym i archiwizowanie danych. Ponadto program oferuje możliwość konfiguracji i programowania urządzenia AVR-Monitor.

### Terminal

AVR-Terminal umożliwia komunikacje z urządzeniem AVR-Monitor przy użyciu klasycznego terminalu. Jest to opcja minimalistyczna pozwalająca na zaprogramowanie i konfiguracje urządzenia oraz pozwalająca przetestować większość jego funkcji.

### Biblioteka "Bindings"

AVR-Bindings pozwala na użycie klasy ułatwiającej komunikacje z urządzeniem i nadzoruje jego pracę po podłączeniu.

### Program kontrolera

Stanowi wsad do procesora AVR użytego w projekci układu i płytki PCB.

## Urządzenia

Projekt zawiera urządzenie mikroprocesorowe oparte o mikrokontroler AVR. Na kompletne urządzenie składają się 3 schematy elektryczne - płyty głównej, front-panelu oraz płyty wierzchniej.

## Zależności

- środowisko Qt
- biblioteka KLLibs
- biblioteka QCustomPlot
- biblioteka boost

## Licencja

AVR-Monitor - Zbiór programów i bibliotek oraz projekt urządzenia elektronicznego do badania analogowych sygnałów wolnozmiennych i sterowania urządzeń sygnałami cyfrowymi. Copyright (C) 2015 Łukasz "Kuszki" Dróżdż.

Niniejszy program jest wolnym oprogramowaniem – możesz go rozpowszechniać dalej i/lub modyfikować na warunkach Powszechnej Licencji Publicznej GNU wydanej przez Fundację Wolnego Oprogramowania, według wersji 3 tej Licencji lub dowolnej z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on użyteczny – jednak BEZ ŻADNEJ GWARANCJI, nawet domyślnej gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH ZASTOSOWAŃ. Bliższe informacje na ten temat można uzyskać z Powszechnej Licencji Publicznej GNU.

Kopia Powszechnej Licencji Publicznej GNU powinna zostać ci dostarczona razem z tym programem. Jeżeli nie została dostarczona, odwiedź http://www.gnu.org/licenses/.
