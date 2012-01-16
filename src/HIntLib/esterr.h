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


#ifndef ESTERR_H
#define ESTERR_H 1
 
#ifdef __GNUG__
#pragma interface
#endif


#include <algorithm>
#include <iosfwd>

#include <HIntLib/mymath.h>
#ifdef PARALLEL
   #include <HIntLib/buffer.h>
#endif


namespace HIntLib
{

class EstErr
{
  real est;
  real err;

public:

  EstErr () : est (0), err (0) {}
  EstErr (real newEst, real newErr)
     : est (newEst), err (std::max (newErr, real())) {}
#ifdef PARALLEL
  EstErr (RecvBuffer &b)  {  b >> *this; }
#endif
 
  real getEstimate() const  { return est; }
  real getError()    const  { return err; }
  real getRelError() const;

  void set (real newEst, real newErr) { est = newEst; err = abs (newErr); }
  void setNoErr (real newEst)         { est = newEst; err = -1.0; }

  EstErr& operator+= (const EstErr &ee);
  EstErr& operator-= (const EstErr &ee);

  void scale (real a)  { est *= a; err *= a; }

#ifdef PARALLEL
   friend SendBuffer& operator<< (SendBuffer &, const EstErr &);
   friend RecvBuffer& operator>> (RecvBuffer &, EstErr &); 
   MPI_Datatype getMPIDatatype () const;
   void initAfterReceive() const {}
#endif
};

std::ostream& operator<< (std::ostream &o, const EstErr &ee);


/********** Implementation *********************************/

inline
real EstErr::getRelError() const
{
   // Don't divide by 0 on the cray

   #ifdef CRAY
      return (est == 0.0) ? REAL_MAX : abs (err / est);
   #else
      return abs (err / est);
   #endif
}

inline
EstErr operator+ (const EstErr &ee1, const EstErr &ee2) 
{
   return EstErr (ee1.getEstimate () + ee2.getEstimate (),
                  ee1.getError () +    ee2.getError ());
}

inline
EstErr operator- (const EstErr &ee1, const EstErr &ee2)
{
   return EstErr (ee1.getEstimate () - ee2.getEstimate (),
                  ee1.getError ()    - ee2.getError ());
}

inline
EstErr& EstErr::operator+= (const EstErr &ee)
{
   est += ee.est;
   err += ee.err;

   return *this;
}

inline
EstErr& EstErr::operator-= (const EstErr &ee)
{
   est -= ee.est;

   if (err > ee.err)
      err -= ee.err;
   else
      err = 0.0;
 
   return *this;
}

#ifdef PARALLEL

inline SendBuffer& operator<< (SendBuffer &b, const EstErr &ee)
{
   b.pack (&ee.est, 2, MPIType<real>::type);
   return b;
}

inline RecvBuffer& operator>> (RecvBuffer &b, EstErr &ee)
{
   b.unpack (&ee.est, 2, MPIType<real>::type);
   ee.initAfterReceive();
   return b;
}

#endif // PARALLEL

}  // namespace HIntLib

#endif
