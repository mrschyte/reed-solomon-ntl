/*
 *  This file is part of reed-solomon-ntl
 *  Copyright (C) 2009   Peter Kasza
 *
 *  reed-solomon-ntl is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  reed-solomon-ntl is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with reed-solomon-ntl.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __RSENCODER_H__
#define __RSENCODER_H__

#include "berlekamp-massey.h"
#include "rsexception.h"
#include "gf2eutil.h"
#include <vector>

class RSEncoder {
private:
	NTL::GF2EX generator; // generator polynomial
	long parityLength;

public:
	// n specifies extension field exponent ie. GF(2^n)
	// k specifies the number of parity bits
	RSEncoder(long n, long k);
	~RSEncoder();

	void encode(const std::vector<unsigned char>& message, std::vector<unsigned char>& buffer);
	void decode(const std::vector<unsigned char>& message, std::vector<unsigned char>& buffer);
	bool calculateSyndromes(NTL::GF2EX& code, NTL::vec_GF2E& syndromes);
	void buildGeneratorPoly();

public:
	long getParityLength();
	long maxLength();
	long maxErrors();
};

#endif

