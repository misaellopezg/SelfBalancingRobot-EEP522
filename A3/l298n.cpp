//Created by: Misa Lopez
//Date: 02/08/23
//Change log: 
//  
//
//
#include "l298n.h"
#include <stdio.h>
#include <stdint.h>
#include <bcm2835.h>
#include <unistd.h>

#define PWM_RANGE 1024
#define PWM_CHANNEL 0

//Initialize motor

l298n::l298n(uint8_t pin1, uint8_t pin2, uint8_t pinEn)
{
    in1 = pin1; 
    in2 = pin2; 
    enA = pinEn;
    pwm = 0;

    // Set the pins to be outputs
    bcm2835_gpio_fsel(in1, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(in2, BCM2835_GPIO_FSEL_OUTP);
    //Turn on both outputs to prevent motors from moving
    bcm2835_gpio_write(in1, HIGH);
    bcm2835_gpio_write(in2, HIGH);
    // Set the output pin to Alt Fun 5, to allow PWM channel 0 to be output there
    bcm2835_gpio_fsel(enA, BCM2835_GPIO_FSEL_ALT5);
    bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_16);
    bcm2835_pwm_set_mode(PWM_CHANNEL, 1, 1);
    bcm2835_pwm_set_range(PWM_CHANNEL, PWM_RANGE);
    bcm2835_pwm_set_data(PWM_CHANNEL, pwm);
}

/*Used to stop motors*/
void l298n::stopMotors()
{
    //Turn on both outputs to prevent motors from moving
    bcm2835_gpio_write(in1, HIGH);
    bcm2835_gpio_write(in2, HIGH);
}


/*Rotate motors clockwise*/
void l298n::clockwiseMotors()
{
    //Turn on both outputs to prevent motors from moving
    bcm2835_gpio_write(in1, LOW);
    bcm2835_gpio_write(in2, HIGH);
}

/*Rotate motors counter-clockwise*/
void l298n::counterclockwiseMotors()//
{
    //Turn on both outputs to prevent motors from moving
    bcm2835_gpio_write(in1, HIGH);
    bcm2835_gpio_write(in2, LOW);
}

/*Set PWM*/
void l298n::setPWM(uint16_t pwmval)
{
    pwm = pwmval; 
    bcm2835_pwm_set_data(PWM_CHANNEL, pwm);
}