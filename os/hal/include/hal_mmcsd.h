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
 * @file    hal_mmcsd.h
 * @brief   MMC/SD cards common header.
 * @details This header defines an abstract interface useful to access MMC/SD
 *          I/O block devices in a standardized way.
 *
 * @addtogroup MMCSD
 * @{
 */

#ifndef _HAL_MMCSD_H_
#define _HAL_MMCSD_H_

#if HAL_USE_MMC_SPI || HAL_USE_SDC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Fixed block size for MMC/SD block devices.
 */
#define MMCSD_BLOCK_SIZE                512

/**
 * @brief   Mask of error bits in R1 responses.
 */
#define MMCSD_R1_ERROR_MASK             0xFDFFE008

/**
 * @brief   Fixed pattern for CMD8.
 */
#define MMCSD_CMD8_PATTERN              0x000001AA

/**
 * @name    SD/MMC status conditions
 * @{
 */
#define MMCSD_STS_IDLE                  0
#define MMCSD_STS_READY                 1
#define MMCSD_STS_IDENT                 2
#define MMCSD_STS_STBY                  3
#define MMCSD_STS_TRAN                  4
#define MMCSD_STS_DATA                  5
#define MMCSD_STS_RCV                   6
#define MMCSD_STS_PRG                   7
#define MMCSD_STS_DIS                   8
/** @} */

/**
 * @name    SD/MMC commands
 * @{
 */
#define MMCSD_CMD_GO_IDLE_STATE         0
#define MMCSD_CMD_INIT                  1
#define MMCSD_CMD_ALL_SEND_CID          2
#define MMCSD_CMD_SEND_RELATIVE_ADDR    3
#define MMCSD_CMD_SET_BUS_WIDTH         6
#define MMCSD_CMD_SWITCH                MMCSD_CMD_SET_BUS_WIDTH
#define MMCSD_CMD_SEL_DESEL_CARD        7
#define MMCSD_CMD_SEND_IF_COND          8
#define MMCSD_CMD_SEND_EXT_CSD          MMCSD_CMD_SEND_IF_COND
#define MMCSD_CMD_SEND_CSD              9
#define MMCSD_CMD_SEND_CID              10
#define MMCSD_CMD_STOP_TRANSMISSION     12
#define MMCSD_CMD_SEND_STATUS           13
#define MMCSD_CMD_SET_BLOCKLEN          16
#define MMCSD_CMD_READ_SINGLE_BLOCK     17
#define MMCSD_CMD_READ_MULTIPLE_BLOCK   18
#define MMCSD_CMD_SET_BLOCK_COUNT       23
#define MMCSD_CMD_WRITE_BLOCK           24
#define MMCSD_CMD_WRITE_MULTIPLE_BLOCK  25
#define MMCSD_CMD_ERASE_RW_BLK_START    32
#define MMCSD_CMD_ERASE_RW_BLK_END      33
#define MMCSD_CMD_ERASE                 38
#define MMCSD_CMD_APP_OP_COND           41
#define MMCSD_CMD_LOCK_UNLOCK           42
#define MMCSD_CMD_APP_CMD               55
#define MMCSD_CMD_READ_OCR              58
/** @} */

/**
 * @name   CSD record offsets
 */
/**
 * @brief  Slice position of values in CSD register.
 */
/* CSD for MMC */
#define MMCSD_CSD_MMC_CSD_STRUCTURE_SLICE       127,126
#define MMCSD_CSD_MMC_SPEC_VERS_SLICE           125,122
#define MMCSD_CSD_MMC_TAAC_SLICE                119,112
#define MMCSD_CSD_MMC_NSAC_SLICE                111,104
#define MMCSD_CSD_MMC_TRAN_SPEED_SLICE          103,96
#define MMCSD_CSD_MMC_CCC_SLICE                 95,84
#define MMCSD_CSD_MMC_READ_BL_LEN_SLICE         83,80
#define MMCSD_CSD_MMC_READ_BL_PARTIAL_SLICE     79,79
#define MMCSD_CSD_MMC_WRITE_BLK_MISALIGN_SLICE  78,78
#define MMCSD_CSD_MMC_READ_BLK_MISALIGN_SLICE   77,77
#define MMCSD_CSD_MMC_DSR_IMP_SLICE             76,76
#define MMCSD_CSD_MMC_C_SIZE_SLICE              73,62
#define MMCSD_CSD_MMC_VDD_R_CURR_MIN_SLICE      61,59
#define MMCSD_CSD_MMC_VDD_R_CURR_MAX_SLICE      58,56
#define MMCSD_CSD_MMC_VDD_W_CURR_MIN_SLICE      55,53
#define MMCSD_CSD_MMC_VDD_W_CURR_MAX_SLICE      52,50
#define MMCSD_CSD_MMC_C_SIZE_MULT_SLICE         49,47
#define MMCSD_CSD_MMC_ERASE_GRP_SIZE_SLICE      46,42
#define MMCSD_CSD_MMC_ERASE_GRP_MULT_SLICE      41,37
#define MMCSD_CSD_MMC_WP_GRP_SIZE_SLICE         36,32
#define MMCSD_CSD_MMC_WP_GRP_ENABLE_SLICE       31,31
#define MMCSD_CSD_MMC_DEFAULT_ECC_SLICE         30,29
#define MMCSD_CSD_MMC_R2W_FACTOR_SLICE          28,26
#define MMCSD_CSD_MMC_WRITE_BL_LEN_SLICE        25,22
#define MMCSD_CSD_MMC_WRITE_BL_PARTIAL_SLICE    21,21
#define MMCSD_CSD_MMC_CONTENT_PROT_APP_SLICE    16,16
#define MMCSD_CSD_MMC_FILE_FORMAT_GRP_SLICE     15,15
#define MMCSD_CSD_MMC_COPY_SLICE                14,14
#define MMCSD_CSD_MMC_PERM_WRITE_PROTECT_SLICE  13,13
#define MMCSD_CSD_MMC_TMP_WRITE_PROTECT_SLICE   12,12
#define MMCSD_CSD_MMC_FILE_FORMAT_SLICE         11,10
#define MMCSD_CSD_MMC_ECC_SLICE                 9,8
#define MMCSD_CSD_MMC_CRC_SLICE                 7,1

/* CSD version 2.0 */
#define MMCSD_CSD_20_CRC_SLICE                  7,1
#define MMCSD_CSD_20_FILE_FORMAT_SLICE          11,10
#define MMCSD_CSD_20_TMP_WRITE_PROTECT_SLICE    12,12
#define MMCSD_CSD_20_PERM_WRITE_PROTECT_SLICE   13,13
#define MMCSD_CSD_20_COPY_SLICE                 14,14
#define MMCSD_CSD_20_FILE_FORMAT_GRP_SLICE      15,15
#define MMCSD_CSD_20_WRITE_BL_PARTIAL_SLICE     21,21
#define MMCSD_CSD_20_WRITE_BL_LEN_SLICE         25,12
#define MMCSD_CSD_20_R2W_FACTOR_SLICE           28,26
#define MMCSD_CSD_20_WP_GRP_ENABLE_SLICE        31,31
#define MMCSD_CSD_20_WP_GRP_SIZE_SLICE          38,32
#define MMCSD_CSD_20_ERASE_SECTOR_SIZE_SLICE    45,39
#define MMCSD_CSD_20_ERASE_BLK_EN_SLICE         46,46
#define MMCSD_CSD_20_C_SIZE_SLICE               69,48
#define MMCSD_CSD_20_DSR_IMP_SLICE              76,76
#define MMCSD_CSD_20_READ_BLK_MISALIGN_SLICE    77,77
#define MMCSD_CSD_20_WRITE_BLK_MISALIGN_SLICE   78,78
#define MMCSD_CSD_20_READ_BL_PARTIAL_SLICE      79,79
#define MMCSD_CSD_20_READ_BL_LEN_SLICE          83,80
#define MMCSD_CSD_20_CCC_SLICE                  95,84
#define MMCSD_CSD_20_TRANS_SPEED_SLICE          103,96
#define MMCSD_CSD_20_NSAC_SLICE                 111,104
#define MMCSD_CSD_20_TAAC_SLICE                 119,112
#define MMCSD_CSD_20_CSD_STRUCTURE_SLICE        127,126

/* CSD version 1.0 */
#define MMCSD_CSD_10_CRC_SLICE                  MMCSD_CSD_20_CRC_SLICE
#define MMCSD_CSD_10_FILE_FORMAT_SLICE          MMCSD_CSD_20_FILE_FORMAT_SLICE
#define MMCSD_CSD_10_TMP_WRITE_PROTECT_SLICE    MMCSD_CSD_20_TMP_WRITE_PROTECT_SLICE
#define MMCSD_CSD_10_PERM_WRITE_PROTECT_SLICE   MMCSD_CSD_20_PERM_WRITE_PROTECT_SLICE
#define MMCSD_CSD_10_COPY_SLICE                 MMCSD_CSD_20_COPY_SLICE
#define MMCSD_CSD_10_FILE_FORMAT_GRP_SLICE      MMCSD_CSD_20_FILE_FORMAT_GRP_SLICE
#define MMCSD_CSD_10_WRITE_BL_PARTIAL_SLICE     MMCSD_CSD_20_WRITE_BL_PARTIAL_SLICE
#define MMCSD_CSD_10_WRITE_BL_LEN_SLICE         MMCSD_CSD_20_WRITE_BL_LEN_SLICE
#define MMCSD_CSD_10_R2W_FACTOR_SLICE           MMCSD_CSD_20_R2W_FACTOR_SLICE
#define MMCSD_CSD_10_WP_GRP_ENABLE_SLICE        MMCSD_CSD_20_WP_GRP_ENABLE_SLICE
#define MMCSD_CSD_10_WP_GRP_SIZE_SLICE          MMCSD_CSD_20_WP_GRP_SIZE_SLICE
#define MMCSD_CSD_10_ERASE_SECTOR_SIZE_SLICE    MMCSD_CSD_20_ERASE_SECTOR_SIZE_SLICE
#define MMCSD_CSD_10_ERASE_BLK_EN_SLICE         MMCSD_CSD_20_ERASE_BLK_EN_SLICE
#define MMCSD_CSD_10_C_SIZE_MULT_SLICE          49,47
#define MMCSD_CSD_10_VDD_W_CURR_MAX_SLICE       52,50
#define MMCSD_CSD_10_VDD_W_CURR_MIN_SLICE       55,53
#define MMCSD_CSD_10_VDD_R_CURR_MAX_SLICE       58,56
#define MMCSD_CSD_10_VDD_R_CURR_MIX_SLICE       61,59
#define MMCSD_CSD_10_C_SIZE_SLICE               73,62
#define MMCSD_CSD_10_DSR_IMP_SLICE              MMCSD_CSD_20_DSR_IMP_SLICE
#define MMCSD_CSD_10_READ_BLK_MISALIGN_SLICE    MMCSD_CSD_20_READ_BLK_MISALIGN_SLICE
#define MMCSD_CSD_10_WRITE_BLK_MISALIGN_SLICE   MMCSD_CSD_20_WRITE_BLK_MISALIGN_SLICE
#define MMCSD_CSD_10_READ_BL_PARTIAL_SLICE      MMCSD_CSD_20_READ_BL_PARTIAL_SLICE
#define MMCSD_CSD_10_READ_BL_LEN_SLICE          83, 80
#define MMCSD_CSD_10_CCC_SLICE                  MMCSD_CSD_20_CCC_SLICE
#define MMCSD_CSD_10_TRANS_SPEED_SLICE          MMCSD_CSD_20_TRANS_SPEED_SLICE
#define MMCSD_CSD_10_NSAC_SLICE                 MMCSD_CSD_20_NSAC_SLICE
#define MMCSD_CSD_10_TAAC_SLICE                 MMCSD_CSD_20_TAAC_SLICE
#define MMCSD_CSD_10_CSD_STRUCTURE_SLICE        MMCSD_CSD_20_CSD_STRUCTURE_SLICE
/** @} */

/**
 * @name   CID record offsets
 */
/**
 * @brief  Slice position of values in CID register.
 */
/* CID for SDC */
#define MMCSD_CID_SDC_CRC_SLICE                 7,1
#define MMCSD_CID_SDC_MDT_M_SLICE               11,8
#define MMCSD_CID_SDC_MDT_Y_SLICE               19,12
#define MMCSD_CID_SDC_PSN_SLICE                 55,24
#define MMCSD_CID_SDC_PRV_M_SLICE               59,56
#define MMCSD_CID_SDC_PRV_N_SLICE               63,60
#define MMCSD_CID_SDC_PNM0_SLICE                71,64
#define MMCSD_CID_SDC_PNM1_SLICE                79,72
#define MMCSD_CID_SDC_PNM2_SLICE                87,80
#define MMCSD_CID_SDC_PNM3_SLICE                95,88
#define MMCSD_CID_SDC_PNM4_SLICE                103,96
#define MMCSD_CID_SDC_OID_SLICE                 119,104
#define MMCSD_CID_SDC_MID_SLICE                 127,120

/* CID for MMC */
#define MMCSD_CID_MMC_CRC_SLICE                 7,1
#define MMCSD_CID_MMC_MDT_Y_SLICE               11,8
#define MMCSD_CID_MMC_MDT_M_SLICE               15,12
#define MMCSD_CID_MMC_PSN_SLICE                 47,16
#define MMCSD_CID_MMC_PRV_M_SLICE               51,48
#define MMCSD_CID_MMC_PRV_N_SLICE               55,52
#define MMCSD_CID_MMC_PNM0_SLICE                63,56
#define MMCSD_CID_MMC_PNM1_SLICE                71,64
#define MMCSD_CID_MMC_PNM2_SLICE                79,72
#define MMCSD_CID_MMC_PNM3_SLICE                87,80
#define MMCSD_CID_MMC_PNM4_SLICE                95,88
#define MMCSD_CID_MMC_PNM5_SLICE                103,96
#define MMCSD_CID_MMC_OID_SLICE                 119,104
#define MMCSD_CID_MMC_MID_SLICE                 127,120
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   @p MMCSDBlockDevice specific methods.
 */
#define _mmcsd_block_device_methods                                         \
  _base_block_device_methods

/**
 * @brief   @p MMCSDBlockDevice specific data.
 * @note    It is empty because @p MMCSDBlockDevice is only an interface
 *          without implementation.
 */
#define _mmcsd_block_device_data                                            \
  _base_block_device_data                                                   \
  /* Card CID.*/                                                            \
  uint32_t              cid[4];                                             \
  /* Card CSD.*/                                                            \
  uint32_t              csd[4];                                             \
  /* Total number of blocks in card.*/                                      \
  uint32_t              capacity;

/**
 * @extends BaseBlockDeviceVMT
 *
 * @brief   @p MMCSDBlockDevice virtual methods table.
 */
struct MMCSDBlockDeviceVMT {
  _base_block_device_methods
};

/**
 * @extends BaseBlockDevice
 *
 * @brief   MCC/SD block device class.
 * @details This class represents a, block-accessible, MMC/SD device.
 */
typedef struct {
  /** @brief Virtual Methods Table.*/
  const struct MMCSDBlockDeviceVMT *vmt;
  _mmcsd_block_device_data
} MMCSDBlockDevice;

/**
 * @brief   Unpacked CID register from SDC.
 */
typedef struct {
  uint8_t   mid;
  uint16_t  oid;
  char      pnm[5];
  uint8_t   prv_n;
  uint8_t   prv_m;
  uint32_t  psn;
  uint8_t   mdt_m;
  uint16_t  mdt_y;
  uint8_t   crc;
} unpacked_sdc_cid_t;

/**
 * @brief   Unpacked CID register from MMC.
 */
typedef struct {
  uint8_t   mid;
  uint16_t  oid;
  char      pnm[6];
  uint8_t   prv_n;
  uint8_t   prv_m;
  uint32_t  psn;
  uint8_t   mdt_m;
  uint16_t  mdt_y;
  uint8_t   crc;
} unpacked_mmc_cid_t;

/**
 * @brief   Unpacked CSD v1.0 register from SDC.
 */
typedef struct {
  uint8_t   csd_structure;
  uint8_t   taac;
  uint8_t   nsac;
  uint8_t   tran_speed;
  uint16_t  ccc;
  uint8_t   read_bl_len;
  uint8_t   read_bl_partial;
  uint8_t   write_blk_misalign;
  uint8_t   read_blk_misalign;
  uint8_t   dsr_imp;
  uint16_t  c_size;
  uint8_t   vdd_r_curr_min;
  uint8_t   vdd_r_curr_max;
  uint8_t   vdd_w_curr_min;
  uint8_t   vdd_w_curr_max;
  uint8_t   c_size_mult;
  uint8_t   erase_blk_en;
  uint8_t   erase_sector_size;
  uint8_t   wp_grp_size;
  uint8_t   wp_grp_enable;
  uint8_t   r2w_factor;
  uint8_t   write_bl_len;
  uint8_t   write_bl_partial;
  uint8_t   file_format_grp;
  uint8_t   copy;
  uint8_t   perm_write_protect;
  uint8_t   tmp_write_protect;
  uint8_t   file_format;
  uint8_t   crc;
} unpacked_sdc_csd_10_t;

/**
 * @brief   Unpacked CSD v2.0 register from SDC.
 */
typedef struct {
  uint8_t   csd_structure;
  uint8_t   taac;
  uint8_t   nsac;
  uint8_t   tran_speed;
  uint16_t  ccc;
  uint8_t   read_bl_len;
  uint8_t   read_bl_partial;
  uint8_t   write_blk_misalign;
  uint8_t   read_blk_misalign;
  uint8_t   dsr_imp;
  uint32_t  c_size;
  uint8_t   erase_blk_en;
  uint8_t   erase_sector_size;
  uint8_t   wp_grp_size;
  uint8_t   wp_grp_enable;
  uint8_t   r2w_factor;
  uint8_t   write_bl_len;
  uint8_t   write_bl_partial;
  uint8_t   file_format_grp;
  uint8_t   copy;
  uint8_t   perm_write_protect;
  uint8_t   tmp_write_protect;
  uint8_t   file_format;
  uint8_t   crc;
} unpacked_sdc_csd_20_t;

/**
 * @brief   Unpacked CSD register from MMC.
 */
typedef struct {
  uint8_t   csd_structure;
  uint8_t   spec_vers;
  uint8_t   taac;
  uint8_t   nsac;
  uint8_t   tran_speed;
  uint16_t  ccc;
  uint8_t   read_bl_len;
  uint8_t   read_bl_partial;
  uint8_t   write_blk_misalign;
  uint8_t   read_blk_misalign;
  uint8_t   dsr_imp;
  uint16_t  c_size;
  uint8_t   vdd_r_curr_min;
  uint8_t   vdd_r_curr_max;
  uint8_t   vdd_w_curr_min;
  uint8_t   vdd_w_curr_max;
  uint8_t   c_size_mult;
  uint8_t   erase_grp_size;
  uint8_t   erase_grp_mult;
  uint8_t   wp_grp_size;
  uint8_t   wp_grp_enable;
  uint8_t   default_ecc;
  uint8_t   r2w_factor;
  uint8_t   write_bl_len;
  uint8_t   write_bl_partial;
  uint8_t   content_prot_app;
  uint8_t   file_format_grp;
  uint8_t   copy;
  uint8_t   perm_write_protect;
  uint8_t   tmp_write_protect;
  uint8_t   file_format;
  uint8_t   ecc;
  uint8_t   crc;
} unpacked_mmc_csd_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    R1 response utilities
 * @{
 */
/**
 * @brief   Evaluates to @p TRUE if the R1 response contains error flags.
 *
 * @param[in] r1        the r1 response
 */
#define MMCSD_R1_ERROR(r1)              (((r1) & MMCSD_R1_ERROR_MASK) != 0)

/**
 * @brief   Returns the status field of an R1 response.
 *
 * @param[in] r1        the r1 response
 */
#define MMCSD_R1_STS(r1)                (((r1) >> 9) & 15)

/**
 * @brief   Evaluates to @p TRUE if the R1 response indicates a locked card.
 *
 * @param[in] r1        the r1 response
 */
#define MMCSD_R1_IS_CARD_LOCKED(r1)     (((r1) >> 21) & 1)
/** @} */

/**
 * @name    Macro Functions
 * @{
 */
/**
 * @brief   Returns the card capacity in blocks.
 *
 * @param[in] ip        pointer to a @p MMCSDBlockDevice or derived class
 *
 * @return              The card capacity.
 *
 * @api
 */
#define mmcsdGetCardCapacity(ip)  ((ip)->capacity)
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  uint32_t _mmcsd_get_slice(const uint32_t *data,
                            uint32_t end,
                            uint32_t start);
  uint32_t _mmcsd_get_capacity(const uint32_t *csd);
  uint32_t _mmcsd_get_capacity_ext(const uint8_t *ext_csd);
  void _mmcsd_unpack_sdc_cid(const MMCSDBlockDevice *sdcp,
                             unpacked_sdc_cid_t *cidsdc);
  void _mmcsd_unpack_mmc_cid(const MMCSDBlockDevice *sdcp,
                             unpacked_mmc_cid_t *cidmmc);
  void _mmcsd_unpack_csd_mmc(const MMCSDBlockDevice *sdcp,
                             unpacked_mmc_csd_t *csdmmc);
  void _mmcsd_unpack_csd_v10(const MMCSDBlockDevice *sdcp,
                             unpacked_sdc_csd_10_t *csd10);
  void _mmcsd_unpack_csd_v20(const MMCSDBlockDevice *sdcp,
                             unpacked_sdc_csd_20_t *csd20);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_MMC_SPI || HAL_USE_MMC_SDC*/

#endif /* _HAL_MMCSD_H_ */

/** @} */
