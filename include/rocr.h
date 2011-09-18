/*
 *  rocr.h
 *  Reverse_OCR
 *
 *  Created by Seb35 on 1 August 2011.
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
		
		// @public
		std::string filename; // filename of the image
		unsigned int PV; // width-resolution
		unsigned int PG; // height-resolution
		
		// @read-only
		unsigned int PW; // width of the image
		unsigned int PH; // height of the image
	};
	
	//
	// Specific textblock
	//
	// The Reverse_OCR only takes as input a specific textblock (for instance a paragraph or an isolated line)
	// These parameters specify this textblock
	// (A page should/must be analyzed previously by a segmentation algorithm to isolate textblocks, since this is
	//  not a core part of the Reverse_OCR.)
	struct input_parameters_textblock {
		
		// objective parameters
		unsigned int TR; // 
		unsigned int TW; // width of the textblock
		unsigned int TH; // height of the textblock
		unsigned int TY; // vertical position of the textblock
		unsigned int TX; // horizontal position of the textblock
		
		// heuristics
		std::string TF;  // presumed font of the textblock
		unsigned int TS; // presumed font size of the textblock
		bool3 TB;        // presumed boldness of the first word
		bool3 TI;        // presumed italicsness of the first word
		caesura_t TC;    // presumed caesuraness of the first word
		unsigned int TD; // presumed caesura index of the first word (if caesura)
		
		double STW; // standard deviation of the width
		double STH; // standard deviation of the height
		double STF; // 
		double STY; // standard deviation of the vertical position
		double STX; // standard deviation of the horizontal position
		
		Language_heuristics* lang;
	};
	
	// Ocerized text of the specific textblock
	struct input_parameters_text {
		
		typedef std::list<text*>::iterator iterator;
		typedef std::list<text*>::const_iterator const_iterator;
		
		std::string type;
		bool3 available_word;
		bool3 available_coordinates;
		bool3 available_caesurae;
		//text texte;
		
		std::list<text*> textblocks;
		unsigned int ntextblocks;
		
		virtual void readPage() = 0;
		
		iterator begin();
		const_iterator begin() const;
		
		iterator end();
		const_iterator end() const;
		
		text* front();
		const text* front() const;
		
		text* back();
		const text* back() const;
	};
	
	//
	// Particle Filter parameters
	//
	// Specific assimilation parameters
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
	
	extern ROCR myROCR;
	
	
	particle gpfInitialise( smc::rng* pRng );
	
	long gpfSelect( long lTime, particle& particle, smc::rng* pRng );
	
	void gpfMoveCommon( long lTime, particle& pFrom, smc::rng* pRng );
	void gpfMoveSameLine( long lTime, particle& pFrom, smc::rng* pRng );
	void gpfMoveCaesura( long lTime, particle& pFrom, smc::rng* pRng );
	void gpfMoveNewLine( long lTime, particle& pFrom, smc::rng* pRng );
	
	double gpfLogLikelihood( long lTime, word& particle );
}

#endif /* __ROCR_H__ */
