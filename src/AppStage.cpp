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
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( AppStage )
	DEFINE_VAR( AppStage, ClassInfo::cptr, m_worldClass );
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( AppStage )

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
		if( !getDescendantByName< World >(""))
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
		if( auto world = getDescendantByName< World >(""))
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
	}
	
}

