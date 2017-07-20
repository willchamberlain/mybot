/**
Copyright (c) 2017, William Chamberlain, ARC Centre of Excellence for Robotic Vision (ACRV: http://roboticvision.org) - Queensland University of Technology (QUT : http://qut.edu.au)
All rights reserved.

Attributions: Kevin Kuei - mybot repository at https://github.com/wennycooper/mybot

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, the above attributions of authorship and contribution, this list of conditions, and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, the above attributions of authorship and contribution, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <ros/ros.h>
//#include <sensor_msgs/JointState.h>
#include <tf/transform_broadcaster.h>

//#include <nav_msgs/Odometry.h>
#include <iostream>
using namespace std;
//double wheelSeparation = 0.22; //for small lambo
//double wheelSeparation = 0.3996; //for Andbot
double wheelSeparation = 0.215; //for KKuei's mobile robot
//double wheelRadius = 0.076;
//double wheelRadius = 0.0637; for small lambo
double wheelRadius = 0.0625; //for KKuei's mobile robot

double vl = 0.0;
double vr = 0.0;

ros::Publisher cmd_wheel_angularVel_pub;
ros::Subscriber cmd_vel_sub;

void cmd_velCallback(const geometry_msgs::Twist &twist_aux)
{
  geometry_msgs::Vector3 vector3;
  geometry_msgs::Twist twist = twist_aux;
  double vel_x = twist_aux.linear.x;
  double vel_th = twist_aux.angular.z;
  double right_vel = 0.0;
  double left_vel = 0.0;

  // (vel_x, vel_th) --> (vl, vr)
  left_vel = (2*vel_x - vel_th * wheelSeparation) / 2 / wheelRadius;
  right_vel =(2*vel_x + vel_th * wheelSeparation) / 2 / wheelRadius;

  // publish to /cmd_wheel_angularVel
  vector3.x = left_vel;
  vector3.y = right_vel;
  cmd_wheel_angularVel_pub.publish(vector3);
  
}

int main(int argc, char** argv){
  ros::init(argc, argv, "mybot_base_controller");
  ros::NodeHandle n1, n2;
  cmd_vel_sub = n1.subscribe("/cmd_vel", 10, cmd_velCallback);
  cmd_wheel_angularVel_pub = n2.advertise<geometry_msgs::Vector3>("cmd_wheel_angularVel", 50);
  ros::Rate loop_rate(10);

  while(ros::ok())
  {
    ros::spinOnce();
    loop_rate.sleep();
  }
}


