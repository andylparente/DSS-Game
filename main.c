#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// Dimensoes constantes da tela
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;	

// Flag para sair do jogo
int g_quitGame = 0;	

// Cria os estados do jogo
int STATE_TITLE_SCREEN = 1;
int STATE_MAIN_MENU = 2;
int STATE_OPTIONS = 3;
int STATE_GAMEPLAY = 4;
int STATE_PAUSE = 5;

// Iniciliza o estado do jogo como tela inicial
int g_gameState = 1;

// Inicializa o SDL e cria janela
void init_boot_game();
	
// Carrega imagem PNG como textura
void load_Texture( char* l_Path );

// Limpa memoria e fecha o SDL e o jogo
int close_game();

// Logica de cada g_gameState
int title_screen_logic();
int main_menu_logic();

// A janela do jogo
SDL_Window* g_window = NULL;

// O renderizador da janela
SDL_Renderer* g_renderer = NULL;

// Textura atual sendo apresentada
SDL_Texture* g_texture = NULL;

/*========================================================================================================================================
  ==================================================================SEPARANDO=============================================================
  ========================================================================================================================================*/

void init_boot_game()
{
	// Inicializa o SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		// Apresenta o erro causado 
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		
		// Fechará o jogo ao passar pelo loop principal
		g_quitGame = 1;
	}
	
	else
	{
		//Cria a janela
		g_window = SDL_CreateWindow( "DSS Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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
	
	// Carrega a imagem PNG da empresa em textura
	load_Texture( "empresa.png" );
	
	// Limpa a tela
	SDL_RenderClear( g_renderer );

	// Renderiza a textura a tela
	SDL_RenderCopy( g_renderer, g_texture, NULL, NULL );

	// Atualiza a tela
	SDL_RenderPresent( g_renderer );
	
	// Espera alguns segundos
	SDL_Delay( 3000 );
	
	// Carrega a imagem PNG dos creditos ao SDL 2.0 em textura, depois aplica a textura e a atualiza a tela
	load_Texture( "sdlcredits.png" );
	SDL_RenderClear( g_renderer );
	SDL_RenderCopy( g_renderer, g_texture, NULL, NULL );
	SDL_RenderPresent( g_renderer );
	SDL_Delay( 3000 );
}

int main( int argc, char *argv[] )
{    	
	// Iniciando o jogo
	init_boot_game() ;

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

		// Muda para a lógica de cada g_gameState
		switch ( g_gameState )
		{
			case 1:
				title_screen_logic();
				break;

			case 2: 
				main_menu_logic(); 
				break; 

			case 3: 
				gameplay_logic(); 
				break;

			case 4: 
		//		options_logic(); 
				break;

			case 5: 
		//		pause_logic(); 
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

int title_screen_logic( int argc, char *argv[] )
{
	// Carrega a imagem PNG dos creditos ao SDL 2.0 em textura
	load_Texture( "titlescreen.png" );
	SDL_RenderClear( g_renderer );
	SDL_RenderCopy( g_renderer, g_texture, NULL, NULL );
	SDL_Event e;
	SDL_RenderPresent( g_renderer );

	while( g_gameState == 1 )
	{
		while( SDL_PollEvent( &e ) != 0 )
        	{
        		if( e.type == SDL_KEYDOWN )
			{
				switch( e.key.keysym.sym )
        	        	{
              				case SDLK_SPACE:
                  				g_gameState = 2;
                   				break;
        			}
        		}
		}	
	}
}

int main_menu_logic( int argc, char *argv[] )
{
	// Carrega a imagem do menu principal
	load_Texture( "pele.png" );
	SDL_RenderClear( g_renderer );
	SDL_RenderCopy( g_renderer, g_texture, NULL, NULL );
	SDL_RenderPresent( g_renderer );
    
    	SDL_Event e;
    
    	// Navega pelo menu e muda o g_gameState
	while( g_gameState == 2 )
        {
                while( SDL_PollEvent( &e ) != 0 )
                {
                        if( e.type == SDL_KEYDOWN )
                        {
                                switch( e.key.keysym.sym )
                                {
                                        case SDLK_RETURN:
                                        g_gameState = 3;
                                        break;
                                }
                        }
                }
        }
}

int gameplay_logic( int argc, char *argv[] )
{
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
    
    	// Area a imagem fonte sera aplicada
	SDL_Rect l_dstRect;	
	l_dstRect.x = SCREEN_WIDTH/2;
  	l_dstRect.y = SCREEN_HEIGHT/2;
   	l_dstRect.w = 80;
   	l_dstRect.h = 80;
	
	load_Texture( "pelefinal.png" );
	SDL_RenderCopy( g_renderer, g_texture, &l_srcRect, &l_dstRect );
	SDL_RenderPresent( g_renderer );

	 while( g_gameState == 3 )
	 {
	 	while( SDL_PollEvent( &e ) != 0 )
        	{
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
						l_srcRect.x -= l_srcRect.w;
						break;

					case SDLK_DOWN:
						l_dstRect.y += velx;
						l_srcRect.x -= l_srcRect.w; 
						break;

					case SDLK_RETURN:
						SDL_QUIT;
						return 0;                                                                     
						break;
                		}
            		}
        	}
	}
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
