//
//  ldPlayerController.h
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_ldPlayerController_h
#define FreshApp_ldPlayerController_h

#include "Essentials.h"
#include "PlayerController.h"

namespace ld
{
	
	class ldPlayerController : public fr::PlayerController
	{
		FRESH_DECLARE_CLASS( ldPlayerController, PlayerController );
	public:
		
		virtual void update() override;
		
	private:
		
		FRESH_DECLARE_CALLBACK( ldPlayerController, onStageKeyDown, fr::EventKeyboard )
		FRESH_DECLARE_CALLBACK( ldPlayerController, onStageKeyUp, fr::EventKeyboard )

	};
	
}

#endif
