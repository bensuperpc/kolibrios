;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                                                 ;;
;; Copyright (C) KolibriOS team 2004-2009. All rights reserved.    ;;
;; Distributed under terms of the GNU General Public License       ;;
;;                                                                 ;;
;; RTL8029/ne2000 driver for KolibriOS                             ;;
;;                                                                 ;;
;;    Written by hidnplayr@kolibrios.org                           ;;
;;     with help from CleverMouse                                  ;;
;;                                                                 ;;
;;          GNU GENERAL PUBLIC LICENSE                             ;;
;;             Version 2, June 1991                                ;;
;;                                                                 ;;
;; current status (september 2009) - UNSTABLE                      ;;
;;                                                                 ;;
;; based on RTL8029.asm driver for menuetos                        ;;
;; and realtek8029.asm for SolarOS by Eugen Brasoveanu             ;;
;;                                                                 ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;$Revision$

format MS COFF

API_VERSION	equ 0x01000100

DEBUG equ 1
__DEBUG__ equ 1
__DEBUG_LEVEL__ equ 1

include 'proc32.inc'
include 'imports.inc'
include 'fdo.inc'
include 'netdrv.inc'

OS_BASE 	equ 0
new_app_base	equ 0x60400000
PROC_BASE	equ OS_BASE+0x0080000


virtual at ebx

       device:

       ETH_DEVICE

      .io_addr		dd ?
      .irq_line 	db ?
      .pci_bus		db ?
      .pci_dev		db ?

      .flags		db ?
      .vendor		db ?
      .asic_base	dw ?
      .memsize		db ?
      .rx_start 	db ?
      .tx_start 	db ?
      .bmem		dd ?
      .rmem		dd ?
      .romdata		rb 16

      .size = $ - device

end virtual


public START
public service_proc
public version

	MAX_DEVICES		  equ 16	  ; Max number of devices this driver may handle

	P0_PSTART		  equ 0x01
	P0_PSTOP		  equ 0x02
	P0_BOUND		  equ 0x03
	P0_TSR			  equ 0x04
	P0_TPSR 		  equ 0x04
	P0_TBCR0		  equ 0x05
	P0_TBCR1		  equ 0x06
	P0_ISR			  equ 0x07
	P0_RSAR0		  equ 0x08
	P0_RSAR1		  equ 0x09
	P0_RBCR0		  equ 0x0A
	P0_RBCR1		  equ 0x0B
	P0_RSR			  equ 0x0C
	P0_RCR			  equ 0x0C
	P0_TCR			  equ 0x0D
	P0_DCR			  equ 0x0E
	P0_IMR			  equ 0x0F

	P1_PAR0 		  equ 0x01
	P1_PAR1 		  equ 0x02
	P1_PAR2 		  equ 0x03
	P1_PAR3 		  equ 0x04
	P1_PAR4 		  equ 0x05
	P1_PAR5 		  equ 0x06
	P1_CURR 		  equ 0x07
	P1_MAR0 		  equ 0x08

	CMD_PS0 		  equ 0x00	  ;  Page 0 select
	CMD_PS1 		  equ 0x40	  ;  Page 1 select
	CMD_PS2 		  equ 0x80	  ;  Page 2 select
	CMD_RD2 		  equ 0x20	  ;  Remote DMA control
	CMD_RD1 		  equ 0x10
	CMD_RD0 		  equ 0x08
	CMD_TXP 		  equ 0x04	  ;  transmit packet
	CMD_STA 		  equ 0x02	  ;  start
	CMD_STP 		  equ 0x01	  ;  stop

	RCR_MON 		  equ 0x20	  ;  monitor mode

	DCR_FT1 		  equ 0x40
	DCR_LS			  equ 0x08	  ;  Loopback select
	DCR_WTS 		  equ 0x01	  ;  Word transfer select

	ISR_PRX 		  equ 0x01	  ;  successful recv
	ISR_PTX 		  equ 0x02	  ;  successful xmit
	ISR_RXE 		  equ 0x04	  ;  receive error
	ISR_TXE 		  equ 0x08	  ;  transmit error
	ISR_OVW 		  equ 0x10	  ;  Overflow
	ISR_CNT 		  equ 0x20	  ;  Counter overflow
	ISR_RDC 		  equ 0x40	  ;  Remote DMA complete
	ISR_RST 		  equ 0x80	  ;  reset

	IRQ_MASK		  equ ISR_PRX ; + ISR_PTX + ISR_TXE

	RSTAT_PRX		  equ 0x01	  ;  successful recv
	RSTAT_CRC		  equ 0x02	  ;  CRC error
	RSTAT_FAE		  equ 0x04	  ;  Frame alignment error
	RSTAT_OVER		  equ 0x08	  ;  FIFO overrun

	TXBUF_SIZE		  equ 6
	RXBUF_END		  equ 32
	PAGE_SIZE		  equ 256

	ETH_ALEN		  equ 6
	ETH_HLEN		  equ 14
	ETH_ZLEN		  equ 60
	ETH_FRAME_LEN		  equ 1514

	FLAG_PIO		  equ 0x01
	FLAG_16BIT		  equ 0x02
	ASIC_PIO		  equ 0

	VENDOR_NONE		  equ 0
	VENDOR_WD		  equ 1
	VENDOR_NOVELL		  equ 2
	VENDOR_3COM		  equ 3

	NE_ASIC_OFFSET		  equ 0x10
	NE_RESET		  equ 0x0F	  ; Used to reset card
	NE_DATA 		  equ 0x00	  ; Used to read/write NIC mem

	MEM_8192		  equ 32
	MEM_16384		  equ 64
	MEM_32768		  equ 128

	ISA_MAX_ADDR		  equ 0x400



section '.flat' code readable align 16

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; proc START
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

align 4
proc START stdcall, state:dword

	cmp	[state], 1
	jne	.exit
  .entry:
	DEBUGF 2,"Registering rtl8029 service \n"
	stdcall RegService, my_service, service_proc
	ret
  .fail:
  .exit:
	xor	eax, eax
	ret

endp


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; proc SERVICE_PROC
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

align 4
proc service_proc stdcall, ioctl:dword

	mov	edx, [ioctl]
	mov	eax, [IOCTL.io_code]

;------------------------------------------------------
		       ;---------------
	cmp	eax, 0 ;SRV_GETVERSION
	jne	@F     ;---------------

	cmp	[IOCTL.out_size], 4
	jl	.fail
	mov	eax, [IOCTL.output]
	mov	[eax], dword API_VERSION

	xor	eax, eax
	ret	4

;------------------------------------------------------
  @@:		       ;---------
	cmp	eax, 1 ;SRV_HOOK
	jne	@F     ;---------

	DEBUGF	2,"Checking if device is already listed..\n"

	mov	eax, [IOCTL.input]

	cmp	[IOCTL.inp_size], 3
	jl	.fail
	cmp	byte [eax], 1
	je	.pci

	cmp	[IOCTL.inp_size], 4
	jl	.fail
	cmp	byte [eax], 0
	je	.isa

	jmp	.fail

  .pci:

; check if the device is already listed

	mov	esi, DEVICE_LIST
	mov	ecx, [DEVICES]
	test	ecx, ecx
	jz	.firstdevice_pci

;        mov     eax, [IOCTL.input]                      ; get the pci bus and device numbers
	mov	ax , [eax+1]				;
  .nextdevice:
	mov	ebx, [esi]
	cmp	ax , word [device.pci_bus]		; compare with pci and device num in device list (notice the usage of word instead of byte)
	je	.find_devicenum 			; Device is already loaded, let's find it's device number
	add	esi, 4
	loop	.nextdevice

  .firstdevice_pci:
	call	create_new_struct

	mov	eax, [IOCTL.input]
	mov	cl , [eax+1]
	mov	[device.pci_bus], cl
	mov	cl , [eax+2]
	mov	[device.pci_dev], cl

; Now, it's time to find the base io addres of the PCI device

	find_io [device.pci_bus], [device.pci_dev], [device.io_addr]

; We've found the io address, find IRQ now

	find_irq [device.pci_bus], [device.pci_dev], [device.irq_line]

	jmp	.hook

  .isa:

	mov	esi, DEVICE_LIST
	mov	ecx, [DEVICES]
	test	ecx, ecx
	jz	.firstdevice_isa
	mov	al , [eax+3]
	movzx	edi, word [eax+1]
  .nextdevice_isa:
	mov	ebx, [esi]
	cmp	edi, [device.io_addr]
	jne	.maybenext
	cmp	al , [device.irq_line]
	je	find_device_num
  .maybenext:
	add	esi, 4
	loop	.nextdevice_isa



  .firstdevice_isa:
	call	create_new_struct

	mov	eax, [IOCTL.input]
	movzx	ecx , word [eax+1]
	mov	[device.io_addr], ecx
	mov	cl, [eax+3]
	mov	[device.irq_line], cl

  .hook:

	DEBUGF	2,"Hooking into device, dev:%x, bus:%x, irq:%x, addr:%x\n",\
	[device.pci_dev]:1,[device.pci_bus]:1,[device.irq_line]:1,[device.io_addr]:4

	call	probe							; this function will output in eax
	test	eax, eax
	jnz	.err							; If an error occured, exit

	mov	eax, [DEVICES]
	mov	[DEVICE_LIST+4*eax], ebx
	inc	[DEVICES]

	call	EthRegDev				     ; Register the device to kernel (ebx points to device struct)
	cmp	eax, -1
	jz	.err
	ret	4


; If the device was already loaded, find the device number and return it in eax

  .find_devicenum:
	DEBUGF	1,"Trying to find device number of already registered device\n"
	mov	ebx, eax
	call	EthStruc2Dev						; This kernel procedure converts a pointer to device struct in ebx
									; into a device number in edi
	mov	eax, edi						; Application wants it in eax instead
	DEBUGF	1,"Kernel says: %u\n", eax
	ret

  .err:
	DEBUGF	1,"Failed, removing device structure\n"
	stdcall KernelFree, ebx

	jmp	.fail

;------------------------------------------------------
  @@:
.fail:
	or	eax, -1
	ret	4

;------------------------------------------------------
endp


create_new_struct:

	cmp	[DEVICES], MAX_DEVICES
	jge	.fail

	push	edx
	stdcall KernelAlloc, device.size
	pop	edx
	test	eax, eax
	jz	.fail
	mov	ebx, eax

	mov	[device.reset], reset
	mov	[device.transmit], transmit
	mov	[device.get_MAC], read_mac
	mov	[device.set_MAC], write_mac
	mov	[device.unload], unload
	mov	[device.name], my_service

	ret

.fail:
	add	esp, 4
	or	eax, -1
	ret

find_device_num:

	DEBUGF	1,"Trying to find device number of already registered device\n"
	mov	ebx, eax
	call	EthStruc2Dev						; This kernel procedure converts a pointer to device struct in ebx
									; into a device number in edi
	mov	eax, edi						; Application wants it in eax instead
	DEBUGF	1,"Kernel says: %u\n", eax
	ret


;;/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\;;
;;                                                                        ;;
;;        Actual Hardware dependent code starts here                      ;;
;;                                                                        ;;
;;/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\;;


unload:   ; TODO
	or	eax, -1
	ret



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;  probe: enables the device and clears the rx buffer
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

probe:
	mov	[device.vendor], VENDOR_NONE
	mov	[device.bmem], 0
	mov	eax,[device.io_addr]
	add	eax, NE_ASIC_OFFSET
	mov	[device.asic_base], ax

	DEBUGF	2,"Trying 16-bit mode\n"

	or	[device.flags], FLAG_16BIT or FLAG_PIO
	mov	[device.memsize], MEM_32768
	mov	[device.tx_start], 64
	mov	[device.rx_start], TXBUF_SIZE + 64

	set_io	0
	set_io	P0_DCR
	mov	al, DCR_WTS + DCR_FT1 + DCR_LS
	out	dx, al

	set_io	P0_PSTART
	mov	al, MEM_16384
	out	dx, al

	set_io	P0_PSTOP
	mov	al, MEM_32768
	out	dx, al

	mov	esi, test_data
	mov	di, 16384
	mov	cx, 14
	call	eth_pio_write

	mov	si, 16384
	mov	cx, 14
	lea	edi, [device.romdata]
	call	eth_pio_read

	lea	esi, [device.romdata]
	mov	edi, test_data
	mov	ecx, 13

     repz    cmpsb
     jz      ep_set_vendor


	DEBUGF	2,"Trying 8-bit mode\n"

	mov	[device.flags], FLAG_PIO
	mov	[device.memsize], MEM_16384
	mov	[device.tx_start], 32
	mov	[device.rx_start], TXBUF_SIZE + 32

	mov	dx, [device.asic_base]
	add	dx, NE_RESET

	in	al, dx
	out	dx, al

	in	al, 0x84

	set_io	0
	mov	al, CMD_RD2 + CMD_STP
	out	dx, al

	set_io	P0_RCR
	mov	al, RCR_MON
	out	dx, al

	set_io	P0_DCR
	mov	al, DCR_FT1 + DCR_LS
	out	dx, al

	set_io	P0_PSTART
	mov	al, MEM_8192
	out	dx, al

	set_io	P0_PSTOP
	mov	al, MEM_16384
	out	dx, al

	mov	esi, test_data
	mov	di, 8192
	mov	cx, 14
	call	eth_pio_write

	mov	si, 8192
	mov	cx, 14
	lea	edi, [device.romdata]
	call	eth_pio_read

	mov	esi, test_data
	lea	edi, [device.romdata]
	mov	ecx, 13

    repz      cmpsb
    jz	      ep_set_vendor

	DEBUGF	2,"This is not a valid ne2000 device!\n"
	or	eax, -1
	ret


ep_set_vendor:

	cmp	[device.io_addr], ISA_MAX_ADDR
	jbe	ep_001

	DEBUGF	2,"Card is using PCI bus\n"

;        or      [flags], FLAG_16BIT

ep_001:
	mov	[device.vendor], VENDOR_NOVELL

ep_check_have_vendor:


	mov	al, [device.vendor]
	cmp	al, VENDOR_NONE
;        je      rtl8029_exit

	cmp	al, VENDOR_3COM
	je	reset

	mov	eax, [device.bmem]
	mov	[device.rmem], eax

	call	read_mac

	push	.hack
	sub	esp, 6
	mov	edi, esp
	lea	esi, [device.mac]
	movsd
	movsw
	jmp	write_mac
       .hack:


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;   reset: Place the chip into a virgin state
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

reset:
	DEBUGF	2,"Resetting device\n"

; attach int handler
	movzx	eax, [device.irq_line]
	DEBUGF	1,"Attaching int handler to irq %x\n",eax:1
	stdcall AttachIntHandler, eax, int_handler, dword 0

; Stop mode

	set_io	0
	mov	al, CMD_PS0 + CMD_RD2 + CMD_STP
	out	dx, al

	set_io	P0_DCR
	test	[device.flags], FLAG_16BIT
	jz	nsr_001

	mov	al, 0x49
	jmp	nsr_002

nsr_001:
	mov	al, 0x48

nsr_002:
	out	dx, al

;clear remote bytes count
	set_io	0

	xor	al, al

	set_io	P0_RBCR0
	out	dx, al

	set_io	P0_RBCR1
	out	dx, al


;initialize Receive configuration register
	set_io	P0_RCR
	mov	al, 0x20	; monitor mode
	out	dx, al


; transmit configuration register
	set_io	P0_TCR
	mov	al, 2		; internal loopback
	out	dx, al


; transmit page stuff
	set_io	P0_TPSR
	mov	al, [device.tx_start]
	out	dx, al

; set receive control register ;;;;
	set_io	P0_RCR
	mov	al, 4		; accept broadcast
	out	dx, al

; pagestart
	set_io	P0_PSTART
	mov	al, [device.rx_start]
	out	dx, al

; pagestop
	set_io	P0_PSTOP
	mov	al, [device.memsize]
	out	dx, al

; page boundary
	set_io	P0_BOUND
	mov	al, [device.memsize]
	dec	al
	out	dx, al


;;clear IRQ mask
;        set_io  P0_IMR
;        xor     al, al
;        out     dx, al

	set_io	0
	mov	al, CMD_PS1 + CMD_RD2 + CMD_STP ; page 1, stop mode
	out	dx, al

	set_io	P1_CURR
	mov	al, [device.rx_start]
	out	dx, al

	set_io	0
	mov	al, CMD_PS0 + CMD_RD2 + CMD_STA ; go to page 0
	out	dx, al

; Read MAC address
	call	read_mac

; clear interupt status
	set_io	0
	set_io	P0_ISR
	mov	al, 0xff
	out	dx, al

; set IRQ mask
	set_io	P0_IMR
	mov	al, IRQ_MASK
	out	dx, al

;; start mode
;        set_io  0
;        mov     al, CMD_STA
;        out     dx, al

; clear transmit control register
	set_io	P0_TCR
	mov	al, 0		; no loopback
	out	dx, al

; clear packet/byte counters
	xor	eax, eax
	lea	edi, [device.bytes_tx]
	mov	ecx, 6
	rep	stosd

; Indicate that we have successfully reset the card
	DEBUGF	2,"Done!\n"

	ret



;***************************************************************************
;   Function
;      transmit
; buffer in [esp+4], size in [esp+8], pointer to device struct in ebx
;***************************************************************************

; TODO: use a RING-buffer

align 4
transmit:

	mov	esi, [esp + 4]
	mov	ecx, [esp + 8]
	DEBUGF	2,"Transmitting packet, buffer:%x, size:%u\n",esi, ecx
	DEBUGF	2,"To: %x-%x-%x-%x-%x-%x From: %x-%x-%x-%x-%x-%x Type:%x%x\n",[esi+0]:2,[esi+1]:2,[esi+2]:2,[esi+3]:2,[esi+4]:2,[esi+5]:2,[esi+6]:2,[esi+7]:2,[esi+8]:2,[esi+9]:2,[esi+10]:2,[esi+11]:2,[esi+13]:2,[esi+12]:2

	cmp	ecx, ETH_FRAME_LEN
	jg	.err ; packet is too long
	cmp	ecx, 60
	jl	.err ; packet is too short

	movzx	edi, [device.tx_start]
	shl	edi, 8
	push	cx
	call	eth_pio_write
	pop	cx

	set_io	0
	mov	al, CMD_PS0 + CMD_RD2 + CMD_STA
	out	dx, al

	set_io	P0_TPSR
	mov	al, [device.tx_start]
	out	dx, al

	set_io	P0_TBCR0
	mov	al, cl
	out	dx, al

	set_io	P0_TBCR1
	mov	al, ch
	out	dx, al

	set_io	0
	mov	al, CMD_PS0 + CMD_TXP + CMD_RD2 + CMD_STA
	out	dx, al

	DEBUGF	2," - Packet Sent!\n"

	inc	[device.packets_tx]
	mov	eax, [esp + 8]			 ; Get packet size in eax

	add	dword [device.bytes_tx], eax
	adc	dword [device.bytes_tx + 4], 0

.finish:
	xor	eax, eax
	ret

.err:
	or	eax, -1
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Interrupt handler
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
align 4
int_handler:

	DEBUGF	2,"IRQ %x ",eax:2

; find pointer of device wich made INT occur
	mov	esi, DEVICE_LIST
	mov	ecx, [DEVICES]
.nextdevice:
	mov	ebx, [esi]

	set_io	0
	set_io	P0_ISR
	in	al, dx

	DEBUGF	2,"isr %x ",eax:2

	test	al, ISR_PRX    ; packet received ok ?
	jnz	.rx

	add	esi, 4

	loop	.nextdevice
	ret



; looks like we've found a device wich received a packet..
.rx:
	stdcall KernelAlloc, ETH_FRAME_LEN  ; size doesnt really matter as packet size is smaller then kernel's page size
	test	eax, eax
	jz	.fail_2

;--------------------------------------
; allocate memory for temp variables in stack

	sub	esp, 14+8

	eth_type	equ esp
	pkthdr		equ esp + 2
	pktoff		equ esp + 6
	eth_rx_data_ptr equ esp + 8
	eth_tmp_len	equ esp + 12

; These will be used by eth_receiver when the packet gets there

	pointer 	equ esp + 14
	size		equ esp + 18

;-------------------------------------

	mov	[pointer], eax
	mov	[eth_rx_data_ptr], eax

	set_io	P0_BOUND
	in	al, dx
	inc	al

	cmp	al, [device.memsize]
	jb	.nsp_001

	mov	al, [device.rx_start]

.nsp_001:
	mov	ch, al

	set_io	0
	mov	al, CMD_PS1
	out	dx, al

	set_io	P1_CURR
	in	al, dx		     ; get current page
	mov	cl, al

	set_io	0
	mov	al, CMD_PS0
	out	dx, al

	cmp	cl, [device.memsize]
	jb	.nsp_002

	mov	cl, [device.rx_start]

.nsp_002:
	cmp	cl, ch
	je	.fail

	xor	ax, ax
	mov	ah, ch

	mov	[pktoff], ax

	mov	al, [device.flags]
	test	al, FLAG_PIO
	jz	.nsp_003

	mov	si, word [pktoff]
	lea	edi, [pkthdr]
	mov	cx, 4
	call	eth_pio_read
	jmp	.nsp_004

.nsp_003:
	mov	edi, [device.rmem]
	movzx	eax, word [pktoff]
	add	edi, eax
	mov	eax, [edi]
	mov	[pkthdr], eax

.nsp_004:
	add	word[pktoff] , 4

	xor	eax, eax
	mov	ax , [pkthdr + 2]
	sub	ax , 4

	DEBUGF	2,"Received %u bytes\n",eax

	add	dword [device.bytes_rx], eax  ; Update stats
	adc	dword [device.bytes_rx + 4], 0
	inc	dword [device.packets_rx]     ;

	mov	[eth_tmp_len], ax
	mov	dword[size], eax

	cmp	ax, ETH_ZLEN
	jb	.fail

	cmp	ax , ETH_FRAME_LEN
	ja	.fail

	mov	al , [pkthdr]
	test	al , RSTAT_PRX
	jz	.fail

   ; Right, we can now get the data

	movzx	esi, [device.memsize]
	sub	si , [pktoff]

	cmp	[eth_tmp_len], si
	jbe	.nsp_005

	DEBUGF	2,"WRAP!\n"

	mov	al , [device.flags]
	test	al , FLAG_PIO
	jz	.nsp_006

	push	esi
	mov	cx , si
	mov	si , [pktoff+4]
	mov	edi, [eth_rx_data_ptr+4]
	call	eth_pio_read
	pop	esi
	jmp	.nsp_007

.nsp_006:
	DEBUGF	2,"PIO mode not supported by HW!\n"
   ; Not implemented, as we are using PIO mode on this card

.nsp_007:
	xor	al, al
	mov	ah, [device.rx_start]
	mov	[pktoff], ax

	add	[eth_rx_data_ptr], esi
	sub	[eth_tmp_len], si

.nsp_005:
	test	[device.flags], FLAG_PIO
	jz	.nsp_008

	movzx	esi, word [pktoff]
	movzx	ecx, word [eth_tmp_len]
	mov	edi, [eth_rx_data_ptr]
	call	eth_pio_read
	jmp	.nsp_009

.nsp_008:
	DEBUGF	2,"PIO mode not supported by HW!\n"
   ; Not implemented, as we are using PIO mode on this card

.nsp_009:
	mov	al, [pkthdr+1]
	cmp	al, [device.rx_start]
	jne	.nsp_010

	mov	al, [device.memsize]

.nsp_010:
	set_io	0
	set_io	P0_BOUND
	dec	al
	out	dx, al

	add	esp, 14
	jmp	EthReceiver	; send it to the kernel

.fail:
	add	esp, 14+8
.fail_2:
	DEBUGF	2,"done\n"
ret





;;;;;;;;;;;;;;;;;;;;;;;
;;                   ;;
;; Write MAC address ;;
;;                   ;;
;;;;;;;;;;;;;;;;;;;;;;;

align 4
write_mac:	; in: mac on stack (6 bytes)

	DEBUGF	1,"Writing MAC: "

	set_io	0
	mov	al, CMD_PS1; + CMD_RD2 + CMD_STP
	out	dx, al

	set_io	P1_PAR0
	mov	esi, esp
	mov	cx, 6
 @@:
	lodsb
	out	dx, al
	inc	dx
	loopw	@r

	add	esp, 6

; Notice this procedure does not ret, but continues to read_mac instead.

;;;;;;;;;;;;;;;;;;;;;;
;;                  ;;
;; Read MAC address ;;
;;                  ;;
;;;;;;;;;;;;;;;;;;;;;;

read_mac:

	DEBUGF	1,"Reading MAC: "

;        set_io  0
;        mov     al, CMD_PS1; + CMD_RD2 + CMD_STP ; select page 1
;        out     dx, al
;
;        set_io  P1_PAR0
;        lea     edi, [mac]
;
;        mov     cx, 6
; .loop:
;        in      al, dx
;        stosb
;        inc     dx
;        loopw   .loop
;
;        set_io  0
;        mov     al, CMD_PS0; + CMD_RD2 + CMD_STA  ; set page back to 0
;        out     dx, al

	mov	si, 0
	mov	cx, 16
	lea	edi, [device.romdata]
	call	eth_pio_read

	lea	esi, [device.romdata]
	lea	edi, [device.mac]
	mov	ecx, 6

  .loop:
	movsb
	test	[device.flags], FLAG_16BIT
	jz	.8bit
	inc	esi
  .8bit:
	loop	.loop

	DEBUGF	1,"%x-%x-%x-%x-%x-%x\n",[edi-6]:2,[edi-5]:2,[edi-4]:2,[edi-3]:2,[edi-2]:2,[edi-1]:2

	ret


;***************************************************************************
;   Function
;      eth_pio_read
;
;   Description
;       Read a frame from the ethernet card via Programmed I/O
;      src in si
;      cnt in cx
;       dst in edi
;***************************************************************************
eth_pio_read:

	DEBUGF	1,"Eth PIO Read from %x to %x, %u bytes ",si,edi,cx

	set_io	0
	mov	al, CMD_RD2 + CMD_STA
	out	dx, al

	mov	al, cl
	set_io	P0_RBCR0
	out	dx, al

	mov	al, ch
	set_io	P0_RBCR1
	out	dx, al

	mov	ax, si
	set_io	P0_RSAR0
	out	dx, al
	shr	ax, 8
	set_io	P0_RSAR1
	out	dx, al

	mov	al, CMD_RD0 + CMD_STA
	set_io	0
	out	dx, al

	mov	dx, [device.asic_base]

	test	[device.flags], FLAG_16BIT
	jz	epr_003

	DEBUGF	1,"in 16-bits mode"

	shr	cx, 1	; note that if the number was odd, carry flag will be set
	pushf		; save the flags for later

epr_002:
	in	ax, dx
	stosw
	loopw	epr_002

	inc	cx
	popf
	jnc	epr_004

epr_003:
	in	al, dx
	stosb
	loopw	epr_003


epr_004:
	set_io	0
	set_io	P0_ISR

epr_005:				; Wait for Remote DMA Complete
	in	al, dx
	test	al, ISR_RDC
	jz	epr_005
;        and     al, not ISR_RDC
	out	dx, al			; clear the bit


	DEBUGF	1,"\n"
	ret




;***************************************************************************
;   Function
;      eth_pio_write
;
;   Description
;       writes a frame to the ethernet card via Programmed I/O
;      dst in di
;      cnt in cx
;       src in esi
;***************************************************************************
eth_pio_write:

	DEBUGF	1,"Eth PIO Write from %x to %x, %u bytes ",esi,di,cx

	set_io	0
	mov	al, CMD_RD2 + CMD_STA
	out	dx, al

	set_io	P0_ISR
	mov	al, ISR_RDC
	out	dx, al

	set_io	P0_RBCR0
	mov	al, cl
	out	dx, al

	set_io	P0_RBCR1
	mov	al, ch
	out	dx, al

	mov	ax, di
	set_io	P0_RSAR0
	out	dx, al
	shr	ax, 8
	set_io	P0_RSAR1
	out	dx, al

	set_io	0
	mov	al, CMD_RD1 + CMD_STA
	out	dx, al

	mov	dx, [device.asic_base]
	test	[device.flags], FLAG_16BIT
	jz	epw_003

	DEBUGF	1,"in 16-bits mode"

	shr	cx, 1	; note that if the number was odd, carry flag will be set
	pushf		; save the flags for later

epw_002:
	lodsw
	out	dx, ax
	loopw	epw_002

	popf
	jnc	epw_004
	inc	cx

epw_003:
	lodsb
	out	dx, al
	loopw	epw_003

epw_004:
	set_io	0
	set_io	P0_ISR

epw_005:				; Wait for Remote DMA Complete
	in	al, dx
	test	al, ISR_RDC
	jz	epw_005
;        and     al, not ISR_RDC
	out	dx, al			; clear the bit


	DEBUGF	1,"\n"
	ret



;all initialized data place here
align 4

DEVICES 	dd 0
version 	dd (5 shl 16) or (API_VERSION and 0xFFFF)
my_service	db 'RTL8029/ne2000',0  ;max 16 chars include zero

device_1	db 'Realtek 8029',0
device_2	db 'Realtek 8019',0
device_3	db 'Realtek 8019AS',0
device_4	db 'ne2000',0
device_5	db 'DP8390',0

test_data	db 'NE*000 memory',0

include_debug_strings

section '.data' data readable writable align 16  ;place all uninitialized data place here

DEVICE_LIST	rd MAX_DEVICES





