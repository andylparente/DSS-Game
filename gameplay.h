#pragma once
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "loaders_and_effects.h"
#include <math.h>  

typedef struct _character
{
	// Textura que contera o sprite e informaces da textura
	SDL_Texture* sprite;
	SDL_Texture* spriteDeath;
	SDL_Texture* spriteShooting;
	SDL_Rect snipRect;
	SDL_Rect presentedRect;
	
	// Efeitos sonoros quando for acertado e quando morre
	Mix_Chunk* gotHit;
	Mix_Chunk* deathSound;
	
	// Se esta aparecendo na tela, pontos de vida, velocidade com que se move e dano causado por contato fisico
	int activate;
	int healthPoints;
	float speed;
	int damage;

	// Contador para as animacoes
	int animationCycleCounter;
	int deathAnimationCounter;

	// Para qual direcao e sentido o personagem esta indo (utilizado na IA de inimigos atiradores)
	char direction;
	int way;

	// Tipo de projetil que o personagem atira
	//MagicProjectile type;

} Character;


typedef struct _magic_projectile
{
	SDL_Texture* sprite;
	SDL_Rect snipRect;
	SDL_Rect presentedRect;
	
	int activate;

	// Direcao do projetil
	int direction;

	// Efeitos sonoros quando uma parede e acertada e quando um personagem e acertado
	Mix_Chunk* hitWall;
	Mix_Chunk* hitCharacter;
	
	int damage;
	float speed;
	int animationCycleCounter;
} MagicProjectile;


typedef struct _item
{
	SDL_Texture* sprite;
	SDL_Rect snipRect;
	SDL_Rect presentedRect;

	int activate;
	int animationCycleCounter;
	
	// Efeito sonoro de quando e coletado
	Mix_Chunk* collectedSound;	
} Item;


typedef struct _map
{
	// Textura do mapa
	SDL_Texture* texture;
	SDL_Rect snipRect;
	SDL_Rect presentedRect;
} Map;


// Funcao chamada para atualizar a posicao do inimigo que vai atras do jogador
void chasing( Character* enemy, Character typeOfEnemy, Character player );

// Detecta se ha colisao de sprites atraves dos raios dos sprites, caso o Rect do player seja informado, ele deve ser o primeiro parametro e o int ira diminuir a hitbox
int circular_collision_detection( SDL_Rect aPR, SDL_Rect bPR, int playerHitBoxCorrector );

// Corrige a posicao do player ao tomar um hit, jogando ele para o lado oposto
void circular_sprite_arrangement( SDL_Rect enemyPR, SDL_Rect* playerPR );

// Atualiza a posicao do mapa e do player, em um loop, gera uma animacao 
void scroll_map_animation( char c, Map* map, Character* player );

// Coloca o inimigo na porta correspondente
void passing_through_door( char doorLocation, Character* enemy );

// Inteligencia artificial do beholder
void linear_walk_and_shoot( Character* enemy, Character typeOfEnemy, Character player );


// UHUUUUUUUUUUUUUUUUUHUHUHUUHUHUUUUUUUUUUUUUUUUUU *_* HHHHHHHHHHHHHHHHHHHHUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUHUHUHUUUUUUUUUUUUUUUUUUUUUUUUUU *_* UUUUUUUUUUUHUHUHUHUUUUHUHU //

int gameplay_logic( SDL_Window* l_window, SDL_Renderer* l_renderer )
{
	// Contadores (preguica de colocar l_ para essas variaveis locais)
	int i = 0;
	int i2 = 0;
	int a = 0, x, y;

	// Flag para trocar o estado do jogo
	int l_gameState= 3;
	
	// Personagens que atiram coisas (inclui o jogador)
	Character player;
	Character beholder;
	//Character necromancer;
	//Character demonBoss;

	// Personagens que causam dano somente atraves do toque
	Character skeleton;
	//Character gatekeeper;

	// Tipos de projeteis/tiros
	//MagicProjectile arcaneMissile;
	MagicProjectile fireBall;
	//MagicProjectile iceSpear;
	//MagicProjectile laserBeam;
	//MagicProjectile ghastlyShooting;
	//MagicProjectile demonicBolt;
	
	// Itens que modificam o jogador
	//Item blueRobe;
	//Item redRobe;
	//Item greenRobe;
	
	// Tipos de colecionaveis (geram score)
	Item greenCrystal;
	//Item blueCrystal;
	//Item redCrystal;

	
	// Mapas do jogo
	Map firstMap;
	//Map lastMap;
	
	// Informacoes do player no estado incicial
	player.sprite = load_texture( "images/sprites/sombra80-60.png", l_window, l_renderer );
	player.spriteDeath = load_texture( "images/sprites/sombradeath100-60.png", l_window, l_renderer );
	player.snipRect.x = 320;
	player.snipRect.y = 0;
    player.snipRect.w = 80;
    player.snipRect.h = 60;
	player.presentedRect.x = 800/2 - 80/2;
	player.presentedRect.y = 600/2 - 60/2;
	player.presentedRect.w = 66;
	player.presentedRect.h = 66;
	//player.gotHit = load_sfx( "?" );
	//player.deathSound = load_sfx( "?" );
	player.healthPoints = 100;
	player.speed = 5.0;
	//player.damage = 0;
	player.animationCycleCounter = 0;
	player.deathAnimationCounter = 0;

	// Quantidade de tiros
	//MagicProjectile arcaneMissileV[50];
	MagicProjectile fireBallV[50];
	//MagicProjectile iceSpearV[50];

	// Definindo info do vetor de fireBalls 
	for( a = 0 ; a < 50 ; a++ )
	{
		fireBallV[a].snipRect.x = 0;
		fireBallV[a].snipRect.y = 0;
		fireBallV[a].snipRect.w = 39;
		fireBallV[a].snipRect.h = 20;
		fireBallV[a].presentedRect.w = 46;
		fireBallV[a].presentedRect.h = 46; 
		fireBallV[a].animationCycleCounter = 0;
		fireBallV[a].activate = 0;
	}

	// Informacoes do tipo de projetil
	fireBall.sprite = load_texture( "images/sprites/newfireball39x20.png", l_window, l_renderer );
	//fireBall.hitWall = load_sfx( "?" );
	//fireBall.hitCharacter = load_sfx( "?" );
	fireBall.damage = 50;
	fireBall.speed = 8.0;
	
	// Informacoes do esqueleto (chaser)
	skeleton.sprite = load_texture( "images/sprites/esqueleto55x55.png", l_window, l_renderer );
	skeleton.spriteDeath = load_texture( "images/sprites/esqueletoM55x55.png", l_window, l_renderer );
	//skeleton.gotHit = load_sfx( "?" );
	//skeleton.deathSound = load_sfx( "?" );
	skeleton.healthPoints = 100;
	skeleton.speed = 3.5;
	skeleton.damage = 1;
	
	// Quantidade de inimigos
	Character skeletonWave1[5];
	//Character skeletonWave2[10];

	for( a = 0 ; a < 4 ; a++ )
	{
		skeletonWave1[a].snipRect.x = 0;
		skeletonWave1[a].snipRect.y = 0;
		skeletonWave1[a].snipRect.h = 55;
		skeletonWave1[a].snipRect.w = 55;
		skeletonWave1[a].presentedRect.w = 60;
		skeletonWave1[a].presentedRect.h = 60; 
		skeletonWave1[a].activate = 0;
		skeletonWave1[a].animationCycleCounter = 0;
		skeletonWave1[a].deathAnimationCounter = 0;
	}

	// Informacoes do beholder (shooter)
	beholder.sprite = load_texture( "images/sprites/beholder_90x90.png", l_window, l_renderer );
	beholder.spriteDeath = load_texture( "images/sprites/beholderM90x90.png", l_window, l_renderer );
	//beholder.gotHit = load_sfx( "?" );
	//beholder.deathSound = load_sfx( "?" );
	beholder.healthPoints = 250;
	beholder.speed = 7.0;
	beholder.damage = 2;
	
	// Quantidade de inimigos
	Character beholderWave1[5];
	//Character beholderWave2[10];

	for( a = 0 ; a < 4 ; a++ )
	{
		beholderWave1[a].snipRect.x = 0;
		beholderWave1[a].snipRect.y = 0;
		beholderWave1[a].snipRect.h = 90;
		beholderWave1[a].snipRect.w = 90;
		beholderWave1[a].presentedRect.w = 80;
		beholderWave1[a].presentedRect.h = 80; 
		beholderWave1[a].activate = 0;
		beholderWave1[a].animationCycleCounter = 0;
		beholderWave1[a].deathAnimationCounter = 0;
	}

	// Informacoes do colecionavel
	greenCrystal.sprite = load_texture( "images/sprites/green_crystal.png", l_window, l_renderer );
	//greenCrystal.collectedSound = load_sfx( "?" );

	// Quantidade de itens
	Item greenCrystalV[3];

	for( a = 0 ; a < 3 ; a++ )
	{
		greenCrystalV[a].presentedRect.w = 36;
		greenCrystalV[a].presentedRect.h = 36;
		greenCrystalV[a].activate = 0;
	}
	
	// Informacoes sobre o mapa
	firstMap.texture = load_texture( "images/levels/mapabase.png", l_window, l_renderer );
	firstMap.snipRect.x = 1600;
	firstMap.snipRect.y = 600;
	firstMap.snipRect.w = 800;
	firstMap.snipRect.h = 600;
	firstMap.presentedRect.x = 0;
	firstMap.presentedRect.y = 0;
	firstMap.presentedRect.w = 800;
	firstMap.presentedRect.h = 600;

	SDL_RenderCopy( l_renderer, firstMap.texture, &firstMap.snipRect, &firstMap.presentedRect );
	SDL_RenderCopy( l_renderer, player.sprite, &player.snipRect, &player.presentedRect );
	SDL_RenderPresent( l_renderer );    
	
	// Cuidador de evento
    SDL_Event e;
    
    // Teclas pressionadas (movimentacao utiliza esse tipo)
	const Uint8 *pressedKeyStates = SDL_GetKeyboardState(NULL);

	// Velocidade x e y do player
	float velx = player.speed;
	float vely = player.speed;

	// FPS do jogo
	int fps = 60;

	while( l_gameState == 3 )
	{		
		SDL_RenderCopy( l_renderer, firstMap.texture, &firstMap.snipRect, &firstMap.presentedRect );
        	
        // Onde o projetil sera mostrado na tela
       	for( a = 0 ; a < 50 ; a++ )
		{
			if( fireBallV[a].activate == 0)
			{
				fireBallV[a].presentedRect.x = 900;
				fireBallV[a].presentedRect.y = 900;  
				fireBallV[a].animationCycleCounter = 0;
			}	
			if( fireBallV[a].presentedRect.y < 132 || fireBallV[a].presentedRect.x < 18 || (fireBallV[a].presentedRect.x+fireBallV[a].presentedRect.w) > 780 || (fireBallV[a].presentedRect.y+fireBallV[a].presentedRect.h) > 580 )
			{
				fireBallV[a].activate = 0;
			}

			if( fireBallV[a].activate == 1 )
			{
				switch( fireBallV[a].direction )
				{
					case 1:
						fireBallV[a].presentedRect.y -= fireBall.speed;

						if( fireBallV[a].animationCycleCounter%10 == 0 )
						{
							fireBallV[a].snipRect.x += 30;
							if( fireBallV[a].snipRect.x < 0 || fireBallV[a].snipRect.x > 30 )
							{
								fireBallV[a].snipRect.x = 0;
							}
						}

						fireBallV[a].animationCycleCounter++;
						break;

					case 2:
						fireBallV[a].presentedRect.y += fireBall.speed;
						if( fireBallV[a].animationCycleCounter%10 == 0 )
						{
							fireBallV[a].snipRect.x += 30;
							if( fireBallV[a].snipRect.x < 120 || fireBallV[a].snipRect.x > 150 )
							{
								fireBallV[a].snipRect.x = 120;
							}
						}
						fireBallV[a].animationCycleCounter++;
						break;

					case 3:
						fireBallV[a].presentedRect.x -= fireBall.speed;
						if( fireBallV[a].animationCycleCounter%10 == 0 )
						{
							fireBallV[a].snipRect.x += 30;
							if( fireBallV[a].snipRect.x < 60 || fireBallV[a].snipRect.x > 90 )
							{
								fireBallV[a].snipRect.x = 60;
							}
						}
						fireBallV[a].animationCycleCounter++;
						break;

					case 4:
						fireBallV[a].presentedRect.x += fireBall.speed;
						if( fireBallV[a].animationCycleCounter%8 == 0 )
						{
							fireBallV[a].snipRect.x += 39;
							if( fireBallV[a].snipRect.x < 0 || fireBallV[a].snipRect.x > 350 )
							{
								fireBallV[a].snipRect.x = 0;
							}
						}
						fireBallV[a].animationCycleCounter++;
						break;
				}

				SDL_RenderCopy( l_renderer, fireBall.sprite, &fireBallV[a].snipRect, &fireBallV[a].presentedRect );
			}
		}

        // Onde os inimigos aparecerao na tela
        for( a = 0 ; a < 4 ; a++ )
        {	
        	// Sobre o esqueleto
           	if( skeletonWave1[a].activate == 1 )
           	{
				chasing( &skeletonWave1[a], skeleton, player );

				if ( circular_collision_detection( player.presentedRect, skeletonWave1[a].presentedRect, 5 ) != 0 )
				{
					player.healthPoints -= skeleton.damage;
				}

				for ( i2 = 0 ; i2 < 50 ; i2++ )
				{
					if( circular_collision_detection( fireBallV[i2].presentedRect, skeletonWave1[a].presentedRect, 0 ) != 0 )
					{
						skeletonWave1[a].healthPoints -= fireBall.damage;
						fireBallV[i2].activate = 0;
						if( skeletonWave1[a].healthPoints <= 0 )
						{
							skeletonWave1[a].activate = 2;
						}
					}				
				}

				for( x = 0 ; x < 4 ; x++ )
				{
					circular_sprite_arrangement( skeletonWave1[x].presentedRect, &player.presentedRect );
				}

				SDL_RenderCopy( l_renderer, skeleton.sprite, &skeletonWave1[a].snipRect, &skeletonWave1[a].presentedRect );	
			}
			// Animacao de morte do esqueleto
			if( skeletonWave1[a].activate == 2 )
			{
				if( skeletonWave1[a].deathAnimationCounter == 0)
				{
					skeletonWave1[a].snipRect.x = 0;
				}

				SDL_RenderCopy( l_renderer, skeleton.spriteDeath, &skeletonWave1[a].snipRect, &skeletonWave1[a].presentedRect );

				if( skeletonWave1[a].deathAnimationCounter%30 == 0 )
				{
					skeletonWave1[a].snipRect.x += 55;
				}

				skeletonWave1[a].deathAnimationCounter++;

				if( skeletonWave1[a].deathAnimationCounter == 120 )
				{
					skeletonWave1[a].activate = 0;
				}
			}	
        }

        for( a = 0 ; a < 4 ; a++ )
        {	
        	// Sobre o beholder
           	if( beholderWave1[a].activate == 1 )
           	{
				linear_walk_and_shoot( &beholderWave1[a], beholder, player );

				if ( circular_collision_detection( player.presentedRect, beholderWave1[a].presentedRect, 5 ) != 0 )
				{
					player.healthPoints -= beholder.damage;
				}

				for ( i2 = 0 ; i2 < 50 ; i2++ )
				{
					if( circular_collision_detection( fireBallV[i2].presentedRect, beholderWave1[a].presentedRect, 0 ) != 0 )
					{
						beholderWave1[a].healthPoints -= fireBall.damage;
						fireBallV[i2].activate = 0;
						if( beholderWave1[a].healthPoints <= 0 )
						{
							beholderWave1[a].activate = 2;
						}
					}				
				}

				for( x = 0 ; x < 4 ; x++ )
				{
					circular_sprite_arrangement( beholderWave1[x].presentedRect, &player.presentedRect );
				}

				SDL_RenderCopy( l_renderer, beholder.sprite, &beholderWave1[a].snipRect, &beholderWave1[a].presentedRect );	
			}
			// Animacao de morte do beholder
			if( beholderWave1[a].activate == 2 )
			{
				if( beholderWave1[a].deathAnimationCounter == 0 ) 
				{
					beholderWave1[a].snipRect.x = 0;
				}

				SDL_RenderCopy( l_renderer, beholder.spriteDeath, &beholderWave1[a].snipRect, &beholderWave1[a].presentedRect );

				if( beholderWave1[a].deathAnimationCounter%30 == 0 )
				{
					beholderWave1[a].snipRect.x += 90;
				}

				beholderWave1[a].deathAnimationCounter++;

				if( beholderWave1[a].deathAnimationCounter == 120 )
				{
					beholderWave1[a].activate = 0;
				}
			}
		}	
        

        // Onde os itens aparecerao na tela
        for( a = 0 ; a < 3 ; a++ )
        {
        	if( greenCrystalV[a].activate == 1 )
        	{
        		if( circular_collision_detection( player.presentedRect, greenCrystalV[a].presentedRect, 5 ) != 0 )
        		{
        			greenCrystalV[a].activate = 0;
        		}

        		else
        		{
        			SDL_RenderCopy( l_renderer, greenCrystal.sprite, NULL, &greenCrystalV[a].presentedRect );
        		}
        	}

			else
			{
				greenCrystalV[a].presentedRect.x = rand() % (695 - greenCrystalV[a].presentedRect.w) + 105;
				greenCrystalV[a].presentedRect.y = rand() % (470 - greenCrystalV[a].presentedRect.h) + 130;
			}	
        }

        // Enquanto o jogador esta vivo
        if( player.healthPoints > 0 )
        {
	        // Lista de eventos
			while( SDL_PollEvent( &e ) != 0 )
			{
				// Fechou o jogo clicando no X
	        	if ( e.type == SDL_QUIT )
				{
			    	l_gameState = -1;
			    }

			    // Pressionou uma tecla
				else if ( e.type == SDL_KEYDOWN )
				{
					// Esqueletos aparecem
					if( e.key.keysym.sym == SDLK_RETURN )
					{
						/*for( a = 0 ; a < 4 ; a++ )
						{
							skeletonWave1[a].activate = 1;
						}
						passing_through_door( 'l', &skeletonWave1[0] );
						passing_through_door( 'r', &skeletonWave1[1] );
						passing_through_door( 'u', &skeletonWave1[2] );
						passing_through_door( 'd', &skeletonWave1[3] );*/

						for( a = 0 ; a < 4 ; a++ )
						{
							beholderWave1[a].activate = 1;
						}
						passing_through_door( 'l', &beholderWave1[0] );
						passing_through_door( 'r', &beholderWave1[1] );
						passing_through_door( 'u', &beholderWave1[2] );
						passing_through_door( 'd', &beholderWave1[3] );
					}

					// Itens aparecem
					if( e.key.keysym.sym == SDLK_LCTRL )
					{
						for( a = 0 ; a < 3 ; a++ )
						{
							if( greenCrystalV[a].activate == 0 )
							{
								greenCrystalV[a].activate = 1;
							}
						}                                  
					}

					// Mata o jogador
					if( e.key.keysym.sym == SDLK_SPACE )
					{
						player.healthPoints = -1;                                
					}

					// O player atira 
					if( e.key.keysym.sym == SDLK_UP )
					{
						fireBallV[i].snipRect.x = 0;
						fireBallV[i].presentedRect.x = player.presentedRect.x;
						fireBallV[i].presentedRect.y = player.presentedRect.y; 
						fireBallV[i].direction = 1;
						fireBallV[i].activate = 1;
						fireBallV[i].animationCycleCounter = 0;

						i++;

						if( i > 49 )
						{
							i = 0;
						}

						if( player.animationCycleCounter%5 == 0 )
						{
							player.snipRect.x += 80;
						}
						if( player.snipRect.x < 640 || player.snipRect.x > 800 )
					   	{
					   		player.snipRect.x = 640;
						}
						player.animationCycleCounter++;
					}

					else if( e.key.keysym.sym == SDLK_DOWN )
					{
						fireBallV[i].snipRect.x = 120;
						fireBallV[i].presentedRect.x = player.presentedRect.x;
						fireBallV[i].presentedRect.y = player.presentedRect.y; 
						fireBallV[i].direction = 2;
						fireBallV[i].activate = 1;
						fireBallV[i].animationCycleCounter = 0;
						i++;
						if( i > 49 )
						{
							i = 0;
						}
						if( player.animationCycleCounter%5 == 0 )
						{
							player.snipRect.x += 80;
						}
						if( player.snipRect.x < 880 || player.snipRect.x > 1040 )
					   	{
					   		player.snipRect.x = 880;
						}
						player.animationCycleCounter++;
					}

					else if( e.key.keysym.sym == SDLK_LEFT )
					{
						fireBallV[i].snipRect.x = 60;
						fireBallV[i].presentedRect.x = player.presentedRect.x;
						fireBallV[i].presentedRect.y = player.presentedRect.y; 
						fireBallV[i].direction = 3;
						fireBallV[i].activate = 1;
						fireBallV[i].animationCycleCounter = 0;
						i++;
						if( i > 49 )
						{
							i = 0;
						}
						if( player.animationCycleCounter%5 == 0 )
						{
							player.snipRect.x += 80;
						}
						if( player.snipRect.x < 1600 || player.snipRect.x > 1680 )
					   	{
					   		player.snipRect.x = 1600;
						}
						player.animationCycleCounter++;
					}

					else if( e.key.keysym.sym == SDLK_RIGHT )
					{
						//fireBallV[i].snipRect.x = 0;
						fireBallV[i].presentedRect.x = player.presentedRect.x;
						fireBallV[i].presentedRect.y = player.presentedRect.y; 
						fireBallV[i].direction = 4;
						fireBallV[i].activate = 1;
						//fireBallV[i].animationCycleCounter = 0;
						i++;
						if( i > 49 )
						{
							i = 0;
						}
						/*if( player.animationCycleCounter%5 == 0 )
						{
							player.snipRect.x += 80;
						}
						if( player.snipRect.x < 1280 || player.snipRect.x > 1360 )
					   	{
					   		player.snipRect.x = 1280;
						}
						player.animationCycleCounter++;*/
					}
				}

			    // Soltou uma tecla
				else if ( e.type == SDL_KEYUP )
				{
				    if( e.key.keysym.sym == SDLK_d )
				    {
						// Reseta o ciclo de animacao
						player.animationCycleCounter = 0;

						// A velocidade se torna zero, logo o player nao andara mais	
				    	velx = 0.0;
					}
					if( e.key.keysym.sym == SDLK_a )
					{
						player.animationCycleCounter = 0; 
						velx = 0.0;
					}
					if( e.key.keysym.sym == SDLK_w )
					{		
						player.animationCycleCounter = 0;
						vely = 0.0;
					}
					if( e.key.keysym.sym == SDLK_s )
					{
						player.animationCycleCounter = 0;
						vely = 0.0;
					}
				}
			}
			
				
			// Pressionou uma tecla de movimentacao
	  		if( pressedKeyStates[SDL_SCANCODE_D] )
	   		{
				// Velocidade do player que sera acrescentada a posicao atual	
	   			velx = player.speed;
			   			
	   			// Define a velociade que ele troca o sprite
	   			if( player.animationCycleCounter%10 == 0 )
				{
					player.snipRect.x += 80;
				}
				player.animationCycleCounter++;

				//Ciclo de animacao 
	   			if( player.snipRect.x < 1120 || player.snipRect.x > 1200 )
		   		{
		   			player.snipRect.x = 1120;
				}
			}		
			if( pressedKeyStates[SDL_SCANCODE_A] )
			{ 
		   		velx = (-1)*player.speed;
		   		if( player.animationCycleCounter%10 == 0 )
				{
					player.snipRect.x += 80;
				}
				if( player.snipRect.x < 1440 || player.snipRect.x > 1520 )
				{
					player.snipRect.x = 1440;
				}
				player.animationCycleCounter++;
			}				
			if( pressedKeyStates[SDL_SCANCODE_W] )
			{
		   		vely = (-1)*player.speed;
		   		if( player.animationCycleCounter%10 == 0 )
				{
					player.snipRect.x += 80;
				}
				if( player.snipRect.x < 160 || player.snipRect.x > 240 )
			   	{
			   		player.snipRect.x = 160;
				}
				player.animationCycleCounter++;
			}			
			if( pressedKeyStates[SDL_SCANCODE_S] )
			{
		   		vely = player.speed;
		   		if( player.animationCycleCounter%10 == 0 )
				{
					player.snipRect.x += 80;
				}
				if( player.snipRect.x < 0 || player.snipRect.x > 80 )
			   	{	
			   		player.snipRect.x = 0;
				}
				player.animationCycleCounter++;
			}

			// Atualiza a posicao do player somando a velocidade
			player.presentedRect.x += velx;
			player.presentedRect.y += vely;

			// Se os esqueletos estao desativados e possivel sair da sala
			if( skeletonWave1[0].activate == 0 && skeletonWave1[1].activate == 0 && skeletonWave1[2].activate == 0 && skeletonWave1[3].activate == 0 )
			{
				// Permite passar pelas portas
				if( player.presentedRect.x <= 19 )
				{
					if( (player.presentedRect.y < 259) || (player.presentedRect.y+player.presentedRect.h > 340) )
					{
						player.presentedRect.x = 20;
					}
				}
				if( player.presentedRect.x+player.presentedRect.w >= 780 )
				{
					if( (player.presentedRect.y < 259) || (player.presentedRect.y+player.presentedRect.h > 340) )
					{
						player.presentedRect.x = 779 - player.presentedRect.w;
					}
				}
				if( player.presentedRect.y <= 129 )
				{
					if( (player.presentedRect.x < 359) || (player.presentedRect.x+player.presentedRect.w > 440) )
					{
						player.presentedRect.y = 130;
					}
				}
				if( player.presentedRect.y+player.presentedRect.h >= 580 )
				{
					if( (player.presentedRect.x < 359) || (player.presentedRect.x+player.presentedRect.w > 440) )
					{
						player.presentedRect.y = 579 - player.presentedRect.h;
					}
				}

				// Permite a transicao dos mapas
				if( player.presentedRect.x == (-1)*(player.presentedRect.w-20) )
				{
					puts( "Tentou ir pra eskerda\n" );
					for( a = 0 ; a < 160 ; a++ )
					{
						SDL_RenderClear( l_renderer );
						scroll_map_animation( 'l', &firstMap, &player );
						SDL_RenderCopy( l_renderer, firstMap.texture, &firstMap.snipRect, &firstMap.presentedRect );
						SDL_RenderCopy( l_renderer, player.sprite, &player.snipRect, &player.presentedRect );
						SDL_RenderPresent( l_renderer );
						SDL_Delay( 1000/fps );
					}
					player.presentedRect.x -= 5;
				}
				if( player.presentedRect.x == 780 )
				{
					puts( "Tentou ir pra direita\n" );
					for( a = 0 ; a < 160 ; a++ )
					{
						SDL_RenderClear( l_renderer );
						scroll_map_animation( 'r', &firstMap, &player );
						SDL_RenderCopy( l_renderer, firstMap.texture, &firstMap.snipRect, &firstMap.presentedRect );
						SDL_RenderCopy( l_renderer, player.sprite, &player.snipRect, &player.presentedRect );
						SDL_RenderPresent( l_renderer );
						SDL_Delay( 1000/fps );
					}
					player.presentedRect.x += 5;
				}
				if( player.presentedRect.y == (-1)*(player.presentedRect.h-20) )
				{
					puts( "Tentou ir pra cima\n" );
					for( a = 0 ; a < 120 ; a++ )
					{
						SDL_RenderClear( l_renderer );
						scroll_map_animation( 'u', &firstMap, &player );
						SDL_RenderCopy( l_renderer, firstMap.texture, &firstMap.snipRect, &firstMap.presentedRect );
						SDL_RenderCopy( l_renderer, player.sprite, &player.snipRect, &player.presentedRect );
						SDL_RenderPresent( l_renderer );
						SDL_Delay( 1000/fps );
					}
					player.presentedRect.y += 5;
				}
				if( player.presentedRect.y == 580 )
				{
					puts( "Tentou ir pra baixo\n" );
					for( a = 0 ; a < 120 ; a++ )
					{
						SDL_RenderClear( l_renderer );
						scroll_map_animation( 'd', &firstMap, &player );
						SDL_RenderCopy( l_renderer, firstMap.texture, &firstMap.snipRect, &firstMap.presentedRect );
						SDL_RenderCopy( l_renderer, player.sprite, &player.snipRect, &player.presentedRect );
						SDL_RenderPresent( l_renderer );
						SDL_Delay( 1000/fps );
					}
					player.presentedRect.y -= 5;
				}
			}
			

			else
			{
				if( player.presentedRect.x <= 19 )
				{
					player.presentedRect.x = 20;
				}
				else if( player.presentedRect.x+player.presentedRect.w >= 780 )
				{
					player.presentedRect.x = 779 - player.presentedRect.w;
				}
				if( player.presentedRect.y <= 129 )
				{
					player.presentedRect.y = 130;
				}
				else if( player.presentedRect.y+player.presentedRect.h >= 580 )
				{
					player.presentedRect.y = 579 - player.presentedRect.h;
				}
			}

			SDL_RenderCopy( l_renderer, player.sprite, &player.snipRect, &player.presentedRect );
		}

		// Animacao de morte do jogador
		else
		{	
			if( player.deathAnimationCounter == 0 )
			{
				player.snipRect.x = 0;
			}

			SDL_RenderCopy( l_renderer, player.spriteDeath, &player.snipRect, &player.presentedRect );

			if( player.deathAnimationCounter%20 == 0 )
			{
				player.snipRect.x += 100;
			}

			player.deathAnimationCounter++;

			if( player.deathAnimationCounter == 180 )
			{
				l_gameState = 2;
			}

			if( player.presentedRect.x <= 19 )
			{
				player.presentedRect.x = 20;
			}
			else if( player.presentedRect.x+player.presentedRect.w >= 780 )
			{
				player.presentedRect.x = 779 - player.presentedRect.w;
			}
			if( player.presentedRect.y <= 129 )
			{
				player.presentedRect.y = 130;
			}
			else if( player.presentedRect.y+player.presentedRect.h >= 580 )
			{
				player.presentedRect.y = 579 - player.presentedRect.h;
			}
		}

		SDL_RenderPresent( l_renderer );
		SDL_Delay( 1000/fps );
		SDL_RenderClear( l_renderer );
	}
	
	// Libera imagens carregadas
	SDL_DestroyTexture( player.sprite );
	SDL_DestroyTexture( firstMap.texture );
	SDL_DestroyTexture( skeleton.sprite );
	SDL_DestroyTexture( beholder.sprite );
	SDL_DestroyTexture( greenCrystal.sprite);
	player.sprite = NULL;
	firstMap.texture = NULL;
	skeleton.sprite = NULL;
	beholder.sprite = NULL;
	greenCrystal.sprite = NULL;
	
	// Limpa o renderizador
	SDL_RenderClear( l_renderer );	
	
	return l_gameState;
}

// FFFFFFFFFFFFFFFFFFFFFFFFFFUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUNCOES ESTAO AKI!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// FFFFFFFFFFFFFFFFFFFFFFFFFFUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUNCOES ESTAO AKI!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// FFFFFFFFFFFFFFFFFFFFFFFFFFUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUNCOES ESTAO AKI!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

void chasing( Character* enemy, Character typeOfEnemy, Character player )
{
	if( (player.presentedRect.x + player.presentedRect.w/2) > (enemy->presentedRect.x + enemy->presentedRect.w/2) )
	{
		enemy->presentedRect.x += typeOfEnemy.speed;
	}
	if( (player.presentedRect.x + player.presentedRect.w/2) < (enemy->presentedRect.x + enemy->presentedRect.w/2) )
	{
		enemy->presentedRect.x -= typeOfEnemy.speed;
	}
	if( (player.presentedRect.y + player.presentedRect.h/2) > (enemy->presentedRect.y + enemy->presentedRect.h/2) )
	{
		enemy->presentedRect.y += typeOfEnemy.speed;
	}
	if( (player.presentedRect.y + player.presentedRect.h/2) < (enemy->presentedRect.y + enemy->presentedRect.h/2) )
	{
		enemy->presentedRect.y -= typeOfEnemy.speed;
	}
}

int circular_collision_detection( SDL_Rect aPR, SDL_Rect bPR, int playerHitBoxCorrector )
{
	int YES = 1, NO = 0;

	// Raio do sprite
	float aRadius = (float)(aPR.w/2.0 - 2*playerHitBoxCorrector);
	float bRadius = (float)bPR.w/2.0;

	// Definindo a coordenada X do centro do sprite a
	int aux = aPR.x + aPR.w/2;
	float aAxisX = (float)aux;

	aux = bPR.x + bPR.w/2;
	float bAxisX = (float)aux;
	aux = aPR.y + aPR.h/2;
	float aAxisY = (float)aux;
	aux = bPR.y + bPR.h/2;
	float bAxisY = (float)aux;

	// Se a distancia entre os centros dos sprites e igual a soma dos raios
	if( sqrt( pow( aPR.x-bPR.x , 2.0 ) + pow( aPR.y-bPR.y , 2.0 ) )  >= aRadius+bRadius )
	{
		return NO;
	}

	else
	{
		return YES;
	}
}

void circular_sprite_arrangement( SDL_Rect enemyPR, SDL_Rect* playerPR )
{
	int occur;
	// Raio do sprite
	float aRadius = (float)enemyPR.w/2.0;
	float bRadius = (float)playerPR->w/2.0;

	// Definindo a coordenada X do centro do sprite a
	int aux = enemyPR.x + enemyPR.w/2;
	float aAxisX = (float)aux;

	aux = playerPR->x + playerPR->w/2;
	float bAxisX = (float)aux;
	aux = enemyPR.y + enemyPR.h/2;
	float aAxisY = (float)aux;
	aux = playerPR->y + playerPR->h/2;
	float bAxisY = (float)aux;

	// Se a distancia entre os centros dos sprites e igual ou maior a soma dos raios
	while( sqrt( pow( enemyPR.x-playerPR->x , 2.0 ) + pow( enemyPR.y-playerPR->y , 2.0 ) )  <= aRadius+bRadius )
	{
		if( playerPR->x < enemyPR.x )
		{
			playerPR->x--;
			occur = 1;
		}

		else if( playerPR->x > enemyPR.x )
		{
			playerPR->x++;
			occur = 2;
		}

		if( playerPR->y < enemyPR.y )
		{
			playerPR->y--;
			occur = 3;
		}

		else if( playerPR->y > enemyPR.y )
		{
			playerPR->y++;
			occur = 4;
		}
	}

	switch( occur )
	{
		case 1:
			playerPR->x -= 10;
			break;
		case 2:
			playerPR->x += 10;
			break;
		case 3:
			playerPR->y -= 10;
			break;
		case 4:
			playerPR->y += 10;
			break;
	}
}

void scroll_map_animation( char c, Map* map, Character* player )
{
	switch( c )
	{
		case 'u':
			map->snipRect.y -= 5;
			player->presentedRect.y += 5;
			break;

		case 'd':
			map->snipRect.y += 5;
			player->presentedRect.y -= 5;
			break;

		case 'l':
			map->snipRect.x -= 5;
			player->presentedRect.x += 5;
			break;

		case 'r':
			map->snipRect.x += 5;
			player->presentedRect.x -= 5;
			break;
	}
}

void passing_through_door( char doorLocation, Character* enemy )
{
	switch( doorLocation )
	{
		case 'u':
			enemy->presentedRect.x = 400 - enemy->presentedRect.w/2;
			enemy->presentedRect.y = 131;
			break;

		case 'd':
			enemy->presentedRect.x = 400 - enemy->presentedRect.w/2;
			enemy->presentedRect.y = 580 - enemy->presentedRect.h;
			break;

		case 'l':
			enemy->presentedRect.x = 21;
			enemy->presentedRect.y = 300 - enemy->presentedRect.h/2;
			break;

		case 'r':
			enemy->presentedRect.x = 780 - enemy->presentedRect.w;
			enemy->presentedRect.y = 300 - enemy->presentedRect.h/2;
			break;
	}

	enemy->direction = doorLocation;
}

void linear_walk_and_shoot( Character* enemy, Character typeOfEnemy, Character player )
{
	// Andar horizontal 
	if( enemy->direction == 'u' || enemy->direction == 'd' ) 
	{
		if( (enemy->presentedRect.x+enemy->presentedRect.w/2) >= (player.presentedRect.x+25) && (enemy->presentedRect.x+enemy->presentedRect.w/2) <= (player.presentedRect.x+41) )
		{
			enemy->damage++; //Tirar isso
		}

		else
		{
			if( enemy->way == 0 ) // Esquerda
			{
				if( enemy->presentedRect.x > 21 )
				{
					enemy->presentedRect.x -= typeOfEnemy.speed;
				}
				else
				{
					enemy->way = 1;
				}
			}
			else if( enemy->way == 1 ) // Direta
			{
				if( enemy->presentedRect.x+enemy->presentedRect.w < (780 - enemy->presentedRect.w) )
				{
					enemy->presentedRect.x += typeOfEnemy.speed;
				}
				else
				{
					enemy->way = 0;
				}	
			}
		}
	}
	// Andar vertical
	else if( enemy->direction == 'l' || enemy->direction ==  'r' )
	{
		if( (enemy->presentedRect.y+enemy->presentedRect.h/2) >= (player.presentedRect.y+25) && (enemy->presentedRect.y+enemy->presentedRect.h/2) <= (player.presentedRect.y+41) )
		{
			enemy->damage++; //Tirar isso // Atirar
		}
		else
		{
			if( enemy->way == 0 ) // Cima
			{
				if( enemy->presentedRect.y > 131 )
				{
					enemy->presentedRect.y -= typeOfEnemy.speed;
				}
				else
				{
					enemy->way = 1;
				}
			}
			else if( enemy->way == 1 ) // Baixa
			{
				if( enemy->presentedRect.y+enemy->presentedRect.h < (580 - enemy->presentedRect.h) )
				{
					enemy->presentedRect.y += typeOfEnemy.speed;
				}
				else
				{
					enemy->way = 0;
				}	
			}
		}
	} 
}