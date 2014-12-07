//
//  HUD.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/7/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "HUD.h"
#include "ldWorld.h"
#include "Human.h"
#include "AppStage.h"
#include "SimpleButton.h"
#include "TextField.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( HUD )
	
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( HUD )
	
	DEFINE_METHOD( HUD, onButtonPause )
	DEFINE_METHOD( HUD, onButtonTake )
	DEFINE_METHOD( HUD, onButtonDrop )
	DEFINE_METHOD( HUD, onButtonUse )
	
	ldWorld& HUD::world() const
	{
		auto theWorld = stage().getDescendantByName< ldWorld >( "" );
		ASSERT( theWorld );
		return *theWorld;
	}
	
	void HUD::onButtonPause()
	{}
	
	void HUD::onButtonTake()
	{
		if( auto player = world().player())
		{
			player->pickupTouchingActor();
		}
	}
	
	void HUD::onButtonDrop()
	{
		if( auto player = world().player())
		{
			player->dropHeldActor();
		}
	}
	
	void HUD::onButtonUse()
	{
		if( auto player = world().player())
		{
			player->placeHeldActor();
		}
	}
	
	void HUD::update()
	{
		updateButtonEnablement();
		
		// Update clock.
		//
		if( auto clockText = getDescendantByName< TextField >( "_clocktext" ))
		{
			int hours, mins, seconds;
			
			const int timePlayedSeconds = world().timePlayedSeconds();
			
			hours = timePlayedSeconds / ( 60 * 60 );
			mins = ( timePlayedSeconds / 60 ) % 60;
			seconds = timePlayedSeconds % 60;
			
			std::ostringstream text;
			text << std::setfill( '0' ) << std::setw( 2 ) << hours << ":"
				<< std::setfill( '0' ) << std::setw( 2 ) << mins << ":"
				<< std::setfill( '0' ) << std::setw( 2 ) << seconds;
			
			clockText->text( text.str() );
		}
		
		
		Super::update();
	}
	
	void HUD::updateButtonEnablement()
	{
		auto player = world().player();

		if( auto button = getDescendantByName< SimpleButton >( "_take" ))
		{
			button->enabled( player && player->canPickupTouchingActor() );
		}
		if( auto button = getDescendantByName< SimpleButton >( "_drop" ))
		{
			button->enabled( player && player->canDropHeldActor() );
		}
		if( auto button = getDescendantByName< SimpleButton >( "_use" ))
		{
			button->enabled( player && player->canUseHeldActor() );
		}
	}
}

