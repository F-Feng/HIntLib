/*
 *  HIntLib  -  Library for High-dimensional Numerical Integration 
 *
 *  Copyright (C) 2002  Rudolf Sch�rer <rudolf.schuerer@sbg.ac.at>
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

/**
 *  rule7phillips.cpp
 *
 *  Cubature rule of degree 7
 *  All points are inside the hypercube.
 *
 *  This rule was published in
 *     G.M.Phillips:
 *     Numerical Integration over an N-Dimensional Rectangular Region
 *     The Computer Journal 10 (1967), 297-299.
 *
 *  It is also presented in
 *     A. H. Stoud. Approximate Calculation of Multiple Integrals (1971)
 *  as formula Cn: 7-1
 */                                                                             

#ifdef __GNUG__
#pragma implementation
#endif

#include <HIntLib/rule7phillips.h>

#include <HIntLib/defaultcubaturerulefactory.h>
#include <HIntLib/mymath.h>
#include <HIntLib/exception.h>


namespace L = HIntLib;
using L::real;
using L::Index;

namespace
{
   #if HINTLIB_STATIC_WORKS == 1
      const real rCD = sqrt (3.0 / 5.0);
   #else
      real rCD;
   #endif

   const real weightD = 125.0 / 5832.0;
}


/**
 *  The constructor is used primarily to initialize all the dimension dependent
 *  constatns and to allocate (dimension dependent) memory
 */

L::Rule7Phillips::Rule7Phillips (unsigned dim)
: OrbitRule (dim),
  aCD (dim),

  eN (((25 * signed (dim) - 165) * signed (dim) + 302) / 972.0),
  temp (14.0 * eN + 1.0),

  rB2 (sqrt ((21.0 * eN - 1.0) / (35.0 * eN))),

  weightB1 (eN / temp),
  weightB2 (490.0 * cube (eN) / temp / (21.0 * eN - 1.0)),
  weightC  ((475 - 125 * signed (dim)) / 2916.0),
  weightA  (1.0 + dim * (  125.0/2187.0 * (dim-1) * (dim-4.7)
                         - 2.0 * (weightB1 + weightB2)))
{
   checkDimensionNotZero (dim);

   if (dim < 5 && dim != 2)  throw InvalidDimension (dim);

   #if HINTLIB_STATIC_WORKS == 0
      rCD = sqrt (3.0 / 5.0);
   #endif
}


Index L::Rule7Phillips::getNumPoints () const
{
   return numRRR0_0fs () + numRR0_0fs () + 2 * numR0_0fs () + num0_0 ();
}


real L::Rule7Phillips::getSumAbsWeight() const
{
   // Multiply each weight with the number of sampling points it is used for.
   // Don't forget to take the absolute value for weights that meight be
   // negative

   return numRRR0_0fs () * weightD
        + numRR0_0fs  () * abs (weightC)
        + numR0_0fs   () * (abs (weightB1) + abs (weightB2))
        + num0_0      () * abs (weightA);
}
 

/**
 *  Do the actual function evaluation
 */

real L::Rule7Phillips::eval (Function &f, const Hypercube &h)
{
   // Calculate offsets from the center for points A and B

   const real* width = h.getWidth();
   for (unsigned i = 0; i < dim; ++i)  aCD [i] = width [i] * rCD;

   // Set up a scaler for point C

   Scaler scalerB2 (width, rB2);

   // Evaluate integrand

   const real* c = h.getCenter();
   setCenter (c);

   return h.getVolume() *
   (
        weightD  * evalRRR0_0fs (f, c, aCD)
      + weightC  * evalRR0_0fs  (f, c, aCD)
      + weightB1 * evalR0_0fs   (f, c, width)     // rB1 = 1.0
      + weightB2 * evalR0_0fs   (f, c, scalerB2)
      + weightA  * eval0_0      (f)
   ); 
}

/**
 *  getFactory()
 */

L::CubatureRuleFactory* L::Rule7Phillips::getFactory()
{
   return new DefaultCubatureRuleFactory<L::Rule7Phillips> ();
}

