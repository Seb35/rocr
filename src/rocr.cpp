/*
 *  rocr.cpp
 *  Reverse_OCR
 *
 *  Created by Seb35 on 1 August 2011.
 *  Copyleft 2011. No rights reserved. WTFPL.
 *
 */

#include "../include/rocr.h"
#include "../include/text_providers.h"

#include <boost/mem_fn.hpp>

namespace rocr {

input_parameters_text::iterator input_parameters_text::begin() {
	
	return textblocks.begin();
}

input_parameters_text::const_iterator input_parameters_text::begin() const {
	
	return textblocks.begin();
}

input_parameters_text::iterator input_parameters_text::end() {
	
	return textblocks.end();
}

input_parameters_text::const_iterator input_parameters_text::end() const {
	
	return textblocks.end();
}

text* input_parameters_text::front() {
	
	return textblocks.front();
}

const text* input_parameters_text::front() const {
	
	return textblocks.front();
}

text* input_parameters_text::back() {
	
	return textblocks.back();
}

const text* input_parameters_text::back() const {
	
	return textblocks.back();
}

ROCR myROCR;

ROCR::ROCR() : params(), sampler(), moveset(), lang(0) {
	
	space.WK = " ";
}

void ROCR::initialize_page( input_parameters_image* params_image ) {
	
	this->params.params_image = params_image;
	
	image_page.load( this->params.params_image->filename.c_str() );
	imcreator.set_resolution( params_image->PV, params_image->PG );
	
	params_image->PH = image_page.height();
	params_image->PW = image_page.width();
}

void ROCR::initialize_textblock( input_parameters_textblock* params_textblock, input_parameters_text* params_text ) {
	
	this->params.params_textblock = params_textblock;
	this->params.params_text = params_text;
	this->lang = this->params.params_textblock->lang;
}

void ROCR::initialize_PF( input_parameters_PF* params_pf ) {
	
	this->params.params_pf = params_pf;
}

particle ROCR::pfInitialise( smc::rng* pRng ) {
	
	particle particle;
	word* value = particle.GetValuePointer();
	
	value->set_WF( this->params.params_textblock->TF );
	value->set_WS( this->params.params_textblock->TS );
	value->set_WB( this->params.params_textblock->TB );
	value->set_WI( this->params.params_textblock->TI );
	//value->set_WE( "" );
	value->set_WK( "" );
	value->set_WC( this->params.params_textblock->TC );
	value->set_WD( this->params.params_textblock->TD );
	value->set_WN( true );
	value->set_WY( this->params.params_textblock->TY );
	value->set_WX( this->params.params_textblock->TX );
	
	moveset.DoMove( 0, particle, pRng );
	
	return particle;
}

long ROCR::pfSelect( long lTime, particle& particle, smc::rng* pRng ) {
	
	const word& value = particle.GetValue();
	
	
	double endTextblock = pRng->Normal( params.params_textblock->TX + params.params_textblock->TW, sqrt(params.params_textblock->STX*params.params_textblock->STX + params.params_textblock->STW*params.params_textblock->STW) );
	
	if( value.WS > endTextblock ) return MOVE_NEWLINE;
	else return MOVE_SAMELINE;
}

void ROCR::pfMoveCommon( long lTime, particle& particle, smc::rng* pRng ) {
	
	word* value = particle.GetValuePointer();
	
	// Get the word from the source text
	word estimated_word;
	
	
	
	// Word //
	
	
	
	// Boldness //
	double random_boldness = pRng->UniformS();
	if( random_boldness < 0.02 * lang->SWB( particle ) + 0.98 * estimated_word.WB ) value->set_WB( true );
	else value->set_WB( false );
	
	
	// Italics //
	double random_italics = pRng->UniformS();
	if( random_italics < 0.02 * lang->SWI( particle ) + 0.98 * estimated_word.WI ) value->set_WI( true );
	else value->set_WI( false );
	
	
	// Caesura //
	double mean_caesura = value->WX + value->WW + space.WW;
//	double std_caesura = sqrt( value->SWX*value->SWX + value->SWW*value->SWW + space.SWW*space.SWW );
	double mean_limit_textblock = params.params_textblock->TX + params.params_textblock->TW;
	double std_limit_textblock = sqrt( params.params_textblock->STX * params.params_textblock->STX + params.params_textblock->STW * params.params_textblock->STW );
	
	// 1) The end of the word is very before the end of the textblock -> no caesura
	if( mean_limit_textblock - mean_caesura > 10 ) {
		
		value->set_WC( false );
	}
	
	// 2) The end of the word is very after the end of the textblock -> caesura -> search on what letter
	else if( mean_caesura - mean_limit_textblock > 10 ) {
		
		value->set_WC( true );
		
		double random_limit_textblock = pRng->Normal( mean_limit_textblock, std_limit_textblock );
//		double random_caesura = pRng->Normal( mean_caesura, std_caesura );
		unsigned int index = 0;
		double mean_word_size = double(estimated_word.WW)/double(estimated_word.WL);
		
//		do {} while( random_caesura + mean_word_size*index <= random_limit_textblock && index < estimated_word.WL );
		if( index == estimated_word.WL ) throw std::exception();
		
		double p1 = pRng->Normal( mean_word_size*index-random_limit_textblock, sqrt(std_limit_textblock*std_limit_textblock+100) );
		double p2 = index > 0 ? pRng->Normal( mean_word_size*(index-1)-random_limit_textblock, sqrt(std_limit_textblock*std_limit_textblock+100) ) : 0.0;
		double p3 = index < estimated_word.WL-1 ? pRng->Normal( mean_word_size*(index+1)-random_limit_textblock, sqrt(std_limit_textblock*std_limit_textblock+100) ) : 0.0;
		double ps = 0.15*p1 + 0.75*p2 + 0.1*p3;
		double multiP[] = { 0.15*p1/ps, 0.75*p2/ps, 0.1*p3/ps };
		
		unsigned int length;
		pRng->Multinomial( 3, 1, multiP, &length );
		
		value->set_WC( true );
		value->set_WD( length );
	}
	
	// 3) The end of the word is about in the end of the textblock
	else {
		
		double random_limit_textblock = pRng->Normal( mean_limit_textblock, std_limit_textblock );
		unsigned int index = 0;
		double mean_word_size = double(estimated_word.WW)/double(estimated_word.WL);
		
		do {} while( mean_word_size*index < random_limit_textblock && index < estimated_word.WL );
		if( index == estimated_word.WL ) {
			
			value->set_WC( false );
		}
		else {
			double p1 = pRng->Normal( mean_word_size*index-random_limit_textblock, sqrt(std_limit_textblock*std_limit_textblock+100) );
			double p2 = index > 0 ? pRng->Normal( mean_word_size*(index-1)-random_limit_textblock, sqrt(std_limit_textblock*std_limit_textblock+100) ) : 0.0;
			double p3 = index < estimated_word.WL-1 ? pRng->Normal( mean_word_size*(index+1)-random_limit_textblock, sqrt(std_limit_textblock*std_limit_textblock+100) ) : 0.0;
			double ps = 0.33*p1 + 0.34*p2 + 0.33*p3;
			double multiP[] = { 0.33*p1/ps, 0.34*p2/ps, 0.33*p3/ps };
			
			unsigned int length;
			pRng->Multinomial( 3, 1, multiP, &length );
			
			value->set_WC( true );
			value->set_WD( length );
		}
	}
	
	//double random_caesura = pRng->Normal( mean_limit_textblock-mean_caesura, sqrt(std_limit_textblock*std_limit_textblock+std_caesura*std_caesura) );
	
	
	
	
	//if( value.WN == CAESURA_NO ) value->set_WN( false ); // TODO: very generally false, apart for very long words
	
	
	// Unchanged properties
	value->set_WF( value->WF );
	value->set_WS( value->WS );
	
}

void ROCR::pfMoveSameLine( long lTime, particle& particle, smc::rng* pRng ) {
	
	pfMoveCommon( lTime, particle, pRng );
	word* value = particle.GetValuePointer();
	
	// Caesura
	value->set_WC( true );
	
	// Position
//	value->set_WY( pRng->Normal( value->WY, value->STY ) );
//	value->set_WX( pRng->Normal( value->WX + value->WW, sqrt( value->STX*value->SWX + value->SWW*value->SWW ) ) );
	
	particle.AddToLogWeight( gpfLogLikelihood( lTime, *value ) );
}

void ROCR::pfMoveCaesura( long lTime, particle& particle, smc::rng* pRng ) {
	
	pfMoveCommon( lTime, particle, pRng );
	word* value = particle.GetValuePointer();
	
	// Caesura
	value->set_WC( true );
	value->set_WC( true );  // TODO: very generally CAESURA_ONE, apart for very long words -> CAESURA_HALF
	
	// Position
//	value->set_WY( pRng->Normal( value->WY, value->STY ) );
//	value->set_WX( pRng->Normal( value->WX + value->WW, sqrt( value->STX*value->SWX + value->SWW*value->SWW ) ) );
	
	particle.AddToLogWeight( gpfLogLikelihood( lTime, *value ) );
}

void ROCR::pfMoveNewLine( long lTime, particle& particle, smc::rng* pRng ) {
	
	pfMoveCommon( lTime, particle, pRng );
	word* value = particle.GetValuePointer();
	
	// Caesura
	//if( value->WC == CAESURA_ONE || value->WC == CAESURA_HALF || value->WC == CAESURA_TWO ) value->set_WE( value->WE );
	//else value->set_WE( "" );
	value->set_WN( true );
	value->set_WC( true );
	
	// Position
//	value->set_WY( pRng->Normal( value->WY + value->WH, sqrt( value->STY*value->STY + value->SWH*value->SWH ) ) );
	value->set_WX( pRng->Normal( this->params.params_textblock->TX, this->params.params_textblock->STX ) );
	
	particle.AddToLogWeight( gpfLogLikelihood( lTime, *value ) );
}

double ROCR::integrand_mean_WF( const word& word, void* ) {
	
	return 0.0;
}

double ROCR::integrand_mean_WS( const word& word, void* ) {
	
	return double(word.WS);
}

double ROCR::integrand_mean_WB( const word& word, void* ) {
	
	double wb = double( word.WB );
	if( wb == B3UNDEF ) wb = B3FALSE;
	return wb;
}

double ROCR::integrand_mean_WI( const word& word, void* ) {
	
	double wi = double( word.WI );
	if( wi == B3UNDEF ) wi = B3FALSE;
	return wi;
}

double ROCR::integrand_mean_WK( const word& word, void* ) {
	
	return 0.0;
}

double ROCR::integrand_mean_WC( const word& word, void* ) {
	
	return 0.0;
}

double ROCR::integrand_mean_WD( const word& word, void* ) {
	
	return 0.0;
}

double ROCR::integrand_mean_WY( const word& word, void* ) {
	
	return word.WY;
}

double ROCR::integrand_mean_WX( const word& word, void* ) {
	
	return word.WX;
}

double ROCR::integrand_var_WF( const word& word, void* ) {
	
	double d = 0;//word.stats-;
	return d*d;
}

double ROCR::integrand_var_WS( const word& word, void* ) {
	
	double d = word.WS - word.stats->MWS;
	return d*d;
}

double ROCR::integrand_var_WB( const word& word, void* ) {
	
	double wb = word.WB;
	if( wb == B3UNDEF ) wb = B3FALSE;
	double d = wb - double(word.stats->MWB);
	return d*d;
}

double ROCR::integrand_var_WI( const word& word, void* ) {
	
	double wi = word.WI;
	if( wi == B3UNDEF ) wi = B3FALSE;
	double d = wi - word.stats->MWI;
	return d*d;
}

double ROCR::integrand_var_WK( const word& word, void* ) {
	
	double d = 0;//word.stats-;
	return d*d;
}

double ROCR::integrand_var_WC( const word& word, void* ) {
	
	double d = 0;//word.stats-;
	return d*d;
}

double ROCR::integrand_var_WD( const word& word, void* ) {
	
	double d = 0;//word.stats-;
	return d*d;
}

double ROCR::integrand_var_WY( const word& word, void* ) {
	
	double d = word.WY - word.stats->MWY;
	return d*d;
}

double ROCR::integrand_var_WX( const word& word, void* ) {
	
	double d = word.WX - word.stats->MWX;
	return d*d;
}

void ROCR::run() {
	
	// Load image
	
	
	// Load observations
	
	//lIterates = load_data("data.csv", &y);
	
	
	
	//Initialise and run the sampler
	//smc::sampler<word> sampler( lNumber, SMC_HISTORY_NONE);  
	//smc::moveset<word> moveset( pfInitialise, pfMove, NULL );
	
	//particle ROCR::pfInitialise( smc::rng* pRng )
	
	boost::mem_fn( &ROCR::pfInitialise );
	
	void (*pfNewMoves[])( long, particle&, smc::rng* ) = { &gpfMoveSameLine, &gpfMoveCaesura, &gpfMoveNewLine };
	
	//particle (*pf)( smc::rng* pRng ) = ;
	//this->*run;
	//myROCR.*run;
	
	
	moveset.SetInitialisor( gpfInitialise );
	moveset.SetMoveSelectionFunction( gpfSelect );
	moveset.SetMoveFunctions( 3, pfNewMoves);
	moveset.SetMCMCFunction( 0 );
	
	sampler.Initialize( params.params_pf->N, SMC_HISTORY_NONE );
	sampler.SetResampleParams( SMC_RESAMPLE_RESIDUAL, params.params_pf->resampling_threshold );
	sampler.SetMoveSet( moveset );
	sampler.Initialise();
	
//	for( int n=1 ; n < lIterates ; ++n) {
		sampler.Iterate();
//		
//		double xm,xv,ym,yv;
//		xm = Sampler.Integrate(integrand_mean_x,NULL);
//		xv = Sampler.Integrate(integrand_var_x, (void*)&xm);
//		ym = Sampler.Integrate(integrand_mean_y,NULL);
//		yv = Sampler.Integrate(integrand_var_y, (void*)&ym);
//		
//		cout << xm << "," << ym << "," << xv << "," << yv << endl;
//	}
}

double ROCR::pfLogLikelihood( long lTime, word& word ) {
	
	cimg_library::CImg<unsigned char> image_text;
	cimg_library::CImg<unsigned char> image_orig;
	cimg_library::CImg<unsigned char> image_diff;
	
	imcreator.createTextImage( word, image_orig );
	image_orig = image_page.crop( word.WX, word.WY, word.WX+word.WW, word.WY+word.WH );
	image_diff = image_orig - image_text;
	
	return double(image_diff.abs().max());
}

particle gpfInitialise( smc::rng* pRng ) {
	
	return myROCR.pfInitialise( pRng );
}
long gpfSelect( long lTime, particle& particle, smc::rng* pRng ) {
	
	return myROCR.pfSelect( lTime, particle, pRng );
}
void gpfMoveCommon( long lTime, particle& particle, smc::rng* pRng ) {
	
	myROCR.pfMoveCommon( lTime, particle, pRng );
}
void gpfMoveSameLine( long lTime, particle& particle, smc::rng* pRng ) {
	
	myROCR.pfMoveSameLine( lTime, particle, pRng );
}
void gpfMoveCaesura( long lTime, particle& particle, smc::rng* pRng ) {
	
	myROCR.pfMoveCaesura( lTime, particle, pRng );
}
void gpfMoveNewLine( long lTime, particle& particle, smc::rng* pRng ) {
	
	myROCR.pfMoveNewLine( lTime, particle, pRng );
}
double gpfLogLikelihood( long lTime, word& word ) {
	
	return myROCR.pfLogLikelihood( lTime, word );
}

}

//double logLikelihood( long lTime, const particle& X ) {
//	
//	return - 0.5 * (nu_y + 1.0) * (log(1 + pow((X.x_pos - y[lTime].x_pos)/scale_y,2) / nu_y) + log(1 + pow((X.y_pos - y[lTime].y_pos)/scale_y,2) / nu_y));
//}
