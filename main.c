#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

// Dimensoes constantes da tela
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Cria os estados do jogo
enum gameState
{ 
	STATE_TITLE_SCREEN,
	STATE_MAIN_MENU,
	STATE_GAMEPLAY,
	STATE_OPTIONS,
	STATE_PAUSE
};
	
// Inicializa o SDL e cria janela
void init_boot_game();

// Carrega e otimiza imagem PNG 
SDL_Surface* load_and_optimizeSurface( char* l_Path );

//Aplica e atualiza a surface
void apply_update_Surface (SDL_Surface* l_Surface);

// Limpa memoria e fecha o SDL e o jogo
int close_game();

// A janela do jogo
SDL_Window* g_Window = NULL;

// Surface que vai conter nossa janela
SDL_Surface* g_ScreenSurface = NULL;

/*========================================================================================================================================*/
/*==================================================================SEPARANDO=============================================================*/
/*========================================================================================================================================*/

void init_boot_game()
{
	// Inicializa o SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
	}
	
	else
	{
		//Cria a janela
		g_Window = SDL_CreateWindow( "Peleh Legacy v0.03", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( g_Window == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		}
		
		else
		{
			// Inicializa o carregamento de PNG
			int imgFlags = IMG_INIT_PNG;
			if( !( IMG_Init( imgFlags ) & imgFlags ) )
			{
				printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
			}
			
			else
			{
				// Pegando a surface da janela
				g_ScreenSurface = SDL_GetWindowSurface( g_Window );
			}
		}
	}
	// Carregando imagem PNG da empresa e otimiza, depois aplica a surface e a atualiza
	apply_update_Surface ( load_and_optimizeSurface( "C:\Users\User\Desktop\Facul\Lazy Foo\meu_jogo\empresa.png" ) );
	SDL_Delay( 3000 );
	
	// Carregando imagem PNG dos creditos ao SDL2.0 e otimiza, depois aplica a surface e a atualiza
	apply_update_Surface ( load_and_optimizeSurface( "C:\Users\User\Desktop\Facul\Lazy Foo\meu_jogo\sdlcredits.png" ) );
	SDL_Delay( 3000 );

	return sucess;
}

int main(int argc, char *argv[])
{    	
	// Testando init_boot_game
	if( !init_boot_game() )
	{
		printf( "Failed to initialize!\n" );
	}
	
	else
	{
		// Flag do loop principal
		int quit_game = 0;
		
		// Cuidador de evento
		SDL_Event e;

		// Iniciliza o estado do jogo como tela inicial
		gameState = STATE_TITLE_SCREEN;

		// Enquanto a aplicação esta rodando
		while( !quit_game )
		{
			// Cuida dos eventos na fila
			while( SDL_PollEvent( &e ) != 0 )
			{
				// Usuario deseja sair
				if( e.type == SDL_QUIT )
				{
					quit = true;
				}
			}

			switch ( gameState )
			{
				case STATE_TITLE_SCREEN:
					title_screen_logic();
					break;

				case STATE_MAIN_MENU: 
					main_menu_logic(); 
					break; 

				case STATE_GAMEPLAY: 
					gameplay_logic(); 
					break;

				case STATE_OPTIONS: 
					options_logic(); 
					break;

				case STATE_PAUSE: 
					pause_logic(); 
					break; 
			}
		}
	}	
	return close_game();
}

SDL_Surface* load_and_optimizeSurface( char* l_Path )
{
	//Imagem PNG e Imagem PNG otimizada
	SDL_Surface* l_loadedSurface = NULL;
	SDL_Surface* l_optimizedSurface = NULL;
	
	// Carrega a imagem de uma localizacao
	l_loadedSurface = IMG_Load( l_Path );
	if( l_loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", l_Path, IMG_GetError() );
	}
	
	else
	{
		// Converte a surface para o formato da tela
		l_optimizedSurface = SDL_ConvertSurface( l_loadedSurface, g_ScreenSurface->format, 0 );
		if( l_optimizedSurface == NULL )
		{
			printf( "Unable to optimize image %s! SDL Error: %s\n", l_Path, SDL_GetError() );
		}

		// Se livra da surface que nao sera usada
		SDL_FreeSurface( l_loadedSurface );
	}

	return l_optimizedSurface;
}

void apply_update_Surface (SDL_Surface* l_Surface)
{
	// Aplica a imagem 
	SDL_BlitSurface( l_Surface, NULL, g_ScreenSurface, NULL );
		
	// Atualiza a surface
	SDL_UpdateWindowSurface( g_Window );
}

int close_game()
{
	// Destroi a janela
	SDL_DestroyWindow( g_Window );
	g_Window = NULL;

	// Sai dos subsistemas do SDL
	SDL_Quit();
	
	return 0;
}
