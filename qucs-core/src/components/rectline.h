/*
 * rectline.h - rectangular waveguide class definitions
 *
 * Copyright (C) 2008 Bastien ROUCARIES <roucaries.bastien@gmail.com>
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this package; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street - Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *
 */

#ifndef __RECTLINE_H__
#define __RECTLINE_H__

#include "circuit.h"
/*! A TE10 rectangular waveguide component
    \note Evanecent mode are not implemented
    \todo Implement evanecent mode
*/
class rectline : public qucs::circuit
{
 public:
  CREATOR (rectline);
  void initSP (void);
  void calcSP (nr_double_t);
  void calcNoiseSP (nr_double_t);
  void initDC (void);
  void initAC (void);
  void calcAC (nr_double_t);
  void calcNoiseAC (nr_double_t);
  void saveCharacteristics (nr_complex_t);

 private:
  void calcPropagation (nr_double_t);
  void initCheck (void);
  void calcResistivity (const char * const, nr_double_t);
  /*! attenuation constant */
  nr_double_t alpha;
  /*! propagation constant */
  nr_double_t beta;
  /*! wave impedance */
  nr_complex_t zl;
  /*! cut off frequency lower bound */
  nr_double_t fc_low;
  /*! cut off frequency higher mode */
  nr_double_t fc_high;
  /*! resistivity */
  nr_double_t rho;
};

#endif /* __RECTLINE_H__ */
