#pragma once
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "loaders_and_effects.h"

int opening_logic( SDL_Window* l_window, SDL_Renderer* l_renderer )
{
	// Flag para ir para o proximo estado
	int l_gameState = 1; 
	
	// Efeito sonoro que sera usado
	Mix_Chunk* l_sfx = NULL;
	
	// Imagem PNG em surface e textura
	SDL_Surface* l_loadedSurface = NULL;
	SDL_Texture* l_texture = NULL;
	
	// Efeito de fade in da textura
	l_gameState = fade_in_texture( "images/non_sprites/empresa.png", l_window, l_renderer, l_gameState );
	if( l_gameState != -1 )
	{
		// Carrega efeito sonoro e o reproduz
	    l_sfx = load_sfx( "sound_music/wav/slide_whistle_up.wav" );
	    if( l_sfx == NULL )
	    {
	        l_gameState = -1;
	    }
	
		else
		{
		    // Reproduz o efeito sonoro
			Mix_PlayChannel( -1, l_sfx, 0 );
		    
		    // Carrega a textura com opacidade normal
			l_texture = load_texture( "images/non_sprites/empresa.png", l_window, l_renderer );
			if( l_texture == NULL )
			{
				printf( "Unable to create texture from [images/non_sprites/empresa.png]! SDL Error: %s\n", SDL_GetError() );
				l_gameState = -1;
			}
			
			else
			{
				// Apresenta a imagem com a opacidade normal
				SDL_RenderCopy( l_renderer, l_texture, NULL, NULL );
				SDL_RenderPresent( l_renderer );
				
				// Espera 1500 milisegundos, ou seja, 1.5 segundos
				SDL_Delay( 1500 );
				
				// Limpa a tela para introduzir o efeito de fade out
				SDL_RenderClear( l_renderer );
				
				// Efeito de fade out da textura
				l_gameState = fade_out_texture( "images/non_sprites/empresa.png", l_window, l_renderer, l_gameState );
				if( l_gameState != -1 )
				{
					// Libera imagem carregada
					SDL_DestroyTexture( l_texture );
					l_texture = NULL;
					
					l_gameState = fade_in_texture( "images/non_sprites/sdlcredits.png", l_window, l_renderer, l_gameState );
					if( l_gameState != -1 )
					{
						l_texture = load_texture( "images/non_sprites/sdlcredits.png", l_window, l_renderer );
						if( l_texture == NULL )
						{
							printf( "Unable to create texture from [images/non_sprites/empresa.png]! SDL Error: %s\n", SDL_GetError() );
							l_gameState = -1;
						}
						
						else
						{
							SDL_RenderCopy( l_renderer, l_texture, NULL, NULL );
							SDL_RenderPresent( l_renderer );
							SDL_Delay( 1000 );
							SDL_RenderClear( l_renderer );
							l_gameState = fade_out_texture( "images/non_sprites/sdlcredits.png", l_window, l_renderer, l_gameState );
						}
					}
				}
			}
		}
	}
	
	SDL_DestroyTexture( l_texture );
	l_texture = NULL;
	
	// Libera o efeito sonoro usado
	Mix_FreeChunk( l_sfx );
	l_sfx = NULL;
	
	return l_gameState;
}
