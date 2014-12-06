//
//  Scene.h
//  FreshApp
//
//  Created by Jeff Wofford on 12/5/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_Scene_h
#define FreshApp_Scene_h

#include "Essentials.h"
#include "Body.h"

namespace ld
{
	class Scene : public fr::DisplayObjectContainer
	{
		FRESH_DECLARE_CLASS( Scene, DisplayObjectContainer );
	public:
		
		typedef std::pair< Body::wptr, Body::wptr > Link;
		
		void linkBodies( Body::ptr a, Body::ptr b );
		
		virtual void onAddedToStage() override;
		
		virtual void update() override;
		
		template< typename FunctionT >
		void eachBody( FunctionT&& fn )
		{
			forEachChild< Body >( std::move( fn ));
		}
		
	private:
		
		VAR( vec2, m_gravity );
		VAR( vec2, m_wind );
		
		VAR( std::vector< Link >, m_links );
		
	};
	
}

#endif
