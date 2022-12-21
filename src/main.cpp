#define DCSBIOS_DEFAULT_SERIAL

#include <Arduino.h>
#include <DcsBios.h>
#include <icp.h>

long last_button_states = 0;
long button_states = 0;

DcsBios::PotentiometerEWMA<5, 128, 5> HudBrt("ICP_HUD_BRT_KNB", A0);
DcsBios::PotentiometerEWMA<5, 128, 5> RasterBrt("ICP_RASTER_BRT_KNB", A1);
DcsBios::PotentiometerEWMA<5, 128, 5> RetDepr("ICP_RETICLE_DEPRESS_KNB", A2);
DcsBios::PotentiometerEWMA<5, 128, 5> RasterContr("ICP_RASTER_CONTR_KNB", A3);

void load_pio() {
  digitalWrite(PIN_77HC165_SH_LD, LOW);
  delayMicroseconds(DELAY_77HC165);
  digitalWrite(PIN_77HC165_SH_LD, HIGH);
  delayMicroseconds(DELAY_77HC165);
}

void pushButtonCheck(const char* item, int bitnum) {
  if (bitRead(last_button_states, bitnum) != bitRead(button_states, bitnum)) {
    if (bitRead(button_states, bitnum)) {
      DcsBios::sendDcsBiosMessage(item, "1");
    }
    else {
      DcsBios::sendDcsBiosMessage(item, "0");
    }
  }
}

void Switch3PosCheck(const char* item, int bitnumUP, int bitnumDN) {
  if ( (bitRead(last_button_states, bitnumUP) != bitRead(button_states, bitnumUP)) || (bitRead(last_button_states, bitnumDN) != bitRead(button_states, bitnumDN)) ) {
    if (bitRead(button_states, bitnumUP)) {
      DcsBios::sendDcsBiosMessage(item, "2");
    }
    else if (bitRead(button_states, bitnumDN)) {
      DcsBios::sendDcsBiosMessage(item, "0");
    }
    else {
      DcsBios::sendDcsBiosMessage(item, "1");
    }
  }
}

void setup() {
  pinMode(PIN_77HC165_SH_LD, OUTPUT);
  pinMode(PIN_77HC165_CLK, OUTPUT);
  pinMode(PIN_77HC165_QH, INPUT);
  DcsBios::setup();
}

void loop() {
  
  load_pio();
  button_states += (long)shiftIn(PIN_77HC165_QH, PIN_77HC165_CLK, LSBFIRST) << 24;
  button_states += (long)shiftIn(PIN_77HC165_QH, PIN_77HC165_CLK, LSBFIRST) << 16;
  button_states += (long)shiftIn(PIN_77HC165_QH, PIN_77HC165_CLK, LSBFIRST) << 8;
  button_states += (long)shiftIn(PIN_77HC165_QH, PIN_77HC165_CLK, LSBFIRST);

  pushButtonCheck("ICP_COM1_BTN", BIT_ICP_COM1_BTN);
  pushButtonCheck("ICP_COM2_BTN", BIT_ICP_COM2_BTN);
  pushButtonCheck("ICP_IFF_BTN", BIT_ICP_IFF_BTN);
  pushButtonCheck("ICP_LIST_BTN", BIT_ICP_LIST_BTN);
  pushButtonCheck("ICP_AA_MODE_BTN", BIT_ICP_AA_MODE_BTN);
  pushButtonCheck("ICP_AG_MODE_BTN", BIT_ICP_AG_MODE_BTN);
  pushButtonCheck("ICP_BTN_1", BIT_ICP_BTN_1);
  pushButtonCheck("ICP_BTN_2", BIT_ICP_BTN_2);
  pushButtonCheck("ICP_BTN_3", BIT_ICP_BTN_3);
  pushButtonCheck("ICP_BTN_4", BIT_ICP_BTN_4);
  pushButtonCheck("ICP_BTN_5", BIT_ICP_BTN_5);
  pushButtonCheck("ICP_BTN_6", BIT_ICP_BTN_6);
  pushButtonCheck("ICP_BTN_7", BIT_ICP_BTN_7);
  pushButtonCheck("ICP_BTN_8", BIT_ICP_BTN_8);
  pushButtonCheck("ICP_BTN_9", BIT_ICP_BTN_9);
  pushButtonCheck("ICP_BTN_0", BIT_ICP_BTN_0);
  pushButtonCheck("ICP_RCL_BTN", BIT_ICP_RCL_BTN);
  pushButtonCheck("ICP_ENTR_BTN", BIT_ICP_ENTR_BTN);
  pushButtonCheck("ICP_WX_BTN", BIT_ICP_WX_BTN);
  Switch3PosCheck("ICP_DATA_RTN_SEQ_SW", BIT_ICP_DATA_RTN_SEQ_SW_SEQ, BIT_ICP_DATA_RTN_SEQ_SW_RNT);
  Switch3PosCheck("ICP_DATA_UP_DN_SW", BIT_ICP_DATA_UP_DN_SW_UP, BIT_ICP_DATA_UP_DN_SW_DN);
  Switch3PosCheck("ICP_DRIFT_SW", BIT_ICP_DRIFT_SW_DRIFT, BIT_BIT_ICP_DRIFT_SW_DRIFT_WARN);
  Switch3PosCheck("ICP_FLIR_SW", BIT_ICP_FLIR_SW_UP, BIT_ICP_FLIR_SW_DN);
  Switch3PosCheck("ICP_DED_SW", BIT_ICP_DED_SW_UP, BIT_ICP_DED_SW_DN);
  Switch3PosCheck("ICP_FLIR_GAIN_SW", BIT_ICP_FLIR_GAIN_SW_GAIN, BIT_ICP_FLIR_GAIN_SW_AUTO);

  DcsBios::loop();

  last_button_states = button_states;
  button_states=0;

}