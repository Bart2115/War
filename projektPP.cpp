#define _CRT_SECURE_NO_WARNINGS
#define CZERWONY_KOLOR "\x1b[31m\x1b[47m"
#define CZARNY_KOLOR "\x1b[30m\x1b[47m"
#define ZIELONY_KOLOR "\x1b[0m\x1b[92m"
#define BIALY_KOLOR "\x1b[37m"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

using namespace std;

typedef struct {
	char kolor[6];
	int wartosc;
}karty_t;

typedef struct lista {
	karty_t dane;
	struct lista* next;
}lista_t;

typedef struct {
	int ilosc_el;
	lista_t* glowa;
	lista_t* ogon;
}kolejka_t;

void inicjuj_kolejke(kolejka_t* kolejka);
int czy_pusta(kolejka_t* kolejka);
void add(kolejka_t* kolejka, karty_t karta);
karty_t pop(kolejka_t* kolejka);
void kolor(karty_t karta);
void resetuj_kolor();
void wyswietl_kolejke(lista_t* ogon);
int wartosc_reki(lista_t* ogon);
int wartosc_reki2(lista_t* ogon);
void usun_kolejke(lista_t* ogon);
void wypelnij_talie(karty_t* talia, const char* kolor, int rozmiar_talii, int* i, int* j);
karty_t* generuj_talie(int rozmiar_talii);
void tasuj_talie(karty_t* talia, int rozmiar_talii);
void losuj_do_reki(kolejka_t* reka_A, kolejka_t* reka_B, karty_t* talia, int rozmiar_talii);
char przypisz_litere(const karty_t karta);
int sprawdz_kto_wygral(kolejka_t* reka_A, kolejka_t* reka_B);
karty_t* stworz_stosy_kart();
void wypisz_zakryta_karte();
void przypisz_znak_karty(const karty_t karta, char* cel);
int wojnaA(kolejka_t* reka_A, kolejka_t* reka_B, int i, int kto_dolozyl, karty_t* stos_kart_A, karty_t* stos_kart_B, int* ilosc_ruchow, const char wariant_gry);
int wojnaB(kolejka_t* reka_A, kolejka_t* reka_B, int i, int kto_dolozyl, karty_t* stos_kart_A, karty_t* stos_kart_B, int* ilosc_ruchow, const char wariant_gry);
int bitwa(kolejka_t* reka_A, kolejka_t* reka_B, karty_t* stos_kart_A, karty_t* stos_kart_B, int i, int kto_dolozyl, int* ilosc_ruchow, const char wariant_gry);
void domyslne_parametry(char* wariant_gry, int* rozmiar_talii);
void gra(int* ilosc_ruchow, int* wygranaA, int* wygranaB, int* remis, int* inne, int* rozmiar_talii, char* wariant_gry);
void ranga_reki(kolejka_t* reka_A, kolejka_t* reka_B);


int main()
{
	resetuj_kolor();
	//zmienne do zliczenia statystyk oraz uruchomienia gry
	int rozmiar_talii = 0, cala_liczba_ruchow = 0, wygranaA = 0, wygranaB = 0, remis = 0, inne = 0;
	//zmienna określająca ilość gier
	int i = 1;
	char wariant_gry;
	cout << "Podaj wariant gry A lub B\n";
	cin >> wariant_gry;
	cout << "Podaj rozmiar talii(od 20 do 52, podzielna przez 4)\n";
	cin >> rozmiar_talii;
	srand(time(NULL));
	while (i--) {
		//zmienna do liczenia ruchow w jednej grze
		int liczba_ruchow = 0;
		gra(&liczba_ruchow, &wygranaA, &wygranaB, &remis, &inne, &rozmiar_talii, &wariant_gry);
		//zmienna do liczenia ruchow we wszystkich grach
		cala_liczba_ruchow += liczba_ruchow;
	}
	cout << "\nIlosc ruchow w grze: " << cala_liczba_ruchow << " " << wygranaA << " " << wygranaB << " " << remis << " " << inne;
	printf(BIALY_KOLOR);
	return 0;
}
//funkcja przypisuje wartosci poczatkowe dla kolejki
void inicjuj_kolejke(kolejka_t* kolejka) {
	kolejka->glowa = NULL;
	kolejka->ogon = NULL;
	kolejka->ilosc_el = 0;
}
//funkcja sprawdza czy kolejka jest pusta
int czy_pusta(kolejka_t* kolejka) {
	return (kolejka->ogon == NULL);
}
//funkcja dodaje element na koniec kolejki
void add(kolejka_t* kolejka, karty_t karta) {
	if (karta.wartosc != 0) {
		lista_t* a = NULL;
		a = (lista_t*)malloc(sizeof(lista_t));
		a->dane = karta;
		a->next = NULL;
		//jesli kolejka jest pusta
		if (czy_pusta(kolejka)) {
			kolejka->ogon = a;
			kolejka->glowa = a;
			kolejka->ilosc_el += 1;
		}
		else {
			a->next = kolejka->ogon;
			kolejka->ogon = a;
			kolejka->ilosc_el += 1;
		}
	}
}
//funkcja usuwa element z poczatku kolejki i zwraca usunieta karte
karty_t pop(kolejka_t* kolejka) {
	karty_t karta = { "",0 };
	if (!czy_pusta(kolejka)) {
		lista_t* a, * b;
		a = kolejka->ogon;
		b = kolejka->glowa;
		//JAK NIE MA KART
		if (a == NULL) {
			free(b);
			return karta;
		}
		strcpy(karta.kolor, b->dane.kolor);
		karta.wartosc = b->dane.wartosc;
		//JAK JEST JEDNA KARTA
		if (a == b) {
			kolejka->ogon = kolejka->glowa = NULL;
			kolejka->ilosc_el = 0;
			free(b);
			return karta;
		}
		//JAK JEST WIECEJ NIZ JEDNA KARTA
		while (a->next != b) {
			a = a->next;
		}
		kolejka->ilosc_el -= 1;
		kolejka->glowa = a;
		free(b);
		a->next = NULL;
		return karta;
	}
	return karta;
}
//funkcja przypisuje kolor czerwony lub czarny w zaleznosci od rodzaju karty
void kolor(karty_t karta) {
	if (!strcmp(karta.kolor, "TREFL") || !strcmp(karta.kolor, "PIK")) {
		printf(CZARNY_KOLOR);
	}
	else if (!strcmp(karta.kolor, "KARO") || !strcmp(karta.kolor, "KIER")) {
		printf(CZERWONY_KOLOR);
	}
}
//funkcja zmienia kolor tekstu na zielony
void resetuj_kolor() {
	printf(ZIELONY_KOLOR);
}
//funkcja wyswietla kolejke, jako argument pobiera wskaznik na ostatni element
void wyswietl_kolejke(lista_t* ogon) {
	if (ogon != NULL) {
		char znak[3];
		przypisz_znak_karty(ogon->dane, znak);
		kolor(ogon->dane);
		printf("[%5s,%2s]", ogon->dane.kolor, znak);
		resetuj_kolor();
		printf(" ");
		wyswietl_kolejke(ogon->next);
	}
}
//funkcja usuwa zawartosc kolejki
void usun_kolejke(lista_t* ogon) {
	if (ogon != NULL) {
		free(ogon->next);
	}
	free(ogon);
}

//funkcja wypelnia talie kartami, argument i to liczba wskazujaca na indeks tablicy, argument j to liczba dla ktorego koloru ma dziac sie przypisanie
void wypelnij_talie(karty_t* talia, const char* kolor, int rozmiar_talii, int* i, int* j) {
	++(*j);
	//k to zmienna o wartosci karty
	int k = 13 - (rozmiar_talii / 4);
	for (; *i < *j * (rozmiar_talii / 4); ++k, ++(*i)) {
		talia[(*i)].wartosc = 2 + k;
		strcpy(talia[(*i)].kolor, kolor);
	}
}
//funkcja tworzy niepotasowana talie kart
karty_t* generuj_talie(int rozmiar_talii) {
	int i = 0, j = 0;
	karty_t* talia;
	talia = (karty_t*)malloc(sizeof(karty_t) * rozmiar_talii);
	wypelnij_talie(talia, "PIK", rozmiar_talii, &i, &j);
	wypelnij_talie(talia, "TREFL", rozmiar_talii, &i, &j);
	wypelnij_talie(talia, "KIER", rozmiar_talii, &i, &j);
	wypelnij_talie(talia, "KARO", rozmiar_talii, &i, &j);
	return talia;
}
//funkcja do tasowania kart
void tasuj_talie(karty_t* talia, int rozmiar_talii) {
	karty_t* kopia_talii;
	int x;
	kopia_talii = (karty_t*)malloc(rozmiar_talii * sizeof(karty_t));
	for (int i = 0; i < rozmiar_talii; ++i) {
		x = rand() % rozmiar_talii;
		while (talia[x].wartosc == 0) {
			x = rand() % rozmiar_talii;
		}
		kopia_talii[i] = talia[x];
		talia[x].wartosc = 0;
	}
	for (int i = 0; i < rozmiar_talii; ++i) {
		talia[i].wartosc = kopia_talii[i].wartosc;
		strcpy(talia[i].kolor, kopia_talii[i].kolor);
	}
	free(kopia_talii);
}
//funkcja przypisania kart z talii do rąk graczy
void losuj_do_reki(kolejka_t* reka_A, kolejka_t* reka_B, karty_t* talia, int rozmiar_talii) {
	for (int i = 0; i < (rozmiar_talii / 2); ++i) {
		add(reka_A, talia[i]);
	}
	for (int i = rozmiar_talii - 1; i > (rozmiar_talii / 2) - 1; --i) {
		add(reka_B, talia[i]);
	}
}
//funkcja zwraca symbol karty powyżej numeru 10
char przypisz_litere(const karty_t karta) {
	switch (karta.wartosc) {
	case 11: {
		return 'J';
	}
	case 12: {
		return 'Q';
	}
	case 13: {
		return 'K';
	}
	case 14: {
		return 'A';
	}
	}
	return ' ';
}
//funkcja sprawdza czy graczom skończyły się karty i określa wygranego, zwraca '-1' gdy gracze mogą kontynuować grę
int sprawdz_kto_wygral(kolejka_t* reka_A, kolejka_t* reka_B) {
	if (reka_A->ilosc_el == 0 && reka_B->ilosc_el == 0) {
		cout << "Remis.";
		return 0;
	}
	else if (reka_A->ilosc_el < 1) {
		cout << "Gre wygral gracz B.";
		return 2;
	}
	else if (reka_B->ilosc_el < 1) {
		cout << "Gre wygral gracz A.";
		return 1;
	}
	return -1;
}
//funkcja tworzy i zwraca stos kart
karty_t* stworz_stosy_kart() {
	karty_t* stos_kart;
	int ilosc_kart = 30;
	stos_kart = (karty_t*)malloc(sizeof(karty_t) * ilosc_kart);
	return stos_kart;
}
//funkcja wypisuje odwrócone karty
void wypisz_zakryta_karte() {
	printf(CZARNY_KOLOR "[XXXXX,XX]\t[XXXXX,XX]\n");
	resetuj_kolor();
}
//funkcja przypisuje argumentowi 'cel' symbol argumentu 'karta'
void przypisz_znak_karty(const karty_t karta, char* cel) {
	if (karta.wartosc > 10) {
		*cel = przypisz_litere(karta);
		cel[1] = '\0';
	}
	else {
		sprintf(cel, "%d", karta.wartosc);
	}
}
//funkcja oblicza wartosc kart wedlug systemu rankingowego zliczajacego wartosc kazdej karty
int wartosc_reki(lista_t* ogon) {
	int suma = 0;
	if (ogon != NULL) {
		suma += ogon->dane.wartosc;
		suma += wartosc_reki(ogon->next);
	}
	return suma;
}
//funkcja oblicza wartosc kart wedlug systemu rankingowego zliczajacego wartosc krolow i asow
int wartosc_reki2(lista_t* ogon) {
	int suma = 0;
	if (ogon != NULL) {
		int x = 0;
		x = ogon->dane.wartosc;
		if(x > 12){
			suma += x;
		}
		suma += wartosc_reki2(ogon->next);
	}
	return suma;
}
//funkcja zwraca liczbę określającą czy gra dobiegła końca, '-1' to brak rozstrzygnięcia, '0' to remis, '1' to wygrana gracza A, '2' wygrana gracza B, '3' zapętlenie gry, argument 'i' to liczba wskazujaca na indeks tablicy stosu
int bitwa(kolejka_t* reka_A, kolejka_t* reka_B, karty_t* stos_kart_A, karty_t* stos_kart_B, int i, int kto_dolozyl, int* ilosc_ruchow, const char wariant_gry) {
	int spr = sprawdz_kto_wygral(reka_A, reka_B);
	if (spr != -1) {
		return spr;
	}
	if (i == 0) {
		system("cls");
		//wyswietlone zostana rece graczy w celach testowych
		wyswietl_kolejke(reka_A->ogon);
		printf("\n\n");
		wyswietl_kolejke(reka_B->ogon);
		printf("\n\n");
		cout << "Gracz A:\t" << "Gracz B:\n";
	}
	//warunek do przerwania gry jeśli się zapętli
	else if (*ilosc_ruchow > 3000) {
		*ilosc_ruchow = 0;
		return 3;
	}
	karty_t karta_A, karta_B;
	karta_A = pop(reka_A);
	karta_B = pop(reka_B);
	stos_kart_A[i] = karta_A;
	stos_kart_B[i] = karta_B;
	++i;
	++(*ilosc_ruchow);
	//zmienne przechowujace wartosc karty jako symbol
	char a[3], b[3];
	przypisz_znak_karty(karta_A, a);
	przypisz_znak_karty(karta_B, b);
	//wygrana bitwy gracza A
	if (karta_A.wartosc > karta_B.wartosc) {
		kolor(karta_A);
		printf("[%5s,%2s]\t", karta_A.kolor, a);
		kolor(karta_B);
		printf("[%5s,%2s]\n", karta_B.kolor, b);
		resetuj_kolor();
		cout << "Bitwe wygrywa gracz A\n\n";
		system("PAUSE");
		for (int j = 0; j < i; ++j) {
			add(reka_A, stos_kart_A[j]);
		}
		for (int j = 0; j < i; ++j) {
			add(reka_A, stos_kart_B[j]);
		}
		return sprawdz_kto_wygral(reka_A, reka_B);
	}
	//wygrana bitwy gracza B
	else if (karta_A.wartosc < karta_B.wartosc) {
		kolor(karta_A);
		printf("[%5s,%2s]\t", karta_A.kolor, a);
		kolor(karta_B);
		printf("[%5s,%2s]\n", karta_B.kolor, b);
		resetuj_kolor();
		cout << "Bitwe wygrywa gracz B\n\n";
		system("PAUSE");
		for (int j = 0; j < i; ++j) {
			add(reka_B, stos_kart_B[j]);
		}
		for (int j = 0; j < i; ++j) {
			add(reka_B, stos_kart_A[j]);
		}
		return sprawdz_kto_wygral(reka_A, reka_B);
	}
	//remis, a wiec wojna
	else if (karta_A.wartosc == karta_B.wartosc) {
		kolor(karta_A);
		printf("[%5s,%2s]\t", karta_A.kolor, a);
		kolor(karta_B);
		printf("[%5s,%2s]\n", karta_B.kolor, b);
		resetuj_kolor();
		if (wariant_gry == 'A') {
			return wojnaA(reka_A, reka_B, i, kto_dolozyl, stos_kart_A, stos_kart_B, ilosc_ruchow, wariant_gry);
		}
		else if (wariant_gry == 'B') {
			//jesli obu graczom skończą się karty
			if (reka_A->ilosc_el == 0 && reka_B->ilosc_el == 0) {
				if (kto_dolozyl == 0) {
					cout << "\nRemis";
					return 0;
				}
				else if (kto_dolozyl == 1) {
					cout << "\nGre wygrywa gracz A";
					return 1;
				}
				else if (kto_dolozyl == 2) {
					cout << "\nGre wygrywa gracz B";
					return 2;
				}
				return -1;
			}
			//jeśli jeden z graczy ma mniej niż 3 karty potrzebne do wojny
			else if (reka_A->ilosc_el < 2 || reka_B->ilosc_el < 2) {
				if (kto_dolozyl == 0) {
					//przypadek gdy oboje z graczy mają mniej niż 3 karty
					if ((reka_A->ilosc_el + reka_B->ilosc_el) < 4) {
						if (reka_A->ilosc_el > reka_B->ilosc_el) {
							return 1;
						}
						else if (reka_A->ilosc_el < reka_B->ilosc_el) {
							return 2;
						}
						return 0;
					}
					//przypadek gdy mozliwe jest dolozenie kart przez jednego z graczy
					return wojnaB(reka_A, reka_B, i, kto_dolozyl, stos_kart_A, stos_kart_B, ilosc_ruchow, wariant_gry);
				}
				//przypadek gdy gracz A dokladal juz raz karty
				else if (kto_dolozyl == 1) {
					cout << "\nGre wygrywa gracz A";
					return 1;
				}
				//przypadek gdy gracz B dokladal juz raz karty
				else if (kto_dolozyl == 2) {
					cout << "\nGre wygrywa gracz B";
					return 2;
				}
			}
			//przypadek gdy jest mozliwa normalna wojna
			else {
				return wojnaA(reka_A, reka_B, i, kto_dolozyl, stos_kart_A, stos_kart_B, ilosc_ruchow, wariant_gry);
			}
		}
		return -1;
	}
	return -1;
}
//funkcja do wojny w wariancie 'A', zwraca wynik ostatniej bitwy, gdy wojna jest niemożliwa do przeprowadzenia z powodu braku kart wyświetli się komunikat, argument 'i' to liczba wskazujaca na indeks tablicy stosu 
int wojnaA(kolejka_t* reka_A, kolejka_t* reka_B, int i, int kto_dolozyl, karty_t* stos_kart_A, karty_t* stos_kart_B, int* ilosc_ruchow, const char wariant_gry) {
	karty_t karta_A, karta_B;
	karta_A = pop(reka_A);
	stos_kart_A[i] = karta_A;
	karta_B = pop(reka_B);
	stos_kart_B[i] = karta_B;
	++i;
	++(*ilosc_ruchow);
	if (!(reka_A->ilosc_el < 1 || reka_B->ilosc_el < 1)) {
		wypisz_zakryta_karte();
	}
	else {
		printf("Graczowi skonczyly sie karty!\n");
	}
	return bitwa(reka_A, reka_B, stos_kart_A, stos_kart_B, i, kto_dolozyl, ilosc_ruchow, wariant_gry);
}
//funkcja do wojny w wariancie 'B', zwraca wynik ostatniej bitwy, argument 'i' to liczba wskazujaca na indeks tablicy stosu, argument 'kto_dolozyl' wskazuje na gracza, który dołożył drugiemu graczowi karty ze swojego stosu 
int wojnaB(kolejka_t* reka_A, kolejka_t* reka_B, int i, int kto_dolozyl, karty_t* stos_kart_A, karty_t* stos_kart_B, int* ilosc_ruchow, const char wariant_gry) {
	karty_t karta_A, karta_B;
	//jesli gracz B mial przed wojna 2 karty
	if (reka_B->ilosc_el == 1) {
		karta_B = pop(reka_B);
		karta_A = pop(reka_A);
		stos_kart_B[i] = karta_B;
		stos_kart_A[i] = karta_A;
		++i;
		++(*ilosc_ruchow);
		karta_A = pop(reka_A);
		add(reka_B, karta_A);
		wypisz_zakryta_karte();
		kto_dolozyl = 1;
		return bitwa(reka_A, reka_B, stos_kart_A, stos_kart_B, i, kto_dolozyl, ilosc_ruchow, wariant_gry);
	}
	//jesli graczA mial przed wojna 2 karty
	else if (reka_A->ilosc_el == 1) {
		karta_B = pop(reka_B);
		karta_A = pop(reka_A);
		stos_kart_B[i] = karta_B;
		stos_kart_A[i] = karta_A;
		++i;
		++(*ilosc_ruchow);
		karta_B = pop(reka_B);
		add(reka_A, karta_B);
		wypisz_zakryta_karte();
		kto_dolozyl = 2;
		return bitwa(reka_A, reka_B, stos_kart_A, stos_kart_B, i, kto_dolozyl, ilosc_ruchow, wariant_gry);
	}
	//jesli graczB miał przed wojną 1 kartę
	else if (reka_B->ilosc_el == 0) {
		karta_A = pop(reka_A);
		stos_kart_B[i] = karta_A;
		karta_A = pop(reka_A);
		add(reka_B, karta_A);
		karta_A = pop(reka_A);
		stos_kart_A[i] = karta_A;
		++i;
		++(*ilosc_ruchow);
		wypisz_zakryta_karte();
		kto_dolozyl = 1;
		return bitwa(reka_A, reka_B, stos_kart_A, stos_kart_B, i, kto_dolozyl, ilosc_ruchow, wariant_gry);
	}
	//jesli graczA miał przed wojną 1 kartę
	else if (reka_A->ilosc_el == 0) {
		karta_B = pop(reka_B);
		stos_kart_A[i] = karta_B;
		karta_B = pop(reka_B);
		add(reka_A, karta_B);
		karta_B = pop(reka_B);
		stos_kart_B[i] = karta_B;
		++i;
		++(*ilosc_ruchow);
		wypisz_zakryta_karte();
		kto_dolozyl = 2;
		return bitwa(reka_A, reka_B, stos_kart_A, stos_kart_B, i, kto_dolozyl, ilosc_ruchow, wariant_gry);
	}
	kto_dolozyl = 0;
	return -1;
}
//funkcja wprowadza domyslne ustawienia gry w przypadku blednego podania ich na starcie, docelowo wariant gry = 'A' , rozmiar talii = '52'
void domyslne_parametry(char* wariant_gry, int* rozmiar_talii) {
	if (*wariant_gry != 'A') {
		if (*wariant_gry != 'B') {
			*wariant_gry = 'A';
		}
	}
	if (*rozmiar_talii % 4 != 0 || *rozmiar_talii < 20 || *rozmiar_talii > 52) {
		*rozmiar_talii = 52;
	}
}
//funkcja na podstawie podanego w argumentach wariantu gry i rozmiaru talii tworzy zmienne potrzebne do rozgrywki a nastepnie przeprowadza jedną grę karcianą w wojnę
void gra(int* ilosc_ruchow, int* wygranaA, int* wygranaB, int* remis, int* inne, int* rozmiar_talii, char* wariant_gry) {
	int i = 0;
	domyslne_parametry(wariant_gry, rozmiar_talii);
	//ręce graczy
	kolejka_t* reka_A, * reka_B;
	reka_A = (kolejka_t*)malloc(sizeof(kolejka_t));
	reka_B = (kolejka_t*)malloc(sizeof(kolejka_t));
	inicjuj_kolejke(reka_A);
	inicjuj_kolejke(reka_B);
	karty_t* talia = generuj_talie(*rozmiar_talii);
	tasuj_talie(talia, *rozmiar_talii);
	losuj_do_reki(reka_A, reka_B, talia, *rozmiar_talii);
	//ranga_reki(reka_A, reka_B);
	//system("PAUSE");
	/*printf("\n\n");
	wyswietl_kolejke(reka_A->ogon);
	printf("\n\n");
	wyswietl_kolejke(reka_B->ogon);
	printf("\n\n");*/

	//petla przerwie sie gdy dojdzie do końca gry 
	while (1) {
		karty_t* stosA, * stosB;
		stosA = stworz_stosy_kart();
		stosB = stworz_stosy_kart();
		int spr = bitwa(reka_A, reka_B, stosA, stosB, i, 0, ilosc_ruchow, *wariant_gry);
		if (spr != -1) {
			if (spr == 0) {
				(*remis)++;
			}
			else if (spr == 1) {
				(*wygranaA)++;
			}
			else if (spr == 2) {
				(*wygranaB)++;
			}
			else {
				(*inne)++;
			}
			/*
			FILE* plik;
			plik = fopen("ranga.txt", "a");
			fprintf(plik, "%d\n",spr);
			fclose(plik);*/
			free(stosA);
			free(stosB);
			break;
		}
		free(stosA);
		free(stosB);
	};
	free(talia);
	usun_kolejke(reka_A->ogon);
	usun_kolejke(reka_B->ogon);
	free(reka_A);
	free(reka_B);
}
//funkcja oblicza range rąk graczy
void ranga_reki(kolejka_t* reka_A, kolejka_t* reka_B) {
	int suma1, suma2;
	suma1 = wartosc_reki2(reka_A->ogon);
	suma2 = wartosc_reki2(reka_B->ogon);
	/*FILE* plik;
	plik = fopen("ranga.txt", "a");
	fprintf(plik, "%d, %d, ",	suma1, suma2);
	fclose(plik);*/
}