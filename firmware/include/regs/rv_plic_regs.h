/**
 * @file
 * @brief Generated register defines for rv_plic
 */

// Copyright information found in source file:
// Copyright lowRISC contributors (OpenTitan project).

// Licensing information found in source file:
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#ifndef _RV_PLIC_REG_DEFS_
#define _RV_PLIC_REG_DEFS_

#ifdef __cplusplus
extern "C" {
#endif
// Number of interrupt sources
#define RV_PLIC_PARAM_NUM_SRC 186

// Number of Targets (Harts)
#define RV_PLIC_PARAM_NUM_TARGET 1

// Width of priority signals
#define RV_PLIC_PARAM_PRIO_WIDTH 2

// Number of alerts
#define RV_PLIC_PARAM_NUM_ALERTS 1

// Register width
#define RV_PLIC_PARAM_REG_WIDTH 32

// Interrupt Source Priority (common parameters)
#define RV_PLIC_PRIO_PRIO_FIELD_WIDTH 2
#define RV_PLIC_PRIO_MULTIREG_COUNT 186

// Interrupt Source Priority
#define RV_PLIC_PRIO_0_REG_OFFSET 0x0
#define RV_PLIC_PRIO_0_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_0_PRIO_0_MASK 0x3u
#define RV_PLIC_PRIO_0_PRIO_0_OFFSET 0
#define RV_PLIC_PRIO_0_PRIO_0_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_0_PRIO_0_MASK, .index = RV_PLIC_PRIO_0_PRIO_0_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_1_REG_OFFSET 0x4
#define RV_PLIC_PRIO_1_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_1_PRIO_1_MASK 0x3u
#define RV_PLIC_PRIO_1_PRIO_1_OFFSET 0
#define RV_PLIC_PRIO_1_PRIO_1_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_1_PRIO_1_MASK, .index = RV_PLIC_PRIO_1_PRIO_1_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_2_REG_OFFSET 0x8
#define RV_PLIC_PRIO_2_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_2_PRIO_2_MASK 0x3u
#define RV_PLIC_PRIO_2_PRIO_2_OFFSET 0
#define RV_PLIC_PRIO_2_PRIO_2_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_2_PRIO_2_MASK, .index = RV_PLIC_PRIO_2_PRIO_2_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_3_REG_OFFSET 0xc
#define RV_PLIC_PRIO_3_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_3_PRIO_3_MASK 0x3u
#define RV_PLIC_PRIO_3_PRIO_3_OFFSET 0
#define RV_PLIC_PRIO_3_PRIO_3_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_3_PRIO_3_MASK, .index = RV_PLIC_PRIO_3_PRIO_3_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_4_REG_OFFSET 0x10
#define RV_PLIC_PRIO_4_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_4_PRIO_4_MASK 0x3u
#define RV_PLIC_PRIO_4_PRIO_4_OFFSET 0
#define RV_PLIC_PRIO_4_PRIO_4_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_4_PRIO_4_MASK, .index = RV_PLIC_PRIO_4_PRIO_4_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_5_REG_OFFSET 0x14
#define RV_PLIC_PRIO_5_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_5_PRIO_5_MASK 0x3u
#define RV_PLIC_PRIO_5_PRIO_5_OFFSET 0
#define RV_PLIC_PRIO_5_PRIO_5_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_5_PRIO_5_MASK, .index = RV_PLIC_PRIO_5_PRIO_5_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_6_REG_OFFSET 0x18
#define RV_PLIC_PRIO_6_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_6_PRIO_6_MASK 0x3u
#define RV_PLIC_PRIO_6_PRIO_6_OFFSET 0
#define RV_PLIC_PRIO_6_PRIO_6_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_6_PRIO_6_MASK, .index = RV_PLIC_PRIO_6_PRIO_6_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_7_REG_OFFSET 0x1c
#define RV_PLIC_PRIO_7_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_7_PRIO_7_MASK 0x3u
#define RV_PLIC_PRIO_7_PRIO_7_OFFSET 0
#define RV_PLIC_PRIO_7_PRIO_7_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_7_PRIO_7_MASK, .index = RV_PLIC_PRIO_7_PRIO_7_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_8_REG_OFFSET 0x20
#define RV_PLIC_PRIO_8_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_8_PRIO_8_MASK 0x3u
#define RV_PLIC_PRIO_8_PRIO_8_OFFSET 0
#define RV_PLIC_PRIO_8_PRIO_8_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_8_PRIO_8_MASK, .index = RV_PLIC_PRIO_8_PRIO_8_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_9_REG_OFFSET 0x24
#define RV_PLIC_PRIO_9_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_9_PRIO_9_MASK 0x3u
#define RV_PLIC_PRIO_9_PRIO_9_OFFSET 0
#define RV_PLIC_PRIO_9_PRIO_9_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_9_PRIO_9_MASK, .index = RV_PLIC_PRIO_9_PRIO_9_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_10_REG_OFFSET 0x28
#define RV_PLIC_PRIO_10_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_10_PRIO_10_MASK 0x3u
#define RV_PLIC_PRIO_10_PRIO_10_OFFSET 0
#define RV_PLIC_PRIO_10_PRIO_10_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_10_PRIO_10_MASK, .index = RV_PLIC_PRIO_10_PRIO_10_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_11_REG_OFFSET 0x2c
#define RV_PLIC_PRIO_11_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_11_PRIO_11_MASK 0x3u
#define RV_PLIC_PRIO_11_PRIO_11_OFFSET 0
#define RV_PLIC_PRIO_11_PRIO_11_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_11_PRIO_11_MASK, .index = RV_PLIC_PRIO_11_PRIO_11_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_12_REG_OFFSET 0x30
#define RV_PLIC_PRIO_12_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_12_PRIO_12_MASK 0x3u
#define RV_PLIC_PRIO_12_PRIO_12_OFFSET 0
#define RV_PLIC_PRIO_12_PRIO_12_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_12_PRIO_12_MASK, .index = RV_PLIC_PRIO_12_PRIO_12_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_13_REG_OFFSET 0x34
#define RV_PLIC_PRIO_13_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_13_PRIO_13_MASK 0x3u
#define RV_PLIC_PRIO_13_PRIO_13_OFFSET 0
#define RV_PLIC_PRIO_13_PRIO_13_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_13_PRIO_13_MASK, .index = RV_PLIC_PRIO_13_PRIO_13_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_14_REG_OFFSET 0x38
#define RV_PLIC_PRIO_14_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_14_PRIO_14_MASK 0x3u
#define RV_PLIC_PRIO_14_PRIO_14_OFFSET 0
#define RV_PLIC_PRIO_14_PRIO_14_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_14_PRIO_14_MASK, .index = RV_PLIC_PRIO_14_PRIO_14_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_15_REG_OFFSET 0x3c
#define RV_PLIC_PRIO_15_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_15_PRIO_15_MASK 0x3u
#define RV_PLIC_PRIO_15_PRIO_15_OFFSET 0
#define RV_PLIC_PRIO_15_PRIO_15_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_15_PRIO_15_MASK, .index = RV_PLIC_PRIO_15_PRIO_15_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_16_REG_OFFSET 0x40
#define RV_PLIC_PRIO_16_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_16_PRIO_16_MASK 0x3u
#define RV_PLIC_PRIO_16_PRIO_16_OFFSET 0
#define RV_PLIC_PRIO_16_PRIO_16_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_16_PRIO_16_MASK, .index = RV_PLIC_PRIO_16_PRIO_16_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_17_REG_OFFSET 0x44
#define RV_PLIC_PRIO_17_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_17_PRIO_17_MASK 0x3u
#define RV_PLIC_PRIO_17_PRIO_17_OFFSET 0
#define RV_PLIC_PRIO_17_PRIO_17_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_17_PRIO_17_MASK, .index = RV_PLIC_PRIO_17_PRIO_17_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_18_REG_OFFSET 0x48
#define RV_PLIC_PRIO_18_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_18_PRIO_18_MASK 0x3u
#define RV_PLIC_PRIO_18_PRIO_18_OFFSET 0
#define RV_PLIC_PRIO_18_PRIO_18_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_18_PRIO_18_MASK, .index = RV_PLIC_PRIO_18_PRIO_18_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_19_REG_OFFSET 0x4c
#define RV_PLIC_PRIO_19_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_19_PRIO_19_MASK 0x3u
#define RV_PLIC_PRIO_19_PRIO_19_OFFSET 0
#define RV_PLIC_PRIO_19_PRIO_19_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_19_PRIO_19_MASK, .index = RV_PLIC_PRIO_19_PRIO_19_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_20_REG_OFFSET 0x50
#define RV_PLIC_PRIO_20_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_20_PRIO_20_MASK 0x3u
#define RV_PLIC_PRIO_20_PRIO_20_OFFSET 0
#define RV_PLIC_PRIO_20_PRIO_20_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_20_PRIO_20_MASK, .index = RV_PLIC_PRIO_20_PRIO_20_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_21_REG_OFFSET 0x54
#define RV_PLIC_PRIO_21_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_21_PRIO_21_MASK 0x3u
#define RV_PLIC_PRIO_21_PRIO_21_OFFSET 0
#define RV_PLIC_PRIO_21_PRIO_21_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_21_PRIO_21_MASK, .index = RV_PLIC_PRIO_21_PRIO_21_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_22_REG_OFFSET 0x58
#define RV_PLIC_PRIO_22_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_22_PRIO_22_MASK 0x3u
#define RV_PLIC_PRIO_22_PRIO_22_OFFSET 0
#define RV_PLIC_PRIO_22_PRIO_22_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_22_PRIO_22_MASK, .index = RV_PLIC_PRIO_22_PRIO_22_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_23_REG_OFFSET 0x5c
#define RV_PLIC_PRIO_23_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_23_PRIO_23_MASK 0x3u
#define RV_PLIC_PRIO_23_PRIO_23_OFFSET 0
#define RV_PLIC_PRIO_23_PRIO_23_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_23_PRIO_23_MASK, .index = RV_PLIC_PRIO_23_PRIO_23_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_24_REG_OFFSET 0x60
#define RV_PLIC_PRIO_24_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_24_PRIO_24_MASK 0x3u
#define RV_PLIC_PRIO_24_PRIO_24_OFFSET 0
#define RV_PLIC_PRIO_24_PRIO_24_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_24_PRIO_24_MASK, .index = RV_PLIC_PRIO_24_PRIO_24_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_25_REG_OFFSET 0x64
#define RV_PLIC_PRIO_25_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_25_PRIO_25_MASK 0x3u
#define RV_PLIC_PRIO_25_PRIO_25_OFFSET 0
#define RV_PLIC_PRIO_25_PRIO_25_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_25_PRIO_25_MASK, .index = RV_PLIC_PRIO_25_PRIO_25_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_26_REG_OFFSET 0x68
#define RV_PLIC_PRIO_26_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_26_PRIO_26_MASK 0x3u
#define RV_PLIC_PRIO_26_PRIO_26_OFFSET 0
#define RV_PLIC_PRIO_26_PRIO_26_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_26_PRIO_26_MASK, .index = RV_PLIC_PRIO_26_PRIO_26_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_27_REG_OFFSET 0x6c
#define RV_PLIC_PRIO_27_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_27_PRIO_27_MASK 0x3u
#define RV_PLIC_PRIO_27_PRIO_27_OFFSET 0
#define RV_PLIC_PRIO_27_PRIO_27_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_27_PRIO_27_MASK, .index = RV_PLIC_PRIO_27_PRIO_27_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_28_REG_OFFSET 0x70
#define RV_PLIC_PRIO_28_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_28_PRIO_28_MASK 0x3u
#define RV_PLIC_PRIO_28_PRIO_28_OFFSET 0
#define RV_PLIC_PRIO_28_PRIO_28_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_28_PRIO_28_MASK, .index = RV_PLIC_PRIO_28_PRIO_28_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_29_REG_OFFSET 0x74
#define RV_PLIC_PRIO_29_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_29_PRIO_29_MASK 0x3u
#define RV_PLIC_PRIO_29_PRIO_29_OFFSET 0
#define RV_PLIC_PRIO_29_PRIO_29_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_29_PRIO_29_MASK, .index = RV_PLIC_PRIO_29_PRIO_29_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_30_REG_OFFSET 0x78
#define RV_PLIC_PRIO_30_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_30_PRIO_30_MASK 0x3u
#define RV_PLIC_PRIO_30_PRIO_30_OFFSET 0
#define RV_PLIC_PRIO_30_PRIO_30_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_30_PRIO_30_MASK, .index = RV_PLIC_PRIO_30_PRIO_30_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_31_REG_OFFSET 0x7c
#define RV_PLIC_PRIO_31_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_31_PRIO_31_MASK 0x3u
#define RV_PLIC_PRIO_31_PRIO_31_OFFSET 0
#define RV_PLIC_PRIO_31_PRIO_31_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_31_PRIO_31_MASK, .index = RV_PLIC_PRIO_31_PRIO_31_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_32_REG_OFFSET 0x80
#define RV_PLIC_PRIO_32_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_32_PRIO_32_MASK 0x3u
#define RV_PLIC_PRIO_32_PRIO_32_OFFSET 0
#define RV_PLIC_PRIO_32_PRIO_32_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_32_PRIO_32_MASK, .index = RV_PLIC_PRIO_32_PRIO_32_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_33_REG_OFFSET 0x84
#define RV_PLIC_PRIO_33_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_33_PRIO_33_MASK 0x3u
#define RV_PLIC_PRIO_33_PRIO_33_OFFSET 0
#define RV_PLIC_PRIO_33_PRIO_33_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_33_PRIO_33_MASK, .index = RV_PLIC_PRIO_33_PRIO_33_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_34_REG_OFFSET 0x88
#define RV_PLIC_PRIO_34_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_34_PRIO_34_MASK 0x3u
#define RV_PLIC_PRIO_34_PRIO_34_OFFSET 0
#define RV_PLIC_PRIO_34_PRIO_34_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_34_PRIO_34_MASK, .index = RV_PLIC_PRIO_34_PRIO_34_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_35_REG_OFFSET 0x8c
#define RV_PLIC_PRIO_35_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_35_PRIO_35_MASK 0x3u
#define RV_PLIC_PRIO_35_PRIO_35_OFFSET 0
#define RV_PLIC_PRIO_35_PRIO_35_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_35_PRIO_35_MASK, .index = RV_PLIC_PRIO_35_PRIO_35_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_36_REG_OFFSET 0x90
#define RV_PLIC_PRIO_36_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_36_PRIO_36_MASK 0x3u
#define RV_PLIC_PRIO_36_PRIO_36_OFFSET 0
#define RV_PLIC_PRIO_36_PRIO_36_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_36_PRIO_36_MASK, .index = RV_PLIC_PRIO_36_PRIO_36_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_37_REG_OFFSET 0x94
#define RV_PLIC_PRIO_37_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_37_PRIO_37_MASK 0x3u
#define RV_PLIC_PRIO_37_PRIO_37_OFFSET 0
#define RV_PLIC_PRIO_37_PRIO_37_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_37_PRIO_37_MASK, .index = RV_PLIC_PRIO_37_PRIO_37_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_38_REG_OFFSET 0x98
#define RV_PLIC_PRIO_38_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_38_PRIO_38_MASK 0x3u
#define RV_PLIC_PRIO_38_PRIO_38_OFFSET 0
#define RV_PLIC_PRIO_38_PRIO_38_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_38_PRIO_38_MASK, .index = RV_PLIC_PRIO_38_PRIO_38_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_39_REG_OFFSET 0x9c
#define RV_PLIC_PRIO_39_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_39_PRIO_39_MASK 0x3u
#define RV_PLIC_PRIO_39_PRIO_39_OFFSET 0
#define RV_PLIC_PRIO_39_PRIO_39_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_39_PRIO_39_MASK, .index = RV_PLIC_PRIO_39_PRIO_39_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_40_REG_OFFSET 0xa0
#define RV_PLIC_PRIO_40_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_40_PRIO_40_MASK 0x3u
#define RV_PLIC_PRIO_40_PRIO_40_OFFSET 0
#define RV_PLIC_PRIO_40_PRIO_40_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_40_PRIO_40_MASK, .index = RV_PLIC_PRIO_40_PRIO_40_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_41_REG_OFFSET 0xa4
#define RV_PLIC_PRIO_41_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_41_PRIO_41_MASK 0x3u
#define RV_PLIC_PRIO_41_PRIO_41_OFFSET 0
#define RV_PLIC_PRIO_41_PRIO_41_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_41_PRIO_41_MASK, .index = RV_PLIC_PRIO_41_PRIO_41_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_42_REG_OFFSET 0xa8
#define RV_PLIC_PRIO_42_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_42_PRIO_42_MASK 0x3u
#define RV_PLIC_PRIO_42_PRIO_42_OFFSET 0
#define RV_PLIC_PRIO_42_PRIO_42_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_42_PRIO_42_MASK, .index = RV_PLIC_PRIO_42_PRIO_42_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_43_REG_OFFSET 0xac
#define RV_PLIC_PRIO_43_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_43_PRIO_43_MASK 0x3u
#define RV_PLIC_PRIO_43_PRIO_43_OFFSET 0
#define RV_PLIC_PRIO_43_PRIO_43_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_43_PRIO_43_MASK, .index = RV_PLIC_PRIO_43_PRIO_43_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_44_REG_OFFSET 0xb0
#define RV_PLIC_PRIO_44_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_44_PRIO_44_MASK 0x3u
#define RV_PLIC_PRIO_44_PRIO_44_OFFSET 0
#define RV_PLIC_PRIO_44_PRIO_44_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_44_PRIO_44_MASK, .index = RV_PLIC_PRIO_44_PRIO_44_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_45_REG_OFFSET 0xb4
#define RV_PLIC_PRIO_45_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_45_PRIO_45_MASK 0x3u
#define RV_PLIC_PRIO_45_PRIO_45_OFFSET 0
#define RV_PLIC_PRIO_45_PRIO_45_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_45_PRIO_45_MASK, .index = RV_PLIC_PRIO_45_PRIO_45_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_46_REG_OFFSET 0xb8
#define RV_PLIC_PRIO_46_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_46_PRIO_46_MASK 0x3u
#define RV_PLIC_PRIO_46_PRIO_46_OFFSET 0
#define RV_PLIC_PRIO_46_PRIO_46_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_46_PRIO_46_MASK, .index = RV_PLIC_PRIO_46_PRIO_46_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_47_REG_OFFSET 0xbc
#define RV_PLIC_PRIO_47_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_47_PRIO_47_MASK 0x3u
#define RV_PLIC_PRIO_47_PRIO_47_OFFSET 0
#define RV_PLIC_PRIO_47_PRIO_47_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_47_PRIO_47_MASK, .index = RV_PLIC_PRIO_47_PRIO_47_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_48_REG_OFFSET 0xc0
#define RV_PLIC_PRIO_48_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_48_PRIO_48_MASK 0x3u
#define RV_PLIC_PRIO_48_PRIO_48_OFFSET 0
#define RV_PLIC_PRIO_48_PRIO_48_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_48_PRIO_48_MASK, .index = RV_PLIC_PRIO_48_PRIO_48_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_49_REG_OFFSET 0xc4
#define RV_PLIC_PRIO_49_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_49_PRIO_49_MASK 0x3u
#define RV_PLIC_PRIO_49_PRIO_49_OFFSET 0
#define RV_PLIC_PRIO_49_PRIO_49_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_49_PRIO_49_MASK, .index = RV_PLIC_PRIO_49_PRIO_49_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_50_REG_OFFSET 0xc8
#define RV_PLIC_PRIO_50_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_50_PRIO_50_MASK 0x3u
#define RV_PLIC_PRIO_50_PRIO_50_OFFSET 0
#define RV_PLIC_PRIO_50_PRIO_50_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_50_PRIO_50_MASK, .index = RV_PLIC_PRIO_50_PRIO_50_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_51_REG_OFFSET 0xcc
#define RV_PLIC_PRIO_51_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_51_PRIO_51_MASK 0x3u
#define RV_PLIC_PRIO_51_PRIO_51_OFFSET 0
#define RV_PLIC_PRIO_51_PRIO_51_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_51_PRIO_51_MASK, .index = RV_PLIC_PRIO_51_PRIO_51_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_52_REG_OFFSET 0xd0
#define RV_PLIC_PRIO_52_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_52_PRIO_52_MASK 0x3u
#define RV_PLIC_PRIO_52_PRIO_52_OFFSET 0
#define RV_PLIC_PRIO_52_PRIO_52_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_52_PRIO_52_MASK, .index = RV_PLIC_PRIO_52_PRIO_52_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_53_REG_OFFSET 0xd4
#define RV_PLIC_PRIO_53_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_53_PRIO_53_MASK 0x3u
#define RV_PLIC_PRIO_53_PRIO_53_OFFSET 0
#define RV_PLIC_PRIO_53_PRIO_53_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_53_PRIO_53_MASK, .index = RV_PLIC_PRIO_53_PRIO_53_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_54_REG_OFFSET 0xd8
#define RV_PLIC_PRIO_54_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_54_PRIO_54_MASK 0x3u
#define RV_PLIC_PRIO_54_PRIO_54_OFFSET 0
#define RV_PLIC_PRIO_54_PRIO_54_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_54_PRIO_54_MASK, .index = RV_PLIC_PRIO_54_PRIO_54_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_55_REG_OFFSET 0xdc
#define RV_PLIC_PRIO_55_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_55_PRIO_55_MASK 0x3u
#define RV_PLIC_PRIO_55_PRIO_55_OFFSET 0
#define RV_PLIC_PRIO_55_PRIO_55_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_55_PRIO_55_MASK, .index = RV_PLIC_PRIO_55_PRIO_55_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_56_REG_OFFSET 0xe0
#define RV_PLIC_PRIO_56_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_56_PRIO_56_MASK 0x3u
#define RV_PLIC_PRIO_56_PRIO_56_OFFSET 0
#define RV_PLIC_PRIO_56_PRIO_56_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_56_PRIO_56_MASK, .index = RV_PLIC_PRIO_56_PRIO_56_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_57_REG_OFFSET 0xe4
#define RV_PLIC_PRIO_57_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_57_PRIO_57_MASK 0x3u
#define RV_PLIC_PRIO_57_PRIO_57_OFFSET 0
#define RV_PLIC_PRIO_57_PRIO_57_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_57_PRIO_57_MASK, .index = RV_PLIC_PRIO_57_PRIO_57_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_58_REG_OFFSET 0xe8
#define RV_PLIC_PRIO_58_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_58_PRIO_58_MASK 0x3u
#define RV_PLIC_PRIO_58_PRIO_58_OFFSET 0
#define RV_PLIC_PRIO_58_PRIO_58_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_58_PRIO_58_MASK, .index = RV_PLIC_PRIO_58_PRIO_58_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_59_REG_OFFSET 0xec
#define RV_PLIC_PRIO_59_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_59_PRIO_59_MASK 0x3u
#define RV_PLIC_PRIO_59_PRIO_59_OFFSET 0
#define RV_PLIC_PRIO_59_PRIO_59_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_59_PRIO_59_MASK, .index = RV_PLIC_PRIO_59_PRIO_59_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_60_REG_OFFSET 0xf0
#define RV_PLIC_PRIO_60_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_60_PRIO_60_MASK 0x3u
#define RV_PLIC_PRIO_60_PRIO_60_OFFSET 0
#define RV_PLIC_PRIO_60_PRIO_60_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_60_PRIO_60_MASK, .index = RV_PLIC_PRIO_60_PRIO_60_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_61_REG_OFFSET 0xf4
#define RV_PLIC_PRIO_61_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_61_PRIO_61_MASK 0x3u
#define RV_PLIC_PRIO_61_PRIO_61_OFFSET 0
#define RV_PLIC_PRIO_61_PRIO_61_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_61_PRIO_61_MASK, .index = RV_PLIC_PRIO_61_PRIO_61_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_62_REG_OFFSET 0xf8
#define RV_PLIC_PRIO_62_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_62_PRIO_62_MASK 0x3u
#define RV_PLIC_PRIO_62_PRIO_62_OFFSET 0
#define RV_PLIC_PRIO_62_PRIO_62_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_62_PRIO_62_MASK, .index = RV_PLIC_PRIO_62_PRIO_62_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_63_REG_OFFSET 0xfc
#define RV_PLIC_PRIO_63_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_63_PRIO_63_MASK 0x3u
#define RV_PLIC_PRIO_63_PRIO_63_OFFSET 0
#define RV_PLIC_PRIO_63_PRIO_63_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_63_PRIO_63_MASK, .index = RV_PLIC_PRIO_63_PRIO_63_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_64_REG_OFFSET 0x100
#define RV_PLIC_PRIO_64_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_64_PRIO_64_MASK 0x3u
#define RV_PLIC_PRIO_64_PRIO_64_OFFSET 0
#define RV_PLIC_PRIO_64_PRIO_64_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_64_PRIO_64_MASK, .index = RV_PLIC_PRIO_64_PRIO_64_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_65_REG_OFFSET 0x104
#define RV_PLIC_PRIO_65_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_65_PRIO_65_MASK 0x3u
#define RV_PLIC_PRIO_65_PRIO_65_OFFSET 0
#define RV_PLIC_PRIO_65_PRIO_65_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_65_PRIO_65_MASK, .index = RV_PLIC_PRIO_65_PRIO_65_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_66_REG_OFFSET 0x108
#define RV_PLIC_PRIO_66_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_66_PRIO_66_MASK 0x3u
#define RV_PLIC_PRIO_66_PRIO_66_OFFSET 0
#define RV_PLIC_PRIO_66_PRIO_66_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_66_PRIO_66_MASK, .index = RV_PLIC_PRIO_66_PRIO_66_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_67_REG_OFFSET 0x10c
#define RV_PLIC_PRIO_67_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_67_PRIO_67_MASK 0x3u
#define RV_PLIC_PRIO_67_PRIO_67_OFFSET 0
#define RV_PLIC_PRIO_67_PRIO_67_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_67_PRIO_67_MASK, .index = RV_PLIC_PRIO_67_PRIO_67_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_68_REG_OFFSET 0x110
#define RV_PLIC_PRIO_68_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_68_PRIO_68_MASK 0x3u
#define RV_PLIC_PRIO_68_PRIO_68_OFFSET 0
#define RV_PLIC_PRIO_68_PRIO_68_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_68_PRIO_68_MASK, .index = RV_PLIC_PRIO_68_PRIO_68_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_69_REG_OFFSET 0x114
#define RV_PLIC_PRIO_69_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_69_PRIO_69_MASK 0x3u
#define RV_PLIC_PRIO_69_PRIO_69_OFFSET 0
#define RV_PLIC_PRIO_69_PRIO_69_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_69_PRIO_69_MASK, .index = RV_PLIC_PRIO_69_PRIO_69_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_70_REG_OFFSET 0x118
#define RV_PLIC_PRIO_70_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_70_PRIO_70_MASK 0x3u
#define RV_PLIC_PRIO_70_PRIO_70_OFFSET 0
#define RV_PLIC_PRIO_70_PRIO_70_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_70_PRIO_70_MASK, .index = RV_PLIC_PRIO_70_PRIO_70_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_71_REG_OFFSET 0x11c
#define RV_PLIC_PRIO_71_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_71_PRIO_71_MASK 0x3u
#define RV_PLIC_PRIO_71_PRIO_71_OFFSET 0
#define RV_PLIC_PRIO_71_PRIO_71_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_71_PRIO_71_MASK, .index = RV_PLIC_PRIO_71_PRIO_71_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_72_REG_OFFSET 0x120
#define RV_PLIC_PRIO_72_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_72_PRIO_72_MASK 0x3u
#define RV_PLIC_PRIO_72_PRIO_72_OFFSET 0
#define RV_PLIC_PRIO_72_PRIO_72_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_72_PRIO_72_MASK, .index = RV_PLIC_PRIO_72_PRIO_72_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_73_REG_OFFSET 0x124
#define RV_PLIC_PRIO_73_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_73_PRIO_73_MASK 0x3u
#define RV_PLIC_PRIO_73_PRIO_73_OFFSET 0
#define RV_PLIC_PRIO_73_PRIO_73_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_73_PRIO_73_MASK, .index = RV_PLIC_PRIO_73_PRIO_73_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_74_REG_OFFSET 0x128
#define RV_PLIC_PRIO_74_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_74_PRIO_74_MASK 0x3u
#define RV_PLIC_PRIO_74_PRIO_74_OFFSET 0
#define RV_PLIC_PRIO_74_PRIO_74_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_74_PRIO_74_MASK, .index = RV_PLIC_PRIO_74_PRIO_74_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_75_REG_OFFSET 0x12c
#define RV_PLIC_PRIO_75_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_75_PRIO_75_MASK 0x3u
#define RV_PLIC_PRIO_75_PRIO_75_OFFSET 0
#define RV_PLIC_PRIO_75_PRIO_75_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_75_PRIO_75_MASK, .index = RV_PLIC_PRIO_75_PRIO_75_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_76_REG_OFFSET 0x130
#define RV_PLIC_PRIO_76_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_76_PRIO_76_MASK 0x3u
#define RV_PLIC_PRIO_76_PRIO_76_OFFSET 0
#define RV_PLIC_PRIO_76_PRIO_76_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_76_PRIO_76_MASK, .index = RV_PLIC_PRIO_76_PRIO_76_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_77_REG_OFFSET 0x134
#define RV_PLIC_PRIO_77_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_77_PRIO_77_MASK 0x3u
#define RV_PLIC_PRIO_77_PRIO_77_OFFSET 0
#define RV_PLIC_PRIO_77_PRIO_77_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_77_PRIO_77_MASK, .index = RV_PLIC_PRIO_77_PRIO_77_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_78_REG_OFFSET 0x138
#define RV_PLIC_PRIO_78_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_78_PRIO_78_MASK 0x3u
#define RV_PLIC_PRIO_78_PRIO_78_OFFSET 0
#define RV_PLIC_PRIO_78_PRIO_78_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_78_PRIO_78_MASK, .index = RV_PLIC_PRIO_78_PRIO_78_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_79_REG_OFFSET 0x13c
#define RV_PLIC_PRIO_79_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_79_PRIO_79_MASK 0x3u
#define RV_PLIC_PRIO_79_PRIO_79_OFFSET 0
#define RV_PLIC_PRIO_79_PRIO_79_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_79_PRIO_79_MASK, .index = RV_PLIC_PRIO_79_PRIO_79_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_80_REG_OFFSET 0x140
#define RV_PLIC_PRIO_80_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_80_PRIO_80_MASK 0x3u
#define RV_PLIC_PRIO_80_PRIO_80_OFFSET 0
#define RV_PLIC_PRIO_80_PRIO_80_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_80_PRIO_80_MASK, .index = RV_PLIC_PRIO_80_PRIO_80_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_81_REG_OFFSET 0x144
#define RV_PLIC_PRIO_81_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_81_PRIO_81_MASK 0x3u
#define RV_PLIC_PRIO_81_PRIO_81_OFFSET 0
#define RV_PLIC_PRIO_81_PRIO_81_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_81_PRIO_81_MASK, .index = RV_PLIC_PRIO_81_PRIO_81_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_82_REG_OFFSET 0x148
#define RV_PLIC_PRIO_82_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_82_PRIO_82_MASK 0x3u
#define RV_PLIC_PRIO_82_PRIO_82_OFFSET 0
#define RV_PLIC_PRIO_82_PRIO_82_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_82_PRIO_82_MASK, .index = RV_PLIC_PRIO_82_PRIO_82_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_83_REG_OFFSET 0x14c
#define RV_PLIC_PRIO_83_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_83_PRIO_83_MASK 0x3u
#define RV_PLIC_PRIO_83_PRIO_83_OFFSET 0
#define RV_PLIC_PRIO_83_PRIO_83_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_83_PRIO_83_MASK, .index = RV_PLIC_PRIO_83_PRIO_83_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_84_REG_OFFSET 0x150
#define RV_PLIC_PRIO_84_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_84_PRIO_84_MASK 0x3u
#define RV_PLIC_PRIO_84_PRIO_84_OFFSET 0
#define RV_PLIC_PRIO_84_PRIO_84_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_84_PRIO_84_MASK, .index = RV_PLIC_PRIO_84_PRIO_84_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_85_REG_OFFSET 0x154
#define RV_PLIC_PRIO_85_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_85_PRIO_85_MASK 0x3u
#define RV_PLIC_PRIO_85_PRIO_85_OFFSET 0
#define RV_PLIC_PRIO_85_PRIO_85_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_85_PRIO_85_MASK, .index = RV_PLIC_PRIO_85_PRIO_85_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_86_REG_OFFSET 0x158
#define RV_PLIC_PRIO_86_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_86_PRIO_86_MASK 0x3u
#define RV_PLIC_PRIO_86_PRIO_86_OFFSET 0
#define RV_PLIC_PRIO_86_PRIO_86_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_86_PRIO_86_MASK, .index = RV_PLIC_PRIO_86_PRIO_86_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_87_REG_OFFSET 0x15c
#define RV_PLIC_PRIO_87_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_87_PRIO_87_MASK 0x3u
#define RV_PLIC_PRIO_87_PRIO_87_OFFSET 0
#define RV_PLIC_PRIO_87_PRIO_87_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_87_PRIO_87_MASK, .index = RV_PLIC_PRIO_87_PRIO_87_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_88_REG_OFFSET 0x160
#define RV_PLIC_PRIO_88_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_88_PRIO_88_MASK 0x3u
#define RV_PLIC_PRIO_88_PRIO_88_OFFSET 0
#define RV_PLIC_PRIO_88_PRIO_88_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_88_PRIO_88_MASK, .index = RV_PLIC_PRIO_88_PRIO_88_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_89_REG_OFFSET 0x164
#define RV_PLIC_PRIO_89_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_89_PRIO_89_MASK 0x3u
#define RV_PLIC_PRIO_89_PRIO_89_OFFSET 0
#define RV_PLIC_PRIO_89_PRIO_89_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_89_PRIO_89_MASK, .index = RV_PLIC_PRIO_89_PRIO_89_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_90_REG_OFFSET 0x168
#define RV_PLIC_PRIO_90_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_90_PRIO_90_MASK 0x3u
#define RV_PLIC_PRIO_90_PRIO_90_OFFSET 0
#define RV_PLIC_PRIO_90_PRIO_90_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_90_PRIO_90_MASK, .index = RV_PLIC_PRIO_90_PRIO_90_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_91_REG_OFFSET 0x16c
#define RV_PLIC_PRIO_91_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_91_PRIO_91_MASK 0x3u
#define RV_PLIC_PRIO_91_PRIO_91_OFFSET 0
#define RV_PLIC_PRIO_91_PRIO_91_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_91_PRIO_91_MASK, .index = RV_PLIC_PRIO_91_PRIO_91_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_92_REG_OFFSET 0x170
#define RV_PLIC_PRIO_92_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_92_PRIO_92_MASK 0x3u
#define RV_PLIC_PRIO_92_PRIO_92_OFFSET 0
#define RV_PLIC_PRIO_92_PRIO_92_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_92_PRIO_92_MASK, .index = RV_PLIC_PRIO_92_PRIO_92_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_93_REG_OFFSET 0x174
#define RV_PLIC_PRIO_93_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_93_PRIO_93_MASK 0x3u
#define RV_PLIC_PRIO_93_PRIO_93_OFFSET 0
#define RV_PLIC_PRIO_93_PRIO_93_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_93_PRIO_93_MASK, .index = RV_PLIC_PRIO_93_PRIO_93_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_94_REG_OFFSET 0x178
#define RV_PLIC_PRIO_94_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_94_PRIO_94_MASK 0x3u
#define RV_PLIC_PRIO_94_PRIO_94_OFFSET 0
#define RV_PLIC_PRIO_94_PRIO_94_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_94_PRIO_94_MASK, .index = RV_PLIC_PRIO_94_PRIO_94_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_95_REG_OFFSET 0x17c
#define RV_PLIC_PRIO_95_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_95_PRIO_95_MASK 0x3u
#define RV_PLIC_PRIO_95_PRIO_95_OFFSET 0
#define RV_PLIC_PRIO_95_PRIO_95_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_95_PRIO_95_MASK, .index = RV_PLIC_PRIO_95_PRIO_95_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_96_REG_OFFSET 0x180
#define RV_PLIC_PRIO_96_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_96_PRIO_96_MASK 0x3u
#define RV_PLIC_PRIO_96_PRIO_96_OFFSET 0
#define RV_PLIC_PRIO_96_PRIO_96_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_96_PRIO_96_MASK, .index = RV_PLIC_PRIO_96_PRIO_96_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_97_REG_OFFSET 0x184
#define RV_PLIC_PRIO_97_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_97_PRIO_97_MASK 0x3u
#define RV_PLIC_PRIO_97_PRIO_97_OFFSET 0
#define RV_PLIC_PRIO_97_PRIO_97_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_97_PRIO_97_MASK, .index = RV_PLIC_PRIO_97_PRIO_97_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_98_REG_OFFSET 0x188
#define RV_PLIC_PRIO_98_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_98_PRIO_98_MASK 0x3u
#define RV_PLIC_PRIO_98_PRIO_98_OFFSET 0
#define RV_PLIC_PRIO_98_PRIO_98_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_98_PRIO_98_MASK, .index = RV_PLIC_PRIO_98_PRIO_98_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_99_REG_OFFSET 0x18c
#define RV_PLIC_PRIO_99_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_99_PRIO_99_MASK 0x3u
#define RV_PLIC_PRIO_99_PRIO_99_OFFSET 0
#define RV_PLIC_PRIO_99_PRIO_99_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_99_PRIO_99_MASK, .index = RV_PLIC_PRIO_99_PRIO_99_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_100_REG_OFFSET 0x190
#define RV_PLIC_PRIO_100_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_100_PRIO_100_MASK 0x3u
#define RV_PLIC_PRIO_100_PRIO_100_OFFSET 0
#define RV_PLIC_PRIO_100_PRIO_100_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_100_PRIO_100_MASK, .index = RV_PLIC_PRIO_100_PRIO_100_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_101_REG_OFFSET 0x194
#define RV_PLIC_PRIO_101_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_101_PRIO_101_MASK 0x3u
#define RV_PLIC_PRIO_101_PRIO_101_OFFSET 0
#define RV_PLIC_PRIO_101_PRIO_101_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_101_PRIO_101_MASK, .index = RV_PLIC_PRIO_101_PRIO_101_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_102_REG_OFFSET 0x198
#define RV_PLIC_PRIO_102_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_102_PRIO_102_MASK 0x3u
#define RV_PLIC_PRIO_102_PRIO_102_OFFSET 0
#define RV_PLIC_PRIO_102_PRIO_102_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_102_PRIO_102_MASK, .index = RV_PLIC_PRIO_102_PRIO_102_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_103_REG_OFFSET 0x19c
#define RV_PLIC_PRIO_103_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_103_PRIO_103_MASK 0x3u
#define RV_PLIC_PRIO_103_PRIO_103_OFFSET 0
#define RV_PLIC_PRIO_103_PRIO_103_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_103_PRIO_103_MASK, .index = RV_PLIC_PRIO_103_PRIO_103_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_104_REG_OFFSET 0x1a0
#define RV_PLIC_PRIO_104_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_104_PRIO_104_MASK 0x3u
#define RV_PLIC_PRIO_104_PRIO_104_OFFSET 0
#define RV_PLIC_PRIO_104_PRIO_104_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_104_PRIO_104_MASK, .index = RV_PLIC_PRIO_104_PRIO_104_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_105_REG_OFFSET 0x1a4
#define RV_PLIC_PRIO_105_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_105_PRIO_105_MASK 0x3u
#define RV_PLIC_PRIO_105_PRIO_105_OFFSET 0
#define RV_PLIC_PRIO_105_PRIO_105_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_105_PRIO_105_MASK, .index = RV_PLIC_PRIO_105_PRIO_105_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_106_REG_OFFSET 0x1a8
#define RV_PLIC_PRIO_106_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_106_PRIO_106_MASK 0x3u
#define RV_PLIC_PRIO_106_PRIO_106_OFFSET 0
#define RV_PLIC_PRIO_106_PRIO_106_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_106_PRIO_106_MASK, .index = RV_PLIC_PRIO_106_PRIO_106_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_107_REG_OFFSET 0x1ac
#define RV_PLIC_PRIO_107_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_107_PRIO_107_MASK 0x3u
#define RV_PLIC_PRIO_107_PRIO_107_OFFSET 0
#define RV_PLIC_PRIO_107_PRIO_107_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_107_PRIO_107_MASK, .index = RV_PLIC_PRIO_107_PRIO_107_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_108_REG_OFFSET 0x1b0
#define RV_PLIC_PRIO_108_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_108_PRIO_108_MASK 0x3u
#define RV_PLIC_PRIO_108_PRIO_108_OFFSET 0
#define RV_PLIC_PRIO_108_PRIO_108_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_108_PRIO_108_MASK, .index = RV_PLIC_PRIO_108_PRIO_108_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_109_REG_OFFSET 0x1b4
#define RV_PLIC_PRIO_109_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_109_PRIO_109_MASK 0x3u
#define RV_PLIC_PRIO_109_PRIO_109_OFFSET 0
#define RV_PLIC_PRIO_109_PRIO_109_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_109_PRIO_109_MASK, .index = RV_PLIC_PRIO_109_PRIO_109_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_110_REG_OFFSET 0x1b8
#define RV_PLIC_PRIO_110_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_110_PRIO_110_MASK 0x3u
#define RV_PLIC_PRIO_110_PRIO_110_OFFSET 0
#define RV_PLIC_PRIO_110_PRIO_110_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_110_PRIO_110_MASK, .index = RV_PLIC_PRIO_110_PRIO_110_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_111_REG_OFFSET 0x1bc
#define RV_PLIC_PRIO_111_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_111_PRIO_111_MASK 0x3u
#define RV_PLIC_PRIO_111_PRIO_111_OFFSET 0
#define RV_PLIC_PRIO_111_PRIO_111_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_111_PRIO_111_MASK, .index = RV_PLIC_PRIO_111_PRIO_111_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_112_REG_OFFSET 0x1c0
#define RV_PLIC_PRIO_112_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_112_PRIO_112_MASK 0x3u
#define RV_PLIC_PRIO_112_PRIO_112_OFFSET 0
#define RV_PLIC_PRIO_112_PRIO_112_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_112_PRIO_112_MASK, .index = RV_PLIC_PRIO_112_PRIO_112_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_113_REG_OFFSET 0x1c4
#define RV_PLIC_PRIO_113_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_113_PRIO_113_MASK 0x3u
#define RV_PLIC_PRIO_113_PRIO_113_OFFSET 0
#define RV_PLIC_PRIO_113_PRIO_113_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_113_PRIO_113_MASK, .index = RV_PLIC_PRIO_113_PRIO_113_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_114_REG_OFFSET 0x1c8
#define RV_PLIC_PRIO_114_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_114_PRIO_114_MASK 0x3u
#define RV_PLIC_PRIO_114_PRIO_114_OFFSET 0
#define RV_PLIC_PRIO_114_PRIO_114_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_114_PRIO_114_MASK, .index = RV_PLIC_PRIO_114_PRIO_114_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_115_REG_OFFSET 0x1cc
#define RV_PLIC_PRIO_115_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_115_PRIO_115_MASK 0x3u
#define RV_PLIC_PRIO_115_PRIO_115_OFFSET 0
#define RV_PLIC_PRIO_115_PRIO_115_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_115_PRIO_115_MASK, .index = RV_PLIC_PRIO_115_PRIO_115_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_116_REG_OFFSET 0x1d0
#define RV_PLIC_PRIO_116_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_116_PRIO_116_MASK 0x3u
#define RV_PLIC_PRIO_116_PRIO_116_OFFSET 0
#define RV_PLIC_PRIO_116_PRIO_116_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_116_PRIO_116_MASK, .index = RV_PLIC_PRIO_116_PRIO_116_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_117_REG_OFFSET 0x1d4
#define RV_PLIC_PRIO_117_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_117_PRIO_117_MASK 0x3u
#define RV_PLIC_PRIO_117_PRIO_117_OFFSET 0
#define RV_PLIC_PRIO_117_PRIO_117_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_117_PRIO_117_MASK, .index = RV_PLIC_PRIO_117_PRIO_117_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_118_REG_OFFSET 0x1d8
#define RV_PLIC_PRIO_118_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_118_PRIO_118_MASK 0x3u
#define RV_PLIC_PRIO_118_PRIO_118_OFFSET 0
#define RV_PLIC_PRIO_118_PRIO_118_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_118_PRIO_118_MASK, .index = RV_PLIC_PRIO_118_PRIO_118_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_119_REG_OFFSET 0x1dc
#define RV_PLIC_PRIO_119_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_119_PRIO_119_MASK 0x3u
#define RV_PLIC_PRIO_119_PRIO_119_OFFSET 0
#define RV_PLIC_PRIO_119_PRIO_119_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_119_PRIO_119_MASK, .index = RV_PLIC_PRIO_119_PRIO_119_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_120_REG_OFFSET 0x1e0
#define RV_PLIC_PRIO_120_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_120_PRIO_120_MASK 0x3u
#define RV_PLIC_PRIO_120_PRIO_120_OFFSET 0
#define RV_PLIC_PRIO_120_PRIO_120_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_120_PRIO_120_MASK, .index = RV_PLIC_PRIO_120_PRIO_120_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_121_REG_OFFSET 0x1e4
#define RV_PLIC_PRIO_121_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_121_PRIO_121_MASK 0x3u
#define RV_PLIC_PRIO_121_PRIO_121_OFFSET 0
#define RV_PLIC_PRIO_121_PRIO_121_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_121_PRIO_121_MASK, .index = RV_PLIC_PRIO_121_PRIO_121_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_122_REG_OFFSET 0x1e8
#define RV_PLIC_PRIO_122_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_122_PRIO_122_MASK 0x3u
#define RV_PLIC_PRIO_122_PRIO_122_OFFSET 0
#define RV_PLIC_PRIO_122_PRIO_122_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_122_PRIO_122_MASK, .index = RV_PLIC_PRIO_122_PRIO_122_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_123_REG_OFFSET 0x1ec
#define RV_PLIC_PRIO_123_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_123_PRIO_123_MASK 0x3u
#define RV_PLIC_PRIO_123_PRIO_123_OFFSET 0
#define RV_PLIC_PRIO_123_PRIO_123_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_123_PRIO_123_MASK, .index = RV_PLIC_PRIO_123_PRIO_123_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_124_REG_OFFSET 0x1f0
#define RV_PLIC_PRIO_124_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_124_PRIO_124_MASK 0x3u
#define RV_PLIC_PRIO_124_PRIO_124_OFFSET 0
#define RV_PLIC_PRIO_124_PRIO_124_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_124_PRIO_124_MASK, .index = RV_PLIC_PRIO_124_PRIO_124_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_125_REG_OFFSET 0x1f4
#define RV_PLIC_PRIO_125_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_125_PRIO_125_MASK 0x3u
#define RV_PLIC_PRIO_125_PRIO_125_OFFSET 0
#define RV_PLIC_PRIO_125_PRIO_125_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_125_PRIO_125_MASK, .index = RV_PLIC_PRIO_125_PRIO_125_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_126_REG_OFFSET 0x1f8
#define RV_PLIC_PRIO_126_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_126_PRIO_126_MASK 0x3u
#define RV_PLIC_PRIO_126_PRIO_126_OFFSET 0
#define RV_PLIC_PRIO_126_PRIO_126_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_126_PRIO_126_MASK, .index = RV_PLIC_PRIO_126_PRIO_126_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_127_REG_OFFSET 0x1fc
#define RV_PLIC_PRIO_127_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_127_PRIO_127_MASK 0x3u
#define RV_PLIC_PRIO_127_PRIO_127_OFFSET 0
#define RV_PLIC_PRIO_127_PRIO_127_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_127_PRIO_127_MASK, .index = RV_PLIC_PRIO_127_PRIO_127_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_128_REG_OFFSET 0x200
#define RV_PLIC_PRIO_128_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_128_PRIO_128_MASK 0x3u
#define RV_PLIC_PRIO_128_PRIO_128_OFFSET 0
#define RV_PLIC_PRIO_128_PRIO_128_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_128_PRIO_128_MASK, .index = RV_PLIC_PRIO_128_PRIO_128_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_129_REG_OFFSET 0x204
#define RV_PLIC_PRIO_129_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_129_PRIO_129_MASK 0x3u
#define RV_PLIC_PRIO_129_PRIO_129_OFFSET 0
#define RV_PLIC_PRIO_129_PRIO_129_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_129_PRIO_129_MASK, .index = RV_PLIC_PRIO_129_PRIO_129_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_130_REG_OFFSET 0x208
#define RV_PLIC_PRIO_130_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_130_PRIO_130_MASK 0x3u
#define RV_PLIC_PRIO_130_PRIO_130_OFFSET 0
#define RV_PLIC_PRIO_130_PRIO_130_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_130_PRIO_130_MASK, .index = RV_PLIC_PRIO_130_PRIO_130_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_131_REG_OFFSET 0x20c
#define RV_PLIC_PRIO_131_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_131_PRIO_131_MASK 0x3u
#define RV_PLIC_PRIO_131_PRIO_131_OFFSET 0
#define RV_PLIC_PRIO_131_PRIO_131_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_131_PRIO_131_MASK, .index = RV_PLIC_PRIO_131_PRIO_131_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_132_REG_OFFSET 0x210
#define RV_PLIC_PRIO_132_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_132_PRIO_132_MASK 0x3u
#define RV_PLIC_PRIO_132_PRIO_132_OFFSET 0
#define RV_PLIC_PRIO_132_PRIO_132_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_132_PRIO_132_MASK, .index = RV_PLIC_PRIO_132_PRIO_132_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_133_REG_OFFSET 0x214
#define RV_PLIC_PRIO_133_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_133_PRIO_133_MASK 0x3u
#define RV_PLIC_PRIO_133_PRIO_133_OFFSET 0
#define RV_PLIC_PRIO_133_PRIO_133_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_133_PRIO_133_MASK, .index = RV_PLIC_PRIO_133_PRIO_133_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_134_REG_OFFSET 0x218
#define RV_PLIC_PRIO_134_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_134_PRIO_134_MASK 0x3u
#define RV_PLIC_PRIO_134_PRIO_134_OFFSET 0
#define RV_PLIC_PRIO_134_PRIO_134_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_134_PRIO_134_MASK, .index = RV_PLIC_PRIO_134_PRIO_134_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_135_REG_OFFSET 0x21c
#define RV_PLIC_PRIO_135_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_135_PRIO_135_MASK 0x3u
#define RV_PLIC_PRIO_135_PRIO_135_OFFSET 0
#define RV_PLIC_PRIO_135_PRIO_135_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_135_PRIO_135_MASK, .index = RV_PLIC_PRIO_135_PRIO_135_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_136_REG_OFFSET 0x220
#define RV_PLIC_PRIO_136_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_136_PRIO_136_MASK 0x3u
#define RV_PLIC_PRIO_136_PRIO_136_OFFSET 0
#define RV_PLIC_PRIO_136_PRIO_136_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_136_PRIO_136_MASK, .index = RV_PLIC_PRIO_136_PRIO_136_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_137_REG_OFFSET 0x224
#define RV_PLIC_PRIO_137_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_137_PRIO_137_MASK 0x3u
#define RV_PLIC_PRIO_137_PRIO_137_OFFSET 0
#define RV_PLIC_PRIO_137_PRIO_137_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_137_PRIO_137_MASK, .index = RV_PLIC_PRIO_137_PRIO_137_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_138_REG_OFFSET 0x228
#define RV_PLIC_PRIO_138_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_138_PRIO_138_MASK 0x3u
#define RV_PLIC_PRIO_138_PRIO_138_OFFSET 0
#define RV_PLIC_PRIO_138_PRIO_138_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_138_PRIO_138_MASK, .index = RV_PLIC_PRIO_138_PRIO_138_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_139_REG_OFFSET 0x22c
#define RV_PLIC_PRIO_139_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_139_PRIO_139_MASK 0x3u
#define RV_PLIC_PRIO_139_PRIO_139_OFFSET 0
#define RV_PLIC_PRIO_139_PRIO_139_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_139_PRIO_139_MASK, .index = RV_PLIC_PRIO_139_PRIO_139_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_140_REG_OFFSET 0x230
#define RV_PLIC_PRIO_140_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_140_PRIO_140_MASK 0x3u
#define RV_PLIC_PRIO_140_PRIO_140_OFFSET 0
#define RV_PLIC_PRIO_140_PRIO_140_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_140_PRIO_140_MASK, .index = RV_PLIC_PRIO_140_PRIO_140_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_141_REG_OFFSET 0x234
#define RV_PLIC_PRIO_141_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_141_PRIO_141_MASK 0x3u
#define RV_PLIC_PRIO_141_PRIO_141_OFFSET 0
#define RV_PLIC_PRIO_141_PRIO_141_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_141_PRIO_141_MASK, .index = RV_PLIC_PRIO_141_PRIO_141_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_142_REG_OFFSET 0x238
#define RV_PLIC_PRIO_142_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_142_PRIO_142_MASK 0x3u
#define RV_PLIC_PRIO_142_PRIO_142_OFFSET 0
#define RV_PLIC_PRIO_142_PRIO_142_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_142_PRIO_142_MASK, .index = RV_PLIC_PRIO_142_PRIO_142_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_143_REG_OFFSET 0x23c
#define RV_PLIC_PRIO_143_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_143_PRIO_143_MASK 0x3u
#define RV_PLIC_PRIO_143_PRIO_143_OFFSET 0
#define RV_PLIC_PRIO_143_PRIO_143_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_143_PRIO_143_MASK, .index = RV_PLIC_PRIO_143_PRIO_143_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_144_REG_OFFSET 0x240
#define RV_PLIC_PRIO_144_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_144_PRIO_144_MASK 0x3u
#define RV_PLIC_PRIO_144_PRIO_144_OFFSET 0
#define RV_PLIC_PRIO_144_PRIO_144_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_144_PRIO_144_MASK, .index = RV_PLIC_PRIO_144_PRIO_144_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_145_REG_OFFSET 0x244
#define RV_PLIC_PRIO_145_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_145_PRIO_145_MASK 0x3u
#define RV_PLIC_PRIO_145_PRIO_145_OFFSET 0
#define RV_PLIC_PRIO_145_PRIO_145_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_145_PRIO_145_MASK, .index = RV_PLIC_PRIO_145_PRIO_145_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_146_REG_OFFSET 0x248
#define RV_PLIC_PRIO_146_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_146_PRIO_146_MASK 0x3u
#define RV_PLIC_PRIO_146_PRIO_146_OFFSET 0
#define RV_PLIC_PRIO_146_PRIO_146_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_146_PRIO_146_MASK, .index = RV_PLIC_PRIO_146_PRIO_146_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_147_REG_OFFSET 0x24c
#define RV_PLIC_PRIO_147_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_147_PRIO_147_MASK 0x3u
#define RV_PLIC_PRIO_147_PRIO_147_OFFSET 0
#define RV_PLIC_PRIO_147_PRIO_147_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_147_PRIO_147_MASK, .index = RV_PLIC_PRIO_147_PRIO_147_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_148_REG_OFFSET 0x250
#define RV_PLIC_PRIO_148_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_148_PRIO_148_MASK 0x3u
#define RV_PLIC_PRIO_148_PRIO_148_OFFSET 0
#define RV_PLIC_PRIO_148_PRIO_148_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_148_PRIO_148_MASK, .index = RV_PLIC_PRIO_148_PRIO_148_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_149_REG_OFFSET 0x254
#define RV_PLIC_PRIO_149_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_149_PRIO_149_MASK 0x3u
#define RV_PLIC_PRIO_149_PRIO_149_OFFSET 0
#define RV_PLIC_PRIO_149_PRIO_149_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_149_PRIO_149_MASK, .index = RV_PLIC_PRIO_149_PRIO_149_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_150_REG_OFFSET 0x258
#define RV_PLIC_PRIO_150_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_150_PRIO_150_MASK 0x3u
#define RV_PLIC_PRIO_150_PRIO_150_OFFSET 0
#define RV_PLIC_PRIO_150_PRIO_150_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_150_PRIO_150_MASK, .index = RV_PLIC_PRIO_150_PRIO_150_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_151_REG_OFFSET 0x25c
#define RV_PLIC_PRIO_151_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_151_PRIO_151_MASK 0x3u
#define RV_PLIC_PRIO_151_PRIO_151_OFFSET 0
#define RV_PLIC_PRIO_151_PRIO_151_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_151_PRIO_151_MASK, .index = RV_PLIC_PRIO_151_PRIO_151_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_152_REG_OFFSET 0x260
#define RV_PLIC_PRIO_152_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_152_PRIO_152_MASK 0x3u
#define RV_PLIC_PRIO_152_PRIO_152_OFFSET 0
#define RV_PLIC_PRIO_152_PRIO_152_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_152_PRIO_152_MASK, .index = RV_PLIC_PRIO_152_PRIO_152_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_153_REG_OFFSET 0x264
#define RV_PLIC_PRIO_153_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_153_PRIO_153_MASK 0x3u
#define RV_PLIC_PRIO_153_PRIO_153_OFFSET 0
#define RV_PLIC_PRIO_153_PRIO_153_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_153_PRIO_153_MASK, .index = RV_PLIC_PRIO_153_PRIO_153_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_154_REG_OFFSET 0x268
#define RV_PLIC_PRIO_154_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_154_PRIO_154_MASK 0x3u
#define RV_PLIC_PRIO_154_PRIO_154_OFFSET 0
#define RV_PLIC_PRIO_154_PRIO_154_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_154_PRIO_154_MASK, .index = RV_PLIC_PRIO_154_PRIO_154_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_155_REG_OFFSET 0x26c
#define RV_PLIC_PRIO_155_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_155_PRIO_155_MASK 0x3u
#define RV_PLIC_PRIO_155_PRIO_155_OFFSET 0
#define RV_PLIC_PRIO_155_PRIO_155_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_155_PRIO_155_MASK, .index = RV_PLIC_PRIO_155_PRIO_155_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_156_REG_OFFSET 0x270
#define RV_PLIC_PRIO_156_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_156_PRIO_156_MASK 0x3u
#define RV_PLIC_PRIO_156_PRIO_156_OFFSET 0
#define RV_PLIC_PRIO_156_PRIO_156_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_156_PRIO_156_MASK, .index = RV_PLIC_PRIO_156_PRIO_156_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_157_REG_OFFSET 0x274
#define RV_PLIC_PRIO_157_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_157_PRIO_157_MASK 0x3u
#define RV_PLIC_PRIO_157_PRIO_157_OFFSET 0
#define RV_PLIC_PRIO_157_PRIO_157_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_157_PRIO_157_MASK, .index = RV_PLIC_PRIO_157_PRIO_157_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_158_REG_OFFSET 0x278
#define RV_PLIC_PRIO_158_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_158_PRIO_158_MASK 0x3u
#define RV_PLIC_PRIO_158_PRIO_158_OFFSET 0
#define RV_PLIC_PRIO_158_PRIO_158_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_158_PRIO_158_MASK, .index = RV_PLIC_PRIO_158_PRIO_158_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_159_REG_OFFSET 0x27c
#define RV_PLIC_PRIO_159_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_159_PRIO_159_MASK 0x3u
#define RV_PLIC_PRIO_159_PRIO_159_OFFSET 0
#define RV_PLIC_PRIO_159_PRIO_159_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_159_PRIO_159_MASK, .index = RV_PLIC_PRIO_159_PRIO_159_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_160_REG_OFFSET 0x280
#define RV_PLIC_PRIO_160_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_160_PRIO_160_MASK 0x3u
#define RV_PLIC_PRIO_160_PRIO_160_OFFSET 0
#define RV_PLIC_PRIO_160_PRIO_160_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_160_PRIO_160_MASK, .index = RV_PLIC_PRIO_160_PRIO_160_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_161_REG_OFFSET 0x284
#define RV_PLIC_PRIO_161_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_161_PRIO_161_MASK 0x3u
#define RV_PLIC_PRIO_161_PRIO_161_OFFSET 0
#define RV_PLIC_PRIO_161_PRIO_161_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_161_PRIO_161_MASK, .index = RV_PLIC_PRIO_161_PRIO_161_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_162_REG_OFFSET 0x288
#define RV_PLIC_PRIO_162_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_162_PRIO_162_MASK 0x3u
#define RV_PLIC_PRIO_162_PRIO_162_OFFSET 0
#define RV_PLIC_PRIO_162_PRIO_162_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_162_PRIO_162_MASK, .index = RV_PLIC_PRIO_162_PRIO_162_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_163_REG_OFFSET 0x28c
#define RV_PLIC_PRIO_163_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_163_PRIO_163_MASK 0x3u
#define RV_PLIC_PRIO_163_PRIO_163_OFFSET 0
#define RV_PLIC_PRIO_163_PRIO_163_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_163_PRIO_163_MASK, .index = RV_PLIC_PRIO_163_PRIO_163_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_164_REG_OFFSET 0x290
#define RV_PLIC_PRIO_164_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_164_PRIO_164_MASK 0x3u
#define RV_PLIC_PRIO_164_PRIO_164_OFFSET 0
#define RV_PLIC_PRIO_164_PRIO_164_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_164_PRIO_164_MASK, .index = RV_PLIC_PRIO_164_PRIO_164_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_165_REG_OFFSET 0x294
#define RV_PLIC_PRIO_165_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_165_PRIO_165_MASK 0x3u
#define RV_PLIC_PRIO_165_PRIO_165_OFFSET 0
#define RV_PLIC_PRIO_165_PRIO_165_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_165_PRIO_165_MASK, .index = RV_PLIC_PRIO_165_PRIO_165_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_166_REG_OFFSET 0x298
#define RV_PLIC_PRIO_166_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_166_PRIO_166_MASK 0x3u
#define RV_PLIC_PRIO_166_PRIO_166_OFFSET 0
#define RV_PLIC_PRIO_166_PRIO_166_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_166_PRIO_166_MASK, .index = RV_PLIC_PRIO_166_PRIO_166_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_167_REG_OFFSET 0x29c
#define RV_PLIC_PRIO_167_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_167_PRIO_167_MASK 0x3u
#define RV_PLIC_PRIO_167_PRIO_167_OFFSET 0
#define RV_PLIC_PRIO_167_PRIO_167_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_167_PRIO_167_MASK, .index = RV_PLIC_PRIO_167_PRIO_167_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_168_REG_OFFSET 0x2a0
#define RV_PLIC_PRIO_168_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_168_PRIO_168_MASK 0x3u
#define RV_PLIC_PRIO_168_PRIO_168_OFFSET 0
#define RV_PLIC_PRIO_168_PRIO_168_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_168_PRIO_168_MASK, .index = RV_PLIC_PRIO_168_PRIO_168_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_169_REG_OFFSET 0x2a4
#define RV_PLIC_PRIO_169_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_169_PRIO_169_MASK 0x3u
#define RV_PLIC_PRIO_169_PRIO_169_OFFSET 0
#define RV_PLIC_PRIO_169_PRIO_169_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_169_PRIO_169_MASK, .index = RV_PLIC_PRIO_169_PRIO_169_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_170_REG_OFFSET 0x2a8
#define RV_PLIC_PRIO_170_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_170_PRIO_170_MASK 0x3u
#define RV_PLIC_PRIO_170_PRIO_170_OFFSET 0
#define RV_PLIC_PRIO_170_PRIO_170_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_170_PRIO_170_MASK, .index = RV_PLIC_PRIO_170_PRIO_170_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_171_REG_OFFSET 0x2ac
#define RV_PLIC_PRIO_171_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_171_PRIO_171_MASK 0x3u
#define RV_PLIC_PRIO_171_PRIO_171_OFFSET 0
#define RV_PLIC_PRIO_171_PRIO_171_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_171_PRIO_171_MASK, .index = RV_PLIC_PRIO_171_PRIO_171_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_172_REG_OFFSET 0x2b0
#define RV_PLIC_PRIO_172_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_172_PRIO_172_MASK 0x3u
#define RV_PLIC_PRIO_172_PRIO_172_OFFSET 0
#define RV_PLIC_PRIO_172_PRIO_172_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_172_PRIO_172_MASK, .index = RV_PLIC_PRIO_172_PRIO_172_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_173_REG_OFFSET 0x2b4
#define RV_PLIC_PRIO_173_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_173_PRIO_173_MASK 0x3u
#define RV_PLIC_PRIO_173_PRIO_173_OFFSET 0
#define RV_PLIC_PRIO_173_PRIO_173_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_173_PRIO_173_MASK, .index = RV_PLIC_PRIO_173_PRIO_173_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_174_REG_OFFSET 0x2b8
#define RV_PLIC_PRIO_174_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_174_PRIO_174_MASK 0x3u
#define RV_PLIC_PRIO_174_PRIO_174_OFFSET 0
#define RV_PLIC_PRIO_174_PRIO_174_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_174_PRIO_174_MASK, .index = RV_PLIC_PRIO_174_PRIO_174_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_175_REG_OFFSET 0x2bc
#define RV_PLIC_PRIO_175_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_175_PRIO_175_MASK 0x3u
#define RV_PLIC_PRIO_175_PRIO_175_OFFSET 0
#define RV_PLIC_PRIO_175_PRIO_175_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_175_PRIO_175_MASK, .index = RV_PLIC_PRIO_175_PRIO_175_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_176_REG_OFFSET 0x2c0
#define RV_PLIC_PRIO_176_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_176_PRIO_176_MASK 0x3u
#define RV_PLIC_PRIO_176_PRIO_176_OFFSET 0
#define RV_PLIC_PRIO_176_PRIO_176_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_176_PRIO_176_MASK, .index = RV_PLIC_PRIO_176_PRIO_176_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_177_REG_OFFSET 0x2c4
#define RV_PLIC_PRIO_177_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_177_PRIO_177_MASK 0x3u
#define RV_PLIC_PRIO_177_PRIO_177_OFFSET 0
#define RV_PLIC_PRIO_177_PRIO_177_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_177_PRIO_177_MASK, .index = RV_PLIC_PRIO_177_PRIO_177_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_178_REG_OFFSET 0x2c8
#define RV_PLIC_PRIO_178_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_178_PRIO_178_MASK 0x3u
#define RV_PLIC_PRIO_178_PRIO_178_OFFSET 0
#define RV_PLIC_PRIO_178_PRIO_178_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_178_PRIO_178_MASK, .index = RV_PLIC_PRIO_178_PRIO_178_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_179_REG_OFFSET 0x2cc
#define RV_PLIC_PRIO_179_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_179_PRIO_179_MASK 0x3u
#define RV_PLIC_PRIO_179_PRIO_179_OFFSET 0
#define RV_PLIC_PRIO_179_PRIO_179_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_179_PRIO_179_MASK, .index = RV_PLIC_PRIO_179_PRIO_179_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_180_REG_OFFSET 0x2d0
#define RV_PLIC_PRIO_180_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_180_PRIO_180_MASK 0x3u
#define RV_PLIC_PRIO_180_PRIO_180_OFFSET 0
#define RV_PLIC_PRIO_180_PRIO_180_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_180_PRIO_180_MASK, .index = RV_PLIC_PRIO_180_PRIO_180_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_181_REG_OFFSET 0x2d4
#define RV_PLIC_PRIO_181_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_181_PRIO_181_MASK 0x3u
#define RV_PLIC_PRIO_181_PRIO_181_OFFSET 0
#define RV_PLIC_PRIO_181_PRIO_181_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_181_PRIO_181_MASK, .index = RV_PLIC_PRIO_181_PRIO_181_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_182_REG_OFFSET 0x2d8
#define RV_PLIC_PRIO_182_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_182_PRIO_182_MASK 0x3u
#define RV_PLIC_PRIO_182_PRIO_182_OFFSET 0
#define RV_PLIC_PRIO_182_PRIO_182_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_182_PRIO_182_MASK, .index = RV_PLIC_PRIO_182_PRIO_182_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_183_REG_OFFSET 0x2dc
#define RV_PLIC_PRIO_183_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_183_PRIO_183_MASK 0x3u
#define RV_PLIC_PRIO_183_PRIO_183_OFFSET 0
#define RV_PLIC_PRIO_183_PRIO_183_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_183_PRIO_183_MASK, .index = RV_PLIC_PRIO_183_PRIO_183_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_184_REG_OFFSET 0x2e0
#define RV_PLIC_PRIO_184_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_184_PRIO_184_MASK 0x3u
#define RV_PLIC_PRIO_184_PRIO_184_OFFSET 0
#define RV_PLIC_PRIO_184_PRIO_184_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_184_PRIO_184_MASK, .index = RV_PLIC_PRIO_184_PRIO_184_OFFSET })

// Interrupt Source Priority
#define RV_PLIC_PRIO_185_REG_OFFSET 0x2e4
#define RV_PLIC_PRIO_185_REG_RESVAL 0x0u
#define RV_PLIC_PRIO_185_PRIO_185_MASK 0x3u
#define RV_PLIC_PRIO_185_PRIO_185_OFFSET 0
#define RV_PLIC_PRIO_185_PRIO_185_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_PRIO_185_PRIO_185_MASK, .index = RV_PLIC_PRIO_185_PRIO_185_OFFSET })

// Interrupt Pending (common parameters)
#define RV_PLIC_IP_P_FIELD_WIDTH 1
#define RV_PLIC_IP_MULTIREG_COUNT 6

// Interrupt Pending
#define RV_PLIC_IP_0_REG_OFFSET 0x1000
#define RV_PLIC_IP_0_REG_RESVAL 0x0u
#define RV_PLIC_IP_0_P_0_BIT 0
#define RV_PLIC_IP_0_P_1_BIT 1
#define RV_PLIC_IP_0_P_2_BIT 2
#define RV_PLIC_IP_0_P_3_BIT 3
#define RV_PLIC_IP_0_P_4_BIT 4
#define RV_PLIC_IP_0_P_5_BIT 5
#define RV_PLIC_IP_0_P_6_BIT 6
#define RV_PLIC_IP_0_P_7_BIT 7
#define RV_PLIC_IP_0_P_8_BIT 8
#define RV_PLIC_IP_0_P_9_BIT 9
#define RV_PLIC_IP_0_P_10_BIT 10
#define RV_PLIC_IP_0_P_11_BIT 11
#define RV_PLIC_IP_0_P_12_BIT 12
#define RV_PLIC_IP_0_P_13_BIT 13
#define RV_PLIC_IP_0_P_14_BIT 14
#define RV_PLIC_IP_0_P_15_BIT 15
#define RV_PLIC_IP_0_P_16_BIT 16
#define RV_PLIC_IP_0_P_17_BIT 17
#define RV_PLIC_IP_0_P_18_BIT 18
#define RV_PLIC_IP_0_P_19_BIT 19
#define RV_PLIC_IP_0_P_20_BIT 20
#define RV_PLIC_IP_0_P_21_BIT 21
#define RV_PLIC_IP_0_P_22_BIT 22
#define RV_PLIC_IP_0_P_23_BIT 23
#define RV_PLIC_IP_0_P_24_BIT 24
#define RV_PLIC_IP_0_P_25_BIT 25
#define RV_PLIC_IP_0_P_26_BIT 26
#define RV_PLIC_IP_0_P_27_BIT 27
#define RV_PLIC_IP_0_P_28_BIT 28
#define RV_PLIC_IP_0_P_29_BIT 29
#define RV_PLIC_IP_0_P_30_BIT 30
#define RV_PLIC_IP_0_P_31_BIT 31

// Interrupt Pending
#define RV_PLIC_IP_1_REG_OFFSET 0x1004
#define RV_PLIC_IP_1_REG_RESVAL 0x0u
#define RV_PLIC_IP_1_P_32_BIT 0
#define RV_PLIC_IP_1_P_33_BIT 1
#define RV_PLIC_IP_1_P_34_BIT 2
#define RV_PLIC_IP_1_P_35_BIT 3
#define RV_PLIC_IP_1_P_36_BIT 4
#define RV_PLIC_IP_1_P_37_BIT 5
#define RV_PLIC_IP_1_P_38_BIT 6
#define RV_PLIC_IP_1_P_39_BIT 7
#define RV_PLIC_IP_1_P_40_BIT 8
#define RV_PLIC_IP_1_P_41_BIT 9
#define RV_PLIC_IP_1_P_42_BIT 10
#define RV_PLIC_IP_1_P_43_BIT 11
#define RV_PLIC_IP_1_P_44_BIT 12
#define RV_PLIC_IP_1_P_45_BIT 13
#define RV_PLIC_IP_1_P_46_BIT 14
#define RV_PLIC_IP_1_P_47_BIT 15
#define RV_PLIC_IP_1_P_48_BIT 16
#define RV_PLIC_IP_1_P_49_BIT 17
#define RV_PLIC_IP_1_P_50_BIT 18
#define RV_PLIC_IP_1_P_51_BIT 19
#define RV_PLIC_IP_1_P_52_BIT 20
#define RV_PLIC_IP_1_P_53_BIT 21
#define RV_PLIC_IP_1_P_54_BIT 22
#define RV_PLIC_IP_1_P_55_BIT 23
#define RV_PLIC_IP_1_P_56_BIT 24
#define RV_PLIC_IP_1_P_57_BIT 25
#define RV_PLIC_IP_1_P_58_BIT 26
#define RV_PLIC_IP_1_P_59_BIT 27
#define RV_PLIC_IP_1_P_60_BIT 28
#define RV_PLIC_IP_1_P_61_BIT 29
#define RV_PLIC_IP_1_P_62_BIT 30
#define RV_PLIC_IP_1_P_63_BIT 31

// Interrupt Pending
#define RV_PLIC_IP_2_REG_OFFSET 0x1008
#define RV_PLIC_IP_2_REG_RESVAL 0x0u
#define RV_PLIC_IP_2_P_64_BIT 0
#define RV_PLIC_IP_2_P_65_BIT 1
#define RV_PLIC_IP_2_P_66_BIT 2
#define RV_PLIC_IP_2_P_67_BIT 3
#define RV_PLIC_IP_2_P_68_BIT 4
#define RV_PLIC_IP_2_P_69_BIT 5
#define RV_PLIC_IP_2_P_70_BIT 6
#define RV_PLIC_IP_2_P_71_BIT 7
#define RV_PLIC_IP_2_P_72_BIT 8
#define RV_PLIC_IP_2_P_73_BIT 9
#define RV_PLIC_IP_2_P_74_BIT 10
#define RV_PLIC_IP_2_P_75_BIT 11
#define RV_PLIC_IP_2_P_76_BIT 12
#define RV_PLIC_IP_2_P_77_BIT 13
#define RV_PLIC_IP_2_P_78_BIT 14
#define RV_PLIC_IP_2_P_79_BIT 15
#define RV_PLIC_IP_2_P_80_BIT 16
#define RV_PLIC_IP_2_P_81_BIT 17
#define RV_PLIC_IP_2_P_82_BIT 18
#define RV_PLIC_IP_2_P_83_BIT 19
#define RV_PLIC_IP_2_P_84_BIT 20
#define RV_PLIC_IP_2_P_85_BIT 21
#define RV_PLIC_IP_2_P_86_BIT 22
#define RV_PLIC_IP_2_P_87_BIT 23
#define RV_PLIC_IP_2_P_88_BIT 24
#define RV_PLIC_IP_2_P_89_BIT 25
#define RV_PLIC_IP_2_P_90_BIT 26
#define RV_PLIC_IP_2_P_91_BIT 27
#define RV_PLIC_IP_2_P_92_BIT 28
#define RV_PLIC_IP_2_P_93_BIT 29
#define RV_PLIC_IP_2_P_94_BIT 30
#define RV_PLIC_IP_2_P_95_BIT 31

// Interrupt Pending
#define RV_PLIC_IP_3_REG_OFFSET 0x100c
#define RV_PLIC_IP_3_REG_RESVAL 0x0u
#define RV_PLIC_IP_3_P_96_BIT 0
#define RV_PLIC_IP_3_P_97_BIT 1
#define RV_PLIC_IP_3_P_98_BIT 2
#define RV_PLIC_IP_3_P_99_BIT 3
#define RV_PLIC_IP_3_P_100_BIT 4
#define RV_PLIC_IP_3_P_101_BIT 5
#define RV_PLIC_IP_3_P_102_BIT 6
#define RV_PLIC_IP_3_P_103_BIT 7
#define RV_PLIC_IP_3_P_104_BIT 8
#define RV_PLIC_IP_3_P_105_BIT 9
#define RV_PLIC_IP_3_P_106_BIT 10
#define RV_PLIC_IP_3_P_107_BIT 11
#define RV_PLIC_IP_3_P_108_BIT 12
#define RV_PLIC_IP_3_P_109_BIT 13
#define RV_PLIC_IP_3_P_110_BIT 14
#define RV_PLIC_IP_3_P_111_BIT 15
#define RV_PLIC_IP_3_P_112_BIT 16
#define RV_PLIC_IP_3_P_113_BIT 17
#define RV_PLIC_IP_3_P_114_BIT 18
#define RV_PLIC_IP_3_P_115_BIT 19
#define RV_PLIC_IP_3_P_116_BIT 20
#define RV_PLIC_IP_3_P_117_BIT 21
#define RV_PLIC_IP_3_P_118_BIT 22
#define RV_PLIC_IP_3_P_119_BIT 23
#define RV_PLIC_IP_3_P_120_BIT 24
#define RV_PLIC_IP_3_P_121_BIT 25
#define RV_PLIC_IP_3_P_122_BIT 26
#define RV_PLIC_IP_3_P_123_BIT 27
#define RV_PLIC_IP_3_P_124_BIT 28
#define RV_PLIC_IP_3_P_125_BIT 29
#define RV_PLIC_IP_3_P_126_BIT 30
#define RV_PLIC_IP_3_P_127_BIT 31

// Interrupt Pending
#define RV_PLIC_IP_4_REG_OFFSET 0x1010
#define RV_PLIC_IP_4_REG_RESVAL 0x0u
#define RV_PLIC_IP_4_P_128_BIT 0
#define RV_PLIC_IP_4_P_129_BIT 1
#define RV_PLIC_IP_4_P_130_BIT 2
#define RV_PLIC_IP_4_P_131_BIT 3
#define RV_PLIC_IP_4_P_132_BIT 4
#define RV_PLIC_IP_4_P_133_BIT 5
#define RV_PLIC_IP_4_P_134_BIT 6
#define RV_PLIC_IP_4_P_135_BIT 7
#define RV_PLIC_IP_4_P_136_BIT 8
#define RV_PLIC_IP_4_P_137_BIT 9
#define RV_PLIC_IP_4_P_138_BIT 10
#define RV_PLIC_IP_4_P_139_BIT 11
#define RV_PLIC_IP_4_P_140_BIT 12
#define RV_PLIC_IP_4_P_141_BIT 13
#define RV_PLIC_IP_4_P_142_BIT 14
#define RV_PLIC_IP_4_P_143_BIT 15
#define RV_PLIC_IP_4_P_144_BIT 16
#define RV_PLIC_IP_4_P_145_BIT 17
#define RV_PLIC_IP_4_P_146_BIT 18
#define RV_PLIC_IP_4_P_147_BIT 19
#define RV_PLIC_IP_4_P_148_BIT 20
#define RV_PLIC_IP_4_P_149_BIT 21
#define RV_PLIC_IP_4_P_150_BIT 22
#define RV_PLIC_IP_4_P_151_BIT 23
#define RV_PLIC_IP_4_P_152_BIT 24
#define RV_PLIC_IP_4_P_153_BIT 25
#define RV_PLIC_IP_4_P_154_BIT 26
#define RV_PLIC_IP_4_P_155_BIT 27
#define RV_PLIC_IP_4_P_156_BIT 28
#define RV_PLIC_IP_4_P_157_BIT 29
#define RV_PLIC_IP_4_P_158_BIT 30
#define RV_PLIC_IP_4_P_159_BIT 31

// Interrupt Pending
#define RV_PLIC_IP_5_REG_OFFSET 0x1014
#define RV_PLIC_IP_5_REG_RESVAL 0x0u
#define RV_PLIC_IP_5_P_160_BIT 0
#define RV_PLIC_IP_5_P_161_BIT 1
#define RV_PLIC_IP_5_P_162_BIT 2
#define RV_PLIC_IP_5_P_163_BIT 3
#define RV_PLIC_IP_5_P_164_BIT 4
#define RV_PLIC_IP_5_P_165_BIT 5
#define RV_PLIC_IP_5_P_166_BIT 6
#define RV_PLIC_IP_5_P_167_BIT 7
#define RV_PLIC_IP_5_P_168_BIT 8
#define RV_PLIC_IP_5_P_169_BIT 9
#define RV_PLIC_IP_5_P_170_BIT 10
#define RV_PLIC_IP_5_P_171_BIT 11
#define RV_PLIC_IP_5_P_172_BIT 12
#define RV_PLIC_IP_5_P_173_BIT 13
#define RV_PLIC_IP_5_P_174_BIT 14
#define RV_PLIC_IP_5_P_175_BIT 15
#define RV_PLIC_IP_5_P_176_BIT 16
#define RV_PLIC_IP_5_P_177_BIT 17
#define RV_PLIC_IP_5_P_178_BIT 18
#define RV_PLIC_IP_5_P_179_BIT 19
#define RV_PLIC_IP_5_P_180_BIT 20
#define RV_PLIC_IP_5_P_181_BIT 21
#define RV_PLIC_IP_5_P_182_BIT 22
#define RV_PLIC_IP_5_P_183_BIT 23
#define RV_PLIC_IP_5_P_184_BIT 24
#define RV_PLIC_IP_5_P_185_BIT 25

// Interrupt Enable for Target 0 (common parameters)
#define RV_PLIC_IE0_E_FIELD_WIDTH 1
#define RV_PLIC_IE0_MULTIREG_COUNT 6

// Interrupt Enable for Target 0
#define RV_PLIC_IE0_0_REG_OFFSET 0x2000
#define RV_PLIC_IE0_0_REG_RESVAL 0x0u
#define RV_PLIC_IE0_0_E_0_BIT 0
#define RV_PLIC_IE0_0_E_1_BIT 1
#define RV_PLIC_IE0_0_E_2_BIT 2
#define RV_PLIC_IE0_0_E_3_BIT 3
#define RV_PLIC_IE0_0_E_4_BIT 4
#define RV_PLIC_IE0_0_E_5_BIT 5
#define RV_PLIC_IE0_0_E_6_BIT 6
#define RV_PLIC_IE0_0_E_7_BIT 7
#define RV_PLIC_IE0_0_E_8_BIT 8
#define RV_PLIC_IE0_0_E_9_BIT 9
#define RV_PLIC_IE0_0_E_10_BIT 10
#define RV_PLIC_IE0_0_E_11_BIT 11
#define RV_PLIC_IE0_0_E_12_BIT 12
#define RV_PLIC_IE0_0_E_13_BIT 13
#define RV_PLIC_IE0_0_E_14_BIT 14
#define RV_PLIC_IE0_0_E_15_BIT 15
#define RV_PLIC_IE0_0_E_16_BIT 16
#define RV_PLIC_IE0_0_E_17_BIT 17
#define RV_PLIC_IE0_0_E_18_BIT 18
#define RV_PLIC_IE0_0_E_19_BIT 19
#define RV_PLIC_IE0_0_E_20_BIT 20
#define RV_PLIC_IE0_0_E_21_BIT 21
#define RV_PLIC_IE0_0_E_22_BIT 22
#define RV_PLIC_IE0_0_E_23_BIT 23
#define RV_PLIC_IE0_0_E_24_BIT 24
#define RV_PLIC_IE0_0_E_25_BIT 25
#define RV_PLIC_IE0_0_E_26_BIT 26
#define RV_PLIC_IE0_0_E_27_BIT 27
#define RV_PLIC_IE0_0_E_28_BIT 28
#define RV_PLIC_IE0_0_E_29_BIT 29
#define RV_PLIC_IE0_0_E_30_BIT 30
#define RV_PLIC_IE0_0_E_31_BIT 31

// Interrupt Enable for Target 0
#define RV_PLIC_IE0_1_REG_OFFSET 0x2004
#define RV_PLIC_IE0_1_REG_RESVAL 0x0u
#define RV_PLIC_IE0_1_E_32_BIT 0
#define RV_PLIC_IE0_1_E_33_BIT 1
#define RV_PLIC_IE0_1_E_34_BIT 2
#define RV_PLIC_IE0_1_E_35_BIT 3
#define RV_PLIC_IE0_1_E_36_BIT 4
#define RV_PLIC_IE0_1_E_37_BIT 5
#define RV_PLIC_IE0_1_E_38_BIT 6
#define RV_PLIC_IE0_1_E_39_BIT 7
#define RV_PLIC_IE0_1_E_40_BIT 8
#define RV_PLIC_IE0_1_E_41_BIT 9
#define RV_PLIC_IE0_1_E_42_BIT 10
#define RV_PLIC_IE0_1_E_43_BIT 11
#define RV_PLIC_IE0_1_E_44_BIT 12
#define RV_PLIC_IE0_1_E_45_BIT 13
#define RV_PLIC_IE0_1_E_46_BIT 14
#define RV_PLIC_IE0_1_E_47_BIT 15
#define RV_PLIC_IE0_1_E_48_BIT 16
#define RV_PLIC_IE0_1_E_49_BIT 17
#define RV_PLIC_IE0_1_E_50_BIT 18
#define RV_PLIC_IE0_1_E_51_BIT 19
#define RV_PLIC_IE0_1_E_52_BIT 20
#define RV_PLIC_IE0_1_E_53_BIT 21
#define RV_PLIC_IE0_1_E_54_BIT 22
#define RV_PLIC_IE0_1_E_55_BIT 23
#define RV_PLIC_IE0_1_E_56_BIT 24
#define RV_PLIC_IE0_1_E_57_BIT 25
#define RV_PLIC_IE0_1_E_58_BIT 26
#define RV_PLIC_IE0_1_E_59_BIT 27
#define RV_PLIC_IE0_1_E_60_BIT 28
#define RV_PLIC_IE0_1_E_61_BIT 29
#define RV_PLIC_IE0_1_E_62_BIT 30
#define RV_PLIC_IE0_1_E_63_BIT 31

// Interrupt Enable for Target 0
#define RV_PLIC_IE0_2_REG_OFFSET 0x2008
#define RV_PLIC_IE0_2_REG_RESVAL 0x0u
#define RV_PLIC_IE0_2_E_64_BIT 0
#define RV_PLIC_IE0_2_E_65_BIT 1
#define RV_PLIC_IE0_2_E_66_BIT 2
#define RV_PLIC_IE0_2_E_67_BIT 3
#define RV_PLIC_IE0_2_E_68_BIT 4
#define RV_PLIC_IE0_2_E_69_BIT 5
#define RV_PLIC_IE0_2_E_70_BIT 6
#define RV_PLIC_IE0_2_E_71_BIT 7
#define RV_PLIC_IE0_2_E_72_BIT 8
#define RV_PLIC_IE0_2_E_73_BIT 9
#define RV_PLIC_IE0_2_E_74_BIT 10
#define RV_PLIC_IE0_2_E_75_BIT 11
#define RV_PLIC_IE0_2_E_76_BIT 12
#define RV_PLIC_IE0_2_E_77_BIT 13
#define RV_PLIC_IE0_2_E_78_BIT 14
#define RV_PLIC_IE0_2_E_79_BIT 15
#define RV_PLIC_IE0_2_E_80_BIT 16
#define RV_PLIC_IE0_2_E_81_BIT 17
#define RV_PLIC_IE0_2_E_82_BIT 18
#define RV_PLIC_IE0_2_E_83_BIT 19
#define RV_PLIC_IE0_2_E_84_BIT 20
#define RV_PLIC_IE0_2_E_85_BIT 21
#define RV_PLIC_IE0_2_E_86_BIT 22
#define RV_PLIC_IE0_2_E_87_BIT 23
#define RV_PLIC_IE0_2_E_88_BIT 24
#define RV_PLIC_IE0_2_E_89_BIT 25
#define RV_PLIC_IE0_2_E_90_BIT 26
#define RV_PLIC_IE0_2_E_91_BIT 27
#define RV_PLIC_IE0_2_E_92_BIT 28
#define RV_PLIC_IE0_2_E_93_BIT 29
#define RV_PLIC_IE0_2_E_94_BIT 30
#define RV_PLIC_IE0_2_E_95_BIT 31

// Interrupt Enable for Target 0
#define RV_PLIC_IE0_3_REG_OFFSET 0x200c
#define RV_PLIC_IE0_3_REG_RESVAL 0x0u
#define RV_PLIC_IE0_3_E_96_BIT 0
#define RV_PLIC_IE0_3_E_97_BIT 1
#define RV_PLIC_IE0_3_E_98_BIT 2
#define RV_PLIC_IE0_3_E_99_BIT 3
#define RV_PLIC_IE0_3_E_100_BIT 4
#define RV_PLIC_IE0_3_E_101_BIT 5
#define RV_PLIC_IE0_3_E_102_BIT 6
#define RV_PLIC_IE0_3_E_103_BIT 7
#define RV_PLIC_IE0_3_E_104_BIT 8
#define RV_PLIC_IE0_3_E_105_BIT 9
#define RV_PLIC_IE0_3_E_106_BIT 10
#define RV_PLIC_IE0_3_E_107_BIT 11
#define RV_PLIC_IE0_3_E_108_BIT 12
#define RV_PLIC_IE0_3_E_109_BIT 13
#define RV_PLIC_IE0_3_E_110_BIT 14
#define RV_PLIC_IE0_3_E_111_BIT 15
#define RV_PLIC_IE0_3_E_112_BIT 16
#define RV_PLIC_IE0_3_E_113_BIT 17
#define RV_PLIC_IE0_3_E_114_BIT 18
#define RV_PLIC_IE0_3_E_115_BIT 19
#define RV_PLIC_IE0_3_E_116_BIT 20
#define RV_PLIC_IE0_3_E_117_BIT 21
#define RV_PLIC_IE0_3_E_118_BIT 22
#define RV_PLIC_IE0_3_E_119_BIT 23
#define RV_PLIC_IE0_3_E_120_BIT 24
#define RV_PLIC_IE0_3_E_121_BIT 25
#define RV_PLIC_IE0_3_E_122_BIT 26
#define RV_PLIC_IE0_3_E_123_BIT 27
#define RV_PLIC_IE0_3_E_124_BIT 28
#define RV_PLIC_IE0_3_E_125_BIT 29
#define RV_PLIC_IE0_3_E_126_BIT 30
#define RV_PLIC_IE0_3_E_127_BIT 31

// Interrupt Enable for Target 0
#define RV_PLIC_IE0_4_REG_OFFSET 0x2010
#define RV_PLIC_IE0_4_REG_RESVAL 0x0u
#define RV_PLIC_IE0_4_E_128_BIT 0
#define RV_PLIC_IE0_4_E_129_BIT 1
#define RV_PLIC_IE0_4_E_130_BIT 2
#define RV_PLIC_IE0_4_E_131_BIT 3
#define RV_PLIC_IE0_4_E_132_BIT 4
#define RV_PLIC_IE0_4_E_133_BIT 5
#define RV_PLIC_IE0_4_E_134_BIT 6
#define RV_PLIC_IE0_4_E_135_BIT 7
#define RV_PLIC_IE0_4_E_136_BIT 8
#define RV_PLIC_IE0_4_E_137_BIT 9
#define RV_PLIC_IE0_4_E_138_BIT 10
#define RV_PLIC_IE0_4_E_139_BIT 11
#define RV_PLIC_IE0_4_E_140_BIT 12
#define RV_PLIC_IE0_4_E_141_BIT 13
#define RV_PLIC_IE0_4_E_142_BIT 14
#define RV_PLIC_IE0_4_E_143_BIT 15
#define RV_PLIC_IE0_4_E_144_BIT 16
#define RV_PLIC_IE0_4_E_145_BIT 17
#define RV_PLIC_IE0_4_E_146_BIT 18
#define RV_PLIC_IE0_4_E_147_BIT 19
#define RV_PLIC_IE0_4_E_148_BIT 20
#define RV_PLIC_IE0_4_E_149_BIT 21
#define RV_PLIC_IE0_4_E_150_BIT 22
#define RV_PLIC_IE0_4_E_151_BIT 23
#define RV_PLIC_IE0_4_E_152_BIT 24
#define RV_PLIC_IE0_4_E_153_BIT 25
#define RV_PLIC_IE0_4_E_154_BIT 26
#define RV_PLIC_IE0_4_E_155_BIT 27
#define RV_PLIC_IE0_4_E_156_BIT 28
#define RV_PLIC_IE0_4_E_157_BIT 29
#define RV_PLIC_IE0_4_E_158_BIT 30
#define RV_PLIC_IE0_4_E_159_BIT 31

// Interrupt Enable for Target 0
#define RV_PLIC_IE0_5_REG_OFFSET 0x2014
#define RV_PLIC_IE0_5_REG_RESVAL 0x0u
#define RV_PLIC_IE0_5_E_160_BIT 0
#define RV_PLIC_IE0_5_E_161_BIT 1
#define RV_PLIC_IE0_5_E_162_BIT 2
#define RV_PLIC_IE0_5_E_163_BIT 3
#define RV_PLIC_IE0_5_E_164_BIT 4
#define RV_PLIC_IE0_5_E_165_BIT 5
#define RV_PLIC_IE0_5_E_166_BIT 6
#define RV_PLIC_IE0_5_E_167_BIT 7
#define RV_PLIC_IE0_5_E_168_BIT 8
#define RV_PLIC_IE0_5_E_169_BIT 9
#define RV_PLIC_IE0_5_E_170_BIT 10
#define RV_PLIC_IE0_5_E_171_BIT 11
#define RV_PLIC_IE0_5_E_172_BIT 12
#define RV_PLIC_IE0_5_E_173_BIT 13
#define RV_PLIC_IE0_5_E_174_BIT 14
#define RV_PLIC_IE0_5_E_175_BIT 15
#define RV_PLIC_IE0_5_E_176_BIT 16
#define RV_PLIC_IE0_5_E_177_BIT 17
#define RV_PLIC_IE0_5_E_178_BIT 18
#define RV_PLIC_IE0_5_E_179_BIT 19
#define RV_PLIC_IE0_5_E_180_BIT 20
#define RV_PLIC_IE0_5_E_181_BIT 21
#define RV_PLIC_IE0_5_E_182_BIT 22
#define RV_PLIC_IE0_5_E_183_BIT 23
#define RV_PLIC_IE0_5_E_184_BIT 24
#define RV_PLIC_IE0_5_E_185_BIT 25

// Threshold of priority for Target 0
#define RV_PLIC_THRESHOLD0_REG_OFFSET 0x200000
#define RV_PLIC_THRESHOLD0_REG_RESVAL 0x0u
#define RV_PLIC_THRESHOLD0_THRESHOLD0_MASK 0x3u
#define RV_PLIC_THRESHOLD0_THRESHOLD0_OFFSET 0
#define RV_PLIC_THRESHOLD0_THRESHOLD0_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_THRESHOLD0_THRESHOLD0_MASK, .index = RV_PLIC_THRESHOLD0_THRESHOLD0_OFFSET })

// Claim interrupt by read, complete interrupt by write for Target 0.
#define RV_PLIC_CC0_REG_OFFSET 0x200004
#define RV_PLIC_CC0_REG_RESVAL 0x0u
#define RV_PLIC_CC0_CC0_MASK 0xffu
#define RV_PLIC_CC0_CC0_OFFSET 0
#define RV_PLIC_CC0_CC0_FIELD \
  ((bitfield_field32_t) { .mask = RV_PLIC_CC0_CC0_MASK, .index = RV_PLIC_CC0_CC0_OFFSET })

// msip for Hart 0.
#define RV_PLIC_MSIP0_REG_OFFSET 0x4000000
#define RV_PLIC_MSIP0_REG_RESVAL 0x0u
#define RV_PLIC_MSIP0_MSIP0_BIT 0

// Alert Test Register.
#define RV_PLIC_ALERT_TEST_REG_OFFSET 0x4004000
#define RV_PLIC_ALERT_TEST_REG_RESVAL 0x0u
#define RV_PLIC_ALERT_TEST_FATAL_FAULT_BIT 0

#ifdef __cplusplus
}  // extern "C"
#endif
#endif  // _RV_PLIC_REG_DEFS_
// End generated register defines for rv_plic