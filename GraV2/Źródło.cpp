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
#include<list>
#include<string>
#include<cmath>

using namespace std;

#pragma region Okno render zdarzenia
SDL_Renderer *render;
SDL_Window *window;
SDL_Event zdarzenie;
SDL_Event zdarzenie2;
#pragma endregion  

SDL_Texture*loadTexture(string path)
{
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Gowno nie zaladowalo %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(render, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Gowno nie zaladowalo %s! SDL_image Error: %s\n", path.c_str(), SDL_GetError());
		}
		SDL_FreeSurface(loadedSurface);
	}
	return newTexture;
}

#pragma region Funkcje
		
class Klasa_fps
		{
		public:
			int fps_poczatek;
			int fps_koniec;
			int roznica;
			float delay;
			int fps = 60;

			void start()
			{
				fps_poczatek = clock();
			}
			void koniec()
			{
				fps_koniec = clock();
				roznica = fps_koniec - fps_poczatek;
				delay = (1000.0 / (float)fps) - roznica;
				if (delay > 0)
				{
					SDL_Delay(delay);
				}
			}
		};

list<Mapa*> g_mapy(int liczba_pomieszczen, SDL_Texture *obr1, SDL_Texture *obr2, SDL_Texture *obr3)
		{
			srand(time(NULL));
			list<Mapa*> k_mapy;

			Mapa* temp;
			
			int a = 0;
			int b = 0;
			int szerokosc = 0;
			for (int i = 0; i < liczba_pomieszczen; i++)
			{
				int los = rand() % 3;
			
				if (los == 0) 
				{
					temp = new cPokoj1(a,b, obr1);
					k_mapy.push_back(temp);
					if (szerokosc < 1000) {
						a += temp->szer ;
						szerokosc += temp->szer;
					}
					else {
						a = 0;
						b += temp->wys+100;
					}
					cout << "1mapa" << endl;
				}
				else if (los == 1)
				{
					temp = new cPokoj2(a, b , obr2);
					k_mapy.push_back(temp);
					if (szerokosc < 1000) {
						a += temp->szer ;
						szerokosc += temp->szer;
					}
					else {
						a = 0;
						b += temp->wys+100;
					}
					cout << "2mapa" << endl;
				}
				else if (los == 2)
				{
					temp = new cPokoj3(a , b, obr3);
					k_mapy.push_back(temp);
					if (szerokosc < 1000) {
						a += temp->szer;
						szerokosc += temp->szer;
					}
					else {
						a = 0;
						b += temp->wys+100;
					}
					cout << "3mapa" << endl;
				}
			}
			


			return k_mapy;
		}
string gen_nazw(char rodzaj)
		{
			string nazwa;
			srand(time(NULL));
			int a = rand() % 10;
			if (rodzaj == 'm')
			{
				switch (a)
				{
				case 0: nazwa = "Katana"; break;
				case 1: nazwa = "Lewiatan"; break;
				case 2: nazwa = "Mionir"; break;
				case 3: nazwa = "Szabla"; break;
				case 4: nazwa = "Elo"; break;
				case 5: nazwa = "Gowno a nie miecz"; break;
				case 6: nazwa = "Zadlo"; break;
				case 7: nazwa = "Szczerbiec"; break;
				case 8: nazwa = "Rapier"; break;
				case 9: nazwa = "Pustulka"; break;

				}
			}
			if (rodzaj == 'z')
			{

			}
			if (rodzaj == 't')
			{

			}
			srand(time(NULL));
			return nazwa;
		}
	
#pragma endregion

#pragma region Zmienne globalne
int scena = 0;
int klatka = 0;
bool zmiana_levelu = true;
int przesuniecieX=0;
int przesuniecieY=0;
double skala=1;
double gowno = 1;
SDL_Rect rect1, rect2, rect3;
list<Mapa*> mapka;
Klasa_fps fps;
//Taimery:

int t_szkielet = 0;
int s_szkielet = 0;

int t_postac = 0;
int s_postac = 0;

int mysz_x=500;
int mysz_y = 500;
#pragma endregion  


int main(int argc, char * args[])
{
	
	
	#pragma region Inicjalizacja i drfinicja okna i rendera
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_AUDIO);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	TTF_Init();
	

	window = SDL_CreateWindow("Gra", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)cout << "errro" << endl;
	#pragma endregion

	std::srand(time(NULL));



	//Pêtla wykonawcza gry:
	while (true)
	{
		
		if (scena == 0) 
		{
		#pragma region Tekstury
			SDL_Texture *tlo = loadTexture("Grafiki/Tlo.png");
			SDL_Texture *g_gracz = loadTexture("Grafiki/g_postac2.png");
			SDL_Texture *g_zbroja_r = loadTexture("Grafiki/g_zbroja_r.png");
			SDL_Texture *g_gnom = loadTexture("Grafiki/gnom.png");
			SDL_Texture *g_miecz = loadTexture("Grafiki/g_miecz.png");
			SDL_Texture *g_miecz_r = loadTexture("Grafiki/g_miecz_r.png");
			SDL_Texture *g_tarcza = loadTexture("Grafiki/g_tarcza.png");
			SDL_Texture *g_zbroja = loadTexture("Grafiki/g_zbroja.png");
			SDL_Texture *g_okno_ekwipunku = loadTexture("Grafiki/g_okno_ekwipunku.png");
			SDL_Texture *g_znacznik = loadTexture("Grafiki/g_znacznik.png");
			SDL_Texture *g_znacznik2 = loadTexture("Grafiki/g_znacznik2.png");
			SDL_Texture *g_okno_przedmiotu = loadTexture("Grafiki/g_okno_przedmiotu.png");
			SDL_Texture *g_szkielet = loadTexture("Grafiki/g_szkielet.png");
			SDL_Texture *g_gracz_chodzenie = loadTexture("Grafiki/g_postac_chodzenie.png");
			SDL_Texture *g_ciecie = loadTexture("Grafiki/g_ciecie.png");
			SDL_Texture *g_zdrowie = loadTexture("Grafiki/g_zdrowie.png");
			SDL_Texture *g_UI = loadTexture("Grafiki/g_UI.png");
			SDL_Texture *g_kulazycia = loadTexture("Grafiki/g_kulazycia.png");
			SDL_Texture *g_potion_zdrowia = loadTexture("Grafiki/g_potion_zdrowia.png");
		//Czcionki:
			TTF_Font*arial = TTF_OpenFont("arial.ttf", 30);
			#pragma endregion

		#pragma region Dzwieki
			Mix_Chunk *m_ciecie = Mix_LoadWAV("Dzwieki/m_ciecie.wav");
			Mix_Chunk *m_chodzenie = Mix_LoadWAV("Dzwieki/m_chodzenie.wav");
			Mix_Chunk *m_obrazenia = Mix_LoadWAV("Dzwieki/m_obrazenia.wav");

		#pragma endregion

		#pragma region Zmienne
			///wektory
			vector<Przedmiot> v_przedmioty;
			vector<Przedmiot*> przedmiksy;
			vector<Przeciwnik> v_przeciwniki;
			///obiekty

			UI ui(0, 0, 720, 1280, g_UI, g_kulazycia);

			Gracz gracz(100, 300, 100, 100, true, g_gracz,g_gracz_chodzenie);
			Przeciwnik gnom1("Gnom", 400, 500, 100,100,3,2,0,g_szkielet,ustawianie_rect_spraj(32,20,4));
			Przeciwnik gnom2("Gnom inny", 500, 700, 100, 100, 3, 2, 0, g_szkielet, ustawianie_rect_spraj(32, 20, 4));
			Przedmiot miecz1( gen_nazw('m') , 200, 200, 100, 100,g_miecz,g_miecz_r,true,'m');
			Przedmiot miecz2("Zbroja Tebigonga", 400, 200, 100, 100, g_zbroja,g_zbroja_r,true,'z');
			Przedmiot miecz3("Tarcza Ateny", 600, 200, 100, 100, g_tarcza,g_tarcza,true,'t');
			Przedmiot miecz4("Miecz Kuby" , 800, 200, 100, 100, g_miecz,g_miecz_r,true,'m');
			Przedmiot miecz5("Miecz Kuby", 1000, 200, 100, 100, g_miecz, g_miecz_r, true, 'm');
			Przedmiot a;
			Potion aaa("Potion Sily", 100, 100, 100, 100, g_potion_zdrowia, true, 's');
			Potion zdrowko("Potion Zdrowia", 100, 200, 100, 100, g_potion_zdrowia, true, 'u');
			Okno_eq eq(100, 10, 600, 600, false);
			///push back test
			//przedmioty
			v_przedmioty.push_back(miecz1);
			v_przedmioty.push_back(miecz2);
			v_przedmioty.push_back(miecz3);
			v_przedmioty.push_back(miecz4);
			v_przedmioty.push_back(miecz5);
			//przeciwniki
			v_przeciwniki.push_back(gnom1);
			v_przeciwniki.push_back(gnom2);
			Przedmiot* tp;
			tp = new Przedmiot;
			*tp = miecz1;
			przedmiksy.push_back(tp);
			tp = new Przedmiot;
			*tp = miecz2;
			przedmiksy.push_back(tp);
			tp = new Przedmiot;
			*tp = miecz3;
			przedmiksy.push_back(tp);
			tp = new Przedmiot;
			*tp = miecz4;
			przedmiksy.push_back(tp);
			tp = new Przedmiot;
			*tp = miecz5;
			przedmiksy.push_back(tp);
			tp = new Potion;
			*tp = aaa;
			przedmiksy.push_back(tp);
			tp = new Potion;
			*tp = zdrowko;
			przedmiksy.push_back(tp);
			#pragma endregion

		//Generowanie mapy:
			if (zmiana_levelu == true)
			{
				mapka.clear();
				mapka = g_mapy(8,tlo,tlo,tlo);
				zmiana_levelu = false;
				
			}

			while (scena == 0) 
			{
				fps.start();
			//ZDARZENIA:
				///1.Przycisk X
					while (SDL_PollEvent(&zdarzenie))
					{
						if (zdarzenie.type == SDL_QUIT) return 0;
						if (zdarzenie.type == SDL_MOUSEMOTION)
						{
							mysz_x = zdarzenie.button.x;
							mysz_y = zdarzenie.button.y;
							
						}
					
						
					}
				///2.Przesuwanie Kamery:
					if (mysz_x < 30&&mysz_x!=0) {
						if(abs(przesuniecieX<8))przesuniecieX += 1;
					}

					else if (mysz_x > 1250 && mysz_x != 1280) {
						if (abs(przesuniecieX)<8)przesuniecieX -= 1;
					}

					else if (mysz_y > 690 && mysz_y != 720) {
						if (abs(przesuniecieY)<8)przesuniecieY-= 1;
					}
					
					else if (mysz_y < 30 && mysz_y != 0) {
						if (abs(przesuniecieY)<8)przesuniecieY += 1;
					}
					else {
						przesuniecieX = 0;
						przesuniecieY = 0;
					}
					if ( mysz_x > 1250 && mysz_x < 30) przesuniecieX = 0;
					if ( mysz_y > 30 && mysz_y < 690) przesuniecieY = 0;
				///3.Podnoszeni przedmiotow przez gracza:
					gracz.podnoszenie(przedmiksy);
					gracz.pauza(v_przeciwniki);
			//STEROWANIE:
				///1.Gracz
					
					skala = 1;
					if (GetAsyncKeyState(0x5A)) // scroll dow
					{
						skala -= 0.2;
						gowno -= 0.2;
					}
					else if (GetAsyncKeyState(0x58)) // scroll up
					{
						skala += 0.2;
						gowno += 0.2;
					}
					gracz.poruszanie(eq, gowno,v_przeciwniki,m_chodzenie);
					gracz.efekty_pasywne();
				///1.1 Gracz atak:
					gracz.atak_przycisk();
				///2.UI
					eq.sterowanie(gracz);
			//RENDERE:
				SDL_RenderClear(render);
				
				///1. Tlo:
					rect2.x = 0;
					rect2.y = 0;
					rect2.w = 1280;
					rect2.h = 720;
					
						for (auto it = mapka.begin(); it != mapka.end(); it++) {
						(*it)->update(render, przesuniecieX, przesuniecieY,skala);
						}
				///2. Gracz:
					gracz.update(render,s_postac,t_postac,przesuniecieX,przesuniecieY, skala,g_zdrowie);

				///3. Przeciwniki:\

					for (int i = 0; i < v_przeciwniki.size(); i++)
					{
						v_przeciwniki[i].poruszanie(render, gracz, s_szkielet, t_szkielet, przesuniecieX, przesuniecieY, skala, g_zdrowie);
					}
					for (int i = 0; i < v_przeciwniki.size(); i++)
					{
						v_przeciwniki[i].update(render,s_szkielet, t_szkielet, przesuniecieX, przesuniecieY, skala, g_zdrowie,g_ciecie);
						
					}
		
				///4. Przedmiot:
					for (auto i = przedmiksy.begin(); i != przedmiksy.end(); i++)
					{
						(*i)->update(render, przesuniecieX, przesuniecieY, skala);
					}


				///.6 Ataki
					gracz.atak(v_przeciwniki, g_znacznik2, g_ciecie,render,m_ciecie);
					//gnom1.atak(gracz);
					for (int i = 0; i < v_przeciwniki.size(); i++)
					{
						v_przeciwniki[i].atak(gracz,m_obrazenia);
					}
				///0. UI:
					ui.update(gracz, render,arial);
					eq.zucanie(gracz, v_przeciwniki, render);
				///5. Okno ekwipunku:
					eq.update(g_okno_ekwipunku, g_znacznik, g_okno_przedmiotu, arial, render, gracz);
				#pragma region Napisy

				//1.Sila tekstura:
				string tekstk2 = "Sila:";
				tekstk2 += to_string(gracz.sila);
				char const* pchar = tekstk2.c_str();
				SDL_Color kolor = { 255,255,255 };
				SDL_Surface* tekst = TTF_RenderText_Blended(arial, pchar, kolor);
				SDL_Texture* tt_sila = SDL_CreateTextureFromSurface(render, tekst);
				//2.Zrecznosc tekstura:
				tekstk2 = "Zrecznosc:";
				tekstk2 += to_string(gracz.zrecznosc);
				pchar = tekstk2.c_str();
				SDL_Surface* tekst2 = TTF_RenderText_Blended(arial, pchar, kolor);
				SDL_Texture* tt_zrecznosc = SDL_CreateTextureFromSurface(render, tekst2);
				//3.Inteligencja tekstura:
				tekstk2 = "Inteligencja:";
				tekstk2 += to_string(gracz.inteligencja);
				pchar = tekstk2.c_str();
				SDL_Surface* tekst3 = TTF_RenderText_Blended(arial, pchar, kolor);
				SDL_Texture* tt_inteligencja = SDL_CreateTextureFromSurface(render, tekst3);
				//4.Render Napisow:
				rect1.x = 10;
				rect1.y = 10;
				rect1.h = 50;
				rect1.w = 150;
				SDL_RenderCopy(render, tt_sila, NULL, &rect1);

				rect1.y = 60;
				SDL_RenderCopy(render, tt_zrecznosc, NULL, &rect1);

				rect1.y = 110;
				SDL_RenderCopy(render, tt_inteligencja, NULL, &rect1);



				#pragma endregion

				//Odswiertzanie i czyszczenie:
				klatka++;
				SDL_RenderPresent(render);
				fps.koniec();
				gracz.koniec(v_przeciwniki);

	
				SDL_FreeSurface(tekst2);
				SDL_FreeSurface(tekst);
				SDL_FreeSurface(tekst3);
				SDL_DestroyTexture(tt_sila);
				SDL_DestroyTexture(tt_zrecznosc);
				SDL_DestroyTexture(tt_inteligencja);
				
				
				
			}

			#pragma region Niszczenie Tekstur Muzyki i Napisow
			SDL_DestroyTexture(g_gracz);
			SDL_DestroyTexture(g_gracz_chodzenie);
			SDL_DestroyTexture(g_szkielet);
			SDL_DestroyTexture(g_zbroja_r);
			SDL_DestroyTexture(g_gnom);
			SDL_DestroyTexture(g_miecz);
			SDL_DestroyTexture(g_miecz_r);
			SDL_DestroyTexture(g_tarcza);
			SDL_DestroyTexture(g_zbroja);
			SDL_DestroyTexture(g_okno_ekwipunku);
			SDL_DestroyTexture(g_znacznik);
			SDL_DestroyTexture(g_znacznik2);
			SDL_DestroyTexture(g_okno_przedmiotu);
			SDL_DestroyTexture(g_UI);
			SDL_DestroyTexture(g_kulazycia);
			SDL_DestroyTexture(g_ciecie);
			SDL_DestroyTexture(g_zdrowie);
			Mix_FreeChunk(m_ciecie);
			Mix_FreeChunk(m_chodzenie);
			Mix_FreeChunk(m_obrazenia);
			#pragma endregion

		}

	}

}
