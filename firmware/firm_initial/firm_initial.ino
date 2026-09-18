#include <WiFi.h>
#include <esp_now.h>
#include <driver/i2s.h>
#include <Wire.h>
#include <FastLED.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// pin defs. 
#define I2C_SDA 9
#define I2C_SCL 8
#define LED_DIN 11
#define POTENT_PIN 3
#define PUSH_SPEAK 10

// INMP441 Microphone
#define MIC_SCK 5
#define MIC_WS 6
#define MIC_SD 4

// MAX98357A amp
#define AMP_DIN 7
#define AMP_BCLK 15
#define AMP_LRCK 16

//peripherals
#define SC_WIDTH 128
#define SC_HEIGHT 32
Adafruit_SSD1306 display(SC_WIDTH, SC_HEIGHT, &Wire, -1);
CRGB leds[1];



//esp-now rf comms. 
//  ESP32 receiver MAC address
uint8_t broadcastAddress[] = {0x30, 0xAE, 0xA4, 0x07, 0x0D, 0x64}; // change to receiver mac addr. 
esp_now_peer_info_t peerInfo;
// sys_state & audio
bool transmitting=false;
#define AUDIO_SIZE 1440
uint8_t audioBuffer[AUDIO_SIZE];

//protoype
void updatemsg(String);
void i2sInit();
void sendAudio();
void onRecieve(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len);

void setup() {
  Serial.begin(115200);

  //init ui
  pinMode(PUSH_SPEAK, INPUT_PULLUP);
  pinMode(POTENT_PIN, INPUT);

  FastLED.addLeds<WS2812B, LED_DIN, GRB>(leds,1);
  leds[0]= CRGB::Blue;
  FastLED.show();

  Wire.begin(I2C_SDA,I2C_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC,0x3C)){
    Serial.println("OLED START FAIL!!");
  }
  else{
    updatemsg("Sanchar-Jaal Boot");
  }

  //init. espnow
  WiFi.mode(WIFI_STA);
  if(esp_now_init()!=ESP_OK){
    Serial.println("Error ESP-NOW communication");
    return;
  }
  memcpy(peerInfo.peer_addr, broadcastAddress,6);
  peerInfo.channel=0;
  peerInfo.encrypt=false;
  esp_now_add_peer(&peerInfo);
  esp_now_register_recv_cb(onRecieve);

  // ini. audio
  i2sInit();

  //boot success
  //listening ready
  leds[0]= CRGB::Black;
  FastLED.show();
  updatemsg("Talk!! I am Listening..");

}

void loop() {
  //read pust-to-talk buttons
  bool talkbutt= (digitalRead(PUSH_SPEAK)==LOW);
  if (talkbutt && !transmitting){
    transmitting=true;
    leds[0]= CRGB::Red;
    FastLED.show();
    updatemsg("Transmitting.....!");
  }
  else if(!talkbutt && transmitting){
    transmitting=false;
    leds[0]= CRGB::Black;
    FastLED.show();
    updatemsg("Listening.....!");
  }

  //capture mic audio..send
  if(transmitting){
    sendAudio();
  }
  else{
    delay(2);
  }

}

void updatemsg(String msg) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("Channel: 1");
  display.setCursor(0,16);
  display.println(msg);
  display.display();
}

void i2sInit() {
  //config. i2s port mic
  i2s_config_t i2s_mic_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 720,
    .use_apll = false,
  };

  const i2s_pin_config_t MIC_pin_config = {
    .bck_io_num = MIC_SCK,
    .ws_io_num = MIC_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = MIC_SD
  };

  i2s_driver_install(I2S_NUM_0, &i2s_mic_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &MIC_pin_config);

  // config. i2s port for speaker
  i2s_config_t i2s_amp_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 720,
    .use_apll = false,
  };

  const i2s_pin_config_t AMP_pin_config = {
    .bck_io_num = AMP_BCLK,
    .ws_io_num = AMP_LRCK,
    .data_out_num = AMP_DIN,
    .data_in_num = I2S_PIN_NO_CHANGE
  };

  i2s_driver_install(I2S_NUM_1, &i2s_amp_config, 0, NULL);
  i2s_set_pin(I2S_NUM_1, &AMP_pin_config);
}

void sendAudio() {
  size_t bytesRead;
  // inmp441 raw audio buffer
  esp_err_t res = i2s_read(I2S_NUM_0, audioBuffer, AUDIO_SIZE, &bytesRead, portMAX_DELAY);
  if(res == ESP_OK && bytesRead > 0) {
     esp_now_send(broadcastAddress, audioBuffer, bytesRead);
  }
}

// WHEN TALKING STOP PLAYING AUDIO SO THAT HALF-DUPLEX MODE APPLIES, AMPLIFY, LED ON
void onRecieve(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  if(transmitting) return;
  
  leds[0] = CRGB::Green;
  FastLED.show();

  // read potent.(0-4095 esp le read garcha) 
  float volmull= analogRead(POTENT_PIN)/4095;

  //crete output buffer
  uint8_t outgoingdata[AUDIO_SIZE];

  //DONE WITH AI

  // Cast the raw bytes into 16-bit audio numbers so we can do math on them
  int16_t *incomingSamples = (int16_t *)incomingData;
  int16_t *adjustedSamples = (int16_t *)outputBuffer;
  
  // Calculate how many 16-bit samples are in this packet (length / 2)
  int numSamples = len / 2; 

  // 3. Multiply every single audio sample by our volume fraction
  for (int i = 0; i < numSamples; i++) {
    adjustedSamples[i] = incomingSamples[i] * volmull;
  }

  // 


  // END  AI

  size_t bytes_written;
  // transmit to amp. 
  i2s_write(I2S_NUM_1, outgoingdata, len, &bytes_written, portMAX_DELAY);

  leds[0] = CRGB::Black;
  FastLED.show();
}


