/********************************************************************
	filename: 	GameCarTypes
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __GameCarTypes_h__
#define __GameCarTypes_h__

#include "GameComponents/PANDUCar2DType.h"

namespace Game
{
	enum GameCarTypes
	{
		GCT_Mercedes = 0,

		GCT_Num
	};

	/*
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
			float DRAG,				// factor for air resistance (drag) 
			float RESISTANCE,		// factor for rolling resistance
			float CA_R,				// cornering stiffness 
			float CA_F,				// cornering stiffness 
			float MAX_GRIP,			// maximum (normalised) friction force, =diameter of friction circle 
		};
	*/

	static const Pandu::Car2DType kGameCarTypes[GCT_Num] = 
									{
										{
											1.0f,		//wheelbase			
											1.0f,		//b
											1.0f + 1.0f,//c
											1.0f,		//h
											1500.0f,	//mass
											1500.0f,	//inertia
											1.5f,		//length
											3.0f,		//width
											0.7f,		//wheellength
											0.3f,		//wheelwidth
											5.0f,		//DRAG
											30.0f,		//RESISTANCE	
											-5.20f,		//CA_R
											-5.0f,		//CA_F
											2.0f,		//MAX_GRIP
											9.8f,		//gravity
										},				//GCT_Mercedes
									};
}

#endif