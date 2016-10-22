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

class XmeControl;
class XsensCallbackHandler;

namespace yarp {
    namespace os {
        class Searchable;
    }
}

class yarp::dev::XsensMVN::XsensMVNPrivate
{
    XmeLicense m_license;
    XmeControl *m_connection;
    XsensCallbackHandler *m_callback;

    XsensMVNPrivate(const XsensMVNPrivate&) = delete;
    XsensMVNPrivate& operator=(const XsensMVNPrivate&) = delete;

public:
    XsensMVNPrivate();
    ~XsensMVNPrivate();

    bool init(yarp::os::Searchable&);
    bool fini();

    void scanForDevice(std::function<void ()> callback);


};

class XsensCallbackHandler : public XmeCallback {
public:
    volatile bool m_calibrationComplete;
    volatile bool m_calibrationProcessed;
    volatile bool m_calibrationAborted;

    virtual ~XsensCallbackHandler();

protected:
   
};


#endif //XSENSMVNPRIVATE_h
