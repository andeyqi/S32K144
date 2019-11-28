/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * @file mpu_pal.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3,  Taking address of near auto variable.
 * The code is not dynamically linked. An absolute stack address is obtained
 * when taking the address of the near auto variable.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * The function is defined for use by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.3, Expression assigned to a different essential type.
 * The cast is required expression assigned to a narrower or different essential type.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 10.5, Impermissible cast;
 * cannot cast from 'essentially unsigned' type to 'essentially enum<i>';
 * cannot cast from 'essentially enum<i>' to 'essentially enum<i>'.
 * This is required by the conversion of a bit-field of a register into a enum type
 * or between two different enum types.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.3, The cast performed between a pointer to
 * object type and a pointer to a different object type.
 * This is required for working with the common initialize function in initialize function.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.5, conversion from pointer to void to pointer to other type
 * This is required for working with the common initialize function in initialize function.
 */

#include "mpu_pal.h"
#include <stddef.h>

/*******************************************************************************
 * Privates
 ******************************************************************************/
#if defined(MPU_OVER_MPU)
/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_ConvertMpuErrorConfig
 * Description   : Converts MPU error structure.
 *
 *END**************************************************************************/
static void MPU_ConvertMpuErrorConfig(const mpu_access_err_info_t * mpuErrPtr,
                                      mpu_error_info_t * errResult)
{
    /* Converts error information */
    errResult->master = mpuErrPtr->master;
    errResult->overrun = false;
    errResult->attributes = (mpu_error_attributes_t)mpuErrPtr->attributes;
    errResult->accessType = (mpu_error_access_type_t)mpuErrPtr->accessType;
    errResult->accessCtr = mpuErrPtr->accessCtr;
    errResult->addr = mpuErrPtr->addr;
#if FEATURE_MPU_HAS_PROCESS_IDENTIFIER
    errResult->processId = mpuErrPtr->processorIdentification;
#endif /* FEATURE_MPU_HAS_PROCESS_IDENTIFIER */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_ConvertMpuMasterStruct
 * Description   : Converts MPU region structure.
 *
 *END**************************************************************************/
static void MPU_ConvertMpuRegionConfig(const mpu_region_config_t * regionConfig,
                                       mpu_master_access_right_t * masterAccessRight,
                                       mpu_user_config_t * configResult)
{
    uint8_t masterIdx = 0U;
    mpu_master_privilege_right_t masterPrivilegeRight;

    /* Allocates region address */
    configResult->startAddr = regionConfig->startAddr;
    configResult->endAddr = regionConfig->endAddr;

    /* Allocates access rights for all masters */
    for (masterIdx = 0U; masterIdx < FEATURE_MPU_MASTER_COUNT; masterIdx++)
    {
        masterPrivilegeRight = regionConfig->masterAccRight[masterIdx];

        /* Allocates master number */
        masterAccessRight[masterIdx].masterNum = masterPrivilegeRight.masterNum;
        /* Allocates access rights */
        masterAccessRight[masterIdx].accessRight = (mpu_access_rights_t)masterPrivilegeRight.accessRight;
    #if FEATURE_MPU_HAS_PROCESS_IDENTIFIER
        /* Allocates process identifier */
        masterAccessRight[masterIdx].processIdentifierEnable = ((regionConfig->processIdEnable
                                                                 & (1UL << masterPrivilegeRight.masterNum))
                                                                != 0U);
    #endif /* FEATURE_MPU_HAS_PROCESS_IDENTIFIER */
    }

    /* Allocates master configuration in region */
    configResult->masterAccRight = masterAccessRight;

#if FEATURE_MPU_HAS_PROCESS_IDENTIFIER
    /* Allocates process identifier */
    configResult->processIdentifier = regionConfig->processIdentifier;
    configResult->processIdMask = regionConfig->processIdMask;
#endif /* FEATURE_MPU_HAS_PROCESS_IDENTIFIER */
}
#elif defined(MPU_OVER_SMPU)
/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_ConvertSmpuErrorConfig
 * Description   : Converts SMPU error structure.
 *
 *END**************************************************************************/
static void MPU_ConvertSmpuErrorConfig(const smpu_access_err_info_t * smpuErrPtr,
                                       mpu_error_info_t * errResult)
{
    /* Converts error information */
    errResult->master = smpuErrPtr->master;
    errResult->overrun = smpuErrPtr->Overrun;
    errResult->attributes = (mpu_error_attributes_t)smpuErrPtr->attributes;
    errResult->accessType = (mpu_error_access_type_t)smpuErrPtr->accessType;
    errResult->accessCtr = smpuErrPtr->accessCtr;
    errResult->addr = smpuErrPtr->addr;
#if FEATURE_SMPU_HAS_PROCESS_IDENTIFIER
    errResult->processId = smpuErrPtr->processorIdentification;
#endif /* FEATURE_SMPU_HAS_PROCESS_IDENTIFIER */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_ConvertSmpuRegionConfig
 * Description   : Converts SMPU region structure.
 *
 *END**************************************************************************/
static void MPU_ConvertSmpuRegionConfig(const mpu_region_config_t * regionConfig,
                                        smpu_master_access_right_t * masterAccessRight,
                                        smpu_user_config_t * configResult)
{
    uint8_t masterIdx = 0U;
    mpu_master_privilege_right_t masterPrivilegeRight;

    /* Allocates region address */
    configResult->startAddr = regionConfig->startAddr;
    configResult->endAddr = regionConfig->endAddr;

    /* Allocates access rights for masters */
    for (masterIdx = 0U; masterIdx < FEATURE_SMPU_MASTER_COUNT; masterIdx++)
    {
        masterPrivilegeRight = regionConfig->masterAccRight[masterIdx];

        /* Allocates master number */
        masterAccessRight[masterIdx].masterNum = masterPrivilegeRight.masterNum;
        /* Allocates access rights */
        masterAccessRight[masterIdx].accessRight = (smpu_access_rights_t)masterPrivilegeRight.accessRight;
    }

    /* Allocates master configuration in region */
    configResult->masterAccRight = masterAccessRight;

#if FEATURE_SMPU_HAS_PROCESS_IDENTIFIER
    /* Allocates process identifier */
    configResult->processIdEnable = (regionConfig->processIdEnable != 0U);
    configResult->processIdentifier = regionConfig->processIdentifier;
    configResult->processIdMask = regionConfig->processIdMask;
#endif /* FEATURE_SMPU_HAS_PROCESS_IDENTIFIER */

    if (regionConfig->extension != NULL)
    {
        configResult->cacheInhibitEnable = ((mpu_extension_smpu_region_t *)regionConfig->extension)->cacheInhibitEnable;
        configResult->lockConfig = (smpu_lock_t)((mpu_extension_smpu_region_t *)regionConfig->extension)->lockConfig;
    }
    else
    {
        configResult->cacheInhibitEnable = true;
        configResult->lockConfig = SMPU_UNLOCK;
    }
}
#endif /* defined(MPU_OVER_MPU) & defined(MPU_OVER_SMPU) */

/*******************************************************************************
 * Code
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_Init
 * Description   : Initializes memory protection unit by allocating regions
 *                 and granting access rights for masters.
 *
 * Implements    : MPU_Init_Activity
 *END**************************************************************************/
status_t MPU_Init(mpu_instance_t instance,
                  uint8_t regionCnt,
                  const mpu_region_config_t * configPtr)
{
    DEV_ASSERT(configPtr != NULL);
    DEV_ASSERT(regionCnt > 0U);

    status_t status = STATUS_ERROR;
    uint8_t regionIdx = 0U;

#if defined(MPU_OVER_MPU)
    DEV_ASSERT(regionCnt <= MPU_RGD_COUNT);

    mpu_user_config_t mpuConfig[MPU_RGD_COUNT];
    mpu_master_access_right_t mpuMasterConfig[MPU_RGD_COUNT * FEATURE_MPU_MASTER_COUNT];

    /* Allocates all region configuration */
    for (regionIdx = 0U; regionIdx < regionCnt; regionIdx++)
    {
        MPU_ConvertMpuRegionConfig(&configPtr[regionIdx],
                                   &mpuMasterConfig[regionIdx * FEATURE_MPU_MASTER_COUNT],
                                   &mpuConfig[regionIdx]);
    }
    /* Initializes MPU module */
    status = MPU_DRV_Init((uint32_t)instance, regionCnt, mpuConfig);

#elif defined(MPU_OVER_SMPU)
    DEV_ASSERT(regionCnt <= SMPU_RGD_COUNT);

    smpu_user_config_t smpuConfig[SMPU_RGD_COUNT];
    smpu_master_access_right_t smpuMasterConfig[SMPU_RGD_COUNT * FEATURE_SMPU_MASTER_COUNT];

    /* Allocates all region configuration */
    for (regionIdx = 0U; regionIdx < regionCnt; regionIdx++)
    {
        MPU_ConvertSmpuRegionConfig(&configPtr[regionIdx],
                                    &smpuMasterConfig[regionIdx * FEATURE_SMPU_MASTER_COUNT],
                                    &smpuConfig[regionIdx]);
    }
    /* Initializes SMPU module */
    status = SMPU_DRV_Init((uint32_t)instance, regionCnt, smpuConfig);

#endif /* defined(MPU_OVER_MPU) & defined(MPU_OVER_SMPU) */

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_Deinit
 * Description   : De-initializes memory protection unit by reseting all regions
 *                 and masters to default and disable module.
 *
 * Implements    : MPU_Deinit_Activity
 *END**************************************************************************/
status_t MPU_Deinit(mpu_instance_t instance)
{
    status_t status = STATUS_ERROR;

#if defined(MPU_OVER_MPU)
    /* De-initializes MPU module */
    MPU_DRV_Deinit((uint32_t)instance);
    status = STATUS_SUCCESS;

#elif defined(MPU_OVER_SMPU)
    /* De-initializes SMPU module */
    status = SMPU_DRV_Deinit((uint32_t)instance);

#endif /* defined(MPU_OVER_MPU) & defined(MPU_OVER_SMPU) */

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_GetDefautRegionConfig
 * Description   : Gets default region configuration. Grants all access rights for masters;
 * disable PID and cache inhibit; unlock region descriptor.
 * Please note that the length of master array should be defined by number of masters supported by hardware.
 *
 * Implements    : MPU_GetDefautRegionConfig_Activity
 *END**************************************************************************/
void MPU_GetDefautRegionConfig(mpu_master_privilege_right_t * masterAccRight,
                               mpu_region_config_t * regionConfig)
{
    DEV_ASSERT(masterAccRight != NULL);
    DEV_ASSERT(regionConfig != NULL);

    uint8_t masterIdx = 0U;

#if defined(MPU_OVER_MPU)
    uint8_t masterNum[FEATURE_MPU_MASTER_COUNT] = FEATURE_MPU_MASTER;

    /* Gets default access rights for all masters */
    for (masterIdx = 0U; masterIdx < FEATURE_MPU_MASTER_COUNT; masterIdx++)
    {
        masterAccRight[masterIdx].masterNum = masterNum[masterIdx];
        if (masterNum[masterIdx] <= FEATURE_MPU_MAX_LOW_MASTER_NUMBER)
        {
            masterAccRight[masterIdx].accessRight = MPU_SUPERVISOR_RWX_USER_RWX;
        }
        else
        {
            masterAccRight[masterIdx].accessRight = MPU_RW;
        }
    }
#elif defined(MPU_OVER_SMPU)
    uint8_t masterNum[FEATURE_SMPU_MASTER_COUNT] = FEATURE_SMPU_MASTER;

    /* Gets default access rights for all masters */
    for (masterIdx = 0U; masterIdx < FEATURE_SMPU_MASTER_COUNT; masterIdx++)
    {
        masterAccRight->masterNum = masterNum[masterIdx];
        masterAccRight[masterIdx].accessRight = MPU_RW;
    }
#endif /* defined(MPU_OVER_MPU) & defined(MPU_OVER_SMPU) */

    /* Gets default region configuration */
    regionConfig->startAddr = 0U;
    regionConfig->endAddr = 0xFFFFFFFFU;
    regionConfig->masterAccRight = masterAccRight;
    regionConfig->processIdEnable = 0U;
    regionConfig->processIdentifier = 0U;
    regionConfig->processIdMask = 0U;
    regionConfig->extension = NULL;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_UpdateRegion
 * Description   : Updates region configuration.
 *
 * Implements    : MPU_UpdateRegion_Activity
 *END**************************************************************************/
status_t MPU_UpdateRegion(mpu_instance_t instance,
                          uint8_t regionNum,
                          const mpu_region_config_t * configPtr)
{
    DEV_ASSERT(configPtr != NULL);

    status_t status = STATUS_ERROR;

#if defined(MPU_OVER_MPU)
    mpu_user_config_t mpuConfig;
    mpu_master_access_right_t mpuMasterConfig[FEATURE_MPU_MASTER_COUNT];

    /* Allocates region configuration */
    MPU_ConvertMpuRegionConfig(configPtr, mpuMasterConfig, &mpuConfig);
    /* Updates region configuration */
    status = MPU_DRV_SetRegionConfig((uint32_t)instance, regionNum, &mpuConfig);

#elif defined(MPU_OVER_SMPU)
    smpu_user_config_t smpuConfig;
    smpu_master_access_right_t smpuMasterConfig[FEATURE_SMPU_MASTER_COUNT];

    /* Allocates region configuration */
    MPU_ConvertSmpuRegionConfig(configPtr, smpuMasterConfig, &smpuConfig);
    /* Updates region configuration */
    status = SMPU_DRV_SetRegionConfig((uint32_t)instance, regionNum, &smpuConfig);

#endif /* defined(MPU_OVER_MPU) & defined(MPU_OVER_SMPU) */

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_EnableRegion
 * Description   : Enables or disables an exist region configuration.
 *
 * Implements    : MPU_EnableRegion_Activity
 *END**************************************************************************/
status_t MPU_EnableRegion(mpu_instance_t instance,
                          uint8_t regionNum,
                          bool enable)
{
    status_t status = STATUS_ERROR;

#if defined(MPU_OVER_MPU)
    /* Enables region */
    MPU_DRV_EnableRegion((uint32_t)instance, regionNum, enable);
    status = STATUS_SUCCESS;

#elif defined(MPU_OVER_SMPU)
    /* Enables region */
    status = SMPU_DRV_EnableRegion((uint32_t)instance, regionNum, enable);

#endif /* defined(MPU_OVER_SMPU) */

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_GetError
 * Description   : Checks and gets the access error detail information
 * then clear error flag if the error caused by a master.
 *
 * Implements    : MPU_GetError_Activity
 *END**************************************************************************/
bool MPU_GetError(mpu_instance_t instance,
                  uint8_t channel,
                  mpu_error_info_t * errPtr)
{
    DEV_ASSERT(errPtr != NULL);

    bool status = false;

#if defined(MPU_OVER_MPU)
    mpu_access_err_info_t mpuErrPtr;

    /* Gets detail error information */
    status = MPU_DRV_GetDetailErrorAccessInfo((uint32_t)instance, channel, &mpuErrPtr);

    if (status)
    {
        MPU_ConvertMpuErrorConfig(&mpuErrPtr, errPtr);
    }

#elif defined(MPU_OVER_SMPU)
    smpu_access_err_info_t smpuErrPtr;

    /* Gets detail error information */
    status = SMPU_DRV_GetDetailErrorInfo((uint32_t)instance, channel, &smpuErrPtr);

    if (status)
    {
        MPU_ConvertSmpuErrorConfig(&smpuErrPtr, errPtr);
    }

#endif /* defined(MPU_OVER_MPU) & defined(MPU_OVER_SMPU) */

    return status;
}

#if defined(MPU_OVER_SMPU)
/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_UpdateRegionLock
 * Description   : Updates lock configuration on a region.
 *
 * Implements    : MPU_UpdateRegionLock_Activity
 *END**************************************************************************/
status_t MPU_UpdateRegionLock(mpu_instance_t instance,
                              uint8_t regionNum,
                              mpu_lock_t lockConfig)
{
    status_t status = STATUS_ERROR;

    /* Sets region lock configuration */
    status = SMPU_DRV_SetRegionLockConfig((uint32_t)instance, regionNum, (smpu_lock_t)lockConfig);

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_GetRegionLockInfo
 * Description   : Reports lock status on a region.
 *
 * Implements    : MPU_GetRegionLockInfo_Activity
 *END**************************************************************************/
void MPU_GetRegionLockInfo(mpu_instance_t instance,
                           uint8_t regionNum,
                           mpu_region_lock_t * regionLock)
{
    DEV_ASSERT(regionLock != NULL);

    smpu_region_lock_t smpuRegionLock;

    /* Gets region lock status */
    smpuRegionLock = SMPU_DRV_GetRegionLockInfo((uint32_t)instance, regionNum);

    /* Converts to MPU region lock status structure */
    regionLock->regionNum = smpuRegionLock.regionNum;
#if FEATURE_SMPU_HAS_OWNER_LOCK
    regionLock->masterOwner = smpuRegionLock.masterOwner;
#endif /* FEATURE_SMPU_HAS_OWNER_LOCK */
    regionLock->lockConfig = (mpu_lock_t)smpuRegionLock.lockConfig;
}
#endif /* defined(MPU_OVER_SMPU) */

/*******************************************************************************
 * EOF
 ******************************************************************************/
