/**
 * \file app_startup.c
 * \brief Application startup
 * \author: Vlad Kosten
*/
//===============================================================================[ INCLUDE ]=======================================================================================

#include <stdint.h>
#include <stddef.h>

#include "app_startup.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"


//=====================================================================[ INTERNAL MACRO DEFENITIONS ]===============================================================================

/**
 * \brief Assert macro
 */
#ifndef APP_STARTUP_ASSERT
    #ifdef configASSERT
        #define APP_STARTUP_ASSERT(expr) configASSERT(expr)
    #else
        #define APP_STARTUP_ASSERT(expr)
    #endif
#endif

//====================================================================[ INTERNAL DATA TYPES DEFINITIONS ]===========================================================================


//===============================================================[ INTERNAL FUNCTIONS AND OBJECTS DECLARATION ]=====================================================================

/**
 * \brief Application startup task
 * \note  The task MUST have the highest available priority lvl because its initializes other application tasks
 *        NO TASK SHOULD BE PERFORMED UNTIL THIS TASK IS COMPLETE
 */
static void appStartupTask(void *taskParamPtr);

/**
 * \brief Application flag
*/
static bool AppInitFlag = false;

/**
 * \brief Application startup task handle
*/
static TaskHandle_t AppStartupTaskHandle = NULL;

/**
 * \brief Events global counters
 */
volatile uint64_t AppStartupGlobalTimeTickCount   = 0;  // Global system timer tick counter
volatile uint32_t AppStartupIdleCount             = 0;  // Global low priority idle cycles counter

//=======================================================================[PUBLIC INTERFACE FUNCTIONS]==============================================================================

/**
 * \brief Application startup
 * \param[in] no;
 * \param[out] no;
 * \return error code, non-zero = an error occured.
 */
AppStartupErr_t AppStartup(void)
{
	/* Creating of the startup task */
    TaskHandle_t appStartupTaskHandle = NULL;
    (void)appStartupTaskHandle;
	BaseType_t startupTaskCreateErr = xTaskCreate
	(	appStartupTask,									// Startup task function
		"APP_START",									// Starttup task name
		APP_STARTUP_TASK_STACK_SIZE,					// Startup task stack size
		NULL,											// Startup task params = nothing;
		APP_STARTUP_TASK_PRIORITY,						// Startup task priority = set the highest priority
		&appStartupTaskHandle							// Startup task handle
	);

    APP_STARTUP_ASSERT(appStartupTaskHandle);

    /* Checking */
	if(startupTaskCreateErr != pdPASS)
	{
		return APP_STARTUP_INIT_ERR;    // Exit: Error: Application init error
	}



	/* Start the RTOS */
	vTaskStartScheduler();

	return APP_STARTUP_EXEC_ERR;	// Exit: The application execution has been terminated with an error
}


//============================================================================[PRIVATE FUNCTIONS]===================================================================================

/**
 * \brief Application startup task
 * \param[in] void *taskParamPtr - task parameters (may be ignored);
 * \param[out] no;
 * \return error code, non-zero = an error occured.
 */
static void appStartupTask(void* param)
{
	int16_t status = 0;
    (void)status;
	(void)param;

	while(1)
	{
        /* Application startup delay */
        vTaskDelay(pdMS_TO_TICKS(APP_STARTUP_DELAY_MS));

        /* Place an application-specific startup code here ... */
        // ... //



        /* --------------- DO NOT TOUCH IT ! ----------------*/
        /* Set the Application init completion flag */
        AppInitFlag = true;

		/* Delete the startup task */
        AppStartupTaskHandle = NULL;
		vTaskDelete(NULL);

		/* We should not get here !*/
		APP_STARTUP_ASSERT(0);
	}
}


//============================================================================[RTOS FUNCTIONS]===================================================================================

/**
 * \brief      Stack overflow handler
 * \param[in]  TaskHandle_t xTask   - task handle;
 * \param[in]  char *pcTaskName     - pointer to the task name;
 * \param[out] no;
 * \return     no.
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)xTask;
    (void)pcTaskName;
    APP_STARTUP_ASSERT(0);
}

/**
 * \brief      Application tick hook
 * \param[in]  no;
 * \param[out] no;
 * \return     no.
 */
void vApplicationTickHook(void)
{
    // portENTER_CRITICAL();
    AppStartupGlobalTimeTickCount++;
    // portEXIT_CRITICAL();
}

/**
 * \brief      This function calls when RTOS processing IDLE task
 * \param[in]  no;
 * \param[out] no;
 * \return     no.
 */
void vApplicationIdleHook(void)
{
    AppStartupIdleCount++;
}


/**
 * \brief      Application malloc failed hook.
 *             In other words this function is handling memory allocation errors from the RTOS heap
 * \param[in]  no;
 * \param[out] no;
 * \return     no.
 */
void vApplicationMallocFailedHook(void)
{
    APP_STARTUP_ASSERT(0); // Insert assert to catch such case
}


/* configSUPPORT_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
#if (configSUPPORT_STATIC_ALLOCATION == 1)
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    /* If the buffers to be provided to the Idle task are declared inside this
    function then they must be declared static - otherwise they will be allocated on
    the stack and so not exists after this function exits. */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
    state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
#endif


/* configSUPPORT_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
#if ((configSUPPORT_STATIC_ALLOCATION == 1) && (configUSE_TIMERS == 1) )
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    /* If the buffers to be provided to the Timer task are declared inside this
    function then they must be declared static - otherwise they will be allocated on
    the stack and so not exists after this function exits. */
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
    task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configTIMER_TASK_STACK_DEPTH is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
#endif