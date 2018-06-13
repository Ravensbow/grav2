#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include<Windows.h>
#include<iostream>
#include<vector>
#include<sstream>
#include<time.h>
#include"klasy.h"
#include<string>

using namespace std;

//Funkcje:


SDL_Texture* napis(int c1,int c2,int c3, TTF_Font*arial,SDL_Renderer* render,string napis)
{
	SDL_Color kolor = { c1,c2,c3 };
	char const* pchar = napis.c_str();
	SDL_Surface* tekst = TTF_RenderText_Blended(arial, pchar, kolor);
	SDL_Texture* tt_sila = SDL_CreateTextureFromSurface(render, tekst);

	SDL_FreeSurface(tekst);
	
	return  tt_sila;
	
}

vector<SDL_Rect> ustawianie_rect_spraj(int wys, int szer, int kolumny)
{
	vector<SDL_Rect> r_spraj;
	r_spraj.resize(kolumny);

	for (int i = 0; i < kolumny; i++)
	{
		r_spraj[i].w = szer;
		r_spraj[i].h = wys;
		r_spraj[i].x = 0 + i * szer;
		r_spraj[i].y = 0;


	}

	return r_spraj;
}

int taimer_animacji(int &_t, int &_s, int liczba_klatek, int odswierzanie)
{

	if (_t == odswierzanie) {
		_s++;
		_t = 0;
	}
	if (_s > liczba_klatek - 1)_s = 0;
	_t++;
	return _s;
}

int taimer(Uint32 czas, int liczba_klatek, Uint32 odswierzanie)
{
	Uint32 uplyniety = SDL_GetTicks() - czas;
	int a = liczba_klatek-1;
	for (int i = 0; i < liczba_klatek; i++)
	{
		if (uplyniety > odswierzanie*i&&uplyniety < odswierzanie*(i + 1))
		{
			a = i;
			break;
		}
	}

	return a;
}

string zmiana_nazwy(int nr_podziemia)
{
	string a;
	
	return a;
}



//Przeciwnik:

Przeciwnik::Przeciwnik(string imie,double px,double py, double wy, double sze,double max,double zre,double inte,double obra,SDL_Texture *przec, vector<SDL_Rect> spr)
{
	aktywny = true;
	nazwa = imie;
	posX = px;
	posY = py;
	szerokosc = sze;
	wysokosc = wy;
	tekstura = przec;
	
	zrecznosc = zre;
	inteligencja=inte;
	spreje = spr;
	obrazenia = obra;
	zycie = max ;
	max_zycie = max;
}

Przeciwnik::~Przeciwnik()
{

}

void Przeciwnik::poruszanie(SDL_Renderer *render , Gracz &gracz1,int &_s,int &_t,SDL_Texture *zdrowie,Map mapa)
{
	
	if (aktywny == true) {
		if (tura == true && posX > gracz1.posX&&
			posX != gracz1.posX + gracz1.szerokosc &&
			posX != gracz1.posX - gracz1.szerokosc &&
			kolizja_przeciwnik(mapa,1)== true)
		{
			posX = posX - szerokosc;
			gracz1.tura = true;
			tura = false;
		}
		else if (tura == true && posX < gracz1.posX &&
			posX != gracz1.posX + gracz1.szerokosc &&
			posX != gracz1.posX - gracz1.szerokosc&&
			kolizja_przeciwnik(mapa, 2) == true)
		{
			posX = posX + szerokosc;
			gracz1.tura = true;
			tura = false;
		}
		else if (tura == true && posY < gracz1.posY&&
			posY != gracz1.posY + gracz1.wysokosc &&
			posY != gracz1.posY - gracz1.wysokosc &&
			kolizja_przeciwnik(mapa, 4) == true)
		{
			posY = posY + szerokosc;
			gracz1.tura = true;
			tura = false;
		}
		else if (tura == true && posY > gracz1.posY&& posY != gracz1.posY + gracz1.wysokosc &&
			posY != gracz1.posY - gracz1.wysokosc&&
			kolizja_przeciwnik(mapa, 3) == true)
		{
			posY = posY - szerokosc;
			gracz1.tura = true;
			tura = false;
		}
	}
}
bool Przeciwnik::kolizja_przeciwnik(Map mapa, int a)
{
	bool niemoszna = false;
	if (a == 1) {
		for (auto i = mapa.pokoje.begin(); i != mapa.pokoje.end(); i++)
		{
			for (auto j = (*i)->kafelki.begin(); j != (*i)->kafelki.end(); j++)
			{
				if ((*j)->posX == posX - szerokosc && (*j)->posY == posY && (*j)->chodzonosc == true)niemoszna = true;
			}
		}
	}
	if (a == 2) {
		for (auto i = mapa.pokoje.begin(); i != mapa.pokoje.end(); i++)
		{
			for (auto j = (*i)->kafelki.begin(); j != (*i)->kafelki.end(); j++)
			{
				if ((*j)->posX == posX + szerokosc && (*j)->posY == posY && (*j)->chodzonosc == true)niemoszna = true;
			}
		}
	}
	if (a == 3) {
		for (auto i = mapa.pokoje.begin(); i != mapa.pokoje.end(); i++)
		{
			for (auto j = (*i)->kafelki.begin(); j != (*i)->kafelki.end(); j++)
			{
				if ((*j)->posY == posY - szerokosc && (*j)->posX == posX && (*j)->chodzonosc == true)niemoszna = true;
			}
		}
	}
	if (a == 4) {
		for (auto i = mapa.pokoje.begin(); i != mapa.pokoje.end(); i++)
		{
			for (auto j = (*i)->kafelki.begin(); j != (*i)->kafelki.end(); j++)
			{
				if ((*j)->posY == posY + szerokosc && (*j)->posX == posX && (*j)->chodzonosc == true)niemoszna = true;
			}
		}
	}
	return niemoszna;
}
void Przeciwnik::atak(Gracz &gracz, Mix_Chunk *m_obrazenia)
{
	
	
	if (tura == true &&
		(int)posX >= (int)gracz.posX - (int)gracz.szerokosc&&
		(int)posX<=(int)gracz.posX + (int)gracz.szerokosc && 
		(int)posY>=(int)gracz.posY - (int)gracz.wysokosc&&
		(int)posY <= (int)gracz.posY + (int)gracz.wysokosc&&
		aktywny==true&&zycie>0)
	{
		
		if (rand() % 100<(100-gracz.zrecznosc))
		{
			gracz.obr.x = gracz.posX; gracz.obr.y = gracz.posY + 10;
			int zadane;
			if (0 > obrazenia - gracz.ochrona) zadane = 0;
			else zadane = obrazenia - gracz.ochrona;
			gracz.zycie -= zadane;
			gracz.przedchwila_zadane_obrazenia =zadane;
			gracz.b_ciecia = true;
			gracz.animacja_ciecia = SDL_GetTicks();
			Mix_PlayChannel(1, m_obrazenia, 0);
		}
		else
		{
			gracz.b_miss = true;
			gracz.animacja_miss = SDL_GetTicks();
		}
		
		
		
		
		
		//Podpalenie:
		if (rand() % 4 == 0)
		{
			gracz.podpalenie = true;
			gracz.licznik_podpalenia = 8;
			gracz.tura_wczasie_nalzoenia = gracz.tura;
		}

		
		gracz.tura = true;
		tura = false;
	}
}

void Przeciwnik::update(SDL_Renderer *render, int &_s, int &_t,SDL_Texture *zdrowie,SDL_Texture *ciecie,TTF_Font *arial)
{
	if (aktywny == true)
	{
		if (zycie <= 0)aktywny = false;

		SDL_Rect rect;
		
		rect.x = posX;
		rect.y = posY;
		rect.w = wysokosc;
		rect.h = szerokosc;
		
		obr.w = szerokosc/2;
		obr.h =wysokosc/2;

		SDL_RenderCopy(render, tekstura, &spreje[taimer_animacji(_t, _s, 4, 20)], &rect);
		
	//Animacja: 
		animacje_ataku(render, ciecie, arial);
	//Pasek Zdrowia:	
		rect.y -= 10;
		rect.h = wysokosc / 10;
		rect.w = szerokosc * ((double)zycie / (double)max_zycie);
		SDL_RenderCopy(render, zdrowie, NULL, &rect);
		
	}
}
void Przeciwnik::animacje_ataku(SDL_Renderer *render, SDL_Texture *tekstura_ataku,TTF_Font *arial)
{
	SDL_Rect rect;

	rect.x = posX;
	rect.y = posY;
	rect.w = wysokosc;
	rect.h = szerokosc;

	obr.w = 30;
	obr.h = 50;

	
	SDL_Texture *na = napis(255, 0, 0, arial, render, to_string(przedchwila_zadane_obrazenia));
	//Animacja Ciecia:
	if (b_ciecia == true)
	{

		SDL_RenderCopy(render, tekstura_ataku, &ustawianie_rect_spraj(60, 40, 6)[taimer(animacja_ciecia, 6, 80)], &rect);

		obr.x += 2;
		obr.y -= 2;
		SDL_RenderCopy(render, na, NULL, &obr);
		if (Uint32 i = SDL_GetTicks() - animacja_ciecia > 80 * 6)
		{
			
			b_ciecia = false;
			
		}
	}
	//Animacja Ciecia:
	if (b_miss == true)
	{
		na = napis(255, 255, 200, arial, render, "UNIK");

		SDL_RenderCopy(render, na, NULL, &rect);
		if (Uint32 i = SDL_GetTicks() - animacja_miss > 1000)
		{

			b_miss = false;
			
		}
	}
	SDL_DestroyTexture(na);
}

void Przeciwnik::potion(char rodzaj)
{
	if (rodzaj=='u')
	{
		zycie += (int)(max_zycie * 0.25);
		if (zycie> max_zycie)zycie = max_zycie;
	}
	if (rodzaj == 'p')
	{
		podpalenie = true;
	}
	if (rodzaj == 'z')
	{
		zamrozenie = true;
	}
	if (rodzaj == 's')
	{
		obrazenia += 1;
		
	}
}
void Przeciwnik::skalowanie(double przesuniecieX, double przesuniecieY, double skala)
{
	posX += przesuniecieX;
	posY += przesuniecieY;
	posX *= skala;
	posY *= skala;

	szerokosc *= skala;
	wysokosc *= skala;
}

//Szczury:



void Szczur::update(SDL_Renderer *render, int &_s, int &_t, SDL_Texture *zdrowie, SDL_Texture *ciecie, TTF_Font *arial)
{
	if (aktywny == true)
	{
		if (zycie <= 0)aktywny = false;

		SDL_Rect rect;

		rect.x = posX;
		rect.y = posY; 
		rect.w = wysokosc;
		rect.h = szerokosc;

		obr.w = szerokosc / 2;
		obr.h = wysokosc / 2;

		SDL_RenderCopy(render, tekstura, &spreje[taimer_animacji(_t, _s, 3, 100)], &rect);

		//Animacja: 
		animacje_ataku(render, ciecie, arial);
		//Pasek Zdrowia:	
		rect.y -= 10;
		rect.h = wysokosc / 10;
		rect.w = szerokosc * ((double)zycie / (double)max_zycie);
		SDL_RenderCopy(render, zdrowie, NULL, &rect);

	}
}

void Ogien::update(SDL_Renderer *render, int &_s, int &_t, SDL_Texture *zdrowie, SDL_Texture *ciecie, TTF_Font *arial)
{
	if (aktywny == true)
	{
		if (zycie <= 0)aktywny = false;

		SDL_Rect rect;

		rect.x = posX;
		rect.y = posY;
		rect.w = wysokosc;
		rect.h = szerokosc;

		obr.w = szerokosc / 2;
		obr.h = wysokosc / 2;

		SDL_RenderCopy(render, tekstura, &spreje[taimer_animacji(_t, _s, 2, 100)], &rect);

		//Animacja: 
		animacje_ataku(render, ciecie, arial);
		//Pasek Zdrowia:	
		rect.y -= 10;
		rect.h = wysokosc / 10;
		rect.w = szerokosc * ((double)zycie / (double)max_zycie);
		SDL_RenderCopy(render, zdrowie, NULL, &rect);

	}
}

//Przedmiot:

Przedmiot::Przedmiot(string imie, double px, double py, double sze, double wy,double obraz,double ochr, SDL_Texture *teks,SDL_Texture *teks_r,bool akt,char rodz)
{
	nazwa = imie;
	posX = px;
	posY = py;
	szerokosc = sze;
	wysokosc = wy;
	tekstura = teks;
	tekstura_r = teks_r;
	aktywny = akt;
	rodzaj = rodz;
	obrazenia = obraz;
	ochrona = ochr;
	
	zucane = false;
	wymagana_sila = rand() % 5;
	if (rodzaj == 'm')
	{
		zrecznosc = 2 + rand() % 10;
		sila = 0;
		inteligencja = 0;
	}
	if (rodzaj == 't')
	{
		 sila = 2 + rand() % 10;
		 zrecznosc = 0;
		 inteligencja = 0;
	}
	if (rodzaj == 'z')
	{
		sila = 2 + rand() % 10;
		zrecznosc = 0;
		inteligencja = 0;
	}
}

Przedmiot::Przedmiot()
{
	nazwa = "";
	posX = NULL;
	posY = NULL;
	szerokosc = NULL;
	wysokosc = NULL;
	tekstura = NULL;
	tekstura_r = NULL;
	aktywny = NULL;
	rodzaj = NULL;
	sila = 0;
	obrazenia = 0;
	zrecznosc = 0;
	inteligencja = 0;
	wymagana_sila = 0;
}

Przedmiot::~Przedmiot()
{

}

 void Przedmiot::update(SDL_Renderer *render)
{
	if (aktywny == true)
	{
		SDL_Rect rect;
		rect.x = posX;
		rect.y = posY;
		rect.w = wysokosc;
		rect.h = szerokosc;
		SDL_RenderCopy(render, tekstura, NULL, &rect);
	}
}

 void Przedmiot::okno_informacji(SDL_Renderer* render, SDL_Texture *tekst,TTF_Font *font, double px, double py)
 {
	 SDL_Rect rect;
	 rect.x = px;
	 rect.y = py;
	 rect.w =400 ;
	 rect.h =200 ;
	 SDL_RenderCopy(render, tekst, NULL, &rect);
	 rect.w = 100;
	 rect.h = 100;
	 rect.x = px+35;
	 rect.y = py+65;
	 SDL_RenderCopy(render, tekstura, NULL, &rect);
	 rect.h = 40;
	 rect.w = to_string((int)wymagana_sila).size() * 30;
	 rect.x = px + 35+70;
	 rect.y = py + 65;
	 SDL_Texture *na = napis(255, 255, 255, font, render,":"+ to_string((int)wymagana_sila));
	 SDL_RenderCopy(render, na, NULL, &rect);
	
	 rect.h = 20;
	 rect.w = nazwa.size()*10;
	 rect.x = px+205-(nazwa.size()*5);
	 rect.y = py+20;
	 na = napis(0, 0, 0, font, render, nazwa);
	 SDL_RenderCopy(render, na, NULL, &rect);
	 SDL_DestroyTexture(na);
	 if (obrazenia != 0)
	 {
		 rect.h = 30;
		 rect.w = 70;
		 rect.x =px+ 220;
		 rect.y = py+60;
		 na = napis(255, 155, 0, font, render, "Obr: " + to_string((int)obrazenia));
		 SDL_RenderCopy(render,na,NULL,&rect);
		 SDL_DestroyTexture(na);
	 }
	 else if (ochrona != 0|| rodzaj_kalsy() == "Zbroja")
	 {
		 rect.h = 30;
		 rect.w = 70;
		 rect.x = px + 220;
		 rect.y = py + 60;
		 na = napis(20, 0, 255, font, render, "Ochr: " + to_string((int)ochrona));
		 SDL_RenderCopy(render , na , NULL, &rect);
		 SDL_DestroyTexture(na);
	 }
	 int i = 0;
	 if (sila != 0)
		 {
			 rect.h = 20;
			 rect.w = 60;
			 rect.x = px + 215;
			 rect.y = py +100+(20*i);
			 na = napis(255, 0, 20, font, render, "+" + to_string((int)sila) + " sily");
			 SDL_RenderCopy(render,na , NULL, &rect);
			 i++;
			 SDL_DestroyTexture(na);
		 }
	 if (zrecznosc != 0)
		 {
			 rect.h = 20;
			 rect.w = 100;
			 rect.x = px + 205;
			 rect.y = py + 100 + (20 * i);
			 if(zrecznosc>=0)na = napis(20, 255, 25, font, render, "+" + to_string((int)zrecznosc) + " zrecznosci");
			 else na = napis(255, 155, 0, font, render, to_string((int)zrecznosc) + " zrecznosci");
			 SDL_RenderCopy(render, na , NULL, &rect);
			 i++;
			 SDL_DestroyTexture(na);
		 }
	
	 if (inteligencja != 0)
	 {
		 rect.h = 20;
		 rect.w = 100;
		 rect.x = px + 205;
		 rect.y = py + 100 + (20 * i);
		 na = napis(255, 100, 255, font, render, "+" + to_string((int)inteligencja) + " inteligencji");
		 SDL_RenderCopy(render,na, NULL, &rect);
		 i++;
		 SDL_DestroyTexture(na);
	 }
	 SDL_DestroyTexture(na);
	
 }	
 void Przedmiot::skalowanie(double przesuniecieX, double przesuniecieY, double skala)
 {
	 posX += przesuniecieX;
	 posY += przesuniecieY;
	 posX *= skala;
	 posY *= skala;

	 szerokosc *= skala;
	 wysokosc *= skala;
 }

 //Potion:

Potion::Potion(string imie, double px, double py, double sze, double wy, SDL_Texture *teks, bool akt, char rodz,int amo,double obra)
{
	nazwa = imie;
	posX = px;
	posY = py;
	szerokosc = sze;
	wysokosc = wy;
	tekstura = teks;
	aktywny = akt;
	rodzaj = rodz;
	amunicja = amo;
	zucane = true;
	obrazenia = obra;
}
bool Potion::uzycie(Gracz &gracz)
{
	if (rodzaj == 's')gracz.sila++;
	if (rodzaj == 'd')gracz.zrecznosc++;
	if (rodzaj == 'i')gracz.inteligencja++;
	if (rodzaj == 'u') {
		gracz.zycie += (int)(0.25*gracz.max_zycie);
		if (gracz.zycie > gracz.max_zycie)gracz.zycie = gracz.max_zycie;
	}
	if (rodzaj == '1') {
		gracz.exp += 100;
		
	}
	if(rodzaj!='q')return true;
	else return false;
}
void Potion::update(SDL_Renderer *render)
{
	if (aktywny == true)
	{
		SDL_Rect rect;
		rect.x = posX;
		rect.y = posY;
		rect.w = wysokosc;
		rect.h = szerokosc;
		SDL_RenderCopy(render, tekstura, NULL, &rect);
	}
}


//Okno EQ:

Okno_eq::Okno_eq(double px, double py, double wy, double sze,bool przel,SDL_Texture *statystyki)
{
	posX = px;
	posY = py;
	szerokosc = sze;
	wysokosc = wy;
	prz_eq = przel;
	g_statystyki = statystyki;
}

Okno_eq::~Okno_eq()
{

}

void Okno_eq::update(SDL_Texture *tekstura, SDL_Texture *g_znacznik,SDL_Texture *g_okno_przedmiotu, TTF_Font*arial, SDL_Renderer *render,Gracz &gracz)
{
	if (prz_eq == true)
	{
		SDL_Rect rect;
		SDL_Rect rect_napisy;
		rect.x = posX;
		rect.y = posY;
		rect.w = wysokosc;
		rect.h = szerokosc;
		SDL_RenderCopy(render, tekstura, NULL, &rect);
		rect.x = 345-90;
		rect.y = 80-90;
		rect.w = 50;
		rect.h = 50;
		int i = 0;
		for (auto itr = gracz.ekwipunek.begin(); itr != gracz.ekwipunek.end();itr++)
		{
			rect.x = rect.x + 90;
			if (i % 4 == 0)
			{
				rect.y = rect.y + 90;
				rect.x = 345;
			}
			
			rect.w = 50;
			rect.h = 50;
			
			SDL_RenderCopy(render,(*itr)->tekstura, NULL, &rect);
			i++;
		}
		i = 0;
		rect.x = 160;
		rect.y =90;
		rect.w = 120;
		rect.h = 120;
		SDL_RenderCopy(render, gracz.tekstura, &ustawianie_rect_spraj(60, 40, 2)[0], &rect); //Render postaci na EQ
		
		SDL_RenderCopy(render, gracz.zalozone[0].tekstura_r, &ustawianie_rect_spraj(60, 40, 2)[0], &rect);//Render zbroi na postaci

		//SDL_RenderCopy(render, gracz.zalozone[2].tekstura_r, NULL, &rect);//Render tarczy na postaci
		
		SDL_RenderCopy(render, gracz.zalozone[1].tekstura_r, &ustawianie_rect_spraj(60, 40, 2)[0], &rect);//Render miecza na postaci

		//Chodzenie po UI:
		rect.x = znacznikX;
		rect.y = znacznikY;
		rect.w =szer_znacz;
		rect.h = szer_znacz;
			///W PRAWO:
		if (znacznikX == 249 && GetAsyncKeyState(VK_RIGHT))
		{
			szer_znacz = 100;
			znacznikX = 317;
			znacznikY = 59;
			SDL_Delay(200);
		}
		else if (znacznikX < 317 && GetAsyncKeyState(VK_RIGHT))
		{
			znacznikX += 60;
			SDL_Delay(200);
		}
		else if (GetAsyncKeyState(VK_RIGHT) && znacznikX >=317&&znacznikX<587)
		{
			znacznikX =znacznikX + 90;
			SDL_Delay(200);
		}
			///W LEWO:
		if (znacznikX == 317 && GetAsyncKeyState(VK_LEFT))
		{
			szer_znacz = 60;
			znacznikX = 249;
			znacznikY = 272;
			SDL_Delay(200);
		}
		else if (znacznikX < 317 && GetAsyncKeyState(VK_LEFT)&&znacznikX>129)
		{
			znacznikX -= 60;
			SDL_Delay(200);
		}
		else if (GetAsyncKeyState(VK_LEFT)&& znacznikX >317)
		{
			znacznikX = znacznikX - 90;
			SDL_Delay(200);
		}
		
		if (GetAsyncKeyState(VK_DOWN)&&znacznikX>=317)
		{
			znacznikY = znacznikY + 90;
			SDL_Delay(200);
		}
		if (GetAsyncKeyState(VK_UP) && znacznikX >= 317 && znacznikY > 59)
		{
			znacznikY = znacznikY - 90;
			SDL_Delay(200);
		}
		SDL_RenderCopy(render, g_znacznik, NULL, &rect);
	
		//Ekwipowanie:
		int p = 0; int p1 = 0;
		int j = 0;
		for (auto itr = gracz.ekwipunek.begin(); itr != gracz.ekwipunek.end(); itr++)
		{
			if (i != 0 && i % 4 == 0) {
				p++;
				j = 0;
			}
			if (znacznikX == 317 + (j * 90))
			{
				if (znacznikY==59+(p*90)) {
					
					(*itr)->okno_informacji(render, g_okno_przedmiotu, arial, 800, 500);
					
				}
			}
			
			
			if (znacznikX == 317 + (j * 90) && GetAsyncKeyState(VK_RETURN))
			{
				
				
				
				if (znacznikY == 59 + (p * 90)) {
					SDL_Delay(200);
					if ((*itr)->rodzaj == 'z'&& (*itr)->wymagana_sila<=gracz.sila)
					{


						if (gracz.zalozone[0].nazwa != "")
						{
							Przedmiot* tp;
							tp = new Przedmiot;
							*tp = gracz.zalozone[0];
							gracz.ekwipunek.push_back(tp);
							gracz.sila -= gracz.zalozone[0].sila;
							gracz.zrecznosc -= gracz.zalozone[0].zrecznosc;
							gracz.inteligencja -= gracz.zalozone[0].inteligencja;
							gracz.obrazenia -= gracz.zalozone[0].obrazenia;
							gracz.ochrona -= gracz.zalozone[0].ochrona;

						}



						gracz.zalozone[0] = *gracz.ekwipunek[i];
						gracz.sila += (*itr)->sila;
						gracz.zrecznosc += (*itr)->zrecznosc;
						gracz.inteligencja += (*itr)->inteligencja;
						gracz.obrazenia += (*itr)->obrazenia;
						gracz.ochrona += (*itr)->ochrona;


						itr = gracz.ekwipunek.erase(itr);
						break;
					}
					else if ((*itr)->rodzaj == 'm' && (*itr)->wymagana_sila <= gracz.sila)
					{


						if (gracz.zalozone[1].nazwa != "") {
							Przedmiot* tp;
							tp = new Przedmiot;
							*tp = gracz.zalozone[1];
							gracz.ekwipunek.push_back(tp);
							gracz.sila -= gracz.zalozone[1].sila;
							gracz.zrecznosc -= gracz.zalozone[1].zrecznosc;
							gracz.obrazenia -= gracz.zalozone[1].obrazenia;
							gracz.inteligencja -= gracz.zalozone[1].inteligencja;
							gracz.ochrona -= gracz.zalozone[1].ochrona;
						}
						gracz.zalozone[1] = *gracz.ekwipunek[i];
						gracz.sila += (*itr)->sila;
						gracz.zrecznosc += (*itr)->zrecznosc;
						gracz.inteligencja += (*itr)->inteligencja;
						gracz.obrazenia += (*itr)->obrazenia;
						gracz.ochrona += (*itr)->ochrona;


						itr = gracz.ekwipunek.erase(itr);
						break;

					}
					else if ((*itr)->rodzaj == 't' && (*itr)->wymagana_sila <= gracz.sila)
					{


						if (gracz.zalozone[2].nazwa != "") {
							Przedmiot* tp;
							tp = new Przedmiot;
							*tp = gracz.zalozone[2];
							gracz.ekwipunek.push_back(tp);
							gracz.sila -= gracz.zalozone[2].sila;
							gracz.obrazenia -= gracz.zalozone[2].obrazenia;
							gracz.zrecznosc -= gracz.zalozone[2].zrecznosc;
							gracz.inteligencja -= gracz.zalozone[2].inteligencja;
							gracz.ochrona -= gracz.zalozone[2].ochrona;
						}
						gracz.zalozone[2] = *gracz.ekwipunek[i];
						gracz.sila += (*itr)->sila;
						gracz.zrecznosc += (*itr)->zrecznosc;
						gracz.inteligencja += (*itr)->inteligencja;
						gracz.obrazenia += (*itr)->obrazenia;
						gracz.ochrona += (*itr)->ochrona;
						itr = gracz.ekwipunek.erase(itr);
						break;

					}
					///OKIENKO INFORMACJI O PRZEDMIOCIE:
					if ((*itr)->uzycie(gracz) == true)
					{
						itr = gracz.ekwipunek.erase(itr);
						break;
					}
				}
			}

			///Uruchamianie trybu zucania:
			if (GetAsyncKeyState(0x45) && (znacznikX == 317 + (j * 90)))
			{
				
				
				if (znacznikY == 59 + (p * 90)) 
				{
					SDL_Delay(200);
					if ((*itr)->zucane == true)
					{

						prz_zutu = true;
						prz_eq = false;
						polozenie_zutuX = gracz.posX;
						polozenie_zutuY = gracz.posY;
						zutka = *itr;
					}
				}
			}

			i++;
			j++;

		}
		//Wyswietlanie okienka zalozonych:
		if (znacznikX == 249 && gracz.zalozone[0].nazwa != "")
		{
			gracz.zalozone[0].okno_informacji(render, g_okno_przedmiotu, arial, 800, 500);
		}
		if (znacznikX == 189&&gracz.zalozone[2].nazwa!="")
		{
			gracz.zalozone[2].okno_informacji(render, g_okno_przedmiotu, arial, 800, 500);
		}
		if (znacznikX == 129 && gracz.zalozone[1].nazwa != "")
		{
			gracz.zalozone[1].okno_informacji(render, g_okno_przedmiotu, arial, 800, 500);
		}

		i = 0;
		statystyki(gracz, render, arial);
		//Render Zalozonych:
		for (int c = 0; c < gracz.zalozone.size(); c++)
		{
			if (gracz.zalozone[c].rodzaj == 'm')
			{
				rect.x = 134;
				rect.y = 275;
				rect.w = 50;
				rect.h = 50;
				SDL_RenderCopy(render, gracz.zalozone[c].tekstura, NULL, &rect);
			}
			if (gracz.zalozone[c].rodzaj == 't')
			{
				rect.x = 194;
				rect.y = 275;
				rect.w = 50;
				rect.h = 50;
				SDL_RenderCopy(render, gracz.zalozone[c].tekstura, NULL, &rect);
			}
			if (gracz.zalozone[c].rodzaj == 'z')
			{
				rect.x = 254;
				rect.y = 275;
				rect.w = 50;
				rect.h = 50;
				SDL_RenderCopy(render, gracz.zalozone[c].tekstura, NULL, &rect);
			}
		}
		
		
	}
}

void Okno_eq::sterowanie(Gracz gracz)
{
	
	if (GetAsyncKeyState(0x49) && gracz.tura == true)
	{
		SDL_Delay(100);
		if (prz_eq == true) prz_eq = false;
		else if (prz_eq == false) prz_eq = true;
		SDL_Delay(200);
	}
	
}

void Okno_eq::zucanie(Gracz &gracz, vector<Przeciwnik*> &przeciwniki,SDL_Renderer *render, vector<Przedmiot*> &przedmiksy,Map mapa)
{
	if (prz_zutu==true)
	{
		SDL_Rect rect;
		rect.x = polozenie_zutuX;
		rect.y = polozenie_zutuY;
		rect.h = gracz.wysokosc;
		rect.w = gracz.szerokosc;
		SDL_RenderCopy(render, zutka->tekstura, NULL, &rect);
		if (GetAsyncKeyState(0x45)&&prz_lotu==0)
		{
			SDL_Delay(100);
			prz_zutu = false;
		}
		if (GetAsyncKeyState(VK_UP)&&gracz.posX==polozenie_zutuX&&gracz.posY==polozenie_zutuY)
		{
			prz_lotu = 1;
		}
		if (GetAsyncKeyState(VK_DOWN) && gracz.posX == polozenie_zutuX && gracz.posY == polozenie_zutuY)
		{
			prz_lotu = -1;
		}
		if (GetAsyncKeyState(VK_RIGHT) && gracz.posX == polozenie_zutuX && gracz.posY == polozenie_zutuY)
		{
			prz_lotu = 2;
		}
		if (GetAsyncKeyState(VK_LEFT) && gracz.posX == polozenie_zutuX && gracz.posY == polozenie_zutuY)
		{
			prz_lotu = -2;
		}
		if (prz_lotu == 1) polozenie_zutuY-=10;
		if (prz_lotu == -1)polozenie_zutuY += 10 ;
		if (prz_lotu == 2)polozenie_zutuX += 10 ;
		if (prz_lotu == -2)polozenie_zutuX -= 10;
		
		for (int i = 0; i < przeciwniki.size(); i++)
		{
			if ( przeciwniki[i]->posX == polozenie_zutuX && przeciwniki[i]->posY == polozenie_zutuY && przeciwniki[i]->aktywny == true)
			{
				
				prz_zutu = false;

				for (auto itr = gracz.ekwipunek.begin(); itr != gracz.ekwipunek.end(); itr++)
				{
					if (*itr == zutka)
					{
						(*itr)->amunicja--;
						przeciwniki[i]->zycie -= (*itr)->obrazenia;
						przeciwniki[i]->potion((*itr)->rodzaj);
						if ((*itr)->amunicja <= 0)itr = gracz.ekwipunek.erase(itr);
						prz_lotu = 0;
						break;
					}
				} //usuwanie po trafieniu
				
			}
		}

		for (auto i = mapa.pokoje.begin(); i != mapa.pokoje.end(); i++)
		{
			for(auto j=(*i)->kafelki.begin();j!=(*i)->kafelki.end();j++)
			{ 
				if ((*j)->posX == polozenie_zutuX && (*j)->posY == polozenie_zutuY && (*j)->chodzonosc == false)
				{
					
					prz_zutu = false;

					for (auto itr = gracz.ekwipunek.begin(); itr != gracz.ekwipunek.end(); itr++)
					{
						std::cout << prz_lotu << std::endl;
						if (*itr == zutka&&prz_lotu==1)
						{
							(*itr)->amunicja--;
							Potion elo((*itr)->nazwa, polozenie_zutuX, polozenie_zutuY+ (*itr)->szerokosc, (*itr)->szerokosc, (*itr)->wysokosc, (*itr)->tekstura, true, (*itr)->rodzaj, 1, (*itr)->obrazenia);
							Przedmiot* tp;
							tp = new Potion;
							*tp = elo;
							przedmiksy.push_back(tp);
							if ((*itr)->amunicja <= 0)itr = gracz.ekwipunek.erase(itr);
							prz_lotu = 0;
							break;
						}
						if (*itr == zutka && prz_lotu == -1)
						{
							(*itr)->amunicja--;
							Potion elo((*itr)->nazwa, polozenie_zutuX, polozenie_zutuY - (*itr)->szerokosc, (*itr)->szerokosc, (*itr)->wysokosc, (*itr)->tekstura, true, (*itr)->rodzaj, 1, (*itr)->obrazenia);
							Przedmiot* tp;
							tp = new Potion;
							*tp = elo;
							przedmiksy.push_back(tp);
							if ((*itr)->amunicja <= 0)itr = gracz.ekwipunek.erase(itr);
							prz_lotu = 0;
							break;
						}
						if (*itr == zutka && prz_lotu == 2)
						{
							(*itr)->amunicja--;
							Potion elo((*itr)->nazwa, polozenie_zutuX - (*itr)->szerokosc, polozenie_zutuY , (*itr)->szerokosc, (*itr)->wysokosc, (*itr)->tekstura, true, (*itr)->rodzaj, 1, (*itr)->obrazenia);
							Przedmiot* tp;
							tp = new Potion;
							*tp = elo;
							przedmiksy.push_back(tp);
							if ((*itr)->amunicja <= 0)itr = gracz.ekwipunek.erase(itr);
							prz_lotu = 0;
							break;
						}
						if (*itr == zutka && prz_lotu == -2)
						{
							(*itr)->amunicja--;
							Potion elo((*itr)->nazwa, polozenie_zutuX + (*itr)->szerokosc, polozenie_zutuY , (*itr)->szerokosc, (*itr)->wysokosc, (*itr)->tekstura, true, (*itr)->rodzaj, 1, (*itr)->obrazenia);
							Przedmiot* tp;
							tp = new Potion;
							*tp = elo;
							przedmiksy.push_back(tp);
							if ((*itr)->amunicja <= 0)itr = gracz.ekwipunek.erase(itr);
							prz_lotu = 0;
							break;
						}
					} 
					//usuwanie po trafieniu
				}
			}
		}

		if ((polozenie_zutuX == gracz.posX+(5*gracz.szerokosc))||( polozenie_zutuX == gracz.posX-(5 * gracz.szerokosc)  ) || (polozenie_zutuY == gracz.posY - (5 * gracz.szerokosc)) || (polozenie_zutuY == gracz.posY + (5 * gracz.szerokosc)))
		{
			
			prz_zutu = false;
			for (auto itr = gracz.ekwipunek.begin(); itr != gracz.ekwipunek.end(); itr++)
			{
				if (*itr == zutka)
				{
					
					(*itr)->amunicja--;
					
					Potion elo((*itr)->nazwa, polozenie_zutuX, polozenie_zutuY, (*itr)->szerokosc, (*itr)->wysokosc, (*itr)->tekstura, true, (*itr)->rodzaj, 1, (*itr)->obrazenia);
					Przedmiot* tp;
					tp = new Potion;
					*tp = elo;
					przedmiksy.push_back(tp);
					
					if ((*itr)->amunicja <= 0)itr = gracz.ekwipunek.erase(itr);
					prz_lotu = 0;
					break;
				}
			}
		}
	}
}

void Okno_eq::statystyki(Gracz &gracz, SDL_Renderer *render,TTF_Font *arial)
{
	SDL_Rect rect;
	rect.x = posX+59;
	rect.y = posY+370;
	rect.h = 174;
	rect.w = 200;
	SDL_RenderCopy(render, g_statystyki, NULL, &rect);
	//Sila:
	SDL_Texture *na = napis(255, 255, 255, arial, render, to_string((int)gracz.sila));
	rect.x += 60;
	rect.y += 2;
	rect.w = to_string((int)gracz.sila).size()*10;
	rect.h = 20;
	SDL_RenderCopy(render, na, NULL, &rect);
	//Zrecznosc:
	na = napis(255, 255, 255, arial, render, to_string((int)gracz.zrecznosc));
	rect.y += 50;
	rect.w = to_string((int)gracz.zrecznosc).size() * 10;
	SDL_RenderCopy(render, na, NULL, &rect);
	SDL_DestroyTexture(na);
	//Inteligencja:
	na = napis(255, 255, 255, arial, render, to_string((int)gracz.inteligencja));
	rect.y += 50;
	rect.w = to_string((int)gracz.inteligencja).size() * 10;
	SDL_RenderCopy(render, na, NULL, &rect);
	SDL_DestroyTexture(na);
	//Max HP:
	na = napis(255, 255, 255, arial, render, to_string((int)gracz.max_zycie));
	rect.y += 50;
	rect.w = to_string((int)gracz.max_zycie).size() * 10;
	SDL_RenderCopy(render, na, NULL, &rect);
	SDL_DestroyTexture(na);
	//Obrona:
	na = napis(255, 255, 255, arial, render, to_string((int)gracz.ochrona));
	rect.y = posY +372;
	rect.x += 90;
	rect.w = to_string((int)gracz.ochrona).size() * 10;
	SDL_RenderCopy(render, na, NULL, &rect);
	SDL_DestroyTexture(na);
	//LVL:
	na= napis(0, 0, 0, arial, render, "LVL: "+ to_string((int)gracz.lvl));
	
	rect.h = 40;
	rect.x -= 110;
	rect.y -= 60;
	rect.w = (to_string((int)gracz.lvl).size()+5) * 20;
	SDL_RenderCopy(render, na, NULL, &rect);
	SDL_DestroyTexture(na);
	na = napis(0, 0, 0, arial, render, to_string((int)gracz.exp)+"/"+to_string((int)gracz.next_exp));
	rect.h = 20;
	rect.w = (to_string((int)gracz.exp).size()+ to_string((int)gracz.next_exp).size() + 1) * 10;
	rect.y += 40;
	SDL_RenderCopy(render, na, NULL, &rect);
	SDL_DestroyTexture(na);
}


//Gracz:

Gracz::Gracz(double px, double py, double wy, double sze,bool tur, SDL_Texture *gracz,SDL_Texture *animacja_chodu)
{
	posX = px;
	posY = py;
	szerokosc = sze;
	wysokosc = wy;
	tura = true;
	tekstura = gracz;
	sila=3;
	zrecznosc = 15;
	inteligencja = 0;
	zalozone.resize(3);
	chodzenie = animacja_chodu;
	zycie = 20;
	max_zycie = 20;
	obrazenia = 4;
	ochrona = 0;
	
}

Gracz::~Gracz()
{

}

void Gracz::update(SDL_Renderer *render,int &_s,int &_t,SDL_Texture *zdrowie,TTF_Font *arial,SDL_Texture *g_obrazenia)
{
	lvlup();
	SDL_Rect rect;
	rect.x = posX;
	rect.y = posY;
	rect.w = wysokosc;
	rect.h = szerokosc;
	SDL_RenderCopyEx(render, tekstura, &ustawianie_rect_spraj(60, 40, 2)[taimer_animacji(_t, _s, 2,40)], &rect, 0, NULL,flip);
	animacje_ataku(render, g_obrazenia, arial);
	SDL_RenderCopyEx(render, zalozone[0].tekstura_r, &ustawianie_rect_spraj(60, 40, 2)[taimer_animacji(_t, _s, 2, 40)], &rect, 0, NULL, flip);//Render zbroji na postaci
	SDL_RenderCopyEx(render, zalozone[1].tekstura_r, &ustawianie_rect_spraj(60, 40, 2)[taimer_animacji(_t, _s, 2, 40)], &rect, 0, NULL, flip);//Render miecza na postaci
	
	
	
	rect.y -= 10;
	rect.h = wysokosc/10;

	rect.w = szerokosc * ((double)zycie / (double)max_zycie);
	SDL_RenderCopy(render, zdrowie, NULL, &rect);


}

bool Gracz::przesuwanie_gracz(vector<Przeciwnik*> &przeciwniki, Map mapa, int a){
	bool gowno=true;
	if (a == 1)
	{
		for (auto i = przeciwniki.begin(); i != przeciwniki.end(); i++)
		{
			if (posX-szerokosc==(*i)->posX&&posY==(*i)->posY&&(*i)->aktywny==true)
			{
				gowno = false;
				
			}
		}
		
	}
	if (a == 2)
	{
		for (auto i = przeciwniki.begin(); i != przeciwniki.end(); i++)
		{
			if (posX + szerokosc == (*i)->posX&&posY == (*i)->posY&& (*i)->aktywny == true)
			{
				gowno = false;
				
			}
		}
	}
	if (a == 3)
	{
		for (auto i = przeciwniki.begin(); i != przeciwniki.end(); i++)
		{
			if (posY - szerokosc == (*i)->posY&&posX == (*i)->posX&& (*i)->aktywny == true)
			{
				gowno = false;
				
			}
		}
	}
	if (a == 4)
	{
		for (auto i = przeciwniki.begin(); i != przeciwniki.end(); i++)
		{
			if (posY+ szerokosc == (*i)->posY&&posX == (*i)->posX&& (*i)->aktywny == true)
			{
				gowno = false;
				
			}
		}
	}
	return gowno;
}

bool Gracz::kolizja_gracz(Map mapa, int a)
{
	bool niemoszna = false;
	if (a == 1) {
		for (auto i = mapa.pokoje.begin(); i != mapa.pokoje.end(); i++)
		{
			for (auto j = (*i)->kafelki.begin(); j != (*i)->kafelki.end(); j++)
			{
				if ((*j)->posX == posX - szerokosc && (*j)->posY == posY&&(*j)->chodzonosc==true)niemoszna = true;
			}
		}
	}
	if (a == 2) {
		for (auto i = mapa.pokoje.begin(); i != mapa.pokoje.end(); i++)
		{
			for (auto j = (*i)->kafelki.begin(); j != (*i)->kafelki.end(); j++)
			{
				if ((*j)->posX ==posX + szerokosc && (*j)->posY == posY && (*j)->chodzonosc == true)niemoszna = true;
			}
		}
	}
	if (a == 3) {
		for (auto i = mapa.pokoje.begin(); i != mapa.pokoje.end(); i++)
		{
			for (auto j = (*i)->kafelki.begin(); j != (*i)->kafelki.end(); j++)
			{
				if ((*j)->posY == posY - szerokosc && (*j)->posX == posX && (*j)->chodzonosc == true)niemoszna = true;
			}
		}
	}
	if (a == 4) {
		for (auto i = mapa.pokoje.begin(); i != mapa.pokoje.end(); i++)
		{
			for (auto j = (*i)->kafelki.begin(); j != (*i)->kafelki.end(); j++)
			{
				if ((*j)->posY == posY + szerokosc && (*j)->posX == posX && (*j)->chodzonosc == true)niemoszna = true;
			}
		}
	}
	return niemoszna;
}

void Gracz::poruszanie(Okno_eq okno_eq,vector<Przeciwnik*> &przeciwniki, Mix_Chunk *m_chodzenie, Map mapa)
{
	
		if (GetAsyncKeyState(VK_LEFT) && tura == true && okno_eq.prz_eq == false && wlaczanie_ataku == false&&okno_eq.prz_zutu==false)
		{
			
				if (przesuwanie_gracz(przeciwniki,  mapa,1)==true&&kolizja_gracz(mapa,1)==true) {
					
					posX -= szerokosc;
					flip = SDL_FLIP_HORIZONTAL;
					tura = false;
					for (int i = 0; i < przeciwniki.size(); i++)przeciwniki[i]->tura = true;
					Mix_PlayChannel(1, m_chodzenie, 0);
					SDL_Delay(155);
					

				}
			
		}
		//2.right
		if (GetAsyncKeyState(VK_RIGHT) && tura == true && okno_eq.prz_eq == false && wlaczanie_ataku == false && okno_eq.prz_zutu == false)
		{
			
				if (przesuwanie_gracz(przeciwniki, mapa, 2) == true && kolizja_gracz(mapa, 2)==true) {
					posX = posX + szerokosc;
					flip = SDL_FLIP_NONE;
					tura = false;
					for (int i = 0; i < przeciwniki.size(); i++)przeciwniki[i]->tura = true;
					Mix_PlayChannel(1, m_chodzenie, 0);
					SDL_Delay(155);
					
				}
			
		}
		//3.up
		if (GetAsyncKeyState(VK_UP) && tura == true && okno_eq.prz_eq == false && wlaczanie_ataku == false && okno_eq.prz_zutu == false)
		{
			
				if (przesuwanie_gracz(przeciwniki, mapa, 3) == true && kolizja_gracz(mapa, 3) == true) {
					posY = posY - szerokosc;
					tura = false;
					for (int i = 0; i < przeciwniki.size(); i++)przeciwniki[i]->tura = true;
					Mix_PlayChannel(1, m_chodzenie, 0);
					SDL_Delay(155);
					
				}
			
		}
		//4.down
		if (GetAsyncKeyState(VK_DOWN) && tura == true && okno_eq.prz_eq == false && wlaczanie_ataku == false && okno_eq.prz_zutu == false)
		{
			
				if (przesuwanie_gracz(przeciwniki, mapa, 4) == true && kolizja_gracz(mapa, 4) == true) {
					posY = posY + szerokosc;
					tura = false;
					for (int i = 0; i < przeciwniki.size(); i++)przeciwniki[i]->tura = true;
					Mix_PlayChannel(1, m_chodzenie, 0);
					SDL_Delay(155);
					
				}
			
		}
	
}

void Gracz::koniec(vector<Przeciwnik*> &przeciwnik)
{
	tura = true;
	
}

void Gracz::podnoszenie(vector<Przedmiot*> &przedmiot,SDL_Renderer* render,SDL_Texture* teks,TTF_Font* arial)
{
	for (auto itr = przedmiot.begin(); itr != przedmiot.end();itr++) {
		if ((int)posX == (int)(*itr)->posX && (int)posY == (int)(*itr)->posY&& (*itr)->aktywny == true)
		{
			(*itr)->okno_informacji(render, teks, arial, 700, 400);
			cout << "elo" << endl;
		}
		if ((int)posX == (int)(*itr)->posX && (int)posY == (int)(*itr)->posY&&GetAsyncKeyState(VK_SPACE))
		{

			if ((*itr)->aktywny == true)
			{

				ekwipunek.push_back((*itr));
				(*itr)->aktywny = false;
			}
		}
	}
}

void Gracz::atak_przycisk()
{

	if (GetAsyncKeyState(0x41) && tura == true)
	{
		if (wlaczanie_ataku == true)
		{
			wlaczanie_ataku = false;
		}
		else if (wlaczanie_ataku == false) {
			wlaczanie_ataku = true;
			znaczX = posX;
			znaczY = posY;
		}
		SDL_Delay(200);
		

	}
}

void Gracz::atak(vector<Przeciwnik*> &przeciwnik, SDL_Texture *g_znacznik, SDL_Texture *g_ciecie, SDL_Renderer *render,Mix_Chunk *m_ciecie)
{
	if (wlaczanie_ataku==true)
	{
		
		SDL_Rect rect;
		rect.x = znaczX;
		rect.y = znaczY;
		rect.w = szerokosc;
		rect.h = wysokosc;
	
		
		
		if (GetAsyncKeyState(VK_LEFT) && znaczX - szerokosc >= posX - szerokosc)
		{
			znaczX -= szerokosc;
			SDL_Delay(200);
		}
		if (GetAsyncKeyState(VK_RIGHT) && znaczX + szerokosc <= posX + szerokosc)
		{
			znaczX += szerokosc;
			SDL_Delay(200);
		}
		cout<< znaczY - wysokosc <<":"<< posY - wysokosc <<endl;
		if (GetAsyncKeyState(VK_UP) && znaczY - wysokosc >= posY - wysokosc)
		{
			znaczY -= szerokosc;
			SDL_Delay(200);
		}
		if (GetAsyncKeyState(VK_DOWN) && znaczY + wysokosc <= posY + wysokosc)
		{
			znaczY += szerokosc;
			SDL_Delay(200);
		}
		SDL_RenderCopy(render, g_znacznik, NULL, &rect);
		
		for (int i = 0; i < przeciwnik.size(); i++)
		{
			cout << znaczY << ":" << przeciwnik[i]->posX << endl;
			if (znaczX == przeciwnik[i]->posX && znaczY == przeciwnik[i]->posY&&przeciwnik[i]->aktywny == true && GetAsyncKeyState(VK_RETURN))
			{
				
				if (rand()%100<70+((int)zrecznosc-(int)(przeciwnik[i]->zrecznosc/2)))
				{
					przeciwnik[i]->obr.x = przeciwnik[i]->posX; przeciwnik[i]->obr.y = przeciwnik[i]->posY+10;
					przeciwnik[i]->zycie -= obrazenia;
					przeciwnik[i]->przedchwila_zadane_obrazenia = obrazenia;
					przeciwnik[i]->b_ciecia = true;
					przeciwnik[i]->animacja_ciecia = SDL_GetTicks();
					Mix_PlayChannel(-1, m_ciecie, 0);
					if (przeciwnik[i]->zycie <= 0)exp += przeciwnik[i]->wartosc_exp;
				}
				else
				{
					przeciwnik[i]->b_miss = true;
					przeciwnik[i]->animacja_miss = SDL_GetTicks();
				}
				wlaczanie_ataku = false;
				tura = false;
				
				

			}
			if (wlaczanie_ataku == false)
			{
				for (int i = 0; i < przeciwnik.size(); i++) {
					przeciwnik[i]->tura = true;
				}
			}
		}

	}
}

void Gracz::pauza(vector<Przeciwnik*> &przeciwnik)
{
	if (GetAsyncKeyState(0x50)) {
		SDL_Delay(200);
		tura = false;
		for (auto itr = przeciwnik.begin(); itr != przeciwnik.end(); itr++)
		{
			
				(*itr)->tura = true;
			
		}
	}
}

void Gracz::efekty_pasywne()
{
	
	if (podpalenie == true)
	{
		if (tura_wczasie_nalzoenia != tura)
		{
			cout << "podpalilo cie: " << licznik_podpalenia << endl;
			zycie -= 0.5;
			licznik_podpalenia--;
			tura_wczasie_nalzoenia = tura;
		}
		if (licznik_podpalenia <= 0)podpalenie = false;
	}
	
	if (stun == true)
	{

	}
}

void Gracz::animacje_ataku(SDL_Renderer *render, SDL_Texture *tekstura_ataku, TTF_Font *arial)
{
	SDL_Rect rect;

	rect.x = posX;
	rect.y = posY;
	rect.w = wysokosc;
	rect.h = szerokosc;

	obr.w = 0.6*szerokosc;
	obr.h = szerokosc;


	SDL_Texture *na = napis(255, 50, 50, arial, render, to_string(przedchwila_zadane_obrazenia));
	//Animacja Ciecia:
	if (b_ciecia == true)
	{

		//SDL_RenderCopy(render, tekstura_ataku, NULL, &rect);
		SDL_RenderCopyEx(render, tekstura_ataku, NULL, &rect, 0, NULL, flip);

		obr.x += 2;
		obr.y -= 2;
		SDL_RenderCopy(render, na, NULL, &obr);
		if (Uint32 i = SDL_GetTicks() - animacja_ciecia > 80 * 6)
		{

			b_ciecia = false;

		}
	}
	//Animacja Ciecia:
	if (b_miss == true)
	{
		na = napis(255, 255, 200, arial, render, "UNIK");

		SDL_RenderCopy(render, na, NULL, &rect);
		if (Uint32 i = SDL_GetTicks() - animacja_miss > 1000)
		{

			b_miss = false;

		}
	}
	SDL_DestroyTexture(na);
}

void Gracz::skalowanie(double przesuniecieX,double przesuniecieY,double skala)
{
	posX += przesuniecieX;
	posY += przesuniecieY;
	posX *= skala;
	posY *= skala;

	szerokosc *= skala;
	wysokosc *= skala;
}

void Gracz::usuwanie_mgly(Map &mapa,SDL_Texture* g_mgla)
{
	for (auto i = mapa.pokoje.begin(); i != mapa.pokoje.end(); i++) {
		for (auto j = (*i)->kafelki.begin(); j != (*i)->kafelki.end(); j++) {
			if ((posX <= (*j)->posX + 3 * szerokosc&&posY == (*j)->posY) &&
				(posX >= (*j)->posX - 3 * szerokosc) ||
				(posY <= (*j)->posY + 3 * szerokosc&&posX == (*j)->posX) &&
				(posY >= (*j)->posY - 3 * szerokosc) ||
				(posX <= (*j)->posX + 2 * szerokosc&& posX >= (*j)->posX - 2 * szerokosc&&(posY == (*j)->posY-szerokosc|| posY == (*j)->posY+szerokosc))||
				(posX <= (*j)->posX +  szerokosc&& posX >= (*j)->posX -  szerokosc && (posY == (*j)->posY - 2*szerokosc || posY == (*j)->posY + 2*szerokosc))) {
				(*j)->widoczny = false;
				(*j)->odwiedzony = true;
			}
			else if ((*j)->odwiedzony == true) {
				(*j)->widoczny = true;
				(*j)->tekstura = g_mgla;
			}
		}
	}
}

void Gracz::lvlup()
{
	
	if (exp >= next_exp)
	{
		max_zycie += 4;
		sila += 1;
		obrazenia += 1;
		exp = 0;
		next_exp += 50;
		lvl++;
	}
}

//UI:

UI::UI(double px, double py, double wys, double szer, SDL_Texture* g_in, SDL_Texture* g_zy, SDL_Texture* g_z_p)
{
	posX = px;
	posY = py;
	wysokosc = wys;
	szerokosc = szer;
	g_interfejs = g_in;
	g_zycie = g_zy;
	g_znacznik_podpalenia = g_z_p;
}

void UI::update(Gracz gracz,SDL_Renderer* render, TTF_Font*arial,int nr_pietra,string nazwa_pietra)
{
	SDL_Rect rect;
	SDL_Rect od;

	od.x = 0;
	od.y = 500-(500*((double)gracz.zycie/(double)gracz.max_zycie));
	od.w = 500;
	od.h = 500;



	rect.x = 5;
	rect.y = 505 + (190 - (190 * ((double)gracz.zycie / (double)gracz.max_zycie)));
	rect.w = 190;
	rect.h = 0 + (190 * ((double)gracz.zycie / (double)gracz.max_zycie));

	SDL_RenderCopy(render, g_zycie, &od, &rect);

	rect.x = posX;
	rect.y = posY;
	rect.w = szerokosc;
	rect.h = wysokosc;

	SDL_RenderCopy(render, g_interfejs, NULL, &rect);

	rect.x = 1085;
	rect.y = 610;
	rect.w = 60;
	rect.h = 60;
	

	SDL_RenderCopy(render, gracz.zalozone[1].tekstura, NULL, &rect);

	rect.x = 63;
	rect.y = 680;
	rect.w = 80;
	rect.h = 30;

	SDL_Texture*na = napis(255, 255, 255, arial, render, to_string((int)gracz.zycie) + "/" + to_string((int)gracz.max_zycie));

	SDL_RenderCopy(render,na, NULL, &rect);

	if (gracz.podpalenie == true)
	{
		rect.x = 200;
		rect.y = 650;
		rect.w = 32;
		rect.h = 32;

		SDL_RenderCopy(render, g_znacznik_podpalenia, NULL, &rect);
	}

	rect.x = 520;
	rect.y = 35;
	rect.w = nazwa_pietra.size()*20;
	rect.h = 30;
	
	na = napis(255, 255, 255, arial, render, nazwa_pietra);

	SDL_RenderCopy(render, na, NULL, &rect);

	rect.x = 520+20+(nazwa_pietra.size()*30);
	rect.y = 35;
	rect.w = to_string(nr_pietra).size() * 20;
	rect.h = 30;

	na = napis(255, 255, 255, arial, render, to_string(nr_pietra));

	SDL_RenderCopy(render, na, NULL, &rect);

	SDL_DestroyTexture(na);
	
	

}





















                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           