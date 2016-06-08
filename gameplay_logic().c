int gameplay_logic( int argc, char *argv[] )
{
	free_music_sfx( 'b' );
	free_texture();

	// "Velocidade" do sprites
	float velx = 2.5;	
	
	load_Texture( "mapa.png" ); 
	SDL_RenderClear( g_renderer );
    SDL_RenderCopy( g_renderer, g_texture, NULL, NULL );
    
    // Area que sera apresentada da imagem fonte
	SDL_Rect l_srcRect;
	l_srcRect.x = 320;
	l_srcRect.y = 0;
	l_srcRect.w = 80;
	l_srcRect.h = 80; 
    
    // Area onde a imagem fonte sera aplicada
	SDL_Rect l_dstRect;	
	l_dstRect.x = SCREEN_WIDTH/2;
	l_dstRect.y = SCREEN_HEIGHT/2;
	l_dstRect.w = 80;
	l_dstRect.h = 80;
    
    SDL_Event e;
    
	while( g_gameState == 3 )
	{
		while( SDL_PollEvent( &e ) != 0 )
        {	
        	if( l_srcRect.x > 560)
        	{
        		l_srcRect.x = 320;
			}
			
			load_Texture( "sprites/sombra100-60.png" );
			SDL_RenderCopy( g_renderer, g_texture, &l_srcRect, &l_dstRect );
			SDL_RenderPresent( g_renderer ); 
			
			if( e.type == SDL_KEYDOWN )
			{
           		switch( e.key.keysym.sym )
        		{
					case SDLK_RIGHT:
						while( e.type != SDL_KEYUP)
						{
							l_dstRect.x += velx;
							
						}
						break;
        				
					case SDLK_LEFT:
						while( e.type != SDL_KEYUP) 
						{
							l_dstRect.x -= velx;
						}
						break;

					case SDLK_UP:
						while( e.type != SDL_KEYUP)
						{
							l_dstRect.y -= velx;
							l_srcRect.x -= l_srcRect.w;
						}
						break;

					case SDLK_DOWN:
						while( e.type != SDL_KEYUP)
						{
							l_dstRect.y += velx;
							l_srcRect.x -= l_srcRect.w;
						}
						break;

					case SDLK_RETURN:
                        g_quitGame = 1;                                      
						break;
           		}
           		
           		// Retomado aos sprites static
           		l_srcRect.x = 320;
        	}
        	
        	l_srcRect.x += 80;
        }
	}
}
