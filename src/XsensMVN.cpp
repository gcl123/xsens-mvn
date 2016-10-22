/*
 * Copyright (C) 2016 iCub Facility
 * Authors: Silvio Traversaro
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

#include "XsensMVN.h"

#include <yarp/os/LockGuard.h>
#include <xme.h>
#include <cassert>

yarp::dev::XsensMVN::XsensMVN() : m_sensorReadings(6)
{
    // We fill the sensor readings only once in the constructor in this example
    // In reality, the buffer will be updated once a new measurement is avaible
    
    // Set force on x,y,z axis 
    m_sensorReadings[0] = 1.0;
    m_sensorReadings[1] = 2.0;
    m_sensorReadings[2] = 3.0;
    
    // Set torque on x,y,z axis 
    m_sensorReadings[3] = 0.1;
    m_sensorReadings[4] = 0.2;
    m_sensorReadings[5] = 0.3;
    
    // When you update the sensor readings, you also need to update the timestamp
    m_timestamp.update();
    
}

yarp::dev::XsensMVN::~XsensMVN()
{
}

bool yarp::dev::XsensMVN::open(yarp::os::Searchable &config)
{
    yarp::os::LockGuard guard(m_mutex);
    


    return true;
}

bool yarp::dev::XsensMVN::close()
{
    yarp::os::LockGuard guard(m_mutex);
    
    return true;
}

yarp::dev::XsensMVN::XsensMVN(const yarp::dev::XsensMVN& /*other*/)
{
    // Copy is disabled 
    assert(false);
}

yarp::dev::XsensMVN& yarp::dev::XsensMVN::operator=(const yarp::dev::XsensMVN& /*other*/)
{
    // Copy is disabled 
    assert(false);
    return *this;
}

yarp::os::Stamp yarp::dev::XsensMVN::getLastInputStamp()
{
    return m_timestamp;
}
