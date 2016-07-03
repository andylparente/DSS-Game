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
    SDL_Rect l_arrowPresentedRect; 
    l_arrowPresentedRect.x = 20;
    l_arrowPresentedRect.y = 222;
    l_arrowPresentedRect.w = 30;
    l_arrowPresentedRect.h = 30;

    SDL_Rect l_optionPackPresentedRect; 
    l_optionPackPresentedRect.x = 57;
    l_optionPackPresentedRect.y = 220;
    l_optionPackPresentedRect.w = 300;
    l_optionPackPresentedRect.h = 270;

    SDL_Rect l_optionSelectedPresentedRect; 
    l_optionSelectedPresentedRect.x = 57;
    l_optionSelectedPresentedRect.y = 220;
    l_optionSelectedPresentedRect.w = 300;
    l_optionSelectedPresentedRect.h = 45;

	// Texturas que serao usadas
	SDL_Texture* l_textureMenu = NULL;
    SDL_Texture* l_textureOptionsFull = NULL;
    SDL_Texture* l_textureOption1 = NULL;
    SDL_Texture* l_textureOption2 = NULL;
    SDL_Texture* l_textureOption3 = NULL;
    SDL_Texture* l_textureOption4 = NULL;
    SDL_Texture* l_textureOption5 = NULL;
    SDL_Texture* l_textureOption6 = NULL;
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
        	SDL_RenderCopy( l_renderer, l_textureArrow, NULL, &l_arrowPresentedRect );

            // Carrega as imagens de opcoes do menu
            l_textureOptionsFull = load_texture( "images/non_sprites/opcoes300x270.png", l_window, l_renderer );
            l_textureOption1 = load_texture( "images/non_sprites/op1.png", l_window, l_renderer );
            l_textureOption2 = load_texture( "images/non_sprites/op2.png", l_window, l_renderer );
            l_textureOption3 = load_texture( "images/non_sprites/op3.png", l_window, l_renderer );
            l_textureOption4 = load_texture( "images/non_sprites/op4.png", l_window, l_renderer );
            l_textureOption5 = load_texture( "images/non_sprites/op5.png", l_window, l_renderer );
            l_textureOption6 = load_texture( "images/non_sprites/op6.png", l_window, l_renderer );

            // Area onde a imagem fonte sera aplicada
            SDL_RenderCopy( l_renderer, l_textureOptionsFull, NULL, &l_optionPackPresentedRect );
            SDL_RenderCopy( l_renderer, l_textureOption1, NULL, &l_optionSelectedPresentedRect );

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
                    	l_arrowPresentedRect.y -= 45;
                    	if ( l_arrowPresentedRect.y < 222 )
                    	{
                    		l_arrowPresentedRect.y = 447;
						}
                        SDL_RenderClear( l_renderer );
                        SDL_RenderCopy( l_renderer, l_textureMenu, NULL, NULL );
                        SDL_RenderCopy( l_renderer, l_textureOptionsFull, NULL, &l_optionPackPresentedRect );
                    	SDL_RenderCopy( l_renderer, l_textureArrow, NULL, &l_arrowPresentedRect );
                    	break;

                	case SDLK_DOWN:
                		l_arrowPresentedRect.y += 45;
                    	if ( l_arrowPresentedRect.y > 447 )
                    	{
                    		l_arrowPresentedRect.y = 222;
						}
                        SDL_RenderClear( l_renderer );
                        SDL_RenderCopy( l_renderer, l_textureMenu, NULL, NULL );
                        SDL_RenderCopy( l_renderer, l_textureOptionsFull, NULL, &l_optionPackPresentedRect );
                    	SDL_RenderCopy( l_renderer, l_textureArrow, NULL, &l_arrowPresentedRect );
                    	break;
                    	
                    case SDLK_RETURN:
                    	switch( l_arrowPresentedRect.y )
                    	{
                    		case 222:
                    			l_gameState = 3; //STATE_GAMEPLAY
                    			break;
                    			
                    		case 267:
                    			l_gameState = 4; //STATE_HIGHSCORE
                    			break;
                    			
                    		case 312:
                    			l_gameState = 6; //STATE_OPTIONS
                    			break;
                    			
                    		case 357:
                    			l_gameState = 5; //STATE_CREDITS
                    			break;

                            case 402:
                                l_gameState = 8; //STATE_HISTORY
                                break;
                    			
                    		case 447:
								l_gameState = -1; //Quit
                    			break;
						}
                    	break;
                }

                if( l_arrowPresentedRect.y == 222 )
                {
                    l_optionSelectedPresentedRect.y = 220 + 0*45;
                    SDL_RenderCopy( l_renderer, l_textureOption1, NULL, &l_optionSelectedPresentedRect );
                }
                else if( l_arrowPresentedRect.y == 267 )
                {
                    l_optionSelectedPresentedRect.y = 220 + 1*45;
                    SDL_RenderCopy( l_renderer, l_textureOption2, NULL, &l_optionSelectedPresentedRect );
                }
                else if( l_arrowPresentedRect.y == 312 )
                {
                    l_optionSelectedPresentedRect.y = 220 + 2*45;
                    SDL_RenderCopy( l_renderer, l_textureOption3, NULL, &l_optionSelectedPresentedRect );
                }
                else if( l_arrowPresentedRect.y == 357 )
                {
                    l_optionSelectedPresentedRect.y = 220 + 3*45;
                    SDL_RenderCopy( l_renderer, l_textureOption4, NULL, &l_optionSelectedPresentedRect );
                }
                else if( l_arrowPresentedRect.y == 402 )
                {
                    l_optionSelectedPresentedRect.y = 220 + 4*45;
                    SDL_RenderCopy( l_renderer, l_textureOption5, NULL, &l_optionSelectedPresentedRect );
                }
                else if( l_arrowPresentedRect.y == 447 )
                {
                    l_optionSelectedPresentedRect.y = 220 + 5*45;
                    SDL_RenderCopy( l_renderer, l_textureOption6, NULL, &l_optionSelectedPresentedRect );
                }
            }
        }

        SDL_RenderPresent( l_renderer );
	}

    // Liberas imagens carregadas
    SDL_DestroyTexture( l_textureMenu );
    SDL_DestroyTexture( l_textureArrow );
    SDL_DestroyTexture( l_textureOptionsFull );
    SDL_DestroyTexture( l_textureOption1 );
    SDL_DestroyTexture( l_textureOption2 );
    SDL_DestroyTexture( l_textureOption3 );
    SDL_DestroyTexture( l_textureOption4 );
    SDL_DestroyTexture( l_textureOption5 );
    SDL_DestroyTexture( l_textureOption6 );

    l_textureMenu = NULL;
    l_textureArrow = NULL;
    l_textureOptionsFull = NULL;
    l_textureOption1 = NULL;
    l_textureOption2 = NULL;
    l_textureOption3 = NULL;
    l_textureOption4 = NULL;
    l_textureOption5 = NULL;
    l_textureOption6 = NULL;
    
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
