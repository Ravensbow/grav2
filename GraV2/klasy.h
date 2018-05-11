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
	int znaczX = 0;
	int znaczY = 0;

	//2.Statystyki:
	double sila;
	double zrecznosc;
	double inteligencja;
	int zycie;
	int max_zycie;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	//3. ...
	bool tura;
	bool chodzi=false;
	bool wlaczanie_ataku = false;
	double aktualne_zycie;
	SDL_Texture *tekstura;
	SDL_Texture *chodzenie;
	vector<Przedmiot*> ekwipunek;
	vector<Przedmiot> zalozone;

///   METODY:
	Gracz( double, double, double, double,bool, SDL_Texture*, SDL_Texture*);
	~Gracz();
	void update(SDL_Renderer *render,int &_s,int &_t, int przesuniecieX, int przesuniecieY,double s, SDL_Texture *zdrowie);
	void poruszanie(Okno_eq okno_eq, double s,vector<Przeciwnik> &przeciwniki);
	void koniec(vector<Przeciwnik> &przeciwnik);
	void podnoszenie(vector<Przedmiot*> &przedmiot);
	void atak_przycisk();
	void atak(vector<Przeciwnik> &przeciwnik, SDL_Texture *g_znacznik, SDL_Texture *g_ciecie, SDL_Renderer *render);
	void pauza(vector<Przeciwnik> &przeciwnik);
	bool przesuwanie_gracz(vector<Przeciwnik> &przeciwniki, int a);
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
	//2.Statystyki:
	double sila;
	double zrecznosc;
	double inteligencja;
	int zycie;
	int max_zycie;

	Przeciwnik(string,double, double, double, double,double,double,double,SDL_Texture*, vector<SDL_Rect> spreje);
	~Przeciwnik();
	void update(SDL_Renderer *render,Gracz &gracz1,int &_s , int &_t, int przesuniecieX, int przesuniecieY, double s, SDL_Texture *zdrowie);
	void atak(Gracz &gracz);

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
//3.Rodzaj:
	char rodzaj;
//3. ...
	SDL_Texture *tekstura;
	SDL_Texture *tekstura_r;
	bool aktywny=true;
///METODY:
	Przedmiot(string , double, double, double, double, SDL_Texture*,SDL_Texture*,bool,char);
	Przedmiot();
	~Przedmiot();
	virtual bool uzycie(Gracz &gracz) { return false; }
	virtual void update( SDL_Renderer *render, int przesuniecieX, int przesuniecieY, double s);
};

class Potion : public Przedmiot {

public:
	Potion(){}
	Potion(string, double, double, double, double, SDL_Texture*, bool, char);
	void zucanie(vector<Przeciwnik> &przeciwniki,Gracz gracz);
	bool uzycie(Gracz &gracz);
	void update(SDL_Renderer *render, int przesuniecieX, int przesuniecieY, double s);
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

	Okno_eq(double = 100, double = 100, double = 90, double = 90, bool=false);
	~Okno_eq();
	void update(SDL_Texture *tekstura, SDL_Texture *g_znacznik, SDL_Texture *g_okno_przedmiotu, TTF_Font*arial,SDL_Renderer *render,Gracz &gracz);
	void sterowanie(Gracz gracz);
};

class Mapa
{
	friend class Przedmiot;
	friend class Gracz;
public:

	double posX;
	double posY;
	double szer;
	double wys;
	bool widocznosc=true;
	bool obrot;


	virtual void update(SDL_Renderer *render, int przesuniecieX, int przesuniecieY, double s) = 0;

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

	UI(double px, double py, double wys, double szer, SDL_Texture* g_in, SDL_Texture* g_zy);

	void update(Gracz gracz, SDL_Renderer* render, TTF_Font*arial);


};











///POKOJ 1///////
class cPokoj1 : public Mapa {
	friend class Przedmiot;
	friend class Gracz;


public:
	SDL_Texture * obr;

	cPokoj1(double x, double y, SDL_Texture *tekstura) {
		szer = 400;
		wys = 200;
		posX = x;
		posY = y;
		obr = tekstura;
	
	}

	virtual void update(SDL_Renderer *render,int przesuniecieX,int przesuniecieY, double s);
};

///POKOJ 2///////
class cPokoj2 : public Mapa {
	friend class Przedmiot;
	friend class Gracz;


public:
	SDL_Texture * obr;

	cPokoj2(double x, double y, SDL_Texture *tekstura) {
		szer = 300;
		wys = 300;
		posX = x;
		posY = y;
		obr = tekstura;
		
	}

	virtual void update(SDL_Renderer *render, int przesuniecieX, int przesuniecieY, double s);
};

///POKOJ 3///////
class cPokoj3 : public Mapa {
	friend class Przedmiot;
	friend class Gracz;


public:
	SDL_Texture * obr;

	cPokoj3(double x, double y, SDL_Texture *tekstura) {
		szer =  200;
		wys = 300;
		posX = x;
		posY = y;
		obr = tekstura;
		
	}

	virtual void update(SDL_Renderer *render, int przesuniecieX, int przesuniecieY, double s);
};

//Funkcje:

vector<SDL_Rect> ustawianie_rect_spraj(int wys, int szer, int kolumny);
int taimer_animacji(int &_t, int &_s,int liczba_klatek, int odswierzanie);
