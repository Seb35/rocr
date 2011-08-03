/*
 *  rocr.h
 *  Reverse_OCR
 *
 *  Created by sbeyou on 1 août 2011.
 *  Copyright 2011 INRIA. All rights reserved.
 *
 */

#ifndef __ROCR_H__
#define __ROCR_H__

#include "spaces.h"


namespace rocr {
	
	/////////////////////
	// Data structures //
	/////////////////////
	
	// Observation parameters
	struct input_parameters_image {
		
		std::string filename;
	};
	
	// Specific textblock
	struct input_parameters_textblock {
		
		unsigned int TR;
		unsigned int TW;
		unsigned int TH;
		std::string TF;
		unsigned int TS;
		bool3 TB;
		bool3 TI;
		caesura_t TC;
		unsigned int TD;
		unsigned int TY;
		unsigned int TX;
		
		double STW;
		double STH;
		double STF;
		double STY;
		double STX;
		
		Language_heuristics* lang;
	};
	
	// Ocerized text of the specific textblock
	struct input_parameters_text {
		
		std::string type;
		bool3 available_word;
		bool3 available_coordinates;
		bool3 available_caesurae;
		text text;
	};
	
	// PF parameters
	struct input_parameters_PF {
		
		long N; ///< Number of particles
	};
	
	struct input_parameters {
		
		input_parameters_image* params_image;         ///< observation
		input_parameters_textblock* params_textblock; ///< fixed parameters
		input_parameters_PF* params_pf;               ///< PF parameters
		input_parameters_text* params_text;           ///< proposed text
	};
	
	struct ROCR {
		
		enum { MOVE_SAMELINE, MOVE_CAESURA, MOVE_NEWLINE }; 
		
		word space;
		
		input_parameters params;
		smc::sampler<word> sampler;
		smc::moveset<word> moveset;
		Language_heuristics* lang;
		
		
		ROCR();
		
		void initialize_page( input_parameters_image* params_image );
		void initialize_textblock( input_parameters_textblock* params_textblock, input_parameters_text* params_text );
		void initialize_PF( input_parameters_PF* params_pf );
		
		// Particle filter
		particle pfInitialise( smc::rng* pRng );
		
		long pfSelect( long lTime, particle& particle, smc::rng* pRng );
		
		void pfMoveCommon( long lTime, particle& pFrom, smc::rng* pRng );
		void pfMoveSameLine( long lTime, particle& pFrom, smc::rng* pRng );
		void pfMoveCaesura( long lTime, particle& pFrom, smc::rng* pRng );
		void pfMoveNewLine( long lTime, particle& pFrom, smc::rng* pRng );
		
		double pfLogLikelihood( long lTime, const particle& X );
		
		
		
	};
}

#endif /* __ROCR_H__ */
