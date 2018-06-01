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
class Map;




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
	void poruszanie(Okno_eq okno_eq, vector<Przeciwnik> &przeciwniki, Mix_Chunk *m_ciecie, Map mapa);
	void koniec(vector<Przeciwnik> &przeciwnik);
	void podnoszenie(vector<Przedmiot*> &przedmiot);
	void atak_przycisk();
	void atak(vector<Przeciwnik> &przeciwnik, SDL_Texture *g_znacznik, SDL_Texture *g_ciecie, SDL_Renderer *render, Mix_Chunk *m_chodzenie);
	void pauza(vector<Przeciwnik> &przeciwnik);
	bool przesuwanie_gracz(vector<Przeciwnik> &przeciwniki, Map mapa, int a);
	void efekty_pasywne();
	void animacje_ataku(SDL_Renderer *render, SDL_Texture *tekstura_ataku, TTF_Font *arial);
	void skalowanie(double przesuniecieX, double przesuniecieY,double skala);
	bool kolizja_gracz(Map mapa, int a);
	void usuwanie_mgly(Map &mapa, SDL_Texture* g_mgla);
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
	void poruszanie(SDL_Renderer *render,Gracz &gracz1,int &_s , int &_t, SDL_Texture *zdrowie, Map mapa);
	bool kolizja_przeciwnik(Map mapa, int a);
	void atak(Gracz &gracz, Mix_Chunk *m_obrazenia);
	void update(SDL_Renderer *render, int &_s, int &_t,SDL_Texture *zdrowie, SDL_Texture *ciecie, TTF_Font *arial);
	void potion(char rodzaj);
	void animacje_ataku(SDL_Renderer *render, SDL_Texture *tekstura_ataku, TTF_Font *arial);
	void skalowanie(double przesuniecieX, double przesuniecieY, double skala);
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
	virtual void update( SDL_Renderer *render);
	virtual void okno_informacji(SDL_Renderer* render, SDL_Texture *tekstura, TTF_Font *font, double px, double py);
	void skalowanie(double przesuniecieX, double przesuniecieY, double skala);
};

class Potion : public Przedmiot {

public:
	
	Potion(){}
	Potion(string, double, double, double, double, SDL_Texture*, bool, char,int,double);
	bool uzycie(Gracz &gracz);
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
	void zucanie(Gracz &gracz, vector<Przeciwnik> &przeciwniki, SDL_Renderer *render, vector<Przedmiot*> &przedmiksy);
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
	void update(SDL_Renderer *render)
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

class Room
{
	friend class Map;
public:
	vector<Klocek*> kafelki;
	string uklad;
	Klocek kanalup, kanaldown, kanalright, kanalleft;
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

	void update(SDL_Renderer *render)
	{
		for (auto itr = kafelki.begin(); itr != kafelki.end(); itr++)
		{
			(*itr)->update(render);
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

class Map
{
	friend class Gracz;
public:
	vector<Room*> pokoje;
	Map() {};
	~Map() {};
	
	string rodzaje_pomieszczen()
	{
		int a;
		string pom;
		a = rand() % 4;
		if (a == 0)
		{
			pom = "CCCC`CCC/CHHHHHHC/CHHHHHHC/CHHHHHHC/CHHHHHHCCCCCCCCCCCC/<HHHHHHHHHHHHHHHHH>/CHHHHHHHHHHHHHHHHHC/CHHHHHHHHHHHHHHHHHC/C,CCCCCCC,CCCCCCC,C";
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

	void ukladanie_pokoi(SDL_Texture *podloga, SDL_Texture *sciana,Map &mapa)
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
			Room pokoj(rodz, podloga, sciana);
			Room pokoj2(rodz, podloga, sciana);

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
		
		//for (int i = 0; i < 9; i++) {
		//	for (int j = 0; j < 9; j++) {
		//		if(j!=i)pokoje[i]->dodawanie_sciezek(pokoje[j], podloga);
		//	 }
		//}
		
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
	
	void update(SDL_Renderer *render)
	{
		
		for (int i=8;i>=0;i--)
		{
			pokoje[i]->update(render);
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

	void kopiowanie(Map &mapa)
	{
		
		
	}
};

//Funkcje:

vector<SDL_Rect> ustawianie_rect_spraj(int wys, int szer, int kolumny);
int taimer_animacji(int &_t, int &_s,int liczba_klatek, int odswierzanie);
int taimer(Uint32 czas, int liczba_klatek, Uint32 odswierzanie);
