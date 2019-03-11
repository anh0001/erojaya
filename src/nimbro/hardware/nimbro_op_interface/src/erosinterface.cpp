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

// Constants
const std::string ErosInterface::CONFIG_PARAM_PATH = "/eros_interface/";

//
// ErosInterface class
//
// Default constructor
ErosInterface::ErosInterface()
 : RobotInterface()
 , m_buttonPress0(CONFIG_PARAM_PATH + "button/pressButton0", false)
 , m_buttonPress1(CONFIG_PARAM_PATH + "button/pressButton1", false)
 , m_buttonPress2(CONFIG_PARAM_PATH + "button/pressButton2", false)
 , m_buttonPress3(CONFIG_PARAM_PATH + "button/pressButton3", false)
 , m_buttonPress4(CONFIG_PARAM_PATH + "button/pressButton4", false)
 {
	// Reset the button press config parameters just in case
	m_buttonPress0.set(false);
	m_buttonPress1.set(false);
	m_buttonPress2.set(false);
	m_buttonPress3.set(false);
	m_buttonPress4.set(false);
}

int ErosInterface::readFeedbackData(bool onlyTryCM730)
{
	int ret = RobotInterface::readFeedbackData(onlyTryCM730);

	// Populate the header data
	m_boardData.id = CM730::ID_CM730;
	m_boardData.length = CM730::READ_CM730_LENGTH;
	m_boardData.startAddress = CM730::READ_SERVO_ADDRESS;
	
	// Miscellaneous
	m_boardData.power = 1; // The servos always have power
	m_boardData.ledPanel = 0; // No LEDs are on
	m_boardData.rgbled5 = 0; // RGBLED5 is off
	m_boardData.rgbled6 = 0; // RGBLED6 is off
	m_boardData.voltage = (unsigned char) ((15.0 / INT_TO_VOLTS) + 0.5); // The board voltage is always 15.0V
	m_boardData.temp = 35; // The board temperature is always 35 
	
	// Button presses
	unsigned char button = 0x00;
	if(m_buttonPress0()) { button |= 0x01; m_buttonPress0.set(false); }
	if(m_buttonPress1()) { button |= 0x02; m_buttonPress1.set(false); }
	if(m_buttonPress2()) { button |= 0x04; m_buttonPress2.set(false); }
	if(m_buttonPress3()) { button |= 0x08; m_buttonPress3.set(false); }
	if(m_buttonPress4()) { button |= 0x10; m_buttonPress4.set(false); }
	m_boardData.button = button;

	return ret;
}

PLUGINLIB_EXPORT_CLASS(nimbro_op_interface::ErosInterface, robotcontrol::HardwareInterface);
// EOF
