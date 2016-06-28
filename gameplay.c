#pragma once
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "loaders_and_effects.h"

typedef struct _inimigo
{
	// Textura que contera o sprite e informaces da textura
	SDL_Texture* sprite;
	SDL_Rect snipRect;
	SDL_Rect presentedRect;
	
	// Efeitos sonoros quando for acertado e quando morre
	Mix_Chunk* gotHit;
	Mix_Chunk* deathSound;
	
	// Pontos de vida, velocidade com que se move e dano causado por contato fisico
	int activate;
	int healthPoints;
	float speed;
	int damage;
} Inimigo;



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
	//MagicProjectile type;
} Shooter;

int i2 = 0;
int i = 0;
int a = 0;

typedef struct _magic_projectile
{
	SDL_Texture* sprite;
	SDL_Rect snipRect;
	SDL_Rect presentedRect;
	
	int activate;
	int direction;
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
	// Flag para trocar o estado do jogo
	int l_gameState= 3;
	
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
	Inimigo skeleton;
	//Inimigo gatekeeper;
	
	// Itens que modificam o jogador
	//Item blueRobe;
	//Item redRobe;
	//Item greenRobe;
	
	// Tipos de colecionaveis (geram score)
	//Collectible greenCrystal;
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
	//player.healthPoints = 100;
	player.speed = 5.0;
	//player.damage = 0;


	//quantidade de tiros
	MagicProjectile V[50];


	for( a=0;a<50;a++ )
	{
	
	V[a].snipRect.x = 0;
	V[a].snipRect.y = 0;
	V[a].snipRect.h = 30;
	V[a].snipRect.w = 30;
	V[a].presentedRect.x = 900;
	V[a].presentedRect.y = 900; 
	V[a].presentedRect.w = 30;
	V[a].presentedRect.h = 30; 
	
	}

	// Informacoes do projetil
	fireBall.sprite = load_texture( "images/sprites/fireball30x30.png", l_window, l_renderer );
	//fireBall.hitWall = load_sfx( "?" );
	//fireBall.hitCharacter = load_sfx( "?" );
	fireBall.damage = 50;
	fireBall.speed = 2.0;
	
	//  Informacoes do inimigo
	skeleton.sprite = load_texture( "images/sprites/esqueleto_80x80.png", l_window, l_renderer );
	//skeleton.gotHit = load_sfx( "?" );
	//skeleton.deathSound = load_sfx( "?" );
	skeleton.healthPoints = 100;
	skeleton.speed = 1.5;
	skeleton.damage = 30;
	
	//quantidade de inimigos
	Inimigo E[5];

	for( a=0; a<5; a++ )
	{
		E[a].snipRect.x = 0;
		E[a].snipRect.y = 0;
		E[a].snipRect.h = 80;
		E[a].snipRect.w = 80;
		E[a].presentedRect.x = 0;
		E[a].presentedRect.y = (0 + a*100); 
		E[a].presentedRect.w = 80;
		E[a].presentedRect.h = 80; 
		E[a].activate = 0;
	}

	// Informacoes do colecionavel
	//greenCrystal.sprite = load_texture( "images/sprites/green_crystal.png", l_window, l_renderer );
	//greenCrystal.collectedSound = load_sfx( "?" );
	
	// Informacoes sobre o mapa
	firstMap.texture = load_texture( "images/levels/4_doors.png", l_window, l_renderer );


	SDL_RenderCopy( l_renderer, firstMap.texture, NULL, NULL );
	SDL_RenderCopy( l_renderer, player.sprite, &player.snipRect, &player.presentedRect );
	//SDL_RenderCopy( l_renderer, skeleton.sprite, &skeleton.snipRect, &skeleton.presentedRect );
	SDL_RenderPresent( l_renderer );    
	
	// Cuidador de evento
    SDL_Event e;
    
	const Uint8 *keystates = SDL_GetKeyboardState(NULL);
/*
	// Definindo teclas para atirar
	int keyUpIsDown;
	int keyDownIsDown;
	int keyLeftIsDown;
	int keyRightIsDown;
*/
	// Definindo teclas para movimentar-se	
	int keyWIsDown;
	int keySIsDown;
	int keyAIsDown;
	int keyDIsDown;
	int keyZIsDown;
	int keyUPIsDown;
	int keyDOWNIsDown;
	int keyLEFTIsDown;
	int keyRIGHTIsDown;


	while( l_gameState == 3 )
	{

		SDL_RenderClear( l_renderer );		
		SDL_RenderCopy( l_renderer, firstMap.texture, NULL, NULL );
		SDL_RenderCopy( l_renderer, player.sprite, &player.snipRect, &player.presentedRect );
        	
        	for(a=0; a<50; a++)
			{	
				if(V[a].presentedRect.y <= 0 || V[a].presentedRect.x <= 0 || V[a].presentedRect.x > 800 || V[a].presentedRect.y > 800 )
				{
					V[a].activate = 0;
				}					
				if(V[a].activate == 1){
					switch( V[a].direction )
					{
						case 1:

							V[a].presentedRect.y -= fireBall.speed;
							break;

						case 2:

							V[a].presentedRect.y += fireBall.speed;
							break;

						case 3:

							V[a].presentedRect.x -= fireBall.speed;
							break;

						case 4:

							V[a].presentedRect.x += fireBall.speed;
							break;

						case 5:

							V[a].presentedRect.y -= fireBall.speed;
							V[a].presentedRect.x -= fireBall.speed;
							break;

						case 6:

							V[a].presentedRect.y -= fireBall.speed;
							V[a].presentedRect.x += fireBall.speed;
							break;

						case 7:

							V[a].presentedRect.y += fireBall.speed;
							V[a].presentedRect.x -= fireBall.speed;
							break;

						case 8:

							V[a].presentedRect.y += fireBall.speed;
							V[a].presentedRect.x += fireBall.speed;
							break;
					}
				
					SDL_RenderCopy( l_renderer, fireBall.sprite, &V[a].snipRect, &V[a].presentedRect );
				}
			}
           
            for( a=0; a<5; a++)
            {
            	if (E[a].activate == 1)
            	{
					if( player.presentedRect.x > E[a].presentedRect.x )
					{
						E[a].presentedRect.x += skeleton.speed;
					}
					if( player.presentedRect.x < E[a].presentedRect.x )
					{
						E[a].presentedRect.x -= skeleton.speed;
					}
					if( player.presentedRect.y > E[a].presentedRect.y )
					{
						E[a].presentedRect.y += skeleton.speed;
					}
					if( player.presentedRect.y < E[a].presentedRect.y )
					{
						E[a].presentedRect.y -= skeleton.speed;
					}
					for (i2=0; i2<50; i2++)
					{
						if( V[i2].presentedRect.x == E[a].presentedRect.x && V[i2].presentedRect.y == E[a].presentedRect.y )
						{
							E[a].activate = 0;
						}
					}
					SDL_RenderCopy( l_renderer, skeleton.sprite, &E[a].snipRect, &E[a].presentedRect );
				}							
            }

        SDL_RenderPresent( l_renderer );

		while( SDL_PollEvent( &e ) != 0 )
		{

			if( e.type == SDL_QUIT )
            {
                l_gameState = -1;
            }

            
    		if( e.type == SDL_KEYDOWN )
			{
				// Definindo teclas com o scancode
				keyWIsDown = keystates[SDL_SCANCODE_W];
				keySIsDown = keystates[SDL_SCANCODE_S];
				keyAIsDown = keystates[SDL_SCANCODE_A];
				keyDIsDown = keystates[SDL_SCANCODE_D];
				keyUPIsDown = keystates[SDL_SCANCODE_UP];
				keyDOWNIsDown = keystates[SDL_SCANCODE_DOWN];
				keyLEFTIsDown = keystates[SDL_SCANCODE_LEFT];
				keyRIGHTIsDown = keystates[SDL_SCANCODE_RIGHT];

				if( keyWIsDown && keyDIsDown )
				{
					player.presentedRect.x += player.speed;
					player.presentedRect.y -= player.speed;
					player.snipRect.x -= 100;
					if( player.snipRect.x < 200 || player.snipRect.x > 300 )
	    			{
	    				player.snipRect.x = 200;
					}
				} 
				else if( keyWIsDown && keyAIsDown )
				{
					player.presentedRect.x -= player.speed;
					player.presentedRect.y -= player.speed;
					player.snipRect.x -= 100;
					if( player.snipRect.x < 200 || player.snipRect.x > 300 )
	    			{
	    				player.snipRect.x = 200;
					}
				}
				else if( keySIsDown && keyDIsDown )
				{
					player.presentedRect.x += player.speed;
					player.presentedRect.y += player.speed;
					player.snipRect.x += 100;
					if( player.snipRect.x < 0 || player.snipRect.x > 100 )
	    			{
	    				player.snipRect.x = 0;
					}
				}
				else if( keySIsDown && keyAIsDown )
				{
					player.presentedRect.x -= player.speed;
					player.presentedRect.y += player.speed;
					player.snipRect.x += 100;
					if( player.snipRect.x < 0 || player.snipRect.x > 100 )
	    			{
	    				player.snipRect.x = 0;
					}
				}
				else if(keyUPIsDown && keyLEFTIsDown )
				{

					V[i].presentedRect.x = player.presentedRect.x;
					V[i].presentedRect.y = player.presentedRect.y; 
					V[i].direction = 5;
					V[i].activate = 1;

					i = i+1;

					if(i>49){
						i = 0;
					}
	
				}
				else if(keyUPIsDown && keyRIGHTIsDown )
				{

					V[i].presentedRect.x = player.presentedRect.x;
					V[i].presentedRect.y = player.presentedRect.y; 
					V[i].direction = 6;
					V[i].activate = 1;

					i = i+1;

					if(i>49){
						i = 0;
					}
	
				}
				else if(keyDOWNIsDown && keyLEFTIsDown )
				{

					V[i].presentedRect.x = player.presentedRect.x;
					V[i].presentedRect.y = player.presentedRect.y; 
					V[i].direction = 7;
					V[i].activate = 1;

					i = i+1;

					if(i>49){
						i = 0;
					}
	
				}
				else if(keyDOWNIsDown && keyRIGHTIsDown )
				{

					V[i].presentedRect.x = player.presentedRect.x;
					V[i].presentedRect.y = player.presentedRect.y; 
					V[i].direction = 8;
					V[i].activate = 1;

					i = i+1;

					if(i>49){
						i = 0;
					}
	
				}
				else if( keyUPIsDown )
				{

					V[i].presentedRect.x = player.presentedRect.x;
					V[i].presentedRect.y = player.presentedRect.y; 
					V[i].direction = 1;
					V[i].activate = 1;

					i = i+1;

					if(i>49){
						i = 0;
					}
	
				}
				else if( keyDOWNIsDown )
				{

					V[i].presentedRect.x = player.presentedRect.x;
					V[i].presentedRect.y = player.presentedRect.y; 
					V[i].direction = 2;
					V[i].activate = 1;

					i = i+1;

					if(i>49){
						i = 0;
					}
	
				}
				else if( keyLEFTIsDown )
				{

					V[i].presentedRect.x = player.presentedRect.x;
					V[i].presentedRect.y = player.presentedRect.y; 
					V[i].direction = 3;
					V[i].activate = 1;

					i = i+1;

					if(i>49){
						i = 0;
					}
	
				}
				else if(keyRIGHTIsDown)
				{

					V[i].presentedRect.x = player.presentedRect.x;
					V[i].presentedRect.y = player.presentedRect.y; 
					V[i].direction = 4;
					V[i].activate = 1;

					i = i+1;

					if(i>49){
						i = 0;
					}
	
				} 
				else
				{
					
           			switch( e.key.keysym.sym )
        			{
						case SDLK_d:	
        					player.presentedRect.x += player.speed;
        					player.snipRect.x += 100;
        					if( player.snipRect.x < 1400 || player.snipRect.x > 1500 )
			    			{
			    				player.snipRect.x = 1400;
							}
							break;
	
						case SDLK_a: 
							player.presentedRect.x -= player.speed;
							player.snipRect.x -= 100;
							if( player.snipRect.x < 1800 || player.snipRect.x > 1900 )
			    			{
			    				player.snipRect.x = 1800;
							}
							break;
	
						case SDLK_w:
							player.presentedRect.y -= player.speed;
							player.snipRect.x -= 100;
							if( player.snipRect.x < 200 || player.snipRect.x > 300 )
			    			{
			    				player.snipRect.x = 200;
							}
							break;
	
						case SDLK_s:
							player.presentedRect.y += player.speed;
							player.snipRect.x += 100;
							if( player.snipRect.x < 0 || player.snipRect.x > 100 )
			    			{
			    				player.snipRect.x = 0;
							}
							break;
						
						case SDLK_RETURN:
							
							for(a=0; a<5; a++)
							{
								E[a].activate = 1;
							}                                  
							break;
        			}
    			}	
    		}
    		
    		else
			{
    			player.snipRect.x += 80;
    			if( player.snipRect.x < 400 || player.snipRect.x >700 )
    			{
    				player.snipRect.x = 400;
				}
			}
    	}
	}
	
	// Libera imagens carregadas
	SDL_DestroyTexture( player.sprite );
	SDL_DestroyTexture( firstMap.texture );
	player.sprite = NULL;
	firstMap.texture = NULL;
	
	// Limpa o renderizador
	SDL_RenderClear( l_renderer );	
	
	return l_gameState;
}
