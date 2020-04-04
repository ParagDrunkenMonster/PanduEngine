#ifndef __PANDUCar2DType_h__
#define __PANDUCar2DType_h__

namespace Pandu
{
	struct Car2DType
	{
		float wheelbase;		// wheelbase in m
		float b;				// in m, distance from CG to front axle
		float c;				// in m, idem to rear axle
		float h;				// in m, height of CM from ground
		float mass;				// in kg
		float inertia;			// in kg.m
		float length;
		float width;
		float wheellength;
		float wheelwidth;
		float DRAG;				// factor for air resistance (drag) 
		float RESISTANCE;		// factor for rolling resistance
		float CA_R;				// cornering stiffness 
		float CA_F;				// cornering stiffness 
		float MAX_GRIP;			// maximum (normalised) friction force, =diameter of friction circle 
		float gravity;			// gravitational force applied downwards

	};
}

#endif