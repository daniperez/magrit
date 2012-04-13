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
#include "share.hpp"
#include "build_send.hpp"
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
const char*
magrit::share::get_name() const
{
  if ( is_executable() )
  {
    return "magrit-share";
  }
  else
  {
    return "share"; 
  }
} 

/////////////////////////////////////////////////////////////////////////
const char* magrit::share::get_description() const
{
  return "Sends to magrit server all the commits";
}

/////////////////////////////////////////////////////////////////////////
void
magrit::share::process_parsed_options
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
      ( "share only accepts 1 parameter: commit revision" );
  }
  else if ( unrecognized_arguments.size() == 1 )
  {
    rev = unrecognized_arguments[0];
  }

  share_impl ( rev, color );
}

/////////////////////////////////////////////////////////////////////////
void
magrit::share::share_impl ( const std::string& rev, bool color )
{
  std::string sha1 
    = start_git_rev_parse_process ( std::vector < std::string > { rev } );

  start_git_process
  ( 
    std::vector < std::string > 
    {
      "push",
      "--force",
      get_repo_remote_name(),
      sha1
    },
    bp_close(), bp_inherit(), bp_inherit(),
    [] ( const std::string& line ) {},
    true
  );

  magrit::build_send::send_build
    ( false, "", std::vector < std::string > { rev }, color); 
}
