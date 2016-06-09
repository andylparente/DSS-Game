#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "loaders_and_effects.c"

int opening_logic( SDL_Window* l_window, SDL_Renderer* l_renderer )
{
	// Flag para fechar o jogo
	int l_quitGame = 0; 
	
	// Efeito sonoro que sera usado
	Mix_Chunk* l_sfx = NULL;
	
	// Imagem PNG em surface e textura
	SDL_Surface* l_loadedSurface = NULL;
	SDL_Texture* l_texture = NULL;
	
	// Efeito de fade in da textura
	l_quitGame = fade_in_texture( "images/non_sprites/empresa.png", l_window, l_renderer, l_texture );
	
	// Carrega efeito sonoro e o reproduz
    l_quitGame = load_play_sfx( "sound_music/wav/slide_whistle_up.wav", l_sfx );
    
    // Carrega a textura com opacidade normal
	l_quitGame = load_texture( "images/non_sprites/empresa.png", l_window, l_renderer, l_texture );
	
	// Espera 1500 milisegundos, ou seja, 1.5 segundos
	SDL_Delay( 1500 );
	
	// Limpa a tela para introduzir o efeito de fade out
	SDL_RenderClear( l_renderer );
	
	// Efeito de fade out da textura
	l_quitGame = fade_out_texture( "images/non_sprites/empresa.png", l_window, l_renderer, l_texture );
	
	// Libera imagem carregada
	SDL_DestroyTexture( l_texture );
	l_texture = NULL;
	
	l_quitGame = fade_in_texture( "images/non_sprites/sdlcredits.png", l_window, l_renderer, l_texture );
	l_quitGame = load_texture( "images/non_sprites/sdlcredits.png", l_window, l_renderer, l_texture );
	SDL_Delay( 1000 );
	SDL_RenderClear( l_renderer );
	l_quitGame = fade_out_texture( "images/non_sprites/sdlcredits.png", l_window, l_renderer, l_texture );
	
	SDL_DestroyTexture( l_texture );
	l_texture = NULL;
	
	// Libera o efeito sonoro usado
	Mix_FreeChunk( l_sfx );
	l_sfx = NULL;
	
	return l_quitGame;
}
