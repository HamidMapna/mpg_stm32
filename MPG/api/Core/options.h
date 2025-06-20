
#if !defined (PNET_MAX_SUBSLOTS)
/** Per slot (3 needed for DAP). */
#define PNET_MAX_SUBSLOTS       4
#endif

#if !defined (PNET_MAX_SLOTS)
/** Per API. Should be > 1 to allow at least one I/O module. */
#define PNET_MAX_SLOTS          5
#endif

#define APP_TICK_INTERVAL_US 1000 /* 1 ms */
#define APP_GSDML_ALARM_PAYLOAD_SIZE        1 /* bytes */

/* Events handled by main task */
#define APP_EVENT_READY_FOR_DATA BIT (0)
#define APP_EVENT_TIMER          BIT (1)
#define APP_EVENT_ALARM          BIT (2)
#define APP_EVENT_ABORT          BIT (15)


/* Thread configuration for targets where sample
 * event loop is run in a separate thread (not main).
 * This applies for linux sample app implementation.
 */
#define APP_MAIN_THREAD_PRIORITY  15
#define APP_MAIN_THREAD_STACKSIZE 4096 /* bytes */

