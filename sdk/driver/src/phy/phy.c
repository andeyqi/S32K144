/*
 * Copyright 2107 NXP
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

#include <stdint.h>

#include "device_registers.h"
#include "status.h"

#include "phy.h"
#include "enet_driver.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define PHY_MDIO_TIMEOUT	(2U)

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Pointer to an array of pointers towards the driver structures */
static phy_driver_t** s_phyDrivers;
/*! @brief Pointer to an array of PHY configuration structures */
phy_config_t* g_phyConfig;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_FrameworkInit
 * Description   : Initializes the PHY driver.
 *
 *END**************************************************************************/
status_t PHY_FrameworkInit(phy_config_t* phyConfig, phy_driver_t** phyDrivers)
{
    /* Store pointers to configuration and driver structures */
    s_phyDrivers = phyDrivers;
    g_phyConfig = phyConfig;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_Init
 * Description   : Initializes the specified PHY device.
 *
 *END**************************************************************************/
status_t PHY_Init(uint8_t phy)
{
    return s_phyDrivers[g_phyConfig[phy].type]->init(phy);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_Reset
 * Description   : Resets the specified PHY device.
 *
 *END**************************************************************************/
status_t PHY_Reset(uint8_t phy)
{
    return s_phyDrivers[g_phyConfig[phy].type]->reset(phy);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_MainFunction
 * Description   : Provides polling support, by handling specific events.
 *
 *END**************************************************************************/
status_t PHY_MainFunction(uint8_t phy)
{
    return s_phyDrivers[g_phyConfig[phy].type]->mainFunction(phy);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_GetId
 * Description   : Gets the ID of the specified PHY device.
 *
 *END**************************************************************************/
status_t PHY_GetId(uint8_t phy, phy_id_t* id)
{
    return s_phyDrivers[g_phyConfig[phy].type]->getId(phy, id);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_GetLinkStatus
 * Description   : Gets the link status for the specified PHY device.
 *
 *END**************************************************************************/
status_t PHY_GetLinkStatus(uint8_t phy, bool* linkUp)
{
    return s_phyDrivers[g_phyConfig[phy].type]->getLinkStatus(phy, linkUp);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_GetLinkSpeed
 * Description   : Gets the link speed for the specified PHY device.
 *
 *END**************************************************************************/
status_t PHY_GetLinkSpeed(uint8_t phy, phy_speed_t* speed)
{
    return s_phyDrivers[g_phyConfig[phy].type]->getLinkSpeed(phy, speed);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_Suspend
 * Description   : Suspends the specified PHY device.
 *
 *END**************************************************************************/
status_t PHY_Suspend(uint8_t phy)
{
    return s_phyDrivers[g_phyConfig[phy].type]->suspend(phy);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_Resume
 * Description   : Resumes the specified PHY device.
 *
 *END**************************************************************************/
status_t PHY_Resume(uint8_t phy)
{
    return s_phyDrivers[g_phyConfig[phy].type]->resume(phy);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_Sleep
 * Description   : Sends a sleep request to the specified PHY device.
 *
 *END**************************************************************************/
status_t PHY_Sleep(uint8_t phy)
{
    return s_phyDrivers[g_phyConfig[phy].type]->sleep(phy);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_Wakeup
 * Description   : Wakes up the specified PHY device.
 *
 *END**************************************************************************/
status_t PHY_Wakeup(uint8_t phy)
{
    return s_phyDrivers[g_phyConfig[phy].type]->wakeup(phy);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_SetRole
 * Description   : Sets the role of the specified PHY device.
 *
 *END**************************************************************************/
status_t PHY_SetRole(uint8_t phy, phy_role_t role)
{
    return s_phyDrivers[g_phyConfig[phy].type]->setRole(phy, role);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_SetLoopback
 * Description   : Sets the loopback mode for the specified PHY device.
 *
 *END**************************************************************************/
status_t PHY_SetLoopback(uint8_t phy, phy_loopback_t loopbackMode)
{
    return s_phyDrivers[g_phyConfig[phy].type]->setLoopback(phy, loopbackMode);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_EnableInterrupts
 * Description   : Enables interrupts for the specified PHY device.
 *
 *END**************************************************************************/
status_t PHY_EnableInterrupts(uint8_t phy)
{
    return s_phyDrivers[g_phyConfig[phy].type]->enableInterrupts(phy);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_HandleInterrupt
 * Description   : Handles interrupts for the specified PHY device.
 *
 *END**************************************************************************/
status_t PHY_HandleInterrupt(uint8_t phy)
{
    return s_phyDrivers[g_phyConfig[phy].type]->handleInterrupt(phy);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_EnableAutoneg
 * Description   : Enables auto-negotiation for the specified PHY device.
 *
 *END**************************************************************************/
status_t PHY_EnableAutoneg(uint8_t phy)
{
    return s_phyDrivers[g_phyConfig[phy].type]->enableAutoneg(phy);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_DisableAutoneg
 * Description   : Disables auto-negotiation for the specified PHY device.
 *
 *END**************************************************************************/
status_t PHY_DisableAutoneg(uint8_t phy)
{
    return s_phyDrivers[g_phyConfig[phy].type]->disableAutoneg(phy);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_RestartAutoneg
 * Description   : Restarts auto-negotiation for the specified PHY device.
 *
 *END**************************************************************************/
status_t PHY_RestartAutoneg(uint8_t phy)
{
    return s_phyDrivers[g_phyConfig[phy].type]->restartAutoneg(phy);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_Write
 * Description   : Writes a register of the specified PHY device.
 *
 *END**************************************************************************/
status_t PHY_Write(uint8_t phy, uint8_t registerAddr, uint16_t value)
{
    if (g_phyConfig[phy].location == PHY_LOCATION_INTERNAL)
    {
        *(volatile uint16_t *)(g_phyConfig[phy].addr + registerAddr) = value;
    }
    else
    {
        if (ENET_DRV_MDIOWrite(g_phyConfig[phy].enetInstance, (uint8_t)g_phyConfig[phy].addr, registerAddr, value, PHY_MDIO_TIMEOUT) != STATUS_SUCCESS)
        {
            return STATUS_PHY_ACCESS_FAILED;
        }
    }
    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_Read
 * Description   : Reads a register of the specified PHY device.
 *
 *END**************************************************************************/
status_t PHY_Read(uint8_t phy, uint8_t registerAddr, uint16_t* value)
{
    if (g_phyConfig[phy].location == PHY_LOCATION_INTERNAL)
    {
        *value = *(volatile uint16_t *)(g_phyConfig[phy].addr + registerAddr);
    }
    else
    {
        if (ENET_DRV_MDIORead(g_phyConfig[phy].enetInstance, (uint8_t)g_phyConfig[phy].addr, registerAddr, value, PHY_MDIO_TIMEOUT) != STATUS_SUCCESS)
        {
            return STATUS_PHY_ACCESS_FAILED;
        }
    }
    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_RMR
 * Description   : Reads and modifies a register of the specified PHY device.
 *
 *END**************************************************************************/
status_t PHY_RMR(uint8_t phy, uint8_t registerAddr, uint16_t value, uint16_t mask)
{
    status_t phyStatus;
    uint16_t reg;

    /* Read */
    phyStatus = PHY_Read(phy, registerAddr, &reg);
    if (phyStatus != STATUS_SUCCESS)
    {
        return phyStatus;
    }
    /* Modify */
    reg &= (uint16_t)~mask;           /* Clear masked bits */
    reg |= (uint16_t)(mask & value);  /* Set masked bits to value */

    /* Write */
    phyStatus = PHY_Write(phy, registerAddr, reg);

    return phyStatus;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
