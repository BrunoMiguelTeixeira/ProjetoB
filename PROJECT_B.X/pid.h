#ifndef __PID_H__
#define __PID_H__

typedef struct {
    
	/* Controller Gains */
    float kp; 				/**< Proportional Gain */
    float ki; 				/**< Integral Gain */
	float kd; 				/**< Derivative Gain */

    /* Derivative Low-Pass Filter */
	float tau; 				/**< Derivative Low-Pass Filter Time Constant */

	/* Controller Output Limits */
	float minLimit; 		/**< Minimum Output Limit */
	float maxLimit; 		/**< Maximum Output Limit */

	/* Sample time */
	float Ts; 				/**< Sample Time */

	/* Controller Variables */
	float diff 				/**< Differentiator */;
	float integrator; 		/**< Integrator */
	float prevError; 		/**< Previous Error */
	float prevMeasurement; 	/**< Previous Measurement */

	float out; 			/**< Controller Output */		
	
} PID;

void PID_Init(PID *pid);
float PID_Update(PID *pid, float setPoint, float measurement);

#endif // PID_H

