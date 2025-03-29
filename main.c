/*
 * File:   main.c
 * Author: dhara
 *
 * Created on 25 February, 2025, 7:40 PM
 */


#include <xc.h>
#include"clcd.h"
#include"main.h"
#include"microoven.h"
#include"matrix_keypad.h"
#include "timers.h"
#pragma config WDTE=OFF

unsigned char operational_flag = POWER_ON_SCREEN;
unsigned char reset_flag=RESET_NOTHING;
extern unsigned char min,sec;

void init_config(void)
{
    //config mkp, clcd timers, buzzer, fan
    init_clcd();
    init_matrix_keypad();
    
    //config_flag
    FAN_DDR=0;
    FAN=0;
    
    BUZZER_DDR=0;
    BUZZER=0;
    //config timer
    init_timer2();
    PEIE=1;
    GIE=1;
    
}
void main(void) {
    unsigned char key; unsigned char start_mode=0;
    init_config();
    while(1)
    {
        key= read_matrix_keypad(STATE);
        //select the mode based on key pressed in menu screen
        if(operational_flag==MENU_SCREEN)
        {
            
            if(key == 1)
            {    clear_screen();
                operational_flag=MICRO_MODE;
                 start_mode=0;
                clcd_print("Power=900W",LINE2(0));
                __delay_ms(2000);
                clear_screen();
                reset_flag=RESET_MODE;
            }
            else if(key==2)
            {
                clear_screen();
                 start_mode=0;
                operational_flag=GRILL_MODE;
                reset_flag=RESET_MODE;
            }
             else if(key==3)
            {
                 clear_screen();
                 start_mode=0;
                operational_flag=CONVECTION_MODE;
                reset_flag=RESET_MODE;
            }
            else if(key==4)
            {
                 clear_screen();
                operational_flag=START_MODE;
                start_mode=1;
                 reset_flag=RESET_MODE;
            }
        }
        if(operational_flag==DISPLAY_TIME)
        {
              if(key == 4)
            {  
                 // operational_flag=START;
                  if(start_mode)
                  {
                      sec=sec+30;
                      if(sec>59)
                      {
                          min++;
                          sec=sec-60;
                          
                      }
                  }
            }
            else if(key==5)
            {
                operational_flag=PAUSE;
                
            }
             else if(key==6)
            {
               operational_flag=STOP;
            }
           
        }
        else if(operational_flag==PAUSE)
        {
            if(key==4)
            {
                FAN=1;
                TMR2ON=1;
                operational_flag=DISPLAY_TIME;
            }
        }
        
        
        
        
        
        //display different screen and detect key press
               //switch between screen
                //power on screen
                //menu screen
                //micro mode
                //grill mode
                //convention mode
                //start mode
       switch(operational_flag)
       {
           case POWER_ON_SCREEN :
                                  //display power on screen
                                  power_on_screen();
                                  operational_flag=MENU_SCREEN; //switch to menu screen
                                  clear_screen(); //to clear the clcd
                                   break;
           case MENU_SCREEN     :
                                 //display menu
                                 display_menu_screen();
                                   break;
               
               
           case MICRO_MODE  :
               //read the key based on key SET TIME
                                     set_time(key,reset_flag);
               //turn on the microwave and display the time
               
                                     break;
           case DISPLAY_TIME :
                       time_display();
                       break;
           case PAUSE :
               //stop cooking and timer
                             FAN=0;
                             TMR2ON=0;
                             break;
           case STOP :
                            FAN=0;
                             TMR2ON=0;
                             clear_screen();
                             operational_flag=MENU_SCREEN;
                             break; 
           case GRILL_MODE:
                            set_time(key,reset_flag);
                            break;
           case CONVECTION_MODE:
                             set_temp(key,reset_flag);
                            break;
               
           case START_MODE:
                             heat_food();
                             break;
                                       
                                      
       }
       reset_flag=RESET_NOTHING;
    }
    
}
