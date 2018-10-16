#include <string>
#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <tf/transform_broadcaster.h>

int main(int argc, char** argv) {
    ros::init(argc, argv, "state_publisher");
    ros::NodeHandle n;
    tf::TransformBroadcaster pub_tf;
	std::vector<tf::StampedTransform> tf_buf;
    ros::Rate loop_rate(60);

	// Setup tf transform for /ego_rot
	tf::StampedTransform t_imu;
	t_imu.frame_id_ = "/ego_rot";
	t_imu.child_frame_id_ = "/trunk_link";
	t_imu.setIdentity();
	tf_buf.push_back(t_imu);

    while (ros::ok()) {
		ros::Time now = ros::Time::now();
		tf_buf[0].stamp_ = now;

        //send the joint state and transform
		pub_tf.sendTransform(tf_buf);

        // This will adjust as needed per iteration
        loop_rate.sleep();
    }


    return 0;
}
