#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "main.c"

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
	
	// Opacidade da imagem
    int l_opacity = 0;
    
    // Carrega musica
	l_music = Mix_LoadMUS( "sound_music/mp3/music/halloween_hunting.mp3" );
	if( l_music == NULL )
    {
        printf( "Failed to load [sound_music/mp3/music/halloween_hunting.mp3]! SDL_mixer Error: %s\n", Mix_GetError() );
        l_quitGame = 1;
    }

    // Reproduz a musica
    Mix_PlayMusic( l_music, -1 );
	  
    // Efeito de fade in
	while( l_opacity < 255 )
	{
		// Carrega a textura
		l_quitGame = load_Texture( "images/non_sprites/title_screen.png", l_window, l_renderer, l_texture );
	
		// Define a opacidade da textura 
		SDL_SetTextureBlendMode( l_texture, SDL_BLENDMODE_BLEND );
		SDL_SetTextureAlphaMod( l_texture, l_opacity );
		SDL_RenderCopy( l_renderer, l_texture, NULL, NULL );
		SDL_SetRenderDrawColor( l_renderer, 255, 255, 255, 255 );
		
		// Opacidade da proxima de textura
		l_opacity += 2;	
		
		// Limpa a tela 
		SDL_RenderPresent( l_renderer );
		SDL_RenderClear( l_renderer );  
		
		// Sai do loop para fechar o jogo
		if( l_quitGame == 1)
		{
			break;
		}
	}
	
	// Imagem com opacidade normal
	l_quitGame = load_Texture( "images/non_sprites/title_screen.png", l_window, l_renderer, l_texture );
	SDL_RenderPresent( l_renderer );
	
	// Flag de evento
	SDL_Event e;
	
	while( SDL_PollEvent( &e ) != 0 )
    {
    	if( e.type == SDL_KEYDOWN )
		{
			switch( e.key.keysym.sym )
    		{
           		case SDLK_SPACE:
           			// Carrega efeitos sonoros
				    l_sfx = Mix_LoadWAV( "sound_music/wav/one_ring_from_church_bell.wav" );
				    if( l_sfx == NULL )
				    {
				        printf( "Failed to load [sound_music/wav/one_ring_from_church_bell.wav]! SDL_mixer Error: %s\n", Mix_GetError() );
				        l_quitGame = 1;
				    }
					
				    // Reproduz o efeito sonoro
				    Mix_PlayChannel( -1, l_sfx, 0 );
					
					l_opacity = 255;
           			
           			// Efeito de fade out
					while( l_opacity > 0)
					{
						l_quitGame = load_Texture( "images/non_sprites/title_screen.png", l_window, l_renderer, l_texture );
						SDL_SetTextureBlendMode( l_texture, SDL_BLENDMODE_BLEND );
						SDL_SetTextureAlphaMod( l_texture, l_opacity );
						SDL_RenderCopy( l_renderer, l_texture, NULL, NULL );
						SDL_SetRenderDrawColor( l_renderer, 255, 255, 255, 255 );
							
						l_opacity -= 2;	
							
						SDL_RenderPresent( l_renderer );
						SDL_RenderClear( l_renderer );
						if( l_quitGame == 1)
						{
							break;
						}   
					}
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
	
	return = l_quitGame;		
}
