#include <Arduino.h>
#include "channel.h"

ChannelMaximum<float> chmax(-15);
ChannelMinimum<float> chmin(100);

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  while(Serial.available())
  {
    char ch = Serial.read();
    chmin.drive(ch);
    chmax.drive(ch);
  }
  chmax.update();
  chmin.update();
  Serial.print(chmax.get());
  Serial.print(" ");
  Serial.println(chmin.get());

  delay(2500);
}
