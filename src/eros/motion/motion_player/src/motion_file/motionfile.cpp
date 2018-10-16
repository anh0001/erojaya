//Library to manage motion files
//Author: Sebastian Schüller <schuell1@cs.uni-bonn.de>

#include "motion_file/motionfile.h"

#include <fstream>

#include <yaml-cpp-0.3/emitter.h>
#include <yaml-cpp-0.3/parser.h>
#include <yaml-cpp-0.3/node.h>
#include <boost/concept_check.hpp>

using namespace motionfile;

Motion::Motion()
{
}

FrameJoint::FrameJoint()
 : position(0)
 , velocity(0)
 , effort(1)
{}


void operator >> (const YAML_0_3::Node& node, FrameJoint& joint)
{
	node["position"] >> joint.position;
	node["effort"] >> joint.effort;
	node["velocity"] >> joint.velocity;
}


void insertKeyframe(const YAML_0_3::Node& node, Motion::KeyframPtr frame, std::vector<std::string>& jointList)
{
	node["duration"] >> frame->duration;
	node["support"] >> frame->support;

	const YAML_0_3::Node& joints = node["joints"];
	frame->joints.resize(joints.size());
	YAML_0_3::Iterator joint;

	int i = 0;
	for (joint = joints.begin(); joint != joints.end(); ++joint)
	{
		joint.second() >> frame->joints[i++];
		std::string key;
		joint.first() >> key;
		if (std::find(jointList.begin(), jointList.end(), key) == jointList.end())
			jointList.push_back(key);
	}
}

bool Motion::load(std::string name)
{
	std::ifstream inFile;
	inFile.open(name.c_str());
	if (!inFile.is_open())
		return false;

	YAML_0_3::Parser fileParser(inFile);
	YAML_0_3::Node motionFile;
	fileParser.GetNextDocument(motionFile);
	try
	{
		motionFile["header"]["name"] >> motionName;
		motionFile["header"]["preState"] >> preState;
		motionFile["header"]["playState"] >> playState;
		motionFile["header"]["postState"] >> postState;

		const YAML_0_3::Node& keyframes = motionFile["motion"];
		frames.clear();
		frames.resize(keyframes.size());
		jointList.clear();
		for (unsigned i = 0; i < keyframes.size(); i++)
		{
			KeyframPtr newKeyframe(new Keyframe);
			insertKeyframe(keyframes[i], newKeyframe, jointList);
			frames[i] = newKeyframe;
		}

	}
	catch (YAML_0_3::ParserException& e)
	{
		return false;
	}

	return true;
}

bool Motion::parse(std::string motion)
{
	std::stringstream inStream(motion);
	YAML_0_3::Parser fileParser(inStream);
	YAML_0_3::Node motionFile;
	fileParser.GetNextDocument(motionFile);
	try
	{
		motionFile["header"]["name"] >> motionName;
		motionFile["header"]["preState"] >> preState;
		motionFile["header"]["playState"] >> playState;
		motionFile["header"]["postState"] >> postState;

		const YAML_0_3::Node& keyframes = motionFile["motion"];
		frames.clear();
		frames.resize(keyframes.size());
		jointList.clear();
		for (unsigned i = 0; i < keyframes.size(); i++)
		{
			KeyframPtr newKeyframe(new Keyframe);
			insertKeyframe(keyframes[i], newKeyframe, jointList);
			frames[i] = newKeyframe;
		}

	}
	catch (YAML_0_3::ParserException& e)
	{
		return false;
	}

	return true;
}


bool Motion::save(std::string name)
{

	YAML_0_3::Emitter em;
	em << YAML_0_3::BeginMap;

	//write header
	em << YAML_0_3::Key << "header";
	em << YAML_0_3::Value << YAML_0_3::BeginMap;

	em << YAML_0_3::Key << "name" << YAML_0_3::Value << motionName;
	em << YAML_0_3::Key << "preState" << YAML_0_3::Value << preState;
	em << YAML_0_3::Key << "playState" << YAML_0_3::Value << playState;
	em << YAML_0_3::Key << "postState" << YAML_0_3::Value << postState;

	em << YAML_0_3::EndMap;

	//write motion
	em << YAML_0_3::Key << "motion" << YAML_0_3::Value << YAML_0_3::BeginSeq;
	for (int i = 0; i < (int)frames.size(); i++)
	{
		em << YAML_0_3::BeginMap;
		em << YAML_0_3::Key << "duration"<< YAML_0_3::Value << frames[i]->duration;
		em << YAML_0_3::Key << "support" << YAML_0_3::Value << frames[i]->support;
		em << YAML_0_3::Key << "joints" << YAML_0_3::Value << YAML_0_3::BeginMap;
		for (int j = 0; j < (int)frames[i]->joints.size(); j++)
		{
			if (jointList[j].empty())
				continue;
			FrameJoint* joint = &(frames[i]->joints[j]);
			em << YAML_0_3::Key << jointList[j];
			em << YAML_0_3::Value << YAML_0_3::BeginMap;
			em << YAML_0_3::Key << "position" << YAML_0_3::Value << joint->position;
			em << YAML_0_3::Key << "effort" << YAML_0_3::Value << joint->effort;
			em << YAML_0_3::Key << "velocity" << YAML_0_3::Value << joint->velocity;
			em << YAML_0_3::EndMap;
		}
		em << YAML_0_3::EndMap;

		em << YAML_0_3::EndMap;
	}
	em << YAML_0_3::EndSeq;

	em << YAML_0_3::EndMap;

	std::ofstream out;
	std::string fileName = name + ".yaml";

	out.open(fileName.c_str());
	out << em.c_str() << "\n";
	out.close();
	return true;
}

std::string Motion::dump()
{
	YAML_0_3::Emitter em;
	em << YAML_0_3::BeginMap;

	//write header
	em << YAML_0_3::Key << "header";
	em << YAML_0_3::Value << YAML_0_3::BeginMap;

	em << YAML_0_3::Key << "name" << YAML_0_3::Value << motionName;
	em << YAML_0_3::Key << "preState" << YAML_0_3::Value << preState;
	em << YAML_0_3::Key << "playState" << YAML_0_3::Value << playState;
	em << YAML_0_3::Key << "postState" << YAML_0_3::Value << postState;

	em << YAML_0_3::EndMap;

	//write motion
	em << YAML_0_3::Key << "motion" << YAML_0_3::Value << YAML_0_3::BeginSeq;
	for (int i = 0; i < (int)frames.size(); i++)
	{
		em << YAML_0_3::BeginMap;
		em << YAML_0_3::Key << "duration"<< YAML_0_3::Value << frames[i]->duration;
		em << YAML_0_3::Key << "support" << YAML_0_3::Value << frames[i]->support;
		em << YAML_0_3::Key << "joints" << YAML_0_3::Value << YAML_0_3::BeginMap;
		for (int j = 0; j < (int)frames[i]->joints.size(); j++)
		{
			if (jointList[j].empty())
				continue;
			FrameJoint* joint = &(frames[i]->joints[j]);
			em << YAML_0_3::Key << jointList[j];
			em << YAML_0_3::Value << YAML_0_3::BeginMap;
			em << YAML_0_3::Key << "position" << YAML_0_3::Value << joint->position;
			em << YAML_0_3::Key << "effort" << YAML_0_3::Value << joint->effort;
			em << YAML_0_3::Key << "velocity" << YAML_0_3::Value << joint->velocity;
			em << YAML_0_3::EndMap;
		}
		em << YAML_0_3::EndMap;

		em << YAML_0_3::EndMap;
	}
	em << YAML_0_3::EndSeq;

	em << YAML_0_3::EndMap;

	std::stringstream oss;
	oss << em.c_str() << std::endl;


	return oss.str();
}
