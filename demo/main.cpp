#include <Builder.hpp>

#include <iostream>
#include <boost/program_options.hpp>

using namespace boost::program_options;

int main(int argc, char **argv) {
    std::string install_usage = "добавляем этап установки (в директорию _install)";
    std::string pack_usage = "добавляем этап упаковки (в архив формата tar.gz))";

    std::string usage = \
 R"(Usage: builder [options]
    Allowed options:
  --help                        : выводим вспомогательное сообщение
  --config <Release|Debug>      : указываем конфигурацию сборки (по умолчанию Debug)
  --install                     : добавляем этап установки (в директорию _install)
  --pack                        : добавляем этап упаковки (в архив формата tar.gz)
  --timeout <count>             : указываем время ожидания (в секундах)";

    options_description desc("General options");
    std::string task_type;
    desc.add_options()
            ("help", usage.c_str())
            ("config", value<std::string>()->default_value("Debug"))
            ("install", install_usage.c_str())
            ("pack", pack_usage.c_str())


            ("timeout", value<int32_t>()->default_value(-1));
    variables_map vm;
    try {
        parsed_options parsed = command_line_parser(argc, argv).options(desc).allow_unregistered().run();
        store(parsed, vm);

        if (vm.count("help")) {
            std::cout << usage << std::endl;
            return 0;
        }

        std::string config = vm["config"].as<std::string>();
        bool install = false;
        bool pack = false;
        uint32_t timeout = vm["timeout"].as<int32_t>();;

        if (vm.count("install"))
            install = true;

        if (vm.count("pack"))
            pack = true;
        try {
            Builder(config, install, pack, timeout);
        }
        catch (std::exception &ex) {
            std::cout << ex.what() << std::endl;
            return 1;
        }
    }
    catch (std::exception &ex) {
        std::cout << usage << std::endl;
        return 1;
    }

}

