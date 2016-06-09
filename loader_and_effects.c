#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

int load_texture( char* l_path, SDL_Window* l_window, SDL_Renderer* l_renderer, SDL_Texture* l_texture )
{
	// Flag para fechar o jogo
	int l_quitGame = 0;
	
	//Imagem PNG em surface e textura
	SDL_Surface* l_loadedSurface = NULL;

	// Carrega a imagem de um diretorio
	l_loadedSurface = IMG_Load( l_path );
	if( l_loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", l_path, IMG_GetError() );
		l_quitGame = 1;
	}
	
	else
	{
		// Gera textura atraves da surface
        l_texture = SDL_CreateTextureFromSurface( l_renderer, l_loadedSurface );
		if( l_texture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", l_path, SDL_GetError() );
			l_quitGame = 1;
		}

		// Se livra da surface que nao sera usada
		SDL_FreeSurface( l_loadedSurface );
		l_loadedSurface = NULL;
	}
	
	return l_quitGame;
}

int fade_in_texture( char* l_path, SDL_Window* l_window, SDL_Renderer* l_renderer, SDL_Texture* l_texture )
{
	l_quitGame = 0;
	
	// Opacidade da imagem
	l_opacity = 0;
	
	while( l_opacity < 255 )
	{
		// Carrega a textura
		l_quitGame = load_texture( "images/non_sprites/title_screen.png", l_window, l_renderer, l_texture );
	
		// Define a opacidade da textura 
		SDL_SetTextureBlendMode( l_texture, SDL_BLENDMODE_BLEND );
		SDL_SetTextureAlphaMod( l_texture, l_opacity );
		SDL_RenderCopy( l_renderer, l_texture, NULL, NULL );
		SDL_SetRenderDrawColor( l_renderer, 255, 255, 255, 255 );
		
		// Opacidade da proxima de textura aumenta
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
	
	return l_quitGame;
}

int fade_out_texture( char* l_path, SDL_Window* l_window, SDL_Renderer* l_renderer, SDL_Texture* l_texture )
{
	l_quitGame = 0;
	l_opacity = 255;
	
	// Efeito de fade out
	while( l_opacity > 0)
	{
		l_quitGame = load_texture( "images/non_sprites/empresa.png", l_window, l_renderer, l_texture );
		SDL_SetTextureBlendMode( l_texture, SDL_BLENDMODE_BLEND );
		SDL_SetTextureAlphaMod( l_texture, l_opacity );
		SDL_RenderCopy( l_renderer, l_texture, NULL, NULL );
		SDL_SetRenderDrawColor( l_renderer, 255, 255, 255, 255 );
		
		// Opacidade da proxima de textura diminui
		l_opacity -= 2;	
		
		SDL_RenderPresent( l_renderer );
		SDL_RenderClear( l_renderer );
		if( l_quitGame == 1)
		{
			break;
		}   
	}
	
	return l_quitGame;
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

int load_play_sfx( char* l_path, SDL_Music* l_music )
{
	int l_quitGame = 0;
	
	// Carrega efeitos sonoros
    l_sfx = Mix_LoadWAV( l_path );
    if( l_sfx == NULL )
    {
        printf( "Failed to load %s [sfx]! SDL_mixer Error: %s\n", l_path, Mix_GetError() );
        l_quitGame = 1;
    }

    // Reproduz o efeito sonoro
   Mix_PlayChannel( -1, l_sfx, 0 );
    
    return l_quitGame;
}

