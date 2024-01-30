#include "mbed.h"
#include "LCD.h"

lcd mylcd;

int sekunden = 0;
int minuten = 0;
int stunden = 0;

InterruptIn stundenStellen(PA_10);
InterruptIn minutenStellen(PA_6);
InterruptIn sekundenStellen(PA_1);

void anzeigen(){
    mylcd.cursorpos(0);
    mylcd.printf("%02i:%02i:%02i", stunden,minuten,sekunden);
}


void stundenErhoehen(void){
    stunden++;
}

void minutenErhoehen(void){
    minuten++;
}

void sekundenErhoehen(void){
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
    stundenStellen.rise(&stundenErhoehen);

    minutenStellen.mode(PullDown);
    minutenStellen.rise(&minutenErhoehen);

    sekundenStellen.mode(PullDown);
    sekundenStellen.rise(&sekundenErhoehen);

    mylcd.clear();

    initTimer();


    stundenStellen.enable_irq();
    minutenStellen.enable_irq();
    sekundenStellen.enable_irq();
    




    while (true) {

    
        if(TIM6->SR==1){
            TIM6->SR=0;
            sekunden++;
        }

        if (sekunden >= 60) {
            sekunden = 0;
            minuten++;
        }

        if (minuten >= 60){
            minuten = 0;
            stunden++;
        }

        if (stunden >= 24){
            stunden = 0;
        }

        anzeigen();
        ThisThread::sleep_for(5ms);
            
        



    }
}

