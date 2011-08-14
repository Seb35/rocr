/*
 *  dynamics.cpp
 *  Reverse_OCR
 *
 *  Created by Seb35 on 30 July 2011.
 *  Copyleft 2011. No rights reserved. WTFPL.
 *
 */

#ifndef __DYNAMICS_H__
#define __DYNAMICS_H__

#include "rocr.h"
#include "utils.h"
#include <unicode/ustdio.h>
#include <unicode/uchar.h>

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
		
		input_parameters_ALTO() { type = "ALTO"; available_word = B3TRUE; available_coordinates = B3TRUE; available_caesurae = B3TRUE; }
		
		void readPage() {
			
			
		}
	};
	
	//////////////
	// Wikitext //
	//////////////
	
	struct input_parameters_wikitext : protected input_parameters_text {
		
		std::string filename;
		
		input_parameters_wikitext() { type = "wikitext"; available_word = B3TRUE; available_coordinates = B3TRUE; available_caesurae = B3TRUE; }
		
		void readPage() {
			
			UFILE* f = u_fopen( filename.c_str(), "r", NULL, "UTF-8" );
			
			
		}
	};
	
	//////////
	// DjVu //
	//////////
	
	struct input_parameters_DjVu : protected input_parameters_text {
		
		input_parameters_DjVu() { type = "DjVu"; available_word = B3TRUE; available_coordinates = B3TRUE; available_caesurae = B3TRUE; }
		
		void readPage() {
			
		}
	};
	
}

#endif /* __DYNAMICS_H__ */
