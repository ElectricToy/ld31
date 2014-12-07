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
	
	void AppStage::onBeginPlay()
	{
		Super::onBeginPlay();
		
		createWorld();
	}
	
	void AppStage::createWorld()
	{
		ASSERT( m_worldClass );
		auto world = createObject< ldWorld >( *m_worldClass );
		
		addChildAt( world, 0 );
		
		world->onBeginPlay();
		
		if( auto hud = getDescendantByName< HUD >(""))
		{
			hud->onGameBeginning();
		}
	}
	
}

