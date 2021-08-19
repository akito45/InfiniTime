#pragma once

#include <cstdint>
#include "app_timer.h"
#include "components/settings/Settings.h"

namespace Pinetime {
  namespace Controllers {
    static constexpr uint8_t pinMotor = 16;
    
    
    class MotorController {
    

    public:
      enum TuneType : uint8_t {
        NOTIFICATION,
        SHORT,
        RING,
        STOP
      };

      MotorController(Controllers::Settings& settingsController);
      void Init();
      void SetDuration(uint8_t motorDuration);
      void VibrateTune(TuneType tune);

    private:
      struct Tune { 
        uint8_t tune;
        uint8_t length;
        uint8_t tempo;
      };
      
      static TuneType runningTune;
      static uint8_t step;

      static constexpr Tune tunes[] =  {
        [TuneType::NOTIFICATION] = {.tune = 0x29, .length = 6, .tempo = 50},
        [TuneType::SHORT] = {.tune = 0x01, .length = 1, .tempo = 35},
        [TuneType::RING] = {.tune = 0x01, .length = 2, .tempo = 100},
        [TuneType::STOP] = {.tune = 0x00, .length = 0, .tempo = 0},
      };

      Controllers::Settings& settingsController;
      static void vibrate(void* p_context);
    
    
    };
  }
}
