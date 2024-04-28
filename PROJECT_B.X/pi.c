#include <xc.h>
#include <stdlib.h>
#include <stdint.h>
#include "pi.h"


/*
 * Contém código não original
 *
 *Código base para a construção do controlador deriva de código disponibilizado num video
 *sobre controlador PID
 * Autor:https://youtu.be/zOByx3Izf5U?si=p1CvpppLJgU8kiIW
 *
 */


void PI_Init(PI *pi){
	/* Set the limits */
	pi->minLimit = 45.0f;
	pi->maxLimit = 95.0f;

	/* Clear the controller variables */
	pi->integrator 		= 0;
	pi->prevError 			= 0;
	pi->prevMeasurement 	= 0;

	pi->out = 0;
}


float PI_Update(PI *pi, float setpoint, float measurement){

	/* Error signal (e) */
	float error = setpoint - measurement;	// Desired Setpoint - Measure Value

	/* Proportional term (P) */
	float proportional = pi->kp * error;

	/* Integral term (I) */
	pi->integrator += 0.5f * pi->ki * pi->Ts * (error + pi->prevError);
	
	/* --- Anti-wind-up --- */
	float minLimitInt, maxLimitInt;

	if (pi->maxLimit > proportional){
		maxLimitInt = pi->maxLimit - proportional;
	}
	else{
		maxLimitInt = 0.0f;
	}

	if (pi->minLimit < proportional){
		minLimitInt = pi->minLimit - proportional;
	}
	else{
		minLimitInt = 0.0f;
	}

	/* --- Clamp integrator ---*/
	/* Avoids integrator wind-up, which means
	 * that the integrator will not be allowed to
	 * grow indefinitely. */
	if (pi->integrator > maxLimitInt){
		pi->integrator = maxLimitInt;
	}
	else if (pi->integrator < minLimitInt){
		pi->integrator = minLimitInt;
	}

	/* Calculate the controller output */
	pi->out = proportional + pi->integrator;

	/* Limit Output */
	if(pi->out > pi->maxLimit){
		pi->out = pi->maxLimit;
	}
	else if(pi->out < pi->minLimit){
		pi->out = pi->minLimit;
	}

	/* Update previous error and measurement */
	pi->prevError = error;
	pi->prevMeasurement = measurement;

	return pi->out;
}