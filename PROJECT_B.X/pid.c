#include <xc.h>
#include <stdlib.h>
#include <stdint.h>
#include "pid.h"

void PID_Init(PID *pid){

	/* Clear the controller variables */
	pid->integrator 		= 0;
	pid->prevError 			= 0;
	pid->diff 				= 0;
	pid->prevMeasurement 	= 0;

	pid->out = 0;
}

float PID_Update(PID *pid, float setpoint, float measurement){

	/* Error signal (e) */
	float error = setpoint - measurement;	// Desired Setpoint - Measure Value

	/* Proportional term (P) */
	float proportional = pid->kp * error;

	/* Integral term (I) */
	pid->integrator += 0.5f * pid->ki * pid->Ts * (error + pid->prevError);
	
	/* --- Anti-wind-up --- */
	float minLimitInt, maxLimitInt;

	if (pid->maxLimit > proportional){
		maxLimitInt = pid->maxLimit - proportional;
	}
	else{
		maxLimitInt = 0.0f;
	}

	if (pid->minLimit < proportional){
		minLimitInt = pid->minLimit - proportional;
	}
	else{
		minLimitInt = 0.0f;
	}

	/* --- Clamp integrator ---*/
	/* Avoids integrator wind-up, which means
	 * that the integrator will not be allowed to
	 * grow indefinitely. */
	if (pid->integrator > maxLimitInt){
		pid->integrator = maxLimitInt;
	}
	else if (pid->integrator < minLimitInt){
		pid->integrator = minLimitInt;
	}

	/* Derivative term (D) */
	pid->diff 	= (2.0f * pid->kd * (measurement - pid->prevMeasurement) 
				+ (2.0f * pid->tau - pid->Ts) * pid->diff) 
				/ (2.0f * pid->tau + pid->Ts);

	/* Calculate the controller output */
	pid->out = proportional + pid->integrator + pid->diff;

	/* Limit Output */
	if(pid->out > pid->maxLimit){
		pid->out = pid->maxLimit;
	}
	else if(pid->out < pid->minLimit){
		pid->out = pid->minLimit;
	}

	/* Update previous error and measurement */
	pid->prevError = error;
	pid->prevMeasurement = measurement;

	return pid->out;
}