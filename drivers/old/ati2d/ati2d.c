

#define R300_PIO        0


#define API_VERSION     0x01000100

#define SRV_GETVERSION  0


#include "types.h"

#include <stdio.h>
#include <malloc.h>
#include <memory.h>

#include "pci.h"

#include "syscall.h"

#include "radeon_reg.h"

#include "atihw.h"

#include "accel_2d.h"

RHD_t rhd __attribute__ ((aligned (128)));    /* reduce cache lock */

static clip_t  clip;

static local_pixmap_t scr_pixmap;


int __stdcall srv_2d(ioctl_t *io);

u32_t __stdcall drvEntry(int action)
{
    RHDPtr rhdPtr;
    u32_t retval;

    int i;

    if(action != 1)
        return 0;

    if(!dbg_open("/sys/drivers/ati2d.log"))
    {
        printf("Can't open /sys/drivers/ati2d.log\nExit\n");
        return 0;
    }
    if( GetScreenBpp() != 32)
    {
        dbgprintf("32 bpp dispaly mode required !\nExit\t");
        return 0;
    }

    if((rhdPtr=FindPciDevice())==NULL)
    {
        dbgprintf("Device not found\n");
        return 0;
    };

    dbgprintf("detect %s GART\n",
               rhd.gart_type == RADEON_IS_PCIE ? "PCIE":"PCI");

    for(i=0;i<6;i++)
    {
        if(rhd.memBase[i])
            dbgprintf("Memory base_%d 0x%x size 0x%x\n",
                      i,rhd.memBase[i],(1<<rhd.memsize[i]));
    };
    for(i=0;i<6;i++)
    {
        if(rhd.ioBase[i])
            dbgprintf("Io base_%d 0x%x size 0x%x\n",
                      i,rhd.ioBase[i],(1<<rhd.memsize[i]));
    };
    if(!RHDPreInit())
        return 0;

    R5xx2DInit();

    Init3DEngine(&rhd);

    retval = RegService("HDRAW", srv_2d);
    dbgprintf("reg service %s as: %x\n", "HDRAW", retval);

    return retval;
};


int __stdcall srv_2d(ioctl_t *io)
{
    u32_t *inp;
    u32_t *outp;

    inp = io->input;
    outp = io->output;

    switch(io->io_code)
    {
        case SRV_GETVERSION:
            if(io->out_size==4)
            {
                *outp = API_VERSION;
                return 0;
            }
            break;

        case PX_CREATE:
            if(io->inp_size==7)
                return CreatePixmap((pixmap_t*)inp);
            break;

        case PX_DESTROY:
            if(io->inp_size==7)
                return DestroyPixmap((pixmap_t*)inp);
            break;

        case PX_CLEAR:
            if(io->inp_size==2)
                return ClearPixmap((io_clear_t*)inp);
            break;

        case PX_DRAW_RECT:
            if(io->inp_size==7)
                return DrawRect((io_draw_t*)inp);
            break;

        case PX_FILL_RECT:
            if(io->inp_size==10)
                return FillRect((io_fill_t*)inp);
            break;

        case PX_LINE:
            if(io->inp_size==6)
                return Line((io_draw_t*)inp);
            break;

        case PX_BLIT:
            if(io->inp_size==8)
                return Blit((io_blit_t*)inp);
            break;

        case  PX_BLIT_TRANSPARENT:
            if(io->inp_size==9)
                return BlitTransparent((io_blit_t*)inp);
            break;

        case PX_BLIT_ALPHA:
            if(io->inp_size==9)
                return RadeonComposite((io_blit_t*)inp);
            break;

        default:
            return ERR_PARAM;
  };

  return ERR_PARAM;
}


#include "init.c"
#include "pci.c"
#include "ati_mem.c"

#include "r500.inc"

#include "clip.inc"
#include "pixmap.inc"
#include "accel_2d.inc"
#include "init_3d.inc"
#include "blend.inc"

#if !R300_PIO

#include "init_cp.c"

#endif


