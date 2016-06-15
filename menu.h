#pragma once
#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "loaders_and_effects.h"

int main_menu_logic( SDL_Window* l_window, SDL_Renderer* l_renderer )
{
	// Estado de jogo selecionado
	int l_gameState = 2;
	
    // Area onde a imagem fonte sera aplicada
    SDL_Rect l_dstRect; 
    l_dstRect.x = 100;
    l_dstRect.y = 205;
    l_dstRect.w = 35;
    l_dstRect.h = 35;

	// Texturas que serao usadas
	SDL_Texture* l_textureMenu = NULL;
	SDL_Texture* l_textureArrow = NULL;

    // Musica que sera usada
    Mix_Music* l_music = NULL;

    // Efeito sonoro que sera usado
    Mix_Chunk* l_sfx = NULL;
	
	// Carrega a imagem do menu principal
	l_textureMenu = load_texture( "images/non_sprites/main_menu.png", l_window, l_renderer );
    if( l_textureMenu == NULL )
    {
        printf( "Failed to load [images/non_sprites/main_menu.png]! SDL_image Error: %s\n", IMG_GetError() );
        l_gameState = -1;
    }

    else
    {    
    	SDL_RenderCopy( l_renderer, l_textureMenu, NULL, NULL );

    	// Carrega a imagem da seta
    	l_textureArrow = load_texture( "images/non_sprites/arrow.png", l_window, l_renderer );
        if( l_textureArrow == NULL )
        {
            printf( "Failed to load [images/non_sprites/arrow.png]! SDL_image Error: %s\n", IMG_GetError() );
            l_gameState = -1;
        }

        else
        {
        	// Area onde a imagem fonte sera aplicada
        	SDL_RenderCopy( l_renderer, l_textureArrow, NULL, &l_dstRect );

            // Apresenta as imagens
        	SDL_RenderPresent( l_renderer );

        	// Carrega musica do menu principal
            l_music = Mix_LoadMUS( "sound_music/mp3/music/suspended.mp3" );
            if( l_music == NULL )
            {
                printf( "Failed to load [sound_music/mp3/music/suspended.mp3]! SDL_mixer Error: %s\n", Mix_GetError() );
                l_gameState = -1;
            }

            else
            {
                // Reproduz a musica
                Mix_PlayMusic( l_music, -1 );

                // Carrega efeito sonoros
                l_sfx = load_sfx( "sound_music/wav/wood_coffin_lid_impact.wav" );
                if( l_sfx == NULL )
                {
                    l_gameState = -1;
                }
            }
        }
    }
    
    // Cuidador de eventos
    SDL_Event e;
    
    // Navega pelo menu e muda o g_gameState
	while( l_gameState == 2 )
    {
        while( SDL_PollEvent( &e ) != 0 )
        {
            if( e.type == SDL_KEYDOWN )
    	    {
    	    	// Reproduz o efeito sonoro
                Mix_PlayChannel( -1, l_sfx, 0 );

                switch( e.key.keysym.sym )
                {
                    case SDLK_BACKSPACE:
                    	l_gameState = 1;
                    	break;

                    case SDLK_UP:
                    	l_dstRect.y -= 42;
                    	if ( l_dstRect.y < 205 )
                    	{
                    		l_dstRect.y = 373;
						}
                        SDL_RenderClear( l_renderer );
                        SDL_RenderCopy( l_renderer, l_textureMenu, NULL, NULL );
                    	SDL_RenderCopy( l_renderer, l_textureArrow, NULL, &l_dstRect );
                    	SDL_RenderPresent( l_renderer );
                    	break;

                	case SDLK_DOWN:
                		l_dstRect.y += 42;
                    	if ( l_dstRect.y > 373 )
                    	{
                    		l_dstRect.y = 205;
						}
                        SDL_RenderClear( l_renderer );
                        SDL_RenderCopy( l_renderer, l_textureMenu, NULL, NULL );
                    	SDL_RenderCopy( l_renderer, l_textureArrow, NULL, &l_dstRect );
                    	SDL_RenderPresent( l_renderer );
                    	break;
                    	
                    case SDLK_RETURN:
                    	switch( l_dstRect.y )
                    	{
                    		case 205:
                    			l_gameState = 3; //STATE_GAMEPLAY
                    			break;
                    			
                    		case 247:
                    			l_gameState = 6; //STATE_OPTIONS
                    			break;
                    			
                    		case 289:
                    			l_gameState = 4; //STATE_HIGHSCORE
                    			break;
                    			
                    		case 331:
                    			l_gameState = 5; //STATE_CREDITS
                    			break;
                    			
                    		case 373:
								l_gameState = -1; //Quit
                    			break;
						}
                    	break;
                }
            }
        }
	}

    // Liberas imagens carregadas
    SDL_DestroyTexture( l_textureMenu );
    SDL_DestroyTexture( l_textureArrow );
    l_textureMenu = NULL;
    l_textureArrow = NULL;
    
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
