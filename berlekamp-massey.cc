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
#include "berlekamp-massey.h"

bool check_distinct_roots(const NTL::GF2EX& e) {
	NTL::GF2EXModulus m;
	NTL::GF2EX h;

	if (NTL::IsZero(e))
        return false;

	if (NTL::deg(e) == 0 || NTL::deg(e) == 1)
        return true;

	NTL::build(m, e);  	
	NTL::FrobeniusMap (h, m);

	return NTL::IsX(h);
}


bool forney_error_magnitudes(NTL::GF2EX& error_out, NTL::GF2EX& lambda, NTL::vec_GF2E& syndromes) {
	NTL::GF2EX S, omega, error, dlambda;
	NTL::vec_GF2E roots;

	if (!check_distinct_roots(lambda))
		return false;

	NTL::SetCoeff(S, 0);
	for (unsigned long i = 0; i < syndromes.length(); i++) {
		NTL::SetCoeff(S, i + 1, syndromes[i]);
	}

	omega = NTL::trunc(lambda * S, syndromes.length() + 1);
	NTL::diff(dlambda, lambda);

	NTL::MakeMonic(lambda);
	NTL::FindRoots(roots, lambda);
	for (unsigned long i = 0; i < roots.length(); i++) {
		NTL::GF2E invX = roots[i];
		roots[i] = NTL::inv(roots[i]);

		NTL::GF2E e = -(roots[i] * NTL::eval(omega, invX)) / NTL::eval(dlambda, invX);
		NTL::SetCoeff(error, convGF2EToIndex(roots[i]), e);
	}
	error_out = error;
	return true;
}

void berlekamp_massey(NTL::GF2EX& lambda_out, NTL::vec_GF2E& syndromes) {
	unsigned long i, j, k;
	NTL::GF2EX lambda, t;
	NTL::GF2E delta;

	NTL::SetCoeff(lambda, 0);
	NTL::SetCoeff(t, 1);

	for (k = 0, i = 1; i <= syndromes.length(); i++) {
		for (delta = 0, j = 1; j <= k; j++) {
			delta += NTL::coeff(lambda, j) * syndromes[i - j - 1];
		}
		delta = syndromes[i - 1] - delta;

		if (delta != 0) {
			lambda = lambda - (delta * t);
			if (2 * k < i) {
				t = t + lambda / delta;
				k = i - k;
			}
		}
		t <<= 1;
	}
	lambda_out = lambda;
}

