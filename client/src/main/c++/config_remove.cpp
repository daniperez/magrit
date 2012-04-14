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
#include "config_remove.hpp"
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
magrit::config_remove::config_remove ( generic_command* previous_subcommand )
  : generic_command ( previous_subcommand ),
     _config_remove_pos_options
    ("Positional options:")

{
  _config_remove_pos_options.add_options ()
  (
    "alias",
    boost::program_options::value<std::string>(),
      "Configuration to remove"
  );

  generic_command::get_options().add ( _config_remove_pos_options );
}

/////////////////////////////////////////////////////////////////////////
const char*
magrit::config_remove::get_name() const
{
  return "rm"; 
} 

/////////////////////////////////////////////////////////////////////////
const char* magrit::config_remove::get_description() const
{
  return "Removes a magrit configuration";
}

/////////////////////////////////////////////////////////////////////////
void
magrit::config_remove::process_parsed_options
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

  if ( unrecognized_arguments.size() != 1 )
  {
    throw missing_option
    ( "config rm needs 1 positional parameters: alias" );
  }
  else
  {
    remove_config ( unrecognized_arguments[0] ); 
  }
}

/////////////////////////////////////////////////////////////////////////
void
magrit::config_remove::remove_config
(
  const std::string& alias
)
{
 magrit::utils::process::git
 (
   std::vector< std::string > { "remote", "rm", alias },
   bp_close(),
   bp_silent(),
   bp_inherit(),
   [&] ( const std::string& line ){},
   true 
 );
}
