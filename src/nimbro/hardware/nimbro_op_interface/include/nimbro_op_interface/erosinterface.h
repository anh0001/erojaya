// Eros robot hardware interface (dummy with heading from topic)
// Author: Anhar <anhar@pens.ac.id>

// Ensure header is only included once
#ifndef EROS_INTERFACE_H
#define EROS_INTERFACE_H

// Includes
#include <nimbro_op_interface/robotinterface.h>
#include <boost/circular_buffer.hpp>
#include <config_server/parameter.h>
#include <std_msgs/Float32.h>

// NimbRo-OP interface namespace
namespace nimbro_op_interface
{
	// ErosInterface class
	class ErosInterface : public virtual RobotInterface
	{
	public:
		// Constructor/destructor
		ErosInterface();
		virtual ~ErosInterface();

		// Virtual function overrides
		virtual bool sendJointTargets();
		virtual bool readJointStates();

		// Constants
		static const std::string CONFIG_PARAM_PATH;

	protected:
		// Virtual functions to override attempts to connect to the CM730
		virtual bool initCM730() { return true; }
		virtual int  readFeedbackData(bool onlyTryCM730);
		virtual bool syncWriteJointTargets(size_t numDevices, const uint8_t* data);
		virtual bool syncWriteTorqueEnable(size_t numDevices, const uint8_t* data) { return true; }
		virtual bool syncWriteTorqueLimit(size_t numDevices, const uint8_t* data) { return true; }
		virtual bool useModel() const { return m_useModel(); }

	private:
		// Config server parameters
		config_server::Parameter<bool>  m_useModel;
		config_server::Parameter<bool>  m_addDelay;
		config_server::Parameter<bool>  m_noiseEnable;
		config_server::Parameter<float> m_noiseMagnitude;
		config_server::Parameter<bool>  m_buttonPress0;
		config_server::Parameter<bool>  m_buttonPress1;
		config_server::Parameter<bool>  m_buttonPress2;
		config_server::Parameter<int>   m_fakeTemperature;
		config_server::Parameter<float> m_fakeIMUGyroX;
		config_server::Parameter<float> m_fakeIMUGyroY;
		config_server::Parameter<float> m_fakeIMUGyroZ;
		config_server::Parameter<float> m_fakeIMUAccX;
		config_server::Parameter<float> m_fakeIMUAccY;
		config_server::Parameter<float> m_fakeIMUAccZ;
		config_server::Parameter<float> m_fakeIMUMagX;
		config_server::Parameter<float> m_fakeIMUMagY;
		config_server::Parameter<float> m_fakeIMUMagZ;
		config_server::Parameter<bool>  m_fakeAttEnable;
		config_server::Parameter<float> m_fakeAttFusedX;
		config_server::Parameter<float> m_fakeAttFusedY;
		config_server::Parameter<float> m_fakeAttFusedZ;
		config_server::Parameter<bool>  m_fakeAttFusedHemi;

		// Config server callbacks
		void updateUseModel();

		// Misc flags
		bool m_calledReadFeedback;

		// Joint command data buffer
		typedef std::map<uint8_t, JointCmdSyncWriteData> JointCmd;
		boost::circular_buffer<JointCmd> m_jointCmdBuf;

		// ROS subscribers
		ros::Subscriber m_sub_heading;

		// ROS data handlers
		void handleHeadingData(const std_msgs::Float32::ConstPtr& msg);

		// heading data from a topic
		float m_attFusedZ;
	};
}

#endif
// EOF
