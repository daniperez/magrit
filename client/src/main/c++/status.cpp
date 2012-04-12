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
#include "status.hpp"
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
const char*
magrit::status::get_name() const
{
  if ( is_executable() )
  {
    return "magrit-status";
  }
  else
  {
    return "status"; 
  }
} 

/////////////////////////////////////////////////////////////////////////
const char* magrit::status::get_description() const
{
  return "<description to be written>";
}

/////////////////////////////////////////////////////////////////////////
void
magrit::status::process_parsed_options
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

  std::string rev = "HEAD";

  if ( unrecognized_arguments.size() > 1 )
  {
    throw option_not_recognized
      ( "status only accepts 1 parameter: commit revision" );
  }
  else if ( unrecognized_arguments.size() == 1 )
  {
    rev = unrecognized_arguments[0];
  }

  print_status ( rev, color );
}

/////////////////////////////////////////////////////////////////////////
void
magrit::status::print_status ( const std::string& rev, bool color )
{
  std::vector < std::string > status_command 
  {
    "status", get_repo_name(), rev 
  };

  auto print_function = 
    [] ( const std::string& commit_desc, const std::string& status )
    {
      //#error This is not implemented
      /*
      echo "# $(git log --color=$_colorAction -1 --oneline $commit)"
      echo "$(_colorizeStatus $status)"*
      */
    };

  send_status_command
  ( 
    std::vector < std::string > { rev }, 
    status_command,
    print_function,
    color
  );
}




