/*
 * CONTROL_REG (32-bit):
 *  ┌────────────┬──────────────┬────────────────────────────┐
 *  │  Bits 0–7  │   Bits 8–15  │          Bits 16–31        │
 *  │  STATUS    │   CONTROL    │       ARGUMENTS (OPTIONAL) │
 *  └────────────┴──────────────┴────────────────────────────┘
 *
 * Layout Description:
 *
 * - STATUS FLAGS (Bits 0–7):
 *       00 = idle
 *       01 = busy
 *       10 = ready
 *       11 = error/reserved
 *
 * - CONTROL FLAGS (Bits 8–15):
 *       00 = no action
 *       01 = trigger read
 *       10+ = reserved
 *
 * - ARGUMENT FIELDS (Bits 16–31):
 *     Optional 4-bit arguments associated with specific control operations.
 *       Bits 24–27: Rain hour interval
 *
 * Bit Offsets per Sensor:
 *
 *     WIND_SPEED:
 *       STATUS  → bits 0–1
 *       CONTROL → bits 8–9
 *       ARG     → bits 16–19
 *
 *     WIND_DIR:
 *       STATUS  → bits 2–3
 *       CONTROL → bits 10–11
 *       ARG     → bits 20–23
 *
 *     RAIN_AVG:
 *       STATUS  → bits 4–5
 *       CONTROL → bits 12–13
 *       ARG     → bits 24–27
 *
 */

// Status Bits
#define STAT_SHIFT_WIND   0
#define STAT_SHIFT_DIR    2
#define STAT_SHIFT_RAIN   4
#define STAT_SHIFT_WRITE  6

// Control Bits
#define CTRL_SHIFT_WIND   8
#define CTRL_SHIFT_DIR    10
#define CTRL_SHIFT_RAIN   12
#define CTRL_SHIFT_ALL    14

// Arg Bits
#define ARG_SHIFT_WIND    16
#define ARG_SHIFT_DIR     20
#define ARG_SHIFT_RAIN    24

#define STATUS_MASK       0x000000FF
#define CONTROL_MASK      0x0000FF00
#define ARG_MASK          0xFFFF0000

#define READY      0b10
#define PENDING    0b01
#define IDLE       0b00
#define REQUEST    0b01
#define CANCEL     0b10
