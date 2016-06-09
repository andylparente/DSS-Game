#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "opening.c"
#include "title_screen.c"
#include "menu.c"

// Dimensoes constantes da tela
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;	

// Flag para sair do jogo
int g_quitGame = 0;	

// Cria os estados do jogo
int STATE_OPENING = 0;
int STATE_TITLE_SCREEN = 1;
int STATE_MAIN_MENU = 2;
int STATE_OPTIONS = 6;
int STATE_HIGHSCORE = 4;
int STATE_CREDITS = 5;
int STATE_GAMEPLAY = 3;
int STATE_PAUSE = 7;


// Iniciliza o estado do jogo como tela inicial
int g_gameState = 0;

// Inicializa o SDL e cria janela
void init_boot_game( SDL_Window* l_window, SDL_Renderer* l_renderer );
	
// Carrega imagem PNG como textura
void load_Texture( char* l_path, SDL_Window* l_window, SDL_Renderer* l_renderer, SDL_Texture* l_texture );

// Limpa memoria e fecha o SDL e o jogo
int close_game( SDL_Window* l_window, SDL_Renderer* l_renderer );

/*/ Libera textura gerada
void free_texture();

// Carrega WAVE, MOD, MIDI, OGG, MP3 e FLAC
void load_music( char* l_Path );

// Carrega WAVE, AIFF, RIFF, OGG, e VOC 
void load_sfx( char* l_Path );

// Libera a musica que esta sendo tocada e/ou efeito sonoro
void free_music_sfx( char type );

// Logica de cada g_gameState
int title_screen_logic();
int main_menu_logic();
int gameplay_logic();

// A musica que esta sendo tocada
Mix_Music* g_music = NULL;

// Efeito sonoro que sera usado
Mix_Chunk* g_sfx = NULL; */

/*========================================================================================================================================
  ==================================================================SEPARANDO=============================================================
  ========================================================================================================================================*/

void init_boot_game( l_window, l_renderer )
{
	// Inicializa o SDL
	if( SDL_Init( SDL_INIT_VIDEO || SDL_INIT_AUDIO ) < 0 )
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
		if( l_window == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			g_quitGame = 1;
		}
		
		else
		{
			// Cria renderizador para a janela
			l_renderer = SDL_CreateRenderer( l_window, -1, SDL_RENDERER_ACCELERATED );
			if( l_renderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				g_quitGame = 1;
			}
			
			else
			{
				//Inicializa a cor do renderizador
				SDL_SetRenderDrawColor( l_renderer, 255, 0, 0, 0 );
				
				// Inicializa o carregamento de PNG
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					g_quitGame = 1;
				}
				
				else
				{
					// Inicializa o carregamento de sons e musicas
	                if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
	                {
	                    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
	                    g_quitGame = 1;
	                }
	        	}
			}
		}
	}
}

int main( int argc, char *argv[] )
{
	// A janela do jogo
	SDL_Window* l_window = NULL;

	// O renderizador da janela
	SDL_Renderer* l_renderer = NULL;
	    	
	// Iniciando o jogo
	init_boot_game( l_window, l_renderer );

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
			case 0:
				g_quitGame = opening_logic( l_window, l_renderer );
				g_gameState = 1;
				break;
	
			case 1:
				g_quitGame = title_screen_logic( l_window, l_renderer );
				g_gameState = 2;
				break;

			case 2: 
				g_gameState = main_menu_logic( l_window, l_renderer ); 
				break; 

			case 3:
				gameplay_logic(); 
				break;

			case 4:
				//options_logic(); 
				break;
				
			case 5:
				g_quitGame = 1;
				//highscore_logic();
				break;
				
			case 6:
				//credits_logic();		

			case 7:
				//pause_logic(); 
				break; 
		}
	}
	
	return close_game( l_renderer, l_window );
}


int load_Texture( char* l_path, SDL_Window* l_window, SDL_Renderer* l_renderer, SDL_Texture* l_texture )
{
	// Flag para fechar o jogo
	int l_quitGame = 0;
	
	//Imagem PNG em surface e textura
	SDL_Surface* l_loadedSurface = NULL;

	// Carrega a imagem de um diretorio
	l_loadedSurface = IMG_Load( l_path );
	if( l_loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", l_path, IMG_GetError() );
		l_quitGame = 1;
	}
	
	else
	{
		// Gera textura atraves da surface
        l_texture = SDL_CreateTextureFromSurface( l_renderer, l_loadedSurface );
		if( l_texture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", l_path, SDL_GetError() );
			l_quitGame = 1;
		}

		// Se livra da surface que nao sera usada
		SDL_FreeSurface( l_loadedSurface );
		l_loadedSurface = NULL;
	}
	
	return l_quitGame;
}

/*
void free_texture()
{
	// Libera imagem carregada
	SDL_DestroyTexture( g_texture );
	g_texture = NULL;
}

void load_music( char* l_Path )
{	
	// Carrega musica
	g_music = Mix_LoadMUS( l_Path );
	if( g_music == NULL )
    {
        printf( "Failed to load %s [music]! SDL_mixer Error: %s\n", l_Path, Mix_GetError() );
        g_quitGame = 1;
    }

    // Reproduz a musica
    Mix_PlayMusic( g_music, -1 );
}

void load_sfx( char* l_Path )
{
	// Carrega efeitos sonoros
    g_sfx = Mix_LoadWAV( l_Path );
    if( g_sfx == NULL )
    {
        printf( "Failed to load %s [sfx]! SDL_mixer Error: %s\n", l_Path, Mix_GetError() );
        g_quitGame = 1;
    }

    // Reproduz o efeito sonoro
    Mix_PlayChannel( -1, g_sfx, 0 );
}

void free_music_sfx( char type )
{
	switch( type )
	{
		case 'm':
			// Libera a musica carregada
		    Mix_FreeMusic( g_music );
		    g_music = NULL;
		    break;

		case 's':
		    // Libera o chunck 
		    Mix_FreeChunk( g_sfx );
		    g_sfx = NULL;
		    break;

		default:
		    Mix_FreeMusic( g_music );
		    g_music = NULL;
		    Mix_FreeChunk( g_sfx );
		    g_sfx = NULL;
		    break;
	}
}

int title_screen_logic( int argc, char *argv[] )
{
	// Certifica que as midias foram liberadas
	free_music_sfx( 'b' );
	free_texture();

	// Carrega a imagem PNG dos creditos ao SDL 2.0 em textura
	load_Texture( "title_screen.png" );
	SDL_RenderClear( g_renderer );
	SDL_RenderCopy( g_renderer, g_texture, NULL, NULL );
	SDL_RenderPresent( g_renderer );
	
	// Carrega musica da tela inicial
	load_music( "sound_music/halloween_hunting.mp3" );
	
	SDL_Event e;
	
	while( g_gameState == 1 )
	{
		while( SDL_PollEvent( &e ) != 0 )
        {
        	if( e.type == SDL_KEYDOWN )
			{
				switch( e.key.keysym.sym )
        	   	{
            		case SDLK_SPACE:
            			// Carrega sfx 
            			load_sfx( "sound_music/one_ring_from_church_bell.wav" );
            			SDL_Delay( 4000 );
            			g_gameState = 2;
            			break;
        		}
        	}
		}	
	}
}

int main_menu_logic( int argc, char *argv[] )
{
	free_music_sfx( 'b' );
	free_texture();

	SDL_RenderClear( g_renderer );

	// Carrega a imagem do menu principal
	load_Texture( "main_menu.png" );
	SDL_RenderCopy( g_renderer, g_texture, NULL, NULL );

	// Carrega a imagem do menu principal
	load_Texture( "arrow.png" );

	// Area onde a imagem fonte sera aplicada
	SDL_Rect l_dstRect;	
	l_dstRect.x = 100;
  	l_dstRect.y = 205;
   	l_dstRect.w = 35;
   	l_dstRect.h = 35;
	SDL_RenderCopy( g_renderer, g_texture, NULL, &l_dstRect );

	SDL_RenderPresent( g_renderer );

	// Carrega musica do menu principal
	load_music( "sound_music/suspended.mp3" );
    
    SDL_Event e;
    
    // Navega pelo menu e muda o g_gameState
	while( g_gameState == 2 )
    {
        while( SDL_PollEvent( &e ) != 0 )
        {
            if( e.type == SDL_KEYDOWN )
    	    {
    	    	load_sfx( "sound_music/wood_coffin_lid_impact.wav" );

                switch( e.key.keysym.sym )
                {
                    case SDLK_BACKSPACE:
                    	g_gameState = 1;
                    	break;

                    case SDLK_UP:
                    	l_dstRect.y -= 42;
                    	if ( l_dstRect.y < 205 )
                    	{
                    		l_dstRect.y = 373;
						}
                    	free_texture;
                    	load_Texture( "arrow.png" );
                    	SDL_RenderCopy( g_renderer, g_texture, NULL, &l_dstRect );
                    	SDL_RenderPresent( g_renderer );
                    	break;

                	case SDLK_DOWN:
                		l_dstRect.y += 42;
                    	if ( l_dstRect.y > 373 )
                    	{
                    		l_dstRect.y = 205;
						}
                    	free_texture;
                    	load_Texture( "arrow.png" );
                    	SDL_RenderCopy( g_renderer, g_texture, NULL, &l_dstRect );
                    	SDL_RenderPresent( g_renderer );
                    	break;
                    	
                    case SDLK_RETURN:
                    	switch( l_dstRect.y )
                    	{
                    		case 205:
                    			g_gameState = STATE_GAMEPLAY;
                    			break;
                    			
                    		case 247:
                    			g_gameState = STATE_OPTIONS;
                    			break;
                    			
                    		case 289:
                    			g_gameState = STATE_HIGHSCORE;
                    			break;
                    			
                    		case 331:
                    			g_gameState = STATE_CREDITS;
                    			break;
                    			
                    		case 373:
								g_gameState = 5;
                    			break;
						}
                    	break;
                }
            }
        }
	}
}

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
int close_game( l_renderer, l_window )
{
	// Destroi a janela
	SDL_DestroyRenderer( g_renderer );
	SDL_DestroyWindow( g_window );
	g_window = NULL;
	g_renderer = NULL;

	// Sai dos subsistemas do SDL
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
	
	return 0;
}
