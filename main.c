#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

// Dimensoes constantes da tela
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Cria os estados do jogo
int g_STATE_TITLE_SCREEN = 1;
int g_STATE_MAIN_MENU = 2;
int g_STATE_OPTIONS = 3;
int g_STATE_GAMEPLAY = 4;
int g_STATE_PAUSE = 5;

// Iniciliza o estado do jogo como tela inicial
int g_gameState = g_STATE_TITLE_SCREEN;

// Inicializa o SDL e cria janela
void init_boot_game();
	
// Carrega imagem PNG como textura
void load_Texture( char* l_Path );

// Limpa memoria e fecha o SDL e o jogo
int close_game();

// A janela do jogo
SDL_Window* g_Window = NULL;

// O renderizador da janela
SDL_Renderer* g_Renderer = NULL;

// Textura atual sendo apresentada
SDL_Texture* g_Texture = NULL;

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
		g_Window = SDL_CreateWindow( "Peleh Legacy v0.04", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( g_Window == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		}
		
		else
		{
			// Cria renderizador para a janela
			g_Renderer = SDL_CreateRenderer( g_Window, -1, SDL_RENDERER_ACCELERATED );
			if( g_Renderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
			}
			
			else
			{
				//Inicializa a cor do renderizador
				SDL_SetRenderDrawColor( g_Renderer, 0xFF, 0xFF, 0xFF, 0xFF );
				
				// Inicializa o carregamento de PNG
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
				}
			}
		}
	}
	
	// Carrega a imagem PNG da empresa em textura
	load_Texture( "empresa.png" );
	
	// Limpa a tela
	SDL_RenderClear( g_Renderer );

	// Renderiza a textura a tela
	SDL_RenderCopy( g_Renderer, g_Texture, NULL, NULL );

	// Atualiza a tela
	SDL_RenderPresent( g_Renderer );
	
	// Espera alguns segundos
	SDL_Delay( 3000 );
	
	// Carrega a imagem PNG dos creditos ao SDL 2.0 em textura, depois aplica a textura e a atualiza a tela
	load_Texture( "sdlcredits.png" );
	SDL_RenderClear( g_Renderer );
	SDL_RenderCopy( g_Renderer, g_Texture, NULL, NULL );
	SDL_RenderPresent( g_Renderer );
	SDL_Delay( 3000 );
}

int main(int argc, char *argv[])
{    	
	// Iniciando o jogo
	init_boot_game() ;
	
	// Flag do loop principal
	int l_quit_game = 0;
		
	// Cuidador de evento
	SDL_Event e;

	// Enquanto a aplicação esta rodando
	while( l_quit_game != 1 )
	{
		// Cuida dos eventos na fila
		while( SDL_PollEvent( &e ) != 0 )
		{
			// Usuario deseja sair
			if( e.type == SDL_QUIT )
			{
				l_quit_game = 1;
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
	
	return close_game();
}

void load_Texture( char* l_Path )
{
	//Imagem PNG em surface e textura
	SDL_Surface* l_loadedSurface = NULL;

	// Carrega a imagem de um diretorio
	l_loadedSurface = IMG_Load( l_Path );
	if( l_loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", l_Path, IMG_GetError() );
	}
	
	else
	{
		// Gera textura atraves da surface
        g_Texture = SDL_CreateTextureFromSurface( g_Renderer, l_loadedSurface );
		if( g_Texture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", l_Path, SDL_GetError() );
		}

		// Se livra da surface que nao sera usada
		SDL_FreeSurface( l_loadedSurface );
	}
}

void title_screen_logic();
{
	// Carrega a imagem PNG dos creditos ao SDL 2.0 em textura
	load_Texture( "titlescreen.png" );
	SDL_RenderClear( g_Renderer );
	SDL_RenderCopy( g_Renderer, g_Texture, NULL, NULL );
	
	// Tentar fazer carregar e aplicar "botao_titlescreen.png" no meio de "titlescreen.png" 
	
	SDL_RenderPresent( g_Renderer );
	
}

int close_game()
{
	// Libera imagens carregadas
	SDL_DestroyTexture( g_Texture );
	g_Texture = NULL;

	// Destroi a janela
	SDL_DestroyRenderer( g_Renderer );
	SDL_DestroyWindow( g_Window );
	g_Window = NULL;
	g_Renderer = NULL;

	// Sai dos subsistemas do SDL
	IMG_Quit();
	SDL_Quit();
	
	return 0;
}
