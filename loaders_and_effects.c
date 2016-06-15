#pragma once
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

SDL_Texture* load_texture( char* l_path, SDL_Window* l_window, SDL_Renderer* l_renderer )
{
	// Surface auxiliar e textura que sera utilizada
	SDL_Surface* l_auxSurface = NULL;
	SDL_Texture* l_newTexture = NULL;

	// Carrega a imagem de um diretorio
	l_auxSurface = IMG_Load( l_path );
	
	// Gera uma textura atraves da surface auxiliar
	l_newTexture = SDL_CreateTextureFromSurface( l_renderer, l_auxSurface );

	// Se livra da surface que nao sera usada
	SDL_FreeSurface( l_auxSurface );
	l_auxSurface = NULL;
	
	return l_newTexture;
}

int fade_in_texture( char* l_path, SDL_Window* l_window, SDL_Renderer* l_renderer, int l_gameState )
{	
	// Imagem PNG em textura
	SDL_Texture* l_texture = NULL;
	
	// Opacidade da imagem
	int l_opacity = 0;
	
	while( l_opacity < 255 )
	{
		// Carrega a textura
		l_texture = load_texture( l_path, l_window, l_renderer );
	
		// Define a opacidade da textura 
		SDL_SetTextureBlendMode( l_texture, SDL_BLENDMODE_BLEND );
		SDL_SetTextureAlphaMod( l_texture, l_opacity );
		SDL_RenderCopy( l_renderer, l_texture, NULL, NULL );
		SDL_SetRenderDrawColor( l_renderer, 255, 255, 255, 255 );
		
		// Apresenta a imagem
		SDL_RenderPresent( l_renderer );
		
		// Opacidade da proxima textura aumenta
		l_opacity += 4;	
		
		// Limpa a tela 
		SDL_RenderClear( l_renderer );  
		
		// Sai do loop para fechar o jogo
		if( l_texture == NULL )
		{
			l_gameState = -1;
			printf( "Failed to load and fade in texture [%s]!", l_path ); 
			break;
		}
	}
	return l_gameState;
}

int fade_out_texture( char* l_path, SDL_Window* l_window, SDL_Renderer* l_renderer, int l_gameState )
{
	SDL_Texture* l_texture = NULL;
	int l_opacity = 255;
	
	// Efeito de fade out
	while( l_opacity > 0 )
	{
		l_texture = load_texture( l_path, l_window, l_renderer );
		SDL_SetTextureBlendMode( l_texture, SDL_BLENDMODE_BLEND );
		SDL_SetTextureAlphaMod( l_texture, l_opacity );
		SDL_RenderCopy( l_renderer, l_texture, NULL, NULL );
		SDL_SetRenderDrawColor( l_renderer, 255, 255, 255, 255 );
		SDL_RenderPresent( l_renderer );
		
		// Opacidade da proxima textura diminui
		l_opacity -= 4;	
		
		SDL_RenderClear( l_renderer );
		if( l_texture == NULL )
		{
			l_gameState = -1;
			printf( "Failed to load and fade out texture [%s]!", l_path ); 
			break;
		}   
	}
	return l_gameState;
}

/*int load_music( char* l_path, SDL_Music* l_music )
{	
	int l_quitGame = 0;
	
	// Carrega musica
	l_music = Mix_LoadMUS( l_path );
	if( l_music == NULL )
    {
        printf( "Failed to load %s [music]! SDL_mixer Error: %s\n", l_path, Mix_GetError() );
        l_quitGame = 1;
    }
    // Reproduz a musica
    Mix_PlayMusic( l_music, -1 );
    
    return l_quitGame;
}*/

Mix_Chunk* load_sfx( char* l_path )
{
	// Efeito sonoro que sera usado
	Mix_Chunk* l_sfx = NULL;

	// Carrega efeitos sonoros
    l_sfx = Mix_LoadWAV( l_path );
    if( l_sfx == NULL )
    {
       printf( "Failed to load %s [sfx]! SDL_mixer Error: %s\n", l_path, Mix_GetError() );
    }
    return l_sfx;
}
