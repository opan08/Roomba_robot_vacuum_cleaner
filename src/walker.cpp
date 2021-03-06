/**
Copyright [MIT] 2019 Shivam Akhauri
Permission is hereby granted, free of charge, to any person obtaining a copy of 
this software and associated documentation files (the "Software"), to deal in 
the Software without restriction, including without limitation the rights to 
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:
The above copyright notice and this permission notice shall be included in all 
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
 COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER 
 IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**/



/**
* @file walker.cpp
* @author Shivam Akhauri 
* @date 14 November 2019
* @copyright 2019 Shivam Akhauri
* @brief publishes the velocities to the turtlebot 
* to move around changes angular velocity(direction) 
* if the collision flag is received as 1
*/

#include "walker.hpp"

Walker::Walker() {
  velocity = nh.advertise <geometry_msgs::Twist>
 ("/mobile_base/commands/velocity", 1000);
  distanceList = nh.subscribe <sensor_msgs::LaserScan>
 ("/scan", 50, &DepthCalculation::findLaserDepth, &depth);
  // initialize the velocities
  msg.linear.x = 0.0;
  msg.linear.y = 0.0;
  msg.linear.z = 0.0;
  msg.angular.x = 0.0;
  msg.angular.y = 0.0;
  msg.angular.z = 0.0;
  // publish velocity values for turtlebot
  velocity.publish(msg);
}

Walker::~Walker() {
  // stop the robot motion
  msg.linear.x = 0.0;
  msg.linear.y = 0.0;
  msg.linear.z = 0.0;
  msg.angular.x = 0.0;
  msg.angular.y = 0.0;
  msg.angular.z = 0.0;
  velocity.publish(msg);
}

void Walker::walk() {
  // set loop rate
  ros::Rate loop_rate(10);
  while (ros::ok()) {
    // check for obstacle
    if (depth.flagCollision()) {
      // stop linear motion and begin angular velocity of
      // turtlebot to avoid collision
      msg.linear.x = 0.0;
      msg.angular.z = 3.0;
    } else {
      // if no obstacle move straight
      msg.linear.x = 0.5;
      msg.angular.z = 0.0;
    }
    // publish the new velocities
    velocity.publish(msg);
    ros::spinOnce();
    // slows the frequency
    loop_rate.sleep();
  }
}
