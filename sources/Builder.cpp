// Copyright 2020 Your Name <your_email>
#include <Builder.hpp>

namespace bp = boost::process;

Builder::Builder(std::string config, bool install, bool pack, int32_t timeout)
        : _config(config), _install(install), _pack(pack), _timeout(timeout) {
    if (_timeout == 0) {
        std::cout << "Timeout = 0" << std::endl;
        return;
    }

    if (_timeout != -1) {
        auto timer = async::spawn([this] {
            std::cout << "Timer started" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(_timeout));
            if (_child.running()) {
                std::cout << "Time is exceed."
                          << std::endl << "Terminating" << std::endl;
                _child.terminate();
            }
        });
    }

    bool success =
            NewTask("-H. -B_builds -DCMAKE_INSTALL_PREFIX=_install "
                    "-DCMAKE_BUILD_TYPE="
                    + _config);
    if (!success)
        return;
    // build
    success = NewTask("--build _builds");
    if (!success)
        return;

    if (_install && _pack) {
        success = NewTask("--build _builds --target install");
        if (!success)
            return;

        success = NewTask("--build _builds --target package");
        if (!success)
            return;

    } else if (_install && !_pack) {
        success = NewTask("--build _builds --target install");
        if (!success)
            return;

    } else if (!_install && _pack) {
        success = NewTask("--build _builds --target package");
        if (!success)
            return;
    }
}

bool Builder::NewTask(std::string task) {
    bp::ipstream stream;
    auto cmake_path = boost::process::search_path("cmake");


    std::cout << std::endl << std::endl
              << "Executing Task : " << std::endl << task
              << std::endl << std::endl;

    bp::child child(cmake_path.string() + " " + task,
                    boost::process::std_out > stream);
    _child = std::move(child);

    for (std::string line;
         _child.running() && std::getline(stream, line);) {
        std::cout << line << std::endl;
    }

    _child.wait();

    auto exit_code = _child.exit_code();

    if (exit_code != 0) {
        std::cout << "Non zero exit code. Exiting..." << std::endl;
        return false;
    } else {
        return true;
    }
}
