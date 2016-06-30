#pragma once
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "loaders_and_effects.h"

typedef struct _character
{
	// Textura que contera o sprite e informaces da textura
	SDL_Texture* sprite;
	SDL_Texture* spriteDeath;
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

	// Tipo de projetil que o personagem atira
	//MagicProjectile type;

} Character;

/*
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
} Shooter; */

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
} MagicProjectile;


typedef struct _item
{
	SDL_Texture* sprite;
	SDL_Rect snipRect;
	SDL_Rect presentedRect;

	int activate;
	
	// Efeito sonoro de quando e coletado
	Mix_Chunk* collectedSound;	
} Item;

/*
typedef struct _collectible
{
	SDL_Texture* sprite;
	SDL_Rect snipRect;
	SDL_Rect presentedRect;
	
	Mix_Chunk* collectedSound;
} Collectible; */


typedef struct _map
{
	// Textura do mapa
	SDL_Texture* texture;
	SDL_Rect snipRect;
	SDL_Rect presentedRect;
} Map;

// Funcoes para diversas coisas rxrxrxrxrx
void chasing( Character* enemy, Character typeOfEnemy, Character player );
void sprite_arrangement( Character* movingChar, Character staticChar );
int magicProjectile_collision( MagicProjectile mP, Character character );
int item_collision( Item item, Character player );
int on_player_collision( Character enemy, Character player );


// UHUUUUUUUUUUUUUUUUUHUHUHUUHUHUUUUUUUUUUUUUUUUUU *_* HHHHHHHHHHHHHHHHHHHHUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUHUHUHUUUUUUUUUUUUUUUUUUUUUUUUUU *_* UUUUUUUUUUUHUHUHUHUUUUHUHU //

int gameplay_logic( SDL_Window* l_window, SDL_Renderer* l_renderer )
{
	// Contadores (preguica de colocar l_ para essas variaveis locais)
	int i = 0;
	int i2 = 0;
	int a = 0, x, y, l_deathAnimationCounter = 0;

	// Flag para trocar o estado do jogo
	int l_gameState= 3;
	
	// Personagens que atiram coisas (inclui o jogador)
	Character player;
	//Character beholder;
	//Character necromancer;
	//Character demonBoss;

	// Tipos de projeteis/tiros
	//MagicProjectile arcaneMissile;
	MagicProjectile fireBall;
	//MagicProjectile iceSpear;
	//MagicProjectile laserBeam;
	//MagicProjectile ghastlyShooting;
	//MagicProjectile demonicBolt;

	// Personagens que causam dano somente atraves do toque
	Character skeleton;
	//Character gatekeeper;
	
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
	player.sprite = load_texture( "images/sprites/sombra100-60.png", l_window, l_renderer );
	player.spriteDeath = load_texture( "images/sprites/sombradeath100-60.png", l_window, l_renderer );
	player.snipRect.x = 400;
	player.snipRect.y = 0;
    player.snipRect.w = 100;
    player.snipRect.h = 60;
	player.presentedRect.x = 800/2;
	player.presentedRect.y = 600/2;
	player.presentedRect.w = 80;
	player.presentedRect.h = 80;
	//player.gotHit = load_sfx( "?" );
	//player.deathSound = load_sfx( "?" );
	player.healthPoints = 100;
	player.speed = 5.0;
	//player.damage = 0;


	// Quantidade de tiros
	//MagicProjectile arcaneMissileV[50];
	MagicProjectile fireBallV[50];
	//MagicProjectile iceSpearV[50];

	// Definindo info do vetor de fireBalls 
	for( a = 0 ; a < 50 ; a++ )
	{
	
	fireBallV[a].snipRect.x = 0;
	fireBallV[a].snipRect.y = 0;
	fireBallV[a].snipRect.h = 30;
	fireBallV[a].snipRect.w = 30;
	fireBallV[a].presentedRect.x = 900;
	fireBallV[a].presentedRect.y = 900; 
	fireBallV[a].presentedRect.w = 30;
	fireBallV[a].presentedRect.h = 30; 
	
	}

	// Informacoes do tipo de projetil
	fireBall.sprite = load_texture( "images/sprites/fireball30x30.png", l_window, l_renderer );
	//fireBall.hitWall = load_sfx( "?" );
	//fireBall.hitCharacter = load_sfx( "?" );
	fireBall.damage = 50;
	fireBall.speed = 8.0;
	
	// Informacoes do inimigo
	skeleton.sprite = load_texture( "images/sprites/esqueleto55x55.png", l_window, l_renderer );
	//skeleton.spriteDeath = load_texture( "?", l_window, l_renderer );
	//skeleton.gotHit = load_sfx( "?" );
	//skeleton.deathSound = load_sfx( "?" );
	skeleton.healthPoints = 100;
	skeleton.speed = 3.5;
	skeleton.damage = 1;
	
	// Quantidade de inimigos
	Character skeletonV[5];

	for( a = 0 ; a < 5 ; a++ )
	{
		skeletonV[a].snipRect.x = 0;
		skeletonV[a].snipRect.y = 0;
		skeletonV[a].snipRect.h = 55;
		skeletonV[a].snipRect.w = 55;
		skeletonV[a].presentedRect.x = 0;
		skeletonV[a].presentedRect.y = (0 + a*100); 
		skeletonV[a].presentedRect.w = 80;
		skeletonV[a].presentedRect.h = 80; 
		skeletonV[a].activate = 0;
	}

	// Informacoes do colecionavel
	greenCrystal.sprite = load_texture( "images/sprites/green_crystal.png", l_window, l_renderer );
	//greenCrystal.collectedSound = load_sfx( "?" );

	// Quantidade de itens
	Item greenCrystalV[3];

	for( a = 0 ; a < 3 ; a++ )
	{
		greenCrystalV[a].presentedRect.w = 20;
		greenCrystalV[a].presentedRect.h = 20;
		greenCrystalV[a].activate = 0;
	}
	
	// Informacoes sobre o mapa
	firstMap.texture = load_texture( "images/levels/4_doors.png", l_window, l_renderer );
	firstMap.presentedRect.x = 0;
	firstMap.presentedRect.y = 0;
	firstMap.presentedRect.w = 800;
	firstMap.presentedRect.h = 600;

	SDL_RenderCopy( l_renderer, firstMap.texture, NULL, &firstMap.presentedRect );
	SDL_RenderCopy( l_renderer, player.sprite, &player.snipRect, &player.presentedRect );
	//SDL_RenderCopy( l_renderer, skeleton.sprite, &skeleton.snipRect, &skeleton.presentedRect );
	SDL_RenderPresent( l_renderer );    
	
	// Cuidador de evento
    SDL_Event e;
    
	const Uint8 *keystates = SDL_GetKeyboardState(NULL);

	// Velocidade x e y do player
	float velx = player.speed;
	float vely = player.speed;

	// FPS do jogo
	int fps = 60;

	while( l_gameState == 3 )
	{		
		SDL_RenderCopy( l_renderer, firstMap.texture, NULL, NULL );
        	
        // Onde o projetil sera mostrado na tela
       	for( a = 0 ; a < 50 ; a++ )
		{	
			if( fireBallV[a].presentedRect.y <= 0 || fireBallV[a].presentedRect.x <= 0 || fireBallV[a].presentedRect.x > 800 || fireBallV[a].presentedRect.y > 800 )
			{
				fireBallV[a].activate = 0;
			}

			if( fireBallV[a].activate == 1 )
			{
				switch( fireBallV[a].direction )
				{
					case 1:
						fireBallV[a].presentedRect.y -= fireBall.speed;
						break;

					case 2:
						fireBallV[a].presentedRect.y += fireBall.speed;
						break;

					case 3:
						fireBallV[a].presentedRect.x -= fireBall.speed;
						break;

					case 4:
						fireBallV[a].presentedRect.x += fireBall.speed;
						break;
				}

				SDL_RenderCopy( l_renderer, fireBall.sprite, &fireBallV[a].snipRect, &fireBallV[a].presentedRect );
			}
		}

        // Onde o inimigo aparecerao na tela
        for( a = 0 ; a < 5 ; a++ )
        {
           	if ( skeletonV[a].activate == 1 )
           	{
				chasing( &skeletonV[a], skeleton, player );

				if ( on_player_collision( skeletonV[a], player ) != 0)
				{
					player.healthPoints -= skeleton.damage;
				}

				for ( i2 = 0 ; i2 < 50 ; i2++ )
				{
					if( magicProjectile_collision( fireBallV[i2], skeletonV[a] ) != 0 )
					{
						skeletonV[a].activate = 0;
					}				
				}

				for( x = 0 ; x < 5 ; x++ )
				{
					for( y = 0 ; y < 5 ; y++ )
					{
						if( x != y )
						{
							sprite_arrangement( &skeletonV[x], skeletonV[y] );
						}
					}
					sprite_arrangement( &skeletonV[x], player );
				}
				SDL_RenderCopy( l_renderer, skeleton.sprite, &skeletonV[a].snipRect, &skeletonV[a].presentedRect );
			}							
        }

        // Onde os itens aparecerao na tela
        for( a = 0 ; a < 3 ; a++ )
        {
        	if( greenCrystalV[a].activate == 1 )
        	{
        		if( item_collision( greenCrystalV[a], player ) != 0 )
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
				greenCrystalV[a].presentedRect.x = rand() % (695 - 20) + 105;
				greenCrystalV[a].presentedRect.y = rand() % (470 - 20) + 130;
			}	
        }

        // Tudo e mostrado
        //SDL_RenderPresent( l_renderer );

        if( player.healthPoints > 0 )
        {
	        // Lista de eventos
			while( SDL_PollEvent( &e ) != 0 )
			{
	        	switch( e.type )
				{
					case SDL_QUIT:
			            l_gameState = -1;
			            break;
					
					// Pressionou uma tecla
			  		case SDL_KEYDOWN:
			   			switch( e.key.keysym.sym )
			   			{
							case SDLK_d:	
			   					velx = 5.0;
			  					/*player.snipRect.x += 100;
			   					if( player.snipRect.x < 1400 || player.snipRect.x > 1500 )
				    			{
				    				player.snipRect.x = 1400;
								} */
								break;
					
							case SDLK_a: 
				   				velx = -5.0;
								/*player.snipRect.x -= 100;
								if( player.snipRect.x < 1800 || player.snipRect.x > 1900 )
					   			{
					   				player.snipRect.x = 1800;
								}*/
								break;
					
							case SDLK_w:
				   				vely = -5.0;
								/*player.snipRect.x -= 100;
								if( player.snipRect.x < 200 || player.snipRect.x > 300 )
					   			{
					   				player.snipRect.x = 200;
								}*/
								break;
					
							case SDLK_s:
				   				vely = 5.0;
								/*player.snipRect.x += 100;
								if( player.snipRect.x < 0 || player.snipRect.x > 100 )
					   			{	
					   				player.snipRect.x = 0;
								}*/
								break;
					
							case SDLK_RETURN:
								for( a = 0 ; a < 5 ; a++ )
								{
									skeletonV[a].activate = 1;
								}                                  
								break;

							case SDLK_LCTRL:
								for( a = 0 ; a < 3 ; a++ )
								{
									if( greenCrystalV[a].activate == 0 )
									{
										greenCrystalV[a].activate = 1;
									}
								}                                  
								break;

							case SDLK_UP:
								fireBallV[i].presentedRect.x = player.presentedRect.x;
								fireBallV[i].presentedRect.y = player.presentedRect.y; 
								fireBallV[i].direction = 1;
								fireBallV[i].activate = 1;

								i++;

								if( i > 49 )
								{
									i = 0;
								}
								break;

							case SDLK_DOWN:
								fireBallV[i].presentedRect.x = player.presentedRect.x;
								fireBallV[i].presentedRect.y = player.presentedRect.y; 
								fireBallV[i].direction = 2;
								fireBallV[i].activate = 1;
								i++;
								if( i > 49 )
								{
									i = 0;
								}
								break;

							case SDLK_LEFT:
								fireBallV[i].presentedRect.x = player.presentedRect.x;
								fireBallV[i].presentedRect.y = player.presentedRect.y; 
								fireBallV[i].direction = 3;
								fireBallV[i].activate = 1;
								i++;
								if( i > 49 )
								{
									i = 0;
								}
								break;

							case SDLK_RIGHT:
								fireBallV[i].presentedRect.x = player.presentedRect.x;
								fireBallV[i].presentedRect.y = player.presentedRect.y; 
								fireBallV[i].direction = 4;
								fireBallV[i].activate = 1;
								i++;
								if( i > 49 )
								{
									i = 0;
								}
								break;
			        	}
			        	break;

			        // Soltou uma tecla
				    case SDL_KEYUP:
				    	switch( e.key.keysym.sym )
				        {
							case SDLK_d:	
				        		velx = 0.0;
								break;
					
							case SDLK_a: 
								velx = 0.0;
								break;
					
							case SDLK_w:
								vely = 0.0;
								break;
					
							case SDLK_s:
								vely = 0.0;
								break;
				        }
				        break;
				    
				    // Moveu o mouse		
				    case SDL_MOUSEMOTION:
				    	break;

				    // Fez nada
				    default:
						/*player.snipRect.x += 100;
						if( player.snipRect.x < 400 || player.snipRect.x > 700 )
						{
							player.snipRect.x = 400;
						} */
						break;
				}
				player.presentedRect.x += velx;
				player.presentedRect.y += vely;   
			}

			SDL_RenderCopy( l_renderer, player.sprite, &player.snipRect, &player.presentedRect );
		}

		else
		{	
			if( l_deathAnimationCounter == 0 )
			{
				player.snipRect.x = 0;
			}

			SDL_RenderCopy( l_renderer, player.spriteDeath, &player.snipRect, &player.presentedRect );

			if( l_deathAnimationCounter%20 == 0 )
			{
				player.snipRect.x += 100;
			}

			l_deathAnimationCounter++;

			if( l_deathAnimationCounter == 180 )
			{
				l_gameState = 2;
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
	SDL_DestroyTexture( greenCrystal.sprite);
	player.sprite = NULL;
	firstMap.texture = NULL;
	skeleton.sprite = NULL;
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
	if( player.presentedRect.x + player.presentedRect.w/2 > enemy->presentedRect.x )
	{
		enemy->presentedRect.x += typeOfEnemy.speed;
	}
	if( player.presentedRect.x + player.presentedRect.w/2 < enemy->presentedRect.x )
	{
		enemy->presentedRect.x -= typeOfEnemy.speed;
	}
	if( player.presentedRect.y + player.presentedRect.h/2 > enemy->presentedRect.y )
	{
		enemy->presentedRect.y += typeOfEnemy.speed;
	}
	if( player.presentedRect.y + player.presentedRect.h/2 < enemy->presentedRect.y )
	{
		enemy->presentedRect.y -= typeOfEnemy.speed;
	}
}

void sprite_arrangement( Character* movingChar, Character staticChar )
{
	if(		// Atingindo canto superior esquerdo
		(	
			((movingChar->presentedRect.x+movingChar->presentedRect.w) < (staticChar.presentedRect.x+staticChar.presentedRect.w)) 
			&& 
	    	((movingChar->presentedRect.x+movingChar->presentedRect.w) > staticChar.presentedRect.x) 
	    ) 
    	&&
		(	
			((movingChar->presentedRect.y+movingChar->presentedRect.h) < (staticChar.presentedRect.y+staticChar.presentedRect.h))
			&&
			((movingChar->presentedRect.y+movingChar->presentedRect.h) > staticChar.presentedRect.y)
		)
	)
	{
		movingChar->presentedRect.x = staticChar.presentedRect.x - movingChar->presentedRect.w;
		movingChar->presentedRect.y = staticChar.presentedRect.y - movingChar->presentedRect.h;
	}

	else if(	 // Atingindo canto superior direto
			(	
				((movingChar->presentedRect.x) < (staticChar.presentedRect.x+staticChar.presentedRect.w)) 
				&& 
		    	((movingChar->presentedRect.x) > staticChar.presentedRect.x) 
		    ) 
	    	&&
			(	
				((movingChar->presentedRect.y+movingChar->presentedRect.h) < (staticChar.presentedRect.y+staticChar.presentedRect.h))
				&&
				((movingChar->presentedRect.y+movingChar->presentedRect.h) > staticChar.presentedRect.y)
			)
	)
	{
		movingChar->presentedRect.x = staticChar.presentedRect.x + staticChar.presentedRect.w;
		movingChar->presentedRect.y = staticChar.presentedRect.y - movingChar->presentedRect.h;
	}

	else if(	 // Atingindo canto inferior esquerdo
			(	
				((movingChar->presentedRect.x+movingChar->presentedRect.w) < (staticChar.presentedRect.x+staticChar.presentedRect.w)) 
				&& 
		    	((movingChar->presentedRect.x+movingChar->presentedRect.w) > staticChar.presentedRect.x) 
		    ) 
	    	&&
			(	
				((movingChar->presentedRect.y) < (staticChar.presentedRect.y+staticChar.presentedRect.h))
				&&
				((movingChar->presentedRect.y) > staticChar.presentedRect.y)
			)
	)
	{
		movingChar->presentedRect.x = staticChar.presentedRect.x - movingChar->presentedRect.w;
		movingChar->presentedRect.y = staticChar.presentedRect.y + staticChar.presentedRect.h;
	}

	else if( 	// Atingindo canto inferior direito
			(	
				((movingChar->presentedRect.x) < (staticChar.presentedRect.x+staticChar.presentedRect.w)) 
				&& 
		    	((movingChar->presentedRect.x) > staticChar.presentedRect.x) 
		    ) 
	    	&&
			(	
				((movingChar->presentedRect.y) < (staticChar.presentedRect.y+staticChar.presentedRect.h))
				&&
				((movingChar->presentedRect.y) > staticChar.presentedRect.y)
			)
	)
	{
		movingChar->presentedRect.x = staticChar.presentedRect.x + staticChar.presentedRect.w;
		movingChar->presentedRect.y = staticChar.presentedRect.y + staticChar.presentedRect.h;
	}
}
 
int item_collision( Item item, Character player )
{
	int l_YES = 1, l_NO = 0;

	int l_collision = l_NO;

	if((	// Atingindo canto superior esquerdo
			(	
				((item.presentedRect.x+item.presentedRect.w) < (player.presentedRect.x+player.presentedRect.w)) 
				&& 
		    	((item.presentedRect.x+item.presentedRect.w) > player.presentedRect.x) 
		    ) 
	    	&&
			(	
				((item.presentedRect.y+item.presentedRect.h) < (player.presentedRect.y+player.presentedRect.h))
				&&
				((item.presentedRect.y+item.presentedRect.h) > player.presentedRect.y)
			)
		)
		||
		(	 // Atingindo canto superior direto
			(	
			((item.presentedRect.x) < (player.presentedRect.x+player.presentedRect.w)) 
				&& 
		    	((item.presentedRect.x) > player.presentedRect.x) 
		    ) 
	    	&&
			(	
				((item.presentedRect.y+item.presentedRect.h) < (player.presentedRect.y+player.presentedRect.h))
				&&
				((item.presentedRect.y+item.presentedRect.h) > player.presentedRect.y)
			)
		)
		||
		(	 // Atingindo canto inferior esquerdo
			(	
				((item.presentedRect.x+item.presentedRect.w) < (player.presentedRect.x+player.presentedRect.w)) 
				&& 
		    	((item.presentedRect.x+item.presentedRect.w) > player.presentedRect.x) 
		    ) 
	    	&&
			(	
				((item.presentedRect.y) < (player.presentedRect.y+player.presentedRect.h))
				&&
				((item.presentedRect.y) > player.presentedRect.y)
			)
		)
		||
		( 	// Atingindo canto inferior direito
			(	
				((item.presentedRect.x) < (player.presentedRect.x+player.presentedRect.w)) 
				&& 
		    	((item.presentedRect.x) > player.presentedRect.x) 
		    ) 
	    	&&
			(	
				((item.presentedRect.y) < (player.presentedRect.y+player.presentedRect.h))
				&&
				((item.presentedRect.y) > player.presentedRect.y)
			)
		)
	)
	{
		l_collision = l_YES;
	}

	return l_collision;
} 

int magicProjectile_collision( MagicProjectile mP, Character character )
{
	int l_YES = 1, l_NO = 0;

	int l_collision = l_NO;

	if((	// Atingindo canto superior esquerdo
			(	
				((mP.presentedRect.x+mP.presentedRect.w) < (character.presentedRect.x+character.presentedRect.w)) 
				&& 
		    	((mP.presentedRect.x+mP.presentedRect.w) > character.presentedRect.x) 
		    ) 
	    	&&
			(	
				((mP.presentedRect.y+mP.presentedRect.h) < (character.presentedRect.y+character.presentedRect.h))
				&&
				((mP.presentedRect.y+mP.presentedRect.h) > character.presentedRect.y)
			)
		)
		||
		(	 // Atingindo canto superior direto
			(	
			((mP.presentedRect.x) < (character.presentedRect.x+character.presentedRect.w)) 
				&& 
		    	((mP.presentedRect.x) > character.presentedRect.x) 
		    ) 
	    	&&
			(	
				((mP.presentedRect.y+mP.presentedRect.h) < (character.presentedRect.y+character.presentedRect.h))
				&&
				((mP.presentedRect.y+mP.presentedRect.h) > character.presentedRect.y)
			)
		)
		||
		(	 // Atingindo canto inferior esquerdo
			(	
				((mP.presentedRect.x+mP.presentedRect.w) < (character.presentedRect.x+character.presentedRect.w)) 
				&& 
		    	((mP.presentedRect.x+mP.presentedRect.w) > character.presentedRect.x) 
		    ) 
	    	&&
			(	
				((mP.presentedRect.y) < (character.presentedRect.y+character.presentedRect.h))
				&&
				((mP.presentedRect.y) > character.presentedRect.y)
			)
		)
		||
		( 	// Atingindo canto inferior direito
			(	
				((mP.presentedRect.x) < (character.presentedRect.x+character.presentedRect.w)) 
				&& 
		    	((mP.presentedRect.x) > character.presentedRect.x) 
		    ) 
	    	&&
			(	
				((mP.presentedRect.y) < (character.presentedRect.y+character.presentedRect.h))
				&&
				((mP.presentedRect.y) > character.presentedRect.y)
			)
		)
	)
	{
		l_collision = l_YES;
	}
	
	return l_collision;
} 

int on_player_collision( Character enemy, Character player )
{
	int l_YES = 1, l_NO = 0;

	int l_collision = l_NO;

	if((	// Atingindo canto superior esquerdo
			(	
				((enemy.presentedRect.x+enemy.presentedRect.w) < (player.presentedRect.x+player.presentedRect.w)) 
				&& 
		    	((enemy.presentedRect.x+enemy.presentedRect.w) > player.presentedRect.x) 
		    ) 
	    	&&
			(	
				((enemy.presentedRect.y+enemy.presentedRect.h) < (player.presentedRect.y+player.presentedRect.h))
				&&
				((enemy.presentedRect.y+enemy.presentedRect.h) > player.presentedRect.y)
			)
		)
		||
		(	 // Atingindo canto superior direto
			(	
			((enemy.presentedRect.x) < (player.presentedRect.x+player.presentedRect.w)) 
				&& 
		    	((enemy.presentedRect.x) > player.presentedRect.x) 
		    ) 
	    	&&
			(	
				((enemy.presentedRect.y+enemy.presentedRect.h) < (player.presentedRect.y+player.presentedRect.h))
				&&
				((enemy.presentedRect.y+enemy.presentedRect.h) > player.presentedRect.y)
			)
		)
		||
		(	 // Atingindo canto inferior esquerdo
			(	
				((enemy.presentedRect.x+enemy.presentedRect.w) < (player.presentedRect.x+player.presentedRect.w)) 
				&& 
		    	((enemy.presentedRect.x+enemy.presentedRect.w) > player.presentedRect.x) 
		    ) 
	    	&&
			(	
				((enemy.presentedRect.y) < (player.presentedRect.y+player.presentedRect.h))
				&&
				((enemy.presentedRect.y) > player.presentedRect.y)
			)
		)
		||
		( 	// Atingindo canto inferior direito
			(	
				((enemy.presentedRect.x) < (player.presentedRect.x+player.presentedRect.w)) 
				&& 
		    	((enemy.presentedRect.x) > player.presentedRect.x) 
		    ) 
	    	&&
			(	
				((enemy.presentedRect.y) < (player.presentedRect.y+player.presentedRect.h))
				&&
				((enemy.presentedRect.y) > player.presentedRect.y)
			)
		)
	)
	{
		l_collision = l_YES;
	}
	
	return l_collision;
} 