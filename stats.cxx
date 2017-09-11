// Sebastian PUerta Hincapie
// Statistician Class

#include <iostream>
#include <cassert>
#include "stats.h"

namespace main_savitch_2C {

	// CONSTRUCTOR
	statistician::statistician(){reset();}
	// MODIFICATION MEMBER FUNCTIONS
	void statistician::next(double r){
		if(count == 0){
			tinyest = r;
			largest = r;
		}
		else if(r < tinyest && largest == 0){
			largest = tinyest;
			tinyest = r;
		}
		else if(r < tinyest){
			tinyest = r;
		}
		else if(r > largest){
			largest = r;
		}
		total += r;
		++count;
	}
	void statistician::reset( ){
		count = 0;
		total = 0;
		tinyest = 0;
		largest = 0;
	}
	// CONSTANT MEMBER FUNCTIONS
	int statistician::length( ) const { return count;}
	double statistician::sum( ) const { return total;}
	double statistician::mean( ) const {
		return total / count;
	}
	double statistician::minimum( ) const {
		assert(length() != 0);
		return tinyest;}
	double statistician::maximum( ) const {
		assert(length() != 0);
		return largest;
	}
    // FRIEND FUNCTIONS
    statistician operator+(const statistician& s1, const statistician& s2){
    	statistician s3;
    	s3.count = s1.count + s2.count;
    	s3.total = s1.total + s2.total;

    	// min check
    	if(s1.length() == 0){
    		s3.tinyest = s2.tinyest;
    		s3.largest = s2.largest;
    	}
    	else if(s2.length() == 0){
    		s3.tinyest = s1.tinyest;
    		s3.largest = s1.largest;
    	}
    	else{
			if(s1.tinyest > s2.tinyest){
				s3.tinyest = s2.tinyest;
			}
			else if(s2.tinyest > s1.tinyest){
				s3.tinyest = s1.tinyest;
			}
			else if(s1.tinyest == s2.tinyest){
				s3.tinyest = s1.tinyest; // or s2.tinyest
			}
			// max check
			if(s1.largest > s2.largest){
				s3.largest = s1.largest;
			}
			else if(s2.largest > s1.largest){
				s3.largest = s2.largest;
			}
			else if(s1.largest == s2.largest){
				s3.largest = s1.largest; // or s2.largest
			}
    	}

    	return s3;
    }
    statistician operator*(double scale, const statistician& s){
    	statistician s3;
    	s3.count = s.count;
    	s3.total = s.total * scale;
    	if(scale < 0){
    		s3.tinyest = s.largest * scale;
    		s3.largest = s.tinyest * scale;
    	}
    	else {
			s3.tinyest = s.tinyest * scale;
			s3.largest = s.largest * scale;
    	}
    	return s3;
    }
    // NON-MEMBER functions for the statistician class
    bool operator ==(const statistician& s1, const statistician& s2){
    	if(s1.sum() == 0 && s2.sum() == 0 && s1.length() == 0 && s2.length() == 0){
    		return true;
    	}
    	else if(s1.sum() == s2.sum() && s1.length() == s2.length()
    			&& s1.maximum() == s2.maximum() && s1.minimum() == s2.minimum()){
    		return true;
    	}
    	return false;
    }
}




