//
//  Missile.h
//  FreshApp
//
//  Created by Jeff Wofford on 12/7/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_Missile_h
#define FreshApp_Missile_h

#include "Essentials.h"
#include "ldActor.h"

namespace ld
{
	
	class Missile : public ldActor
	{
		FRESH_DECLARE_CLASS( Missile, ldActor );
	public:
		
		virtual void onTouched( ldActor& other ) override;
		
		virtual void update() override;
		
	private:
		
	};
	
}

#endif
