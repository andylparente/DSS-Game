#pragma once
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "loaders_and_effects.h"

int title_screen_logic( SDL_Window* l_window, SDL_Renderer* l_renderer )
{
	// Flag do estado de jogo atual
	int l_gameState = 1; 
	
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
        l_gameState = -1;
    }
    
    else
    {
	    // Reproduz a musica
	    Mix_PlayMusic( l_music, -1 );
		  
	    // Efeito de fade in da textura
		l_gameState = fade_in_texture( "images/non_sprites/title_screen.png", l_window, l_renderer, l_gameState );
		if( l_gameState != -1 )
		{
			// Carrega a textura com opacidade normal
			l_texture = load_texture( "images/non_sprites/title_screen.png", l_window, l_renderer );
			if( l_texture == NULL )
			{
				printf( "Unable to create texture from [images/non_sprites/title_screen.png]! SDL Error: %s\n", SDL_GetError() );
				l_gameState = -1;
			}
			
			else
			{
				// Apresenta a imagem com a opacidade normal
				SDL_RenderCopy( l_renderer, l_texture, NULL, NULL );
				SDL_RenderPresent( l_renderer );
			}
		}
	}

	// Flag de evento
	SDL_Event e;
	
	while( l_gameState == 1 )
	{
		while( SDL_PollEvent( &e ) != 0 )
	    {
	    	// Usuario deseja sair
			if( e.type == SDL_QUIT )
			{
				l_gameState = -1;
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
	    				l_sfx = load_sfx( "sound_music/wav/one_ring_from_church_bell.wav" );
	    				if( l_sfx == NULL )
	    				{
							l_gameState = -1;
	    				}
	    				
						else
						{
		    				// Reproduz o efeito sonoro
							Mix_PlayChannel( -1, l_sfx, 0 );
		    				
		    				// Efeito de fade out da musica
		    				Mix_FadeOutMusic( 1000 );
		    				
		        			// Efeito de fade out da textura
							l_gameState = fade_out_texture( "images/non_sprites/title_screen.png", l_window, l_renderer, l_gameState );
							if( l_gameState != -1 )
							{
								// Avanca para o proximo estado de jogo
								l_gameState = 2;
							}
						}
	        			break;
				}
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

	// Libera o renderizador
	SDL_RenderClear( l_renderer );
	
	return l_gameState;		
}
