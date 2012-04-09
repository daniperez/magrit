/**
 * Copyright 2011 Frederic Menou
 *
 * This file is part of Magrit.
 *
 * Magrit is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Magrit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public
 * License along with Magrit.
 * If not, see <http://www.gnu.org/licenses/>.
 */
/////////////////////////////////////////////////////////////////////////
// MAGRIT 
#include "build_send.hpp"
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
magrit::build_send::build_send( generic_command* previous_subcommand )
  : generic_command ( previous_subcommand ), _send_options ( "Send options" )
{
  namespace bpo = boost::program_options;

  _send_options.add_options()
    ("--force,f", "<description to be written>")
    ("--command,c", bpo::value<std::string>()->required()
                  , "<description to be written>");

  generic_command::get_options().add ( _send_options );
}

/////////////////////////////////////////////////////////////////////////
const char*
magrit::build_send::get_name() const
{
  if ( is_executable() )
  {
    return "magrit-build-send";
  }
  else
  {
    return "send"; 
  }
} 

/////////////////////////////////////////////////////////////////////////
const char* magrit::build_send::get_description() const
{
  return "Sends a commit to be built";
}

/////////////////////////////////////////////////////////////////////////
void
magrit::build_send::process_parsed_options
(
  const std::vector<std::string>& arguments,
  const boost::program_options::variables_map& vm,
  const std::vector<std::string>& unrecognized_arguments,
  bool allow_zero_arguments
)
const
{
  generic_command::process_parsed_options
    ( arguments, vm, unrecognized_arguments, true );

  send_build
  (
    vm.count("force")>0,
    vm["command"].as<std::string>(),
    unrecognized_arguments
  );
}

/////////////////////////////////////////////////////////////////////////
void
magrit::build_send::send_build
(
  bool force,
  const std::string& command,
  const std::vector < std::string >& rev_args 
)
const
{
  std::vector < std::string > build_send_command 
  { 
    "send-build", force? "--force":"", "--command", command, get_repo_name(), "-"
  };

	auto print_function =
    [&] ( const std::string& commit_desc, const std::string& status )
    {
      if ( status.find_first_of ( "ssh error" ) != std::string::npos )
      {
        throw std::runtime_error ( status );
      }
      else if ( status == "0" )
      {
        print_status_line ( commit_desc, "Skipped", color );
      }
      else
      {
        print_status_line ( commit_desc, "Submitted", color );
      }
    };

  send_commit_status_command
  ( 
    rev_args, 
    build_send_command,
    print_function,
    color
  );
}

