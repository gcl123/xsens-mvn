/*
 * Copyright (C) 2016 iCub Facility
 * Authors: Francesco Romano
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */


#ifndef YARP_XSENSMVN_H
#define YARP_XSENSMVN_H

#include <yarp/os/Mutex.h>

#include <yarp/dev/DeviceDriver.h>
#include <yarp/dev/PreciselyTimed.h>

#include <yarp/sig/Vector.h>

namespace yarp {
namespace dev {

class XsensMVN : public yarp::dev::DeviceDriver,
                 public yarp::dev::IPreciselyTimed
{
private:
    // Prevent copy 
    XsensMVN(const XsensMVN & other);
    XsensMVN & operator=(const XsensMVN & other);
    
    // Use a mutex to avoid race conditions
    yarp::os::Mutex m_mutex;
    
    // Buffers of sensor data and timestamp
    yarp::sig::Vector m_sensorReadings;
    yarp::os::Stamp m_timestamp;

    class XsensMVNPrivate;
    XsensMVNPrivate *m_pimpl;
    
public:
    XsensMVN();
    virtual ~XsensMVN();

    // DeviceDriver interface 
    bool open(yarp::os::Searchable &config);
    bool close();
  
    // IPreciselyTimed interface
    virtual yarp::os::Stamp getLastInputStamp();
};

}
}

#endif //YARP_XSENSMVN_H

