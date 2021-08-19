#include "MotorController.h"
#include <hal/nrf_gpio.h>
#include "systemtask/SystemTask.h"
#include "app_timer.h"

APP_TIMER_DEF(vibTimer);

using namespace Pinetime::Controllers;

constexpr MotorController::Tune MotorController::tunes[];


MotorController::MotorController(Controllers::Settings& settingsController) : settingsController {settingsController} {
}



uint8_t MotorController::step = 0;
MotorController::TuneType MotorController::runningTune = MotorController::TuneType::STOP; 



void MotorController::Init() {
  nrf_gpio_cfg_output(pinMotor);
  nrf_gpio_pin_set(pinMotor);
  app_timer_create(&vibTimer, APP_TIMER_MODE_SINGLE_SHOT, vibrate);
}


void MotorController::VibrateTune(TuneType tune) {
  nrf_gpio_pin_set(pinMotor);
  if (settingsController.GetVibrationStatus() == Controllers::Settings::Vibration::OFF)
    return;
  step = 0;
  runningTune = tune;
  vibrate(NULL);
}

void MotorController::SetDuration(uint8_t motorDuration) {
  nrf_gpio_pin_set(pinMotor);
  if (settingsController.GetVibrationStatus() == Controllers::Settings::Vibration::OFF)
    return;
  step = 0;
  runningTune = TuneType::STOP;
  nrf_gpio_pin_clear(pinMotor);
  app_timer_start(vibTimer, APP_TIMER_TICKS(motorDuration), NULL);
}

void MotorController::vibrate(void* p_context) {
  
  if (step >= tunes[runningTune].length || step >= 8) { //end of tune turn off vibration
    nrf_gpio_pin_set(pinMotor);
    return; 
  }  
 
  if (((1 << step) & tunes[runningTune].tune) > 0) {
    nrf_gpio_pin_clear(pinMotor);
  } else {
    nrf_gpio_pin_set(pinMotor);
  }

  ++step;     
  /* Start timer for the next cycle */
  app_timer_start(vibTimer, APP_TIMER_TICKS(tunes[runningTune].tempo), NULL);
}

