#include "mbed.h"
#include "LCD.h"

#define WARTEN 1000ms

DigitalOut led(PC_0);
lcd mylcd;

DigitalIn modus(PB_0);

int sekunden = 0;
int minuten = 0;
int stunden= 0;

InterruptIn stundenStellen(PA_10);
InterruptIn minutenStellen(PA_6);
InterruptIn sekundenStellen(PA_1);

void stundenErhöhen(void){
    stunden++;
}

void minutenErhöhen(void){
    minuten++;
}

void sekundenErhöhen(void){
    sekunden++;
}


void initTimer(){

    RCC->APB1ENR |=0b10000;
    TIM6->PSC=31999; //Takt 1ms
    TIM6->ARR=1000; //1000ms warten
    TIM6->CNT=0; //bei 0 beginnen
    TIM6->SR=0; //UIF = 0
    TIM6->CR1=1; //CEN=1 start

}


int main()
{

    stundenStellen.mode(PullDown);
    stundenStellen.rise(&stundenErhöhen);

    minutenStellen.mode(PullDown);
    minutenStellen.rise(&minutenErhöhen);

    sekundenStellen.mode(PullDown);


    sekundenStellen.rise(&stundenErhöhen);

    mylcd.clear();

    initTimer();
    

    while (true) {

        if(modus == 0){

            stundenStellen.disable_irq();
            minutenStellen.disable_irq();
            sekundenStellen.disable_irq();
        

        if(TIM6->SR==1){
            led = !led;
            TIM6->SR=0;
            sekunden++;

            if(sekunden <= 9 && minuten <= 9){
                mylcd.clear();
                mylcd.printf("%i:0%i:0%i", stunden,minuten,sekunden);
            }
            else if(sekunden <= 9){
                mylcd.clear();
                mylcd.printf("%i:%i:0%i", stunden,minuten,sekunden);
            }
            else if(minuten <= 9){
                mylcd.clear();
                mylcd.printf("%i:0%i:%i", stunden,minuten,sekunden);
            }else{
                mylcd.clear();
                mylcd.printf("%i:%i:%i", stunden,minuten,sekunden);
            }
            
        }

        if (sekunden == 59) {
            sekunden = 0;
            minuten++;
        }

        if (minuten == 59){
            minuten = 0;
            stunden++;
        }

        if (stunden == 23){
            stunden = 0;
        }

        
        else{

            stundenStellen.enable_irq();
            minutenStellen.enable_irq();
            sekundenStellen.enable_irq();

    
            mylcd.clear();
            mylcd.printf("%i:%i:%i", stunden,minuten,sekunden);
            ThisThread::sleep_for(WARTEN);
            
            }
            
        }



    }
}

