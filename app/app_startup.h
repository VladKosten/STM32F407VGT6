#ifndef APP_STARTUP_H_
#define APP_STARTUP_H_

#ifdef __cplusplus
extern "C"{
#endif

/*================================================================[INCLUDE]================================================*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*===========================================================[MACRO DEFINITIONS]===========================================*/

/**
* \brief Application startup task priority (must have the highest level)
*/
#define APP_STARTUP_TASK_PRIORITY		        (tskIDLE_PRIORITY + 5)

/**
* \brief Application startup task stack size
*/
#define APP_STARTUP_TASK_STACK_SIZE             512u

/**
* \brief Application startup delay ms
*/
#define APP_STARTUP_DELAY_MS                    100u

/**
* \brief Using of an external supervisor/watchdog
*/
#define APP_STARTUP_USE_EXT_WDT                 0

/**
 * \brief External watchdog restart period
 */
#define APP_EXTERNAL_WDT_RESTART_TIME_MS        38


/*========================================================[DATA TYPES DEFINITIONS]=========================================*/

/**
* \brief Application startup errors enumeration
*/
typedef enum
{
    APP_STARTUP_NO_ERR = 0,     // No errors
    APP_STARTUP_INIT_ERR,       // Some init error (no recources for startup)
    APP_STARTUP_EXEC_ERR        // The application execution has been terminated with an error

} AppStartupErr_t;


/*===========================================================[PUBLIC INTERFACE]============================================*/

/**
 * \brief Application startup
 * \param[in] no;
 * \param[out] no;
 * \return error code, non-zero = an error occured.
 */
AppStartupErr_t AppStartup(void);


#ifdef __cplusplus
}
#endif


#endif /* APP_STARTUP_H_ */