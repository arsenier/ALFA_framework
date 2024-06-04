#include <Arduino.h>
#include "channel.h"
#include "module.h"

ChannelMaximum<float> chmax(-15);
ChannelMinimum<float> chmin(100);

ChannelMinimum<float> ch1(1), ch2(1);

void module_gain()
{
    {
        ch1.drive(2*ch2.get());
    }
}

ChannelAverage<int> vehicle_velocity_command,
                    vehicle_steering_command,
                    steering_motor_position_command,
                    motor_direction_bits,
                    left_motor_velocity_command,
                    right_motor_velocity_command;

void differential()
{
    { // motor_direction_bits
        const float vel = vehicle_velocity_command.get();
        motor_direction_bits.drive(
            (-127 < vel && vel < -1) ? 0b00010110 :
            (1 < vel && vel < 127) ? 0b00011001 :
            0
        );
    }
    {
        steering_motor_position_command.drive(vehicle_steering_command.get() - 10);
    }
    {
        const float velocity = vehicle_velocity_command.get() * 16;
        const float offset = vehicle_velocity_command.get() * vehicle_steering_command.get() * 2;
        {
            left_motor_velocity_command.drive(fabs(velocity + offset));
            right_motor_velocity_command.drive(fabs(velocity - offset));
        }
    }
}

MODULE differentialM()
{
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
}

ChannelAverage<float> input_channel;
ChannelAverage<unsigned short> output_channel;

MODULE count_transitions()
{
    SLET(count = 0)
    SLET(last_sign = 1)
    DRIVE(output_channel, count)
    LET(this_sign = input_channel.get() < 0 ? -1 : 1)
    if(last_sign != this_sign)
    BLOCK(
        count++;
        last_sign = this_sign;
    )
}


void setup()
{
  Serial.begin(9600);
}

MODULE_POINTER modules[] = {
  count_transitions,
  differential,
  differentialM
};

ChannelNoT *signals[] = {
  &input_channel,
  &output_channel
};

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
  input_channel.update();

  for(auto m : modules)
  {
    (*m)();
  }
  // for(auto s : signals)
  // {
  //   s->update();
  // }

  output_channel.update();

  Serial.print(input_channel.get());
  Serial.print(" ");
  Serial.println(output_channel.get());
  delay(200);
}
