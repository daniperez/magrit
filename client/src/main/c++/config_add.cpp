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
#include "config_add.hpp"
/////////////////////////////////////////////////////////////////////////
// STD 
#include <stdlib.h>
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
magrit::config_add::config_add ( generic_command* previous_subcommand )
  : generic_command ( previous_subcommand ),
     _config_add_options ( "Config add options:" ),
     _config_add_pos_options
    ("Positional options:")

{
  _config_add_options.add_options ()
  ( 
    "alias",
    boost::program_options::value<std::string>()
      ->required()
      ->default_value("magrit"),
    "name to identify the config"
  );

  _config_add_pos_options.add_options ()
  (
    "host",
    boost::program_options::value<std::string>()
      ->default_value("localhost:2022"),
      "host:port where magrit listens to (positional option #1)"
  )
  (
    "name",
    boost::program_options::value<std::string>(),
    "repository name (positional option #2)"
  );

  generic_command::get_options().add ( _config_add_options );
  generic_command::get_options().add ( _config_add_pos_options );
}

/////////////////////////////////////////////////////////////////////////
const char*
magrit::config_add::get_name() const
{
  return "add"; 
} 

/////////////////////////////////////////////////////////////////////////
const char* magrit::config_add::get_description() const
{
  return "Creates a new magrit configuration";
}

/////////////////////////////////////////////////////////////////////////
void
magrit::config_add::process_parsed_options
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

  std::string alias = "magrit";

  if ( vm.count("alias") > 0 )
  {
    alias = vm["alias"].as<std::string>();
  }

  if ( unrecognized_arguments.size() < 2 )
  {
    throw missing_option
    ( "config add needs 2 positional parameters: host repo_name" );
  }
  else if ( unrecognized_arguments.size() > 2 )
  {
    throw option_not_recognized
    ( 
      std::string ("config add only accepts 2 parameters: host repo_name. ") +
      "'" + unrecognized_arguments[2] + "' not recognized."
    );
  }
  else
  {
    add_config ( alias, unrecognized_arguments[0], unrecognized_arguments[1] ); 
  }
}

/////////////////////////////////////////////////////////////////////////
void
magrit::config_add::add_config
(
  const std::string& alias,
  const std::string& host,
  const std::string& project_name
)
{
 const char* user = getenv("LOGNAME");

 if ( user == nullptr )
 {
   throw std::runtime_error ( "$LOGNAME variable not defined" );
 }

 std::string url
  = std::string("ssh://") + user + "@" + host + "/" + project_name;

 magrit::utils::process::git
 (
   std::vector< std::string > { "remote", "add", alias, url },
   bp_close(),
   bp_silent(),
   bp_inherit(),
   [&] ( const std::string& line ){},
   true 
 );
}
