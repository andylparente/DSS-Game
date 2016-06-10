#pragma once
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "loaders_and_effects.h"

typedef struct _melee_damager
{
	// Textura que contera o sprite e informaces da textura
	SDL_Texture* sprite;
	SDL_Rect snipRect;
	SDL_Rect presentedRect;
	
	// Efeitos sonoros quando for acertado e quando morre
	Mix_Chunk* gotHit;
	Mix_Chunk* deathSound;
	
	// Pontos de vida, velocidade com que se move e dano causado por contato fisico
	int healthPoints;
	float speed;
	int damage;
} MeleeDamager;


typedef struct _shooter
{
	SDL_Texture* sprite;
	SDL_Rect snipRect;
	SDL_Rect presentedRect;
	
	Mix_Chunk* gotHitSound;
	Mix_Chunk* deathSound;

	int healthPoints;
	float speed;
	int damage;
	
	// Tipo de projetil que o personagem atira
	MagicProjectile type;
} Shooter;


typedef struct _magic_projectile
{
	SDL_Texture* sprite;
	SDL_Rect snipRect;
	SDL_Rect presentedRect;
	
	// Efeitos sonoros quando uma parede e acertada e quando um personagem e acertado
	Mix_Chunk* hitWall;
	Mix_Chunk* hitCharacter;
	
	int damage;
	float speed;
} MagicProjectile;


typedef struct _item
{
	SDL_Texture* sprite;
	SDL_Rect snipRect;
	SDL_Rect presentedRect;
	
	// Efeito sonoro de quando e coletado
	Mix_Chunk* collectedSound;	
} Item;


typedef struct _collectible
{
	SDL_Texture* sprite;
	SDL_Rect snipRect;
	SDL_Rect presentedRect;
	
	Mix_Chunk* collectedSound
} Collectible;


typedef struct _map
{
	// Textura do mapa
	SDL_Texture* texture;
	SDL_Rect snipRect;
} Map;



int gameplay_logic( SDL_Window* l_window, SDL_Renderer* l_renderer )
{
	// Personagens que atiram coisas (inclui o jogador)
	Shooter player;	
	//Shooter beholder;
	//Shooter necromancer;
	//Shooter demonBoss;

	// Tipos de projeteis/tiros
	//MagicProjectile arcaneMissile;
	MagicProjectile fireBall;
	//MagicProjectile iceSpear;
	//MagicProjectile laserBeam;
	//MagicProjectile ghastlyShooting;
	//MagicProjectile demonicBolt;

	// Personagens que causam dano somente atraves do toque
	MeleeDamager skeleton;
	//MeleeDamager gatekeeper;
	
	// Itens que modificam o jogador
	//Item blueRobe;
	//Item redRobe;
	//Item greenRobe;
	
	// Tipos de colecionaveis (geram score)
	Collectible greenCrystal;
	//Collectible blueCrystal;
	//Collectible redCrystal;
	
	// Mapas do jogo
	Map firstMap;
	//Map lastMap;
	
	// Informacoes do player no estado incicial
	player.sprite = load_texture( "images/sprites/sombra100-60.png", l_window, l_renderer );
	player.snipRect.x = 0;
	player.snipRect.y = 0;
    player.snipRect.w = 80;
    player.snipRect.h = 80;
	player.presentedRect.x = 800/2;
	player.presentedRect.y = 600/2;
	player.presentedRect.w = 80;
	player.presentedRect.h = 80; 
	//player.gotHit = load_sfx( "?" );
	//player.deathSound = load_sfx( "?" );
	player.healthPoints = 100;
	player.speed = 5.0;
	//player.damage = 0;
	
	// Informacoes do projetil
	fireBall.sprite = load_texture( "images/sprites/fireball30x30.png", l_window, l_renderer );
	//fireBall.hitWall = load_sfx( "?" );
	//fireBall.hitCharacter = load_sfx( "?" );
	fireBall.damage = 50;
	
	//  Informacoes do inimigo
	skeleton.sprite = load_texture( "images/sprites/esqueleto_80x80.png", l_window, l_renderer );
	//skeleton.gotHit = load_sfx( "?" );
	//skeleton.deathSound = load_sfx( "?" );
	skeleton.healthPoints = 100;
	skeleton.speed = 3.5;
	skeleton.damage = 30;
	
	// Informacoes do colecionavel
	greenCrystal.sprite = load_texture( "images/sprites/green_crystal.png", l_window, l_renderer );
	//greenCrystal.collectedSound = load_sfx( "?" );
	
	// Informacoes sobre o mapa
	firstMap.texture = load_texture( "images/levels/4_doors.png", l_window, l_renderer );
		 
	SDL_RenderCopy( l_renderer, firstMap.texture, NULL, NULL );
	SDL_RenderCopy( l_renderer, player.sprite, &player.snipRect, &player.presentedRect );
	SDL_RenderCopy( l_renderer, skeleton.sprite, &skeleton.snipRect, &skeleton.presentedRect );
	SDL_RenderPresent( l_renderer );    
	
    SDL_Event e;
    
	const Uint8 *keystates = SDL_GetKeyboardState(NULL);

	// Definindo teclas
	int cima_pressionado;
	int baixo_pressionado;
	int esquerda_pressionado;
	int direita_pressionado;

	while( g_gameState == 3 )
	{

		while( SDL_PollEvent( &e ) != 0 )
		{
			if( e.type == SDL_QUIT )
            {
                g_gameState = 5;
            }
			/*
			SDL_GetMouseState(&x, &y);
            		l_dstRect.x = x;
                	l_dstRect.y = y;
			*/
	    	SDL_RenderClear( g_renderer );		
			SDL_RenderCopy( g_renderer, g_texture, NULL, NULL );
			SDL_RenderCopy( g_renderer, g_pele, &l_srcRect, &l_dstRect );
        	SDL_RenderPresent( g_renderer );
        	
       		if( e.type == SDL_KEYDOWN )
			{
				// Definindo teclas
				cima_pressionado = keystates[SDL_SCANCODE_UP];
				baixo_pressionado = keystates[SDL_SCANCODE_DOWN];
				esquerda_pressionado = keystates[SDL_SCANCODE_LEFT];
				direita_pressionado = keystates[SDL_SCANCODE_RIGHT];

				if( cima_pressionado && direita_pressionado )
				{
					l_dstRect.x += velx;
					l_dstRect.y -= velx;
				} 
				else if( cima_pressionado && esquerda_pressionado )
				{
					l_dstRect.x -= velx;
					l_dstRect.y -= velx;
				}
				else if( baixo_pressionado && direita_pressionado )
				{
					l_dstRect.x += velx;
					l_dstRect.y += velx;
				}
				else if( baixo_pressionado && esquerda_pressionado )
				{
					l_dstRect.x -= velx;
					l_dstRect.y += velx;
				}
				else
				{
           			switch( e.key.keysym.sym )
        			{
						case SDLK_RIGHT:	
	        					l_dstRect.x += velx;
							break;
	
						case SDLK_LEFT: 
							l_dstRect.x -= velx;
							break;
	
						case SDLK_UP:
							l_dstRect.y -= velx;
							//l_srcRect.x -= l_srcRect.w;
							break;
	
						case SDLK_DOWN:
							l_dstRect.y += velx;
							//l_srcRect.x -= l_srcRect.w; 
							break;
	
						case SDLK_RETURN:
							g_gameState = 5;                                     
							break;
        			}
    			}	
    		}
    	}
	}
}
