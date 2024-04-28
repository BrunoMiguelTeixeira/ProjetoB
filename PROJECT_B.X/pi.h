#ifndef __PI_H__
#define __PI_H__

/*
 * Contém código não original
 *
 *Código base para a construção do controlador deriva de código disponibilizado num video
 *sobre controlador PID
 * Autor:https://youtu.be/zOByx3Izf5U?si=p1CvpppLJgU8kiIW
 *
 */
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

/**
* Function: 	PI_Init()\n
* Precondition: \n
* Input: 		pointer to struct pi\n
* Returns:      none\n
* Overview:     Initializes Controller PI values and its limits.\n
* 
*/
void PI_Init(PI *pi);


/**
* Function: 	PI_Update()\n
* Precondition: \n
* Input: 		pointer to struct pi, desired value, read value\n
* Returns:      Desired temperature value to correct error.\n
* Overview:     Updates desired value until error=0\n
*/
float PI_Update(PI *pi, float setPoint, float measurement);

#endif // PI_H

