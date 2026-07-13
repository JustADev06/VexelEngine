#include <fstream>
#include <iostream>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <regex>

int main(int argc, char **argv)
{
    if(argc > 1)
    {
        std::vector<std::string> _func_names(argc - 1, "");
        std::vector<std::string> _func_proc_names(argc - 1, ""); // We expect -hooks along with the next argument being the include directory, to be provided
        std::string include_dir = "";
        bool look_for_flags = true;
        bool include_directory_provided = false;
        bool next_include_dir = false;
        for(auto i = 1u; i < argc; i++)
        {
            if(look_for_flags)
            {
                if(auto str = std::string(argv[i]); str.find('-') != std::string::npos)
                {
                    if(!include_directory_provided)
                    {
                        if(str == "-include")
                        {
                            include_directory_provided = true;
                            next_include_dir = true;
                            continue;
                        }
                    }
                }
            }

            if(next_include_dir)
            {
                include_dir = argv[i];
                next_include_dir = false;
                continue;
            }

            _func_names[i - 1] = argv[i];
            _func_proc_names[i - 1] = argv[i];
            std::transform(_func_proc_names[i-1].cbegin(), _func_proc_names[i-1].cend(), _func_proc_names[i-1].begin(), [](auto c){ return std::toupper(c); });
            _func_proc_names[i-1].insert(0, "PFN");
            _func_proc_names[i-1].append("PROC");
        }

        if(!include_directory_provided)
        {
            std::cout << "No include directory provided, please provide the include directory where you intent to put the gl_hooks.h file!\n";
            return -1;
        }

        std::vector<std::string> func_names(argc - 3, "");
        std::vector<std::string> func_proc_names(argc - 3, "");

        auto index = 0u;
        for(auto i = 0u; i < (argc - 1); i++)
        {
            if(!_func_names[i].empty())
            {
                func_names[index] = _func_names[i];
                func_proc_names[index] = _func_proc_names[i];
                index++;
            }
        }

        // Imagine hardcoding paths in your code, lol
        std::filesystem::path gl_h_file_path = std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) / std::filesystem::path("../glad/include/glad/gl.h");
        std::ifstream gl_h_file(gl_h_file_path, std::ios::in);

        if(!gl_h_file)
        {
            std::cout << "Failed to open glad.h" << '\n';
            return -1;
        }

        std::stringstream strm{};
        strm << gl_h_file.rdbuf();
        std::string gl_h_file_string = strm.str();

        std::regex r(R"(typedef\s+([^\s]+)\s+\(GLAD_API_PTR\s+\*(.*)\)\(([^)]*)\);)");
        std::regex arg_r(R"(\s*([^,]+))");
        std::regex arg_process_r(R"((.*)\s(.*))");

        std::stringstream gl_hooks_cpp_strm{};
        std::stringstream gl_hooks_header_strm{};

        gl_hooks_header_strm << "#pragma once\n"
                             << '\n'
                             << "#include <string_view>\n"
                             << "#include <unordered_map>\n\n"
                             << "#include \"glad/gl.h\"\n"
                             << '\n'
                             << "namespace " << ENGINE_NAME << '\n'
                             << "{\n"
                             << "    using __Proc = void (*)();\n"
                             << "\n"
                             << "    inline std::unordered_map<std::string_view, std::pair<__Proc, __Proc>> gl_hooks_hooks;\n\n"
                             << "    template<class Func1, class Func2>\n"
                             << "    inline void gl_hooks_set_hook_callback(std::string_view name, Func1 *hook_start, Func2 *hook_end)\n"
                             << "    {\n"
                             << "        gl_hooks_hooks[name] = std::make_pair(reinterpret_cast<__Proc>(hook_start), reinterpret_cast<__Proc>(hook_end));\n"
                             << "    }\n"
                             << '\n'; // We'll now start writing names of the hook functions

        gl_hooks_cpp_strm << "#include <cstdint>\n"
                          << "#include <functional>\n\n"
                          << "#include \"SDL3/SDL.h\"\n"
                          << "#include \"" << (std::filesystem::path(include_dir) / std::filesystem::path("gl_hooks.h")).string() << "\"\n\n"
                          << "namespace " << ENGINE_NAME << '\n' //"#include " /* This should be specified when already copied, sorry */
                          << "{\n"
                          << "    static const std::unordered_map<std::string_view, __Proc> hooked_funcs =\n"
                          << "    {\n";
        for(auto i = 0u; i < (argc - 3); i++)
        {
            gl_hooks_cpp_strm << "        {\"" << func_names[i] << "\", " << "reinterpret_cast<__Proc>(hook_" << func_names[i] << ")}" << ((i == (argc - 4)) ? "\n" : ",\n");
        }
        gl_hooks_cpp_strm << "    };\n";

        gl_hooks_cpp_strm << '\n'; // We'll now start writing the definitions of the functions

        std::vector<std::string> check_vector{}; // This will be used for the check, whether we found all function names provided
        for(std::sregex_iterator it(gl_h_file_string.cbegin(), gl_h_file_string.cend(), r);
            it != std::sregex_iterator{};
            it++)
        {
            const auto &match = *it;
            if(auto func = std::find_if(func_proc_names.cbegin(), func_proc_names.cend(), [&](const std::string &str){ return str == match[2].str(); }); func != func_proc_names.cend())
            {
                auto func_name = func_names[std::distance(func_proc_names.cbegin(), func)];
                bool is_void_ret = match[1] == "void";

                check_vector.push_back(func_name); // For checking later

                if(auto args = match[3].str(); !args.empty())
                {
                    gl_hooks_header_strm << "    inline " << match[1] << " hook_" << func_name << '(' << args << ");\n";

                    gl_hooks_cpp_strm << "    inline " << match[1] << ' ' << "hook_" << func_name << '(' << args << ")\n"
                                      << "    {" << '\n'
                                      << "        static " << (*func) << " sdl_func = reinterpret_cast<" << (*func) << " >(SDL_GL_GetProcAddress(\"" << func_name << "\"));\n"
                                      << "        std::function<void ("; // We'll finish later, after building just the typenames list

                    std::string type_list = "";
                    std::string call_list = "";
                    for(std::sregex_iterator _it(args.cbegin(), args.cend(), arg_r);
                        _it != std::sregex_iterator{};
                        _it++)
                    {
                        auto arg = (*_it)[1].str();
                        std::smatch _match;
                        if(std::regex_match(arg, _match, arg_process_r))
                        {
                            type_list += (_match[1].str() + ",");
                            call_list += (_match[2].str() + ",");
                        }
                    }

                    type_list = type_list.substr(0, type_list.length() - 1);
                    call_list = call_list.substr(0, call_list.length() - 1);

                    gl_hooks_cpp_strm << type_list << ")> hook = reinterpret_cast<void (*)(" << type_list << ")>(gl_hooks_hooks.at(\"" << func_name << "\").first);\n"
                                      << "        std::function<void (" << type_list << ")> end_hook = reinterpret_cast<void (*)(" << type_list << ")>(gl_hooks_hooks.at(\"" << func_name << "\").second);\n"
                                      << '\n'
                                      << "        hook(" << call_list << ");\n";
                                      if(is_void_ret) { gl_hooks_cpp_strm << "        sdl_func(" << call_list << ");\n"; } else { gl_hooks_cpp_strm << "        auto ret_value = sdl_func(" << call_list << ");\n"; }
                    gl_hooks_cpp_strm << "        end_hook ? end_hook(" << call_list << ") : void(0);\n"
                                      << (!is_void_ret ? "        return ret_value;\n" : "")
                                      << "    }\n\n";
                }
                else
                {
                    gl_hooks_header_strm << "    inline " << match[1] << " hook_" << func_name << "();\n";
                    gl_hooks_cpp_strm << "    inline " << match[1] << ' ' << "hook_" << func_name << "()\n"
                                      << "    {\n"
                                      << "        static " << (*func) << " sdl_func = reinterpret_cast<" << (*func) << ">(SDL_GL_GetProcAddress(\"" << func_name << "\"));\n"
                                      << "        std::function<void ()> hook = reinterpret_cast<void (*)()>(gl_hooks_hooks.at(\"" << func_name << "\").first);\n"
                                      << "        std::function<void ()> end_hook = reinterpret_cast<void (*)()>(gl_hooks_hooks.at(\"" << func_name << "\").second);\n"
                                      << '\n'
                                      << "        hook();\n";
                                      if(is_void_ret) { gl_hooks_cpp_strm << "        sdl_func();\n"; } else { gl_hooks_cpp_strm << "        auto ret_value = sdl_func();\n"; }
                    gl_hooks_cpp_strm << "        end_hook ? end_hook() : void(0);\n"
                                      << (!is_void_ret ? "        return ret_value;\n" : "")
                                      << "    }\n\n";
                }
            }
        }

        if(check_vector.size() != (argc - 3))
        {
            std::cout << "Failed to find some function names, check your input names for validity!\n";
            return -1;
        }

        gl_hooks_header_strm << "\n    __Proc gl_hooks_gl_hook_proc(const char *name);\n"
                             << "}\n";

        gl_hooks_cpp_strm << "    __Proc gl_hooks_gl_hook_proc(const char *name)\n"
                          << "    {\n"
                          << "        if(!gl_hooks_hooks.contains(name) || !hooked_funcs.contains(name))\n"
                          << "        {\n"
                          << "            return SDL_GL_GetProcAddress(name);\n"
                          << "        }\n"
                          << "        else\n"
                          << "        {\n"
                          << "            return hooked_funcs.at(name);\n"
                          << "        }\n"
                          << "    }\n"
                          << "}\n";

        std::filesystem::create_directory(std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) / std::filesystem::path("output"));
        auto files_path = std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) / std::filesystem::path("output");
        std::ofstream gl_hooks_header_file(files_path / std::filesystem::path("gl_hooks.h"), std::ios::out);
        std::ofstream gl_hooks_cpp_file(files_path / std::filesystem::path("gl_hooks.cpp"), std::ios::out);

        if(!gl_hooks_header_file)
        {
            std::cout << "Failed to open the gl_hooks.h in the output directory, please ensure that the output directory along with gl_hooks.h exist/s!\n";
            return -1;
        }

        if(!gl_hooks_cpp_file)
        {
            std::cout << "Failed to open the gl_hooks.cpp in the output directory, please ensure that the output directory along with gl_hooks.cpp exist/s!\n";
            return -1;
        }

        gl_hooks_cpp_file << gl_hooks_cpp_strm.str();
        gl_hooks_header_file << gl_hooks_header_strm.str();
    }
    else
    {
        std::cout << "Error: No inputs provided" << '\n';
        return -1;
    }

    return 0;
}
