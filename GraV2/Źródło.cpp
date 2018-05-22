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
double przesuniecieX=0;
double przesuniecieY=0;
double skala=1;
double gowno = 1;
SDL_Rect rect1, rect2, rect3;
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
			SDL_Texture *g_okno_ekwipunku = loadTexture("Grafiki/g_okno_ekwipunku2.png");
			SDL_Texture *g_znacznik = loadTexture("Grafiki/g_znacznik.png");
			SDL_Texture *g_znacznik2 = loadTexture("Grafiki/g_znacznik2.png");
			SDL_Texture *g_okno_przedmiotu = loadTexture("Grafiki/g_okno_przedmiotu2.png");
			SDL_Texture *g_szkielet = loadTexture("Grafiki/g_szkielet.png");
			SDL_Texture *g_gracz_chodzenie = loadTexture("Grafiki/g_postac_chodzenie.png");
			SDL_Texture *g_ciecie = loadTexture("Grafiki/g_ciecie.png");
			SDL_Texture *g_zdrowie = loadTexture("Grafiki/g_zdrowie.png");
			SDL_Texture *g_UI = loadTexture("Grafiki/g_UI.png");
			SDL_Texture *g_kulazycia = loadTexture("Grafiki/g_kulazycia.png");
			SDL_Texture *g_potion_zdrowia = loadTexture("Grafiki/g_potion_zdrowia.png");
			SDL_Texture *g_zutka = loadTexture("Grafiki/g_zutka.png");
			SDL_Texture *g_znacznik_podpalenia = loadTexture("Grafiki/g_znacznik_podpalenia.png");
			SDL_Texture *g_statystyki = loadTexture("Grafiki/g_statystyki2.png");
			SDL_Texture *g_obrazenia = loadTexture("Grafiki/g_obrazenia.png");
			SDL_Texture *g_podloga = loadTexture("Grafiki/g_podloga.png");
			SDL_Texture *g_sciana = loadTexture("Grafiki/g_sciana.png");
		//Czcionki:
			TTF_Font*arial = TTF_OpenFont("sredniowiecze.ttf", 30);
			#pragma endregion

		#pragma region Dzwieki
			Mix_Chunk *m_ciecie = Mix_LoadWAV("Dzwieki/m_ciecie.wav");
			Mix_Chunk *m_chodzenie = Mix_LoadWAV("Dzwieki/m_chodzenie.wav");
			Mix_Chunk *m_obrazenia = Mix_LoadWAV("Dzwieki/m_obrazenia.wav");
			Mix_Music *m_muzyka1 = Mix_LoadMUS("Dzwieki/m_muzyka12.wav");

		#pragma endregion

		#pragma region Zmienne
			///wektory
			vector<Przedmiot> v_przedmioty;
			vector<Przedmiot*> przedmiksy;
			vector<Przeciwnik> v_przeciwniki;
			///obiekty

			UI ui(0, 0, 720, 1280, g_UI, g_kulazycia, g_znacznik_podpalenia);
			
			Map testowa_mapa;

			Gracz gracz(100, 300, 100, 100, true, g_gracz,g_gracz_chodzenie);
			Przeciwnik gnom1("Gnom", 400, 500, 100,100,10,10,0,2,g_szkielet,ustawianie_rect_spraj(32,20,4));
			Przeciwnik gnom2("Gnom inny", 500, 700, 100, 100, 10, 10, 0,2, g_szkielet, ustawianie_rect_spraj(32, 20, 4));
			Przedmiot miecz1( gen_nazw('m') , 200, 200, 100, 100,2,0,g_miecz,g_miecz_r,true,'m');
			Przedmiot miecz2("Zbroja Tebigonga", 400, 200, 100, 100,0,2, g_zbroja,g_zbroja_r,true,'z');
			Przedmiot miecz3("Tarcza Ateny", 600, 200, 100, 100,0,1, g_tarcza,g_tarcza,true,'t');
			Przedmiot miecz4("Miecz Kuby" , 800, 200, 100, 100,3,0, g_miecz,g_miecz_r,true,'m');
			Przedmiot miecz5("Miecz Kuby", 1000, 200, 100, 100,5,0, g_miecz, g_miecz_r, true, 'm');
			Przedmiot a;
			Potion aaa("Potion Sily", 100, 100, 100, 100, g_potion_zdrowia, true, 's',0,0);
			Potion zdrowko("Potion Zdrowia", 100, 200, 100, 100, g_potion_zdrowia, true, 'u',0,0);
			Potion szczala("Rzutka", 200, 200, 100, 100, g_zutka, true, 'q', 5, 10);
			Okno_eq eq(60, 10, 700, 600, false,g_statystyki);
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
			tp = new Potion;
			*tp = szczala;
			przedmiksy.push_back(tp);

			testowa_mapa.ukladanie_pokoi(g_podloga, g_sciana);

			#pragma endregion

		//Generowanie mapy:
			if (zmiana_levelu == true)
			{
				
				zmiana_levelu = false;
				
			}

			while (scena == 0) 
			{
				fps.start();
			//ZDARZENIA:
				///0. Muzyka:
				if (Mix_PlayingMusic() == 0)
				{
					//Play the music
					Mix_PlayMusic(m_muzyka1, -1);
				}
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
					gracz.efekty_pasywne();
			//STEROWANIE:
				///1.Gracz
					
					skala = 1;
					if (GetAsyncKeyState(0x5A)) // scroll dow
					{
						skala -= 0.02;
						gowno -= 0.02;
					}
					else if (GetAsyncKeyState(0x58)) // scroll up
					{
						skala += 0.02;
						gowno += 0.02;
					}
				
					gracz.poruszanie(eq, skala,v_przeciwniki,m_chodzenie);
					gracz.efekty_pasywne();
				///1.1 Gracz atak:6
					gracz.atak_przycisk();
					
				///2.UI
					eq.sterowanie(gracz);
					
			//RENDERE:
				SDL_RenderClear(render);
				
				///1. Tlo:
				
					testowa_mapa.update(skala, przesuniecieX, przesuniecieY, render);

				///2. Gracz:
					gracz.update(render,s_postac,t_postac,przesuniecieX,przesuniecieY, skala,g_zdrowie,arial,g_obrazenia);

				///3. Przeciwniki:\

					for (int i = 0; i < v_przeciwniki.size(); i++)
					{
						v_przeciwniki[i].poruszanie(render, gracz, s_szkielet, t_szkielet, przesuniecieX, przesuniecieY, skala, g_zdrowie);
					}
					for (int i = 0; i < v_przeciwniki.size(); i++)
					{
						v_przeciwniki[i].update(render,s_szkielet, t_szkielet, przesuniecieX, przesuniecieY, skala, g_zdrowie,g_ciecie,arial);
						
					}
					eq.zucanie(gracz, v_przeciwniki, render,skala,przedmiksy);
				///4. Przedmiot:
					for (auto i = przedmiksy.begin(); i != przedmiksy.end(); i++)
					{
						
						(*i)->update(render, przesuniecieX, przesuniecieY, skala);
					}


				///.6 Ataki
					
					gracz.atak(v_przeciwniki, g_znacznik2, g_ciecie,render,m_ciecie);
					gracz.efekty_pasywne();
					//gnom1.atak(gracz);
					for (int i = 0; i < v_przeciwniki.size(); i++)
					{
						v_przeciwniki[i].atak(gracz,m_obrazenia);
					}
				///0. UI:
					ui.update(gracz, render,arial);
					
				///5. Okno ekwipunku:
					eq.update(g_okno_ekwipunku, g_znacznik, g_okno_przedmiotu, arial, render, gracz);
			

				//Odswiertzanie i czyszczenie:
				klatka++;
				SDL_RenderPresent(render);
				fps.koniec();
				gracz.koniec(v_przeciwniki);
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
			SDL_DestroyTexture(g_zutka);
			SDL_DestroyTexture(g_znacznik_podpalenia);
			SDL_DestroyTexture(g_statystyki);
			SDL_DestroyTexture(g_obrazenia);
			SDL_DestroyTexture(g_sciana);
			SDL_DestroyTexture(g_podloga);
			Mix_FreeChunk(m_ciecie);
			Mix_FreeChunk(m_chodzenie);
			Mix_FreeChunk(m_obrazenia);
			#pragma endregion

		}

	}

}
