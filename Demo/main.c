#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include "Drivers/interrupts.h"
#include "Drivers/gpio.h"
#include "Drivers/video.h"
#include "Drivers/printf-stdarg.h"

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef long int32_t;

/* The periods assigned to the one-shot and auto-reload timers respectively. */
#define mainONE_SHOT_TIMER_PERIOD	( pdMS_TO_TICKS( 3333UL ) )
#define mainAUTO_RELOAD_TIMER_PERIOD	( pdMS_TO_TICKS( 500UL ) )

/*
 * The callback functions used by the one-shot and auto-reload timers
 * respectively.
 */
static void prvOneShotTimerCallback( TimerHandle_t xTimer );
static void prvAutoReloadTimerCallback( TimerHandle_t xTimer );
/**
 *	This is the systems main entry, some call it a boot thread.
 *
 *	-- Absolutely nothing wrong with this being called main(), just it doesn't have
 *	-- the same prototype as you'd see in a linux program.
 **/
void main (void)
{
	TimerHandle_t xAutoReloadTimer, xOneShotTimer;
	BaseType_t xTimer1Started, xTimer2Started;

	xOneShotTimer = xTimerCreate("OneShot", mainONE_SHOT_TIMER_PERIOD, pdFALSE, 0, prvOneShotTimerCallback);
	xAutoReloadTimer = xTimerCreate("AutoReload", mainAUTO_RELOAD_TIMER_PERIOD, pdTRUE, 0, prvAutoReloadTimerCallback);

	if((xOneShotTimer != NULL) && (xAutoReloadTimer != NULL)){
	    xTimer1Started = xTimerStart(xOneShotTimer, 0);
	    xTimer2Started = xTimerStart(xAutoReloadTimer, 0);
	}

	initFB();

	if((xTimer1Started == pdTRUE) && (xTimer2Started == pdTRUE))
	    vTaskStartScheduler();

	/*
	 *	We should never get here, but just in case something goes wrong,
	 *	we'll place the CPU into a safe loop.
	 */
	while(1) {
		;
	}
}

static void prvOneShotTimerCallback( TimerHandle_t xTimer ){
    static TickType_t xTimeNow;
    char str[20]="OneShot";

    xTimeNow = xTaskGetTickCount();
    sprintf(str, "OneShot : %d", xTimeNow);
    println(str,BLUE_TEXT);
}
static void prvAutoReloadTimerCallback( TimerHandle_t xTimer ){
    static TickType_t xTimeNow;

    xTimeNow = xTaskGetTickCount();
    println("AutoReload ",GREEN_TEXT);
}
