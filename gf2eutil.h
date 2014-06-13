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
#ifndef __GF2EUTIL_H__
#define __GF2EUTIL_H__

#include <NTL/GF2E.h>
#include <NTL/ZZ.h>

void convGF2EToZZ(NTL::ZZ& n, NTL::GF2E& e);
void convZZToGF2E(NTL::GF2E& e, NTL::ZZ& n);

long convGF2EToLong(NTL::GF2E& e);
NTL::GF2E convLongToGF2E(long n);

long convGF2EToIndex(NTL::GF2E& e);
NTL::GF2E convIndexToGF2E(long n);

#endif

