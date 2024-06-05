#include <Arduino.h>
#include "channel.h"
#include "module.h"


ChannelAverage<int> vehicle_velocity_command,
                    vehicle_steering_command,
                    steering_motor_position_command,
                    motor_direction_bits,
                    left_motor_velocity_command,
                    right_motor_velocity_command;

MODULE(differential,
    BLOCK(
        LET(velocity = vehicle_velocity_command.get())
        DRIVE(motor_direction_bits, 
            (-127 < velocity && velocity < -1) ? 0b00010110 :
            (1 < velocity && velocity < 127) ? 0b00011001 :
            0)
    )
    DRIVE(steering_motor_position_command, vehicle_steering_command.get() - 10)
    BLOCK(
        LET(velocity = vehicle_velocity_command.get() * 16)
        LET(offset = vehicle_velocity_command.get() * vehicle_steering_command.get() * 2)
        DRIVE(left_motor_velocity_command, velocity + offset)
        DRIVE(right_motor_velocity_command, velocity - offset)
    )
)

ChannelAverage<float> input_channel;
ChannelAverage<int> count_channel;
ChannelAverage<float> output_channel;

MODULE(count_transitions,
  SLET(count = 0)
  SLET(last_sign = 1)
  LET(this_sign = input_channel.get() < 0 ? -1 : 1)
  if(last_sign != this_sign)
  {
    count++;
    last_sign = this_sign;
  }
  DRIVE(count_channel, count)
)


MODULE(gain2,
  DRIVE(output_channel, 2 * input_channel.get())
)

Updatable *signals[] = {
  &input_channel,
  &gain2,
  &count_transitions,
  &count_channel,
  &output_channel
};

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  // while(Serial.available())
  // {
  //   char ch = Serial.read();
  //   chmin.drive(ch);
  //   chmax.drive(ch);
  // }
  // chmax.update();
  // chmin.update();
  // Serial.print(chmax.get());
  // Serial.print(" ");
  // Serial.println(chmin.get());

  // delay(2500);

  DRIVE(input_channel, sin(millis() * 0.1))

  for(auto s : signals)
  {
    s->update();
  }

  Serial.print(input_channel.get());
  Serial.print(" ");
  Serial.print(count_channel.get());
  Serial.print(" ");
  Serial.println(output_channel.get());
  delay(200);
}
