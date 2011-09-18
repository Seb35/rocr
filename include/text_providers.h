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
	
	typedef std::pair<unsigned int,unsigned int> order_alto;
    
	struct input_parameters_ALTO : public input_parameters_text {
		
		std::string filename;
		double cv, ch, cw, cx, cy;
		
		input_parameters_ALTO();
		
		void readPage();
	};
	
	//////////////
	// Wikitext //
	//////////////
	
	typedef std::pair<unsigned int,unsigned int> order_wikitext;
	
	struct input_parameters_wikitext : protected input_parameters_text {
		
		std::string filename;
		
		input_parameters_wikitext();
		
		void readPage();
	};
	
	//////////
	// DjVu //
	//////////
	
	typedef std::pair<unsigned int,unsigned int> order_djvu;
	
	struct input_parameters_DjVu : protected input_parameters_text {
		
		input_parameters_DjVu();
		
		void readPage();
	};
	
}

#endif /* __DYNAMICS_H__ */
