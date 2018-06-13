#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include<Windows.h>
#include<iostream>
#include<vector>
#include<sstream>
#include<typeinfo>
#include<time.h>



using namespace std;


class Okno_eq;
class Przeciwnik;
class Przedmiot;
class Map;



//Funkcje:

vector<SDL_Rect> ustawianie_rect_spraj(int wys, int szer, int kolumny);
int taimer_animacji(int &_t, int &_s, int liczba_klatek, int odswierzanie);
int taimer(Uint32 czas, int liczba_klatek, Uint32 odswierzanie);
string zmiana_nazwy(int nr_podziemia);
SDL_Texture* napis(int c1, int c2, int c3, TTF_Font*arial, SDL_Renderer* render, string napis);


class Gracz
{
	friend class Okno_eq;
	friend class Przeciwnik;
	friend class Przedmiot;
	friend class Map;
	friend class Pokoj1;
public:
	//1.Pozycja:
	double posX;
	double posY;
	double szerokosc;
	double wysokosc;
	double znaczX = 0;
	double znaczY = 0;

	//2.Statystyki:
	double sila;
	double zrecznosc;
	double inteligencja;
	double zycie;
	double max_zycie;
	double obrazenia;
	double ochrona;
	int przedchwila_zadane_obrazenia;
	int lvl=1;
	int exp = 0;
	int next_exp = 100;
	SDL_Rect obr;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	//3. ...
	bool tura;
	bool chodzi=false;
	bool wlaczanie_ataku = false;
	SDL_Texture *tekstura;
	SDL_Texture *chodzenie;
	vector<Przedmiot*> ekwipunek;
	vector<Przedmiot> zalozone;
	//4. Efekty pasywne:
	bool tura_wczasie_nalzoenia;
	bool podpalenie = false; int licznik_podpalenia=0;
	
	bool stun = false; int licznik_stuna=0;
	//3.Taimery:
	Uint32 animacja_ciecia = 0;
	bool b_ciecia = false;
	Uint32 animacja_miss = 0;
	bool b_miss = false;

///   METODY:
	Gracz( double, double, double, double,bool, SDL_Texture*, SDL_Texture*);
	~Gracz();
	void update(SDL_Renderer *render,int &_s,int &_t, SDL_Texture *zdrowie, TTF_Font *arial, SDL_Texture *g_obrazenia);
	void poruszanie(Okno_eq okno_eq, vector<Przeciwnik*> &przeciwniki, Mix_Chunk *m_ciecie, Map mapa);
	void koniec(vector<Przeciwnik*> &przeciwnik);
	void podnoszenie(vector<Przedmiot*> &przedmiot, SDL_Renderer* render, SDL_Texture* teks, TTF_Font* arial);
	void atak_przycisk();
	void atak(vector<Przeciwnik*> &przeciwnik, SDL_Texture *g_znacznik, SDL_Texture *g_ciecie, SDL_Renderer *render, Mix_Chunk *m_chodzenie);
	void pauza(vector<Przeciwnik*> &przeciwnik);
	bool przesuwanie_gracz(vector<Przeciwnik*> &przeciwniki, Map mapa, int a);
	void efekty_pasywne();
	void animacje_ataku(SDL_Renderer *render, SDL_Texture *tekstura_ataku, TTF_Font *arial);
	void skalowanie(double przesuniecieX, double przesuniecieY,double skala);
	bool kolizja_gracz(Map mapa, int a);
	void usuwanie_mgly(Map &mapa, SDL_Texture* g_mgla);
	void lvlup();
};

class Przeciwnik
{
public:
	
	string nazwa;

	double posX;
	double posY;
	double velY;
	double szerokosc;
	vector<SDL_Rect> spreje;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	SDL_Texture *tekstura;
	double wysokosc;
	bool aktywny;
	bool tura;
	int animacja_obrazen=0;
	//2.Statystyki:
	double sila;
	double zrecznosc;
	double inteligencja;
	double obrazenia;
	int zycie;
	int max_zycie;
	int przedchwila_zadane_obrazenia;
	int wartosc_exp = 10;
	//3.Taimery:
	Uint32 animacja_ciecia = 0; 
	bool b_ciecia = false;
	Uint32 animacja_miss = 0; 
	SDL_Rect obr;
	bool b_miss = false;
	//4.Pasywne:
	bool podpalenie = false;
	bool moznosc_podpalania = false;
	bool zamrozenie = false;

	Przeciwnik(){}
	Przeciwnik(string,double, double, double, double,double,double,double,double,SDL_Texture*, vector<SDL_Rect> spreje);
	~Przeciwnik();
	void poruszanie(SDL_Renderer *render,Gracz &gracz1,int &_s , int &_t, SDL_Texture *zdrowie, Map mapa);
	bool kolizja_przeciwnik(Map mapa, int a);
	void atak(Gracz &gracz, Mix_Chunk *m_obrazenia);
	virtual void update(SDL_Renderer *render, int &_s, int &_t,SDL_Texture *zdrowie, SDL_Texture *ciecie, TTF_Font *arial);
	void potion(char rodzaj);
	void animacje_ataku(SDL_Renderer *render, SDL_Texture *tekstura_ataku, TTF_Font *arial);
	void skalowanie(double przesuniecieX, double przesuniecieY, double skala);
	virtual string rodzaj() { return "zwykle"; }
};

#pragma region Przeciwniki
class Szczur : public Przeciwnik
{
public:
	Szczur() {}
	Szczur(SDL_Texture *przec,double px,double py,double szer)
	{
		aktywny = true;
		nazwa = "Szczur";
		
		szerokosc = szer;
		wysokosc = szer;
		posX = px;
		posY = py;
		tekstura = przec;

		zrecznosc = 10;
		inteligencja = 0;
		spreje = ustawianie_rect_spraj( 60,  60, 3);
		obrazenia = 1;
		zycie = 3;
		max_zycie = 3;
		wartosc_exp = 10;
		moznosc_podpalania = false;
	}
	void update(SDL_Renderer *render, int &_s, int &_t, SDL_Texture *zdrowie, SDL_Texture *ciecie, TTF_Font *arial);
	string rodzaj() { return "Szczur"; }
};
class Ogien : public Przeciwnik
{
public:
	Ogien() {}
	Ogien(SDL_Texture *przec, double px, double py, double szer)
	{
		aktywny = true;
		nazwa = "Zywiolak Ognia";
		
		szerokosc = szer;
		wysokosc = szer;
		posX = px;
		posY = py;
		tekstura = przec;

		zrecznosc = 30;
		inteligencja = 0;
		spreje = ustawianie_rect_spraj(100, 100, 2);
		obrazenia = 3;
		zycie = 6;
		max_zycie = 6;
		wartosc_exp = 25;
		moznosc_podpalania = true;
	}
	void update(SDL_Renderer *render, int &_s, int &_t, SDL_Texture *zdrowie, SDL_Texture *ciecie, TTF_Font *arial);
	string rodzaj() { return "Ogien"; }
};

#pragma endregion

class Przedmiot
{
	friend class Gracz;
	friend class Przedmiot;
	friend class Mapa;
	friend class Pokoj1;
public: 

	string nazwa;
//1.Pozycja:
	double posX;
	double posY;
	double velY;
	double szerokosc;
	double wysokosc;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
//2.Statystyki:	
	double sila;
	double zrecznosc;
	double inteligencja;
	double obrazenia;
	double ochrona;
	double wymagana_sila;
//3.Rodzaj:
	int amunicja;
	char rodzaj;
	bool zucane;
	
//3. ...
	SDL_Texture *tekstura;
	SDL_Texture *tekstura_r;
	bool aktywny=true;
///METODY:
	Przedmiot(string , double, double, double, double,double, double,SDL_Texture*,SDL_Texture*,bool,char);
	Przedmiot();
	~Przedmiot();
	virtual bool uzycie(Gracz &gracz) { return false; }
	virtual string rodzaj_kalsy() { return "zwykly"; }
	virtual void update( SDL_Renderer *render);
	virtual void okno_informacji(SDL_Renderer* render, SDL_Texture *tekstura, TTF_Font *font, double px, double py);
	void skalowanie(double przesuniecieX, double przesuniecieY, double skala);
};

class Potion : public Przedmiot {

public:
	
	Potion(){}
	Potion(string, double, double, double, double, SDL_Texture*, bool, char,int,double);
	bool uzycie(Gracz &gracz);
	string rodzaj_kalsy() { return "Potion"; }
	void update(SDL_Renderer *render);
};


class Okno_eq
{
	friend class Przedmiot;
	friend class Gracz;

public:
	double posX;
	double posY;
	double velY;
	double szerokosc;
	double wysokosc;
	bool prz_eq;
	double znacznikX=317;
	double znacznikY = 59;
	double szer_znacz = 100;
	double polozenie_zutuX, polozenie_zutuY;
	int prz_lotu;
	bool prz_zutu=false;
	Przedmiot *zutka;
	SDL_Texture *g_statystyki;

	Okno_eq(double = 100, double = 100, double = 90, double = 90, bool=false, SDL_Texture* =NULL);
	~Okno_eq();
	void update(SDL_Texture *tekstura, SDL_Texture *g_znacznik, SDL_Texture *g_okno_przedmiotu, TTF_Font*arial,SDL_Renderer *render,Gracz &gracz);
	void sterowanie(Gracz gracz);
	void zucanie(Gracz &gracz, vector<Przeciwnik*> &przeciwniki, SDL_Renderer *render, vector<Przedmiot*> &przedmiksy,Map mapa);
	void statystyki(Gracz &gracz, SDL_Renderer *render,TTF_Font *arial);
};

class UI 
{
public: 
	double posX;
	double posY;
	double wysokosc;
	double szerokosc;
	SDL_Texture* g_interfejs;
	SDL_Texture* g_zycie;
	SDL_Texture* g_znacznik_podpalenia;

	UI(double px, double py, double wys, double szer, SDL_Texture* g_in, SDL_Texture* g_zy, SDL_Texture* g_z_p);

	void update(Gracz gracz, SDL_Renderer* render, TTF_Font*arial, int nr_pietra, string nazwa_pietra);


};

class Klocek
{
	friend class Map;
	friend class Szczur;
	friend class Ogien;
public:
	double posX;
	double posY;
	double szerokosc;
	double wysokosc;
	bool chodzonosc;
	char rodzaj;
	bool widoczny = true;
	bool odwiedzony = false;
	SDL_Texture *tekstura;

	Klocek() {}
	Klocek(double px, double py, SDL_Texture *teks)
	{
		posX = px;
		posY = py;
		tekstura = teks;
		wysokosc = 100.0;
		szerokosc = 100.0;
		rodzaj = NULL;
	}
	virtual void update(SDL_Renderer *render,Gracz gracz, bool &zmiana_lvl)
	{
		

		SDL_Rect rect;
		rect.x = posX;
		rect.y = posY;
		rect.h = wysokosc;
		rect.w = szerokosc;
		if(widoczny==true)SDL_RenderCopy(render, tekstura, NULL, &rect);
	}
	void skalowanie(double przesuniecieX, double przesuniecieY, double skala)
	{
		posX += przesuniecieX;
		posY += przesuniecieY;
		posX *= skala;
		posY *= skala;

		szerokosc *= skala;
		wysokosc *= skala;
	}
};

class Zejscie : public Klocek
{
	friend class Map;
	friend class Szczur;
	friend class Ogien;

public: 
	Zejscie() {}
	Zejscie(double px, double py, SDL_Texture *teks)
	{
		posX = px;
		posY = py;
		widoczny = true;
		tekstura = teks;
		wysokosc = 100.0;
		szerokosc = 100.0;
		rodzaj = NULL;
	}
	void update(SDL_Renderer *render, Gracz gracz, bool &zmiana_lvl)
	{
		SDL_Rect rect;
		rect.x = posX;
		rect.y = posY;
		rect.h = wysokosc;
		rect.w = szerokosc;
		if (widoczny == true)SDL_RenderCopy(render, tekstura, NULL, &rect);
		if (gracz.posX == posX && gracz.posY == posY) zmiana_lvl = true;
	}
};

class Room
{
	friend class Map;
	friend class Szczur;
	friend class Ogien;
public:
	vector<Klocek*> kafelki;
	string uklad;
	Klocek kanalup, kanaldown, kanalright, kanalleft;
	int szerokosc;
	int wysokosc;

	Room() {}

	Room(string uk, SDL_Texture *podloga, SDL_Texture *podloga2, SDL_Texture *podloga3, SDL_Texture *podloga4, SDL_Texture *sciana,bool &zejscie) {
		uklad = uk;
		ukladanie_kafelkow(podloga, podloga2, podloga3, podloga4, sciana,zejscie);
	}

	void ukladanie_kafelkow(SDL_Texture *podloga, SDL_Texture *podloga2, SDL_Texture *podloga3, SDL_Texture *podloga4, SDL_Texture *sciana,bool &zejscie)
	{
		int j = 0;
		int k = 0;
		int tymczasowa_szerokosc = 0;
		Klocek* tp;
		for (int i = 0; i < uklad.size(); i++)
		{
			if (uklad[i] == 'H')
			{
				Klocek kloc(0 + k * 100, 0 + j * 100, podloga);
				kloc.chodzonosc = true;
				int p = rand() % 7;
				if (p == 0)kloc.tekstura=podloga2;
				else if (p == 1)kloc.tekstura=podloga3;
				else if (p == 3)kloc.tekstura=podloga4;
				
				tp = new Klocek;
				*tp = kloc;
				kafelki.push_back(tp);
				tymczasowa_szerokosc++;
				k++;
			}
			if (uklad[i] == '/')
			{
				j++;
				k = 0;
				tymczasowa_szerokosc = 0;
			}
			if (uklad[i] == 'C')
			{
				Klocek kloc(0 + k * 100, 0 + j * 100, sciana);
				kloc.chodzonosc = false;
				tp = new Klocek;
				*tp = kloc;
				kafelki.push_back(tp);
				tymczasowa_szerokosc++;
				k++;
			}
			if (uklad[i] == '>')
			{
				Klocek kloc(0 + k * 100, 0 + j * 100, sciana);
				kloc.chodzonosc = false;
				kloc.rodzaj = '>';
				kanalright = kloc;
				tp = new Klocek;
				*tp = kloc;
				kafelki.push_back(tp);
				tymczasowa_szerokosc++;
				k++;
			}
			if (uklad[i] == '<')
			{
				Klocek kloc(0 + k * 100, 0 + j * 100, sciana);
				kloc.chodzonosc = false;
				kloc.rodzaj = '<';
				kanalleft = kloc;
				tp = new Klocek;
				*tp = kloc;
				kafelki.push_back(tp);
				tymczasowa_szerokosc++;
				k++;
			}
			if (uklad[i] == ',')
			{
				Klocek kloc(0 + k * 100, 0 + j * 100, sciana);
				kloc.chodzonosc = false;
				kloc.rodzaj = ',';
				kanaldown = kloc;
				tp = new Klocek;
				*tp = kloc;
				kafelki.push_back(tp);
				tymczasowa_szerokosc++;
				k++;
			}
			if (uklad[i] == '`')
			{
				Klocek kloc(0 + k * 100, 0 + j * 100, sciana);
				kloc.chodzonosc = false;
				kloc.rodzaj = '`';
				kanalup = kloc;
				tp = new Klocek;
				*tp = kloc;
				kafelki.push_back(tp);
				tymczasowa_szerokosc++;
				k++;
			}
			if (tymczasowa_szerokosc > szerokosc)szerokosc = tymczasowa_szerokosc;

		}
		wysokosc = j;
		j = 0;
		k = 0;
	}

	void przesuniecie(double x, double y)
	{
		for (auto itr = kafelki.begin(); itr != kafelki.end(); itr++)
		{
			(*itr)->posX += x;
			(*itr)->posY += y;
		}
	}

	void update(SDL_Renderer *render, Gracz gracz, bool &zmiana_lvl)
	{
		for (auto itr = kafelki.begin(); itr != kafelki.end(); itr++)
		{
			(*itr)->update(render,gracz,zmiana_lvl);
		}
	}

	void skalowanie(double przesuniecieX, double przesuniecieY, double skala)
	{
		for (auto itr = kafelki.begin(); itr != kafelki.end(); itr++) {
			(*itr)->skalowanie(przesuniecieX, przesuniecieY, skala);
		}
	}

	void dodawanie_sciezek(Room* pokoje, SDL_Texture *podloga)
	{
		Klocek* tp;
		bool przerwac = false;

		double pxl = kanalleft.posX;
		double pxd = kanaldown.posX;
		double pxr = kanalright.posX;
		double pxu = kanalup.posX;

		double pyl = kanalleft.posY;
		double pyd = kanaldown.posY;
		double pyr = kanalright.posY;
		double pyu = kanalup.posY;
		
		
		vector<Klocek*> kafelkiup;
		vector<Klocek*> kafelkidown;
		vector<Klocek*> kafelkiright;
		vector<Klocek*> kafelkileft;

		

		for (int i = 0; i < 14; i++) {  //Gora
			if (przerwac == true)break;

			if(i!=0)pyu -= 100;
			
			Klocek kloc(pxu,pyu,podloga);
			kloc.chodzonosc = true;
			tp = new Klocek;
			*tp = kloc;
			kafelkiup.push_back(tp);
			for (auto itr = pokoje->kafelki.begin(); itr != pokoje->kafelki.end(); itr++) {
				if (kloc.posX == (*itr)->posX&&kloc.posY == (*itr)->posY)przerwac = true;
			}
			
		}
		for (int i = 0; i < 14; i++) {  //Dol
			if (przerwac == true)break;

			if (i != 0)pyd += 100;

			Klocek kloc(pxd, pyd, podloga);
			kloc.chodzonosc = true;
			tp = new Klocek;
			*tp = kloc;
			kafelkidown.push_back(tp);
			for (auto itr = pokoje->kafelki.begin(); itr != pokoje->kafelki.end(); itr++) {
				if (kloc.posX == (*itr)->posX&&kloc.posY == (*itr)->posY)przerwac = true;
			}

		}
		for (int i = 0; i < 14; i++) {  //Prawo
			if (przerwac == true)break;

			if (i != 0)pxr += 100;

			Klocek kloc(pxr, pyr, podloga);
			kloc.chodzonosc = true;
			tp = new Klocek;
			*tp = kloc;
			kafelkiright.push_back(tp);
			for (auto itr = pokoje->kafelki.begin(); itr != pokoje->kafelki.end(); itr++) {
				if (kloc.posX == (*itr)->posX&&kloc.posY == (*itr)->posY)przerwac = true;
			}

		}
		for (int i = 0; i < 14; i++) {  //Lewo
			if (przerwac == true)break;

			if (i != 0)pyl -= 100;

			Klocek kloc(pxl, pyl, podloga);
			kloc.chodzonosc = true;
			tp = new Klocek;
			*tp = kloc;
			kafelkileft.push_back(tp);
			for (auto itr = pokoje->kafelki.begin(); itr != pokoje->kafelki.end(); itr++) {
				if (kloc.posX == (*itr)->posX&&kloc.posY == (*itr)->posY)przerwac = true;
			}

		}

		if (kafelkileft.size() < 14)kafelki.insert(std::end(kafelki), std::begin(kafelkileft), std::end(kafelkileft));
		if (kafelkiright.size() < 14)kafelki.insert(std::end(kafelki), std::begin(kafelkiright), std::end(kafelkiright));
		if (kafelkiup.size() < 14)kafelki.insert(std::end(kafelki), std::begin(kafelkiup), std::end(kafelkiup));
		if (kafelkidown.size() < 14)kafelki.insert(std::end(kafelki), std::begin(kafelkidown), std::end(kafelkidown));

	}

	void kanalowanie()
	{
		for (auto i = kafelki.begin(); i != kafelki.end(); i++) {
			if ((*i)->rodzaj == '`')kanalup = *(*i);
			if ((*i)->rodzaj == ',')kanaldown = *(*i);
			if ((*i)->rodzaj == '>')kanalright = *(*i);
			if ((*i)->rodzaj == '<')kanalleft = *(*i);
		}
	}
};

#pragma region Przedmioty
//Przedmioty:  

class Zbroja :public Przedmiot
{
	friend class Map;
public:
	Zbroja() {};
	Zbroja(SDL_Texture* teks, SDL_Texture* teks_r, string nazw)
	{
		posX = 0;
		posY = 0;
		wysokosc = 100;
		szerokosc = 100;
		rodzaj = 'z';
		nazwa = nazw;
		tekstura = teks;
		tekstura_r = teks_r;
		aktywny = true;
		zucane = false;

		if (nazw == "Skorzana Zbroja")
		{
			if (rand() % 3 == 0)
			{
				zrecznosc = 2;
			}
			else
			{
				zrecznosc = 0;
			}
			sila = 0;
			inteligencja = 0;
			obrazenia = 0;
			ochrona = 2;
			wymagana_sila = 3;
		}
		if (nazw == "Szata Maga")
		{
			if (rand() % 3 == 0)inteligencja = 6;
			else inteligencja = 3;
			sila = 0;
			zrecznosc = 0;
			obrazenia = 0;
			ochrona = 0;
			wymagana_sila = 0;
		}
		if (nazw == "Zbroja Plytowa")
		{
			inteligencja = 0;
			if (rand() % 3 == 0)sila = 2;
			else sila = 0;
			zrecznosc = -2;
			obrazenia = 0;
			ochrona = 3;
			wymagana_sila = 5;
		}
		if (nazw == "Hwang")
		{
			
				inteligencja = 3;
				sila = 3;
				zrecznosc = 3;
				obrazenia = 0;
				ochrona = 8;
				wymagana_sila = 12;
			
		}

	}

	string rodzaj_kalsy() { return "Zbroja"; }

private:


};

class Miecz :public Przedmiot
{
	friend class Map;
public:
	Miecz() {};
	Miecz(SDL_Texture* teks, SDL_Texture* teks_r, string nazw)
	{
		posX = 0;
		posY = 0;
		wysokosc = 100;
		szerokosc = 100;
		rodzaj = 'm';
		nazwa = nazw;
		tekstura = teks;
		tekstura_r = teks_r;
		aktywny = true;
		zucane = false;

		if (nazw == "Maly Sztylet")
		{
			inteligencja = 0;
			sila = 0;
			if (rand() % 3 == 0)zrecznosc = 3;
			else zrecznosc = 1;
			obrazenia = 1;
			ochrona = 0;
			wymagana_sila = 1;
		}
		if (nazw == "Prosty Miecz")
		{
			inteligencja = 0;
			sila = 0;
			zrecznosc = -1;
			obrazenia = 4;
			ochrona = 0;
			wymagana_sila = 4;
		}
		if (nazw == "Katana")
		{
			inteligencja = 0;
			sila = 0;
			zrecznosc = 4;
			obrazenia = 2;
			ochrona = 0;
			wymagana_sila = 4;
		}
		if (nazw == "Miecz Syriusza")
		{
			inteligencja = 2;
			sila = 2;
			zrecznosc = 2;
			obrazenia = 6;
			ochrona = 2;
			wymagana_sila = 6;
		}


	}

	string rodzaj_kalsy() { return "Miecz"; }

private:

};

#pragma endregion


class Map
{
	friend class Gracz;
	friend class Szczur;
	friend class Ogien;
public:
	vector<Room*> pokoje;
	Map() {};
	~Map() {};
	bool zejscie = true;
	Uint32 dodanie_licznik = 0;
	bool dodanie = true;
	
	
	string rodzaje_pomieszczen()
	{
		int a;
		string pom;
		a = rand() % 4;
		if (a == 0)
		{
			//pom = "CCCC`CCC/CHHHHHHC/CHHHHHHC/CHHHHHHC/CHHHHHHCCCCCCC/<HHHHHHHHHHHH>/CHHHHHHHHHHHHC/CHHHHHHHHHHHHC/C,CCCCC,CCCC,C";
			pom = "CCC`CCC/CHHHHHC/CHHHHHC/<HHHHH>/CHHHHHC/CHHHHHC/CCC,CCC";
		}
		if (a == 1)
		{
			pom = "CCC`CCC/CHHHHHC/CHHHHHC/<HHHHH>/CHHHHHC/CHHHHHC/CCC,CCC";
		}
		if (a == 2)
		{
			pom = "CCCC`CCCCC/CHHHHHHHHC/<HHHHHHHH>/CHHHHHHHHC/CCCC,CCCCC";
		}
		if (a == 3)
		{
			pom = "CCC`CCC/CHHHHHC/CHHHHHC/<HHHHH>/CHHHHHC/CHHHHHC/CCC,CCC";
		}
		return pom;
	}

	void ukladanie_pokoi(SDL_Texture *podloga, SDL_Texture *podloga2, SDL_Texture *podloga3, SDL_Texture *podloga4, SDL_Texture *sciana,Map &mapa)
	{
		Room* tp;
		Room* tp2;
		int j = 0;
		int k = 0;
		int przesuniecieX = 0;
		int przesuniecieY = 0;
		int najwieksza_wysokosc = 0;
		for (int i = 0; i < 9; i++)
		{
			if (i == 3 || i == 6)
			{
				j++;
				k = 0;
				przesuniecieX = 0;
				przesuniecieY = najwieksza_wysokosc;
			}
			string rodz = rodzaje_pomieszczen();
			int a = rand() % 10;
			Room  pokoj(rodz, podloga, podloga2, podloga3, podloga4, sciana, zejscie);
			

			Room pokoj2(rodz, podloga, podloga2, podloga3, podloga4, sciana,zejscie);

			if (pokoj.wysokosc > najwieksza_wysokosc)
			{
				najwieksza_wysokosc = pokoj.wysokosc;
			}

			pokoj.przesuniecie(przesuniecieX * 100 + 400*k, przesuniecieY * 100*j + 400*j);
			pokoj2.przesuniecie(przesuniecieX * 100 + 400 * k, przesuniecieY * 100 * j + 400 * j);

			tp = new Room;
			tp2 = new Room;
			*tp = pokoj;
			*tp2 = pokoj2;
			pokoje.push_back(tp);
			mapa.pokoje.push_back(tp2);

			
			przesuniecieX += pokoj.szerokosc;
			k++;
		}
		j = 0;

		for (auto i = pokoje.begin(); i != pokoje.end(); i++) {
			(*i)->kanalowanie();
		}
		for (auto i = mapa.pokoje.begin(); i != mapa.pokoje.end(); i++) {
			(*i)->kanalowanie();
		}
		
		/*for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				if(j!=i)pokoje[i]->dodawanie_sciezek(pokoje[j], podloga);
			 }
		}
		*/
		
		pokoje[0]->dodawanie_sciezek(pokoje[3], podloga); pokoje[0]->dodawanie_sciezek(pokoje[1], podloga);
		pokoje[1]->dodawanie_sciezek(pokoje[2], podloga); pokoje[1]->dodawanie_sciezek(pokoje[4], podloga); pokoje[1]->dodawanie_sciezek(pokoje[5], podloga);
		pokoje[2]->dodawanie_sciezek(pokoje[5], podloga);
		pokoje[3]->dodawanie_sciezek(pokoje[4], podloga); pokoje[3]->dodawanie_sciezek(pokoje[6], podloga);
		pokoje[4]->dodawanie_sciezek(pokoje[5], podloga); pokoje[4]->dodawanie_sciezek(pokoje[7], podloga);
		pokoje[5]->dodawanie_sciezek(pokoje[8], podloga);
		pokoje[6]->dodawanie_sciezek(pokoje[7], podloga);
		pokoje[7]->dodawanie_sciezek(pokoje[8], podloga);

		mapa.pokoje[0]->dodawanie_sciezek(mapa.pokoje[3], podloga); mapa.pokoje[0]->dodawanie_sciezek(mapa.pokoje[1], podloga);
		mapa.pokoje[1]->dodawanie_sciezek(mapa.pokoje[2], podloga); mapa.pokoje[1]->dodawanie_sciezek(mapa.pokoje[4], podloga); mapa.pokoje[1]->dodawanie_sciezek(mapa.pokoje[5], podloga);
		mapa.pokoje[2]->dodawanie_sciezek(mapa.pokoje[5], podloga);
		mapa.pokoje[3]->dodawanie_sciezek(mapa.pokoje[4], podloga); mapa.pokoje[3]->dodawanie_sciezek(mapa.pokoje[6], podloga);
		mapa.pokoje[4]->dodawanie_sciezek(mapa.pokoje[5], podloga); mapa.pokoje[4]->dodawanie_sciezek(mapa.pokoje[7], podloga);
		mapa.pokoje[5]->dodawanie_sciezek(mapa.pokoje[8], podloga);
		mapa.pokoje[6]->dodawanie_sciezek(mapa.pokoje[7], podloga);
		mapa.pokoje[7]->dodawanie_sciezek(mapa.pokoje[8], podloga);
	}
	
	void update(SDL_Renderer *render, Gracz gracz, bool &zmiana_lvl)
	{
		
		for (int i=8;i>=0;i--)
		{
			pokoje[i]->update(render,gracz,zmiana_lvl);
		}
		
	}
	
	void skalowanie(double przesuniecieX, double przesuniecieY, double skala)
	{
		for (auto itr = pokoje.begin(); itr != pokoje.end(); itr++) {
			(*itr)->skalowanie(przesuniecieX, przesuniecieY, skala);
		}
	}

	void czarna_mapa(SDL_Texture* czern)
	{
		for (auto i = pokoje.begin(); i != pokoje.end(); i++)
		{
			for (auto j = (*i)->kafelki.begin(); j != (*i)->kafelki.end(); j++)
			{
				(*j)->tekstura = czern;
			}
		}
	}

	void resp_item(vector<Przedmiot*> szablon_przedmiotow, vector<Przedmiot*> &przedmioty)
	{
		for (auto i = pokoje.begin(); i != pokoje.end(); i++) {
			bool dodalo = false;
			if (rand() % 10 >3)
			{
				do
				{
					int b = rand() % (*i)->kafelki.size();
					if ((*i)->kafelki[b]->chodzonosc == true)
					{
						Przedmiot* tp;

						Przedmiot *itr = szablon_przedmiotow[int((rand() % szablon_przedmiotow.size()))];
						cout << itr->rodzaj_kalsy() << endl;
						if (itr->rodzaj_kalsy() == "Potion") {
							cout << "elo" << endl;
							Potion elo(itr->nazwa, (*i)->kafelki[b]->posX, (*i)->kafelki[b]->posY, itr->szerokosc, itr->wysokosc, itr->tekstura, true, itr->rodzaj, 1, itr->obrazenia);
							tp = new Potion;
							*tp = elo;
							przedmioty.push_back(tp);
						}
						if (itr->rodzaj_kalsy() == "zwykly") {
							cout << "elo" << endl;
							Przedmiot elo(itr->nazwa, (*i)->kafelki[b]->posX, (*i)->kafelki[b]->posY, itr->szerokosc, itr->wysokosc, itr->obrazenia, itr->ochrona, itr->tekstura, itr->tekstura_r, true, itr->rodzaj);
							tp = new Przedmiot;
							*tp = elo;
							przedmioty.push_back(tp);
						}
						if (itr->rodzaj_kalsy() == "Zbroja") {
							cout << "elo" << endl;
							Zbroja elo(itr->tekstura, itr->tekstura_r, itr->nazwa);
							elo.posX = (*i)->kafelki[b]->posX;
							elo.posY = (*i)->kafelki[b]->posY;
							tp = new Zbroja;
							*tp = elo;
							przedmioty.push_back(tp);
						}
						if (itr->rodzaj_kalsy() == "Miecz") {
							cout << "elo" << endl;
							Miecz elo(itr->tekstura, itr->tekstura_r, itr->nazwa);
							elo.posX = (*i)->kafelki[b]->posX;
							elo.posY = (*i)->kafelki[b]->posY;
							tp = new Miecz;
							*tp = elo;
							przedmioty.push_back(tp);
						}

						dodalo = true;
					}

				} while (dodalo == false);
				
			}
			dodalo = false;
		}
		cout<<przedmioty.size()<<endl;
	}

	void resp_przeciwnikow(vector<Przeciwnik*> szablon_przedmiotow, vector<Przeciwnik*> &przedmioty)
	{
		for (auto i = pokoje.begin(); i != pokoje.end(); i++) {
			bool dodalo = false;
			if (rand() % 10 >3)
			{
				do
				{
					int b = rand() % (*i)->kafelki.size();
					if ((*i)->kafelki[b]->chodzonosc == true)
					{
						Przeciwnik* tp;

						Przeciwnik *itr = szablon_przedmiotow[int((rand() % szablon_przedmiotow.size()))];
						cout << itr->rodzaj() << endl;
						if (itr->rodzaj() == "Szczur") {
							cout << "elo" << endl;
							Szczur elo(itr->tekstura, (*i)->kafelki[b]->posX, (*i)->kafelki[b]->posY, (*i)->kafelki[b]->szerokosc);
							tp = new Szczur;
							*tp = elo;
							przedmioty.push_back(tp);
						}
						if (itr->rodzaj() == "Ogien") {
							cout << "elo" << endl;
							Ogien elo(itr->tekstura, (*i)->kafelki[b]->posX, (*i)->kafelki[b]->posY, (*i)->kafelki[b]->szerokosc);
							tp = new Ogien;
							*tp = elo;
							przedmioty.push_back(tp);
						}

						dodalo = true;
					}

				} while (dodalo == false);

			}
			dodalo = false;
		}
		cout << przedmioty.size() << endl;
	}

	void dodanie_zejscia(SDL_Texture *tekstura, SDL_Texture *g_schody,Gracz gracz)
	{
		int a = rand() % pokoje.size();
		bool gowno = false;
		do{
			cout << gowno << endl;
			int b = rand() % pokoje[a]->kafelki.size();
			if (pokoje[a]->kafelki[b]->chodzonosc == true)
			{
				
				Klocek* tp;
				tp = new Zejscie;
				Zejscie elo(pokoje[a]->kafelki[b]->posX, pokoje[a]->kafelki[b]->posY, tekstura);
				elo.chodzonosc = true;
				
				*tp = elo;
				pokoje[a]->kafelki.erase(pokoje[a]->kafelki.begin()+b);
				pokoje[a]->kafelki.push_back(tp);

				
				zejscie = true;
				gowno = true;
			}
			
		} while (gowno == false);
		Klocek* tp;
		tp = new Klocek;
		Klocek elo(gracz.posX, gracz.posY, g_schody);
		elo.chodzonosc = true;
		*tp = elo;
		pokoje[0]->kafelki.push_back(tp);
	}

	void dodanie_przeciwnika(vector<Przeciwnik*> &przeciwniki,SDL_Texture* szczur,SDL_Texture* ogien)
	{
		
		
		if (Uint32 i = SDL_GetTicks() - dodanie_licznik >= 30000&dodanie==false)
		{

			dodanie =true;

		}

		if (dodanie == true) {
			int a = rand() % pokoje.size();
			bool gowno = false;
			do {
				cout << gowno << endl;
				int b = rand() % pokoje[a]->kafelki.size();
				if (pokoje[a]->kafelki[b]->chodzonosc == true)
				{
					if (rand() % 2 == 0)
					{
						Przeciwnik* tp;
						tp = new Szczur;
						Szczur elo(szczur, pokoje[a]->kafelki[b]->posX, pokoje[a]->kafelki[b]->posY, pokoje[a]->kafelki[b]->szerokosc);
						cout << "kafel:" << pokoje[a]->kafelki[b]->szerokosc << endl;
						cout<<"niedidanmy:"<<elo.szerokosc << endl;

						*tp = elo;
						przeciwniki.push_back(tp);
						cout<<"dodany"<<przeciwniki.back()->szerokosc << endl;
					}
					else
					{
						Przeciwnik* tp;
						tp = new Ogien;
						Ogien elo(ogien, pokoje[a]->kafelki[b]->posX, pokoje[a]->kafelki[b]->posY, pokoje[a]->kafelki[b]->szerokosc);
					
						elo.aktywny = true;

						*tp = elo;
						przeciwniki.push_back(tp);
					}
					gowno = true;
				}

			} while (gowno == false);
		}
		if (dodanie == true)
		{
			dodanie_licznik = SDL_GetTicks();
			dodanie = false;
		}
		cout<<przeciwniki.size() << endl;

	}

	void clear()
	{
		pokoje.clear();
	}
};






