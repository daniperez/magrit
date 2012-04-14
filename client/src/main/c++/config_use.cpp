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
#include "config_use.hpp"
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
magrit::config_use::config_use ( generic_command* previous_subcommand )
  : generic_command ( previous_subcommand ),
     _config_use_pos_options
    ("Positional options:")

{
  _config_use_pos_options.add_options ()
  (
    "alias",
    boost::program_options::value<std::string>(),
      "Configuration to start using"
  );

  generic_command::get_options().add ( _config_use_pos_options );
}

/////////////////////////////////////////////////////////////////////////
const char*
magrit::config_use::get_name() const
{
  return "use"; 
} 

/////////////////////////////////////////////////////////////////////////
const char* magrit::config_use::get_description() const
{
  return "Starts using the given configuration";
}

/////////////////////////////////////////////////////////////////////////
void
magrit::config_use::process_parsed_options
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

  if ( unrecognized_arguments.size() > 1 )
  {
    throw missing_option
    ( "config use only accepts 1 parameter: alias" );
  }
  else if ( unrecognized_arguments.size() == 1 )
  {
    alias = unrecognized_arguments[0];
  }
  else 
  {
    use_config ( alias ); 
  }
}

/////////////////////////////////////////////////////////////////////////
void
magrit::config_use::use_config
(
  const std::string& alias
)
{
 magrit::utils::process::git
 (
   std::vector< std::string > { "config", "magrit.remote", alias },
   bp_close(),
   bp_silent(),
   bp_inherit(),
   [&] ( const std::string& line ){},
   true 
 );
}
