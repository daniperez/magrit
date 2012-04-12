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
#include "monitor.hpp"
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
const char*
magrit::monitor::get_name() const
{
  if ( is_executable() )
  {
    return "magrit-monitor";
  }
  else
  {
    return "monitor"; 
  }
} 

/////////////////////////////////////////////////////////////////////////
const char* magrit::monitor::get_description() const
{
  return "<description to be written>";
}

/////////////////////////////////////////////////////////////////////////
void
magrit::monitor::process_parsed_options
(
  const std::vector<std::string>& arguments,
  const boost::program_options::variables_map& vm,
  const std::vector<std::string>& unrecognized_arguments,
  bool allow_zero_arguments
)
const
{
  if ( arguments.size() != 0 )
  {
    throw magrit::option_not_recognized ( "monitor doesn't accept any options" );
  }

  do_monitor ( get_magrit_port(), get_magrit_connection_info() );
}

/////////////////////////////////////////////////////////////////////////
void
magrit::monitor::do_monitor 
  ( int magrit_port, const std::string& magrit_conn_str )
{
  start_ssh_process
  (
    magrit_port,
    magrit_conn_str,
    std::vector < std::string > { "magrit", "monitor" },
    bp_close(),
    bp_inherit(),
    bp_inherit(),
    [](const std::string& line){},
    true 
  );
}

