#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "loaders_and_effects.h"
#include "opening.h"
#include "title_screen.h"
#include "menu.h"
#include "gameplay.h"

// Dimensoes constantes da tela
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;	

// Cria os estados do jogo
int STATE_OPENING = 0;
int STATE_TITLE_SCREEN = 1;
int STATE_MAIN_MENU = 2;
int STATE_GAMEPLAY = 3;
int STATE_HIGHSCORE = 4;
int STATE_CREDITS = 5;
int STATE_OPTIONS = 6;
int STATE_PAUSE = 7;

// Iniciliza o estado do jogo como tela inicial
int g_gameState = 2;

// A janela do jogo
SDL_Window* g_window = NULL;

// O renderizador da janela
SDL_Renderer* g_renderer = NULL;

// Inicializa o SDL e cria janela
void init_boot_game();

// Limpa memoria e fecha o SDL e o jogo
int close_game();

/*========================================================================================================================================
  ==================================================================SEPARANDO=============================================================
  ========================================================================================================================================*/

void init_boot_game()
{
	// Inicializa o SDL
	if( SDL_Init( SDL_INIT_VIDEO || SDL_INIT_AUDIO ) < 0 )
	{
		// Apresenta o erro causado 
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		
		// Fechará o jogo ao passar pelo loop principal
		g_gameState = 666;
	}
	
	else
	{
		//Cria a janela
		g_window = SDL_CreateWindow( "DSS Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( g_window == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			g_gameState = 666;
		}
		
		else
		{
			// Cria renderizador para a janela
			g_renderer = SDL_CreateRenderer( g_window, -1, SDL_RENDERER_ACCELERATED );
			if( g_renderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				g_gameState = 666;
			}
			
			else
			{
				//Inicializa a cor do renderizador
				SDL_SetRenderDrawColor( g_renderer, 255, 0, 0, 0 );
				
				// Inicializa o carregamento de PNG
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					g_gameState = 666;
				}
				
				else
				{
					// Inicializa o carregamento de sons e musicas
	                if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
	                {
	                    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
	                    g_gameState = 666;
	                }
	        	}
			}
		}
	}
}

int main( int argc, char *argv[] )
{  	
	// Iniciando o jogo
	init_boot_game();

	// Cuidador de evento
	SDL_Event e;

	// Enquanto a aplicação esta rodando
	while( g_gameState != 666 )
	{
		// Cuida dos eventos na fila
		while( SDL_PollEvent( &e ) != 0 )
		{
			// Usuario deseja sair
			if( e.type == SDL_QUIT )
			{
				g_gameState = -1;
			}
		}

		// Muda para a lógica de cada g_gameState
		switch ( g_gameState )
		{
			case 0:
				g_gameState = opening_logic( g_window, g_renderer );
				break;
	
			case 1:
				g_gameState = title_screen_logic( g_window, g_renderer );
				break;

			case 2: 
				g_gameState = main_menu_logic( g_window, g_renderer ); 
				break; 

			case 3:
				g_gameState = gameplay_logic( g_window, g_renderer ); 
				break;

			case 6:
				//options_logic( g_window, g_renderer ); 
				g_gameState = -1;
				break;
				
			case 4:
				//highscore_logic( g_window, g_renderer );
				g_gameState = -1;
				break;
				
			case 5:
				//credits_logic( g_window, g_renderer );
				g_gameState = -1;
				break;		

			case 7:
				//pause_logic( g_window, g_renderer );
				g_gameState = -1;
				break;

			default:
				g_gameState = 666;
				break; 
		}
	}
	
	// Destroi a janela e o renderizador
	SDL_DestroyRenderer( g_renderer );
	SDL_DestroyWindow( g_window );
	g_window = NULL;
	g_renderer = NULL;

	return close_game();
}

/*
int gameplay_logic( int argc, char *argv[] )
{
	free_music_sfx( 'b' );
	free_texture();

	// "Velocidade" do sprites
	float velx = 2.5;	
	
	load_Texture( "mapa.png" ); 
	SDL_RenderClear( g_renderer );
    SDL_RenderCopy( g_renderer, g_texture, NULL, NULL );
    
    SDL_Event e;
    
    // Area que sera apresentada da imagem fonte
	SDL_Rect l_srcRect;
	l_srcRect.x = 0;
	l_srcRect.y = 0;
    l_srcRect.w = 80;
    l_srcRect.h = 80;    
    
    // Area onde a imagem fonte sera aplicada
	SDL_Rect l_dstRect;	
	l_dstRect.x = SCREEN_WIDTH/2;
  	l_dstRect.y = SCREEN_HEIGHT/2;
   	l_dstRect.w = 80;
   	l_dstRect.h = 80;
	
	load_Texture( "sprites/sombra100-60.png" );
	SDL_RenderCopy( g_renderer, g_texture, &l_srcRect, &l_dstRect );
	SDL_RenderPresent( g_renderer );

	 while( g_gameState == 3 )
	 {
	 	while( SDL_PollEvent( &e ) != 0 )
		{

			free_texture();
	        SDL_RenderClear( g_renderer );		
			load_Texture( "mapa.png" );
	        SDL_RenderCopy( g_renderer, g_texture, NULL, NULL );
	        free_texture();
			load_Texture( "sprites/sombra100-60.png" );
	        SDL_RenderCopy( g_renderer, g_texture, &l_srcRect, &l_dstRect );
	       	SDL_RenderPresent( g_renderer );
		
	    	if( e.type == SDL_KEYDOWN )
			{
	    		switch( e.key.keysym.sym )
		   		{
					case SDLK_RIGHT:
	        			l_dstRect.x += velx;
						break;
	
					case SDLK_LEFT: 
						l_dstRect.x -= velx;
						break;
	
					case SDLK_UP:
						l_dstRect.y -= velx;
						//l_srcRect.x -= l_srcRect.w;
						break;
	
					case SDLK_DOWN:
						l_dstRect.y += velx;
						//l_srcRect.x -= l_srcRect.w; 
						break;
	
					case SDLK_RETURN:
						g_gameState = 5;                                     
						break;
           		}
        	}
        }
	}
} */

int close_game()
{
	// Sai dos subsistemas do SDL
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
	
	return 0;
}
