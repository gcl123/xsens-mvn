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

XsensCallbackHandler::~XsensCallbackHandler() {}

yarp::dev::XsensMVN::XsensMVNPrivate::XsensMVNPrivate()
: m_connection(0) 
{}

yarp::dev::XsensMVN::XsensMVNPrivate::~XsensMVNPrivate() {}

bool yarp::dev::XsensMVN::XsensMVNPrivate::init(yarp::os::Searchable &config)
{
    if (m_connection) return false;
  
    m_connection = XmeControl::construct();
    m_callback = new XsensCallbackHandler();
    if (!m_connection || ! m_callback) return false;

    m_connection->addCallbackHandler(m_callback);
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

    m_connection->setConfiguration(configuration.c_str());

    //This call is asynchronous. 
    //Add sleeps to wait for the device
    m_connection->setScanMode(true);
    
 
}

bool yarp::dev::XsensMVN::XsensMVNPrivate::fini()
{
    if (m_callback) {
        m_connection->addCallbackHandler(0);
        delete m_callback;
        m_callback = 0;
    }

    if (m_connection) {
        m_connection->destruct();
        m_connection = 0;
    }
}
