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
#include "build_log.hpp"
#include "utils.hpp"
#include "wait.hpp"
/////////////////////////////////////////////////////////////////////////
// STD 
#include <time.h>
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
void clear_screen_linux ()
{
  std::cout << "\x1b[0;0H\x1b[2J";
}

/////////////////////////////////////////////////////////////////////////
void move_up_linux ( size_t num )
{
  std::cout << "\x1b[" << num << "A";
}

/////////////////////////////////////////////////////////////////////////
void print_date ()
{
  time_t epoch_seconds = time ( NULL );

	std::cout << "Last update: "
            << asctime ( localtime ( &epoch_seconds ) ) ; 
}

/////////////////////////////////////////////////////////////////////////
magrit::log::log ( generic_command* previous_subcommand )
  : generic_command ( previous_subcommand, true ),
    _log_options ("Log options")
{
  _log_options.add_options()
    ( "watch,w","activates the automatic refresh" );

  get_options().add ( _log_options );
}

/////////////////////////////////////////////////////////////////////////
const char*
magrit::log::get_name() const
{
  if ( is_executable() )
  {
    return "magrit-build-log";
  }
  else
  {
    return "log"; 
  }
} 

/////////////////////////////////////////////////////////////////////////
const char* magrit::log::get_description() const
{
  return "Shows the status of the commits sent to the server";
}

/////////////////////////////////////////////////////////////////////////
void
magrit::log::process_parsed_options
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

  if ( vm.count ( "watch" ) )
  {
    watch_status ( unrecognized_arguments );
  }
  else
  {
    print_status ( unrecognized_arguments );
  }
}

/////////////////////////////////////////////////////////////////////////
void
magrit::log::watch_status ( const std::vector < std::string >& rev_args )
const
{
  clear_screen_linux();

  while ( true )
  {
    auto sha1s = get_commits ( rev_args );

    get_status
    (
      rev_args,
      [&] ( const std::string& commit_desc, const std::string& status )
      {
        print_status_line ( commit_desc, status, color ); 
      }
    );

    print_date();

    magrit::wait::wait_for ( "SEP", 0, sha1s, true );
    
    move_up_linux ( sha1s.size() + 1 );
  }
}

/////////////////////////////////////////////////////////////////////////
void
magrit::log::get_status
( 
  const std::vector < std::string >& rev_args,
  std::function
    <void (const std::string& commit_desc,const std::string& status)> func 
) const
{
  std::vector < std::string > build_status_command 
  { 
    "status", get_repo_name(), "-"
  };

  magrit::send_commit_status_command
  (
    rev_args, 
    build_status_command,
    func,
    color
  );
}

/////////////////////////////////////////////////////////////////////////
void
magrit::log::print_status ( const std::vector < std::string >& rev_args )
const
{
  get_status
  (
    rev_args, 
    [&] ( const std::string& commit_desc, const std::string& status )
    {
      print_status_line ( commit_desc, status, color );
    }
  );
}


