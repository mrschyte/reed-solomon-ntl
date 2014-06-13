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
#include "gf2eutil.h"

void convGF2EToZZ(NTL::ZZ& n, NTL::GF2E& e) {
	NTL::GF2X g = NTL::rep(e);
	long bytes;
	
	bytes = NTL::NumBytes(g);
	unsigned char* buffer = new unsigned char[bytes];

	BytesFromGF2X(buffer, g, bytes);
	ZZFromBytes(n, buffer, bytes);

	delete[] buffer;
}

void convZZToGF2E(NTL::GF2E& e, NTL::ZZ& n) {
	NTL::GF2X g;
	long bytes;
	
	bytes = NTL::NumBytes(n);
	unsigned char* buffer = new unsigned char[bytes];

	NTL::BytesFromZZ(buffer, n, bytes);
	NTL::GF2XFromBytes(g, buffer, bytes);

	delete[] buffer;
	NTL::conv(e, g);
}

long convGF2EToLong(NTL::GF2E& e) {
	NTL::ZZ z;
	long res;

	convGF2EToZZ(z, e);
	NTL::conv(res, z);
	return res;
}

NTL::GF2E convLongToGF2E(long n) {
	NTL::GF2E e;
	NTL::ZZ z;
	z = n;

	convZZToGF2E(e, z);
	return e;
}

NTL::GF2E convIndexToGF2E(long n) {
	NTL::GF2E alpha, power;

	alpha = convLongToGF2E(1);
	power = convLongToGF2E(2);

	for (long i = 0; i < n; i++) {
		alpha = alpha * power;
	}

	return alpha;
}

long convGF2EToIndex(NTL::GF2E& e) {
	NTL::GF2E alpha, power;
	long i;

	if (e == 0)
		return -1;

	alpha = convLongToGF2E(1);
	power = convLongToGF2E(2);

	for (i = 0; alpha != e && i < NTL::GF2E::cardinality(); i++) {
		alpha = alpha * power;
	}

	return i;
}

