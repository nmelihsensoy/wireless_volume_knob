#include <VirtualWire.h>
#include "HID-Project.h"

char stat;

void setup(){
  pinMode(1, OUTPUT);
  Consumer.begin();
  System.begin();
  Mouse.begin();
  vw_set_rx_pin(11);
  vw_setup(2000);
  vw_rx_start();
}

void loop(){
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;
    if (vw_get_message(buf, &buflen)){
      stat = buf[0];
    }
    
    if(stat == 'P'){
      Consumer.write(MEDIA_PLAY_PAUSE);
      stat = 'S';
    }else if(stat == 'U'){
      Consumer.write(MEDIA_VOLUME_UP);
      stat = 'S';
    }else if(stat == 'D'){
      Consumer.write(MEDIA_VOLUME_DOWN);
      stat = 'S';
    }else if(stat == 'N'){
      Consumer.write(MEDIA_NEXT);
      stat = 'S';
    }else if(stat == 'B'){
      Consumer.write(MEDIA_PREVIOUS);
      stat = 'S';
    }else if(stat == 'M'){
      Consumer.write(MEDIA_VOLUME_MUTE);
      stat = 'S';
    }
}
