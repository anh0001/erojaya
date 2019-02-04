// Eros robot hardware interface (with heading from topic)
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
		ErosInterface() = default;
		virtual ~ErosInterface() = default;

		// Virtual function overrides
		virtual bool sendJointTargets();
		virtual bool readJointStates();

	protected:
		// Virtual functions to override attempts to connect to the CM730
		virtual bool initCM730();
		virtual bool syncWriteJointTargets(size_t numDevices, const uint8_t* data);
	};
}

#endif
// EOF
