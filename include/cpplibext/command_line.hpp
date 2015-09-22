/*
 * command_line.hpp file
 *
 * Copyright (C) 2014 Lukas Hermanns
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef _CPPLIBEXT_COMMAND_LINE_H_
#define _CPPLIBEXT_COMMAND_LINE_H_


#include <string>
#include <vector>


namespace ext
{


template <typename String>
class basic_command_line
{

    public:

        using string_type = String;

        class argument
        {

            public:

                argument(string_type val, std::vector<string_type> opts) :
                    value_  ( std::move(val)  ),
                    options_( std::move(opts) )
                {
                }

                argument(argument&& other) :
                    value_  ( std::move(other.value_)   ),
                    options_( std::move(other.options_) )
                {
                }

                const string_type& value() const
                {
                    return value_;
                }

                const std::vector<string_type>& options() const
                {
                    return options_;
                }

            private:

                string_type value_;
                std::vector<string_type> options_;

        };

        basic_command_line(std::vector<argument>&& args) :
            args_( std::move(args) )
        {
        }

        basic_command_line(basic_command_line&& other) :
            args_( std::move(other.args_) )
        {
        }

        string_type name() const
        {
            return args_.empty() ? string_type() : args_.front().value();
        }

        const std::vector<argument>& arguments() const
        {
            return args_;
        }

    private:

        std::vector<argument> args_;

};

using command_line = basic_command_line<std::string>;
using wcommand_line = basic_command_line<std::wstring>;


template <typename T>
class command_line_grammar
{

    public:
        
        using char_type = T;

        static bool is_token_option(char_type chr)
        {
            return chr == char_type('-');
        }
        static bool is_token_pipe(char_type chr)
        {
            return chr == char_type('|');
        }

};


template <
    typename String,
    typename Grammar = command_line_grammar<typename String::value_type>
>
class basic_command_line_parser
{

    public:
        
        using string_type       = String;
        using char_type         = typename String::value_type;
        using grammar_type      = Grammar;
        using command_line_type = basic_command_line<string_type>;
        using argument_type     = typename command_line_type::argument;

        /*command_line_type parse(const string_type& commandLine)
        {
        }*/

        command_line_type parse(const std::vector<string_type>& arguments)
        {
            if (!arguments.empty())
            {
                arg_it_ = arguments.begin();
                arg_it_end_ = arguments.end();

                std::vector<argument_type> params;
                while (!finished())
                    parse_argument(params);

                return command_line_type(std::move(params));
            }
            return command_line_type(std::vector<argument_type>());
        }

        command_line_type parse(int argc, char_type* argv[])
        {
            std::vector<string_type> args(argc);
            while(argc-- > 0)
                args[argc] = argv[argc];
            return parse(args);
        }

    private:
        
        bool parse_option(std::vector<string_type>& opts)
        {
            if (!token().empty() && grammar_type::is_token_option(token().front()))
            {
                opts.push_back(accept().substr(1));
                return true;
            }
            return false;
        }

        void parse_argument(std::vector<argument_type>& params)
        {
            string_type value = accept();
            std::vector<string_type> opts;

            while (!finished() && parse_option(opts))
            {
                // empty loop
            }

            params.emplace_back(argument_type(std::move(value), std::move(opts)));
        }

        bool finished() const
        {
            return !(arg_it_ != arg_it_end_);
        }

        void accept_it()
        {
            ++arg_it_;
        }

        string_type accept()
        {
            auto tkn = token();
            accept_it();
            return tkn;
        }

        const string_type& token() const
        {
            return *arg_it_;
        }

        typename std::vector<string_type>::const_iterator arg_it_, arg_it_end_;

};

using command_line_parser = basic_command_line_parser<std::string>;
using wcommand_line_parser = basic_command_line_parser<std::wstring>;


} // /namespace ext


#endif


