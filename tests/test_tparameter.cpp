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

#include <iostream>
#include <iomanip>
#include <fstream>
#include <memory>

#include <HIntLib/defaults.h>

#ifdef HINTLIB_HAVE_CSTDLIB
#  include <cstdlib>
#  define HINTLIB_SLN std::
#else
#  include <stdlib.h>
#  define HINTLIB_SLN
#endif

#include <HIntLib/make.h>
#include <HIntLib/tparameter.h>
#include <HIntLib/generatormatrixgen.h>
#include <HIntLib/generatormatrixvirtual.h>
#include <HIntLib/array.h>
#include <HIntLib/hlalgorithm.h>

#include "test.h"

using namespace std;
using namespace HIntLib;

// Option processing

const char options[] = "erbm:d:t";
const char option_msg[] =
   "  net    # of the GeneratorMatrix or filename of libseq-format matrix\n"
   "  -e     Add equidistributed coordinate\n"
   "  -r     Test restricted t parameter\n"
   "  -b     Do tests without optimized bounds\n"
   "  -m m_max\n"
   "  -m m_min-m_max\n"
   "         Range for m (default: 1 - 25)\n"
   "  -d d_max\n"
   "  -d d_min-d_max\n"
   "         Range for the dimension (default: 1 - 55)\n"
   "  -k     Print strength instead of t-parameter\n";
const char testProgramParameters[] = "[OPTIONS] net";
const char testProgramUsage[] =
   "Tests whether determining the t parameter works correctly.\n\n";
const char testProgramName[] = "test_tparameter";
const int  testProgramCopyright = 2003;

bool ADD_EQUI = false;
bool RESTRICTED = false;
bool NO_BOUNDS = false;
bool PRINT_STRENGTH = false;
int MIN_M = 1;
int MIN_S = 1;
int MAX_S = 55;
int MAX_M = 25;

bool opt(int c, const char* s)
{
   switch (c)
   {
   case 'e':  ADD_EQUI = true; return true;
   case 'r':  RESTRICTED = true; return true;
   case 'b':  NO_BOUNDS = true; return true;
   case 'm':  parseRange (s, MIN_M, MIN_M, MAX_M); return true;
   case 'd':  parseRange (s, MIN_S, MIN_S, MAX_S); return true;
   case 'k':  PRINT_STRENGTH = true; return true;
   }

   return false;
}


// Other global names

typedef GeneratorMatrixGen<unsigned char> Matrix;


/**
 *  determineT ()
 */

void determineT (
      int s, int m, const Matrix& matrix, int* t_matrix,
      const Matrix* lowDimMatrix)
{
   int ub = m;  // trivial upper bound

   // t can only increase by one compared to m-1
   // With some thought, you see that it also works for the
   // equidistributed case.

   if (t_matrix [(m-1) * (MAX_S + 1) + s] >= 0)
   {
      ub = t_matrix [(m-1) * (MAX_S + 1) + s] + 1;
   }

   int lb = 0;  // trivial lower bound
   TOption opts = DEFAULT;

   // Do we know t for a lower-dimensional sub-matrix?

   if (lowDimMatrix && lowDimMatrix->getM() >= m
                    && lowDimMatrix->getPrec() >=m)
   {
      if (   DiscardDimensions (s - 1 - ADD_EQUI, AdjustM (m,
                  AdjustPrec (m, *lowDimMatrix)))
          == DiscardDimensions (s - 1 - ADD_EQUI, AdjustM (m,
                  AdjustPrec (m, matrix))))
      {
         // t cannot decrease compared to s-1

         lb = std::max (lb, t_matrix [m * (MAX_S + 1) + (s-1)]);
         opts = LOWER_DIM_OK;
      }
   }

   Matrix g (DiscardDimensions (s, NetFromSequence (m, ADD_EQUI,
                           AdjustPrec (m, matrix))));

   int t = t_matrix [m * (MAX_S + 1) + s] = tParameter (g, lb, ub, opts);

   if (NO_BOUNDS)
   {
      if (t != tParameter (g, 0, m))  error ("Bound optimization broken");
   }

   // check if restricted results make sense

   if (RESTRICTED)
   {
      Array<int> t_restricted (MAX_M + 2);

      for (int i = 0; i <= m; ++i)
      {
          t_restricted [i] = tParameterRestricted (g, 0, m, i, DEFAULT);
      }

      if (t_restricted[0] != 0)  error ("t_res(0) != 0");
      if (t_restricted[m] != t)  error ("t_res(m) != t");
      for (int i = 0; i < m; ++i)
      {
         if (t_restricted[i] > t_restricted[i+1])
         {
            error ("t_res(i) > t_res(i+1)");
         }
      }

      NORMAL
      {
         cout << setw(3) << s << setw(3) << m << setw(4) << t;
         for (int i = 0; i <= m; ++i)
         {
            cout << setw(3) << t_restricted[i];
         }
         cout << endl;
      }
   }
   else
   {
      NORMAL  cout << setw(3) << (PRINT_STRENGTH ? m - t : t) << flush;
   }
}


/**
 *  Main program
 */

void test (int argc, char** argv)
{
   if (argc != 1)  usage("Invalid number of arguments!");

   NORMAL printHeader (cout);

   char* endptr; 
   int matrixNumber = HINTLIB_SLN strtol (argv[0], &endptr, 10);

   Array<Matrix*> matrices (MAX_S + 1, 0);    // The fullsized matrix for each s
   Array<int> t_matrix ((MAX_S + 1) * (MAX_M + 1), -1);

   if (endptr != argv[0] && *endptr == '\0')
   { 
      NORMAL cout << Make::getGeneratorMatrixGenName (matrixNumber) << "\n\n";
      NORMAL if (! RESTRICTED)  cout << "   s=";

      for (int s = MIN_S; s <= MAX_S; ++s)
      {
         try
         {
            matrices [s]
               = Make::generatorMatrixGen (matrixNumber, s - ADD_EQUI, MAX_M);
            NORMAL if (! RESTRICTED)  cout << setw(3) << s << flush;
         }
         catch (InvalidDimension &)
         {
            NORMAL if (! RESTRICTED)  cout << setw(3) << "";
         }
      }
      NORMAL
      {
         if (! RESTRICTED)  cout << "\n";
         else cout << setw(3) << "s" << setw(3) << "m" << setw(4) << "t"
                   << endl;
      }
   }
   else   // reading matrix from file
   { 
      std::ifstream ifs (argv[0]);
      std::auto_ptr<Matrix> gm (loadLibSeq (ifs));
      NORMAL cout << "Matrix from file '" << argv[0] << "'\n\n";
      NORMAL if (! RESTRICTED)  cout << "   s=";

      for (int s = MIN_S; s <= MAX_S; ++s)
      {
         int ss = s - ADD_EQUI;

         if (ss <= gm->getDimension())
         {
            matrices [s] = new Matrix (DiscardDimensions (ss, *gm));
            NORMAL if (! RESTRICTED)  cout << setw(3) << s << flush;
         }
         else
         {
            NORMAL if (! RESTRICTED)  cout << setw(3) << "";
         }
      }
      NORMAL
      {
         if (! RESTRICTED)  cout << "\n";
         else cout << setw(3) << "s" << setw(3) << "m" << setw(4) << "t"
                   << endl;
      }
   }

   for (int m = MIN_M; m <= MAX_M; ++m)
   {
      NORMAL if (! RESTRICTED)  cout << "m=" << setw(2) << m << ' ';

      for (int s = MIN_S; s <= MAX_S; ++s)
      {
         Matrix* matrix = matrices [s];

         if (matrix && matrix->getM() >= m && matrix->getPrec() >= m)
         {
            determineT (s, m, *matrix, t_matrix, matrices[s-1]);
         }
         else
         {
            NORMAL  if (! RESTRICTED)  cout << setw(3) << "";
         }
      }
      NORMAL  if (! RESTRICTED)  cout << endl;
   }

   purge (&matrices[0], &matrices[MAX_S + 1]);
}

