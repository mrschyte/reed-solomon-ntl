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
#ifndef __BERLEKAMP_MASSEY_H__
#define __BERLEKAMP_MASSEY_H__

#include <NTL/GF2XFactoring.h>
#include <NTL/GF2EXFactoring.h>
#include "gf2eutil.h"

bool forney_error_magnitudes(NTL::GF2EX& error, NTL::GF2EX& lambda, NTL::vec_GF2E& syndromes);
void berlekamp_massey(NTL::GF2EX& lambda, NTL::vec_GF2E& syndromes);

#endif

