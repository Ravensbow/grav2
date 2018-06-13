#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include<Windows.h>
#include<iostream>
#include<vector>
#include<sstream>
#include<time.h>
#include<list>
#include<string>
#include<cmath>
#include"klasy.h"


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
			
			return nazwa;
		}
	
#pragma endregion

#pragma region Zmienne globalne
int scena = 1;
int klatka = 0;
int klatka2 = 0;
bool zmiana_levelu = true;
int nr_pietra = 0;
string nazwa_pietra = "KANALY";
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

int t_nazwa = 0;
int s_nazwa = 0;

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
			SDL_Texture *g_sztylet = loadTexture("Grafiki/g_sztylet.png");
			SDL_Texture *g_sztylet_r = loadTexture("Grafiki/g_sztylet_r.png");
			SDL_Texture *g_katana = loadTexture("Grafiki/g_katana.png");
			SDL_Texture *g_katana_r = loadTexture("Grafiki/g_katana_r.png");
			SDL_Texture *g_syriusz = loadTexture("Grafiki/g_syriusz.png");
			SDL_Texture *g_syriusz_r = loadTexture("Grafiki/g_syriusz_r.png");

			SDL_Texture *g_zbroja_skorzana = loadTexture("Grafiki/g_zbroja_skorzana.png");
			SDL_Texture *g_zbroja_skorzana_r = loadTexture("Grafiki/g_zbroja_skorzana_r.png");
			SDL_Texture *g_szata = loadTexture("Grafiki/g_szata.png");
			SDL_Texture *g_szata_r = loadTexture("Grafiki/g_szata_r.png");
			SDL_Texture *g_plytowa = loadTexture("Grafiki/g_zbroja.png");
			SDL_Texture *g_plytowa_r = loadTexture("Grafiki/g_zbroja_r.png");
			SDL_Texture *g_hwang = loadTexture("Grafiki/g_hwang.png");
			SDL_Texture *g_hwang_r = loadTexture("Grafiki/g_hwang_r.png");

			SDL_Texture *tlo = loadTexture("Grafiki/g_czern.png");
			SDL_Texture *g_mgla = loadTexture("Grafiki/g_mgla.png");
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
			SDL_Texture *g_pod_kanaly = loadTexture("Grafiki/g_pod_kanaly.png");
			SDL_Texture *g_pod_kanaly2 = loadTexture("Grafiki/g_pod_kanaly2.png");
			SDL_Texture *g_pod_kanaly3 = loadTexture("Grafiki/g_pod_kanaly3.png");
			SDL_Texture *g_pod_kanaly4 = loadTexture("Grafiki/g_pod_kanaly4.png");
			SDL_Texture *g_sciana = loadTexture("Grafiki/g_sciana2.png");
			SDL_Texture *g_zejscie = loadTexture("Grafiki/g_zejscie.png");
			SDL_Texture *g_schody = loadTexture("Grafiki/g_schody.png");
			SDL_Texture *g_szczur = loadTexture("Grafiki/g_mysz.png");
			SDL_Texture *g_ogien = loadTexture("Grafiki/g_ogien.png");
		//Czcionki:
			TTF_Font*arial = TTF_OpenFont("sredniowiecze.ttf", 30);
			#pragma endregion

		#pragma region Dzwieki
			Mix_Chunk *m_ciecie = Mix_LoadWAV("Dzwieki/m_ciecie.wav");
			Mix_Chunk *m_chodzenie = Mix_LoadWAV("Dzwieki/m_chodzenie2.wav");
			Mix_Chunk *m_obrazenia = Mix_LoadWAV("Dzwieki/m_obrazenia.wav");
			Mix_Chunk *m_wygrales = Mix_LoadWAV("Dzwieki/m_wygrales.wav");
			Mix_Chunk *m_przegrales = Mix_LoadWAV("Dzwieki/m_przegrales.wav");
			Mix_Music *m_muzyka1 = Mix_LoadMUS("Dzwieki/m_muzyka2.wav");

		#pragma endregion

		#pragma region Zmienne
			///wektory
			vector<Przedmiot*> v_przedmioty;
			vector<Przedmiot*> przedmiksy;
			vector<Przeciwnik*> v_przeciwniki;
			vector<Przeciwnik*> przeciwniksy;
			///obiekty

			UI ui(0, 0, 720, 1280, g_UI, g_kulazycia, g_znacznik_podpalenia);
			
			Map testowa_mapa;
			Map mgla_wojny;

			Gracz gracz(100, 300, 100, 100, true, g_gracz,g_gracz_chodzenie);
			Przeciwnik gnom1("Gnom", 400, 500, 100,100,10,10,0,2,g_szkielet,ustawianie_rect_spraj(64,40,4));
			Przeciwnik gnom2("Gnom inny", 500, 700, 100, 100, 10, 10, 0,2, g_szkielet, ustawianie_rect_spraj(64, 40, 4));

			Szczur szczur(g_szczur);
			Ogien ogien(g_ogien);
			
			Przedmiot miecz3("Tarcza Ateny", 600, 200, 100, 100,0,1, g_tarcza,g_tarcza,true,'t');
			Przedmiot a;

			Potion aaa("Potion Sily", 100, 100, 100, 100, g_potion_zdrowia, true, 's',0,0);
			Potion zdrowko("Potion Zdrowia", 100, 200, 100, 100, g_potion_zdrowia, true, 'u',0,0);
			Potion dos("Potion Doswiadczenia", 100, 200, 100, 100, g_potion_zdrowia, true, '1', 0, 0);
			Potion szczala("Rzutka", 200, 200, 100, 100, g_zutka, true, 'q', 5, 3);
			
			Okno_eq eq(60, 10, 700, 600, false,g_statystyki);

			Miecz katana(g_katana, g_katana_r, "Katana");
			Miecz sztylet(g_sztylet, g_sztylet_r, "Maly Sztylet");
			Miecz miecz(g_miecz, g_miecz_r, "Prosty Miecz");
			Miecz syriusz(g_syriusz, g_syriusz_r, "Miecz Syriusza");

			Zbroja skorzana(g_zbroja_skorzana, g_zbroja_skorzana_r, "Skorzana Zbroja");
			Zbroja szata(g_szata, g_szata_r, "Szata Maga");
			Zbroja plytowa(g_zbroja, g_zbroja_r, "Zbroja Plytowa");
			Zbroja hwang(g_hwang, g_hwang_r, "Hwang");
			
			Przeciwnik* tpp;

			tpp = new Szczur;
			*tpp = szczur;
			v_przeciwniki.push_back(tpp);
			tpp = new Ogien;
			*tpp = ogien;
			v_przeciwniki.push_back(tpp);
			
			Przedmiot* tp;
	
			tp = new Miecz;
			*tp =katana;
			v_przedmioty.push_back(tp);
			tp = new Miecz;
			*tp = sztylet;
			v_przedmioty.push_back(tp);
			tp = new Miecz;
			*tp = miecz;
			v_przedmioty.push_back(tp);
			tp = new Miecz;
			*tp = syriusz;
			v_przedmioty.push_back(tp);
			tp = new Zbroja;
			*tp = skorzana;
			v_przedmioty.push_back(tp);
			tp = new Zbroja;
			*tp = hwang;
			v_przedmioty.push_back(tp);
			tp = new Zbroja;
			*tp = szata;
			v_przedmioty.push_back(tp);
			tp = new Zbroja;
			*tp = plytowa;
			v_przedmioty.push_back(tp);
			tp = new Przedmiot;
			*tp = miecz3;
			v_przedmioty.push_back(tp);
			tp = new Potion;
			*tp = aaa;
			v_przedmioty.push_back(tp);
			tp = new Potion;
			*tp = zdrowko;
			v_przedmioty.push_back(tp);
			tp = new Potion;
			*tp = dos;
			v_przedmioty.push_back(tp);
			tp = new Potion;
			*tp = szczala;
			v_przedmioty.push_back(tp);

			testowa_mapa.ukladanie_pokoi(g_pod_kanaly, g_pod_kanaly2, g_pod_kanaly3, g_pod_kanaly4, g_sciana,mgla_wojny);
			testowa_mapa.dodanie_zejscia(g_zejscie,g_schody,gracz);
			testowa_mapa.resp_item(v_przedmioty, przedmiksy);
			testowa_mapa.resp_przeciwnikow(v_przeciwniki, przeciwniksy);

			mgla_wojny.czarna_mapa(tlo);
			#pragma endregion

		

			while (scena == 0) 
			{
				fps.start();
			//ZDARZENIA:
			//Generowanie mapy:
				if (zmiana_levelu == true)
				{
					gracz.posX = 100;
					gracz.posY = 300;
					gracz.wysokosc = 100;
					gracz.szerokosc = 100;

					testowa_mapa.clear();
					mgla_wojny.clear();
					przedmiksy.clear();

					testowa_mapa.ukladanie_pokoi(g_pod_kanaly, g_pod_kanaly2, g_pod_kanaly3, g_pod_kanaly4, g_sciana, mgla_wojny);
					testowa_mapa.dodanie_zejscia(g_zejscie, g_schody, gracz);
					testowa_mapa.resp_item(v_przedmioty, przedmiksy);
					mgla_wojny.czarna_mapa(tlo);

					

					nr_pietra++;

					
					if (nr_pietra == 6)
					{
						scena = 1;
						SDL_Texture*napisek = napis(255, 255, 0, arial, render,"WYGRALES");
						SDL_Rect rect;
						rect.x = 0;
						rect.y = 0;
						rect.w = 1280;
						rect.h = 720;
						SDL_RenderCopy(render, napisek, NULL, &rect);
						SDL_RenderPresent(render);
						Mix_PlayChannel(-1, m_wygrales, 0);
						SDL_Delay(5000);
						SDL_DestroyTexture(napisek);

					}
					zmiana_levelu = false;

				}
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
			
			//STEROWANIE:
				///1.Skalowanie
					
					skala = 1;
					if (GetAsyncKeyState(0x5A)) // scroll dow
					{
						if(skala>0.5)skala = 0.5;
						SDL_Delay(200);
					}
					else if (GetAsyncKeyState(0x58)) // scroll up
					{
						if(skala<2)skala = 2;
						SDL_Delay(200);
					}

					testowa_mapa.skalowanie(przesuniecieX, przesuniecieY, skala);
					mgla_wojny.skalowanie(przesuniecieX, przesuniecieY, skala);
					
					gracz.skalowanie(przesuniecieX, przesuniecieY, skala);
					for (auto itr = przedmiksy.begin(); itr != przedmiksy.end(); itr++) {
						(*itr)->skalowanie(przesuniecieX, przesuniecieY, skala);
					}
					for (auto itr = przeciwniksy.begin(); itr != przeciwniksy.end(); itr++) {
						(*itr)->skalowanie(przesuniecieX, przesuniecieY, skala);
					}

				///Gracz przy krawedzi, przesuwanie kamery:
					if (gracz.posX >1280 -gracz.szerokosc * 2)
					{
						testowa_mapa.skalowanie(-400,0, 1);
						mgla_wojny.skalowanie(-400, 0, 1);

						gracz.skalowanie(-400, 0,1);
						for (auto itr = przedmiksy.begin(); itr != przedmiksy.end(); itr++) {
							(*itr)->skalowanie(-400, 0, skala);
						}
						for (auto itr = przeciwniksy.begin(); itr != przeciwniksy.end(); itr++) {
							(*itr)->skalowanie(-400,0, skala);
						}
					}
					
					if (gracz.posX < gracz.szerokosc*2)
					{
						cout << gracz.posX << endl;
						testowa_mapa.skalowanie(400, 0, 1);
						mgla_wojny.skalowanie(400, 0, 1);

						gracz.skalowanie(400, 0, 1);
						for (auto itr = przedmiksy.begin(); itr != przedmiksy.end(); itr++) {
							(*itr)->skalowanie(400, 0, skala);
						}
						for (auto itr = przeciwniksy.begin(); itr != przeciwniksy.end(); itr++) {
							(*itr)->skalowanie(400, 0, skala);
						}
					}
					if (gracz.posY >720 - gracz.szerokosc * 2)
					{
						testowa_mapa.skalowanie(0, -200, 1);
						mgla_wojny.skalowanie(0, -200, 1);

						gracz.skalowanie(0, -200, 1);
						for (auto itr = przedmiksy.begin(); itr != przedmiksy.end(); itr++) {
							(*itr)->skalowanie(0, -200, skala);
						}
						for (auto itr = przeciwniksy.begin(); itr != przeciwniksy.end(); itr++) {
							(*itr)->skalowanie(0, -200, skala);
						}
					}
					if (gracz.posY < gracz.szerokosc * 2)
					{
						cout << "elo" << endl;
						testowa_mapa.skalowanie(0, 200, 1);
						mgla_wojny.skalowanie(0, 200, 1);

						gracz.skalowanie(0, 200, 1);
						for (auto itr = przedmiksy.begin(); itr != przedmiksy.end(); itr++) {
							(*itr)->skalowanie(0, 200, skala);
						}
						for (auto itr = przeciwniksy.begin(); itr != przeciwniksy.end(); itr++) {
							(*itr)->skalowanie(0, 200, skala);
						}
					}


				///3.Podnoszeni przedmiotow przez gracza:
					gracz.usuwanie_mgly(mgla_wojny,g_mgla);
					

					gracz.pauza(przeciwniksy);
					
					gracz.efekty_pasywne();

				///Gracz:
					gracz.poruszanie(eq, przeciwniksy,m_chodzenie,testowa_mapa);
					gracz.efekty_pasywne();
				///1.1 Gracz atak:6
					gracz.atak_przycisk();
					
				///2.UI
					eq.sterowanie(gracz);
					
			//RENDERE:
				SDL_RenderClear(render);
				
				///1. Tlo:
				
					testowa_mapa.update(render,gracz,zmiana_levelu);

				///2. Gracz:
					gracz.update(render,s_postac,t_postac,g_zdrowie,arial,g_obrazenia);

				///3. Przeciwniki:\

					for (int i = 0; i < przeciwniksy.size(); i++)
					{
						przeciwniksy[i]->poruszanie(render, gracz, s_szkielet, t_szkielet,g_zdrowie,testowa_mapa);
					}
					for (int i = 0; i < przeciwniksy.size(); i++)
					{
						przeciwniksy[i]->update(render,s_szkielet, t_szkielet, g_zdrowie,g_ciecie,arial);
						
					}
					eq.zucanie(gracz, przeciwniksy, render,przedmiksy,testowa_mapa);
				///4. Przedmiot:
					for (auto i = przedmiksy.begin(); i != przedmiksy.end(); i++)
					{
						
						(*i)->update(render);
					}


				///.6 Ataki
					
					gracz.atak(przeciwniksy, g_znacznik2, g_ciecie,render,m_ciecie);
					gracz.efekty_pasywne();
					//gnom1.atak(gracz);
					for (int i = 0; i < przeciwniksy.size(); i++)
					{
						przeciwniksy[i]->atak(gracz,m_obrazenia);
					}
				///7. Mgla wojny:
					mgla_wojny.update(render,gracz,zmiana_levelu);
				///0. UI:
					ui.update(gracz, render,arial,nr_pietra,nazwa_pietra);
					gracz.podnoszenie(przedmiksy, render, g_okno_przedmiotu, arial);
				///5. Okno ekwipunku:
					eq.update(g_okno_ekwipunku, g_znacznik, g_okno_przedmiotu, arial, render, gracz);
					
					if (gracz.zycie <= 0)
					{
						scena = 1;
						SDL_Texture*napisek = napis(255, 255, 0, arial, render, "PRZEGRALES");
						SDL_Rect rect;
						rect.x = 0;
						rect.y = 0;
						rect.w = 1280;
						rect.h = 720;
						SDL_RenderCopy(render, napisek, NULL, &rect);
						SDL_RenderPresent(render);
						Mix_PlayChannel(-1, m_przegrales, 0);
						SDL_Delay(5000);
						SDL_DestroyTexture(napisek);
					}

				//Odswiertzanie i czyszczenie:
				klatka++;
				SDL_RenderPresent(render);
				fps.koniec();
				gracz.koniec(przeciwniksy);
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
			SDL_DestroyTexture(g_pod_kanaly);
			SDL_DestroyTexture(g_pod_kanaly2);
			SDL_DestroyTexture(g_pod_kanaly3);
			SDL_DestroyTexture(g_pod_kanaly4);
			SDL_DestroyTexture(g_mgla);
			SDL_DestroyTexture(g_szczur);
			SDL_DestroyTexture(g_ogien);
			SDL_DestroyTexture(g_zejscie);
			SDL_DestroyTexture(g_schody);
			
			SDL_DestroyTexture(g_sztylet);
			SDL_DestroyTexture(g_sztylet_r);
			SDL_DestroyTexture(g_katana);
			SDL_DestroyTexture(g_katana_r);
			SDL_DestroyTexture(g_syriusz);
			SDL_DestroyTexture(g_syriusz_r);
			SDL_DestroyTexture(g_zbroja_skorzana);
			SDL_DestroyTexture(g_zbroja_skorzana_r);
			SDL_DestroyTexture(g_szata);
			SDL_DestroyTexture(g_szata_r);
			SDL_DestroyTexture(g_plytowa);
			SDL_DestroyTexture(g_plytowa_r);
			SDL_DestroyTexture(g_hwang);
			SDL_DestroyTexture(g_hwang_r);

			Mix_FreeChunk(m_ciecie);
			Mix_FreeChunk(m_chodzenie);
			Mix_FreeChunk(m_obrazenia);

			#pragma endregion

		}

		if (scena == 1)
		{
		#pragma region Tekstury
			SDL_Texture *g_menu = loadTexture("Grafiki/g_menu.png");
			SDL_Texture *g_napis_menu = loadTexture("Grafiki/g_napis_menu.png");
			SDL_Texture *g_znacznik_menu = loadTexture("Grafiki/g_znacznik_menu.png");
	
			//Czcionki:
			TTF_Font*arial = TTF_OpenFont("sredniowiecze.ttf", 30);
		#pragma endregion

		#pragma region Dzwieki
			
			Mix_Chunk *m_hello = Mix_LoadWAV("Dzwieki/m_hello.wav");
			Mix_Music *m_muzyka1 = Mix_LoadMUS("Dzwieki/m_muzyka2.wav");
		#pragma endregion

			int znacznikX = 460;
			int znacznikY = 300;
			Mix_PlayChannel(-1, m_hello, 0);

			///0. Muzyka:
			if (Mix_PlayingMusic() == 0)
			{
				//Play the music
				Mix_PlayMusic(m_muzyka1, -1);
			}
			while (scena == 1)
			{
				while (SDL_PollEvent(&zdarzenie))
				{
					if (zdarzenie.type == SDL_QUIT) return 0;
					if (zdarzenie.type == SDL_MOUSEMOTION)
					{
						mysz_x = zdarzenie.button.x;
						mysz_y = zdarzenie.button.y;

					}


				}

				SDL_RenderClear(render);
				SDL_Rect rect;
				rect.x = 0;
				rect.y = 0;
				rect.w = 1280;
				rect.h = 720;
				SDL_RenderCopy(render, g_menu, NULL, &rect);
				rect.x = 400;
				rect.y = 10;
				rect.w = 500;
				rect.h = 350;
				SDL_RenderCopy(render, g_napis_menu, &ustawianie_rect_spraj(200,300,2)[taimer_animacji(s_nazwa,t_nazwa,2,400)], &rect);

				if (GetAsyncKeyState(VK_UP)&&znacznikY-100>=300)
				{
					znacznikY -= 100;
					SDL_Delay(200);
				}
				if (GetAsyncKeyState(VK_DOWN) && znacznikY + 100 <= 500)
				{
					znacznikY += 100;
					SDL_Delay(200);
				}
				if (GetAsyncKeyState(VK_RETURN) && znacznikY == 300)
				{
					scena = 0;
				}
				if (GetAsyncKeyState(VK_RETURN) && znacznikY == 400)
				{

				}
				if (GetAsyncKeyState(VK_RETURN) && znacznikY == 500)
				{
					return 0;
				}


				rect.x = znacznikX;
				rect.y = znacznikY;
				rect.w = 400;
				rect.h = 100;
				SDL_RenderCopy(render, g_znacznik_menu, NULL, &rect);



				klatka++;
				SDL_RenderPresent(render);
			}

		}
	}

}
