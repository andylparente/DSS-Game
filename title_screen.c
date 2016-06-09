#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "loaders_and_effects.c"

int title_screen_logic( int argc, char *argv[], l_window, l_renderer )
{
	// Flag para fechar o jogo
	int l_quitGame = 0; 
	
	// Imagem PNG em texturizada
	SDL_Texture* l_texture = NULL;
	
	// Musica que sera usada
	Mix_Music* l_music = NULL;	
	
	// Efeito sonoro que sera usado
	Mix_Chunk* l_sfx = NULL;
    
    // Carrega musica
	l_music = Mix_LoadMUS( "sound_music/mp3/music/halloween_hunting.mp3" );
	if( l_music == NULL )
    {
        printf( "Failed to load [sound_music/mp3/music/halloween_hunting.mp3]! SDL_mixer Error: %s\n", Mix_GetError() );
        l_quitGame = 1;
    }

    // Reproduz a musica
    Mix_PlayMusic( l_music, -1 );
	  
    // Efeito de fade in da textura
	l_quitGame = fade_in_texture( "images/non_sprites/title_screen.png", l_window, l_renderer, l_texture );
	
	// Imagem com opacidade normal
	l_quitGame = load_texture( "images/non_sprites/title_screen.png", l_window, l_renderer, l_texture );
	
	// Flag de evento
	SDL_Event e;
	
	while( SDL_PollEvent( &e ) != 0 )
    {
    	// Usuario deseja sair
		if( e.type == SDL_QUIT )
		{
			g_quitGame = 1;
			break;
		}
		
    	if( e.type == SDL_KEYDOWN )
		{
			switch( e.key.keysym.sym )
    		{
           		case SDLK_SPACE:
           			// Limpa a tela para introduzir o efeito de fade out
           			SDL_RenderClear( l_renderer );
           			
				    // Carrega efeito sonoro e o reproduz
    				l_quitGame = load_play_sfx( "sound_music/wav/one_ring_from_church_bell.wav", l_sfx );
    				
    				// Efeito de fade out da musica
    				Mix_FadeOutMusic( 1000 );
           			
           			// Efeito de fade out da textura
					l_quitGame = fade_out_texture( "images/non_sprites/title_screen.png", l_window, l_renderer, l_texture );
					
           			break;
			}
       }
	}
	// Libera imagem carregada
	SDL_DestroyTexture( l_texture );
	l_texture = NULL;
	
	// Libera musica carregada
	Mix_FreeMusic( l_music );
    l_music = NULL;
	
	// Libera o efeito sonoro usado
	Mix_FreeChunk( l_sfx );
	l_sfx = NULL;
	
	return l_quitGame;		
}
