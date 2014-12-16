//
//  AppStage.cpp
//  ld31
//
//  Created by Jeff Wofford on 12/2/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "AppStage.h"
#include "ldWorld.h"
#include "HUD.h"
#include "MusicManager.h"
#include "FreshGameCenter.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( AppStage )
	DEFINE_VAR( AppStage, ClassInfo::cptr, m_worldClass );
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTOR_INERT( AppStage )
	
	FRESH_CUSTOM_STANDARD_CONSTRUCTOR_NAMING( AppStage )
	{
		m_gameCenter.reset( new GameCenter() );
		m_gameCenter->authenticate();
		
		if( AudioSystem::ready() )
		{
			AudioSystem::instance().setDefaultAttenuationRange( Range< float >( 4.0f, 8.0f ));
			m_musicManager = createObject< MusicManager >();
		}
	}

	HUD& AppStage::hud() const
	{
		auto hud = getDescendantByName< HUD >("");
		ASSERT( hud );
		return *hud;
	}
	
	void AppStage::update()
	{
		Super::update();
		
		// Have a world?
		//
		if( !getDescendantByName< FreshWorld >(""))
		{
			// Nope. Make one.
			//
			createWorld();
		}
	}
	
	void AppStage::restartGame()
	{
		// Destroy any existing world.
		//
		if( auto world = getDescendantByName< FreshWorld >(""))
		{
			removeChild( world );
			m_worldPackage = nullptr;
		}
	}
	
	void AppStage::createWorld()
	{
		m_worldPackage = createPackage< DisplayPackage >();
		pushActivePackage( m_worldPackage );
		
		ASSERT( m_worldClass );
		auto world = createObject< ldWorld >( *m_worldClass );
		
		addChildAt( world, 0 );
		
		hud().onGameBeginning();
		
		if( m_musicManager )
		{
			m_musicManager->playMusic( MusicManager::Category::Background, "music_theme" );
		}
	}
	
}

