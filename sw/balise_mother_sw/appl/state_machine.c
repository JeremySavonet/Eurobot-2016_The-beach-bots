#include <stdlib.h>
#include <string.h>

#include "ADXL362.h"
#include "BC118.h"
#include "console.h"
#include "global_variables.h"
#include "state_machine.h"
#include "system.h"
#include "tests.h"
#include "zigbee.h"

//=== CONSTANTS ===
#define PAIRING_TIME1  30000
#define PAIRING_TIME2  20000
#define UPDATE_TIME    60000
#define RET2PAIR_TIME  1000
#define ZB_REQ_PAIRING_TIME 500

u8 update_unlock(void);
u8 pairing_unlock(void);
void test_state_machine(void);

u8 update_unlock(void)
{
    static update_state_t update_state = IDLE;
    static u8 encoder_val_prev = 0;
    static s16 sum_delta = 0;
    u8 encoder_val = g_encoder_cnt;     // Store global variable for atomicity

    //DPRINT(2,"\n\r State = :%d, %d, %d", update_state,sum_delta,encoder_val);

    switch (update_state) {
        case IDLE:
            if (adxl362_reversed()) {
                update_state = DOWN;
                g_gesture_cnt = 1000;
            }
            break;

        case DOWN:
            if (!adxl362_reversed()) {
                update_state = IDLE;
            } else if (g_gesture_cnt == 0) {
                update_state = ENCODER1;
                encoder_val_prev = encoder_val;
            }
            break;

        case ENCODER1:
            //DPRINT(2,"State = : %d, %d, %d, %d", sum_delta,encoder_val,encoder_val_prev,(encoder_val - encoder_val_prev)%16);
            sum_delta += ((encoder_val - encoder_val_prev) % 16);
            encoder_val_prev = encoder_val;

            if (abs(sum_delta) >= 128) {
                update_state = UPDATE_UNLOCK;
                g_state_cnt = PAIRING_TIME1;
            }
            break;

        case UPDATE_UNLOCK:
            break;

        default:
            break;
    }

    // Cancel
    if (!adxl362_reversed()) {
        update_state = IDLE;
    }

    return (update_state == UPDATE_UNLOCK);
}

u8 pairing_unlock(void)
{
    static u8 pairing_state = 0;

    //DPRINT(2,"Reverse remote2 : %d %i", g_gesture_cnt,g_adxl_z);
    if (adxl362_reversed()) {
        if (pairing_state == 0) {
            g_gesture_cnt = RET2PAIR_TIME;
            pairing_state = 1;
        }
    } else {
        pairing_state = 0;
    }

    if (pairing_state == 1 && g_gesture_cnt == 0) {
        pairing_state = 2;
        return 1;
    } else {
        return 0;
    }
}

void state_machine(void)
{
    static u8 first_boot = 1;
    static state_t prev_state = POWER_UP;
    static u8 first_pairing_proc = 1;
    static u8 rssi_cnt = 0;

    switch (g_state) {
        case POWER_UP:
            g_led_state = LED_POWER_UP;
            g_state = PAIRING_CONF;
            g_state_cnt = PAIRING_TIME1;
            break;

        case PAIRING_CONF:
            g_rcv_test_ack_flag = 0;
            g_rcv_info_req_flag = 0;
            g_sleep_cnt = 32 * SLEEP_CREDIT;
            //Power up BT and configure
            DPRINT(2, "BT power up");
            bc118_power_on();
            rssi_cnt = 0;
            g_bc118_rssi = BC118_RSSI_TH_MIN;
            g_bc118_best_rssi = -120;
            if (first_boot) {
                g_led_state = LED_POWER_UP;
                init_bc118();
                bc118_get_melody_version();
                //bc118_enable_disable_scn( 1 );
            } else {
                g_led_state = LED_CONF;
                bc118_wake_from_sleep();
                delay_ms(100);
                if (g_bc118_status == CON || g_bc118_status == CNG) {
                    bc118_close_connection();
                    delay_ms(20);
                }
                bc118_enable_disable_scn(1);
            }
            //Power up ZB
            if (g_zb_enabled) {
                DPRINT(2, "ZB power up");
                zb_power_on();
            }
            DPRINT(2, "RF conf done");
            if (prev_state == PAIRING_IDLE)
                g_state_cnt = PAIRING_TIME2;
            else
                g_state_cnt = PAIRING_TIME1;
            g_state_cnt2 = 0;
            g_sleep_cnt = 4 * SLEEP_CREDIT;
            if (first_boot)
                g_state = PAIRING_IDLE;
            else
                g_state = PAIRING_PROC;
            break;

        case PAIRING_PROC:
            g_led_state = LED_PAIRING_PROC;
            // Never sleep in this state
            g_sleep_cnt = 4 * SLEEP_CREDIT;

            if (!adxl362_reversed() && first_boot == 0 && !TST_BIT(g_test_mode,1)) {
                // Close connection for clean restart
                if (g_bc118_status == CON || g_bc118_status == CNG) {
                    delay_ms(100);
                    bc118_close_connection();
                    delay_ms(20);
                }
                g_state = prev_state;
            }
            // Check Zigbee pairing ack
            if (g_zb_enabled && zb_wait_pairing_ack()) {
                DPRINT(2, "RF Ack received");
                first_boot = 0;
                first_pairing_proc = 0;
                g_state = ZB_CONF;
                // Check BT pairing ack
            } else if (g_bt_state == PAIRING && g_rcv_con_ack_flag == 1) {
                DPRINT(2, "BT Ack received");
                first_boot = 0;
                first_pairing_proc = 0;
                g_state = BT_CONF;
                g_rcv_con_ack_flag = 0;
                strncpy(g_bt_addr,g_bt_addr_pair, sizeof(g_bt_addr) / sizeof(g_bt_addr[0]));
                g_bt_addr_type = g_bt_addr_pair_type;
            // Test mode (never close connexion)
            } else if (g_bt_state == PAIRING && g_rcv_test_ack_flag == 1) {
                DPRINT(2, "BT Test Ack received");
                first_boot = 0;
                first_pairing_proc = 0;
                g_state = BT_CONF;
                strncpy(g_bt_addr,g_bt_addr_pair, sizeof(g_bt_addr) / sizeof(g_bt_addr[0]));
                g_bt_addr_type = g_bt_addr_pair_type;
            // Send ZB and BT pairing request
            } else if (g_state_cnt2 == 0) {
                if (g_zb_enabled) {
                    zb_send_pairing_trame();
                }
                bc118_send_con_request(1);
                if (rssi_cnt > 4) { // 2 seconds
                    g_bc118_rssi = BC118_RSSI_TH_MID; // increase rssi threshold
                }
                if (rssi_cnt < 8) { // after 4 seconds
                    rssi_cnt++;
                } else {
                    g_bc118_rssi = BC118_RSSI_TH_MAX; // increase rssi threshold
                }
                g_state_cnt2 = ZB_REQ_PAIRING_TIME;
            } else {
                bc118_send_con_request(0);
            }

            if (first_pairing_proc && update_unlock()) {
                g_state = UPDATE;
            }
            if (g_state_cnt == 0) {
                g_state = PAIRING_IDLE;
            }
            break;

        case PAIRING_IDLE:
            g_led_state = LED_PAIRING_IDLE;
            DPRINT(2, "Go to pairing idle");
            first_boot = 0;
            bc118_get_status();
            if (g_bc118_status == SCN) {
                bc118_enable_disable_scn(0);
            }
            bc118_activate_sleep_mode();
            if (g_zb_enabled) {
                zb_power_off();
            }
            g_state = PAIRING_IDLE2;
            break;

        case PAIRING_IDLE2:
            //if ( adxl362_reversed() ) {
            if (pairing_unlock() || TST_BIT(g_test_mode,1)) {
                DPRINT(2, "return in conf");
                bc118_wake_from_sleep();
                g_state_cnt = PAIRING_TIME2;
                g_state = PAIRING_CONF;
                prev_state = PAIRING_IDLE;
            }
            break;

        case UPDATE:
            DPRINT(2, "Update mode configuring");
            g_led_state = LED_UPDATE;
            g_state_cnt = UPDATE_TIME;
            g_sleep_cnt = 4 * SLEEP_CREDIT;
            bc118_config_update();
            g_state = UPDATING;
            DPRINT(2, "Update mode activated");
            g_state_cnt = UPDATE_TIME;  // resest counter
            break;

        case UPDATING:
            g_sleep_cnt = 4 * SLEEP_CREDIT;
            if (g_state_cnt == 0 && g_bt_state != BC_UPDATE_CON) {
                first_boot = 1;
                g_state = PAIRING_CONF;
            }
            break;

        case ZB_CONF:
            DPRINT(2, "ZB CONF");
            bc118_enable_disable_scn(0);
            bc118_activate_sleep_mode();
            g_state = ZB_PAIRED;
            break;

        case ZB_PAIRED:
            g_led_state = LED_SEND_MODE;
            if (pairing_unlock()) {
                DPRINT(2, "PAIRING g_state from ZB_PAIRED");
                prev_state = ZB_CONF;
                g_state = PAIRING_CONF;
            }
            break;

        case BT_CONF:
            DPRINT(2, "BT CONF");
            if (g_zb_enabled) {
                zb_power_off();
            }
            g_state = BT_PAIRED;
            break;

        case BT_PAIRED:
            g_led_state = LED_SEND_MODE;
            if (pairing_unlock()) {
                DPRINT(2, "PAIRING g_state from BT_PAIRED");
                prev_state = BT_CONF;
                g_state = PAIRING_CONF;
            }
            break;

        default:
            break;
    }
}
