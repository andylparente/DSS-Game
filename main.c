#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Dimensoes constantes da tela
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Inicializa o SDL e cria janela
void init_boot_game();

// Carrega imagem PNG como textura
void load_Texture( char* l_Path );

//Aplica e atualiza a textura
void apply_Texture();

// Limpa memoria e fecha o SDL e o jogo
int close_game();

// Flag para sair do jogo
int g_quitGame = 0;

// A janela do jogo
SDL_Window* g_window = NULL;

// O renderizador da janela
SDL_Renderer* g_renderer = NULL;

// Textura atual sendo apresentada
SDL_Texture* g_texture = NULL;

/*===================================================================================================================================================================================*/
/*=====================================================================SEPARANDO=====================================================================================================*/
/*===================================================================================================================================================================================*/

void init_boot_game()
{
	// Inicializa o SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		
		// Sai do jogo ao encontrar o loop principal
		g_quitGame = 1;
	}
	
	else
	{
		//Cria a janela
		g_window = SDL_CreateWindow( "DSS_Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( g_window == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			g_quitGame = 1;
		}
		
		else
		{
			// Cria renderizador para a janela
			g_renderer = SDL_CreateRenderer( g_window, -1, SDL_RENDERER_ACCELERATED );
			if( g_renderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				g_quitGame = 1;
			}
			
			else
			{
				//Inicializa a cor do renderizador
				SDL_SetRenderDrawColor( g_renderer, 0xFF, 0xFF, 0xFF, 0xFF );
				
				// Inicializa o carregamento de PNG
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					g_quitGame = 1;
				}
			}
		}
	}
}

int main( int argc, char *argv[] )
{    	
	// Iniciando o jogo
	init_boot_game() ;
	
	// Carrega a imagem PNG da empresa em textura, depois aplica a textura e a atualiza a tela
	load_Texture( "empresa.png" );
	apply_Texture();
	SDL_Delay( 3000 );
		
	// Carrega a imagem PNG dos creditos ao SDL 2.0 em textura, depois aplica a textura e a atualiza a tela
	load_Texture( "sdlcredits.png" );
	apply_Texture();
	SDL_Delay( 3000 );

	// Cuidador de evento
	SDL_Event e;

	// Enquanto a aplicação esta rodando
	while( g_quitGame != 1 )
	{
		// Cuida dos eventos na fila
		while( SDL_PollEvent( &e ) != 0 )
		{
			// Usuario deseja sair
			if( e.type == SDL_QUIT )
			{
				g_quitGame = 1;
			}
		}
			
		// Carrega a imagem PNG da tela inicial em textura, depois aplica a textura e a atualiza a tela
		load_Texture( "titlescreen.png" );
		apply_Texture ();
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
		g_quitGame = 1;
	}
	
	else
	{
	// Gera textura atraves da surface
        g_texture = SDL_CreateTextureFromSurface( g_renderer, l_loadedSurface );
		if( g_texture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", l_Path, SDL_GetError() );
			g_quitGame = 1;
		}

		// Se livra da surface que nao sera usada
		SDL_FreeSurface( l_loadedSurface );
	}
}

void apply_Texture()
{
	// Limpa tela
	SDL_RenderClear( g_renderer );

	// Renderiza a textura a tela
	SDL_RenderCopy( g_renderer, g_texture, NULL, NULL );

	// Atualiza a tela
	SDL_RenderPresent( g_renderer );
}

int close_game()
{
	// Libera imagens carregadas
	SDL_DestroyTexture( g_texture );
	g_texture = NULL;

	// Destroi a janela
	SDL_DestroyRenderer( g_renderer );
	SDL_DestroyWindow( g_window );
	g_window = NULL;
	g_renderer = NULL;

	// Sai dos subsistemas do SDL
	IMG_Quit();
	SDL_Quit();
	
	return 0;
}
