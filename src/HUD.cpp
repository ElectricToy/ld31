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
#include "UIPopup.h"
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
			player->useHeldActor();
		}
	}
	
	void HUD::update()
	{
		updateButtonEnablement();
		
		lookForMessagesToShow();
		
		// Hide startup if necessary.
		//
		if( auto startup = getDescendantByName< UIPopup >( "_startup" ))
		{
			// Player moving?
			//
			if( !startup->isBecomingHidden() && !startup->isFullyHidden() )
			{
				if( auto player = world().player())
				{
					if( !player->stepDirection().isZero() )
					{
						startup->hide();
					}
				}
			}
		}

		
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
			
			clockText->color( colorLerp( clockText->color(), world().isGameActive() ? 0xff9bfe00 : 0xffffed8c, 0.1f ));
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
	
	void HUD::showMessage( const std::string& message, ClassNameRef messageClassName )
	{
		const auto messageClass = getClass( messageClassName );
		ASSERT( messageClass );
		
		if( auto messageHost = getDescendantByName< DisplayObjectContainer >( "_messagehost" ))
		{
			// Hide any prior popup.
			//
			if( messageHost->numChildren() > 0 )
			{
				messageHost->getChildAt( messageHost->numChildren() - 1 )->as< UIPopup >()->hideWithDuration( 0.25, true, 0 );
			}
			
			// Make a new popup.
			//
			auto messagePopup = createObject< UIPopup >( *messageClass );
			
			if( auto text = messagePopup->getDescendantByName< TextField >( "_messagetext" ))
			{
				messageHost->addChild( messagePopup );
			
				text->text( message );
				messagePopup->show();
				messagePopup->hideWithDuration( 4.0, true, 4.0 );
			}
		}
	}
	
	void HUD::lookForMessagesToShow()
	{
		if( auto player = world().player())
		{
			if( auto playerHeld = player->heldActor())
			{
				const auto heldClass = playerHeld->className();
				
				auto iter = m_heldClassMessages.find( heldClass );
				if( iter != m_heldClassMessages.end() )
				{
					showMessage( iter->second, "MessagePopup" );
					m_heldClassMessages.erase( iter );
				}
			}
		}
	}

	void HUD::onGameBeginning()
	{
		populateMessages();
		
		if( auto startup = getDescendantByName< UIPopup >( "_startup" ))
		{
			startup->show();
		}
	}
	
	void HUD::populateMessages()
	{
		m_heldClassMessages.clear();
		
		m_heldClassMessages[ "TorchConfigured" ] = "TORCH: Struggles against the darkness.";
		m_heldClassMessages[ "DoorWood" ] = "WOOD DOOR: You can pass through. Monsters tear it down.";
		m_heldClassMessages[ "DoorIron" ] = "IRON DOOR: You can pass through. Monsters struggle to tear it down.";
		m_heldClassMessages[ "BlockGravel" ] = "GRAVEL BLOCK: Once placed, it stays put. Easy to break through.";
		m_heldClassMessages[ "BlockBrick" ] = "BRICK BLOCK: Once placed, it stays put. Tough to break through.";
		m_heldClassMessages[ "BlockStone" ] = "STONE BLOCK: Once placed, it stays put. Really tough to break.";
		m_heldClassMessages[ "MineConfigured" ] = "MINE: Only monsters trigger it. But when it blows, run!";
		m_heldClassMessages[ "TurretConfigured" ] = "CROSSBOW: Fire by hand, or drop for automatic defense. Limited ammo.";
	}
}

