#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include<Windows.h>
#include<iostream>
#include<vector>
#include<sstream>
#include<time.h>



using namespace std;


class Okno_eq;
class Przeciwnik;
class Przedmiot;




class Gracz
{
	friend class Okno_eq;
	friend class Przeciwnik;
	friend class Przedmiot;
	friend class Mapa;
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
	void update(SDL_Renderer *render,int &_s,int &_t, double przesuniecieX, double przesuniecieY, SDL_Texture *zdrowie, TTF_Font *arial, SDL_Texture *g_obrazenia);
	void poruszanie(Okno_eq okno_eq, double s,vector<Przeciwnik> &przeciwniki, Mix_Chunk *m_ciecie);
	void koniec(vector<Przeciwnik> &przeciwnik);
	void podnoszenie(vector<Przedmiot*> &przedmiot);
	void atak_przycisk();
	void atak(vector<Przeciwnik> &przeciwnik, SDL_Texture *g_znacznik, SDL_Texture *g_ciecie, SDL_Renderer *render, Mix_Chunk *m_chodzenie);
	void pauza(vector<Przeciwnik> &przeciwnik);
	bool przesuwanie_gracz(vector<Przeciwnik> &przeciwniki, int a);
	void efekty_pasywne();
	void animacje_ataku(SDL_Renderer *render, SDL_Texture *tekstura_ataku, TTF_Font *arial);
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
	//3.Taimery:
	Uint32 animacja_ciecia = 0; 
	bool b_ciecia = false;
	Uint32 animacja_miss = 0; 
	SDL_Rect obr;
	bool b_miss = false;
	//4.Pasywne:
	bool podpalenie = false;
	bool zamrozenie = false;

	Przeciwnik(string,double, double, double, double,double,double,double,double,SDL_Texture*, vector<SDL_Rect> spreje);
	~Przeciwnik();
	void poruszanie(SDL_Renderer *render,Gracz &gracz1,int &_s , int &_t, double przesuniecieX, double przesuniecieY, double s, SDL_Texture *zdrowie);
	void atak(Gracz &gracz, Mix_Chunk *m_obrazenia);
	void update(SDL_Renderer *render, int &_s, int &_t, double przesuniecieX, double przesuniecieY, double s, SDL_Texture *zdrowie, SDL_Texture *ciecie, TTF_Font *arial);
	void potion(char rodzaj);
	void animacje_ataku(SDL_Renderer *render, SDL_Texture *tekstura_ataku, TTF_Font *arial);
};

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
//3.Rodzaj:
	int amunicja;
	char rodzaj;
	bool zucane;
	int wymagana_sila;
//3. ...
	SDL_Texture *tekstura;
	SDL_Texture *tekstura_r;
	bool aktywny=true;
///METODY:
	Przedmiot(string , double, double, double, double,double, double,SDL_Texture*,SDL_Texture*,bool,char);
	Przedmiot();
	~Przedmiot();
	virtual bool uzycie(Gracz &gracz) { return false; }
	virtual void update( SDL_Renderer *render, double przesuniecieX, double przesuniecieY, double s);
	virtual void okno_informacji(SDL_Renderer* render, SDL_Texture *tekstura, TTF_Font *font, double px, double py);
};

class Potion : public Przedmiot {

public:
	
	Potion(){}
	Potion(string, double, double, double, double, SDL_Texture*, bool, char,int,double);
	bool uzycie(Gracz &gracz);
	void update(SDL_Renderer *render, double przesuniecieX, double przesuniecieY, double s);
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
	void zucanie(Gracz &gracz, vector<Przeciwnik> &przeciwniki, SDL_Renderer *render,double s, vector<Przedmiot*> &przedmiksy);
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

	void update(Gracz gracz, SDL_Renderer* render, TTF_Font*arial);


};


class Klocek
{
	friend class Map;
public:
	double posX;
	double posY;
	double szerokosc;
	double wysokosc;
	bool chodzonosc;
	SDL_Texture *tekstura;

	Klocek() {}
	Klocek(double px, double py, SDL_Texture *teks)
	{
		posX = px;
		posY = py;
		tekstura = teks;
		wysokosc = 100.0;
		szerokosc = 100.0;
	}
	void update(double skala, double przesuniecieX, double przesuniecieY, SDL_Renderer *render)
	{
		posY += przesuniecieY;
		posX += przesuniecieX;
		posX *= skala;
		posY *= skala;
		szerokosc *= skala;
		wysokosc *= skala;

		SDL_Rect rect;
		rect.x = posX;
		rect.y = posY;
		rect.h = wysokosc;
		rect.w = szerokosc;
		SDL_RenderCopy(render, tekstura, NULL, &rect);
	}

};

class Room
{
	friend class Map;
public:
	vector<Klocek*> kafelki;
	string uklad;
	int szerokosc;
	int wysokosc;

	Room() {}

	Room(string uk, SDL_Texture *podloga, SDL_Texture *sciana) {
		uklad = uk;
		ukladanie_kafelkow(podloga, sciana);
	}

	void ukladanie_kafelkow(SDL_Texture *podloga, SDL_Texture *sciana)
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

	void update(double skala, int przesuniecieX, int przesuniecieY,SDL_Renderer *render)
	{
		for (auto itr = kafelki.begin(); itr != kafelki.end(); itr++)
		{
			(*itr)->update(skala, przesuniecieX, przesuniecieY,render);
		}
	}
};

class Map
{
public:
	vector<Room*> pokoje;
	Map() {};
	~Map() {};
	void ukladanie_pokoi(SDL_Texture *podloga, SDL_Texture *sciana)
	{
		Room* tp;
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

			Room pokoj("CCCCCCC/CHHHHHC/CHHHHHC/CHHHHHC/CHHHHHC/CHHHHHC/CCCCCCC", podloga, sciana);

			if (pokoj.wysokosc > najwieksza_wysokosc)
			{
				najwieksza_wysokosc = pokoj.wysokosc;
			}

			pokoj.przesuniecie(przesuniecieX * 100 + 400*k, przesuniecieY * 100*j + 400*j);

			tp = new Room;
			*tp = pokoj;
			pokoje.push_back(tp);

			
			przesuniecieX += pokoj.szerokosc;
			k++;
		}
		j = 0;
	}
	void update(double skala, int przesuniecieX, int przesuniecieY, SDL_Renderer *render)
	{
		for (auto itr = pokoje.begin(); itr != pokoje.end(); itr++)
		{
			(*itr)->update(skala, przesuniecieX, przesuniecieY,render);
		}
	}

private:

};

//Funkcje:

vector<SDL_Rect> ustawianie_rect_spraj(int wys, int szer, int kolumny);
int taimer_animacji(int &_t, int &_s,int liczba_klatek, int odswierzanie);
int taimer(Uint32 czas, int liczba_klatek, Uint32 odswierzanie);
