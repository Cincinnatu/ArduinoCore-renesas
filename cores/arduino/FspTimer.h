#include "Arduino.h"

#ifndef ARDUINO_FSP_TIMER_H
#define ARDUINO_FSP_TIMER_H


#define FREQUENCY_0_PCLKD     (48000000)
#define FREQUENCY_1_PCLKD     (FREQUENCY_0_PCLKD / 4)
#define FREQUENCY_2_PCLKD     (FREQUENCY_1_PCLKD / 4)
#define FREQUENCY_3_PCLKD     (FREQUENCY_2_PCLKD / 4)
#define FREQUENCY_4_PCLKD     (FREQUENCY_3_PCLKD / 4)
#define FREQUENCY_5_PCLKD     (FREQUENCY_4_PCLKD / 4)

#define TICK_0              (float)(1.0/(float)FREQUENCY_0_PCLKD)
#define TICK_1              (float)(1.0/(float)FREQUENCY_1_PCLKD)
#define TICK_2              (float)(1.0/(float)FREQUENCY_2_PCLKD)
#define TICK_3              (float)(1.0/(float)FREQUENCY_3_PCLKD)
#define TICK_4              (float)(1.0/(float)FREQUENCY_4_PCLKD)
#define TICK_5              (float)(1.0/(float)FREQUENCY_5_PCLKD)

#define CH32BIT_MAX         (4294967295)
#define CH16BIT_MAX         (65535)

#define GPT_TIMER           (0)
#define AGT_TIMER           (1)

#define TIMER_DIVIDER_AUTO       (-1)
#define TIMER_DIVEDER_4          (2)
#define TIMER_DIVIDER_16         (4)
#define TIMER_DIVIDER_64         (6)
#define TIMER_DIVIDER_256        (8)
#define TIMER_DIVIDER_1024       (10)


#define STANDARD_PWM_FREQ_HZ       (490.0)
#define STANDARD_DUTY_CYCLE_PERC   (50.0)

typedef enum {
  CHANNEL_A,
  CHANNEL_B
} TimerPWMChannel_t;


class GPTimer {
  public:
    GPTimer(timer_cfg_t &cfg) {
      ext_cfg.gtioca.output_enabled        = false;
      ext_cfg.gtioca.stop_level            = GPT_PIN_LEVEL_LOW;
      ext_cfg.gtiocb.output_enabled        = false;
      ext_cfg.gtiocb.stop_level            = GPT_PIN_LEVEL_LOW;
      ext_cfg.start_source                 = (gpt_source_t) (GPT_SOURCE_NONE); 
      ext_cfg.stop_source                  = (gpt_source_t) (GPT_SOURCE_NONE);
      ext_cfg.clear_source                 = (gpt_source_t) (GPT_SOURCE_NONE);
      ext_cfg.count_up_source              = (gpt_source_t) (GPT_SOURCE_NONE); 
      ext_cfg.count_down_source            = (gpt_source_t) (GPT_SOURCE_NONE); 
      ext_cfg.capture_a_source             = (gpt_source_t) (GPT_SOURCE_NONE);
      ext_cfg.capture_b_source             = (gpt_source_t) (GPT_SOURCE_NONE); 
      ext_cfg.capture_a_ipl                = (BSP_IRQ_DISABLED);
      ext_cfg.capture_b_ipl                = (BSP_IRQ_DISABLED);
      ext_cfg.capture_a_irq                = FSP_INVALID_VECTOR;
      ext_cfg.capture_b_irq                = FSP_INVALID_VECTOR;
      ext_cfg.capture_filter_gtioca        = GPT_CAPTURE_FILTER_NONE;
      ext_cfg.capture_filter_gtiocb        = GPT_CAPTURE_FILTER_NONE;
      ext_cfg.p_pwm_cfg                    = NULL;
      ext_cfg.gtior_setting.gtior          = 0U;

      cfg.p_extend                         = &ext_cfg;
       
    }
    
    gpt_instance_ctrl_t ctrl;
    gpt_extended_cfg_t ext_cfg;
    

};

class AGTimer {
  public:
  AGTimer(timer_cfg_t &cfg) {
      ext_cfg.count_source                  = AGT_CLOCK_PCLKB;
	    ext_cfg.agto                          = AGT_PIN_CFG_DISABLED;
	    ext_cfg.agtoab_settings_b.agtoa       = AGT_PIN_CFG_DISABLED;
	    ext_cfg.agtoab_settings_b.agtob       = AGT_PIN_CFG_DISABLED;
	    ext_cfg.measurement_mode              = AGT_MEASURE_DISABLED;
	    ext_cfg.agtio_filter                  = AGT_AGTIO_FILTER_NONE;
	    ext_cfg.enable_pin                    = AGT_ENABLE_PIN_NOT_USED;
	    ext_cfg.trigger_edge                  = AGT_TRIGGER_EDGE_RISING;

      cfg.p_extend                         = &ext_cfg;
  }
  agt_extended_cfg_t ext_cfg;
  agt_instance_ctrl_t ctrl;

};



class FspTimer {
  private:
    AGTimer *agt_timer;
    GPTimer *gpt_timer;
    timer_cfg_t timer_cfg;
    uint32_t _period_counts;
    uint32_t _duty_cicle_counts;
    timer_source_div_t _sd;
    
    
    uint8_t type;
    bool init_ok;
    void set_period_counts(float period, uint32_t max);
  public:
    FspTimer();
    ~FspTimer();
    
    bool open();
    bool start();
    bool stop();
    bool set_duty_cycle(uint32_t const duty_cycle_counts, TimerPWMChannel_t pwm_ch);
    bool set_period(uint32_t p);
    bool close();
    void enable_pwm_channel(TimerPWMChannel_t pwm_channel);
    uint32_t get_counter();
    uint32_t get_freq_hz();

    timer_cfg_t *get_cfg() { return &timer_cfg; }
    bool begin_pwm(uint8_t type, uint8_t channel, gpt_extended_pwm_cfg_t *pwm_cfg, TimerPWMChannel_t pwm_channel);
    
    bool begin(timer_mode_t mode, uint8_t type, uint8_t channel,  float freq_hz, float duty_perc, void(*callback)(timer_callback_args_t *) = nullptr  );
    bool begin(timer_mode_t mode, uint8_t type, uint8_t channel,  uint32_t period, uint32_t pulse, timer_source_div_t sd, void(*p_callback)(timer_callback_args_t *) = nullptr );
    
    
    
    uint32_t get_period_raw();
    void set_pwm_extended_cfg(gpt_extended_pwm_cfg_t *cfg);
    bool set_period_ms(double ms);
    bool set_period_us(double us);
    bool set_pulse_ms(double ms,TimerPWMChannel_t pwm_ch);
    bool set_pulse_us(double us,TimerPWMChannel_t pwm_ch);
};


#endif