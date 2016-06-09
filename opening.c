#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "main.c"

int opening_logic( SDL_Window* l_window, SDL_Renderer* l_renderer )
{
	// Flag para fechar o jogo
	int l_quitGame = 0; 
	
	// Efeito sonoro que sera usado
	Mix_Chunk* l_sfx = NULL;
	
	// Imagem PNG em surface e textura
	SDL_Surface* l_loadedSurface = NULL;
	SDL_Texture* l_texture = NULL;
	
	// Opacidade da imagem
    int l_opacity = 0;
	  
    // Efeito de fade in
	while( l_opacity < 255 )
	{
		// Carrega a textura
		l_quitGame = load_Texture( "images/non_sprites/empresa.png", l_window, l_renderer, l_texture );
	
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
	
	// Carrega efeitos sonoros
    l_sfx = Mix_LoadWAV( "sound_music/wav/slide_whistle_up.wav" );
    if( l_sfx == NULL )
    {
        printf( "Failed to load [sound_music/wav/slide_whistle_up.wav]! SDL_mixer Error: %s\n", Mix_GetError() );
        l_quitGame = 1;
    }

    // Reproduz o efeito sonoro
    Mix_PlayChannel( -1, l_sfx, 0 );
	
	SDL_Delay(2000);
	
	l_opacity = 255;

	// Efeito de fade out
	while( l_opacity > 0)
	{
		l_quitGame = load_Texture( "images/non_sprites/empresa.png", l_window, l_renderer, l_texture );
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
	
	// Libera imagem carregada
	SDL_DestroyTexture( l_texture );
	l_texture = NULL;
	
    l_opacity = 0;
    
	while( l_opacity < 255 )
	{
		l_quitGame = load_Texture( "images/non_sprites/sdlcredits.png", l_window, l_renderer, l_texture );
		SDL_SetTextureBlendMode( l_texture, SDL_BLENDMODE_BLEND );
		SDL_SetTextureAlphaMod( l_texture, l_opacity );
		SDL_RenderCopy( l_renderer, l_texture, NULL, NULL );
		SDL_SetRenderDrawColor( l_renderer, 255, 255, 255, 255 );
		l_opacity += 2;	
		SDL_RenderPresent( l_renderer );
		SDL_RenderClear( l_renderer );
		if( l_quitGame == 1)
		{
			break;
		}		  
	}
	
	SDL_Delay(2000);
	
	l_opacity = 255;

	while( l_opacity > 0 )
	{
		l_quitGame = load_Texture( "images/non_sprites/sdlcredits.png", l_window, l_renderer, l_texture );
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
	
	SDL_DestroyTexture( l_texture );
	l_texture = NULL;
	
	// Libera o efeito sonoro usado
	Mix_FreeChunk( l_sfx );
	l_sfx = NULL;
	
	return l_quitGame;
}
