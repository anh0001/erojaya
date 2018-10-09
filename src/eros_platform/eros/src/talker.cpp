#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include "ros/ros.h"
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int32MultiArray.h"

#define MAX_DATA_TO_SERIAL 4
#define MAX_DATA_TO_REC 5

#define MAX_DATA_FROM_SERIAL  7
#define MAX_DATA_FROM_REFEREE 3
#define MAX_DATA_FROM_REC 1
using namespace std;

int dtaFromSerial[MAX_DATA_FROM_SERIAL];
int dtaFromReferee[MAX_DATA_FROM_REFEREE];
int dtaFromRec[MAX_DATA_FROM_REC];
int dtaHeading,dtaFlagOdo,dtaOdoX,dtaOdoZ,dtaOdoH,dtaFall,dtaOdoM;  //Data matang yang akan dikirim ke IPC
void SerialItCallback(const std_msgs::Int32MultiArray::ConstPtr& msg){
    int i=0;
    for(std::vector<int>::const_iterator it=msg->data.begin(); it!=msg->data.end(); ++it){
      dtaFromSerial[i]=*it;
      i++;
    }
    dtaFall=dtaFromSerial[0];
    dtaHeading=dtaFromSerial[1];
    dtaFlagOdo=dtaFromSerial[2];
    dtaOdoX=dtaFromSerial[3];
    dtaOdoZ=dtaFromSerial[4];
    dtaOdoH=dtaFromSerial[5];
    dtaOdoM=dtaFromSerial[6];
    ROS_INFO("I heard Serial: [%d][%d][%d][%d][%d][%d][%d]", dtaHeading,dtaFlagOdo,dtaOdoX,dtaOdoZ,dtaOdoH,dtaFall,dtaOdoM);
}

void RecItCallback(const std_msgs::Int32MultiArray::ConstPtr& msg){
    int i=0;
    for(std::vector<int>::const_iterator it=msg->data.begin(); it!=msg->data.end(); ++it){
      dtaFromRec[i]=*it;
      i++;
    }
    ROS_INFO("I heard Rec: [%d]",dtaFromRec[0]);
}

void RefereeItCallback(const std_msgs::Int32MultiArray::ConstPtr& msg){
    int i=0;
    for(std::vector<int>::const_iterator it=msg->data.begin(); it!=msg->data.end(); ++it){
      dtaFromReferee[i]=*it;
      i++;
    }
    ROS_INFO("I heard Referee: [%d][%d][%d]", dtaFromReferee[0],dtaFromReferee[1],dtaFromReferee[2]);
}

/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
int main(int argc, char **argv)
{
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line.
   * For programmatic remappings you can use a different version of init() which takes
   * remappings directly, but for most command-line programs, passing argc and argv is
   * the easiest way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "talker");

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;

  /**
   * The advertise() function is how you tell ROS that you want to
   * publish on a given topic name. This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing. After this advertise() call is made, the master
   * node will notify anyone who is trying to subscribe to this topic name,
   * and they will in turn negotiate a peer-to-peer connection with this
   * node.  advertise() returns a Publisher object which allows you to
   * publish messages on that topic through a call to publish().  Once
   * all copies of the returned Publisher object are destroyed, the topic
   * will be automatically unadvertised.
   *
   * The second parameter to advertise() is the size of the message queue
   * used for publishing messages.  If messages are published more quickly
   * than we can send them, the number here specifies how many messages to
   * buffer up before throwing some away.
   */

  //ROS Publish/Subscribe Intialize ====================================
  //serial/serialcom.cpp
  std_msgs::Int32MultiArray dtaPublishSERIAL;
  ros::Subscriber serial_it = n.subscribe("serial_it", 100, SerialItCallback);
  ros::Publisher  it_serial = n.advertise<std_msgs::Int32MultiArray>("it_serial", 100);

  //referee/referee.cpp
  ros::Subscriber referee_it = n.subscribe("referee_it", 100, RefereeItCallback);

  //communication/clientrec.cpp
  std_msgs::Int32MultiArray dtaPublishREC;
  ros::Subscriber rec_it = n.subscribe("rec_it", 100, RecItCallback);
  ros::Publisher  it_rec = n.advertise<std_msgs::Int32MultiArray>("it_rec", 100);

  //====================================================================

  //ros::Rate loop_rate(10);

  /**
   * A count of how many messages we have sent. This is used to create
   * a unique string for each message.
   */
  int count = 0;
  int i=0;
  int data[5];
  data[0]=60;
  data[1]=1;
  data[2]=0;
  data[3]=10;
  data[4]=10;
  while (ros::ok())
  {
    /**
     * This is a message object. You stuff it with data, and then publish it.
     */

    dtaPublishSERIAL.data.clear();
    for(i=0; i<MAX_DATA_TO_SERIAL; i++){
      dtaPublishSERIAL.data.push_back(data[i]);
    }

    dtaPublishREC.data.clear();
    for(i=0; i<MAX_DATA_TO_REC; i++){
      dtaPublishREC.data.push_back(data[i]);
    }

    /**
     * The publish() function is how you send messages. The parameter
     * is the message object. The type of this object must agree with the type
     * given as a template parameter to the advertise<>() call, as was done
     * in the constructor above.
     */
    it_serial.publish(dtaPublishSERIAL);
    it_rec.publish(dtaPublishREC);
    //ROS_INFO("Publishing to Serial.. [%d]", count);

    ros::spinOnce();

    //loop_rate.sleep();
    ++count;
  }


  return 0;
}
