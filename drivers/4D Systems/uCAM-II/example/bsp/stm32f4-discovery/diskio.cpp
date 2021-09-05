/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h" /* FatFs lower layer API */
#include "bsp.h"
#include "sd.h"

///* Definitions of physical drive number for each drive */
//#define DEV_RAM		1	/* Example: Map Ramdisk to physical drive 0 */
//#define DEV_MMC		0	/* Example: Map MMC/SD card to physical drive 1 */
//#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */

using namespace microhal;

class FatFSDisc {
 public:
    virtual DSTATUS initialize() { return STA_NOINIT; }
    virtual DSTATUS status() const { return STA_NODISK; }
    virtual DRESULT read(BYTE *buff, DWORD sector, UINT count) { return RES_NOTRDY; }
    virtual DRESULT write(const BYTE *buff, DWORD sector, UINT count) { return RES_NOTRDY; }
    virtual DRESULT ioctl(BYTE cmd, void *buff) { return RES_NOTRDY; }
};

class SDSPIDisc : public FatFSDisc {
 public:
    SDSPIDisc(microhal::SPI &spi, microhal::GPIO &cs) : sd(spi, cs) {}
    DSTATUS initialize() final {
        if (sd.init()) {
            stm32f4xx::SPI::spi3.prescaler(stm32f4xx::SPI::Prescaler16);
            stat &= ~STA_NOINIT;
            return RES_OK;
        }
        return RES_ERROR;
    }
    DSTATUS status() const final { return stat; }
    DRESULT read(BYTE *buff, DWORD sector, UINT count) final {
        Sd::Error result;
        if (count == 1) {
            result = sd.readBlock(buff, sector);
        } else {
            result = sd.readMultipleBlock(buff, sector, count);
        }
        if (result == Sd::Error::None) return RES_OK;
        return RES_ERROR;
    }
    DRESULT write(const BYTE *buff, DWORD sector, UINT count) final {
        Sd::Error result;
        if (count == 1)
            result = sd.writeBlock(buff, sector);
        else
            result = sd.writeMultipleBlock(buff, sector, count);
        if (result == Sd::Error::None) return RES_OK;
        return RES_ERROR;
    }
    DRESULT ioctl(BYTE cmd, void *buff) final {
        DRESULT res = RES_ERROR;

        switch (cmd) {
            case CTRL_SYNC: /* Wait for end of internal write process of the drive */
                            //	if (sdCard.)
                res = RES_OK;
                break;

            case GET_SECTOR_COUNT: /* Get drive capacity in unit of sector (DWORD) */
                *(DWORD *)buff = sd.getSecrorCount();
                res = RES_OK;
                //				if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16)) {
                //					if ((csd[0] >> 6) == 1) {	/* SDC ver 2.00 */
                //						csize = csd[9] + ((WORD)csd[8] << 8) + ((DWORD)(csd[7] & 63) << 16) + 1;
                //						*(DWORD*)buff = csize << 10;
                //					} else {					/* SDC ver 1.XX or MMC ver 3 */
                //						n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
                //						csize = (csd[8] >> 6) + ((WORD)csd[7] << 2) + ((WORD)(csd[6] & 3) << 10) + 1;
                //						*(DWORD*)buff = csize << (n - 9);
                //					}
                //					res = RES_OK;
                //				}
                break;

            case GET_BLOCK_SIZE: /* Get erase block size in unit of sector (DWORD) */
                *(DWORD *)buff = sd.getBlockSize();
                res = RES_OK;
                //				if (CardType & CT_SD2) {	/* SDC ver 2.00 */
                //					if (send_cmd(ACMD13, 0) == 0) {	/* Read SD status */
                //						xchg_spi(0xFF);
                //						if (rcvr_datablock(csd, 16)) {				/* Read partial block */
                //							for (n = 64 - 16; n; n--) xchg_spi(0xFF);	/* Purge trailing data */
                //							*(DWORD*)buff = 16UL << (csd[10] >> 4);
                //							res = RES_OK;
                //						}
                //					}
                //				} else {					/* SDC ver 1.XX or MMC */
                //					if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16)) {	/* Read CSD */
                //						if (CardType & CT_SD1) {	/* SDC ver 1.XX */
                //							*(DWORD*)buff = (((csd[10] & 63) << 1) + ((WORD)(csd[11] & 128) >> 7) + 1) << ((csd[13]
                //>> 6) - 1); 						} else {					/* MMC */
                //							*(DWORD*)buff = ((WORD)((csd[10] & 124) >> 2) + 1) * (((csd[11] & 3) << 3) + ((csd[11]
                //& 224) >> 5) + 1);
                //						}
                //						res = RES_OK;
                //					}
                //				}
                break;
            //
            //			case CTRL_TRIM :	/* Erase a block of sectors (used when _USE_ERASE == 1) */
            //
            //				if (!(CardType & CT_SDC)) break;				/* Check if the card is SDC */
            //				if (disk_ioctl(drv, MMC_GET_CSD, csd)) break;	/* Get CSD */
            //				if (!(csd[0] >> 6) && !(csd[10] & 0x40)) break;	/* Check if sector erase can be applied to the card */
            //				dp = buff; st = dp[0]; ed = dp[1];				/* Load sector block */
            //				if (!(CardType & CT_BLOCK)) {
            //					st *= 512; ed *= 512;
            //				}
            //				if (send_cmd(CMD32, st) == 0 && send_cmd(CMD33, ed) == 0 && send_cmd(CMD38, 0) == 0 && wait_ready(30000)) {	/* Erase
            //sector block */ 					res = RES_OK;	/* FatFs does not check result of this command */
            //				}
            //				break;
            //
            default:
                res = RES_PARERR;
        }

        return res;
    }

 private:
    Sd sd;
    DSTATUS stat = STA_NOINIT;
};

static FatFSDisc nullDisc;
static SDSPIDisc sdDisc(bsp::sdCard::spi, bsp::sdCard::cs);
static FatFSDisc *discs[] = {
    &sdDisc,
    &nullDisc,
    &nullDisc,
    &nullDisc,
};
/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(BYTE pdrv /* Physical drive nmuber to identify the drive */
) {
    if (pdrv < sizeof(discs)) {
        return discs[pdrv]->status();
    }
    return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(BYTE pdrv /* Physical drive nmuber to identify the drive */
) {
    if (pdrv < sizeof(discs)) {
        return discs[pdrv]->initialize();
    }
    return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(BYTE pdrv,    /* Physical drive nmuber to identify the drive */
                  BYTE *buff,   /* Data buffer to store read data */
                  DWORD sector, /* Start sector in LBA */
                  UINT count    /* Number of sectors to read */
) {
    if (pdrv < sizeof(discs)) {
        return discs[pdrv]->read(buff, sector, count);
    }
    return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write(BYTE pdrv,        /* Physical drive nmuber to identify the drive */
                   const BYTE *buff, /* Data to be written */
                   DWORD sector,     /* Start sector in LBA */
                   UINT count        /* Number of sectors to write */
) {
    if (pdrv < sizeof(discs)) {
        return discs[pdrv]->write(buff, sector, count);
    }
    return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(BYTE pdrv, /* Physical drive nmuber (0..) */
                   BYTE cmd,  /* Control code */
                   void *buff /* Buffer to send/receive control data */
) {
    if (pdrv < sizeof(discs)) {
        return discs[pdrv]->ioctl(cmd, buff);
    }
    return RES_PARERR;
}
