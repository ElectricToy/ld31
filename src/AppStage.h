//
//  AppStage.h
//  ld31
//
//  Created by Jeff Wofford on 12/2/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#ifndef ld31_AppStage_h
#define ld31_AppStage_h

#include "Essentials.h"
#include "Stage.h"

namespace fr
{
	class MusicManager;
	class GameCenter;
}

namespace ld
{
	class HUD;
	
	class AppStage : public fr::Stage
	{
		FRESH_DECLARE_CLASS( AppStage, Stage );
	public:
		
		virtual void update() override;
		
		HUD& hud() const;
		
		void restartGame();
		
	protected:
		
		void createWorld();
		
	private:
		
		VAR( ClassInfo::cptr, m_worldClass );
		
		fr::DisplayPackage::ptr m_worldPackage;

		SmartPtr< fr::MusicManager > m_musicManager;
		
		std::unique_ptr< fr::GameCenter > m_gameCenter;
	};
	
}

#endif
