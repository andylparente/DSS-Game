#pragma once
#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "loaders_and_effects.h"

int options_logic( SDL_Window* l_window, SDL_Renderer* l_renderer )
{
	// Estado de jogo selecionado
	int l_gameState = 6;

	// Textura que sera usada
	SDL_Texture* l_texture = NULL;

	// Efeito de fade in da textura
	l_gameState = fade_in_texture( "images/non_sprites/breve.png", l_window, l_renderer, l_gameState );
	if( l_gameState != -1 )
	{
		// Carrega a textura com opacidade normal
		l_texture = load_texture( "images/non_sprites/breve.png", l_window, l_renderer );
		if( l_texture == NULL )
		{
			printf( "Unable to create texture from [images/non_sprites/breve.png]! SDL Error: %s\n", SDL_GetError() );
			l_gameState = -1;
		}
		
		else
		{
			// Apresenta a imagem com a opacidade normal
			SDL_RenderCopy( l_renderer, l_texture, NULL, NULL );
			SDL_RenderPresent( l_renderer );
		}
	}

	// Flag de evento
	SDL_Event e;
	
	while( l_gameState == 6 )
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
	        			l_gameState = 2;
	        			break;

	        		case SDLK_RETURN:
	        			l_gameState = 2;
	        			break;
				}
			}
		}
	}
	// Libera imagem carregada
	SDL_DestroyTexture( l_texture );
	l_texture = NULL;

	// Libera o renderizador
	SDL_RenderClear( l_renderer );
	
	return l_gameState;		
}