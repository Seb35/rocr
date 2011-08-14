/*
 *  rocr.h
 *  Reverse_OCR
 *
 *  Created by Sébastien Beyou on 1 August 2011.
 *  Copyleft 2011. No rights reserved. WTFPL.
 *
 */

#ifndef __ROCR_H__
#define __ROCR_H__

#include "spaces.h"
#include "heuristics.h"
#include "image_creator.h"


namespace rocr {
	
	/////////////////////
	// Data structures //
	/////////////////////
	
	// Observation parameters
	struct input_parameters_image {
		
		std::string filename;
		unsigned int PW;
		unsigned int PH;
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
		
		typedef text* iterator;
		typedef const text* const_iterator;
		
		std::string type;
		bool3 available_word;
		bool3 available_coordinates;
		bool3 available_caesurae;
		text texte;
		
		text* textblocks;
		unsigned int ntextblocks;
		
		virtual void readPage() = 0;
		
		iterator begin();
		const_iterator begin() const;
		
		iterator end();
		const_iterator end() const;
		
		text& front();
		const text& front() const;
		
		text& back();
		const text& back() const;
	};
	
	// PF parameters
	struct input_parameters_PF {
		
		long N;                      ///< Number of particles
		double resampling_threshold; ///< Resampling threshold
	};
	
	struct input_parameters {
		
		input_parameters_image* params_image;         ///< observation
		input_parameters_textblock* params_textblock; ///< fixed parameters
		input_parameters_text* params_text;           ///< proposed text
		input_parameters_PF* params_pf;               ///< PF parameters
	};
	
	struct ROCR {
		
		enum { MOVE_SAMELINE, MOVE_CAESURA, MOVE_NEWLINE }; 
		
		word space;
		
		input_parameters params;
		smc::sampler<word> sampler;
		smc::moveset<word> moveset;
		Language_heuristics* lang;
		Image_Creator imcreator;
		
		cimg_library::CImg<unsigned int> image_page;
		
		
		ROCR();
		
		void initialize_page( input_parameters_image* params_image );
		void initialize_textblock( input_parameters_textblock* params_textblock, input_parameters_text* params_text );
		void initialize_text( input_parameters_text* params_text );
		void initialize_PF( input_parameters_PF* params_pf );
		
		// Particle filter
		particle pfInitialise( smc::rng* pRng );
		
		long pfSelect( long lTime, particle& particle, smc::rng* pRng );
		
		void pfMoveCommon( long lTime, particle& pFrom, smc::rng* pRng );
		void pfMoveSameLine( long lTime, particle& pFrom, smc::rng* pRng );
		void pfMoveCaesura( long lTime, particle& pFrom, smc::rng* pRng );
		void pfMoveNewLine( long lTime, particle& pFrom, smc::rng* pRng );
		
		double pfLogLikelihood( long lTime, word& particle );
		
		double integrand_mean_WF( const word& word, void* );
		double integrand_mean_WS( const word& word, void* );
		double integrand_mean_WB( const word& word, void* );
		double integrand_mean_WI( const word& word, void* );
		double integrand_mean_WK( const word& word, void* );
		double integrand_mean_WC( const word& word, void* );
		double integrand_mean_WD( const word& word, void* );
		double integrand_mean_WY( const word& word, void* );
		double integrand_mean_WX( const word& word, void* );
		
		double integrand_var_WF( const word& word, void* );
		double integrand_var_WS( const word& word, void* );
		double integrand_var_WB( const word& word, void* );
		double integrand_var_WI( const word& word, void* );
		double integrand_var_WK( const word& word, void* );
		double integrand_var_WC( const word& word, void* );
		double integrand_var_WD( const word& word, void* );
		double integrand_var_WY( const word& word, void* );
		double integrand_var_WX( const word& word, void* );
		
		void run();
	};
	
	particle gpfInitialise( smc::rng* pRng );
	
	long gpfSelect( long lTime, particle& particle, smc::rng* pRng );
	
	void gpfMoveCommon( long lTime, particle& pFrom, smc::rng* pRng );
	void gpfMoveSameLine( long lTime, particle& pFrom, smc::rng* pRng );
	void gpfMoveCaesura( long lTime, particle& pFrom, smc::rng* pRng );
	void gpfMoveNewLine( long lTime, particle& pFrom, smc::rng* pRng );
	
	double gpfLogLikelihood( long lTime, word& particle );
}

#endif /* __ROCR_H__ */
