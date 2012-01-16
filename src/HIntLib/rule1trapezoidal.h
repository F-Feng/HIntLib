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
 *  Rule 1 Trapezoidal
 *
 *  Cubature rule of degree 1 with 2^n points.
 *  All points are inside the hypercube.
 *
 *  A product rule based on the one-dimensional 2-point trapezoidal rule
 *
 *  It is presented in
 *     A. H. Stoud. Approximate Calculation of Multiple Integrals (1971)
 *  as formula Cn: 1-2
 */

#ifndef RULE1TRAPEZOIDAL_H
#define RULE1TRAPEZOIDAL_H 1

#ifdef __GNUG__
#pragma interface
#endif

#include <HIntLib/cubaturerule.h>
#include <HIntLib/orbitrule.h>

namespace HIntLib
{
   class CubatureRuleFactory;

   class Rule1Trapezoidal : public CubatureRule, private OrbitRule
   {
   public:
      Rule1Trapezoidal (unsigned dim);

      virtual real eval (Function &, const Hypercube &);

      virtual unsigned getDimension()   const { return dim; }
      virtual Index getNumPoints ()     const { return numR_Rfs(); }
      virtual unsigned getDegree ()     const { return 1; }
      virtual bool isAllPointsInside () const { return true; }
      virtual real getSumAbsWeight ()   const { return 1.0; }

      static CubatureRuleFactory* getFactory();
 
   private:
      Array<real> a;
      const real oneDivTwoPowDim;
   };

}  // namespace HIntLib

#endif

