#include "timer.h"
#include <Arduino.h>\

static int lightState = LOW;
static int pumpState = LOW;
static int pumpCounter = 0;
int pumpCounterEnd = 20; // Change this to alter on/off duty cycle (20 = 5% on)

void IRAM_ATTR timer_group0_isr(void *param) {
    uint32_t int_st_timers = TIMERG0.int_st_timers.val; // Check which timer interrupt occured
    if (int_st_timers & TIMG_T0_INT_ST) {
        timer_group_clr_intr_status_in_isr(TIMER_GROUP_0, TIMER_0); // Clear interrupt
        digitalWrite(LIGHTING_PIN, (lightState = !lightState));
        //Serial.println("Lighting task executed");
        timer_group_enable_alarm_in_isr(TIMER_GROUP_0, TIMER_0);
    }
    if (int_st_timers & TIMG_T1_INT_ST) {
        timer_group_clr_intr_status_in_isr(TIMER_GROUP_0, TIMER_1); // Clear interrupt
        if (pumpCounter == pumpCounterEnd){
            digitalWrite(PUMP_PIN, (pumpState = !pumpState));
            //Serial.println("Pump task executed");
            pumpCounter = 0;
        }else if(pumpState == HIGH){
            digitalWrite(PUMP_PIN, (pumpState = !pumpState));
        }else{pumpCounter += 1;}
        timer_group_enable_alarm_in_isr(TIMER_GROUP_0, TIMER_1);
    }
}

void setup_timer(timer_idx_t timer_num, int seconds) {
    timer_config_t config = {
        .alarm_en = TIMER_ALARM_EN,
        .counter_en = TIMER_PAUSE,
        .intr_type = TIMER_INTR_LEVEL,
        .counter_dir = TIMER_COUNT_UP,
        .auto_reload = TIMER_AUTORELOAD_EN,
        .divider = 80 // 80 MHz / 80 = 1 MHz, i.e. 1 tick per microsecond
    };

    timer_init(TIMER_GROUP_0, timer_num, &config);
    
    // Set alarm value for 4 hours (14400000 milliseconds) = 14400000000ULL
    // 15 seconds = 15000000ULL
    timer_set_alarm_value(TIMER_GROUP_0, timer_num, seconds*1000000ULL);
    
    timer_enable_intr(TIMER_GROUP_0, timer_num);
    timer_isr_register(TIMER_GROUP_0, timer_num, timer_group0_isr, NULL, ESP_INTR_FLAG_IRAM, NULL);

    timer_start(TIMER_GROUP_0, timer_num);
}

void updateTimer(int minutes) {
  // Ensure minutes > 0 && minutes < 1440
  // Assuming your timer counts in microseconds
  uint64_t alarmValue = (uint64_t)minutes * 60 * 1000000;
  timer_pause(TIMER_GROUP_0, TIMER_0);
  timer_disable_intr(TIMER_GROUP_0, TIMER_0);
  timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, alarmValue);
  timer_enable_intr(TIMER_GROUP_0, TIMER_0);
  timer_start(TIMER_GROUP_0, TIMER_0);
}