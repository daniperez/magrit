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
#include "build_cat.hpp"
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
magrit::build_cat::build_cat ( generic_command* previous_subcommand )
  : generic_command ( previous_subcommand )
{
}

/////////////////////////////////////////////////////////////////////////
const char*
magrit::build_cat::get_name() const
{
  if ( _previous_subcommand == nullptr )
  {
    return "magrit-cat-build";
  }
  else
  {
    return "cat"; 
  }
} 

/////////////////////////////////////////////////////////////////////////
const char* magrit::build_cat::get_description() const
{
  return "<to be written>";
}

/////////////////////////////////////////////////////////////////////////
void
magrit::build_cat::process_parsed_options
(
  const std::vector<std::string>& arguments,
  const boost::program_options::variables_map& vm,
  const std::vector<std::string>& unrecognized_arguments,
  bool allow_zero_arguments
)
const
{
  std::string output;

  magrit::start_git_process
  (
    std::vector < std::string >
    {
      "rev-parse", "--verify",
      unrecognized_arguments.size() > 0 ?
      join
      (
        " ",
        unrecognized_arguments.begin(),
        unrecognized_arguments.end()
      )
      :
      "HEAD"
    },
    boost::process::close_stream(),
    boost::process::capture_stream(),
    boost::process::inherit_stream(),
    [&output] ( const std::string& line )
    {
      output = line;
    },
    true
  );

  magrit::start_ssh_process
  (
    get_magrit_port(),
    get_magrit_connection_info(),
    std::vector < std::string >
    {
      "magrit", "cat-build", get_repo_name(), output
    },
    boost::process::close_stream(),
    boost::process::silence_stream(),
    boost::process::inherit_stream(),
    [] ( const std::string& line )
    {
      std::cout << line << std::endl;
    }
  );
}

