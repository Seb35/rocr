/*
 *  dynamics.cpp
 *  Reverse_OCR
 *
 *  Created by sbeyou on 30 juil. 2011.
 *  Copyleft 2011. No rights reserved. WTFPL.
 *
 */

#ifndef __DYNAMICS_H__
#define __DYNAMICS_H__

#include "rocr.h"

namespace rocr {
	
	////////////////////
	// Initialization //
	////////////////////
	
	//particle fInitialise(smc::rng *pRng);
	
	//double logLikelihood( long lTime, const rocr::particle& X );
	
	////////////////////////
	// General parameters //
	////////////////////////
	
	
	
	
	
	//////////
	// ALTO //
	//////////
	
	struct input_parameters_ALTO : protected input_parameters_text {
		
		input_parameters_ALTO() : type("ALTO"), avalaible_word(TRUE), avalaible_coordinates(true), avalaible_caesurae(true), text() {};
		
	};
	
	//////////////
	// Wikitext //
	//////////////
	
	struct input_parameters_wikitext : protected input_parameters_text {
		
		input_parameters_wikitext() : type("wikitext"), avalaible_word(TRUE), avalaible_coordinates(true), avalaible_caesurae(true), text() {};
	};
	
	//////////
	// DjVu //
	//////////
	
	struct input_parameters_DjVu : protected input_parameters_text {
		
		input_parameters_DjVu() : type("DjVu"), avalaible_word(TRUE), avalaible_coordinates(true), avalaible_caesurae(true), text() {};
	};
	
}

#endif /* __DYNAMICS_H__ */
