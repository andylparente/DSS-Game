#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>  
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
	
	// Se esta aparecendo na tela, pontos de vida, velocidade com que se move, dano causado por contato fisico e delay para cada tiro
	int activate;
	int healthPoints;
	float speed;
	int damage;
	int shootDelayCounter;

	// Qual o numero do projetil sendo disparado
	int bulletNumber;

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
	char direction;

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

typedef struct _enemyWave
{
	int activate;

	Character skeleton[60];

	Character jellykiller[80];

	Character beholder[20];
	MagicProjectile beholderBullet[20][20];

	Character necromancer[24];
	MagicProjectile necroShot[24][30];
	
} EnemyWave;

// Delimita onde os personagens opdem andar
void room_limits( Character* character );

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
void linear_walk_and_shoot( Character* enemy, Character typeOfEnemy, Character player,  MagicProjectile* projectile );

// Atualiza a animacao e posicao do projetil
void projectile_update( MagicProjectile* bullet, MagicProjectile typeOfBullet, int sCounter, int sDeath, SDL_Window* l_window, SDL_Renderer* l_renderer );

// Atualiza  animacao e posicao do necromancer
void necromancer_ia( Character* enemy, Character typeOfEnemy, Character player, MagicProjectile* projectile, MagicProjectile* projectile2 );


// UHUUUUUUUUUUUUUUUUUHUHUHUUHUHUUUUUUUUUUUUUUUUUU *_* HHHHHHHHHHHHHHHHHHHHUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUHUHUHUUUUUUUUUUUUUUUUUUUUUUUUUU *_* UUUUUUUUUUUHUHUHUHUUUUHUHU //

int gameplay_logic( SDL_Window* l_window, SDL_Renderer* l_renderer )
{
	// Estado pausado 
	int pauseScreen = 0;

	// Contadores (de loop) para wave, inimigos;
	int x, y, z;

	int victoryScreen;

	// Contadores que auxiliam na wave;
	int up, down, left, right;
	int upSkeleton, downSkeleton, leftSkeleton, rightSkeleton;
	int upJellykiller, downJellykiller, leftJellykiller, rightJellykiller;
	int upBeholder, downBeholder, leftBeholder, rightBeholder;
	int upNecromancer, downNecromancer, leftNecromancer, rightNecromancer;
	int upQuit, downQuit, leftQuit, rightQuit;

	int deathScreen;

	// Contador para a animacao dos logos
	int logoAnimationCounter = 0;
	SDL_Rect logoSnipRect;
	SDL_Rect logoPresentedRect;
	logoSnipRect.x = 0;
	logoSnipRect.y = 0;
	logoSnipRect.w = 100;
	logoSnipRect.h = 100;
	logoPresentedRect.x = 690;
	logoPresentedRect.y = 10;
	logoPresentedRect.w = 100;
	logoPresentedRect.h = 100; 

	// Necessario para numero aleatorios dentro de um loop
	srand ( time( NULL ) );

	// Contadores (preguica de colocar l_ para essas variaveis locais)
	int i = 0, j = 0, j2 = 0;
	int i2 = 0;
	int a = 0;

	// Auxiliar para comparar valores de posicoes passados com o atual
	int pastPresentedRectX, pastPresentedRectY;

	// Tipo de tiro que o jogador esta usando
	int kindOfShot = 0;

	// Barra de vida
	SDL_Rect healthBarRect;
	healthBarRect.x = 90;
	healthBarRect.y = 40;
	healthBarRect.w = 200;
	healthBarRect.h = 50;
	SDL_SetRenderDrawColor( l_renderer, 0, 255, 0, 255 );  
	SDL_RenderFillRect( l_renderer, &healthBarRect );

	// Flag para trocar o estado do jogo
	int l_gameState= 3;

	// Texturas utilizadas durante o jogo
	SDL_Texture* victoryImage = load_texture( "images/non_sprites/fim_legal.png", l_window, l_renderer);
	SDL_Texture* failureImage = load_texture( "images/non_sprites/youlost.png", l_window, l_renderer);
	SDL_Texture* readyImage = load_texture( "images/non_sprites/ready.png", l_window, l_renderer);
	SDL_Texture* pauseImage = load_texture( "images/non_sprites/pause.png", l_window, l_renderer );
	SDL_Texture* arcaneLogo = load_texture( "images/sprites/arcanoativo.png", l_window, l_renderer);
	SDL_Texture* fireLogo = load_texture( "images/sprites/fogo100x100.png", l_window, l_renderer);
	SDL_Texture* iceLogo = load_texture( "images/sprites/gelo100x100.png", l_window, l_renderer);

	// Personagens que atiram coisas (inclui o jogador)
	Character player;
	Character beholder;
	Character necromancer;

	// Personagens que causam dano somente atraves do toque
	Character skeleton;
	Character jellykiller;

	// Tipos de projeteis/tiros	
	MagicProjectile fireBall;
	MagicProjectile iceSpear;
	MagicProjectile arcaneMissile;
	MagicProjectile beholderBullet;
	MagicProjectile necroShot;
	//MagicProjectile demonicBolt;
	
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
	player.presentedRect.w = 54;
	player.presentedRect.h = 54;
	//player.gotHit = load_sfx( "sound_music/wav/inkium_acertado.wav" );
	player.deathSound = load_sfx( "sound_music/wav/inkium_death.wav" );
	player.healthPoints = 100;
	player.speed = 5.5;
	//player.damage = 0;
	player.shootDelayCounter = 0;
	player.animationCycleCounter = 0;
	player.deathAnimationCounter = 0;

	// Quantidade de tiros
	MagicProjectile fireBallV[1][50];
	MagicProjectile iceSpearV[1][12];
	MagicProjectile arcaneMissileV[1][30];
	
	// Informacoes do esqueleto (chaser)
	skeleton.sprite = load_texture( "images/sprites/esqueleto55x55.png", l_window, l_renderer );
	skeleton.spriteDeath = load_texture( "images/sprites/esqueletoM55x55.png", l_window, l_renderer );
	skeleton.gotHit = load_sfx( "sound_music/wav/eskeleton_hited.wav" );
	skeleton.deathSound = load_sfx( "sound_music/wav/eskeleton_death.wav" );
	skeleton.healthPoints = 100;
	skeleton.speed = 1.5;
	skeleton.damage = 1;

	// Informacoes da agua-morta (chaser)
	jellykiller.sprite = load_texture( "images/sprites/aguamorta60x60.png", l_window, l_renderer );
	jellykiller.spriteDeath = load_texture( "images/sprites/aguamortadeath.png", l_window, l_renderer );
	jellykiller.gotHit = load_sfx( "sound_music/wav/aguamorta_hit.wav" );
	jellykiller.deathSound = load_sfx( "sound_music/wav/agua_morta_death.wav" );
	jellykiller.healthPoints = 50;
	jellykiller.speed = 2.0;
	jellykiller.damage = 1;
	
	// Informacoes do beholder (shooter)
	beholder.sprite = load_texture( "images/sprites/beholder_90x90.png", l_window, l_renderer );
	beholder.spriteDeath = load_texture( "images/sprites/beholderM90x90.png", l_window, l_renderer );
	beholder.gotHit = load_sfx( "sound_music/wav/beholder_hitted.wav" );
	beholder.deathSound = load_sfx( "sound_music/wav/beholder_death.wav" );
	beholder.healthPoints = 200;
	beholder.speed = 2.0;
	beholder.damage = 1;

	// Informacoes do necromancer (shooter)
	necromancer.sprite = load_texture( "images/sprites/necromancer85x100.png", l_window, l_renderer );
	necromancer.spriteDeath = load_texture( "images/sprites/necrodeath.png", l_window, l_renderer );
	necromancer.gotHit = load_sfx( "sound_music/wav/necro_hited.wav" );
	necromancer.deathSound = load_sfx( "sound_music/wav/necro_death.wav" );
	necromancer.healthPoints = 200;
	necromancer.speed = 1.5;
	necromancer.damage = 1;

	// Definindo info do vetor de iceSpears
	for( a = 0 ; a < 12 ; a++ )
	{
		iceSpearV[0][a].snipRect.x = 0;
		iceSpearV[0][a].snipRect.y = 0;
		iceSpearV[0][a].snipRect.w = 40;
		iceSpearV[0][a].snipRect.h = 40;
		iceSpearV[0][a].presentedRect.w = 54;
		iceSpearV[0][a].presentedRect.h = 54; 
		iceSpearV[0][a].animationCycleCounter = 0;
		iceSpearV[0][a].activate = 0;
	}

	// Informacoes do projetil fireBall
	fireBall.sprite = load_texture( "images/sprites/fireball40x40.png", l_window, l_renderer );
	fireBall.hitWall = load_sfx( "sound_music/wav/fogo_parede.wav" );
	//fireBall.hitCharacter = load_sfx( "?" );
	fireBall.damage = 25;
	fireBall.speed = 12.0;

	// Definindo info do vetor de fireBalls 
	for( a = 0 ; a < 50 ; a++ )
	{
		fireBallV[0][a].snipRect.x = 0;
		fireBallV[0][a].snipRect.y = 0;
		fireBallV[0][a].snipRect.w = 40;
		fireBallV[0][a].snipRect.h = 40;
		fireBallV[0][a].presentedRect.w = 50;
		fireBallV[0][a].presentedRect.h = 50; 
		fireBallV[0][a].animationCycleCounter = 0;
		fireBallV[0][a].activate = 0;
	}

	// Informacoes do projetil iceSpear
	iceSpear.sprite = load_texture( "images/sprites/iceshard40x40.png", l_window, l_renderer );
	iceSpear.hitWall = load_sfx( "sound_music/wav/gelo_parede.wav" );
	//iceSpear.hitCharacter = load_sfx( "?" );
	iceSpear.damage = 100;
	iceSpear.speed = 5.0;

	// Definindo info do vetor de iceSpears
	for( a = 0 ; a < 12 ; a++ )
	{
		iceSpearV[0][a].snipRect.x = 0;
		iceSpearV[0][a].snipRect.y = 0;
		iceSpearV[0][a].snipRect.w = 40;
		iceSpearV[0][a].snipRect.h = 40;
		iceSpearV[0][a].presentedRect.w = 54;
		iceSpearV[0][a].presentedRect.h = 54; 
		iceSpearV[0][a].animationCycleCounter = 0;
		iceSpearV[0][a].activate = 0;
	}

	// Informacoes do projetil iceSpear
	arcaneMissile.sprite = load_texture( "images/sprites/jaina40x40.png", l_window, l_renderer );
	arcaneMissile.hitWall = load_sfx( "sound_music/wav/arcano_parede.wav" );
	//arcaneMissile.hitCharacter = load_sfx( "?" );
	arcaneMissile.damage = 50;
	arcaneMissile.speed = 8.0;

	// Definindo info do vetor de activates
	for( a = 0 ; a < 30 ; a++ )
	{
		arcaneMissileV[0][a].snipRect.x = 0;
		arcaneMissileV[0][a].snipRect.y = 0;
		arcaneMissileV[0][a].snipRect.w = 40;
		arcaneMissileV[0][a].snipRect.h = 40;
		arcaneMissileV[0][a].presentedRect.w = 54;
		arcaneMissileV[0][a].presentedRect.h = 54; 
		arcaneMissileV[0][a].animationCycleCounter = 0;
		arcaneMissileV[0][a].activate = 0;
	}

	// Informacoes do projetil necroShot
	necroShot.sprite = load_texture( "images/sprites/necroshot40x40.png", l_window, l_renderer );
	//necroShot.hitWall = load_sfx( "?" );
	//necroShot.hitCharacter = load_sfx( "?" );
	necroShot.damage = 35;
	necroShot.speed = 2.0;

	// Informacoes do projetil do beholder
	beholderBullet.sprite = load_texture( "images/sprites/beholdershot40x40.png", l_window, l_renderer );
	//beholderBullet.hitWall = load_sfx( "?" );
	//beholderBullet.hitCharacter = load_sfx( "?" );
	beholderBullet.damage = 25;
	beholderBullet.speed = 3.0;

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
	firstMap.snipRect.x = 0;
	firstMap.snipRect.y = 0;
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
	float velx = 0.0;
	float vely = 0.0;

	// FPS do jogo
	int fps = 60;

	// Numero da wave atual
	int currentWave;

	// Sobre as waves, comecam desativadas
	EnemyWave wave[10];
	for ( x = 0; x < 10 ; x++)
	{
		wave[x].activate = 0;
	}

	// Esqueletos da wave
	for ( x = 0; x < 10 ; x++)
	{ 
		for( y = 0 ; y < 60 ; y++ )
		{
			wave[x].skeleton[y].snipRect.x = 0;
			wave[x].skeleton[y].snipRect.y = 0;
			wave[x].skeleton[y].snipRect.h = 55;
			wave[x].skeleton[y].snipRect.w = 55;
			wave[x].skeleton[y].presentedRect.w = 60;
			wave[x].skeleton[y].presentedRect.h = 60; 
			wave[x].skeleton[y].activate = 0;
			wave[x].skeleton[y].animationCycleCounter = 0;
			wave[x].skeleton[y].deathAnimationCounter = 0;
		}
	}

	// Aguas-Mortas da wave 
	for ( x = 0; x < 10 ; x++)
	{ 
		for( y = 0 ; y < 80 ; y++ )
		{
			wave[x].jellykiller[y].snipRect.x = 0;
			wave[x].jellykiller[y].snipRect.y = 0;
			wave[x].jellykiller[y].snipRect.h = 60;
			wave[x].jellykiller[y].snipRect.w = 60;
			wave[x].jellykiller[y].presentedRect.w = 56;
			wave[x].jellykiller[y].presentedRect.h = 56; 
			wave[x].jellykiller[y].activate = 0;
			wave[x].jellykiller[y].animationCycleCounter = 0;
			wave[x].jellykiller[y].deathAnimationCounter = 0;
			if( y%4 == 0 )
			{
				wave[x].jellykiller[y].direction = 'u';
			}
			else if( y%4 == 1 )
			{
				wave[x].jellykiller[y].direction = 'd';
			}
			else if( y%4 == 2 )
			{
				wave[x].jellykiller[y].direction = 'l';
			}
			else if( y%4 == 3 )
			{
				wave[x].jellykiller[y].direction = 'r';
			}
		}
	}

	// Beholders da wave 
	for ( x = 0; x < 10 ; x++)
	{ 
		for( y = 0 ; y < 20 ; y++ )
		{
			wave[x].beholder[y].snipRect.x = 0;
			wave[x].beholder[y].snipRect.y = 0;
			wave[x].beholder[y].snipRect.w = 90;
			wave[x].beholder[y].snipRect.h = 90;
			wave[x].beholder[y].presentedRect.w = 110;
			wave[x].beholder[y].presentedRect.h = 110; 
			wave[x].beholder[y].activate = 0;
			wave[x].beholder[y].shootDelayCounter = 0;
			wave[x].beholder[y].bulletNumber = 0;
			wave[x].beholder[y].way = rand() %2 + 1;
			wave[x].beholder[y].animationCycleCounter = 0;
			wave[x].beholder[y].deathAnimationCounter = 0;
			if( y%4 == 0 )
			{
				wave[x].beholder[y].direction = 'u';
			}
			else if( y%4 == 1 )
			{
				wave[x].beholder[y].direction = 'd';
			}
			else if( y%4 == 2 )
			{
				wave[x].beholder[y].direction = 'l';
			}
			else if( y%4 == 3 )
			{
				wave[x].beholder[y].direction = 'r';
			}
		}
	}

	// Projeteis dos Beholders da wave
	for ( x = 0; x < 10 ; x++)
	{  
		for( y = 0 ; y < 20 ; y++ )
		{
			for( z = 0 ; z < 20 ; z++ )
			{
				wave[x].beholderBullet[y][z].snipRect.x = 0;
				wave[x].beholderBullet[y][z].snipRect.y = 0;
				wave[x].beholderBullet[y][z].snipRect.w = 40;
				wave[x].beholderBullet[y][z].snipRect.h = 40;
				wave[x].beholderBullet[y][z].presentedRect.w = 70;
				wave[x].beholderBullet[y][z].presentedRect.h = 70; 
				wave[x].beholderBullet[y][z].animationCycleCounter = 0;
				wave[x].beholderBullet[y][z].activate = 0;
			}
		}
	}

	// Necromancers da wave 
	for ( x = 0; x < 10 ; x++)
	{ 
		for( y = 0 ; y < 24 ; y++ )
		{
			wave[x].necromancer[y].snipRect.x = 0;
			wave[x].necromancer[y].snipRect.y = 0;
			wave[x].necromancer[y].snipRect.w = 85;
			wave[x].necromancer[y].snipRect.h = 100;
			wave[x].necromancer[y].presentedRect.w = 100;
			wave[x].necromancer[y].presentedRect.h = 100; 
			wave[x].necromancer[y].activate = 0;
			wave[x].necromancer[y].shootDelayCounter = 0;
			wave[x].necromancer[y].bulletNumber = 0;
			wave[x].necromancer[y].way = rand() % 4;
			wave[x].necromancer[y].animationCycleCounter = 0;
			wave[x].necromancer[y].deathAnimationCounter = 0;
			if( y%4 == 0 )
			{
				wave[x].necromancer[y].direction = 'u';
			}
			else if( y%4 == 1 )
			{
				wave[x].necromancer[y].direction = 'd';
			}
			else if( y%4 == 2 )
			{
				wave[x].necromancer[y].direction = 'l';
			}
			else if( y%4 == 3 )
			{
				wave[x].necromancer[y].direction = 'r';
			}
		}
	}

	//  Projeteis dos Necromancers da wave 
	for ( x = 0; x < 10 ; x++)
	{ 
		for( y = 0 ; y < 24 ; y++ )
		{
			for( z = 0 ; z < 30 ; z++ )
			{
				wave[x].necroShot[y][z].snipRect.x = 0;
				wave[x].necroShot[y][z].snipRect.y = 0;
				wave[x].necroShot[y][z].snipRect.w = 40;
				wave[x].necroShot[y][z].snipRect.h = 40;
				wave[x].necroShot[y][z].presentedRect.w = 72;
				wave[x].necroShot[y][z].presentedRect.h = 72;
				wave[x].necroShot[y][z].animationCycleCounter = 0;
				wave[x].necroShot[y][z].activate = 0;
			}
		}
	}

	while( l_gameState == 3 )
	{		
		SDL_RenderCopy( l_renderer, firstMap.texture, &firstMap.snipRect, &firstMap.presentedRect );

		// Sobre os tiros do jogador
		for( x = 0 ; x < 1 ; x++ )
		{
			for( y = 0 ; y< 30 ; y++ )
			{
				projectile_update( &arcaneMissileV[x][y], arcaneMissile, 5, 1, l_window, l_renderer );
			}
			for( y = 0 ; y < 50 ; y++ )
			{
				projectile_update( &fireBallV[x][y], fireBall, 5, 1, l_window, l_renderer );
			}
			for( y = 0 ; y < 12 ; y++ )
			{
				projectile_update( &iceSpearV[x][y], iceSpear, 5, 1, l_window, l_renderer );
			}
		}

		// Animacoes dos logos dos tipos de tiro
		if( kindOfShot == 0 )
		{
			if( logoSnipRect.x < 0 || logoSnipRect.x > 6300 )
			{
				logoSnipRect.x = 0;
			}
			SDL_RenderCopy( l_renderer, arcaneLogo, &logoSnipRect, &logoPresentedRect );
			logoAnimationCounter++;
			if( logoAnimationCounter%1 == 0 )
			{
				logoSnipRect.x += 100;
			}
		}
		else if( kindOfShot == 1 )
		{
			if( logoSnipRect.x < 0 || logoSnipRect.x > 3300 )
			{
				logoSnipRect.x = 0;
			}
			SDL_RenderCopy( l_renderer, fireLogo, &logoSnipRect, &logoPresentedRect );
			logoAnimationCounter++;
			if( logoAnimationCounter%2 == 0 )
			{
				logoSnipRect.x += 100;
			}
		}
		else if( kindOfShot == 2 )
		{
			if( logoSnipRect.x < 0 || logoSnipRect.x > 1600 )
			{
				logoSnipRect.x = 0;
			}
			SDL_RenderCopy( l_renderer, iceLogo, &logoSnipRect, &logoPresentedRect );
			logoAnimationCounter++;
			if( logoAnimationCounter%3 == 0 )
			{
				logoSnipRect.x += 100;
			}
		}

        // Sobre os tiros dos inmigos
		for( x = 0 ; x < 10 ; x++ )
		{
			for( y = 0 ; y < 20 ; y++ )
			{
				for( z = 0 ; z < 20 ; z++ )
				{
					projectile_update( &wave[x].beholderBullet[y][z], beholderBullet, 8, 2, l_window, l_renderer );

					if( wave[x].beholderBullet[y][z].activate == 1 )
					{
						if( circular_collision_detection( player.presentedRect, wave[x].beholderBullet[y][z].presentedRect, 5 ) != 0 )
						{
							player.healthPoints -= beholderBullet.damage;
							wave[x].beholderBullet[y][z].activate = 2;
							Mix_PlayChannel( -1, player.gotHit, 0 );
						}
					}
				}
			}
			for( y = 0 ; y < 24 ; y++ )
			{
				for( z = 0 ; z < 30 ; z++ )
				{
					projectile_update( &wave[x].necroShot[y][z], necroShot, 5, 2, l_window, l_renderer );
					if( wave[x].necroShot[y][z].activate == 1 )
					{
						if( circular_collision_detection( player.presentedRect, wave[x].necroShot[y][z].presentedRect, 3 ) != 0 )
						{
							player.healthPoints -= necroShot.damage;
							wave[x].necroShot[y][z].activate = 2;
							Mix_PlayChannel( -1, player.gotHit, 0 );
						}
					}
				}
			}
		}

		// Onde os inimigos aparecerao na tela
		for( x = 0 ; x < 10 ; x++ )
		{
	        for( y = 0 ; y < 60 ; y++ ) // Skeletons
	        {
	        	if( wave[x].skeleton[y].activate == 0 )
	        	{
	        		wave[x].skeleton[y].presentedRect.x = 900;
	        		wave[x].skeleton[y].presentedRect.y = 900;
	        		wave[x].skeleton[y].snipRect.x = 0;
	        		wave[x].skeleton[y].healthPoints = skeleton.healthPoints;
	        		wave[x].skeleton[y].animationCycleCounter = 0;
	        		wave[x].skeleton[y].deathAnimationCounter = 0;
	        	}	
	        	// Sobre o esqueleto
	           	if( wave[x].skeleton[y].activate == 1 )
	           	{
	           		wave[x].skeleton[y].animationCycleCounter++;

	           		pastPresentedRectX = wave[x].skeleton[y].presentedRect.x;
	           		pastPresentedRectY = wave[x].skeleton[y].presentedRect.y;

	           		// Esqueletos vao ate ao jogador
					chasing( &wave[x].skeleton[y], skeleton, player );

					if( pastPresentedRectY != wave[x].skeleton[y].presentedRect.y )
					{
						// Indo pra cima
						if( pastPresentedRectY > wave[x].skeleton[y].presentedRect.y ) 
						{
							if( wave[x].skeleton[y].animationCycleCounter%8 == 0 )
							{
								wave[x].skeleton[y].snipRect.x += 55;
							}
							if( wave[x].skeleton[y].snipRect.x < 165 || wave[x].skeleton[y].snipRect.x > 220 )
							{
								wave[x].skeleton[y].snipRect.x = 165;
							}
						}
						// Indo pra baixo
						else if( pastPresentedRectY < wave[x].skeleton[y].presentedRect.y ) 
						{
							if( wave[x].skeleton[y].animationCycleCounter%8 == 0 )
							{
								wave[x].skeleton[y].snipRect.x += 55;
							}
							if( wave[x].skeleton[y].snipRect.x < 0 || wave[x].skeleton[y].snipRect.x > 55 )
							{
								wave[x].skeleton[y].snipRect.x = 0;
							}
						}
					}

					else if( pastPresentedRectX != wave[x].skeleton[y].presentedRect.x )
					{
						// Indo pra esquerda
						if( pastPresentedRectX > wave[x].skeleton[y].presentedRect.x ) 
						{
							if( wave[x].skeleton[y].animationCycleCounter%8 == 0 )
							{
								wave[x].skeleton[y].snipRect.x += 55;
							}
							if( wave[x].skeleton[y].snipRect.x < 495 || wave[x].skeleton[y].snipRect.x > 550 )
							{
								wave[x].skeleton[y].snipRect.x = 495;
							}
						}
						// Indo pra direita
						else if( pastPresentedRectX < wave[x].skeleton[y].presentedRect.x ) 
						{
							if( wave[x].skeleton[y].animationCycleCounter%8 == 0 )
							{
								wave[x].skeleton[y].snipRect.x += 55;
							}
							if( wave[x].skeleton[y].snipRect.x < 330 || wave[x].skeleton[y].snipRect.x > 385 )
							{
								wave[x].skeleton[y].snipRect.x = 330;
							}
						}
					}

					// Esqueleto ataca o jogador ao enconsta-lo
					if ( circular_collision_detection( player.presentedRect, wave[x].skeleton[y].presentedRect, 5 ) != 0 )
					{
						player.healthPoints -= skeleton.damage;
						Mix_PlayChannel( -1, player.gotHit, 0 );
					}

					// Esqueleto sofre dano pelos projeteis do jogador
					for ( i2 = 0 ; i2 < 50 ; i2++ )
					{
						if( fireBallV[0][i2].activate == 1 )
						{
							if( circular_collision_detection( fireBallV[0][i2].presentedRect, wave[x].skeleton[y].presentedRect, 0 ) != 0 )
							{
								wave[x].skeleton[y].healthPoints -= fireBall.damage;
								Mix_PlayChannel( -1, fireBall.hitWall, 0 );
								fireBallV[0][i2].activate = 2;
								if( wave[x].skeleton[y].healthPoints <= 0 )
								{
									wave[x].skeleton[y].activate = 2;
									Mix_PlayChannel( -1, skeleton.deathSound, 0 );
								}
								else
								{
									Mix_PlayChannel( -1, skeleton.gotHit, 0 );
								}
							}
						}				
					}
					for ( i2 = 0 ; i2 < 12 ; i2++ )
					{
						if( iceSpearV[0][i2].activate == 1 )
						{
							if( circular_collision_detection( iceSpearV[0][i2].presentedRect, wave[x].skeleton[y].presentedRect, 0 ) != 0 )
							{
								wave[x].skeleton[y].healthPoints -= iceSpear.damage;
								Mix_PlayChannel( -1, iceSpear.hitWall, 0 );
								iceSpearV[0][i2].activate = 2;
								if( wave[x].skeleton[y].healthPoints <= 0 )
								{
									wave[x].skeleton[y].activate = 2;
									Mix_PlayChannel( -1, skeleton.deathSound, 0 );
								}
								else
								{
									Mix_PlayChannel( -1, skeleton.gotHit, 0 );
								}
							}
						}				
					}
					for ( i2 = 0 ; i2 < 30 ; i2++ )
					{
						if( arcaneMissileV[0][i2].activate == 1 )
						{
							if( circular_collision_detection( arcaneMissileV[0][i2].presentedRect, wave[x].skeleton[y].presentedRect, 0 ) != 0 )
							{
								wave[x].skeleton[y].healthPoints -= arcaneMissile.damage;
								Mix_PlayChannel( -1, arcaneMissile.hitWall, 0 );
								arcaneMissileV[0][i2].activate = 2;
								if( wave[x].skeleton[y].healthPoints <= 0 )
								{
									wave[x].skeleton[y].activate = 2;
									Mix_PlayChannel( -1, skeleton.deathSound, 0 );
								}
								else
								{
									Mix_PlayChannel( -1, skeleton.gotHit, 0 );
								}
							}
						}				
					}
					if( player.healthPoints > 0 )
					{
						circular_sprite_arrangement( wave[x].skeleton[y].presentedRect, &player.presentedRect );
					}
					room_limits( &wave[x].skeleton[y] );

					SDL_RenderCopy( l_renderer, skeleton.sprite, &wave[x].skeleton[y].snipRect, &wave[x].skeleton[y].presentedRect );	
				}
				if( wave[x].skeleton[y].activate == 3 )
	        	{
	        		wave[x].skeleton[y].presentedRect.x = 900;
	        		wave[x].skeleton[y].presentedRect.y = 900;
	        		wave[x].skeleton[y].snipRect.x = 0;
	        		wave[x].skeleton[y].healthPoints = skeleton.healthPoints;
	        		wave[x].skeleton[y].animationCycleCounter = 0;
	        		wave[x].skeleton[y].deathAnimationCounter = 0;
	        	}
				// Animacao de morte do esqueleto
				if( wave[x].skeleton[y].activate == 2 )
				{
					if( wave[x].skeleton[y].deathAnimationCounter == 0)
					{
						wave[x].skeleton[y].snipRect.x = 0;
					}

					wave[x].skeleton[y].deathAnimationCounter++;

					if( wave[x].skeleton[y].deathAnimationCounter%30 == 0 )
					{
						wave[x].skeleton[y].snipRect.x += 55;
					}
					room_limits( &wave[x].skeleton[y] );

					SDL_RenderCopy( l_renderer, skeleton.spriteDeath, &wave[x].skeleton[y].snipRect, &wave[x].skeleton[y].presentedRect );

					if( wave[x].skeleton[y].deathAnimationCounter == 120 )
					{
						wave[x].skeleton[y].activate = 3;
					}
				}	
	        }

	        for( y = 0 ; y < 80 ; y++ ) // Jellykiller
	        {
	        	if( wave[x].jellykiller[y].activate == 0 )
	        	{
	        		wave[x].jellykiller[y].presentedRect.x = 900;
	        		wave[x].jellykiller[y].presentedRect.y = 900;
	        		wave[x].jellykiller[y].snipRect.x = 0;
	        		wave[x].jellykiller[y].healthPoints = jellykiller.healthPoints;
	        		wave[x].jellykiller[y].animationCycleCounter = 0;
	        		wave[x].jellykiller[y].deathAnimationCounter = 0;
	        	}	
	        	// Sobre o agua-morta
	           	if( wave[x].jellykiller[y].activate == 1 )
	           	{
	           		// Agua-morta vai ate ao jogador
					chasing( &wave[x].jellykiller[y], jellykiller, player );

					// Ciclo de animacao
					wave[x].jellykiller[y].animationCycleCounter++;
	           		if( wave[x].jellykiller[y].animationCycleCounter%8 == 0 )
	           		{
	           			wave[x].jellykiller[y].snipRect.x += 60;
	           		}
	           		if( wave[x].jellykiller[y].snipRect.x < 0 || wave[x].jellykiller[y].snipRect.x > 300 )
	           		{
						wave[x].jellykiller[y].snipRect.x = 0;           			
	           		}

					// Agua-morta ataca o jogador ao enconsta-lo
					if ( circular_collision_detection( player.presentedRect, wave[x].jellykiller[y].presentedRect, 5 ) != 0 )
					{
						player.healthPoints -= jellykiller.damage;
						Mix_PlayChannel( -1, player.gotHit, 0 );
						wave[x].jellykiller[y].snipRect.x = 360;
					}

					// Agua-morta sofre dano pelos projeteis do jogador
					for ( i2 = 0 ; i2 < 50 ; i2++ )
					{
						if( fireBallV[0][i2].activate == 1 )
						{
							if( circular_collision_detection( fireBallV[0][i2].presentedRect, wave[x].jellykiller[y].presentedRect, 0 ) != 0 )
							{
								wave[x].jellykiller[y].healthPoints -= fireBall.damage;
								Mix_PlayChannel( -1, fireBall.hitWall, 0 );
								fireBallV[0][i2].activate = 2;
								if( wave[x].jellykiller[y].healthPoints <= 0 )
								{
									wave[x].jellykiller[y].activate = 2;
								}
								else
								{
									Mix_PlayChannel( -1, jellykiller.gotHit, 0 );
								}
							}
						}				
					}
					for ( i2 = 0 ; i2 < 12 ; i2++ )
					{
						if( iceSpearV[0][i2].activate == 1 )
						{
							if( circular_collision_detection( iceSpearV[0][i2].presentedRect, wave[x].jellykiller[y].presentedRect, 0 ) != 0 )
							{
								wave[x].jellykiller[y].healthPoints -= iceSpear.damage;
								Mix_PlayChannel( -1, iceSpear.hitWall, 0 );
								iceSpearV[0][i2].activate = 2;
								if( wave[x].jellykiller[y].healthPoints <= 0 )
								{
									wave[x].jellykiller[y].activate = 2;
								}
								else
								{
									Mix_PlayChannel( -1, jellykiller.gotHit, 0 );
								}
							}
						}				
					}
					for ( i2 = 0 ; i2 < 30 ; i2++ )
					{
						if( arcaneMissileV[0][i2].activate == 1 )
						{
							if( circular_collision_detection( arcaneMissileV[0][i2].presentedRect, wave[x].jellykiller[y].presentedRect, 0 ) != 0 )
							{
								wave[x].jellykiller[y].healthPoints -= arcaneMissile.damage;
								Mix_PlayChannel( -1, arcaneMissile.hitWall, 0 );
								arcaneMissileV[0][i2].activate = 2;
								if( wave[x].jellykiller[y].healthPoints <= 0 )
								{
									wave[x].jellykiller[y].activate = 2;
								}
								else
								{
									Mix_PlayChannel( -1, jellykiller.gotHit, 0 );
								}
							}
						}				
					}
					if( player.healthPoints > 0 )
					{
						circular_sprite_arrangement( wave[x].jellykiller[y].presentedRect, &player.presentedRect );
					}
					room_limits( &wave[x].jellykiller[y] );

					SDL_RenderCopy( l_renderer, jellykiller.sprite, &wave[x].jellykiller[y].snipRect, &wave[x].jellykiller[y].presentedRect );	
				}
				if( wave[x].jellykiller[y].activate == 3 )
	        	{
	        		wave[x].jellykiller[y].presentedRect.x = 900;
	        		wave[x].jellykiller[y].presentedRect.y = 900;
	        		wave[x].jellykiller[y].snipRect.x = 0;
	        		wave[x].jellykiller[y].healthPoints = jellykiller.healthPoints;
	        		wave[x].jellykiller[y].animationCycleCounter = 0;
	        		wave[x].jellykiller[y].deathAnimationCounter = 0;
	        	}
				// Animacao de morte do agua-morta
				if( wave[x].jellykiller[y].activate == 2 )
				{
					if( wave[x].jellykiller[y].deathAnimationCounter == 0)
					{
						wave[x].jellykiller[y].snipRect.x = 0;
					}

					wave[x].jellykiller[y].deathAnimationCounter++;

					if( wave[x].jellykiller[y].deathAnimationCounter%12 == 0 )
					{
						wave[x].jellykiller[y].snipRect.x += 60;
					}
					room_limits( &wave[x].jellykiller[y] );

					SDL_RenderCopy( l_renderer, jellykiller.spriteDeath, &wave[x].jellykiller[y].snipRect, &wave[x].jellykiller[y].presentedRect );

					if( wave[x].jellykiller[y].deathAnimationCounter == 156 )
					{
						wave[x].jellykiller[y].activate = 3;
					}
				}

	        }

	        for( y = 0 ; y < 20 ; y++ ) // Beholder
	        {
	        	if( wave[x].beholder[y].activate == 0 )
	        	{
	        		wave[x].beholder[y].presentedRect.x = 900;
	        		wave[x].beholder[y].presentedRect.y = 900;
	        		wave[x].beholder[y].snipRect.x = 0;
	        		wave[x].beholder[y].healthPoints = beholder.healthPoints;
	        		wave[x].beholder[y].animationCycleCounter = 0;
	        		wave[x].beholder[y].deathAnimationCounter = 0;
	        		wave[x].beholder[y].bulletNumber = 0;
	        	}	
	        	// Sobre o beholder
	           	if( wave[x].beholder[y].activate == 1 )
	           	{
	           		linear_walk_and_shoot( &wave[x].beholder[y], beholder, player, &wave[x].beholderBullet[y][wave[x].beholder[y].bulletNumber] ); 

					// Beholder ataca o jogador ao enconsta-lo
					if ( circular_collision_detection( player.presentedRect, wave[x].beholder[y].presentedRect, 5 ) != 0 )
					{
						player.healthPoints -= beholder.damage;
						Mix_PlayChannel( -1, player.gotHit, 0 );
					}

					// Beholder sofre dano pelos projeteis do jogador
					for ( i2 = 0 ; i2 < 50 ; i2++ )
					{
						if( fireBallV[0][i2].activate == 1 )
						{
							if( circular_collision_detection( fireBallV[0][i2].presentedRect, wave[x].beholder[y].presentedRect, 10 ) != 0 )
							{
								wave[x].beholder[y].healthPoints -= fireBall.damage;
								fireBallV[0][i2].activate = 2;
								Mix_PlayChannel( -1, fireBall.hitWall, 0 );
								if( wave[x].beholder[y].healthPoints <= 0 )
								{
									wave[x].beholder[y].activate = 2;
								}
								else
								{
									Mix_PlayChannel( -1, beholder.gotHit, 0 );
								}
							}
						}				
					}
					for ( i2 = 0 ; i2 < 12 ; i2++ )
					{
						if( iceSpearV[0][i2].activate == 1 )
						{
							if( circular_collision_detection( iceSpearV[0][i2].presentedRect, wave[x].beholder[y].presentedRect, 0 ) != 0 )
							{
								wave[x].beholder[y].healthPoints -= iceSpear.damage;
								iceSpearV[0][i2].activate = 2;
								Mix_PlayChannel( -1, iceSpear.hitWall, 0 );
								if( wave[x].beholder[y].healthPoints <= 0 )
								{
									wave[x].beholder[y].activate = 2;
								}
								else
								{
									Mix_PlayChannel( -1, beholder.gotHit, 0 );
								}
							}
						}				
					}
					for ( i2 = 0 ; i2 < 30 ; i2++ )
					{
						if( arcaneMissileV[0][i2].activate == 1 )
						{
							if( circular_collision_detection( arcaneMissileV[0][i2].presentedRect, wave[x].beholder[y].presentedRect, 0 ) != 0 )
							{
								wave[x].beholder[y].healthPoints -= arcaneMissile.damage;
								arcaneMissileV[0][i2].activate = 2;
								Mix_PlayChannel( -1, arcaneMissile.hitWall, 0 );
								if( wave[x].beholder[y].healthPoints <= 0 )
								{
									wave[x].beholder[y].activate = 2;
								}
								else
								{
									Mix_PlayChannel( -1, beholder.gotHit, 0 );
								}
							}
						}				
					}

					if( player.healthPoints > 0 )
					{
						circular_sprite_arrangement( wave[x].beholder[y].presentedRect, &player.presentedRect );
					}

					SDL_RenderCopy( l_renderer, beholder.sprite, &wave[x].beholder[y].snipRect, &wave[x].beholder[y].presentedRect );	
				}
				if( wave[x].beholder[y].activate == 3 )
	        	{
	        		wave[x].beholder[y].presentedRect.x = 900;
	        		wave[x].beholder[y].presentedRect.y = 900;
	        		wave[x].beholder[y].snipRect.x = 0;
	        		wave[x].beholder[y].healthPoints = beholder.healthPoints;
	        		wave[x].beholder[y].animationCycleCounter = 0;
	        		wave[x].beholder[y].deathAnimationCounter = 0;
	        		wave[x].beholder[y].bulletNumber = 0;
	        	}
				// Animacao de morte do beholder
				if( wave[x].beholder[y].activate == 2 )
				{
					if( wave[x].beholder[y].deathAnimationCounter == 0)
					{
						wave[x].beholder[y].snipRect.x = 0;
					}

					wave[x].beholder[y].deathAnimationCounter++;

					SDL_RenderCopy( l_renderer, beholder.spriteDeath, &wave[x].beholder[y].snipRect, &wave[x].beholder[y].presentedRect );

					if( wave[x].beholder[y].deathAnimationCounter%40 == 0 )
					{
						wave[x].beholder[y].snipRect.x += 90;
					}
					
					if( wave[x].beholder[y].deathAnimationCounter == 160 )
					{
						wave[x].beholder[y].activate = 3;
					}
				}	
	        }

	        for( y = 0 ; y < 24 ; y++ ) // Necromancer
	        {
	        	if( wave[x].necromancer[y].activate == 0 )
	        	{
	        		wave[x].necromancer[y].presentedRect.x = 900;
	        		wave[x].necromancer[y].presentedRect.y = 900;
	        		wave[x].necromancer[y].snipRect.x = 0;
	        		wave[x].necromancer[y].healthPoints = necromancer.healthPoints;
	        		wave[x].necromancer[y].animationCycleCounter = 0;
	        		wave[x].necromancer[y].deathAnimationCounter = 0;
	        		wave[x].necromancer[y].bulletNumber = 0;
	        	}	
	        	// Sobre o necromancer
	           	if( wave[x].necromancer[y].activate == 1 )
	           	{
	           		necromancer_ia( &wave[x].necromancer[y], necromancer, player, &wave[x].necroShot[y][wave[x].necromancer[y].bulletNumber], &wave[x].necroShot[y][wave[x].necromancer[y].bulletNumber+1] ); 

					// Necromancer ataca o jogador ao enconsta-lo
					if ( circular_collision_detection( player.presentedRect, wave[x].necromancer[y].presentedRect, 5 ) != 0 )
					{
						player.healthPoints -= necromancer.damage;
						Mix_PlayChannel( -1, player.gotHit, 0 );
					}

					// Necromancer sofre dano pelos projeteis do jogador
					for ( i2 = 0 ; i2 < 50 ; i2++ )
					{
						if( fireBallV[0][i2].activate == 1 )
						{
							if( circular_collision_detection( fireBallV[0][i2].presentedRect, wave[x].necromancer[y].presentedRect, 10 ) != 0 )
							{
								wave[x].necromancer[y].healthPoints -= fireBall.damage;
								fireBallV[0][i2].activate = 2;
								Mix_PlayChannel( -1, fireBall.hitWall, 0 );
								if( wave[x].necromancer[y].healthPoints <= 0 )
								{
									wave[x].necromancer[y].activate = 2;
								}
								else
								{
									Mix_PlayChannel( -1, necromancer.gotHit, 0 );
								}
							}
						}				
					}
					for ( i2 = 0 ; i2 < 12 ; i2++ )
					{
						if( iceSpearV[0][i2].activate == 1 )
						{
							if( circular_collision_detection( iceSpearV[0][i2].presentedRect, wave[x].necromancer[y].presentedRect, 0 ) != 0 )
							{
								wave[x].necromancer[y].healthPoints -= iceSpear.damage;
								iceSpearV[0][i2].activate = 2;
								Mix_PlayChannel( -1, iceSpear.hitWall, 0 );
								if( wave[x].necromancer[y].healthPoints <= 0 )
								{
									wave[x].necromancer[y].activate = 2;
								}
								else
								{
									Mix_PlayChannel( -1, necromancer.gotHit, 0 );
								}
							}
						}				
					}
					for ( i2 = 0 ; i2 < 30 ; i2++ )
					{
						if( arcaneMissileV[0][i2].activate == 1 )
						{
							if( circular_collision_detection( arcaneMissileV[0][i2].presentedRect, wave[x].necromancer[y].presentedRect, 0 ) != 0 )
							{
								wave[x].necromancer[y].healthPoints -= arcaneMissile.damage;
								arcaneMissileV[0][i2].activate = 2;
								Mix_PlayChannel( -1, arcaneMissile.hitWall, 0 );
								if( wave[x].necromancer[y].healthPoints <= 0 )
								{
									wave[x].necromancer[y].activate = 2;
								}
								else
								{
									Mix_PlayChannel( -1, necromancer.gotHit, 0 );
								}
							}
						}				
					}

					if( player.healthPoints > 0 )
					{
						circular_sprite_arrangement( wave[x].necromancer[y].presentedRect, &player.presentedRect );
					}

					SDL_RenderCopy( l_renderer, necromancer.sprite, &wave[x].necromancer[y].snipRect, &wave[x].necromancer[y].presentedRect );	
				}
				if( wave[x].necromancer[y].activate == 3 )
	        	{
	        		wave[x].necromancer[y].presentedRect.x = 900;
	        		wave[x].necromancer[y].presentedRect.y = 900;
	        		wave[x].necromancer[y].snipRect.x = 0;
	        		wave[x].necromancer[y].healthPoints = necromancer.healthPoints;
	        		wave[x].necromancer[y].animationCycleCounter = 0;
	        		wave[x].necromancer[y].deathAnimationCounter = 0;
	        		wave[x].necromancer[y].bulletNumber = 0;
	        	}
				// Animacao de morte do necromancer
				if( wave[x].necromancer[y].activate == 2 )
				{
					if( wave[x].necromancer[y].deathAnimationCounter == 0)
					{
						wave[x].necromancer[y].snipRect.x = 0;
					}

					wave[x].necromancer[y].deathAnimationCounter++;

					SDL_RenderCopy( l_renderer, necromancer.spriteDeath, &wave[x].necromancer[y].snipRect, &wave[x].necromancer[y].presentedRect );

					if( wave[x].necromancer[y].deathAnimationCounter%22 == 0 )
					{
						wave[x].necromancer[y].snipRect.x += 85;
					}
					
					if( wave[x].necromancer[y].deathAnimationCounter == 150 )
					{
						wave[x].necromancer[y].activate = 3;
					}
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
        	// Controla o firerate do player
        	if( player.shootDelayCounter%20 != 0 )
        	{
        		player.shootDelayCounter++;
        	}

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
					if( e.key.keysym.sym == SDLK_RETURN )
					{
						pauseScreen = 1;
					}

					// Itens aparecem
					if( e.key.keysym.sym == SDLK_LCTRL )
					{
						/*for( a = 0 ; a < 3 ; a++ )
						{
							if( greenCrystalV[a].activate == 0 )
							{
								greenCrystalV[a].activate = 1;
							}
						}*/
						for( a = 0 ; a < 9 ; a++ )
						{
							wave[a].activate = 2;
						}                                  
					}

					if( e.key.keysym.sym == SDLK_q )
					{
						kindOfShot++;
						if( kindOfShot > 2 )
						{
							kindOfShot = 0;
						}
						logoAnimationCounter = 0;
					}

					// Mata o jogador
					if( e.key.keysym.sym == SDLK_SPACE )
					{
						player.healthPoints = -1;                                
					}

					// O player atira 
					if( pressedKeyStates[SDL_SCANCODE_UP] ) 
					{
						if( player.shootDelayCounter%20 == 0 )
						{
							if( kindOfShot == 0 )
							{
								arcaneMissileV[0][j].presentedRect.x = player.presentedRect.x;
								arcaneMissileV[0][j].presentedRect.y = player.presentedRect.y; 
								arcaneMissileV[0][j].direction = 'u';
								arcaneMissileV[0][j].activate = 1;
								j++;
								if( j > 29 )
								{
									j = 0;
								}
							}
							else if( kindOfShot == 1 )
							{
								fireBallV[0][i].presentedRect.x = player.presentedRect.x;
								fireBallV[0][i].presentedRect.y = player.presentedRect.y; 
								fireBallV[0][i].direction = 'u';
								fireBallV[0][i].activate = 1;

								i++;

								if( i > 49 )
								{
									i = 0;
								}
							}

							else if( kindOfShot == 2 )
							{
								iceSpearV[0][j2].presentedRect.x = player.presentedRect.x;
								iceSpearV[0][j2].presentedRect.y = player.presentedRect.y; 
								iceSpearV[0][j2].direction = 'u';
								iceSpearV[0][j2].activate = 1;

								j2++;

								if( j2 > 11 )
								{
									j2 = 0;
								}

							}

							player.shootDelayCounter = 1;
						}
					}

					else if( pressedKeyStates[SDL_SCANCODE_DOWN] )
					{
						if( player.shootDelayCounter%20 == 0 )
						{
							if( kindOfShot == 0 )
							{
								arcaneMissileV[0][j].presentedRect.x = player.presentedRect.x;
								arcaneMissileV[0][j].presentedRect.y = player.presentedRect.y; 
								arcaneMissileV[0][j].direction = 'd';
								arcaneMissileV[0][j].activate = 1;
								j++;
								if( j > 29 )
								{
									j = 0;
								}
							}
							else if( kindOfShot == 1 )
							{
								fireBallV[0][i].presentedRect.x = player.presentedRect.x;
								fireBallV[0][i].presentedRect.y = player.presentedRect.y; 
								fireBallV[0][i].direction = 'd';
								fireBallV[0][i].activate = 1;
								i++;
								if( i > 49 )
								{
									i = 0;
								}
							}
							else if( kindOfShot == 2 )
							{
								iceSpearV[0][j2].presentedRect.x = player.presentedRect.x;
								iceSpearV[0][j2].presentedRect.y = player.presentedRect.y; 
								iceSpearV[0][j2].direction = 'd';
								iceSpearV[0][j2].activate = 1;
								j2++;
								if( j2 > 11 )
								{
									j2 = 0;
								}
							}
							player.shootDelayCounter = 1;
						}
					}

					else if( pressedKeyStates[SDL_SCANCODE_LEFT] )
					{
						if( player.shootDelayCounter%20 == 0 )
						{
							if( kindOfShot == 0 )
							{
								arcaneMissileV[0][j].presentedRect.x = player.presentedRect.x;
								arcaneMissileV[0][j].presentedRect.y = player.presentedRect.y; 
								arcaneMissileV[0][j].direction = 'l';
								arcaneMissileV[0][j].activate = 1;
								j++;
								if( j > 29 )
								{
									j = 0;
								}
							}
							else if( kindOfShot == 1 )
							{
								fireBallV[0][i].presentedRect.x = player.presentedRect.x;
								fireBallV[0][i].presentedRect.y = player.presentedRect.y; 
								fireBallV[0][i].direction = 'l';
								fireBallV[0][i].activate = 1;
								i++;
								if( i > 49 )
								{
									i = 0;
								}
							}
							else if( kindOfShot == 2 )
							{
								iceSpearV[0][j2].presentedRect.x = player.presentedRect.x;
								iceSpearV[0][j2].presentedRect.y = player.presentedRect.y; 
								iceSpearV[0][j2].direction = 'l';
								iceSpearV[0][j2].activate = 1;
								j2++;
								if( j2 > 11 )
								{
									j2 = 0;
								}
							}
							player.shootDelayCounter = 1;
						}
					}

					else if( pressedKeyStates[SDL_SCANCODE_RIGHT] )
					{
						if( player.shootDelayCounter%20 == 0 )
						{
							if( kindOfShot == 0 )
							{
								arcaneMissileV[0][j].presentedRect.x = player.presentedRect.x;
								arcaneMissileV[0][j].presentedRect.y = player.presentedRect.y; 
								arcaneMissileV[0][j].direction = 'r';
								arcaneMissileV[0][j].activate = 1;
								j++;
								if( j > 29 )
								{
									j = 0;
								}
							}
							else if( kindOfShot == 1 )
							{
								fireBallV[0][i].presentedRect.x = player.presentedRect.x;
								fireBallV[0][i].presentedRect.y = player.presentedRect.y; 
								fireBallV[0][i].direction = 'r';
								fireBallV[0][i].activate = 1;
								i++;
								if( i > 49 )
								{
									i = 0;
								}
							}
							else if( kindOfShot == 2 )
							{
								iceSpearV[0][j2].presentedRect.x = player.presentedRect.x;
								iceSpearV[0][j2].presentedRect.y = player.presentedRect.y; 
								iceSpearV[0][j2].direction = 'r';
								iceSpearV[0][j2].activate = 1;
								j2++;
								if( j2 > 11 )
								{
									j2 = 0;
								}
							}
							player.shootDelayCounter = 1;
						}
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

			// Apertou para torcar de tiro
			/*if( pressedKeyStates[SDL_SCANCODE_Q] ) 
			{
				kindOfShot++;
				if( kindOfShot > 2 )
				{
					kindOfShot = 0;
				}
				logoAnimationCounter = 0;
			}*/
					
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////FUNCIONAMENTO DAS WAVES///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			// Onde cada wave e ativada
			if( firstMap.snipRect.x/800 == 0 && firstMap.snipRect.y/600 == 1 && wave[0].activate == 0 )
			{
				wave[0].activate = 1;
				currentWave = 0;
				upSkeleton = 0, downSkeleton = 1, leftSkeleton = 2, rightSkeleton = 3;
				upJellykiller = 0, downJellykiller = 1, leftJellykiller = 2, rightJellykiller = 3;
				upBeholder = 0, downBeholder = 1, leftBeholder = 2, rightBeholder = 3;
				upNecromancer = 0, downNecromancer = 1, leftNecromancer = 2, rightNecromancer = 3;
				upQuit = 0, downQuit = 0, leftQuit = 0, rightQuit = 0;
			}
			else if( firstMap.snipRect.x/800 == 1 && firstMap.snipRect.y/600 == 0 && wave[1].activate == 0 )
			{
				wave[1].activate = 1;
				currentWave = 1;
				upSkeleton = 0, downSkeleton = 1, leftSkeleton = 2, rightSkeleton = 3;
				upJellykiller = 0, downJellykiller = 1, leftJellykiller = 2, rightJellykiller = 3;
				upBeholder = 0, downBeholder = 1, leftBeholder = 2, rightBeholder = 3;
				upNecromancer = 0, downNecromancer = 1, leftNecromancer = 2, rightNecromancer = 3;
				upQuit = 0, downQuit = 0, leftQuit = 0, rightQuit = 0;
			}
			else if( (firstMap.snipRect.x/800 == 2 && firstMap.snipRect.y/600 == 0) && wave[2].activate == 0 )
			{
				wave[2].activate = 1;
				currentWave = 2;
				upSkeleton = 0, downSkeleton = 1, leftSkeleton = 2, rightSkeleton = 3;
				upJellykiller = 0, downJellykiller = 1, leftJellykiller = 2, rightJellykiller = 3;
				upBeholder = 0, downBeholder = 1, leftBeholder = 2, rightBeholder = 3;
				upNecromancer = 0, downNecromancer = 1, leftNecromancer = 2, rightNecromancer = 3;
				upQuit = 0, downQuit = 0, leftQuit = 0, rightQuit = 0;
			}
			else if( (firstMap.snipRect.x/800 == 2 && firstMap.snipRect.y/600 == 1) && wave[3].activate == 0 )
			{
				wave[3].activate = 1;
				currentWave = 3;
				upSkeleton = 0, downSkeleton = 1, leftSkeleton = 2, rightSkeleton = 3;
				upJellykiller = 0, downJellykiller = 1, leftJellykiller = 2, rightJellykiller = 3;
				upBeholder = 0, downBeholder = 1, leftBeholder = 2, rightBeholder = 3;
				upNecromancer = 0, downNecromancer = 1, leftNecromancer = 2, rightNecromancer = 3;
				upQuit = 0, downQuit = 0, leftQuit = 0, rightQuit = 0;
			}
			else if( (firstMap.snipRect.x/800 == 3 && firstMap.snipRect.y/600 == 1) && wave[4].activate == 0 )
			{
				wave[4].activate = 1;
				currentWave = 4;
				upSkeleton = 0, downSkeleton = 1, leftSkeleton = 2, rightSkeleton = 3;
				upJellykiller = 0, downJellykiller = 1, leftJellykiller = 2, rightJellykiller = 3;
				upBeholder = 0, downBeholder = 1, leftBeholder = 2, rightBeholder = 3;
				upNecromancer = 0, downNecromancer = 1, leftNecromancer = 2, rightNecromancer = 3;
				upQuit = 0, downQuit = 0, leftQuit = 0, rightQuit = 0;
			}
			else if( (firstMap.snipRect.x/800 == 4 && firstMap.snipRect.y/600 == 1) && wave[5].activate == 0 )
			{
				wave[5].activate = 1;
				currentWave = 5;
				upSkeleton = 0, downSkeleton = 1, leftSkeleton = 2, rightSkeleton = 3;
				upJellykiller = 0, downJellykiller = 1, leftJellykiller = 2, rightJellykiller = 3;
				upBeholder = 0, downBeholder = 1, leftBeholder = 2, rightBeholder = 3;
				upNecromancer = 0, downNecromancer = 1, leftNecromancer = 2, rightNecromancer = 3;
				upQuit = 0, downQuit = 0, leftQuit = 0, rightQuit = 0;
			}
			else if( (firstMap.snipRect.x/800 == 4 && firstMap.snipRect.y/600 == 2) && wave[6].activate == 0 )
			{
				wave[6].activate = 1;
				currentWave = 6;
				upSkeleton = 0, downSkeleton = 1, leftSkeleton = 2, rightSkeleton = 3;
				upJellykiller = 0, downJellykiller = 1, leftJellykiller = 2, rightJellykiller = 3;
				upBeholder = 0, downBeholder = 1, leftBeholder = 2, rightBeholder = 3;
				upNecromancer = 0, downNecromancer = 1, leftNecromancer = 2, rightNecromancer = 3;
				upQuit = 0, downQuit = 0, leftQuit = 0, rightQuit = 0;
			}
			else if( (firstMap.snipRect.x/800 == 5 && firstMap.snipRect.y/600 == 2) && wave[7].activate == 0 )
			{
				wave[7].activate = 1;
				currentWave = 7;
				upSkeleton = 0, downSkeleton = 1, leftSkeleton = 2, rightSkeleton = 3;
				upJellykiller = 0, downJellykiller = 1, leftJellykiller = 2, rightJellykiller = 3;
				upBeholder = 0, downBeholder = 1, leftBeholder = 2, rightBeholder = 3;
				upNecromancer = 0, downNecromancer = 1, leftNecromancer = 2, rightNecromancer = 3;
				upQuit = 0, downQuit = 0, leftQuit = 0, rightQuit = 0;
			}
			else if( (firstMap.snipRect.x/800 == 5 && firstMap.snipRect.y/600 == 1) && wave[8].activate == 0 )
			{
				wave[8].activate = 1;
				currentWave = 8;
				upSkeleton = 0, downSkeleton = 1, leftSkeleton = 2, rightSkeleton = 3;
				upJellykiller = 0, downJellykiller = 1, leftJellykiller = 2, rightJellykiller = 3;
				upBeholder = 0, downBeholder = 1, leftBeholder = 2, rightBeholder = 3;
				upNecromancer = 0, downNecromancer = 1, leftNecromancer = 2, rightNecromancer = 3;
				upQuit = 0, downQuit = 0, leftQuit = 0, rightQuit = 0;
			}

			if( wave[0].activate == 2 && wave[1].activate == 2 && wave[2].activate == 2 && wave[3].activate == 2 && wave[4].activate == 2 && 
				wave[5].activate == 2 && wave[6].activate == 2 && wave[7].activate == 2 && wave[8].activate == 2 )
			{
				victoryScreen = 1;
			}

			if( wave[currentWave].activate == 1 )
			{
				room_limits( &player );

				switch( currentWave )
				{
					case 0:
						if( wave[0].skeleton[upSkeleton].activate == 0 )
						{
							wave[0].skeleton[upSkeleton].activate = 1;
							passing_through_door( 'u', &wave[0].skeleton[upSkeleton] );
						}

						else if( wave[0].skeleton[upSkeleton].activate == 2 )
						{
							if( upSkeleton < (40-4) )
							{
								upSkeleton += 4;
							}
							else
							{
								upSkeleton = 0;
							}
						}

						if( wave[0].skeleton[downSkeleton].activate == 0 )
						{
							wave[0].skeleton[downSkeleton].activate = 1;
							passing_through_door( 'd', &wave[0].skeleton[downSkeleton] );
						}
						else if( wave[0].skeleton[downSkeleton].activate == 2 )
						{
							if( downSkeleton < (40-4) )
							{
								downSkeleton += 4;
							}
							else
							{
								downSkeleton = 0;
							}
						}
						if( wave[0].skeleton[leftSkeleton].activate == 0 )
						{
							wave[0].skeleton[leftSkeleton].activate = 1;
							passing_through_door( 'l', &wave[0].skeleton[leftSkeleton] );
						}
						else if( wave[0].skeleton[leftSkeleton].activate == 2 )
						{
							if( leftSkeleton < (40-4) )
							{
								leftSkeleton += 4;
							}
							else
							{
								leftSkeleton = 0;
							}
						}
						if( wave[0].skeleton[rightSkeleton].activate == 0 )
						{
							wave[0].skeleton[rightSkeleton].activate = 1;
							passing_through_door( 'r', &wave[0].skeleton[rightSkeleton] );
						}
						else if( wave[0].skeleton[rightSkeleton].activate == 2 )
						{
							if( rightSkeleton < (40-4) )
							{
								rightSkeleton += 4;
							}
							else
							{
								rightSkeleton = 0;
							}
						}

						if( upSkeleton == 0 && downSkeleton == 0 && leftSkeleton == 0 && rightSkeleton == 0 )
						{
							wave[0].activate = 2;
						}
						break;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					case 1:
						if( wave[1].jellykiller[upJellykiller].activate == 0 )
						{
							wave[1].jellykiller[upJellykiller].activate = 1;
							passing_through_door( 'u', &wave[1].jellykiller[upJellykiller] );
						}

						else if( wave[1].jellykiller[upJellykiller].activate == 2 )
						{
							if( upJellykiller < (80-4) )
							{
								upJellykiller += 4;
							}
							else
							{
								upJellykiller = 0;
							}
						}

						if( wave[1].jellykiller[downJellykiller].activate == 0 )
						{
							wave[1].jellykiller[downJellykiller].activate = 1;
							passing_through_door( 'd', &wave[1].jellykiller[downJellykiller] );
						}
						else if( wave[1].jellykiller[downJellykiller].activate == 2 )
						{
							if( downJellykiller < (80-4) )
							{
								downJellykiller += 4;
							}
							else
							{
								downJellykiller = 0;
							}
						}
						if( wave[1].jellykiller[leftJellykiller].activate == 0 )
						{
							wave[1].jellykiller[leftJellykiller].activate = 1;
							passing_through_door( 'l', &wave[1].jellykiller[leftJellykiller] );
						}
						else if( wave[1].jellykiller[leftJellykiller].activate == 2 )
						{
							if( leftJellykiller < (80-4) )
							{
								leftJellykiller += 4;
							}
							else
							{
								leftJellykiller = 0;
							}
						}
						if( wave[1].jellykiller[rightJellykiller].activate == 0 )
						{
							wave[1].jellykiller[rightJellykiller].activate = 1;
							passing_through_door( 'r', &wave[1].jellykiller[rightJellykiller] );
						}
						else if( wave[1].jellykiller[rightJellykiller].activate == 2 )
						{
							if( rightJellykiller < (80-4) )
							{
								rightJellykiller += 4;
							}
							else
							{
								rightJellykiller = 0;
							}
						}

						if( upJellykiller == 0 && downJellykiller == 0 && leftJellykiller == 0 && rightJellykiller == 0 )
						{
							wave[1].activate = 2;
						}
						break;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					case 2:
						if( upSkeleton >= (1*4 - 4)  && upSkeleton <= (5*4 - 4) )
						{
							if( wave[currentWave].skeleton[upSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[upSkeleton].activate = 1;
								passing_through_door( 'u', &wave[currentWave].skeleton[upSkeleton] );
							}

							else if( wave[currentWave].skeleton[upSkeleton].activate == 2 )
							{
								upSkeleton += 4;
							}
						}

						else if( upJellykiller >= (1*4 - 4) && upJellykiller <= (7*4 - 4) )
						{
							if( wave[currentWave].jellykiller[upJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[upJellykiller].activate = 1;
								passing_through_door( 'u', &wave[currentWave].jellykiller[upJellykiller] );
							}
							else if( wave[currentWave].jellykiller[upJellykiller].activate == 2 )
							{
								upJellykiller += 4;
							}
						}

						else if( upSkeleton >= (6*4 - 4)  && upSkeleton <= (10*4 - 4) )
						{
							if( wave[currentWave].skeleton[upSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[upSkeleton].activate = 1;
								passing_through_door( 'u', &wave[currentWave].skeleton[upSkeleton] );
							}

							else if( wave[currentWave].skeleton[upSkeleton].activate == 2 )
							{
								upSkeleton += 4;
							}
						}

						else if( upJellykiller >= (8*4 - 4) && upJellykiller <= (13*4 - 4) )
						{
							if( wave[currentWave].jellykiller[upJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[upJellykiller].activate = 1;
								passing_through_door( 'u', &wave[currentWave].jellykiller[upJellykiller] );
							}
							else if( wave[currentWave].jellykiller[upJellykiller].activate == 2 )
							{
								upJellykiller += 4;
							}
						}
						else
						{
							upQuit = 1;
						}

						if( downSkeleton >= (1*4 - 4)  && downSkeleton <= (10*4 - 4) )
						{
							if( wave[currentWave].skeleton[downSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[downSkeleton].activate = 1;
								passing_through_door( 'd', &wave[currentWave].skeleton[downSkeleton] );
							}

							else if( wave[currentWave].skeleton[downSkeleton].activate == 2 )
							{
								downSkeleton += 4;
							}
						}

						else if( downJellykiller >= (1*4 - 4) && downJellykiller <= (13*4 - 4) )
						{
							if( wave[currentWave].jellykiller[downJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[downJellykiller].activate = 1;
								passing_through_door( 'd', &wave[currentWave].jellykiller[downJellykiller] );
							}
							else if( wave[currentWave].jellykiller[downJellykiller].activate == 2 )
							{
								downJellykiller += 4;
							}
						}
						else
						{
							downQuit = 1;
						}

						if( leftJellykiller >= (1*4 - 4) && leftJellykiller <= (13*4 - 4) )
						{
							if( wave[currentWave].jellykiller[leftJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[leftJellykiller].activate = 1;
								passing_through_door( 'l', &wave[currentWave].jellykiller[leftJellykiller] );
							}
							else if( wave[currentWave].jellykiller[leftJellykiller].activate == 2 )
							{
								leftJellykiller += 4;
							}
						}

						else if( leftSkeleton >= (1*4 - 4)  && leftSkeleton <= (10*4 - 4) )
						{
							if( wave[currentWave].skeleton[leftSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[leftSkeleton].activate = 1;
								passing_through_door( 'l', &wave[currentWave].skeleton[leftSkeleton] );
							}

							else if( wave[currentWave].skeleton[leftSkeleton].activate == 2 )
							{
								leftSkeleton += 4;
							}
						}
						else
						{
							leftQuit = 1;
						}

						if( rightJellykiller >= (1*4 - 4) && rightJellykiller <= (7*4 - 4) )
						{
							if( wave[currentWave].jellykiller[rightJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[rightJellykiller].activate = 1;
								passing_through_door( 'r', &wave[currentWave].jellykiller[rightJellykiller] );
							}
							else if( wave[currentWave].jellykiller[rightJellykiller].activate == 2 )
							{
								rightJellykiller += 4;
							}
						}

						else if( rightSkeleton >= (1*4 - 4)  && rightSkeleton <= (5*4 - 4) )
						{
							if( wave[currentWave].skeleton[rightSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[rightSkeleton].activate = 1;
								passing_through_door( 'r', &wave[currentWave].skeleton[rightSkeleton] );
							}

							else if( wave[currentWave].skeleton[rightSkeleton].activate == 2 )
							{
								rightSkeleton += 4;
							}
						}

						else if( rightJellykiller >= (8*4 - 4) && rightJellykiller <= (13*4 - 4) )
						{
							if( wave[currentWave].jellykiller[rightJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[rightJellykiller].activate = 1;
								passing_through_door( 'r', &wave[currentWave].jellykiller[rightJellykiller] );
							}
							else if( wave[currentWave].jellykiller[rightJellykiller].activate == 2 )
							{
								rightJellykiller += 4;
							}
						}

						else if( rightSkeleton >= (6*4 - 4)  && rightSkeleton <= (10*4 - 4) )
						{
							if( wave[currentWave].skeleton[rightSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[rightSkeleton].activate = 1;
								passing_through_door( 'r', &wave[currentWave].skeleton[rightSkeleton] );
							}

							else if( wave[currentWave].skeleton[rightSkeleton].activate == 2 )
							{
								rightSkeleton += 4;
							}
						}
						else
						{
							rightQuit = 1;
						}

						if( upQuit == 1 && downQuit == 1 && leftQuit == 1 && rightQuit == 1 ) 
						{
							wave[currentWave].activate = 2;
						}
						break;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					case 3:
						if( upSkeleton >= (1*4 - 4)  && upSkeleton <= (8*4 - 4) )
						{
							if( wave[currentWave].skeleton[upSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[upSkeleton].activate = 1;
								passing_through_door( 'u', &wave[currentWave].skeleton[upSkeleton] );
							}

							else if( wave[currentWave].skeleton[upSkeleton].activate == 2 )
							{
								upSkeleton += 4;
							}
						}

						else if( upBeholder >= (1*4 - 4)  && upBeholder <= (2*4 - 4) )
						{
							if( wave[currentWave].beholder[upBeholder].activate == 0 )
							{
								wave[currentWave].beholder[upBeholder].activate = 1;
								passing_through_door( 'u', &wave[currentWave].beholder[upBeholder] );
							}

							else if( wave[currentWave].beholder[upBeholder].activate == 2 )
							{
								upBeholder += 4;
							}
						}

						else if( upJellykiller >= (1*4 - 4) && upJellykiller <= (10*4 - 4) )
						{
							if( wave[currentWave].jellykiller[upJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[upJellykiller].activate = 1;
								passing_through_door( 'u', &wave[currentWave].jellykiller[upJellykiller] );
							}
							else if( wave[currentWave].jellykiller[upJellykiller].activate == 2 )
							{
								upJellykiller += 4;
							}
						}
						else
						{
							upQuit = 1;
						}

						if( downSkeleton >= (1*4 - 4)  && downSkeleton <= (8*4 - 4) )
						{
							if( wave[currentWave].skeleton[downSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[downSkeleton].activate = 1;
								passing_through_door( 'd', &wave[currentWave].skeleton[downSkeleton] );
							}

							else if( wave[currentWave].skeleton[downSkeleton].activate == 2 )
							{
								downSkeleton += 4;
							}
						}

						else if( downBeholder >= (1*4 - 4)  && downBeholder <= (2*4 - 4) )
						{
							if( wave[currentWave].beholder[downBeholder].activate == 0 )
							{
								wave[currentWave].beholder[downBeholder].activate = 1;
								passing_through_door( 'd', &wave[currentWave].beholder[downBeholder] );
							}

							else if( wave[currentWave].beholder[downBeholder].activate == 2 )
							{
								downBeholder += 4;
							}
						}

						else if( downJellykiller >= (1*4 - 4) && downJellykiller <= (10*4 - 4) )
						{
							if( wave[currentWave].jellykiller[downJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[downJellykiller].activate = 1;
								passing_through_door( 'd', &wave[currentWave].jellykiller[downJellykiller] );
							}
							else if( wave[currentWave].jellykiller[downJellykiller].activate == 2 )
							{
								downJellykiller += 4;
							}
						}
						else
						{
							downQuit = 1;
						}

						if( leftJellykiller >= (1*4 - 4) && leftJellykiller <= (10*4 - 4) )
						{
							if( wave[currentWave].jellykiller[leftJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[leftJellykiller].activate = 1;
								passing_through_door( 'l', &wave[currentWave].jellykiller[leftJellykiller] );
							}
							else if( wave[currentWave].jellykiller[leftJellykiller].activate == 2 )
							{
								leftJellykiller += 4;
							}
						}

						else if( leftSkeleton >= (1*4 - 4)  && leftSkeleton <= (8*4 - 4) )
						{
							if( wave[currentWave].skeleton[leftSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[leftSkeleton].activate = 1;
								passing_through_door( 'l', &wave[currentWave].skeleton[leftSkeleton] );
							}

							else if( wave[currentWave].skeleton[leftSkeleton].activate == 2 )
							{
								leftSkeleton += 4;
							}
						}

						else if( leftBeholder >= (1*4 - 4)  && leftBeholder <= (2*4 - 4) )
						{
							if( wave[currentWave].beholder[leftBeholder].activate == 0 )
							{
								wave[currentWave].beholder[leftBeholder].activate = 1;
								passing_through_door( 'l', &wave[currentWave].beholder[leftBeholder] );
							}

							else if( wave[currentWave].beholder[leftBeholder].activate == 2 )
							{
								leftBeholder += 4;
							}
						}
						else
						{
							leftQuit = 1;
						}

						if( rightJellykiller >= (1*4 - 4) && rightJellykiller <= (10*4 - 4) )
						{
							if( wave[currentWave].jellykiller[rightJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[rightJellykiller].activate = 1;
								passing_through_door( 'r', &wave[currentWave].jellykiller[rightJellykiller] );
							}
							else if( wave[currentWave].jellykiller[rightJellykiller].activate == 2 )
							{
								rightJellykiller += 4;
							}
						}

						else if( rightSkeleton >= (1*4 - 4)  && rightSkeleton <= (8*4 - 4) )
						{
							if( wave[currentWave].skeleton[rightSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[rightSkeleton].activate = 1;
								passing_through_door( 'r', &wave[currentWave].skeleton[rightSkeleton] );
							}

							else if( wave[currentWave].skeleton[rightSkeleton].activate == 2 )
							{
								rightSkeleton += 4;
							}
						}

						else if( rightBeholder >= (1*4 - 4)  && rightBeholder <= (2*4 - 4) )
						{
							if( wave[currentWave].beholder[rightBeholder].activate == 0 )
							{
								wave[currentWave].beholder[rightBeholder].activate = 1;
								passing_through_door( 'r', &wave[currentWave].beholder[rightBeholder] );
							}

							else if( wave[currentWave].beholder[rightBeholder].activate == 2 )
							{
								rightBeholder += 4;
							}
						}
						else
						{
							rightQuit = 1;
						}		

						if( upQuit == 1 && downQuit == 1 && leftQuit == 1 && rightQuit == 1 )
						{
							wave[currentWave].activate = 2;
						}
						break;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					case 4:
						if( upSkeleton >= (1*4 - 4)  && upSkeleton <= (7*4 - 4) )
						{
							if( wave[currentWave].skeleton[upSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[upSkeleton].activate = 1;
								passing_through_door( 'u', &wave[currentWave].skeleton[upSkeleton] );
							}

							else if( wave[currentWave].skeleton[upSkeleton].activate == 2 )
							{
								upSkeleton += 4;
							}
						}

						else if( upBeholder == (1*4 - 4) )
						{
							if( wave[currentWave].beholder[upBeholder].activate == 0 )
							{
								wave[currentWave].beholder[upBeholder].activate = 1;
								passing_through_door( 'u', &wave[currentWave].beholder[upBeholder] );
							}

							else if( wave[currentWave].beholder[upBeholder].activate == 2 )
							{
								upBeholder += 4;
							}
						}

						else if( upJellykiller >= (1*4 - 4) && upJellykiller <= (13*4 - 4) )
						{
							if( wave[currentWave].jellykiller[upJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[upJellykiller].activate = 1;
								passing_through_door( 'u', &wave[currentWave].jellykiller[upJellykiller] );
							}
							else if( wave[currentWave].jellykiller[upJellykiller].activate == 2 )
							{
								upJellykiller += 4;
							}
						}

						else if( upBeholder >= (2*4 - 4) && upBeholder <= (3*4 - 4) )
						{
							if( wave[currentWave].beholder[upBeholder].activate == 0 )
							{
								wave[currentWave].beholder[upBeholder].activate = 1;
								passing_through_door( 'u', &wave[currentWave].beholder[upBeholder] );
							}

							else if( wave[currentWave].beholder[upBeholder].activate == 2 )
							{
								upBeholder += 4;
							}
						}
						else
						{
							upQuit = 1;
						}

						if( downSkeleton >= (1*4 - 4)  && downSkeleton <= (7*4 - 4) )
						{
							if( wave[currentWave].skeleton[downSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[downSkeleton].activate = 1;
								passing_through_door( 'd', &wave[currentWave].skeleton[downSkeleton] );
							}

							else if( wave[currentWave].skeleton[downSkeleton].activate == 2 )
							{
								downSkeleton += 4;
							}
						}

						else if( downBeholder >= (1*4 - 4) && downBeholder <= (2*4 - 4) )
						{
							if( wave[currentWave].beholder[downBeholder].activate == 0 )
							{
								wave[currentWave].beholder[downBeholder].activate = 1;
								passing_through_door( 'd', &wave[currentWave].beholder[downBeholder] );
							}

							else if( wave[currentWave].beholder[downBeholder].activate == 2 )
							{
								downBeholder += 4;
							}
						}

						else if( downJellykiller >= (1*4 - 4) && downJellykiller <= (13*4 - 4) )
						{
							if( wave[currentWave].jellykiller[downJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[downJellykiller].activate = 1;
								passing_through_door( 'd', &wave[currentWave].jellykiller[downJellykiller] );
							}
							else if( wave[currentWave].jellykiller[downJellykiller].activate == 2 )
							{
								downJellykiller += 4;
							}
						}

						else if( downBeholder == (3*4 - 4) )
						{
							if( wave[currentWave].beholder[downBeholder].activate == 0 )
							{
								wave[currentWave].beholder[downBeholder].activate = 1;
								passing_through_door( 'd', &wave[currentWave].beholder[downBeholder] );
							}

							else if( wave[currentWave].beholder[downBeholder].activate == 2 )
							{
								downBeholder += 4;
							}
						}
						else
						{
							downQuit = 1;
						}

						if( leftJellykiller >= (1*4 - 4) && leftJellykiller <= (7*4 - 4) )
						{
							if( wave[currentWave].jellykiller[leftJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[leftJellykiller].activate = 1;
								passing_through_door( 'l', &wave[currentWave].jellykiller[leftJellykiller] );
							}
							else if( wave[currentWave].jellykiller[leftJellykiller].activate == 2 )
							{
								leftJellykiller += 4;
							}
						}

						else if( leftSkeleton >= (1*4 - 4)  && leftSkeleton <= (7*4 - 4) )
						{
							if( wave[currentWave].skeleton[leftSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[leftSkeleton].activate = 1;
								passing_through_door( 'l', &wave[currentWave].skeleton[leftSkeleton] );
							}

							else if( wave[currentWave].skeleton[leftSkeleton].activate == 2 )
							{
								leftSkeleton += 4;
							}
						}

						else if( leftJellykiller >= (8*4 - 4) && leftJellykiller <= (13*4 - 4) )
						{
							if( wave[currentWave].jellykiller[leftJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[leftJellykiller].activate = 1;
								passing_through_door( 'l', &wave[currentWave].jellykiller[leftJellykiller] );
							}
							else if( wave[currentWave].jellykiller[leftJellykiller].activate == 2 )
							{
								leftJellykiller += 4;
							}
						}

						else if( leftBeholder >= (1*4 - 4)  && leftBeholder <= (3*4 - 4) )
						{
							if( wave[currentWave].beholder[leftBeholder].activate == 0 )
							{
								wave[currentWave].beholder[leftBeholder].activate = 1;
								passing_through_door( 'l', &wave[currentWave].beholder[leftBeholder] );
							}

							else if( wave[currentWave].beholder[leftBeholder].activate == 2 )
							{
								leftBeholder += 4;
							}
						}
						else
						{
							leftQuit = 1;
						}

						if( rightJellykiller >= (1*4 - 4) && rightJellykiller <= (7*4 - 4) )
						{
							if( wave[currentWave].jellykiller[rightJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[rightJellykiller].activate = 1;
								passing_through_door( 'r', &wave[currentWave].jellykiller[rightJellykiller] );
							}
							else if( wave[currentWave].jellykiller[rightJellykiller].activate == 2 )
							{
								rightJellykiller += 4;
							}
						}

						else if( rightSkeleton >= (1*4 - 4)  && rightSkeleton <= (7*4 - 4) )
						{
							if( wave[currentWave].skeleton[rightSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[rightSkeleton].activate = 1;
								passing_through_door( 'r', &wave[currentWave].skeleton[rightSkeleton] );
							}

							else if( wave[currentWave].skeleton[rightSkeleton].activate == 2 )
							{
								rightSkeleton += 4;
							}
						}

						else if( rightJellykiller >= (8*4 - 4) && rightJellykiller <= (13*4 - 4) )
						{
							if( wave[currentWave].jellykiller[rightJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[rightJellykiller].activate = 1;
								passing_through_door( 'r', &wave[currentWave].jellykiller[rightJellykiller] );
							}
							else if( wave[currentWave].jellykiller[rightJellykiller].activate == 2 )
							{
								rightJellykiller += 4;
							}
						}

						else if( rightBeholder >= (1*4 - 4)  && rightBeholder <= (3*4 - 4) )
						{
							if( wave[currentWave].beholder[rightBeholder].activate == 0 )
							{
								wave[currentWave].beholder[rightBeholder].activate = 1;
								passing_through_door( 'r', &wave[currentWave].beholder[rightBeholder] );
							}

							else if( wave[currentWave].beholder[rightBeholder].activate == 2 )
							{
								rightBeholder += 4;
							}
						}
						else
						{
							rightQuit = 1;
						}		

						if( upQuit == 1 && downQuit == 1 && leftQuit == 1 && rightQuit == 1 ) 
						{
							wave[currentWave].activate = 2;
						}
						break;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					case 5:
						if( upBeholder >= (1*4 - 4) && upBeholder <= (2*4 - 4) )
						{
							if( wave[currentWave].beholder[upBeholder].activate == 0 )
							{
								wave[currentWave].beholder[upBeholder].activate = 1;
								passing_through_door( 'u', &wave[currentWave].beholder[upBeholder] );
							}

							else if( wave[currentWave].beholder[upBeholder].activate == 2 )
							{
								upBeholder += 4;
							}
						}

						else if( upSkeleton >= (1*4 - 4)  && upSkeleton <= (10*4 - 4) )
						{
							if( wave[currentWave].skeleton[upSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[upSkeleton].activate = 1;
								passing_through_door( 'u', &wave[currentWave].skeleton[upSkeleton] );
							}

							else if( wave[currentWave].skeleton[upSkeleton].activate == 2 )
							{
								upSkeleton += 4;
							}
						}

						else if( upBeholder >= (3*4 - 4) && upBeholder <= (4*4 - 4) )
						{
							if( wave[currentWave].beholder[upBeholder].activate == 0 )
							{
								wave[currentWave].beholder[upBeholder].activate = 1;
								passing_through_door( 'u', &wave[currentWave].beholder[upBeholder] );
							}

							else if( wave[currentWave].beholder[upBeholder].activate == 2 )
							{
								upBeholder += 4;
							}
						}

						else if( upJellykiller >= (1*4 - 4) && upJellykiller <= (7*4 - 4) )
						{
							if( wave[currentWave].jellykiller[upJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[upJellykiller].activate = 1;
								passing_through_door( 'u', &wave[currentWave].jellykiller[upJellykiller] );
							}
							else if( wave[currentWave].jellykiller[upJellykiller].activate == 2 )
							{
								upJellykiller += 4;
							}
						}
						else
						{
							upQuit = 1;
						}

						if( downBeholder >= (1*4 - 4) && downBeholder <= (2*4 - 4) )
						{
							if( wave[currentWave].beholder[downBeholder].activate == 0 )
							{
								wave[currentWave].beholder[downBeholder].activate = 1;
								passing_through_door( 'd', &wave[currentWave].beholder[downBeholder] );
							}

							else if( wave[currentWave].beholder[downBeholder].activate == 2 )
							{
								downBeholder += 4;
							}
						}

						else if( downJellykiller >= (1*4 - 4) && downJellykiller <= (7*4 - 4) )
						{
							if( wave[currentWave].jellykiller[downJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[downJellykiller].activate = 1;
								passing_through_door( 'd', &wave[currentWave].jellykiller[downJellykiller] );
							}
							else if( wave[currentWave].jellykiller[downJellykiller].activate == 2 )
							{
								downJellykiller += 4;
							}
						}

						else if( downSkeleton >= (1*4 - 4)  && downSkeleton <= (10*4 - 4) )
						{
							if( wave[currentWave].skeleton[downSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[downSkeleton].activate = 1;
								passing_through_door( 'd', &wave[currentWave].skeleton[downSkeleton] );
							}

							else if( wave[currentWave].skeleton[downSkeleton].activate == 2 )
							{
								downSkeleton += 4;
							}
						}

						else if( downBeholder >= (3*4 - 4) && downBeholder <= (4*4 - 4) )
						{
							if( wave[currentWave].beholder[downBeholder].activate == 0 )
							{
								wave[currentWave].beholder[downBeholder].activate = 1;
								passing_through_door( 'd', &wave[currentWave].beholder[downBeholder] );
							}

							else if( wave[currentWave].beholder[downBeholder].activate == 2 )
							{
								downBeholder += 4;
							}
						}
						else
						{
							downQuit = 1;
						}

						if( leftBeholder >= (1*4 - 4) && leftBeholder <= (2*4 - 4) )
						{
							if( wave[currentWave].beholder[leftBeholder].activate == 0 )
							{
								wave[currentWave].beholder[leftBeholder].activate = 1;
								passing_through_door( 'l', &wave[currentWave].beholder[leftBeholder] );
							}

							else if( wave[currentWave].beholder[leftBeholder].activate == 2 )
							{
								leftBeholder += 4;
							}
						}

						else if( leftSkeleton >= (1*4 - 4)  && leftSkeleton <= (10*4 - 4) )
						{
							if( wave[currentWave].skeleton[leftSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[leftSkeleton].activate = 1;
								passing_through_door( 'l', &wave[currentWave].skeleton[leftSkeleton] );
							}

							else if( wave[currentWave].skeleton[leftSkeleton].activate == 2 )
							{
								leftSkeleton += 4;
							}
						}

						else if( leftBeholder >= (3*4 - 4) && leftBeholder <= (4*4 - 4) )
						{
							if( wave[currentWave].beholder[leftBeholder].activate == 0 )
							{
								wave[currentWave].beholder[leftBeholder].activate = 1;
								passing_through_door( 'l', &wave[currentWave].beholder[leftBeholder] );
							}

							else if( wave[currentWave].beholder[leftBeholder].activate == 2 )
							{
								leftBeholder += 4;
							}
						}

						else if( leftJellykiller >= (1*4 - 4) && leftJellykiller <= (7*4 - 4) )
						{
							if( wave[currentWave].jellykiller[leftJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[leftJellykiller].activate = 1;
								passing_through_door( 'l', &wave[currentWave].jellykiller[leftJellykiller] );
							}
							else if( wave[currentWave].jellykiller[leftJellykiller].activate == 2 )
							{
								leftJellykiller += 4;
							}
						}
						else
						{
							leftQuit = 1;
						}

						if( rightBeholder >= (1*4 - 4) && rightBeholder <= (2*4 - 4) )
						{
							if( wave[currentWave].beholder[rightBeholder].activate == 0 )
							{
								wave[currentWave].beholder[rightBeholder].activate = 1;
								passing_through_door( 'r', &wave[currentWave].beholder[rightBeholder] );
							}

							else if( wave[currentWave].beholder[rightBeholder].activate == 2 )
							{
								rightBeholder += 4;
							}
						}

						else if( rightJellykiller >= (1*4 - 4) && rightJellykiller <= (7*4 - 4) )
						{
							if( wave[currentWave].jellykiller[rightJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[rightJellykiller].activate = 1;
								passing_through_door( 'r', &wave[currentWave].jellykiller[rightJellykiller] );
							}
							else if( wave[currentWave].jellykiller[rightJellykiller].activate == 2 )
							{
								rightJellykiller += 4;
							}
						}

						else if( rightSkeleton >= (1*4 - 4)  && rightSkeleton <= (10*4 - 4) )
						{
							if( wave[currentWave].skeleton[rightSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[rightSkeleton].activate = 1;
								passing_through_door( 'r', &wave[currentWave].skeleton[rightSkeleton] );
							}

							else if( wave[currentWave].skeleton[rightSkeleton].activate == 2 )
							{
								rightSkeleton += 4;
							}
						}

						else if( rightBeholder >= (3*4 - 4) && rightBeholder <= (4*4 - 4) )
						{
							if( wave[currentWave].beholder[rightBeholder].activate == 0 )
							{
								wave[currentWave].beholder[rightBeholder].activate = 1;
								passing_through_door( 'r', &wave[currentWave].beholder[rightBeholder] );
							}

							else if( wave[currentWave].beholder[rightBeholder].activate == 2 )
							{
								rightBeholder += 4;
							}
						}
						else
						{
							rightQuit = 1;
						}		

						if( upQuit == 1 && downQuit == 1 && leftQuit == 1 && rightQuit == 1 )
						{
							wave[currentWave].activate = 2;
						}
						break;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					case 6:
						if( upJellykiller >= (1*4 - 4) && upJellykiller <= (7*4 - 4) )
						{
							if( wave[currentWave].jellykiller[upJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[upJellykiller].activate = 1;
								passing_through_door( 'u', &wave[currentWave].jellykiller[upJellykiller] );
							}
							else if( wave[currentWave].jellykiller[upJellykiller].activate == 2 )
							{
								upJellykiller += 4;
							}
						}

						else if( upBeholder >= (1*4 - 4) && upBeholder <= (2*4 - 4) )
						{
							if( wave[currentWave].beholder[upBeholder].activate == 0 )
							{
								wave[currentWave].beholder[upBeholder].activate = 1;
								passing_through_door( 'u', &wave[currentWave].beholder[upBeholder] );
							}

							else if( wave[currentWave].beholder[upBeholder].activate == 2 )
							{
								upBeholder += 4;
							}
						}

						else if( upSkeleton >= (1*4 - 4)  && upSkeleton <= (4*4 - 4) )
						{
							if( wave[currentWave].skeleton[upSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[upSkeleton].activate = 1;
								passing_through_door( 'u', &wave[currentWave].skeleton[upSkeleton] );
							}

							else if( wave[currentWave].skeleton[upSkeleton].activate == 2 )
							{
								upSkeleton += 4;
							}
						}

						else if( upNecromancer >= (1*4 - 4) && upNecromancer <= (2*4 - 4) )
						{
							if( wave[currentWave].necromancer[upNecromancer].activate == 0 )
							{
								wave[currentWave].necromancer[upNecromancer].activate = 1;
								passing_through_door( 'u', &wave[currentWave].necromancer[upNecromancer] );
							}

							else if( wave[currentWave].necromancer[upNecromancer].activate == 2 )
							{
								upNecromancer += 4;
							}
						}

						else if( upSkeleton >= (5*4 - 4)  && upSkeleton <= (8*4 - 4) )
						{
							if( wave[currentWave].skeleton[upSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[upSkeleton].activate = 1;
								passing_through_door( 'u', &wave[currentWave].skeleton[upSkeleton] );
							}

							else if( wave[currentWave].skeleton[upSkeleton].activate == 2 )
							{
								upSkeleton += 4;
							}
						}
						else
						{
							upQuit = 1;
						}

						if( downJellykiller >= (1*4 - 4) && downJellykiller <= (7*4 - 4) )
						{
							if( wave[currentWave].jellykiller[downJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[downJellykiller].activate = 1;
								passing_through_door( 'd', &wave[currentWave].jellykiller[downJellykiller] );
							}
							else if( wave[currentWave].jellykiller[downJellykiller].activate == 2 )
							{
								downJellykiller += 4;
							}
						}

						else if( downBeholder >= (1*4 - 4) && downBeholder <= (2*4 - 4) )
						{
							if( wave[currentWave].beholder[downBeholder].activate == 0 )
							{
								wave[currentWave].beholder[downBeholder].activate = 1;
								passing_through_door( 'd', &wave[currentWave].beholder[downBeholder] );
							}

							else if( wave[currentWave].beholder[downBeholder].activate == 2 )
							{
								downBeholder += 4;
							}
						}

						else if( downSkeleton >= (1*4 - 4)  && downSkeleton <= (4*4 - 4) )
						{
							if( wave[currentWave].skeleton[downSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[downSkeleton].activate = 1;
								passing_through_door( 'd', &wave[currentWave].skeleton[downSkeleton] );
							}

							else if( wave[currentWave].skeleton[downSkeleton].activate == 2 )
							{
								downSkeleton += 4;
							}
						}

						else if( downNecromancer >= (1*4 - 4) && downNecromancer <= (2*4 - 4) )
						{
							if( wave[currentWave].necromancer[downNecromancer].activate == 0 )
							{
								wave[currentWave].necromancer[downNecromancer].activate = 1;
								passing_through_door( 'd', &wave[currentWave].necromancer[downNecromancer] );
							}

							else if( wave[currentWave].necromancer[downNecromancer].activate == 2 )
							{
								downNecromancer += 4;
							}
						}

						else if( downSkeleton >= (5*4 - 4)  && downSkeleton <= (8*4 - 4) )
						{
							if( wave[currentWave].skeleton[downSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[downSkeleton].activate = 1;
								passing_through_door( 'd', &wave[currentWave].skeleton[downSkeleton] );
							}

							else if( wave[currentWave].skeleton[downSkeleton].activate == 2 )
							{
								downSkeleton += 4;
							}
						}
						else
						{
							downQuit = 1;
						}

						if( leftJellykiller >= (1*4 - 4) && leftJellykiller <= (7*4 - 4) )
						{
							if( wave[currentWave].jellykiller[leftJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[leftJellykiller].activate = 1;
								passing_through_door( 'l', &wave[currentWave].jellykiller[leftJellykiller] );
							}
							else if( wave[currentWave].jellykiller[leftJellykiller].activate == 2 )
							{
								leftJellykiller += 4;
							}
						}

						else if( leftBeholder >= (1*4 - 4) && leftBeholder <= (2*4 - 4) )
						{
							if( wave[currentWave].beholder[leftBeholder].activate == 0 )
							{
								wave[currentWave].beholder[leftBeholder].activate = 1;
								passing_through_door( 'l', &wave[currentWave].beholder[leftBeholder] );
							}

							else if( wave[currentWave].beholder[leftBeholder].activate == 2 )
							{
								leftBeholder += 4;
							}
						}

						else if( leftNecromancer >= (1*4 - 4) && leftNecromancer <= (2*4 - 4) )
						{
							if( wave[currentWave].necromancer[leftNecromancer].activate == 0 )
							{
								wave[currentWave].necromancer[leftNecromancer].activate = 1;
								passing_through_door( 'l', &wave[currentWave].necromancer[leftNecromancer] );
							}

							else if( wave[currentWave].necromancer[leftNecromancer].activate == 2 )
							{
								leftNecromancer += 4;
							}
						}

						else if( leftSkeleton >= (1*4 - 4)  && leftSkeleton <= (8*4 - 4) )
						{
							if( wave[currentWave].skeleton[leftSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[leftSkeleton].activate = 1;
								passing_through_door( 'l', &wave[currentWave].skeleton[leftSkeleton] );
							}

							else if( wave[currentWave].skeleton[leftSkeleton].activate == 2 )
							{
								leftSkeleton += 4;
							}
						}
						else
						{
							leftQuit = 1;
						}

						if( rightJellykiller >= (1*4 - 4) && rightJellykiller <= (7*4 - 4) )
						{
							if( wave[currentWave].jellykiller[rightJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[rightJellykiller].activate = 1;
								passing_through_door( 'r', &wave[currentWave].jellykiller[rightJellykiller] );
							}
							else if( wave[currentWave].jellykiller[rightJellykiller].activate == 2 )
							{
								rightJellykiller += 4;
							}
						}

						else if( rightBeholder >= (1*4 - 4) && rightBeholder <= (2*4 - 4) )
						{
							if( wave[currentWave].beholder[rightBeholder].activate == 0 )
							{
								wave[currentWave].beholder[rightBeholder].activate = 1;
								passing_through_door( 'r', &wave[currentWave].beholder[rightBeholder] );
							}

							else if( wave[currentWave].beholder[rightBeholder].activate == 2 )
							{
								rightBeholder += 4;
							}
						}

						else if( rightNecromancer >= (1*4 - 4) && rightNecromancer <= (2*4 - 4) )
						{
							if( wave[currentWave].necromancer[rightNecromancer].activate == 0 )
							{
								wave[currentWave].necromancer[rightNecromancer].activate = 1;
								passing_through_door( 'r', &wave[currentWave].necromancer[rightNecromancer] );
							}

							else if( wave[currentWave].necromancer[rightNecromancer].activate == 2 )
							{
								rightNecromancer += 4;
							}
						}

						else if( rightSkeleton >= (1*4 - 4)  && rightSkeleton <= (8*4 - 4) )
						{
							if( wave[currentWave].skeleton[rightSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[rightSkeleton].activate = 1;
								passing_through_door( 'r', &wave[currentWave].skeleton[rightSkeleton] );
							}

							else if( wave[currentWave].skeleton[rightSkeleton].activate == 2 )
							{
								rightSkeleton += 4;
							}
						}
						else
						{
							rightQuit = 1;
						}	

						if( upQuit == 1 && downQuit == 1 && leftQuit == 1 && rightQuit == 1 )
						{
							wave[currentWave].activate = 2;
						}
						break;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					case 7:
						if( upNecromancer >= (1*4 - 4) && upNecromancer <= (3*4 - 4) )
						{
							if( wave[currentWave].necromancer[upNecromancer].activate == 0 )
							{
								wave[currentWave].necromancer[upNecromancer].activate = 1;
								passing_through_door( 'u', &wave[currentWave].necromancer[upNecromancer] );
							}

							else if( wave[currentWave].necromancer[upNecromancer].activate == 2 )
							{
								upNecromancer += 4;
							}
						}

						else if( upSkeleton >= (1*4 - 4)  && upSkeleton <= (4*4 - 4) )
						{
							if( wave[currentWave].skeleton[upSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[upSkeleton].activate = 1;
								passing_through_door( 'u', &wave[currentWave].skeleton[upSkeleton] );
							}

							else if( wave[currentWave].skeleton[upSkeleton].activate == 2 )
							{
								upSkeleton += 4;
							}
						}

						else if( upJellykiller >= (1*4 - 4) && upJellykiller <= (5*4 - 4) )
						{
							if( wave[currentWave].jellykiller[upJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[upJellykiller].activate = 1;
								passing_through_door( 'u', &wave[currentWave].jellykiller[upJellykiller] );
							}
							else if( wave[currentWave].jellykiller[upJellykiller].activate == 2 )
							{
								upJellykiller += 4;
							}
						}

						else if( upSkeleton >= (5*4 - 4)  && upSkeleton <= (8*4 - 4) )
						{
							if( wave[currentWave].skeleton[upSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[upSkeleton].activate = 1;
								passing_through_door( 'u', &wave[currentWave].skeleton[upSkeleton] );
							}

							else if( wave[currentWave].skeleton[upSkeleton].activate == 2 )
							{
								upSkeleton += 4;
							}
						}

						else if( upBeholder >= (1*4 - 4) && upBeholder <= (2*4 - 4) )
						{
							if( wave[currentWave].beholder[upBeholder].activate == 0 )
							{
								wave[currentWave].beholder[upBeholder].activate = 1;
								passing_through_door( 'u', &wave[currentWave].beholder[upBeholder] );
							}

							else if( wave[currentWave].beholder[upBeholder].activate == 2 )
							{
								upBeholder += 4;
							}
						}

						else if( upSkeleton >= (9*4 - 4)  && upSkeleton <= (12*4 - 4) )
						{
							if( wave[currentWave].skeleton[upSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[upSkeleton].activate = 1;
								passing_through_door( 'u', &wave[currentWave].skeleton[upSkeleton] );
							}

							else if( wave[currentWave].skeleton[upSkeleton].activate == 2 )
							{
								upSkeleton += 4;
							}
						}

						else if( upJellykiller >= (6*4 - 4) && upJellykiller <= (10*4 - 4) )
						{
							if( wave[currentWave].jellykiller[upJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[upJellykiller].activate = 1;
								passing_through_door( 'u', &wave[currentWave].jellykiller[upJellykiller] );
							}
							else if( wave[currentWave].jellykiller[upJellykiller].activate == 2 )
							{
								upJellykiller += 4;
							}
						}
						else
						{
							upQuit = 1;
						}

						if( downJellykiller >= (1*4 - 4) && downJellykiller <= (5*4 - 4) )
						{
							if( wave[currentWave].jellykiller[downJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[downJellykiller].activate = 1;
								passing_through_door( 'd', &wave[currentWave].jellykiller[downJellykiller] );
							}
							else if( wave[currentWave].jellykiller[downJellykiller].activate == 2 )
							{
								downJellykiller += 4;
							}
						}

						else if( downSkeleton >= (1*4 - 4)  && downSkeleton <= (4*4 - 4) )
						{
							if( wave[currentWave].skeleton[downSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[downSkeleton].activate = 1;
								passing_through_door( 'd', &wave[currentWave].skeleton[downSkeleton] );
							}

							else if( wave[currentWave].skeleton[downSkeleton].activate == 2 )
							{
								downSkeleton += 4;
							}
						}

						else if( downBeholder >= (1*4 - 4) && downBeholder <= (2*4 - 4) )
						{
							if( wave[currentWave].beholder[downBeholder].activate == 0 )
							{
								wave[currentWave].beholder[downBeholder].activate = 1;
								passing_through_door( 'd', &wave[currentWave].beholder[downBeholder] );
							}

							else if( wave[currentWave].beholder[downBeholder].activate == 2 )
							{
								downBeholder += 4;
							}
						}

						else if( downSkeleton >= (5*4 - 4)  && downSkeleton <= (8*4 - 4) )
						{
							if( wave[currentWave].skeleton[downSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[downSkeleton].activate = 1;
								passing_through_door( 'd', &wave[currentWave].skeleton[downSkeleton] );
							}

							else if( wave[currentWave].skeleton[downSkeleton].activate == 2 )
							{
								downSkeleton += 4;
							}
						}

						else if( downJellykiller >= (6*4 - 4) && downJellykiller <= (10*4 - 4) )
						{
							if( wave[currentWave].jellykiller[downJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[downJellykiller].activate = 1;
								passing_through_door( 'd', &wave[currentWave].jellykiller[downJellykiller] );
							}
							else if( wave[currentWave].jellykiller[downJellykiller].activate == 2 )
							{
								downJellykiller += 4;
							}
						}

						else if( downSkeleton >= (9*4 - 4)  && downSkeleton <= (12*4 - 4) )
						{
							if( wave[currentWave].skeleton[downSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[downSkeleton].activate = 1;
								passing_through_door( 'd', &wave[currentWave].skeleton[downSkeleton] );
							}

							else if( wave[currentWave].skeleton[downSkeleton].activate == 2 )
							{
								downSkeleton += 4;
							}
						}

						else if( downNecromancer >= (1*4 - 4) && downNecromancer <= (3*4 - 4) )
						{
							if( wave[currentWave].necromancer[downNecromancer].activate == 0 )
							{
								wave[currentWave].necromancer[downNecromancer].activate = 1;
								passing_through_door( 'd', &wave[currentWave].necromancer[downNecromancer] );
							}

							else if( wave[currentWave].necromancer[downNecromancer].activate == 2 )
							{
								downNecromancer += 4;
							}
						}
						else
						{
							downQuit = 1;
						}

						if( leftSkeleton >= (1*4 - 4)  && leftSkeleton <= (6*4 - 4) )
						{
							if( wave[currentWave].skeleton[leftSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[leftSkeleton].activate = 1;
								passing_through_door( 'l', &wave[currentWave].skeleton[leftSkeleton] );
							}

							else if( wave[currentWave].skeleton[leftSkeleton].activate == 2 )
							{
								leftSkeleton += 4;
							}
						}

						else if( leftJellykiller >= (1*4 - 4) && leftJellykiller <= (5*4 - 4) )
						{
							if( wave[currentWave].jellykiller[leftJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[leftJellykiller].activate = 1;
								passing_through_door( 'l', &wave[currentWave].jellykiller[leftJellykiller] );
							}
							else if( wave[currentWave].jellykiller[leftJellykiller].activate == 2 )
							{
								leftJellykiller += 4;
							}
						}

						else if( leftNecromancer >= (1*4 - 4) && leftNecromancer <= (3*4 - 4) )
						{
							if( wave[currentWave].necromancer[leftNecromancer].activate == 0 )
							{
								wave[currentWave].necromancer[leftNecromancer].activate = 1;
								passing_through_door( 'l', &wave[currentWave].necromancer[leftNecromancer] );
							}

							else if( wave[currentWave].necromancer[leftNecromancer].activate == 2 )
							{
								leftNecromancer += 4;
							}
						}

						else if( leftBeholder >= (1*4 - 4) && leftBeholder <= (2*4 - 4) )
						{
							if( wave[currentWave].beholder[leftBeholder].activate == 0 )
							{
								wave[currentWave].beholder[leftBeholder].activate = 1;
								passing_through_door( 'l', &wave[currentWave].beholder[leftBeholder] );
							}

							else if( wave[currentWave].beholder[leftBeholder].activate == 2 )
							{
								leftBeholder += 4;
							}
						}

						else if( leftJellykiller >= (6*4 - 4) && leftJellykiller <= (10*4 - 4) )
						{
							if( wave[currentWave].jellykiller[leftJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[leftJellykiller].activate = 1;
								passing_through_door( 'l', &wave[currentWave].jellykiller[leftJellykiller] );
							}
							else if( wave[currentWave].jellykiller[leftJellykiller].activate == 2 )
							{
								leftJellykiller += 4;
							}
						}

						else if( leftSkeleton >= (7*4 - 4)  && leftSkeleton <= (12*4 - 4) )
						{
							if( wave[currentWave].skeleton[leftSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[leftSkeleton].activate = 1;
								passing_through_door( 'l', &wave[currentWave].skeleton[leftSkeleton] );
							}

							else if( wave[currentWave].skeleton[leftSkeleton].activate == 2 )
							{
								leftSkeleton += 4;
							}
						}
						else
						{
							leftQuit = 1;
						}

						if( rightSkeleton >= (1*4 - 4)  && rightSkeleton <= (6*4 - 4) )
						{
							if( wave[currentWave].skeleton[rightSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[rightSkeleton].activate = 1;
								passing_through_door( 'r', &wave[currentWave].skeleton[rightSkeleton] );
							}

							else if( wave[currentWave].skeleton[rightSkeleton].activate == 2 )
							{
								rightSkeleton += 4;
							}
						}

						else if( rightJellykiller >= (1*4 - 4) && rightJellykiller <= (5*4 - 4) )
						{
							if( wave[currentWave].jellykiller[rightJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[rightJellykiller].activate = 1;
								passing_through_door( 'r', &wave[currentWave].jellykiller[rightJellykiller] );
							}
							else if( wave[currentWave].jellykiller[rightJellykiller].activate == 2 )
							{
								rightJellykiller += 4;
							}
						}

						else if( rightBeholder >= (1*4 - 4) && rightBeholder <= (2*4 - 4) )
						{
							if( wave[currentWave].beholder[rightBeholder].activate == 0 )
							{
								wave[currentWave].beholder[rightBeholder].activate = 1;
								passing_through_door( 'r', &wave[currentWave].beholder[rightBeholder] );
							}

							else if( wave[currentWave].beholder[rightBeholder].activate == 2 )
							{
								rightBeholder += 4;
							}
						}

						else if( rightNecromancer >= (1*4 - 4) && rightNecromancer <= (3*4 - 4) )
						{
							if( wave[currentWave].necromancer[rightNecromancer].activate == 0 )
							{
								wave[currentWave].necromancer[rightNecromancer].activate = 1;
								passing_through_door( 'r', &wave[currentWave].necromancer[rightNecromancer] );
							}

							else if( wave[currentWave].necromancer[rightNecromancer].activate == 2 )
							{
								rightNecromancer += 4;
							}
						}

						else if( rightJellykiller >= (6*4 - 4) && rightJellykiller <= (10*4 - 4) )
						{
							if( wave[currentWave].jellykiller[rightJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[rightJellykiller].activate = 1;
								passing_through_door( 'r', &wave[currentWave].jellykiller[rightJellykiller] );
							}
							else if( wave[currentWave].jellykiller[rightJellykiller].activate == 2 )
							{
								rightJellykiller += 4;
							}
						}

						else if( rightSkeleton >= (7*4 - 4)  && rightSkeleton <= (12*4 - 4) )
						{
							if( wave[currentWave].skeleton[rightSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[rightSkeleton].activate = 1;
								passing_through_door( 'r', &wave[currentWave].skeleton[rightSkeleton] );
							}

							else if( wave[currentWave].skeleton[rightSkeleton].activate == 2 )
							{
								rightSkeleton += 4;
							}
						}
						else
						{
							rightQuit = 1;
						}	

						if( upQuit == 1 && downQuit == 1 && leftQuit == 1 && rightQuit == 1 )
						{
							wave[currentWave].activate = 2;
						}
						break;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					case 8:
						if( upNecromancer >= (1*4 - 4) && upNecromancer <= (3*4 - 4) )
						{
							if( wave[currentWave].necromancer[upNecromancer].activate == 0 )
							{
								wave[currentWave].necromancer[upNecromancer].activate = 1;
								passing_through_door( 'u', &wave[currentWave].necromancer[upNecromancer] );
							}

							else if( wave[currentWave].necromancer[upNecromancer].activate == 2 )
							{
								upNecromancer += 4;
							}
						}

						else if( upJellykiller >= (1*4 - 4) && upJellykiller <= (10*4 - 4) )
						{
							if( wave[currentWave].jellykiller[upJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[upJellykiller].activate = 1;
								passing_through_door( 'u', &wave[currentWave].jellykiller[upJellykiller] );
							}
							else if( wave[currentWave].jellykiller[upJellykiller].activate == 2 )
							{
								upJellykiller += 4;
							}
						}

						else if( upSkeleton >= (1*4 - 4)  && upSkeleton <= (15*4 - 4) )
						{
							if( wave[currentWave].skeleton[upSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[upSkeleton].activate = 1;
								passing_through_door( 'u', &wave[currentWave].skeleton[upSkeleton] );
							}

							else if( wave[currentWave].skeleton[upSkeleton].activate == 2 )
							{
								upSkeleton += 4;
							}
						}

						else if( upJellykiller >= (11*4 - 4) && upJellykiller <= (20*4 - 4) )
						{
							if( wave[currentWave].jellykiller[upJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[upJellykiller].activate = 1;
								passing_through_door( 'u', &wave[currentWave].jellykiller[upJellykiller] );
							}
							else if( wave[currentWave].jellykiller[upJellykiller].activate == 2 )
							{
								upJellykiller += 4;
							}
						}

						else if( upBeholder >= (1*4 - 4) && upBeholder <= (2*4 - 4) )
						{
							if( wave[currentWave].beholder[upBeholder].activate == 0 )
							{
								wave[currentWave].beholder[upBeholder].activate = 1;
								passing_through_door( 'u', &wave[currentWave].beholder[upBeholder] );
							}

							else if( wave[currentWave].beholder[upBeholder].activate == 2 )
							{
								upBeholder += 4;
							}
						}

						else if( upNecromancer >= (4*4 - 4) && upNecromancer <= (6*4 - 4) )
						{
							if( wave[currentWave].necromancer[upNecromancer].activate == 0 )
							{
								wave[currentWave].necromancer[upNecromancer].activate = 1;
								passing_through_door( 'u', &wave[currentWave].necromancer[upNecromancer] );
							}

							else if( wave[currentWave].necromancer[upNecromancer].activate == 2 )
							{
								upNecromancer += 4;
							}
						}

						else if( upBeholder >= (3*4 - 4) && upBeholder <= (5*4 - 4) )
						{
							if( wave[currentWave].beholder[upBeholder].activate == 0 )
							{
								wave[currentWave].beholder[upBeholder].activate = 1;
								passing_through_door( 'u', &wave[currentWave].beholder[upBeholder] );
							}

							else if( wave[currentWave].beholder[upBeholder].activate == 2 )
							{
								upBeholder += 4;
							}
						}
						else
						{
							upQuit = 1;
						}

						if( downNecromancer >= (1*4 - 4) && downNecromancer <= (3*4 - 4) )
						{
							if( wave[currentWave].necromancer[downNecromancer].activate == 0 )
							{
								wave[currentWave].necromancer[downNecromancer].activate = 1;
								passing_through_door( 'd', &wave[currentWave].necromancer[downNecromancer] );
							}

							else if( wave[currentWave].necromancer[downNecromancer].activate == 2 )
							{
								downNecromancer += 4;
							}
						}

						else if( downJellykiller >= (1*4 - 4) && downJellykiller <= (10*4 - 4) )
						{
							if( wave[currentWave].jellykiller[downJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[downJellykiller].activate = 1;
								passing_through_door( 'd', &wave[currentWave].jellykiller[downJellykiller] );
							}
							else if( wave[currentWave].jellykiller[downJellykiller].activate == 2 )
							{
								downJellykiller += 4;
							}
						}


						else if( downBeholder >= (1*4 - 4) && downBeholder <= (2*4 - 4) )
						{
							if( wave[currentWave].beholder[downBeholder].activate == 0 )
							{
								wave[currentWave].beholder[downBeholder].activate = 1;
								passing_through_door( 'd', &wave[currentWave].beholder[downBeholder] );
							}

							else if( wave[currentWave].beholder[downBeholder].activate == 2 )
							{
								downBeholder += 4;
							}
						}

						else if( downJellykiller >= (11*4 - 4) && downJellykiller <= (20*4 - 4) )
						{
							if( wave[currentWave].jellykiller[downJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[downJellykiller].activate = 1;
								passing_through_door( 'd', &wave[currentWave].jellykiller[downJellykiller] );
							}
							else if( wave[currentWave].jellykiller[downJellykiller].activate == 2 )
							{
								downJellykiller += 4;
							}
						}

						else if( downSkeleton >= (1*4 - 4)  && downSkeleton <= (15*4 - 4) )
						{
							if( wave[currentWave].skeleton[downSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[downSkeleton].activate = 1;
								passing_through_door( 'd', &wave[currentWave].skeleton[downSkeleton] );
							}

							else if( wave[currentWave].skeleton[downSkeleton].activate == 2 )
							{
								downSkeleton += 4;
							}
						}

						else if( downBeholder >= (3*4 - 4) && downBeholder <= (5*4 - 4) )
						{
							if( wave[currentWave].beholder[downBeholder].activate == 0 )
							{
								wave[currentWave].beholder[downBeholder].activate = 1;
								passing_through_door( 'd', &wave[currentWave].beholder[downBeholder] );
							}

							else if( wave[currentWave].beholder[downBeholder].activate == 2 )
							{
								downBeholder += 4;
							}
						}

						else if( downNecromancer >= (4*4 - 4) && downNecromancer <= (6*4 - 4) )
						{
							if( wave[currentWave].necromancer[downNecromancer].activate == 0 )
							{
								wave[currentWave].necromancer[downNecromancer].activate = 1;
								passing_through_door( 'd', &wave[currentWave].necromancer[downNecromancer] );
							}

							else if( wave[currentWave].necromancer[downNecromancer].activate == 2 )
							{
								downNecromancer += 4;
							}
						}
						else
						{
							downQuit = 1;
						}

						if( leftNecromancer >= (1*4 - 4) && leftNecromancer <= (3*4 - 4) )
						{
							if( wave[currentWave].necromancer[leftNecromancer].activate == 0 )
							{
								wave[currentWave].necromancer[leftNecromancer].activate = 1;
								passing_through_door( 'l', &wave[currentWave].necromancer[leftNecromancer] );
							}

							else if( wave[currentWave].necromancer[leftNecromancer].activate == 2 )
							{
								leftNecromancer += 4;
							}
						}

						else if( leftJellykiller >= (1*4 - 4) && leftJellykiller <= (10*4 - 4) )
						{
							if( wave[currentWave].jellykiller[leftJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[leftJellykiller].activate = 1;
								passing_through_door( 'l', &wave[currentWave].jellykiller[leftJellykiller] );
							}
							else if( wave[currentWave].jellykiller[leftJellykiller].activate == 2 )
							{
								leftJellykiller += 4;
							}
						}

						else if( leftSkeleton >= (1*4 - 4)  && leftSkeleton <= (15*4 - 4) )
						{
							if( wave[currentWave].skeleton[leftSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[leftSkeleton].activate = 1;
								passing_through_door( 'l', &wave[currentWave].skeleton[leftSkeleton] );
							}

							else if( wave[currentWave].skeleton[leftSkeleton].activate == 2 )
							{
								leftSkeleton += 4;
							}
						}

						else if( leftJellykiller >= (11*4 - 4) && leftJellykiller <= (20*4 - 4) )
						{
							if( wave[currentWave].jellykiller[leftJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[leftJellykiller].activate = 1;
								passing_through_door( 'l', &wave[currentWave].jellykiller[leftJellykiller] );
							}
							else if( wave[currentWave].jellykiller[leftJellykiller].activate == 2 )
							{
								leftJellykiller += 4;
							}
						}

						else if( leftBeholder >= (1*4 - 4) && leftBeholder <= (2*4 - 4) )
						{
							if( wave[currentWave].beholder[leftBeholder].activate == 0 )
							{
								wave[currentWave].beholder[leftBeholder].activate = 1;
								passing_through_door( 'l', &wave[currentWave].beholder[leftBeholder] );
							}

							else if( wave[currentWave].beholder[leftBeholder].activate == 2 )
							{
								leftBeholder += 4;
							}
						}

						else if( leftNecromancer >= (4*4 - 4) && leftNecromancer <= (6*4 - 4) )
						{
							if( wave[currentWave].necromancer[leftNecromancer].activate == 0 )
							{
								wave[currentWave].necromancer[leftNecromancer].activate = 1;
								passing_through_door( 'l', &wave[currentWave].necromancer[leftNecromancer] );
							}

							else if( wave[currentWave].necromancer[leftNecromancer].activate == 2 )
							{
								leftNecromancer += 4;
							}
						}

						else if( leftBeholder >= (3*4 - 4) && leftBeholder <= (5*4 - 4) )
						{
							if( wave[currentWave].beholder[leftBeholder].activate == 0 )
							{
								wave[currentWave].beholder[leftBeholder].activate = 1;
								passing_through_door( 'l', &wave[currentWave].beholder[leftBeholder] );
							}

							else if( wave[currentWave].beholder[leftBeholder].activate == 2 )
							{
								leftBeholder += 4;
							}
						}
						else
						{
							leftQuit = 1;
						}

						if( rightNecromancer >= (1*4 - 4) && rightNecromancer <= (3*4 - 4) )
						{
							if( wave[currentWave].necromancer[rightNecromancer].activate == 0 )
							{
								wave[currentWave].necromancer[rightNecromancer].activate = 1;
								passing_through_door( 'r', &wave[currentWave].necromancer[rightNecromancer] );
							}

							else if( wave[currentWave].necromancer[rightNecromancer].activate == 2 )
							{
								rightNecromancer += 4;
							}
						}

						else if( rightJellykiller >= (1*4 - 4) && rightJellykiller <= (10*4 - 4) )
						{
							if( wave[currentWave].jellykiller[rightJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[rightJellykiller].activate = 1;
								passing_through_door( 'r', &wave[currentWave].jellykiller[rightJellykiller] );
							}
							else if( wave[currentWave].jellykiller[rightJellykiller].activate == 2 )
							{
								rightJellykiller += 4;
							}
						}


						else if( rightBeholder >= (1*4 - 4) && rightBeholder <= (2*4 - 4) )
						{
							if( wave[currentWave].beholder[rightBeholder].activate == 0 )
							{
								wave[currentWave].beholder[rightBeholder].activate = 1;
								passing_through_door( 'r', &wave[currentWave].beholder[rightBeholder] );
							}

							else if( wave[currentWave].beholder[rightBeholder].activate == 2 )
							{
								rightBeholder += 4;
							}
						}

						else if( rightJellykiller >= (11*4 - 4) && rightJellykiller <= (20*4 - 4) )
						{
							if( wave[currentWave].jellykiller[rightJellykiller].activate == 0 )
							{
								wave[currentWave].jellykiller[rightJellykiller].activate = 1;
								passing_through_door( 'r', &wave[currentWave].jellykiller[rightJellykiller] );
							}
							else if( wave[currentWave].jellykiller[rightJellykiller].activate == 2 )
							{
								rightJellykiller += 4;
							}
						}

						else if( rightSkeleton >= (1*4 - 4)  && rightSkeleton <= (15*4 - 4) )
						{
							if( wave[currentWave].skeleton[rightSkeleton].activate == 0 )
							{
								wave[currentWave].skeleton[rightSkeleton].activate = 1;
								passing_through_door( 'r', &wave[currentWave].skeleton[rightSkeleton] );
							}

							else if( wave[currentWave].skeleton[rightSkeleton].activate == 2 )
							{
								rightSkeleton += 4;
							}
						}

						else if( rightBeholder >= (3*4 - 4) && rightBeholder <= (5*4 - 4) )
						{
							if( wave[currentWave].beholder[rightBeholder].activate == 0 )
							{
								wave[currentWave].beholder[rightBeholder].activate = 1;
								passing_through_door( 'r', &wave[currentWave].beholder[rightBeholder] );
							}

							else if( wave[currentWave].beholder[rightBeholder].activate == 2 )
							{
								rightBeholder += 4;
							}
						}

						else if( rightNecromancer >= (4*4 - 4) && rightNecromancer <= (6*4 - 4) )
						{
							if( wave[currentWave].necromancer[rightNecromancer].activate == 0 )
							{
								wave[currentWave].necromancer[rightNecromancer].activate = 1;
								passing_through_door( 'r', &wave[currentWave].necromancer[rightNecromancer] );
							}

							else if( wave[currentWave].necromancer[rightNecromancer].activate == 2 )
							{
								rightNecromancer += 4;
							}
						}
						else
						{
							rightQuit = 1;
						}	

						if( upQuit == 1 && downQuit == 1 && leftQuit == 1 && rightQuit == 1 )						
						{
							wave[currentWave].activate = 2;
						}
						break;
				}
			}	

			// Se a wave foi derrotada, pode sair da sala
			else if( wave[currentWave].activate == 0 || wave[currentWave].activate == 2 )
			{
				// Permite passar pelas portas
				if( player.presentedRect.x <= 19 )
				{
					if( (player.presentedRect.y < 259) || (player.presentedRect.y+player.presentedRect.h > 340) )
					{
						player.presentedRect.x = 20;
					}
					else if ( (firstMap.snipRect.x/800 == 1 && firstMap.snipRect.y/600 == 0) 
							|| (firstMap.snipRect.x/800 == 2 && firstMap.snipRect.y/600 == 0)
							|| (firstMap.snipRect.x/800 == 3 && firstMap.snipRect.y/600 == 0)
							|| (firstMap.snipRect.x/800 == 1 && firstMap.snipRect.y/600 == 1)
							|| (firstMap.snipRect.x/800 == 2 && firstMap.snipRect.y/600 == 1)
							|| (firstMap.snipRect.x/800 == 3 && firstMap.snipRect.y/600 == 1)
							|| (firstMap.snipRect.x/800 == 4 && firstMap.snipRect.y/600 == 1)
							|| (firstMap.snipRect.x/800 == 3 && firstMap.snipRect.y/600 == 2)
							|| (firstMap.snipRect.x/800 == 5 && firstMap.snipRect.y/600 == 2) )
					{
						for( a = 0 ; a < 160 ; a++ )
						{
							SDL_RenderClear( l_renderer );
							scroll_map_animation( 'l', &firstMap, &player );
							SDL_RenderCopy( l_renderer, firstMap.texture, &firstMap.snipRect, &firstMap.presentedRect );
							SDL_RenderCopy( l_renderer, player.sprite, &player.snipRect, &player.presentedRect );
							SDL_RenderPresent( l_renderer );
							SDL_Delay( 1000/fps );
						}
					}
				}
				if( player.presentedRect.x+player.presentedRect.w >= 780 )
				{
					if( (player.presentedRect.y < 259) || (player.presentedRect.y+player.presentedRect.h > 340) )
					{
						player.presentedRect.x = 779 - player.presentedRect.w;
					}
					else if( (firstMap.snipRect.x/800 == 0 && firstMap.snipRect.y/600 == 0) 
							|| (firstMap.snipRect.x/800 == 1 && firstMap.snipRect.y/600 == 0)
							|| (firstMap.snipRect.x/800 == 2 && firstMap.snipRect.y/600 == 0)
							|| (firstMap.snipRect.x/800 == 0 && firstMap.snipRect.y/600 == 1)
							|| (firstMap.snipRect.x/800 == 1 && firstMap.snipRect.y/600 == 1)
							|| (firstMap.snipRect.x/800 == 2 && firstMap.snipRect.y/600 == 1)
							|| (firstMap.snipRect.x/800 == 3 && firstMap.snipRect.y/600 == 1)
							|| (firstMap.snipRect.x/800 == 2 && firstMap.snipRect.y/600 == 2)
							|| (firstMap.snipRect.x/800 == 4 && firstMap.snipRect.y/600 == 2) )
					{
						for( a = 0 ; a < 160 ; a++ )
						{
							SDL_RenderClear( l_renderer );
							scroll_map_animation( 'r', &firstMap, &player );
							SDL_RenderCopy( l_renderer, firstMap.texture, &firstMap.snipRect, &firstMap.presentedRect );
							SDL_RenderCopy( l_renderer, player.sprite, &player.snipRect, &player.presentedRect );
							SDL_RenderPresent( l_renderer );
							SDL_Delay( 1000/fps );
						}
					}
				}
				if( player.presentedRect.y <= 129 )
				{
					if( (player.presentedRect.x < 359) || (player.presentedRect.x+player.presentedRect.w > 440) )
					{
						player.presentedRect.y = 130;
					}
					else if( (firstMap.snipRect.x/800 == 0 && firstMap.snipRect.y/600 == 1) 
							|| (firstMap.snipRect.x/800 == 1 && firstMap.snipRect.y/600 == 1)
							|| (firstMap.snipRect.x/800 == 2 && firstMap.snipRect.y/600 == 1)
							|| (firstMap.snipRect.x/800 == 4 && firstMap.snipRect.y/600 == 1)
							|| (firstMap.snipRect.x/800 == 5 && firstMap.snipRect.y/600 == 1)
							|| (firstMap.snipRect.x/800 == 0 && firstMap.snipRect.y/600 == 2)
							|| (firstMap.snipRect.x/800 == 2 && firstMap.snipRect.y/600 == 2)
							|| (firstMap.snipRect.x/800 == 4 && firstMap.snipRect.y/600 == 2)
							|| (firstMap.snipRect.x/800 == 5 && firstMap.snipRect.y/600 == 2) )
					{
						for( a = 0 ; a < 120 ; a++ )
						{
							SDL_RenderClear( l_renderer );
							scroll_map_animation( 'u', &firstMap, &player );
							SDL_RenderCopy( l_renderer, firstMap.texture, &firstMap.snipRect, &firstMap.presentedRect );
							SDL_RenderCopy( l_renderer, player.sprite, &player.snipRect, &player.presentedRect );
							SDL_RenderPresent( l_renderer );
							SDL_Delay( 1000/fps );
						}
					}
				}
				if( player.presentedRect.y+player.presentedRect.h >= 580 )
				{
					if( (player.presentedRect.x < 359) || (player.presentedRect.x+player.presentedRect.w > 440) )
					{
						player.presentedRect.y = 579 - player.presentedRect.h;
					}
					else if( (firstMap.snipRect.x/800 == 0 && firstMap.snipRect.y/600 == 0) 
							|| (firstMap.snipRect.x/800 == 1 && firstMap.snipRect.y/600 == 0)
							|| (firstMap.snipRect.x/800 == 2 && firstMap.snipRect.y/600 == 0)
							|| (firstMap.snipRect.x/800 == 4 && firstMap.snipRect.y/600 == 0)
							|| (firstMap.snipRect.x/800 == 5 && firstMap.snipRect.y/600 == 0)
							|| (firstMap.snipRect.x/800 == 0 && firstMap.snipRect.y/600 == 1)
							|| (firstMap.snipRect.x/800 == 2 && firstMap.snipRect.y/600 == 1)
							|| (firstMap.snipRect.x/800 == 4 && firstMap.snipRect.y/600 == 1)
							|| (firstMap.snipRect.x/800 == 5 && firstMap.snipRect.y/600 == 1) )
					{
						for( a = 0 ; a < 120 ; a++ )
						{
							SDL_RenderClear( l_renderer );
							scroll_map_animation( 'd', &firstMap, &player );
							SDL_RenderCopy( l_renderer, firstMap.texture, &firstMap.snipRect, &firstMap.presentedRect );
							SDL_RenderCopy( l_renderer, player.sprite, &player.snipRect, &player.presentedRect );
							SDL_RenderPresent( l_renderer );
							SDL_Delay( 1000/fps );
						}
					}
				}
			}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////RESTANTE DO GAMEPLAY ////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			SDL_RenderCopy( l_renderer, player.sprite, &player.snipRect, &player.presentedRect );

			// Sobre a barra de vida
			healthBarRect.w = 2*player.healthPoints;

			if( healthBarRect.w >= 160 )
			{
				SDL_SetRenderDrawColor( l_renderer, 0, 255, 0, 255 );
			}

			else if( healthBarRect.w < 120 && healthBarRect.w >= 60 )
			{
				SDL_SetRenderDrawColor( l_renderer, 255, 255, 0, 255 );
			}

			else if( healthBarRect.w < 60 )
			{
				SDL_SetRenderDrawColor( l_renderer, 255, 0, 0, 255 );
			}


			  
			SDL_RenderFillRect( l_renderer, &healthBarRect );
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
				deathScreen = 1;
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

		while( pauseScreen == 1)
		{
			SDL_RenderCopy( l_renderer, pauseImage, NULL, NULL );
			SDL_RenderPresent( l_renderer );
			
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
					if( e.key.keysym.sym == SDLK_RETURN )
					{
						if( pressedKeyStates[SDL_SCANCODE_RETURN] )
						{
							pauseScreen = 0;
						}
					}
				}
			}
		}

		while( deathScreen == 1)
		{
			SDL_RenderCopy( l_renderer, failureImage, NULL, NULL );
			SDL_RenderPresent( l_renderer );

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
					if( e.key.keysym.sym == SDLK_RETURN )
					{
						if( pressedKeyStates[SDL_SCANCODE_RETURN] )
						{
							deathScreen = 0;
							l_gameState = 2;
						}
					}
				}
			}
		}

		while( victoryScreen == 1)
		{
			SDL_RenderCopy( l_renderer, victoryImage, NULL, NULL );
			SDL_RenderPresent( l_renderer );

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
					if( e.key.keysym.sym == SDLK_RETURN )
					{
						if( pressedKeyStates[SDL_SCANCODE_RETURN] )
						{
							victoryScreen = 0;
							l_gameState = 2;
						}
					}
				}
			}
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////SE RE TIRANDO DO GAMEPLAY ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Libera imagens carregadas
	SDL_DestroyTexture( victoryImage );
	SDL_DestroyTexture( failureImage );
	SDL_DestroyTexture( readyImage );
	SDL_DestroyTexture( arcaneLogo );
	SDL_DestroyTexture( fireLogo );
	SDL_DestroyTexture( iceLogo );
	SDL_DestroyTexture( player.sprite );
	SDL_DestroyTexture( player.spriteDeath );
	SDL_DestroyTexture( firstMap.texture );
	SDL_DestroyTexture( skeleton.sprite );
	SDL_DestroyTexture( skeleton.spriteDeath );
	SDL_DestroyTexture( jellykiller.sprite );
	SDL_DestroyTexture( jellykiller.spriteDeath );
	SDL_DestroyTexture( beholder.sprite );
	SDL_DestroyTexture( beholder.spriteDeath );
	SDL_DestroyTexture( necromancer.sprite );
	SDL_DestroyTexture( necromancer.spriteDeath );
	SDL_DestroyTexture( fireBall.sprite );
	SDL_DestroyTexture( iceSpear.sprite );
	SDL_DestroyTexture( arcaneMissile.sprite );
	SDL_DestroyTexture( necroShot.sprite );
	SDL_DestroyTexture( beholderBullet.sprite );
	SDL_DestroyTexture( greenCrystal.sprite);
	victoryImage = NULL;
	failureImage = NULL;
	readyImage = NULL;
	arcaneLogo = NULL;
	fireLogo = NULL;
	iceLogo = NULL;
	player.sprite = NULL;
	player.spriteDeath = NULL;
	firstMap.texture = NULL;
	skeleton.sprite = NULL;
	skeleton.spriteDeath = NULL;
	jellykiller.sprite = NULL;
	jellykiller.spriteDeath = NULL;
	beholder.sprite = NULL;
	beholder.spriteDeath = NULL;
	necromancer.sprite = NULL;
	necromancer.spriteDeath = NULL;
	fireBall.sprite = NULL;
	iceSpear.sprite = NULL;
	arcaneMissile.sprite = NULL;
	necroShot.sprite = NULL;
	beholderBullet.sprite = NULL;
	greenCrystal.sprite = NULL;

	// Libera sons carregados
	Mix_FreeChunk( player.gotHit );
	Mix_FreeChunk( player.deathSound );
	Mix_FreeChunk( skeleton.gotHit );
	Mix_FreeChunk( skeleton.deathSound );
	Mix_FreeChunk( jellykiller.gotHit );
	Mix_FreeChunk( jellykiller.deathSound );
	Mix_FreeChunk( beholder.gotHit );
	Mix_FreeChunk( beholder.deathSound );
	Mix_FreeChunk( necromancer.gotHit );
	Mix_FreeChunk( necromancer.deathSound );
	Mix_FreeChunk( fireBall.hitWall );
	Mix_FreeChunk( iceSpear.hitWall );
	Mix_FreeChunk( arcaneMissile.hitWall );
	player.gotHit = NULL;
	player.deathSound = NULL;
	skeleton.gotHit = NULL;
	skeleton.deathSound = NULL;
	jellykiller.gotHit = NULL;
	jellykiller.deathSound = NULL;
	beholder.gotHit = NULL;
	beholder.deathSound = NULL;
	necromancer.gotHit = NULL;
	necromancer.deathSound = NULL;
	fireBall.hitWall = NULL;
	iceSpear.hitWall = NULL;
	arcaneMissile.hitWall = NULL;
	
	// Limpa o renderizador
	SDL_RenderClear( l_renderer );	
	
	return l_gameState;
}

// FFFFFFFFFFFFFFFFFFFFFFFFFFUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUNCOES ESTAO AKI!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// FFFFFFFFFFFFFFFFFFFFFFFFFFUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUNCOES ESTAO AKI!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// FFFFFFFFFFFFFFFFFFFFFFFFFFUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUNCOES ESTAO AKI!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

void room_limits( Character* character )
{	
	if( character->presentedRect.x <= 19 )
	{
		character->presentedRect.x = 20;
	}
	else if( character->presentedRect.x+character->presentedRect.w >= 780 )
	{
		character->presentedRect.x = 779 - character->presentedRect.w;
	}
	if( character->presentedRect.y <= 129 )
	{
		character->presentedRect.y = 130;
	}
	else if( character->presentedRect.y+character->presentedRect.h >= 580 )
	{
		character->presentedRect.y = 579 - character->presentedRect.h;
	}
}

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
	int occur, occur2;
	// Raio do sprite
	float aRadius = (float)enemyPR.w/2.0 - 10;
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
			occur2 = 1;
		}

		else if( playerPR->y > enemyPR.y )
		{
			playerPR->y++;
			occur2 = 2;
		}
	}

	switch( occur )
	{
		case 1:
			playerPR->x -= 25;
			break;
		case 2:
			playerPR->x += 25;
			break;
	}

	switch( occur2 )
	{
		case 1:
			playerPR->y -= 25;
			break;
		case 2:
			playerPR->y += 25;
			break;
	}
}

void scroll_map_animation( char c, Map* map, Character* player )
{
	switch( c )
	{
		case 'u':
			map->snipRect.y -= 5;
			player->presentedRect.y += 3;
			break;

		case 'd':
			map->snipRect.y += 5;
			player->presentedRect.y -= 3;
			break;

		case 'l':
			map->snipRect.x -= 5;
			player->presentedRect.x += 3;
			break;

		case 'r':
			map->snipRect.x += 5;
			player->presentedRect.x -= 3;
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

void projectile_update( MagicProjectile* bullet, MagicProjectile typeOfBullet, int sCounter, int sDeath, SDL_Window* l_window, SDL_Renderer* l_renderer ) // 6 e 4 para a fireBall
{
	if( bullet->activate == 0 )
	{
		bullet->presentedRect.x = 900;
		bullet->presentedRect.y = 900;  
		bullet->animationCycleCounter = 0;
		SDL_RenderCopy( l_renderer, typeOfBullet.sprite, &bullet->snipRect, &bullet->presentedRect );
	}

	else if( bullet->activate == 1 )
	{
		switch( bullet->direction )
		{
			case 'u':
				bullet->presentedRect.y -= typeOfBullet.speed;

				if( bullet->animationCycleCounter%sCounter == 0 )
				{
					bullet->snipRect.x += bullet->snipRect.w;
					if( bullet->snipRect.x < (15*bullet->snipRect.w) || bullet->snipRect.x > (24*bullet->snipRect.w) )
					{
						bullet->snipRect.x = (15*bullet->snipRect.w);
					}
				}

				bullet->animationCycleCounter++;
				break;

			case 'd':
				bullet->presentedRect.y += typeOfBullet.speed;
				if( bullet->animationCycleCounter%sCounter == 0 )
				{
					bullet->snipRect.x += bullet->snipRect.w;
					if( bullet->snipRect.x < (45*bullet->snipRect.w) || bullet->snipRect.x > (54*bullet->snipRect.w) )
					{
						bullet->snipRect.x = (45*bullet->snipRect.w);
					}
				}
				bullet->animationCycleCounter++;
				break;

			case 'l':
				bullet->presentedRect.x -= typeOfBullet.speed;
				if( bullet->animationCycleCounter%sCounter == 0 )
				{
					bullet->snipRect.x += bullet->snipRect.w;
					if( bullet->snipRect.x < (30*bullet->snipRect.w) || bullet->snipRect.x > (39*bullet->snipRect.w) )
					{
						bullet->snipRect.x = (30*bullet->snipRect.w);
					}
				}
				bullet->animationCycleCounter++;
				break;

			case 'r':
				bullet->presentedRect.x += typeOfBullet.speed;
				if( bullet->animationCycleCounter%sCounter == 0 )
				{
					bullet->snipRect.x += bullet->snipRect.w;
					if( bullet->snipRect.x < 0 || bullet->snipRect.x > (9*bullet->snipRect.w) )
					{
						bullet->snipRect.x = 0;
					}
				}
				bullet->animationCycleCounter++;
				break;
		}

		if( bullet->presentedRect.y < 130 || bullet->presentedRect.x < 18 || (bullet->presentedRect.x+bullet->presentedRect.w) > 780 || (bullet->presentedRect.y+bullet->presentedRect.h) > 580 )
		{
			if( bullet->direction == 'u' )
			{
				bullet->snipRect.x = (24*bullet->snipRect.w);
			}
			else if( bullet->direction == 'd' )
			{
				bullet->snipRect.x = (54*bullet->snipRect.w);
			}
			else if( bullet->direction == 'l' )
			{
				bullet->snipRect.x = (39*bullet->snipRect.w);
			}
			else if( bullet->direction == 'r' )
			{
				bullet->snipRect.x = (9*bullet->snipRect.w);
			}
			bullet->animationCycleCounter = 0;
			bullet->activate = 2;
			Mix_PlayChannel( -1, typeOfBullet.hitWall, 0 );
		}
	}
			
	else if( bullet->activate == 2 )
	{
		if( bullet->direction == 'u' )
		{
			bullet->animationCycleCounter++;
			if( bullet->animationCycleCounter%sDeath == 0 )
			{
				bullet->snipRect.x += bullet->snipRect.w;
				if( bullet->snipRect.x > 1160 )
				{
					bullet->presentedRect.x = 900;
					bullet->presentedRect.y = 900;  
					bullet->animationCycleCounter = 0;
				}
			}
		}
		else if( bullet->direction == 'd' )
		{
			bullet->animationCycleCounter++;
			if( bullet->animationCycleCounter%sDeath == 0 )
			{
				bullet->snipRect.x += bullet->snipRect.w;
				if( bullet->snipRect.x > 2360 )
				{
					bullet->presentedRect.x = 900;
					bullet->presentedRect.y = 900;  
					bullet->animationCycleCounter = 0;
				}
			}
		}

		else if( bullet->direction == 'l' )
		{
			bullet->animationCycleCounter++;
			if( bullet->animationCycleCounter%sDeath == 0 )
			{
				bullet->snipRect.x += bullet->snipRect.w;
				if( bullet->snipRect.x > 1760 )
				{
					bullet->presentedRect.x = 900;
					bullet->presentedRect.y = 900;  
					bullet->animationCycleCounter = 0;
				}
			}
		}

		else if( bullet->direction == 'r' )
		{
			bullet->animationCycleCounter++;
			if( bullet->animationCycleCounter%sDeath == 0 )
			{
				bullet->snipRect.x += bullet->snipRect.w;
				if( bullet->snipRect.x > 560 )
				{
					bullet->presentedRect.x = 900;
					bullet->presentedRect.y = 900;  
					bullet->animationCycleCounter = 0;
				}
			}
		}	
	}
	SDL_RenderCopy( l_renderer, typeOfBullet.sprite, &bullet->snipRect, &bullet->presentedRect );
}

void linear_walk_and_shoot( Character* enemy, Character typeOfEnemy, Character player, MagicProjectile* projectile )
{
	// Andar horizontal 
	if( enemy->direction == 'u' || enemy->direction == 'd' ) 
	{
		// Vai atirar
		if( (enemy->presentedRect.x+enemy->presentedRect.w/2) >= (player.presentedRect.x+25) && (enemy->presentedRect.x+enemy->presentedRect.w/2) <= (player.presentedRect.x+41) )
		{
			// Saída norte
			if( enemy->direction == 'u' )
			{
				enemy->snipRect.x = 180;

				if( enemy->shootDelayCounter%55 == 0 )
				{
					projectile->presentedRect.x = (enemy->presentedRect.x+enemy->presentedRect.w/2-projectile->presentedRect.w/2);
					projectile->presentedRect.y = (enemy->presentedRect.y+enemy->presentedRect.h/2-5); 
					projectile->direction = 'd';
					projectile->activate = 1;
					projectile->animationCycleCounter = 0;

					enemy->bulletNumber++;

					if( enemy->bulletNumber > 19 )
					{
						enemy->bulletNumber = 0;
					}

					enemy->shootDelayCounter = 1;
				}
			}
			// Saída sul
			else if( enemy->direction == 'd' )
			{
				enemy->snipRect.x = 810;
				if( enemy->shootDelayCounter%55 == 0 )
				{
					projectile->presentedRect.x = (enemy->presentedRect.x+enemy->presentedRect.w/2-projectile->presentedRect.w/2);
					projectile->presentedRect.y = (enemy->presentedRect.y); 
					projectile->direction = 'u';
					projectile->activate = 1;
					projectile->animationCycleCounter = 0;
					enemy->bulletNumber++;
					if( enemy->bulletNumber > 19 )
					{
						enemy->bulletNumber = 0;
					}
					enemy->shootDelayCounter = 1;	
				}
			}	
			enemy->animationCycleCounter = 0;
		}

		else
		{
			if( enemy->way == 1 ) // Esquerda
			{
				if( enemy->presentedRect.x > 21 )
				{
					enemy->presentedRect.x -= typeOfEnemy.speed;
				}
				else
				{
					enemy->way = 2;

					enemy->animationCycleCounter = 0;
				}

				if( enemy->animationCycleCounter%18 == 0 )
				{
					enemy->snipRect.x += 90;
				}

				if( enemy->snipRect.x < 270 || enemy->snipRect.x > 360 )
				{
					enemy->snipRect.x = 270;
				}
			}
			else if( enemy->way == 2 ) // Direta
			{
				if( enemy->presentedRect.x+enemy->presentedRect.w < 780 )
				{
					enemy->presentedRect.x += typeOfEnemy.speed;
				}
				else
				{
					enemy->way = 1;
					enemy->animationCycleCounter = 0;
				}
				if( enemy->animationCycleCounter%18 == 0 )
				{
					enemy->snipRect.x += 90;
				}
				if( enemy->snipRect.x < 540 || enemy->snipRect.x > 630 )
				{
					enemy->snipRect.x = 540;
				}
			}
			enemy->animationCycleCounter++;
		}
	}
	// Andar vertical
	else if( enemy->direction == 'l' || enemy->direction == 'r' )
	{
		if( (enemy->presentedRect.y+enemy->presentedRect.h/2) >= (player.presentedRect.y+25) && (enemy->presentedRect.y+enemy->presentedRect.h/2) <= (player.presentedRect.y+41) )
		{
			// Saída oeste
			if( enemy->direction == 'l' )
			{
				enemy->snipRect.x = 720;
				if( enemy->shootDelayCounter%55 == 0 )
				{
					projectile->presentedRect.x = (enemy->presentedRect.x+enemy->presentedRect.w/2-10);
					projectile->presentedRect.y = (enemy->presentedRect.y+enemy->presentedRect.h/2-projectile->presentedRect.h/2); 
					projectile->direction = 'r';
					projectile->activate = 1;
					projectile->animationCycleCounter = 0;
					enemy->bulletNumber++;
					if( enemy->bulletNumber > 19 )
					{
						enemy->bulletNumber = 0;
					}
					enemy->shootDelayCounter = 1;
				}
			}
			// Saída leste
			else if( enemy->direction == 'r' )
			{
				enemy->snipRect.x = 450;
				if( enemy->shootDelayCounter%55 == 0 )
				{
					projectile->presentedRect.x = (enemy->presentedRect.x);
					projectile->presentedRect.y = (enemy->presentedRect.y+enemy->presentedRect.h/2-projectile->presentedRect.h/2); 
					projectile->direction = 'l';
					projectile->activate = 1;
					projectile->animationCycleCounter = 0;
					enemy->bulletNumber++;
					if( enemy->bulletNumber > 19 )
					{
						enemy->bulletNumber = 0;
					}
					enemy->shootDelayCounter = 1;
				}
			}
			enemy->animationCycleCounter = 0;
		}
		else
		{
			if( enemy->way == 1 ) // Cima
			{
				if( enemy->presentedRect.y > 131 )
				{
					enemy->presentedRect.y -= typeOfEnemy.speed;
				}
				else
				{
					enemy->way = 2;
					enemy->animationCycleCounter = 0;
				}
				if( enemy->animationCycleCounter%18 == 0 )
				{
					enemy->snipRect.x += 90;
				}
				if( enemy->snipRect.x < 810 || enemy->snipRect.x > 900 )
				{
					enemy->snipRect.x = 810;
				}
			}
			else if( enemy->way == 2 ) // Baixa
			{
				if( enemy->presentedRect.y+enemy->presentedRect.h < 580 )
				{
					enemy->presentedRect.y += typeOfEnemy.speed;
				}
				else
				{
					enemy->way = 1;
					enemy->animationCycleCounter = 0;
				}
				if( enemy->animationCycleCounter%18 == 0 )
				{
					enemy->snipRect.x += 90;
				}
				if( enemy->snipRect.x < 0 || enemy->snipRect.x > 90 )
				{
					enemy->snipRect.x = 0;
				}	
			}
			enemy->animationCycleCounter++;	
		}
	}

	if( enemy->shootDelayCounter%55 != 0 )
	{
		enemy->shootDelayCounter++;
	} 
}

void necromancer_ia( Character* enemy, Character typeOfEnemy, Character player, MagicProjectile* projectile, MagicProjectile* projectile2 )
{
	int l_up = 0, l_down = 1, l_left = 2, l_right = 3; 

	// Vai atirar
	if( (enemy->presentedRect.x+enemy->presentedRect.w/2) >= (player.presentedRect.x+25) && (enemy->presentedRect.x+enemy->presentedRect.w/2) <= (player.presentedRect.x+41) )
	{
		if( enemy->shootDelayCounter%55 == 0 )
		{
			if( enemy->way == l_left )
			{
				enemy->snipRect.x = 425; // "Atirando pra esquerda"
			}

			else if( enemy->way == l_right )
			{
				enemy->snipRect.x = 680; // "Atirando pra direita"
			}

			projectile->presentedRect.x = (enemy->presentedRect.x+enemy->presentedRect.w/2-projectile->presentedRect.w/2);
			projectile->presentedRect.y = (enemy->presentedRect.y+enemy->presentedRect.h/2-5); 
			projectile->direction = 'd';
			projectile->activate = 1;
			projectile->animationCycleCounter = 0;
		
			projectile2->presentedRect.x = (enemy->presentedRect.x+enemy->presentedRect.w/2-projectile->presentedRect.w/2);
			projectile2->presentedRect.y = (enemy->presentedRect.y); 
			projectile2->direction = 'u';
			projectile2->activate = 1;
			projectile2->animationCycleCounter = 0;

			enemy->bulletNumber += 2;

			if( enemy->bulletNumber > 29 )
			{
				enemy->bulletNumber = 0;
			}

			enemy->shootDelayCounter = 1;	
		}

		enemy->animationCycleCounter = 0;

		enemy->way = rand() % 4;
	}	
	
	// Andar vertical
	else if( (enemy->presentedRect.y+enemy->presentedRect.h/2) >= (player.presentedRect.y+25) && (enemy->presentedRect.y+enemy->presentedRect.h/2) <= (player.presentedRect.y+41) )
	{
		if( enemy->shootDelayCounter%55 == 0 )
		{
			if( enemy->way == l_up )
			{
				enemy->snipRect.x = 935; // "Atirando pra cima"
			}
			else if( enemy->way == l_down )
			{
				enemy->snipRect.x = 170; // "Atirando pra baixo"
			}
			projectile->presentedRect.x = (enemy->presentedRect.x+enemy->presentedRect.w/2-10);
			projectile->presentedRect.y = (enemy->presentedRect.y+enemy->presentedRect.h/2-projectile->presentedRect.h/2); 
			projectile->direction = 'r';
			projectile->activate = 1;
			projectile->animationCycleCounter = 0;
			
			projectile2->presentedRect.x = (enemy->presentedRect.x);
			projectile2->presentedRect.y = (enemy->presentedRect.y+enemy->presentedRect.h/2-projectile->presentedRect.h/2); 
			projectile2->direction = 'l';
			projectile2->activate = 1;
			projectile2->animationCycleCounter = 0;

			enemy->bulletNumber += 2;

			if( enemy->bulletNumber > 29 )
			{
				enemy->bulletNumber = 0;
			}
			enemy->shootDelayCounter = 1;
		}
		enemy->animationCycleCounter = 0;
		enemy->way = rand() % 4;
	}
	
	else
	{
		if( enemy->way == l_up ) // Cima
		{
			if( enemy->presentedRect.y > 131 )
			{
				enemy->presentedRect.y -= typeOfEnemy.speed;
			}
			else
			{
				enemy->way = rand() % 4;
				enemy->animationCycleCounter = 0;
			}
			if( enemy->animationCycleCounter%16 == 0 )
			{
				enemy->snipRect.x += 85;
			}
			if( enemy->snipRect.x < 765 || enemy->snipRect.x > 850 )
			{
				enemy->snipRect.x = 765;
			}
		}
		else if( enemy->way == l_down ) // Baixo
		{
			if( enemy->presentedRect.y+enemy->presentedRect.h < 580 )
			{
				enemy->presentedRect.y += typeOfEnemy.speed;
			}
			else
			{
				enemy->way = rand() % 4;
				enemy->animationCycleCounter = 0;
			}
			if( enemy->animationCycleCounter%16 == 0 )
			{
				enemy->snipRect.x += 85;
			}
			if( enemy->snipRect.x < 0 || enemy->snipRect.x > 85 )
			{
				enemy->snipRect.x = 0;
			}	
		}
		else if( enemy->way == l_left ) // Esquerda
		{
			if( enemy->presentedRect.x > 21 )
			{
				enemy->presentedRect.x -= typeOfEnemy.speed;
			}
			else
			{
				enemy->way = rand() % 4;
				enemy->animationCycleCounter = 0;
			}
			if( enemy->animationCycleCounter%16 == 0 )
			{
				enemy->snipRect.x += 85;
			}
			if( enemy->snipRect.x < 255 || enemy->snipRect.x > 340 )
			{
				enemy->snipRect.x = 255;
			}
		}
		else if( enemy->way == l_right ) // Direta
		{
			if( enemy->presentedRect.x+enemy->presentedRect.w < 780 )
			{
				enemy->presentedRect.x += typeOfEnemy.speed;
			}
			else
			{
				enemy->way = rand() % 4;
				enemy->animationCycleCounter = 0;
			}
			if( enemy->animationCycleCounter%16 == 0 )
			{
				enemy->snipRect.x += 85;
			}
			if( enemy->snipRect.x < 510 || enemy->snipRect.x > 595 )
			{
				enemy->snipRect.x = 510;
			}
		}
		enemy->animationCycleCounter++;
	}

	if( enemy->shootDelayCounter%55 != 0 )
	{
		enemy->shootDelayCounter++;
	} 
}