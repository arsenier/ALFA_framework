#include <Arduino.h>
#include "module.h"
#include "channel.h"
#include "network.h"


// ChannelAverage<int> vehicle_velocity_command,
//                     vehicle_steering_command,
//                     steering_motor_position_command,
//                     motor_direction_bits,
//                     left_motor_velocity_command,
//                     right_motor_velocity_command;

// MODULE(differential,
//     BLOCK(
//         LET(velocity = vehicle_velocity_command)
//         DRIVE(motor_direction_bits, 
//             (-127 < velocity && velocity < -1) ? 0b00010110 :
//             (1 < velocity && velocity < 127) ? 0b00011001 :
//             0)
//     )
//     DRIVE(steering_motor_position_command, vehicle_steering_command - 10)
//     BLOCK(
//         LET(velocity = vehicle_velocity_command * 16)
//         LET(offset = vehicle_velocity_command * vehicle_steering_command * 2)
//         DRIVE(left_motor_velocity_command, velocity + offset)
//         DRIVE(right_motor_velocity_command, velocity - offset)
//     )
// )

// ChannelAverage<float> input_channel;
// ChannelAverage<int> count_channel;
// ChannelAverage<float> output_channel;
// ChannelComputed<float> gain2_channel([](){return input_channel * 2;});

// MODULE(count_transitions,
//   SLET(count = 0)
//   SLET(last_sign = 1)
//   LET(this_sign = input_channel < 0 ? -1 : 1)
//   if(last_sign != this_sign)
//   {
//     count++;
//     last_sign = this_sign;
//   }
//   DRIVE(count_channel, count)
// )


// MODULE(gain2,
//   DRIVE(output_channel, 2 * input_channel)
// )

// Updatable *signals[] = {
//   &input_channel,
//   &gain2,
//   &gain2_channel,
// //   &count_transitions,
// //   &count_channel,
//   &output_channel
// };

int sens = 0;
int fout, fout2;
ChannelSensor<int> in(&sens);
ChannelAverage<int> out, out2;

// template <size_t link_count = 128>
// class Network : public Updatable
// {
//     Updatable *links[link_count];
//     size_t links_registered = 0;

// public:
//     Network()
//     {
//         MODULE(test,
//         INPUTS(&in),
//         OUTPUTS(&out),
//           DRIVE(out, -2*in)
//         )
//     }

//     void register_link(Updatable *link)
//     {
//       links[links_registered] = link;
//       links_registered++;
//     }

//     void register_module(Updatable *module)
//     {
//       register_link(module);
//     }

//     void register_input(Updatable *input)
//     {
//       register_link(input);
//     }
    
//     void register_output(Updatable *output)
//     {
//       register_link(output);
//     }

//     void update() override
//     {
//       for(size_t i = 0; i < links_registered; i++)
//       {
//         links[i]->update();
//       }
//     }
// };

// Network<128> network;

NETWORK(test, 120,
  MODULE(mtest,
  INPUTS(&in),
  OUTPUTS(&out, &out2),
    DRIVE(out, -2*in)
    // DRIVE(out2, 3*in)
  )
  // MODULE(gain3,
  // INPUTS(&in),
  // OUTPUTS(&out2),
  //   DRIVE(out2, 3*in)
  // )
)

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  sens = sin(millis() * 0.1)*100;
  // DRIVE(input_channel, sin(millis() * 0.1))

  // for(auto s : signals)
  // {
  //   s->update();
  // }

  uint32_t time0 = micros();
  // in.update();
  // DRIVE(out, 2*in)
  // out.update();
  // out2.update();
  test.update();
  // out2.update();
  // test.update();
  // fout = -2*sens;
  // fout2 = 3*sens;
  uint32_t dt = micros() - time0;

  Serial.print(dt);
  Serial.print(" ");
  Serial.print(sens);
  Serial.print(" ");
  Serial.print(in);
  Serial.print(" ");
  Serial.print(out);
  Serial.print(" ");
  Serial.println(out2);
  delay(200);
}
