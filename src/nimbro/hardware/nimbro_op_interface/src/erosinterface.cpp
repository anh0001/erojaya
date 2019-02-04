// NimbRo-OP robot hardware interface (dummy)
// Author: Philipp Allgeuer <pallgeuer@ais.uni-bonn.de>

// Includes
#include <nimbro_op_interface/erosinterface.h>
#include <pluginlib/class_list_macros.h>
#include <robotcontrol/hw/dynamiccommandgenerator.h>
#include <rot_conv/rot_conv.h>
#include <usb2dyna/dynamixel.h>

// Namespaces
using namespace nimbro_op_interface;
using namespace cm730;

//
// ErosInterface class
//

PLUGINLIB_EXPORT_CLASS(nimbro_op_interface::ErosInterface, robotcontrol::HardwareInterface);
// EOF
