// Copyright 2020 Your Name <your_email>

#include <async++.h>
#include <iostream>
#include <boost/thread.hpp>
#include <boost/thread/thread.hpp>
#include <string>
#include <boost/process.hpp>


#ifndef INCLUDE_BUILDER_HPP_
#define INCLUDE_BUILDER_HPP_

class Builder {
public:
    Builder(std::string config, bool install, bool pack, int32_t timeout);

    void Stop(async::cancellation_token &c);

    bool NewTask(std::string task);


private:
    std::string _config;
    bool _install;
    bool _pack;
    int32_t _timeout;
    boost::process::child _child;
};

#endif // INCLUDE_BUILDER_HPP_
