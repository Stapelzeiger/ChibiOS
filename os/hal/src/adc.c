/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    adc.c
 * @brief   ADC Driver code.
 *
 * @addtogroup ADC
 * @{
 */

#include "hal.h"

#if HAL_USE_ADC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   ADC Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void adcInit(void) {

  adc_lld_init();
}

/**
 * @brief   Initializes the standard part of a @p ADCDriver structure.
 *
 * @param[out] adcp     pointer to the @p ADCDriver object
 *
 * @init
 */
void adcObjectInit(ADCDriver *adcp) {

  adcp->state    = ADC_STOP;
  adcp->config   = NULL;
  adcp->samples  = NULL;
  adcp->depth    = 0;
  adcp->grpp     = NULL;
#if ADC_USE_WAIT
  adcp->thread   = NULL;
#endif /* ADC_USE_WAIT */
#if ADC_USE_MUTUAL_EXCLUSION
  osalMutexObjectInit(&adcp->mutex);
#endif /* ADC_USE_MUTUAL_EXCLUSION */
#if defined(ADC_DRIVER_EXT_INIT_HOOK)
  ADC_DRIVER_EXT_INIT_HOOK(adcp);
#endif
}

/**
 * @brief   Configures and activates the ADC peripheral.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 * @param[in] config    pointer to the @p ADCConfig object. Depending on
 *                      the implementation the value can be @p NULL.
 *
 * @api
 */
void adcStart(ADCDriver *adcp, const ADCConfig *config) {

  osalDbgCheck(adcp != NULL);

  osalSysLock();
  osalDbgAssert((adcp->state == ADC_STOP) || (adcp->state == ADC_READY),
                "invalid state");
  adcp->config = config;
  adc_lld_start(adcp);
  adcp->state = ADC_READY;
  osalSysUnlock();
}

/**
 * @brief   Deactivates the ADC peripheral.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @api
 */
void adcStop(ADCDriver *adcp) {

  osalDbgCheck(adcp != NULL);

  osalSysLock();
  osalDbgAssert((adcp->state == ADC_STOP) || (adcp->state == ADC_READY),
                "invalid state");
  adc_lld_stop(adcp);
  adcp->state = ADC_STOP;
  osalSysUnlock();
}

/**
 * @brief   Starts an ADC conversion.
 * @details Starts an asynchronous conversion operation.
 * @note    The buffer is organized as a matrix of M*N elements where M is the
 *          channels number configured into the conversion group and N is the
 *          buffer depth. The samples are sequentially written into the buffer
 *          with no gaps.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 * @param[in] grpp      pointer to a @p ADCConversionGroup object
 * @param[out] samples  pointer to the samples buffer
 * @param[in] depth     buffer depth (matrix rows number). The buffer depth
 *                      must be one or an even number.
 *
 * @api
 */
void adcStartConversion(ADCDriver *adcp,
                        const ADCConversionGroup *grpp,
                        adcsample_t *samples,
                        size_t depth) {

  osalSysLock();
  adcStartConversionI(adcp, grpp, samples, depth);
  osalSysUnlock();
}

/**
 * @brief   Starts an ADC conversion.
 * @details Starts an asynchronous conversion operation.
 * @post    The callbacks associated to the conversion group will be invoked
 *          on buffer fill and error events.
 * @note    The buffer is organized as a matrix of M*N elements where M is the
 *          channels number configured into the conversion group and N is the
 *          buffer depth. The samples are sequentially written into the buffer
 *          with no gaps.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 * @param[in] grpp      pointer to a @p ADCConversionGroup object
 * @param[out] samples  pointer to the samples buffer
 * @param[in] depth     buffer depth (matrix rows number). The buffer depth
 *                      must be one or an even number.
 *
 * @iclass
 */
void adcStartConversionI(ADCDriver *adcp,
                         const ADCConversionGroup *grpp,
                         adcsample_t *samples,
                         size_t depth) {

  osalDbgCheckClassI();
  osalDbgCheck((adcp != NULL) && (grpp != NULL) && (samples != NULL) &&
               ((depth == 1) || ((depth & 1) == 0)));
  osalDbgAssert((adcp->state == ADC_READY) ||
                (adcp->state == ADC_COMPLETE) ||
                (adcp->state == ADC_ERROR),
                "not ready");

  adcp->samples  = samples;
  adcp->depth    = depth;
  adcp->grpp     = grpp;
  adcp->state    = ADC_ACTIVE;
  adc_lld_start_conversion(adcp);
}

/**
 * @brief   Stops an ongoing conversion.
 * @details This function stops the currently ongoing conversion and returns
 *          the driver in the @p ADC_READY state. If there was no conversion
 *          being processed then the function does nothing.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @api
 */
void adcStopConversion(ADCDriver *adcp) {

  osalDbgCheck(adcp != NULL);

  osalSysLock();
  osalDbgAssert((adcp->state == ADC_READY) || (adcp->state == ADC_ACTIVE),
                "invalid state");
  if (adcp->state != ADC_READY) {
    adc_lld_stop_conversion(adcp);
    adcp->grpp  = NULL;
    adcp->state = ADC_READY;
    _adc_reset_s(adcp);
  }
  osalSysUnlock();
}

/**
 * @brief   Stops an ongoing conversion.
 * @details This function stops the currently ongoing conversion and returns
 *          the driver in the @p ADC_READY state. If there was no conversion
 *          being processed then the function does nothing.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @iclass
 */
void adcStopConversionI(ADCDriver *adcp) {

  osalDbgCheckClassI();
  osalDbgCheck(adcp != NULL);
  osalDbgAssert((adcp->state == ADC_READY) ||
                (adcp->state == ADC_ACTIVE) ||
                (adcp->state == ADC_COMPLETE),
                "invalid state");

  if (adcp->state != ADC_READY) {
    adc_lld_stop_conversion(adcp);
    adcp->grpp  = NULL;
    adcp->state = ADC_READY;
    _adc_reset_i(adcp);
  }
}

#if ADC_USE_WAIT || defined(__DOXYGEN__)
/**
 * @brief   Performs an ADC conversion.
 * @details Performs a synchronous conversion operation.
 * @note    The buffer is organized as a matrix of M*N elements where M is the
 *          channels number configured into the conversion group and N is the
 *          buffer depth. The samples are sequentially written into the buffer
 *          with no gaps.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 * @param[in] grpp      pointer to a @p ADCConversionGroup object
 * @param[out] samples  pointer to the samples buffer
 * @param[in] depth     buffer depth (matrix rows number). The buffer depth
 *                      must be one or an even number.
 * @return              The operation result.
 * @retval RDY_OK       Conversion finished.
 * @retval RDY_RESET    The conversion has been stopped using
 *                      @p acdStopConversion() or @p acdStopConversionI(),
 *                      the result buffer may contain incorrect data.
 * @retval RDY_TIMEOUT  The conversion has been stopped because an hardware
 *                      error.
 *
 * @api
 */
msg_t adcConvert(ADCDriver *adcp,
                 const ADCConversionGroup *grpp,
                 adcsample_t *samples,
                 size_t depth) {
  msg_t msg;

  osalSysLock();
  osalDbgAssert(adcp->thread == NULL, "already waiting");
  adcStartConversionI(adcp, grpp, samples, depth);
  msg = osalThreadSuspendS(&adcp->thread);
  osalSysUnlock();
  return msg;
}
#endif /* ADC_USE_WAIT */

#if ADC_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)
/**
 * @brief   Gains exclusive access to the ADC peripheral.
 * @details This function tries to gain ownership to the ADC bus, if the bus
 *          is already being used then the invoking thread is queued.
 * @pre     In order to use this function the option
 *          @p ADC_USE_MUTUAL_EXCLUSION must be enabled.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @api
 */
void adcAcquireBus(ADCDriver *adcp) {

  osalDbgCheck(adcp != NULL);

  osalMutexLock(&adcp->mutex);
}

/**
 * @brief   Releases exclusive access to the ADC peripheral.
 * @pre     In order to use this function the option
 *          @p ADC_USE_MUTUAL_EXCLUSION must be enabled.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @api
 */
void adcReleaseBus(ADCDriver *adcp) {

  osalDbgCheck(adcp != NULL);

  osalMutexUnlock(&adcp->mutex);
}
#endif /* ADC_USE_MUTUAL_EXCLUSION */

#endif /* HAL_USE_ADC */

/** @} */
