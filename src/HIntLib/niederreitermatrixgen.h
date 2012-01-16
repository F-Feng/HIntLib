/*
 *  HIntLib  -  Library for High-dimensional Numerical Integration 
 *
 *  Copyright (C) 2002,03,04,05  Rudolf Sch�rer <rudolf.schuerer@sbg.ac.at>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#ifndef HINTLIB_NIEDERREITER_MATRIX_GEN_H
#define HINTLIB_NIEDERREITER_MATRIX_GEN_H 1

#ifdef __GNUG__
#pragma interface
#endif

namespace HIntLib
{
   template<typename T> class GeneratorMatrixGen;
   template<class T>    class PolynomialRing;


   template<class A>
   void initNiederreiter (GeneratorMatrixGen<typename A::type> &, A);

   template<class A>
   void initNiederreiter
        (GeneratorMatrixGen<typename A::type> &, A, unsigned d,
            const typename PolynomialRing<A>::type &);

   void initNiederreiter (GeneratorMatrixGen<unsigned char> &);

}   // namespace HIntLib

#endif


