//Created by: Misa Lopez
//Date: 02/14/23
//Reference: https://github.com/br3ttb/Arduino-PID-Library
//Change log: 
//  
//
//
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "pid.h"
#include <time.h>

/*Contructor for PID*/
PID::PID(double* input, double* output, double* setpoint, double kp, double ki, double kd)
{
        PID::input = input; 
        PID::output = output; 
        PID::setpoint = setpoint; 
        PID::kp = kp; 
        PID::ki = ki; 
        PID::kd = kd; 
        PID::sampletime = 20000; //default of 20 miliseconds 20000
        PID::outputMin = -255; 
        PID::outputMax = 255;
        PID::integral = 0; 
} 

/*set PID parameters kp, ki, and kd*/
void PID::setParams(double p, double i, double d)
{
    PID::kp = p; 
    PID::ki = i; 
    PID::kd = d; 
}

/*Set Min and Max value for PID output*/
void PID::setMinMax(double min, double max)
{
    PID::outputMin = min;
    PID::outputMax = max;
}

/*Calculate PID output*/
void PID::computePID()
{ 
    double in = *input; 
    double error = *setpoint - in;
    double dt = (double)sampletime/CLOCKS_PER_SEC; 
    double out; 

    /*Caluclate proportional term*/
    double proportional; 
    proportional = error;
    /*Calculate integral term*/ 
    integral+= error * dt; 
    /*Calculate derivative term*/
    double derivative; 
    derivative = (error - prevError)/dt; 
    out = (kp*proportional) + (ki*integral) + (kd*derivative); 
    if(out > outputMax)
    {
        out = outputMax; 
    }
    if(out < outputMin)
    {
        out = outputMin; 
    }
    prevError = error; 
    *output = out;
}
