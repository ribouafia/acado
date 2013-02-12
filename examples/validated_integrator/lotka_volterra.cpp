/*
 *    This file is part of ACADO Toolkit.
 *
 *    ACADO Toolkit -- A Toolkit for Automatic Control and Dynamic Optimization.
 *    Copyright (C) 2008-2009 by Boris Houska and Hans Joachim Ferreau, K.U.Leuven.
 *    Developed within the Optimization in Engineering Center (OPTEC) under
 *    supervision of Moritz Diehl. All rights reserved.
 *
 *    ACADO Toolkit is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 3 of the License, or (at your option) any later version.
 *
 *    ACADO Toolkit is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with ACADO Toolkit; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */



 /**
 *    \file examples/validated_integrator/harmonoic_oscillator.cpp
 *    \author Boris Houska, Mario Villanueva, Benoit Chachuat
 *    \date 2013
 */

#include <sys/stat.h>
#include <sys/time.h>
#include <acado/validated_integrator/ellipsoidal_integrator.hpp>

double myGetTime( )
{
	double current_time = 0.0;
	struct timeval theclock;
	gettimeofday( &theclock,0 );
	current_time = 1.0*theclock.tv_sec + 1.0e-6*theclock.tv_usec;

	return current_time;
}


USING_NAMESPACE_ACADO
    
typedef TaylorVariable<Interval> T;

/* >>> start tutorial code >>> */
int main( ){

    // DEFINE VARIABLES:
    // ----------------------
	DifferentialState      x,y;
	Parameter                p; 
	DifferentialEquation     f;

	f << dot(x) == p*x*(1.0-y);
	f << dot(y) == p*y*(x-1.0);

	TaylorModel<Interval> Mod( 1, 4 );
	
	Tmatrix<T> x_init(2);
	x_init(0) = 1.2;
	x_init(1) = 1.1;
	
	Tmatrix<T> p_init(1);
	p_init(0) = T( &Mod, 0, Interval(2.95,3.05));
	
	EllipsoidalIntegrator integrator( f, 5 );

	integrator.set(INTEGRATOR_PRINTLEVEL, HIGH );
	integrator.set(INTEGRATOR_TOLERANCE, 1e-6 );
	integrator.set(ABSOLUTE_TOLERANCE, 1e-6 );
	
	double t = -myGetTime();
	
	integrator.integrate( 0.0, 8.0, &x_init, &p_init );
//	integrator.step( 0.0, 1.0, &x_init, &p_init );
	
	t += myGetTime();
	
	printf("CPU time = %.16e \n", t );
	
	
    return 0;
}
/* <<< end tutorial code <<< */

