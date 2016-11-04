/*
* Copyright (C) 2016 iCub Facility
* Authors: Francesco Romano
* CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
*/

#include "XsensMVNPrivate.h"

#include <xme.h>
#include <yarp/os/Searchable.h>
#include <yarp/os/LogStream.h>
#include <string>
#include <chrono>


yarp::dev::XsensMVN::XsensMVNPrivate::XsensMVNPrivate()
: m_connection(0) 
{}

yarp::dev::XsensMVN::XsensMVNPrivate::~XsensMVNPrivate() {}

bool yarp::dev::XsensMVN::XsensMVNPrivate::init(yarp::os::Searchable &config)
{
    if (m_connection) return false;
  
    m_connection = XmeControl::construct();
    if (!m_connection) return false;

    m_connection->addCallbackHandler(this);
    XsStringArray configurations = m_connection->configurationList();
    
    bool configFound = false;
    std::string configuration = config.check("config", yarp::os::Value(""), "checking MVN configuration").asString();
    for (const XsString &conf : configurations) {
        if (conf == configuration.c_str()) {
            configFound = true;
            break;
        }
    }
    if (!configFound) {
        yError("Configuration %s not found", configuration.c_str());
        return false;
    }

    double scanTimeout = config.check("scanTimeout", yarp::os::Value(5.0), "scan timeout before failing (in seconds). -1 for disabling timeout").asDouble();
    m_connection->setConfiguration(configuration.c_str());

    //This call is asynchronous.
    //Manually synchronize it
    //std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now(); //why is this wrong?
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration<double>(scanTimeout);

    std::unique_lock<std::mutex> lock(m_mutex);
    m_hardwareFound = false;
    m_connection->setScanMode(true);
    if (scanTimeout > 0)
        m_initializationVariable.wait_until(lock, now + duration, [&]() { return !m_hardwareFound; });
    else 
        m_initializationVariable.wait(lock, [&]() { return !m_hardwareFound; });
    //I should check if everything is ok..
    XmeStatus status = m_connection->status();
    //TODO: check something here



    //create processor thread to free computation from MVN callbacks
    m_processor = std::thread(&yarp::dev::XsensMVN::XsensMVNPrivate::processNewFrame, this);

    return m_hardwareFound;
}

bool yarp::dev::XsensMVN::XsensMVNPrivate::fini()
{
    if (m_connection) {
        m_connection->addCallbackHandler(0);
        m_connection->setScanMode(false);
        m_connection->disconnectHardware();

        m_connection->destruct();
        m_connection = 0;
    }

    return true;
}

bool yarp::dev::XsensMVN::XsensMVNPrivate::startAcquisition()
{
    if (!m_connection) return false;
    //TODO: do some checks also on the status of the device

    m_connection->setRealTimePoseMode(true);
    return true;

}

bool yarp::dev::XsensMVN::XsensMVNPrivate::stopAcquisition()
{
    if (!m_connection) return false;

    m_connection->setRealTimePoseMode(false);
    return true;
}


void yarp::dev::XsensMVN::XsensMVNPrivate::processNewFrame()
{
    std::unique_lock<std::mutex> lock(m_processorGuard);
    //while no data
    m_processorVariable.wait(lock, [&](){ return !m_frameData.empty(); });
    //get the copied data
    FrameData lastFrame = m_frameData.front();
    m_frameData.pop();
    //release lock
    lock.unlock();

    //process incoming pose to obtain information
    int64_t time = lastFrame.pose.m_absoluteTime;
    for (auto segment : lastFrame.pose.m_segmentStates) {
        //linear information
        segment.m_position;
        segment.m_velocity;
        segment.m_acceleration;
        //angular information
        segment.m_orientation;
        segment.m_angularAcceleration;
        segment.m_angularVelocity;
    }
}



// Callback functions
void yarp::dev::XsensMVN::XsensMVNPrivate::onHardwareReady(XmeControl *dev)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_hardwareFound = true;
    m_initializationVariable.notify_one();
}

void yarp::dev::XsensMVN::XsensMVNPrivate::onHardwareError(XmeControl* dev)
{
    //TODO: For now log this stuff.. 
    XmeStatus status = dev->status();
    yWarning("Hw error received");
}

void yarp::dev::XsensMVN::XsensMVNPrivate::onPoseReady(XmeControl* dev)
{
    FrameData newFrame;
    newFrame.pose = dev->pose(XME_LAST_AVAILABLE_FRAME);
    XmeSensorSampleArray sensorData = dev->sampleData(XME_LAST_AVAILABLE_FRAME);
    // or suitSample (int frameNumber)??
 

    std::unique_lock<std::mutex> lock(m_processorGuard);
    //copy data into processor queue
    m_frameData.push(newFrame);
    m_processorVariable.notify_one();

}
