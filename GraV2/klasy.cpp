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



//Przeciwnik:

Przeciwnik::Przeciwnik(string imie,double px,double py, double wy, double sze,double sil,double zre,double inte,SDL_Texture *przec, vector<SDL_Rect> spr)
{
	aktywny = true;
	nazwa = imie;
	posX = px;
	posY = py;
	szerokosc = sze;
	wysokosc = wy;
	tekstura = przec;
	sila = sil;
	zrecznosc = zre;
	inteligencja=inte;
	spreje = spr;
	zycie = sila * 10;
	max_zycie = sila * 10;
}

Przeciwnik::~Przeciwnik()
{

}

void Przeciwnik::poruszanie(SDL_Renderer *render , Gracz &gracz1,int &_s,int &_t, int przesuniecieX, int przesuniecieY, double s, SDL_Texture *zdrowie)
{
	
	if (aktywny == true) {
		if (tura == true && (int)posX > (int)gracz1.posX&&
			(int)posX != (int)gracz1.posX + (int)gracz1.szerokosc &&
			(int)posX != (int)gracz1.posX - (int)gracz1.szerokosc)
		{
			posX = posX - szerokosc;
			gracz1.tura = true;
			tura = false;
		}
		else if (tura == true && (int)posX < (int)gracz1.posX &&
			(int)posX != (int)gracz1.posX + (int)gracz1.szerokosc &&
			(int)posX != (int)gracz1.posX - (int)gracz1.szerokosc)
		{
			posX = posX + szerokosc;
			gracz1.tura = true;
			tura = false;
		}
		else if (tura == true && (int)posY < (int)gracz1.posY&&
			(int)posY != (int)gracz1.posY + (int)gracz1.wysokosc &&
			(int)posY != (int)gracz1.posY - (int)gracz1.wysokosc)
		{
			posY = posY + szerokosc;
			gracz1.tura = true;
			tura = false;
		}
		else if (tura == true && (int)posY > (int)gracz1.posY&& (int)posY != (int)gracz1.posY + (int)gracz1.wysokosc &&
			(int)posY != (int)gracz1.posY - (int)gracz1.wysokosc)
		{
			posY = posY - szerokosc;
			gracz1.tura = true;
			tura = false;
		}
	}
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
		
		gracz.zycie -= zrecznosc*10;
		Mix_PlayChannel(-1, m_obrazenia, 0);
		gracz.tura = true;
		tura = false;
	}
}

void Przeciwnik::update(SDL_Renderer *render, int &_s, int &_t, int przesuniecieX, int przesuniecieY, double s, SDL_Texture *zdrowie,SDL_Texture *ciecie)
{
	if (aktywny == true)
	{
		if (zycie <= 0)aktywny = false;

		//skalowanie:
		posY += przesuniecieY;
		posX += przesuniecieX;
		posX *= s;
		posY *= s;
		szerokosc *= s;
		wysokosc *= s;


		SDL_Rect rect;
		rect.x = posX;
		rect.y = posY;
		rect.w = wysokosc;
		rect.h = szerokosc;
		SDL_RenderCopy(render, tekstura, &spreje[taimer_animacji(_t, _s, 4, 20)], &rect);
		
	//Animacja Ciecia:
		if (b_ciecia==true)
		{
			SDL_RenderCopy(render, ciecie, &ustawianie_rect_spraj(60,40,6)[taimer(animacja_ciecia,6,80)], &rect);
			if (Uint32 i=SDL_GetTicks() -animacja_ciecia > 80 * 6)
			{

				b_ciecia = false;
			}
		}
		
	//Pasek Zdrowia:	
		rect.y -= 10;
		rect.h = wysokosc / 10;
		rect.w = szerokosc * ((double)zycie / (double)max_zycie);
		SDL_RenderCopy(render, zdrowie, NULL, &rect);
		

	}
}

void Przeciwnik::potion(char rodzaj)
{
	if (rodzaj=='u')
	{
		zycie += max_zycie * 0.25;
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
		sila += 1;
		max_zycie = sila * 10;
	}
}

//Przedmiot:

Przedmiot::Przedmiot(string imie, double px, double py, double sze, double wy, SDL_Texture *teks,SDL_Texture *teks_r,bool akt,char rodz)
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
	zucane = false;
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
	zrecznosc = 0;
	inteligencja = 0;
}

Przedmiot::~Przedmiot()
{

}

 void Przedmiot::update(SDL_Renderer *render, int przesuniecieX, int przesuniecieY, double s)
{
	posY += przesuniecieY;
	posX += przesuniecieX;
	posX *= s;
	posY *= s;
	szerokosc *= s;
	wysokosc *= s;
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
		gracz.zycie += 0.25*gracz.max_zycie;
		if (gracz.zycie > gracz.max_zycie)gracz.zycie = gracz.max_zycie;
	}
	return true;
}
void Potion::update(SDL_Renderer *render, int przesuniecieX, int przesuniecieY, double s)
{
	
	posY += przesuniecieY;
	posX += przesuniecieX;
	posX *= s;
	posY *= s;
	szerokosc *= s;
	wysokosc *= s;
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

Okno_eq::Okno_eq(double px, double py, double wy, double sze,bool przel)
{
	posX = px;
	posY = py;
	szerokosc = sze;
	wysokosc = wy;
	prz_eq = przel;
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
		rect.x = 350-90;
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
				rect.x = 350;
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

		//SDL_RenderCopy(render, gracz.tekstura, NULL, &rect);//Render tarczy na postaci
		
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
		cout << znacznikX << endl;
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
					
					rect.w = 400;
					rect.h = 200;
					rect.x = 800;
					rect.y = 500;
					SDL_RenderCopy(render, g_okno_przedmiotu, NULL, &rect);
					rect.w = 100;
					rect.h = 100;
					rect.x = 835;
					rect.y = 565;
					SDL_RenderCopy(render,(*itr)->tekstura, NULL, &rect);
					rect_napisy.h = 20;
					rect_napisy.w = 150;
					rect_napisy.x = 920;
					rect_napisy.y = 520;
					SDL_RenderCopy(render, napis(255, 255, 255, arial, render, (*itr)->nazwa), NULL, &rect_napisy);
					//Sila:
					rect_napisy.h = 20;
					rect_napisy.w = 150;
					rect_napisy.x = 950;
					rect_napisy.y = 560;
					string tekstk2 = "Sila:";
					tekstk2 += to_string((*itr)->sila);
					char const* pchar = tekstk2.c_str();
					SDL_RenderCopy(render, napis(255, 255, 255, arial, render, pchar), NULL, &rect_napisy);
					//Zrecznosc:
					rect_napisy.y = 585;
					rect_napisy.w = 200;
					tekstk2 = "Zrecznosc:";
					tekstk2 += to_string((*itr)->zrecznosc);
					pchar = tekstk2.c_str();
					SDL_RenderCopy(render, napis(255, 255, 255, arial, render, pchar), NULL, &rect_napisy);
					//Inteligencja:
					rect_napisy.y = 610;
					tekstk2 = "Inteligencja:";
					tekstk2 += to_string((*itr)->inteligencja);
					pchar = tekstk2.c_str();
					SDL_RenderCopy(render, napis(255, 255, 255, arial, render, pchar), NULL, &rect_napisy);
				}
			}
			
			//Wyswietlanie zalozonych:
			if (znacznikX == 249)
			{
				rect.w = 400;
				rect.h = 200;
				rect.x = 800;
				rect.y = 500;
				SDL_RenderCopy(render, g_okno_przedmiotu, NULL, &rect);
				rect.w = 100;
				rect.h = 100;
				rect.x = 835;
				rect.y = 565;
				SDL_RenderCopy(render,gracz.zalozone[0].tekstura, NULL, &rect);
				rect_napisy.h = 20;
				rect_napisy.w = 150;
				rect_napisy.x = 920;
				rect_napisy.y = 520;
				SDL_RenderCopy(render, napis(255, 255, 255, arial, render, gracz.zalozone[0].nazwa), NULL, &rect_napisy);

				rect_napisy.h = 20;
				rect_napisy.w = 150;
				rect_napisy.x = 950;
				rect_napisy.y = 560;
				
				SDL_RenderCopy(render, napis(255, 255, 255, arial, render,"Sila: "+ to_string(gracz.zalozone[0].sila)), NULL, &rect_napisy);

				rect_napisy.y = 585;
				rect_napisy.w = 200;

				SDL_RenderCopy(render, napis(255, 255, 255, arial, render, "Zrecznosc: " + to_string(gracz.zalozone[0].zrecznosc)), NULL, &rect_napisy);

				rect_napisy.y = 610;

				SDL_RenderCopy(render, napis(255, 255, 255, arial, render, "Inteligencja" + to_string(gracz.zalozone[0].inteligencja)), NULL, &rect_napisy);
			}
			if (znacznikX == 189)
			{
				rect.w = 400;
				rect.h = 200;
				rect.x = 800;
				rect.y = 500;
				SDL_RenderCopy(render, g_okno_przedmiotu, NULL, &rect);
				rect.w = 100;
				rect.h = 100;
				rect.x = 835;
				rect.y = 565;
				SDL_RenderCopy(render, gracz.zalozone[2].tekstura, NULL, &rect);
				rect_napisy.h = 20;
				rect_napisy.w = 150;
				rect_napisy.x = 920;
				rect_napisy.y = 520;
				SDL_RenderCopy(render, napis(255, 255, 255, arial, render, gracz.zalozone[2].nazwa), NULL, &rect_napisy);

				rect_napisy.h = 20;
				rect_napisy.w = 150;
				rect_napisy.x = 950;
				rect_napisy.y = 560;

				SDL_RenderCopy(render, napis(255, 255, 255, arial, render, "Sila: " + to_string(gracz.zalozone[2].sila)), NULL, &rect_napisy);

				rect_napisy.y = 585;
				rect_napisy.w = 200;

				SDL_RenderCopy(render, napis(255, 255, 255, arial, render, "Zrecznosc: " + to_string(gracz.zalozone[2].zrecznosc)), NULL, &rect_napisy);

				rect_napisy.y = 610;

				SDL_RenderCopy(render, napis(255, 255, 255, arial, render, "Inteligencja" + to_string(gracz.zalozone[2].inteligencja)), NULL, &rect_napisy);
			}
			if (znacznikX == 129)
			{
				rect.w = 400;
				rect.h = 200;
				rect.x = 800;
				rect.y = 500;
				SDL_RenderCopy(render, g_okno_przedmiotu, NULL, &rect);
				rect.w = 100;
				rect.h = 100;
				rect.x = 835;
				rect.y = 565;
				SDL_RenderCopy(render, gracz.zalozone[1].tekstura, NULL, &rect);
				rect_napisy.h = 20;
				rect_napisy.w = 150;
				rect_napisy.x = 920;
				rect_napisy.y = 520;
				SDL_RenderCopy(render, napis(255, 255, 255, arial, render, gracz.zalozone[1].nazwa), NULL, &rect_napisy);

				rect_napisy.h = 20;
				rect_napisy.w = 150;
				rect_napisy.x = 950;
				rect_napisy.y = 560;

				SDL_RenderCopy(render, napis(255, 255, 255, arial, render, "Sila: " + to_string(gracz.zalozone[1].sila)), NULL, &rect_napisy);

				rect_napisy.y = 585;
				rect_napisy.w = 200;

				SDL_RenderCopy(render, napis(255, 255, 255, arial, render, "Zrecznosc: " + to_string(gracz.zalozone[1].zrecznosc)), NULL, &rect_napisy);

				rect_napisy.y = 610;

				SDL_RenderCopy(render, napis(255, 255, 255, arial, render, "Inteligencja" + to_string(gracz.zalozone[1].inteligencja)), NULL, &rect_napisy);
			}
			
			if (znacznikX == 317 + (j * 90) && GetAsyncKeyState(VK_RETURN))
			{
				
				
				
				if (znacznikY == 59 + (p * 90)) {
					SDL_Delay(200);
					if ((*itr)->rodzaj == 'z')
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
						}



						gracz.zalozone[0] = *gracz.ekwipunek[i];
						gracz.sila += (*itr)->sila;
						gracz.zrecznosc += (*itr)->zrecznosc;
						gracz.inteligencja += (*itr)->inteligencja;
						gracz.max_zycie = 10 * gracz.sila;


						itr = gracz.ekwipunek.erase(itr);
						break;
					}
					else if ((*itr)->rodzaj == 'm')
					{


						if (gracz.zalozone[1].nazwa != "") {
							Przedmiot* tp;
							tp = new Przedmiot;
							*tp = gracz.zalozone[1];
							gracz.ekwipunek.push_back(tp);
							gracz.sila -= gracz.zalozone[1].sila;
							gracz.zrecznosc -= gracz.zalozone[1].zrecznosc;
							gracz.inteligencja -= gracz.zalozone[1].inteligencja;
						}
						gracz.zalozone[1] = *gracz.ekwipunek[i];
						gracz.sila += (*itr)->sila;
						gracz.zrecznosc += (*itr)->zrecznosc;
						gracz.inteligencja += (*itr)->inteligencja;
						gracz.max_zycie = 10 * gracz.sila;



						itr = gracz.ekwipunek.erase(itr);
						break;

					}
					else if ((*itr)->rodzaj == 't')
					{


						if (gracz.zalozone[2].nazwa != "") {
							Przedmiot* tp;
							tp = new Przedmiot;
							*tp = gracz.zalozone[2];
							gracz.ekwipunek.push_back(tp);
							gracz.sila -= gracz.zalozone[2].sila;
							gracz.zrecznosc -= gracz.zalozone[2].zrecznosc;
							gracz.inteligencja -= gracz.zalozone[2].inteligencja;
						}
						gracz.zalozone[2] = *gracz.ekwipunek[i];
						gracz.sila += (*itr)->sila;
						gracz.zrecznosc += (*itr)->zrecznosc;
						gracz.inteligencja += (*itr)->inteligencja;
						gracz.max_zycie = 10 * gracz.sila;

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
		i = 0;
		//Render Zalozonych:
		for (int c = 0; c < gracz.zalozone.size(); c++)
		{
			if (gracz.zalozone[c].rodzaj == 'm')
			{
				rect.x = 150;
				rect.y = 275;
				rect.w = 50;
				rect.h = 50;
				SDL_RenderCopy(render, gracz.zalozone[c].tekstura, NULL, &rect);
			}
			if (gracz.zalozone[c].rodzaj == 't')
			{
				rect.x = 205;
				rect.y = 275;
				rect.w = 50;
				rect.h = 50;
				SDL_RenderCopy(render, gracz.zalozone[c].tekstura, NULL, &rect);
			}
			if (gracz.zalozone[c].rodzaj == 'z')
			{
				rect.x = 255;
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

void Okno_eq::zucanie(Gracz &gracz, vector<Przeciwnik> &przeciwniki,SDL_Renderer *render,double s, vector<Przedmiot*> &przedmiksy)
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
			if ( przeciwniki[i].posX == polozenie_zutuX && przeciwniki[i].posY == polozenie_zutuY && przeciwniki[i].aktywny == true)
			{
				prz_lotu = 0;
				prz_zutu = false;

				for (auto itr = gracz.ekwipunek.begin(); itr != gracz.ekwipunek.end(); itr++)
				{
					if (*itr == zutka)
					{
						(*itr)->amunicja--;
						przeciwniki[i].zycie -= (*itr)->obrazenia;
						przeciwniki[i].potion((*itr)->rodzaj);
						if ((*itr)->amunicja <= 0)itr = gracz.ekwipunek.erase(itr);
						
						break;
					}
				} //usuwanie po trafieniu
				
			}
		}
		if ((polozenie_zutuX == gracz.posX+(500*s))||( polozenie_zutuX == gracz.posX-(500 * s)  ) || (polozenie_zutuY == gracz.posY - (500 * s)) || (polozenie_zutuY == gracz.posY + (500 * s)))
		{
			prz_lotu = 0;
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
					break;
				}
			}
		}
	}
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
	sila=10;
	zrecznosc = 10;
	inteligencja = 10;
	zalozone.resize(3);
	chodzenie = animacja_chodu;
	zycie = (int)sila * 10;
	max_zycie = (int)sila * 10;
	
}

Gracz::~Gracz()
{

}

void Gracz::update(SDL_Renderer *render,int &_s,int &_t,int przesuniecieX,int przesuniecieY, double s,SDL_Texture *zdrowie)
{
	
	posY += przesuniecieY;
	posX += przesuniecieX;
	posX *= s;
	posY *= s;
	szerokosc *= s;
	wysokosc *= s;
	max_zycie = sila * 10;
	SDL_Rect rect;
	rect.x = posX;
	rect.y = posY;
	rect.w = wysokosc;
	rect.h = szerokosc;
	SDL_RenderCopyEx(render, tekstura, &ustawianie_rect_spraj(60, 40, 2)[taimer_animacji(_t, _s, 2,40)], &rect, 0, NULL,flip);
	SDL_RenderCopyEx(render, zalozone[0].tekstura_r, &ustawianie_rect_spraj(60, 40, 2)[taimer_animacji(_t, _s, 2, 40)], &rect, 0, NULL, flip);//Render zbroji na postaci
	SDL_RenderCopyEx(render, zalozone[1].tekstura_r, &ustawianie_rect_spraj(60, 40, 2)[taimer_animacji(_t, _s, 2, 40)], &rect, 0, NULL, flip);//Render miecza na postaci
	

	
	rect.y -= 10;
	rect.h = wysokosc/10;

	rect.w = szerokosc * ((double)zycie / (double)max_zycie);
	SDL_RenderCopy(render, zdrowie, NULL, &rect);
}

bool Gracz::przesuwanie_gracz(vector<Przeciwnik> &przeciwniki,int a) {
	bool gowno=true;
	if (a == 1)
	{
		for (auto i = przeciwniki.begin(); i != przeciwniki.end(); i++)
		{
			if (posX-szerokosc==i->posX&&posY==i->posY&&i->aktywny==true)
			{
				gowno = false;
				break;
			}
		}
	}
	if (a == 2)
	{
		for (auto i = przeciwniki.begin(); i != przeciwniki.end(); i++)
		{
			if (posX + szerokosc == i->posX&&posY == i->posY&&i->aktywny == true)
			{
				gowno = false;
				break;
			}
		}
	}
	if (a == 3)
	{
		for (auto i = przeciwniki.begin(); i != przeciwniki.end(); i++)
		{
			if (posY - szerokosc == i->posY&&posX == i->posX&&i->aktywny == true)
			{
				gowno = false;
				break;
			}
		}
	}
	if (a == 4)
	{
		for (auto i = przeciwniki.begin(); i != przeciwniki.end(); i++)
		{
			if (posY+ szerokosc == i->posY&&posX == i->posX&&i->aktywny == true)
			{
				gowno = false;
				break;
			}
		}
	}
	return gowno;
}

void Gracz::poruszanie(Okno_eq okno_eq,double s,vector<Przeciwnik> &przeciwniki, Mix_Chunk *m_chodzenie)
{
	
		if (GetAsyncKeyState(VK_LEFT) && tura == true && okno_eq.prz_eq == false && wlaczanie_ataku == false&&okno_eq.prz_zutu==false)
		{
			
				if (przesuwanie_gracz(przeciwniki,1)==true) {
					
					posX -= szerokosc;
					flip = SDL_FLIP_HORIZONTAL;
					tura = false;
					for (int i = 0; i < przeciwniki.size(); i++)przeciwniki[i].tura = true;
					Mix_PlayChannel(1, m_chodzenie, 0);
					SDL_Delay(155);
					

				}
			
		}
		//2.right
		if (GetAsyncKeyState(VK_RIGHT) && tura == true && okno_eq.prz_eq == false && wlaczanie_ataku == false && okno_eq.prz_zutu == false)
		{
			
				if (przesuwanie_gracz(przeciwniki, 2) == true) {
					posX = posX + szerokosc;
					flip = SDL_FLIP_NONE;
					tura = false;
					for (int i = 0; i < przeciwniki.size(); i++)przeciwniki[i].tura = true;
					Mix_PlayChannel(1, m_chodzenie, 0);
					SDL_Delay(155);
					
				}
			
		}
		//3.up
		if (GetAsyncKeyState(VK_UP) && tura == true && okno_eq.prz_eq == false && wlaczanie_ataku == false && okno_eq.prz_zutu == false)
		{
			
				if (przesuwanie_gracz(przeciwniki, 3) == true) {
					posY = posY - szerokosc;
					tura = false;
					for (int i = 0; i < przeciwniki.size(); i++)przeciwniki[i].tura = true;
					Mix_PlayChannel(1, m_chodzenie, 0);
					SDL_Delay(155);
					
				}
			
		}
		//4.down
		if (GetAsyncKeyState(VK_DOWN) && tura == true && okno_eq.prz_eq == false && wlaczanie_ataku == false && okno_eq.prz_zutu == false)
		{
			
				if (przesuwanie_gracz(przeciwniki, 4) == true) {
					posY = posY + szerokosc;
					tura = false;
					for (int i = 0; i < przeciwniki.size(); i++)przeciwniki[i].tura = true;
					Mix_PlayChannel(1, m_chodzenie, 0);
					SDL_Delay(155);
					
				}
			
		}
	
}

void Gracz::koniec(vector<Przeciwnik> &przeciwnik)
{
	tura = true;
	
}

void Gracz::podnoszenie(vector<Przedmiot*> &przedmiot)
{
	for (auto itr = przedmiot.begin(); itr != przedmiot.end();itr++) {
		if ((int)posX == (int)(*itr)->posX && (int)posY == (int)(*itr)->posY)
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

void Gracz::atak(vector<Przeciwnik> &przeciwnik, SDL_Texture *g_znacznik, SDL_Texture *g_ciecie, SDL_Renderer *render,Mix_Chunk *m_ciecie)
{
	if (wlaczanie_ataku==true)
	{
		
		SDL_Rect rect;
		rect.x = znaczX;
		rect.y = znaczY;
		rect.w = szerokosc;
		rect.h = wysokosc;
	
		
		
		if (GetAsyncKeyState(VK_LEFT) && rect.x - 100 >= posX - 100 )
		{
			znaczX -= szerokosc;
			SDL_Delay(200);
		}
		if (GetAsyncKeyState(VK_RIGHT) && rect.x + 100 <= posX + 100)
		{
			znaczX += szerokosc;
			SDL_Delay(200);
		}
		if (GetAsyncKeyState(VK_UP) &&rect.y- 100 >= posY - 100)
		{
			znaczY -= szerokosc;
			SDL_Delay(200);
		}
		if (GetAsyncKeyState(VK_DOWN) &&rect.y + 100 <= posY + 100)
		{
			znaczY += szerokosc;
			SDL_Delay(200);
		}
		SDL_RenderCopy(render, g_znacznik, NULL, &rect);
		
		for (int i = 0; i < przeciwnik.size(); i++)
		{
			if ((int)znaczX == (int)przeciwnik[i].posX && (int)znaczY == (int)przeciwnik[i].posY&&przeciwnik[i].aktywny == true && GetAsyncKeyState(VK_RETURN))
			{

				przeciwnik[i].zycie -= zrecznosc;
				przeciwnik[i].b_ciecia = true;
				przeciwnik[i].animacja_ciecia = SDL_GetTicks();
				Mix_PlayChannel(-1, m_ciecie, 0);
				wlaczanie_ataku = false;
				tura = false;
				
				

			}
			if (wlaczanie_ataku == false)
			{
				for (int i = 0; i < przeciwnik.size(); i++) {
					przeciwnik[i].tura = true;
				}
			}
		}

	}
}

void Gracz::pauza(vector<Przeciwnik> &przeciwnik)
{
	if (GetAsyncKeyState(0x50)) {
		SDL_Delay(200);
		tura = false;
		for (auto itr = przeciwnik.begin(); itr != przeciwnik.end(); itr++)
		{
			
				itr->tura = true;
			
		}
	}
}

void Gracz::efekty_pasywne()
{
	
	if (podpalenie == true)
	{
		
	}
	
	if (stun == true)
	{

	}
}
//UI:

UI::UI(double px, double py, double wys, double szer, SDL_Texture* g_in, SDL_Texture* g_zy)
{
	posX = px;
	posY = py;
	wysokosc = wys;
	szerokosc = szer;
	g_interfejs = g_in;
	g_zycie = g_zy;
}

void UI::update(Gracz gracz,SDL_Renderer* render, TTF_Font*arial)
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

	SDL_Texture*na = napis(255, 255, 255, arial, render, to_string(gracz.zycie) + "/" + to_string(gracz.max_zycie));

	SDL_RenderCopy(render,na, NULL, &rect);
	
	SDL_DestroyTexture(na);
	
	

}























//Mapa:

	///1.Pokoj1:

void cPokoj1::update(SDL_Renderer *render, int przesuniecieX, int przesuniecieY, double s) {
	posY += przesuniecieY;
	posX += przesuniecieX;
	posX *= s;
	posY *= s;
	szer *= s;
	wys *= s;
	SDL_Rect rect;
	rect.x = posX;
	rect.y = posY;
	rect.w = wys;
	rect.h = szer;
	SDL_RenderCopy(render, obr, NULL, &rect);

}

	///2.Pokoj2

void cPokoj2::update(SDL_Renderer *render, int przesuniecieX, int przesuniecieY, double s) {
	posY += przesuniecieY;
	posX += przesuniecieX;
	posX *= s;
	posY *= s;
	szer *= s;
	wys *= s;
	SDL_Rect rect;
	rect.x = posX;
	rect.y = posY;
	rect.w = wys;
	rect.h = szer;
	SDL_RenderCopy(render, obr, NULL, &rect);
}

	///3.Pokoj3

void cPokoj3::update(SDL_Renderer *render, int przesuniecieX, int przesuniecieY, double s) {
	posY += przesuniecieY;
	posX += przesuniecieX;
	posX *= s;
	posY *= s;
	szer *= s;
	wys *= s;
	SDL_Rect rect;
	rect.x = posX;
	rect.y = posY;
	rect.w = wys;
	rect.h = szer;
	SDL_RenderCopy(render, obr, NULL, &rect);
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      