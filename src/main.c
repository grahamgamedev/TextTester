#include <stdio.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/ctrl.h>
#include <psp2/power.h>
#include <psp2/touch.h>
#include <psp2/rtc.h>

#include "debugScreen.h"
#define printf psvDebugScreenPrintf

void statusbar() {
    /*Refresh the statusbar at the topof screen*/
    printf("\e[s" "\e[0;0H" "\e[39;49m");
    printf(scePowerIsBatteryCharging()? "Charging   " : "Discharging ");
    
    SceDateTime time; 
    sceRtcGetCurrentClockLocalTime(&time);
	printf("\e[0;50H" "%02d/%02d/%04d %02d:%02d:%02d",
        sceRtcGetDay(&time),
        sceRtcGetMonth(&time),
		sceRtcGetYear(&time),
		
		sceRtcGetHour(&time),
		sceRtcGetMinute(&time),
		sceRtcGetSecond(&time));

    int batteryLifeTime = scePowerGetBatteryLifeTime();
    printf("\e[0;90H" "%02dh%02dm Remaning ", batteryLifeTime/60, batteryLifeTime-(batteryLifeTime/60*60));

    int batteryLifePercent = scePowerGetBatteryLifePercent();
    int color = 9;
    if (batteryLifePercent >= 50) color = 2;
    if (batteryLifePercent <= 10) color = 1;
    printf("\e[0;117H" "\e[3%dm" "%d%%\n", color, batteryLifePercent);

    printf("\e[32m========================================================================================================================");
    printf("\e[u" "\e[39;49m");
}

void clear() {
    /*Clear screen, refresh status bar and move cursor to correst position*/
    printf("\e[2J");
    statusbar();
    printf("\e[4;0H");
    }

int buttonwait(int button) {
    /*Wait for button to be pressed*/
    SceCtrlData ctrl;
    while (ctrl.buttons != button) {
        statusbar();
        sceCtrlPeekBufferPositive(0, &ctrl, 1);
        if (ctrl.buttons == (SCE_CTRL_START | SCE_CTRL_RTRIGGER)) return 1;
    }
}

void maingame() {
    /*Main game code*/
    SceCtrlData ctrl;
    clear();

    printf("Main game\n");
    printf("Press \e[32mStart + R\e[m to stop\n\n");
    printf("Press \e[32mX\e[m to play\n");
    if (buttonwait(SCE_CTRL_CROSS) == 1) return;

    clear();
    printf("Level 1\n\n");

    printf("Press \e[32m<\e[m\n");
    if (buttonwait(SCE_CTRL_LEFT) == 1) return;
    printf("\e[31m" "WARNING I'm a warning!" "\e[m\n");
    printf("Press \e[32mO\e[m to dismiss\n");
    if (buttonwait(SCE_CTRL_CIRCLE) == 1) return;
    printf("Press \e[32mL\e[m\n");
    if (buttonwait(SCE_CTRL_LTRIGGER) == 1) return;
    printf("\e[31m" "WARNING Low primery battery." "\e[m\n");
    if (buttonwait(SCE_CTRL_CIRCLE) == 1) return;

    printf("Press \e[32mL + >\e[m\n");
    if (buttonwait(SCE_CTRL_LTRIGGER | SCE_CTRL_RIGHT) == 1) return;
    printf("\e[31m" "WARNING Difficalty ahead." "\e[m\n");
    printf("Press \e[32mR + ^ + []\e[m\n");
    if (buttonwait(SCE_CTRL_RTRIGGER | SCE_CTRL_UP | SCE_CTRL_SQUARE) == 1) return;
    printf("Press \e[32mv + X + START + L\e[m\n");
    if (buttonwait(SCE_CTRL_DOWN | SCE_CTRL_CROSS | SCE_CTRL_START | SCE_CTRL_LTRIGGER) == 1) return;
    printf("Press \e[32m> + [] + O + R + L\e[m\n");
    if (buttonwait(SCE_CTRL_RIGHT | SCE_CTRL_SQUARE | SCE_CTRL_CIRCLE | SCE_CTRL_LTRIGGER | SCE_CTRL_RTRIGGER) == 1) return;

    printf("\n To complete this level the sticks must be (fully) crossed.\n");
    while (1) {
        statusbar();
        sceCtrlPeekBufferPositive(0, &ctrl, 1);
        if (ctrl.lx > 250 & ctrl.rx < 5) break;
    }
    
    clear();
    printf("Level 2\n\n");
    printf("\e[31m" "WARNING Critical low primery battery!" "\e[m\n");
    printf("\e[39;44m" "\e[6;81H" "                    ");
    printf("\e[7;81H" "                    ");
    printf("\e[8;70H" "                                           ");
     
    int lines;
    lines = 9;
    while (lines <= 66) {                                                            
        printf("\e[%d ;70H" " " "\e[%d ;112H" " ", lines, lines);
        lines += 1;
    }
    printf("\e[67;70H" "                                           ");
    printf("\e[41m" "\e[65;71H" "                                         ");
    printf("\e[41m" "\e[66;71H" "                                         ");
    
    if (buttonwait(SCE_CTRL_CIRCLE) == 1) return;
    printf("\e[39;49m" "\e[7;0H" "Ready");

    while (1) {
        statusbar();
        if (scePowerIsPowerOnline()) break;
        if (ctrl.buttons == (SCE_CTRL_START | SCE_CTRL_RTRIGGER)) return;
    }

    while (lines >= 10) { 
        lines -= 1;
        printf("\e[42m" "\e[%d;71H" "                                         ", lines);
        sceKernelDelayThread(1000000);
        if (ctrl.buttons == (SCE_CTRL_START | SCE_CTRL_RTRIGGER)) return;
    }
    
    printf("\e[39;49m" "\e[8;0H" "Fully Charged\n");
    printf("Press \e[32mX\e[m to restart system");
    if (buttonwait(SCE_CTRL_CROSS) == 1) return;
    
    clear();
    printf("Level 3\n\n");
    printf("\e[31m" "WARNING Not implemented!");
    if (buttonwait(SCE_CTRL_CIRCLE) == 1) return;  
}

int main(int argc, char *argv[]) {
    SceCtrlData ctrl;
    /* to enable analog sampling */
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);

	psvDebugScreenInit();
    
    /*Mainloop*/
    while (1) { 
        clear();
        printf("Input Test \e[32m(X)\e[m\n");
        printf("Touch Test \e[32m(O)\e[m\n");
        printf("Play Game \e[32m([])\e[m\n");

        while (1) {
            statusbar();
            sceCtrlPeekBufferPositive(0, &ctrl, 1); 

            if (ctrl.buttons & SCE_CTRL_CROSS) {
                clear();
	            printf("Input Test\n");
	            printf("Press \e[32mStart + R\e[m to stop\n\n");
	
	            const char* btn_label[]={"SELECT ","","","START ",
		            "^ ","> ","v ","< ","L ","R ","","",
		            "/\\ ","O ","X ","[] "};
	            do {
                    statusbar();
		            sceCtrlPeekBufferPositive(0, &ctrl, 1);
		            printf("Buttons:%08X = ", ctrl.buttons);
		            int i;
		            for (i=0; i < sizeof(btn_label)/sizeof(*btn_label); i++) {
			            printf("\e[9%im%s",(ctrl.buttons & (1<<i)) ? 7 : 0, btn_label[i]);
		            }
		            printf("\e[m Stick:[%3i:%3i][%3i:%3i]\r", ctrl.lx,ctrl.ly, ctrl.rx,ctrl.ry);
	            } while (ctrl.buttons != (SCE_CTRL_START | SCE_CTRL_RTRIGGER) );
                break;
            } 

            if (ctrl.buttons & SCE_CTRL_CIRCLE) {
                clear();
                printf("Touch Test\n");
                printf("Swipe to the bottom with 1 finger to stop\n");
	            sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_START);
	            sceTouchSetSamplingState(SCE_TOUCH_PORT_BACK, SCE_TOUCH_SAMPLING_STATE_START);
	
	            SceTouchData touch_old[SCE_TOUCH_PORT_MAX_NUM];
	            SceTouchData touch[SCE_TOUCH_PORT_MAX_NUM];

	            while (1) {
                    statusbar();
		            memcpy(touch_old, touch, sizeof(touch_old));
		            printf("\e[8;0H");
		            int port,i;
		            /* sample both back and front surfaces */
		            for(port = 0; port < SCE_TOUCH_PORT_MAX_NUM; port++){
			            sceTouchPeek(port, &touch[port], 1);
			            printf("\e[m%s",((const char*[]){"FRONT ","BACK  "})[port]);
			            /* print every touch coordinates on that surface */
			            for(i = 0; i < SCE_TOUCH_MAX_REPORT; i++)
				            printf("\e[9%im%4i:%-4i ", (i < touch[port].reportNum)? 7:0,
				                   touch[port].report[i].x,touch[port].report[i].y);
			            printf("\n");
		            }

		            if ( (touch[SCE_TOUCH_PORT_FRONT].reportNum == 1)
		              && (touch_old[SCE_TOUCH_PORT_FRONT].reportNum == 1)
		              && (touch[SCE_TOUCH_PORT_FRONT].report[0].y >= 1000)
		              && (touch_old[SCE_TOUCH_PORT_FRONT].report[0].y < 1000))
		            break;
	            } 
                break;
            }

            if (ctrl.buttons & SCE_CTRL_SQUARE) {
                maingame();
                break;
            }
        }
	 }   
}
