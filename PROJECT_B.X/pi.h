#ifndef __PI_H__
#define __PI_H__

typedef struct {
    
	/* Controller Gains */
    float kp; 				/**< Proportional Gain */
    float ki; 				/**< Integral Gain */

	/* Controller Output Limits */
	float minLimit; 		/**< Minimum Output Limit */
	float maxLimit; 		/**< Maximum Output Limit */

	/* Sample time */
	float Ts; 				/**< Sample Time */

	/* Controller Variables */
	float integrator; 		/**< Integrator */
	float prevError; 		/**< Previous Error */
	float prevMeasurement; 	/**< Previous Measurement */

	float out; 			/**< Controller Output */		
	
} PI;

void PI_Init(PI *pi);
float PI_Update(PI *pi, float setPoint, float measurement);

#endif // PI_H

