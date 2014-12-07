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
		
		void onGameBeginning();
		
		void onButtonPause();
		void onButtonTake();
		void onButtonDrop();
		void onButtonUse();
		
		void showMessage( const std::string& message, fr::ClassNameRef messageClass );
		
		virtual void update() override;
		
	protected:
		
		void lookForMessagesToShow();
		void updateButtonEnablement();
		
		ldWorld& world() const;
		
		void populateMessages();
		
	private:
		
		std::map< ClassName, std::string > m_heldClassMessages;
		
	};
	
}

#endif
