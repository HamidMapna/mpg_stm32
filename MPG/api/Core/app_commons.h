#ifndef APP_COMMONS_H
#define APP_COMMONS_H

#include "osal.h"
#include "app_utils.h"
/**
 * Start application main loop
 *
 * Application must have been initialized using \a app_init() before
 * this function is called.
 *
 * If \a task_config parameters is set to RUN_IN_SEPARATE_THREAD a
 * thread execution the \a app_loop_forever() function is started.
 * If task_config is set to RUN_IN_MAIN_THREAD no such thread is
 * started and the caller must call the \a app_loop_forever() after
 * calling this function.
 *
 * RUN_IN_MAIN_THREAD is intended for rt-kernel targets.
 * RUN_IN_SEPARATE_THREAD is intended for linux targets.
 *
 * @param app                 In:    Application handle
 * @param task_config         In:    Defines if stack and application
 *                                   is run in main or separate task.
 * @return 0 on success, -1 on error
 */
int app_start(app_data_t* app, app_run_in_separate_task_t task_config);

/**
 * Initialize P-Net stack and application.
 *
 * The \a pnet_cfg argument shall have been initialized using
 * \a app_pnet_cfg_init_default() before this function is
 * called.
 *
 * @param pnet_cfg               In:    P-Net configuration
 * @return Application handle, NULL on error
 */
app_data_t* app_init(const pnet_cfg_t* pnet_cfg);
/**
 * Application task definition. Handles events in eternal loop.
 *
 * @param arg                 In: Application handle
 */
void app_loop_forever(void* arg);
/** Partially initialise config values, and use proper callbacks
 *
 * @param pnet_cfg     Out:   Configuration to be updated
 */
void app_pnet_cfg_init_default(pnet_cfg_t * pnet_cfg);
#endif // !APP_COMMONS_H