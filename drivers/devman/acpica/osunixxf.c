/******************************************************************************
 *
 * Module Name: osunixxf - UNIX OSL interfaces
 *
 *****************************************************************************/

/******************************************************************************
 *
 * 1. Copyright Notice
 *
 * Some or all of this work - Copyright (c) 1999 - 2011, Intel Corp.
 * All rights reserved.
 *
 * 2. License
 *
 * 2.1. This is your license from Intel Corp. under its intellectual property
 * rights.  You may have additional license terms from the party that provided
 * you this software, covering your right to use that party's intellectual
 * property rights.
 *
 * 2.2. Intel grants, free of charge, to any person ("Licensee") obtaining a
 * copy of the source code appearing in this file ("Covered Code") an
 * irrevocable, perpetual, worldwide license under Intel's copyrights in the
 * base code distributed originally by Intel ("Original Intel Code") to copy,
 * make derivatives, distribute, use and display any portion of the Covered
 * Code in any form, with the right to sublicense such rights; and
 *
 * 2.3. Intel grants Licensee a non-exclusive and non-transferable patent
 * license (with the right to sublicense), under only those claims of Intel
 * patents that are infringed by the Original Intel Code, to make, use, sell,
 * offer to sell, and import the Covered Code and derivative works thereof
 * solely to the minimum extent necessary to exercise the above copyright
 * license, and in no event shall the patent license extend to any additions
 * to or modifications of the Original Intel Code.  No other license or right
 * is granted directly or by implication, estoppel or otherwise;
 *
 * The above copyright and patent license is granted only if the following
 * conditions are met:
 *
 * 3. Conditions
 *
 * 3.1. Redistribution of Source with Rights to Further Distribute Source.
 * Redistribution of source code of any substantial portion of the Covered
 * Code or modification with rights to further distribute source must include
 * the above Copyright Notice, the above License, this list of Conditions,
 * and the following Disclaimer and Export Compliance provision.  In addition,
 * Licensee must cause all Covered Code to which Licensee contributes to
 * contain a file documenting the changes Licensee made to create that Covered
 * Code and the date of any change.  Licensee must include in that file the
 * documentation of any changes made by any predecessor Licensee.  Licensee
 * must include a prominent statement that the modification is derived,
 * directly or indirectly, from Original Intel Code.
 *
 * 3.2. Redistribution of Source with no Rights to Further Distribute Source.
 * Redistribution of source code of any substantial portion of the Covered
 * Code or modification without rights to further distribute source must
 * include the following Disclaimer and Export Compliance provision in the
 * documentation and/or other materials provided with distribution.  In
 * addition, Licensee may not authorize further sublicense of source of any
 * portion of the Covered Code, and must include terms to the effect that the
 * license from Licensee to its licensee is limited to the intellectual
 * property embodied in the software Licensee provides to its licensee, and
 * not to intellectual property embodied in modifications its licensee may
 * make.
 *
 * 3.3. Redistribution of Executable. Redistribution in executable form of any
 * substantial portion of the Covered Code or modification must reproduce the
 * above Copyright Notice, and the following Disclaimer and Export Compliance
 * provision in the documentation and/or other materials provided with the
 * distribution.
 *
 * 3.4. Intel retains all right, title, and interest in and to the Original
 * Intel Code.
 *
 * 3.5. Neither the name Intel nor any other trademark owned or controlled by
 * Intel shall be used in advertising or otherwise to promote the sale, use or
 * other dealings in products derived from or relating to the Covered Code
 * without prior written authorization from Intel.
 *
 * 4. Disclaimer and Export Compliance
 *
 * 4.1. INTEL MAKES NO WARRANTY OF ANY KIND REGARDING ANY SOFTWARE PROVIDED
 * HERE.  ANY SOFTWARE ORIGINATING FROM INTEL OR DERIVED FROM INTEL SOFTWARE
 * IS PROVIDED "AS IS," AND INTEL WILL NOT PROVIDE ANY SUPPORT,  ASSISTANCE,
 * INSTALLATION, TRAINING OR OTHER SERVICES.  INTEL WILL NOT PROVIDE ANY
 * UPDATES, ENHANCEMENTS OR EXTENSIONS.  INTEL SPECIFICALLY DISCLAIMS ANY
 * IMPLIED WARRANTIES OF MERCHANTABILITY, NONINFRINGEMENT AND FITNESS FOR A
 * PARTICULAR PURPOSE.
 *
 * 4.2. IN NO EVENT SHALL INTEL HAVE ANY LIABILITY TO LICENSEE, ITS LICENSEES
 * OR ANY OTHER THIRD PARTY, FOR ANY LOST PROFITS, LOST DATA, LOSS OF USE OR
 * COSTS OF PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES, OR FOR ANY INDIRECT,
 * SPECIAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THIS AGREEMENT, UNDER ANY
 * CAUSE OF ACTION OR THEORY OF LIABILITY, AND IRRESPECTIVE OF WHETHER INTEL
 * HAS ADVANCE NOTICE OF THE POSSIBILITY OF SUCH DAMAGES.  THESE LIMITATIONS
 * SHALL APPLY NOTWITHSTANDING THE FAILURE OF THE ESSENTIAL PURPOSE OF ANY
 * LIMITED REMEDY.
 *
 * 4.3. Licensee shall not export, either directly or indirectly, any of this
 * software or system incorporating such software without first obtaining any
 * required license or other approval from the U. S. Department of Commerce or
 * any other agency or department of the United States Government.  In the
 * event Licensee exports any such software from the United States or
 * re-exports any such software from a foreign destination, Licensee shall
 * ensure that the distribution and export/re-export of the software is in
 * compliance with all laws, regulations, orders, or other restrictions of the
 * U.S. Export Administration Regulations. Licensee agrees that neither it nor
 * any of its subsidiaries will export/re-export any technical data, process,
 * software, or service, directly or indirectly, to any country for which the
 * United States government or any agency thereof requires an export license,
 * other governmental approval, or letter of assurance, without first obtaining
 * such license, approval or letter.
 *
 *****************************************************************************/

#include "acpi.h"
#include "accommon.h"
#include "amlcode.h"
#include "acparser.h"
#include "acdebug.h"

/*
 * These interfaces are required in order to compile the ASL compiler under
 * Linux or other Unix-like system.
 */
/*
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/time.h>
#include <semaphore.h>
#include <pthread.h>
*/

#define _COMPONENT          ACPI_OS_SERVICES
ACPI_MODULE_NAME            ("oskolibri")


extern void                    *AcpiGbl_DebugFile;
void                           *AcpiGbl_OutputFile;

static UINT32 sACPIRoot = 0;


void
AeTableOverride (
    ACPI_TABLE_HEADER       *ExistingTable,
    ACPI_TABLE_HEADER       **NewTable);

typedef void* (*PTHREAD_CALLBACK) (void *);


/******************************************************************************
 *
 * FUNCTION:    AcpiOsInitialize, AcpiOsTerminate
 *
 * PARAMETERS:  None
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Init and terminate.  Nothing to do.
 *
 *****************************************************************************/

ACPI_STATUS
AcpiOsInitialize (void)
{
    if(!dbg_open(ACPI_LOG))
    {
        dbgprintf("Can't open %s\nExit\n", ACPI_LOG);
        return AE_ERROR;
    }

    return (AE_OK);
}


ACPI_STATUS
AcpiOsTerminate (void)
{

    return (AE_OK);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiOsGetRootPointer
 *
 * PARAMETERS:  None
 *
 * RETURN:      RSDP physical address
 *
 * DESCRIPTION: Gets the root pointer (RSDP)
 *
 *****************************************************************************/

ACPI_PHYSICAL_ADDRESS
AcpiOsGetRootPointer (
    void)
{
    ACPI_SIZE address;
    ACPI_STATUS status;
    if (sACPIRoot == 0) {
        status = AcpiFindRootPointer(&address);
        if (status == AE_OK)
            sACPIRoot = address;
    }
    dbgprintf("AcpiOsGetRootPointer returning %p\n", (void *)sACPIRoot);
    return sACPIRoot;
}


/******************************************************************************
 *
 * FUNCTION:    AcpiOsPredefinedOverride
 *
 * PARAMETERS:  InitVal             - Initial value of the predefined object
 *              NewVal              - The new value for the object
 *
 * RETURN:      Status, pointer to value. Null pointer returned if not
 *              overriding.
 *
 * DESCRIPTION: Allow the OS to override predefined names
 *
 *****************************************************************************/

ACPI_STATUS
AcpiOsPredefinedOverride (
    const ACPI_PREDEFINED_NAMES *InitVal,
    ACPI_STRING                 *NewVal)
{

    if (!InitVal || !NewVal)
    {
        return (AE_BAD_PARAMETER);
    }

    *NewVal = NULL;
    return (AE_OK);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiOsTableOverride
 *
 * PARAMETERS:  ExistingTable       - Header of current table (probably
 *                                    firmware)
 *              NewTable            - Where an entire new table is returned.
 *
 * RETURN:      Status, pointer to new table. Null pointer returned if no
 *              table is available to override
 *
 * DESCRIPTION: Return a different version of a table if one is available
 *
 *****************************************************************************/

ACPI_STATUS
AcpiOsTableOverride (
    ACPI_TABLE_HEADER       *ExistingTable,
    ACPI_TABLE_HEADER       **NewTable)
{

    if (!ExistingTable || !NewTable)
    {
        return (AE_BAD_PARAMETER);
    }

    *NewTable = NULL;

#ifdef ACPI_EXEC_APP

    AeTableOverride (ExistingTable, NewTable);
    return (AE_OK);
#else

    return (AE_NO_ACPI_TABLES);
#endif
}


/******************************************************************************
 *
 * FUNCTION:    AcpiOsRedirectOutput
 *
 * PARAMETERS:  Destination         - An open file handle/pointer
 *
 * RETURN:      None
 *
 * DESCRIPTION: Causes redirect of AcpiOsPrintf and AcpiOsVprintf
 *
 *****************************************************************************/

void
AcpiOsRedirectOutput (
    void                    *Destination)
{

    AcpiGbl_OutputFile = Destination;
}


/******************************************************************************
 *
 * FUNCTION:    AcpiOsPrintf
 *
 * PARAMETERS:  fmt, ...            - Standard printf format
 *
 * RETURN:      None
 *
 * DESCRIPTION: Formatted output
 *
 *****************************************************************************/

void ACPI_INTERNAL_VAR_XFACE
AcpiOsPrintf (
    const char              *Fmt,
    ...)
{
    va_list                 Args;


    va_start (Args, Fmt);
    AcpiOsVprintf (Fmt, Args);
    va_end (Args);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiOsVprintf
 *
 * PARAMETERS:  fmt                 - Standard printf format
 *              args                - Argument list
 *
 * RETURN:      None
 *
 * DESCRIPTION: Formatted output with argument list pointer
 *
 *****************************************************************************/

void
AcpiOsVprintf (
    const char              *Fmt,
    va_list                 Args)
{
//    INT32                   Count = 0;
//    UINT8                   Flags;

//   static char outputBuffer[1024];
//   vsnprintf(outputBuffer, 1024, Fmt, Args);
}



/******************************************************************************
 *
 * FUNCTION:    AcpiOsMapMemory
 *
 * PARAMETERS:  where               - Physical address of memory to be mapped
 *              length              - How much memory to map
 *
 * RETURN:      Pointer to mapped memory. Null on error.
 *
 * DESCRIPTION: Map physical memory into caller's address space
 *
 *****************************************************************************/

void *
AcpiOsMapMemory (
    ACPI_PHYSICAL_ADDRESS   where,
    ACPI_SIZE               length)
{


    void* retval=NULL;

    if( (UINT64)where+length <= 0x100000000ULL)
        retval = (void*)MapIoMem(where, 4096+(UINT32)length, 0x07);

//    dbgprintf("%s %x-> %x %x\n",__FUNCTION__,
//              where, retval, length);

    return retval;
}


/******************************************************************************
 *
 * FUNCTION:    AcpiOsUnmapMemory
 *
 * PARAMETERS:  where               - Logical address of memory to be unmapped
 *              length              - How much memory to unmap
 *
 * RETURN:      None.
 *
 * DESCRIPTION: Delete a previously created mapping. Where and Length must
 *              correspond to a previous mapping exactly.
 *
 *****************************************************************************/

void
AcpiOsUnmapMemory (
    void                    *where,
    ACPI_SIZE               length)
{
    FreeKernelSpace( 0xFFFFF000 & (UINT32)where);
    return;
}


/******************************************************************************
 *
 * FUNCTION:    AcpiOsAllocate
 *
 * PARAMETERS:  Size                - Amount to allocate, in bytes
 *
 * RETURN:      Pointer to the new allocation. Null on error.
 *
 * DESCRIPTION: Allocate memory. Algorithm is dependent on the OS.
 *
 *****************************************************************************/

void *
AcpiOsAllocate (
    ACPI_SIZE               size)
{
    void                    *Mem;


    Mem = (void *) malloc ((size_t) size);
    return (Mem);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiOsFree
 *
 * PARAMETERS:  mem                 - Pointer to previously allocated memory
 *
 * RETURN:      None.
 *
 * DESCRIPTION: Free memory allocated via AcpiOsAllocate
 *
 *****************************************************************************/

void
AcpiOsFree (
    void                    *mem)
{

    free (mem);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiOsCreateSemaphore
 *
 * PARAMETERS:  InitialUnits        - Units to be assigned to the new semaphore
 *              OutHandle           - Where a handle will be returned
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Create an OS semaphore
 *
 *****************************************************************************/

ACPI_STATUS
AcpiOsCreateSemaphore (
    UINT32              MaxUnits,
    UINT32              InitialUnits,
    ACPI_HANDLE         *OutHandle)
{



    if (!OutHandle)
    {
        return (AE_BAD_PARAMETER);
    }

    *OutHandle = (ACPI_HANDLE) 1;

    return (AE_OK);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiOsDeleteSemaphore
 *
 * PARAMETERS:  Handle              - Handle returned by AcpiOsCreateSemaphore
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Delete an OS semaphore
 *
 *****************************************************************************/

ACPI_STATUS
AcpiOsDeleteSemaphore (
    ACPI_HANDLE         Handle)
{

    if (!Handle)
    {
        return (AE_BAD_PARAMETER);
    }

    return (AE_OK);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiOsWaitSemaphore
 *
 * PARAMETERS:  Handle              - Handle returned by AcpiOsCreateSemaphore
 *              Units               - How many units to wait for
 *              Timeout             - How long to wait
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Wait for units
 *
 *****************************************************************************/

ACPI_STATUS
AcpiOsWaitSemaphore (
    ACPI_HANDLE         Handle,
    UINT32              Units,
    UINT16              Timeout)
{
    ACPI_STATUS         Status = AE_OK;

#if 0
    if (!Sem)
    {
        return (AE_BAD_PARAMETER);
    }

    switch (Timeout)
    {
    /*
     * No Wait:
     * --------
     * A zero timeout value indicates that we shouldn't wait - just
     * acquire the semaphore if available otherwise return AE_TIME
     * (a.k.a. 'would block').
     */
    case 0:

        if (sem_trywait(Sem) == -1)
        {
            Status = (AE_TIME);
        }
        break;

    /* Wait Indefinitely */

    case ACPI_WAIT_FOREVER:

        if (sem_wait (Sem))
        {
            Status = (AE_TIME);
        }
        break;

    /* Wait with Timeout */

    default:

        T.tv_sec = Timeout / 1000;
        T.tv_nsec = (Timeout - (T.tv_sec * 1000)) * 1000000;

#ifdef ACPI_USE_ALTERNATE_TIMEOUT
        /*
         * Alternate timeout mechanism for environments where
         * sem_timedwait is not available or does not work properly.
         */
        while (Timeout)
        {
            if (sem_trywait (Sem) == 0)
            {
                /* Got the semaphore */
                return (AE_OK);
            }
            usleep (1000);  /* one millisecond */
            Timeout--;
        }
        Status = (AE_TIME);
#else

        if (sem_timedwait (Sem, &T))
        {
            Status = (AE_TIME);
        }
#endif

        break;
    }
#endif

    return (Status);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiOsSignalSemaphore
 *
 * PARAMETERS:  Handle              - Handle returned by AcpiOsCreateSemaphore
 *              Units               - Number of units to send
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Send units
 *
 *****************************************************************************/

ACPI_STATUS
AcpiOsSignalSemaphore (
    ACPI_HANDLE         Handle,
    UINT32              Units)
{

    return (AE_OK);
}


/******************************************************************************
 *
 * FUNCTION:    Spinlock interfaces
 *
 * DESCRIPTION: Map these interfaces to semaphore interfaces
 *
 *****************************************************************************/

ACPI_STATUS
AcpiOsCreateLock (
    ACPI_SPINLOCK           *OutHandle)
{
    if (!OutHandle)
    {
        return (AE_BAD_PARAMETER);
    }

    *OutHandle = (ACPI_HANDLE)malloc(sizeof(UINT32));

	if (*OutHandle == NULL)
		return AE_NO_MEMORY;

    *((UINT32*)(*OutHandle)) = 0;
	return AE_OK;

}


void
AcpiOsDeleteLock (
    ACPI_SPINLOCK           Handle)
{
    free (Handle);
}


ACPI_CPU_FLAGS
AcpiOsAcquireLock (
    ACPI_HANDLE             Handle)
{
    AcpiOsWaitSemaphore (Handle, 1, 0xFFFF);
    return (0);
}


void
AcpiOsReleaseLock (
    ACPI_SPINLOCK           Handle,
    ACPI_CPU_FLAGS          Flags)
{
    AcpiOsSignalSemaphore (Handle, 1);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiOsInstallInterruptHandler
 *
 * PARAMETERS:  InterruptNumber     Level handler should respond to.
 *              Isr                 Address of the ACPI interrupt handler
 *              ExceptPtr           Where status is returned
 *
 * RETURN:      Handle to the newly installed handler.
 *
 * DESCRIPTION: Install an interrupt handler.  Used to install the ACPI
 *              OS-independent handler.
 *
 *****************************************************************************/

UINT32
AcpiOsInstallInterruptHandler (
    UINT32                  InterruptNumber,
    ACPI_OSD_HANDLER        ServiceRoutine,
    void                    *Context)
{
    dbgprintf("%s irq %d\n", InterruptNumber );
    return (AE_OK);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiOsRemoveInterruptHandler
 *
 * PARAMETERS:  Handle              Returned when handler was installed
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Uninstalls an interrupt handler.
 *
 *****************************************************************************/

ACPI_STATUS
AcpiOsRemoveInterruptHandler (
    UINT32                  InterruptNumber,
    ACPI_OSD_HANDLER        ServiceRoutine)
{

    return (AE_OK);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiOsExecute
 *
 * PARAMETERS:  Type            - Type of execution
 *              Function        - Address of the function to execute
 *              Context         - Passed as a parameter to the function
 *
 * RETURN:      Status.
 *
 * DESCRIPTION: Execute a new thread
 *
 *****************************************************************************/

ACPI_STATUS
AcpiOsExecute (
    ACPI_EXECUTE_TYPE       Type,
    ACPI_OSD_EXEC_CALLBACK  Function,
    void                    *Context)
{
//    pthread_t               thread;
//    int                     ret;


//    ret = pthread_create (&thread, NULL, (PTHREAD_CALLBACK) Function, Context);
//    if (ret)
//    {
//        AcpiOsPrintf("Create thread failed");
//    }
    return (0);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiOsStall
 *
 * PARAMETERS:  microseconds        - Time to sleep
 *
 * RETURN:      Blocks until sleep is completed.
 *
 * DESCRIPTION: Sleep at microsecond granularity
 *
 *****************************************************************************/

void
AcpiOsStall (
    UINT32                  microseconds)
{

    if (microseconds)
    {
        usleep (microseconds);
    }
}


/******************************************************************************
 *
 * FUNCTION:    AcpiOsSleep
 *
 * PARAMETERS:  milliseconds        - Time to sleep
 *
 * RETURN:      Blocks until sleep is completed.
 *
 * DESCRIPTION: Sleep at millisecond granularity
 *
 *****************************************************************************/

void
AcpiOsSleep (
    UINT64                  milliseconds)
{
    delay(milliseconds / 8);
}

/******************************************************************************
 *
 * FUNCTION:    AcpiOsGetTimer
 *
 * PARAMETERS:  None
 *
 * RETURN:      Current time in 100 nanosecond units
 *
 * DESCRIPTION: Get the current system time
 *
 *****************************************************************************/

UINT64
AcpiOsGetTimer (void)
{
//    struct timeval          time;


//    gettimeofday (&time, NULL);

    /* Seconds * 10^7 = 100ns(10^-7), Microseconds(10^-6) * 10^1 = 100ns */

//    return (((UINT64) time.tv_sec * 10000000) + ((UINT64) time.tv_usec * 10));

    return 0;
}


/******************************************************************************
 *
 * FUNCTION:    AcpiOsReadPciConfiguration
 *
 * PARAMETERS:  PciId               - Seg/Bus/Dev
 *              Register            - Device Register
 *              Value               - Buffer where value is placed
 *              Width               - Number of bits
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Read data from PCI configuration space
 *
 *****************************************************************************/

ACPI_STATUS
AcpiOsReadPciConfiguration (
    ACPI_PCI_ID             *PciId,
    UINT32                  Register,
    UINT64                  *Value,
    UINT32                  Width)
{
    UINT32 devfn = ((PciId->Device & 0x1f)<<3)|(PciId->Function & 0x07);

    switch (Width)
    {
        case 8:
            *(UINT8 *)Value = PciRead8(PciId->Bus,devfn,Register);
			break;
		case 16:
            *(UINT16 *)Value = PciRead16(PciId->Bus,devfn,Register);
			break;
		case 32:
            *(UINT32 *)Value = PciRead32(PciId->Bus,devfn,Register);
			break;
		default:
            dbgprintf("AcpiOsReadPciConfiguration unhandled value width: %u\n",
                Width);
			return AE_ERROR;
	}

    return (AE_OK);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiOsWritePciConfiguration
 *
 * PARAMETERS:  PciId               - Seg/Bus/Dev
 *              Register            - Device Register
 *              Value               - Value to be written
 *              Width               - Number of bits
 *
 * RETURN:      Status.
 *
 * DESCRIPTION: Write data to PCI configuration space
 *
 *****************************************************************************/

ACPI_STATUS
AcpiOsWritePciConfiguration (
    ACPI_PCI_ID             *PciId,
    UINT32                  Register,
    UINT64                  Value,
    UINT32                  Width)
{
    UINT32 devfn = ((PciId->Device & 0x1f)<<3)|(PciId->Function & 0x07);

    switch (Width)
    {
        case 8:
            PciWrite8(PciId->Bus,devfn,Register,Value);
			break;
		case 16:
            PciWrite16(PciId->Bus,devfn,Register,Value);
			break;
		case 32:
            PciWrite32(PciId->Bus,devfn,Register,Value);
			break;
		default:
            dbgprintf("AcpiOsReadPciConfiguration unhandled value width: %u\n",
                Width);
			return AE_ERROR;
	}

    return (AE_OK);
}

/* TEMPORARY STUB FUNCTION */
void
AcpiOsDerivePciId(
    ACPI_HANDLE             Device,
    ACPI_HANDLE             Region,
    ACPI_PCI_ID             **PciId)
{

}


/******************************************************************************
 *
 * FUNCTION:    AcpiOsReadPort
 *
 * PARAMETERS:  Address             - Address of I/O port/register to read
 *              Value               - Where value is placed
 *              Width               - Number of bits
 *
 * RETURN:      Value read from port
 *
 * DESCRIPTION: Read data from an I/O port or register
 *
 *****************************************************************************/

ACPI_STATUS
AcpiOsReadPort (
    ACPI_IO_ADDRESS         Address,
    UINT32                  *Value,
    UINT32                  Width)
{

    switch (Width)
    {
    case 8:
        *Value = in8(Address);
        break;

    case 16:
        *Value = in16(Address);
        break;

    case 32:
        *Value = in32(Address);
        break;

    default:
        return (AE_BAD_PARAMETER);
    }

//    dbgprintf("%s %x, %x\n",__FUNCTION__, Address, *Value);

    return (AE_OK);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiOsWritePort
 *
 * PARAMETERS:  Address             - Address of I/O port/register to write
 *              Value               - Value to write
 *              Width               - Number of bits
 *
 * RETURN:      None
 *
 * DESCRIPTION: Write data to an I/O port or register
 *
 *****************************************************************************/

ACPI_STATUS
AcpiOsWritePort (
    ACPI_IO_ADDRESS         Address,
    UINT32                  Value,
    UINT32                  Width)
{

	switch (Width) {
		case 8:
            out8(Address, Value);
			break;

		case 16:
            out16(Address,Value);
			break;

		case 32:
            out32(Address,Value);
			break;

		default:
			return (AE_ERROR);
	}

//    dbgprintf("%s %x, %x\n",__FUNCTION__, Address, Value);

	return (AE_OK);
};

/******************************************************************************
 *
 * FUNCTION:    AcpiOsReadMemory
 *
 * PARAMETERS:  Address             - Physical Memory Address to read
 *              Value               - Where value is placed
 *              Width               - Number of bits
 *
 * RETURN:      Value read from physical memory address
 *
 * DESCRIPTION: Read data from a physical memory address
 *
 *****************************************************************************/

ACPI_STATUS
AcpiOsReadMemory (
    ACPI_PHYSICAL_ADDRESS   Address,
    UINT32                  *Value,
    UINT32                  Width)
{
    void        *memptr;
    ACPI_STATUS  status = AE_ERROR;

//    dbgprintf("%s %x\n",__FUNCTION__, Address);

    if( (UINT64)Address+Width > 0x100000000ULL)
        return (AE_BAD_PARAMETER);

    if( Address >= 0x400000)
    {
        memptr = AcpiOsMapMemory(Address, Width);

        if(memptr)
        {
            status = AE_OK;

            switch (Width)
            {
                case 8:
                    *Value = *(UINT8*)Address;
                    break;

                case 16:
                    *Value = *(UINT16*)Address;
                    break;

                case 32:
                    *Value = *(UINT32*)Address;
                    break;

                default:
                    status = (AE_BAD_PARAMETER);
            }
            FreeKernelSpace(memptr);
        }
        return status;
    }
	else
		Address+= 0x80000000;

    switch (Width)
    {
        case 8:
            *Value = *(UINT8*)Address;
            break;

        case 16:
            *Value = *(UINT16*)Address;
            break;

        case 32:
            *Value = *(UINT32*)Address;
            break;

        default:
            return (AE_BAD_PARAMETER);
    }
    return (AE_OK);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiOsWriteMemory
 *
 * PARAMETERS:  Address             - Physical Memory Address to write
 *              Value               - Value to write
 *              Width               - Number of bits
 *
 * RETURN:      None
 *
 * DESCRIPTION: Write data to a physical memory address
 *
 *****************************************************************************/

ACPI_STATUS
AcpiOsWriteMemory (
    ACPI_PHYSICAL_ADDRESS   Address,
    UINT32                  Value,
    UINT32                  Width)
{

    void        *memptr;
    ACPI_STATUS  status = AE_ERROR;

//    dbgprintf("%s %x, %x\n",__FUNCTION__, Address, Value);

    if( (UINT64)Address+Width > 0x100000000ULL)
        return (AE_BAD_PARAMETER);

    if( Address >= 0x400000)
    {
        memptr = AcpiOsMapMemory(Address, Width);

        if(memptr)
        {
            status = AE_OK;

            switch (Width)
            {
                case 8:
                    *(UINT8*)Address = (UINT8)Value;
                    break;

                case 16:
                    *(UINT16*)Address = (UINT16)Value;
                    break;

                case 32:
                    *(UINT32*)Address = Value;
                    break;

                default:
                    status = (AE_BAD_PARAMETER);
            }
            FreeKernelSpace(memptr);
        }
        return status;
    }
	else
		Address+= 0x80000000;

    switch (Width)
    {
    case 8:
        *(UINT8*)Address = (UINT8)Value;
        break;

    case 16:
        *(UINT16*)Address = (UINT16)Value;
        break;

    case 32:
        *(UINT32*)Address = Value;
        break;

    default:
        return (AE_BAD_PARAMETER);
    }

    return (AE_OK);

}


/******************************************************************************
 *
 * FUNCTION:    AcpiOsReadable
 *
 * PARAMETERS:  Pointer             - Area to be verified
 *              Length              - Size of area
 *
 * RETURN:      TRUE if readable for entire length
 *
 * DESCRIPTION: Verify that a pointer is valid for reading
 *
 *****************************************************************************/

BOOLEAN
AcpiOsReadable (
    void                    *Pointer,
    ACPI_SIZE               Length)
{

    return (TRUE);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiOsWritable
 *
 * PARAMETERS:  Pointer             - Area to be verified
 *              Length              - Size of area
 *
 * RETURN:      TRUE if writable for entire length
 *
 * DESCRIPTION: Verify that a pointer is valid for writing
 *
 *****************************************************************************/

BOOLEAN
AcpiOsWritable (
    void                    *Pointer,
    ACPI_SIZE               Length)
{

    return (TRUE);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiOsGetThreadId
 *
 * PARAMETERS:  None
 *
 * RETURN:      Id of the running thread
 *
 * DESCRIPTION: Get the Id of the current (running) thread
 *
 * NOTE:        The environment header should contain this line:
 *                  #define ACPI_THREAD_ID pthread_t
 *
 *****************************************************************************/

ACPI_THREAD_ID
AcpiOsGetThreadId (void)
{

    return ( 1 );
}


/******************************************************************************
 *
 * FUNCTION:    AcpiOsSignal
 *
 * PARAMETERS:  Function            - ACPI CA signal function code
 *              Info                - Pointer to function-dependent structure
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Miscellaneous functions. Example implementation only.
 *
 *****************************************************************************/

ACPI_STATUS
AcpiOsSignal (
    UINT32                  Function,
    void                    *Info)
{

    switch (Function)
    {
    case ACPI_SIGNAL_FATAL:
        break;

    case ACPI_SIGNAL_BREAKPOINT:
        break;

    default:
        break;
    }

    return (AE_OK);
}


