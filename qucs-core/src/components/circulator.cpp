/*
 * circulator.cpp - circulator class implementation
 *
 * Copyright (C) 2003, 2004 Stefan Jahn <stefan@lkcc.org>
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
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.  
 *
 * $Id: circulator.cpp,v 1.10 2005/05/02 06:51:00 raimi Exp $
 *
 */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "complex.h"
#include "object.h"
#include "node.h"
#include "circuit.h"
#include "component_id.h"
#include "circulator.h"

circulator::circulator () : circuit (3) {
  type = CIR_CIRCULATOR;
  setVoltageSources (3);
}

void circulator::calcSP (nr_double_t) {
  nr_double_t z1 = getPropertyDouble ("Z1");
  nr_double_t z2 = getPropertyDouble ("Z2");
  nr_double_t z3 = getPropertyDouble ("Z3");
  nr_double_t r1 = (z0 - z1) / (z0 + z1);
  nr_double_t r2 = (z0 - z2) / (z0 + z2);
  nr_double_t r3 = (z0 - z3) / (z0 + z3);
  nr_double_t d  = 1 - r1 * r2 * r3;
  setS (NODE_1, NODE_1, (r2 * r3 - r1) / d);
  setS (NODE_2, NODE_2, (r1 * r3 - r2) / d);
  setS (NODE_3, NODE_3, (r1 * r2 - r3) / d);
  setS (NODE_1, NODE_2, sqrt (z2 / z1) * (z1 + z0) /
	(z2 + z0) * r3 * (1 - r1 * r1) / d);
  setS (NODE_2, NODE_3, sqrt (z3 / z2) * (z2 + z0) /
	(z3 + z0) * r1 * (1 - r2 * r2) / d);
  setS (NODE_3, NODE_1, sqrt (z1 / z3) * (z3 + z0) /
	(z1 + z0) * r2 * (1 - r3 * r3) / d);
  setS (NODE_2, NODE_1, sqrt (z1 / z2) * (z2 + z0) /
	(z1 + z0) * (1 - r2 * r2) / d);
  setS (NODE_1, NODE_3, sqrt (z3 / z1) * (z1 + z0) /
	(z3 + z0) * (1 - r1 * r1) / d);
  setS (NODE_3, NODE_2, sqrt (z2 / z3) * (z3 + z0) /
	(z2 + z0) * (1 - r3 * r3) / d);
}

void circulator::initDC (void) {
  nr_double_t z1 = getPropertyDouble ("Z1");
  nr_double_t z2 = getPropertyDouble ("Z2");
  nr_double_t z3 = getPropertyDouble ("Z3");
  nr_double_t r1 = (z0 - z1) / (z0 + z1);
  nr_double_t r2 = (z0 - z2) / (z0 + z2);
  nr_double_t r3 = (z0 - z3) / (z0 + z3);
  nr_double_t d  = 1 - r1 * r2 * r3;

  nr_double_t s11 = (r2 * r3 - r1) / d;
  nr_double_t s22 = (r1 * r3 - r2) / d;
  nr_double_t s33 = (r1 * r2 - r3) / d;
  nr_double_t s12 = sqrt (z2/z1) * (z1+z0) / (z2+z0) * r3 * (1-r1*r1) / d;
  nr_double_t s23 = sqrt (z3/z2) * (z2+z0) / (z3+z0) * r1 * (1-r2*r2) / d;
  nr_double_t s31 = sqrt (z1/z3) * (z3+z0) / (z1+z0) * r2 * (1-r3*r3) / d;
  nr_double_t s21 = sqrt (z1/z2) * (z2+z0) / (z1+z0) * (1-r2*r2) / d;
  nr_double_t s13 = sqrt (z3/z1) * (z1+z0) / (z3+z0) * (1-r1*r1) / d;
  nr_double_t s32 = sqrt (z2/z3) * (z3+z0) / (z2+z0) * (1-r3*r3) / d;

  allocMatrixMNA ();

  setB (NODE_1, VSRC_1, +1.0); setB (NODE_1, VSRC_2, +0.0);
  setB (NODE_1, VSRC_3, +0.0);
  setB (NODE_2, VSRC_1, +0.0); setB (NODE_2, VSRC_2, +1.0);
  setB (NODE_2, VSRC_3, +0.0);
  setB (NODE_3, VSRC_1, +0.0); setB (NODE_3, VSRC_2, +0.0);
  setB (NODE_3, VSRC_3, +1.0);

  setC (VSRC_1, NODE_1, s11 - 1.0); setC (VSRC_1, NODE_2, s12);
  setC (VSRC_1, NODE_3, s13);
  setC (VSRC_2, NODE_1, s21); setC (VSRC_2, NODE_2, s22 - 1.0);
  setC (VSRC_2, NODE_3, s23);
  setC (VSRC_3, NODE_1, s31); setC (VSRC_3, NODE_2, s32);
  setC (VSRC_3, NODE_3, s33 - 1.0);

  setD (VSRC_1, VSRC_1, z0 * (s11 + 1.0)); setD (VSRC_1, VSRC_2, z0 * s12);
  setD (VSRC_1, VSRC_3, z0 * s13);
  setD (VSRC_2, VSRC_1, z0 * s21); setD (VSRC_2, VSRC_2, z0 * (s22 + 1.0));
  setD (VSRC_2, VSRC_3, z0 * s23);
  setD (VSRC_3, VSRC_1, z0 * s31); setD (VSRC_3, VSRC_2, z0 * s32);
  setD (VSRC_3, VSRC_3, z0 * (s33 + 1.0));

  setE (VSRC_1, +0.0); setE (VSRC_2, +0.0); setE (VSRC_3, +0.0);
}

void circulator::initAC (void) {
  initDC ();
}

void circulator::initTR (void) {
  initDC ();
}
