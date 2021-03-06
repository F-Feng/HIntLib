/*
 *  HIntLib  -  Library for High-dimensional Numerical Integration 
 *
 *  Copyright (C) 2002,03,04,05  Rudolf Schuerer <rudolf.schuerer@sbg.ac.at>
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

#ifndef HINTLIB_GF2_VECTORSPACE_H
#define HINTLIB_GF2_VECTORSPACE_H 1

#include <HIntLib/defaults.h>

#ifdef HINTLIB_USE_INTERFACE_IMPLEMENTATION
#pragma interface
// Implementation in polynomial2base.cpp
#endif

#include <HIntLib/gf2.h>
#include <HIntLib/bitop.h>
#include <HIntLib/exception.h>


namespace HIntLib
{

namespace Private
{

/**
 *  GF2VectorSpace Base
 */

class GF2VectorSpaceBase
{
public:
   GF2 getScalarAlgebra() const  { return GF2(); }

   unsigned size() const  { return 1u << dim; }
   int dimension() const  { return dim; }

   void printSuffix (std::ostream &o) const { GF2().printSuffix (o); }
#ifdef HINTLIB_BUILD_WCHAR
   void printSuffix (std::wostream &o) const { GF2().printSuffix (o); }
#endif

protected:
   GF2VectorSpaceBase (int _dim) : dim(_dim) {}

   const int dim;
};

std::ostream& operator<< (std::ostream&, const GF2VectorSpaceBase&);
#ifdef HINTLIB_BUILD_WCHAR
std::wostream& operator<< (std::wostream&, const GF2VectorSpaceBase&);
#endif

}  // namespace Private


/**
 *  GF2VectorSpace
 */

template<typename T>
class GF2VectorSpace : public Private::GF2VectorSpaceBase,
                       public BitOpBasedAddition<T>
{
public:
   typedef vectorspace_tag algebra_category;
   typedef nopolynomial_tag polynomial_category;
   typedef finite_tag size_category;

   typedef GF2 scalar_algebra;
   typedef GF2::type scalar_type;
   typedef T type;
   typedef BitRef<T> scalar_reference;

   GF2VectorSpace (int _dim) : Private::GF2VectorSpaceBase (_dim)
   {
      if (dim < 1 || dim > std::numeric_limits<T>::digits)
      {
         throw FIXME (__FILE__, __LINE__);
      }
   } 

   template<typename I> void toCoord (type a, I p) const
   {
      for (int i = 0; i != dim; ++i,a>>=1)  *p++ = scalar_type (a & 1);
   }
   template<typename I> void fromCoord (type& a, I p) const
   {
      a = 0;
      p += dim;
      for (int i = 0; i != dim; ++i) a = (a << 1) | (*--p);
   }

   scalar_type coord (const type& a, int k) const
      { return (a >> k) & 1; }
   scalar_reference coord (type& a, int k) const
      { return BitRef<T> (&a, k); }
   
   type element(unsigned i) const  { return type(i); }
   unsigned index (type x) const   { return x; }

   type mul   (const type& a, scalar_type l) const  { return l ? a : 0; }
   void scale (      type& a, scalar_type l) const  {    a = l ? a : 0; }

   void print (std::ostream&, const type&) const;
   void printShort (std::ostream&, const type&) const;
   void printShort (std::ostream& o, const type& a, PrintShortFlag) const
      { printShort (o, a); }
#ifdef HINTLIB_BUILD_WCHAR
   void print (std::wostream&, const type&) const;
   void printShort (std::wostream&, const type&) const;
   void printShort (std::wostream& o, const type& a, PrintShortFlag) const
      { printShort (o, a); }
#endif

   HINTLIB_TRIVIAL_DOMAIN_MEMBERS
};

} // namespace HIntLib

#endif

