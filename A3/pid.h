//Created by: Misa Lopez
//Date: 02/14/23
//Reference: https://github.com/br3ttb/Arduino-PID-Library
//Change log: 
//  
//
//
#ifndef PID_H
#define PID_H

class PID
{
    public: 
        PID(double*, double*, double*, double, double, double); //contructor 
        void computePID(); //used to compute PID output
        void setMinMax(double, double); // set output min and max values
        void setParams(double, double, double); //set kp, ki, kd dynamically; 
    private: 
        double kp; //proportional 
        double ki; //integral
        double kd; //derivative

        double *setpoint; //setpoint to target
        double *input; //input of control system
        double *output; //output for system control

        double outputMin; //maximum output for PID
        double outputMax; //minimum output for PID

        unsigned long sampletime; //sample time in microseconds

        double integral; //accumulates integral error for I term on PID
        double prevError; //stores previous error for derivative term calculation
    
};
#endif