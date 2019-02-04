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

	};
}

#endif
// EOF
