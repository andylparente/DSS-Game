#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "main.c"

int main_menu_logic( int argc, char *argv[], SDL_Window* l_window, SDL_Renderer* l_renderer )
{
	// Estado de jogo selecionado
	int l_gameState = 0;
	
	// Textura que sera usada
	SDL_Texture* l_texture = NULL;
	
	// Carrega a imagem do menu principal
	l_gameState = load_Texture( "main_menu.png", l_window, l_renderer, l_texture );
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
                    			l_gameState = STATE_GAMEPLAY;
                    			break;
                    			
                    		case 247:
                    			l_gameState = STATE_OPTIONS;
                    			break;
                    			
                    		case 289:
                    			l_gameState = STATE_HIGHSCORE;
                    			break;
                    			
                    		case 331:
                    			l_gameState = STATE_CREDITS;
                    			break;
                    			
                    		case 373:
								l_gameState = 5;
                    			break;
						}
                    	break;
                }
            }
        }
	}
}
