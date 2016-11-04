/*
* Copyright (C) 2016 iCub Facility
* Authors: Francesco Romano
* CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
*/

#ifndef XSENSMVNPRIVATE_h
#define XSENSMVNPRIVATE_h

#include "XsensMVN.h"
#include <xsens/xmelicense.h>
#include <xsens/xmecallback.h>
#include <functional>

#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>

class XmeControl;
class XsensCallbackHandler;

namespace yarp {
    namespace os {
        class Searchable;
    }
}

 
class yarp::dev::XsensMVN::XsensMVNPrivate : public XmeCallback
{
    XmeLicense m_license;
    XmeControl *m_connection;

    std::mutex m_mutex;
    std::condition_variable m_initializationVariable;

    //process data
    struct FrameData {
        XmePose pose;
    };

    std::thread m_processor;
    bool m_stopProcessor;
    std::mutex m_processorGuard;
    std::condition_variable m_processorVariable;
    std::queue<FrameData> m_frameData;
    void processNewFrame();

    //hardware scan
    bool m_hardwareFound;

    XsensMVNPrivate(const XsensMVNPrivate&) = delete;
    XsensMVNPrivate& operator=(const XsensMVNPrivate&) = delete;

public:
    XsensMVNPrivate();
    virtual ~XsensMVNPrivate();

    bool init(yarp::os::Searchable&);
    bool fini();

    bool startAcquisition();
    bool stopAcquisition();


    // callbacks
    virtual void onHardwareReady(XmeControl* dev);
    virtual void onHardwareError(XmeControl* dev);
    virtual void onPoseReady(XmeControl* dev);

};



#endif //XSENSMVNPRIVATE_h
