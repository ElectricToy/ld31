//
//  HUD.h
//  FreshApp
//
//  Created by Jeff Wofford on 12/7/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_HUD_h
#define FreshApp_HUD_h

#include "Essentials.h"

namespace ld
{
	class ldWorld;
	
	class HUD : public fr::MovieClip
	{
		FRESH_DECLARE_CLASS( HUD, MovieClip );
	public:
		
		void onButtonPause();
		void onButtonTake();
		void onButtonDrop();
		void onButtonUse();
		
		virtual void update() override;

	protected:
		
		void updateButtonEnablement();
		
		ldWorld& world() const;
		
	private:
		
	};
	
}

#endif
