//Created by: Misa Lopez
//Date: 02/08/23
//Change log: 
//  
//
//
#ifndef L298N_H
#define L298N_H
#include <stdint.h>

class l298n
{
    public: 
        l298n(uint8_t pin1, uint8_t pin2, uint8_t pinEn);//create class to control hbridge and motors
        void stopMotors(); //stop motors
        void clockwiseMotors(); //rotate motors clockwise
        void counterclockwiseMotors();//rotate motors counter-clockwise
        void setPWM(uint16_t pwmval);//set pwm
        int in1; //direction motor pin
        uint8_t in2; //direction motor pin
        uint8_t enA; //enable motor pin
        uint8_t pwm; //pwm val for motors
};
#endif