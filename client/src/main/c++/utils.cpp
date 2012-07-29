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
#include "utils.hpp"
/////////////////////////////////////////////////////////////////////////
// STD
#include <iomanip>
#include <stdexcept> 
#include <string.h>
#include <iterator>
#include <istream>
// stat()
#include <sys/stat.h>
//////////////////////////////////////////////////////////////////////////
// BOOST
#include <boost/lexical_cast.hpp> 
/////////////////////////////////////////////////////////////////////////

// Only checked once per run for the sake of the speed.
static bool git_ok = false;

#define to_string(x) boost::lexical_cast<std::string>(x)
#define join3(x,y,z) magrit::utils::strings::join(x,y,z)

/////////////////////////////////////////////////////////////////////////
std::string magrit::utils::color::clear_color ()
{
  return std::string ( "\033[0m" ); 
}

/////////////////////////////////////////////////////////////////////////
std::string
colorize_linux ( const std::string& status, bool color )
{
  std::stringstream output;

  for ( size_t i = 0 ; i < status.size() ; ++i )
  {
    switch ( status[i] )
    {
      case 'O':
        output << magrit::utils::color::cool ( status[i], color );
        break;
      case 'E':
        output << magrit::utils::color::error ( status[i], color );
        break;
      case 'R':
        output << magrit::utils::color::running ( status[i], color );
        break;
      case 'P':
        output << magrit::utils::color::pending ( status[i], color );
        break;
      case '?':
        output << magrit::utils::color::warning ( status[i], color );
        break;
      default:
        output << status[i];
        break;
    }
  }

  return output.str();
}

/////////////////////////////////////////////////////////////////////////
std::vector< std::string > magrit::utils::strings::split
  ( const std::string& input, char delimiter )
{
  std::vector< std::string > output;

  std::istringstream iss ( input );

  std::copy
  (
    std::istream_iterator<std::string>(iss),
    std::istream_iterator<std::string>(),
    std::back_inserter ( output )
  );

  return output;
}

/////////////////////////////////////////////////////////////////////////
std::string magrit::utils::process::git_single_output_line
( const std::vector < std::string >& args, bool _throw )
{
  std::string output;

  utils::process::git
  (
    args,
    boost::process::inherit_stream(),
    boost::process::capture_stream(),
    boost::process::inherit_stream(),
    [&output]( const std::string& line ) { output = line; },
    _throw
  );

  return output;
}

/////////////////////////////////////////////////////////////////////////
std::string magrit::utils::config::get_repo_remote_name ()
{
  try
  {
    return
      utils::process::git_single_output_line
      (
        std::vector < std::string > { "config", "--get", "magrit.remote" },
        true
      );
  }
  catch (...)
  {
    return "magrit";
  }
}

/////////////////////////////////////////////////////////////////////////
std::string magrit::utils::config::get_magrit_connection_info ()
{
  return get_magrit_user() + std::string("@") + get_magrit_host();
}

/////////////////////////////////////////////////////////////////////////
std::string magrit::utils::config::get_magrit_url ()
{
  std::string var
    = std::string ( "remote." ) + get_repo_remote_name () + std::string ( ".url" );

  std::string url 
    = utils::process::git_single_output_line
      ( 
        std::vector < std::string > { "config", "--local", var },
        false
      );

  if ( url.empty () )
  {
    throw std::runtime_error
    ( 
      var + " variable not set. Did you use 'magrit config add <name>' ?"
    );
  }
  else
  {
    return url;
  }
}

/////////////////////////////////////////////////////////////////////////
std::string magrit::utils::config::get_repo_name ()
{
  std::string url = get_magrit_url ();

  size_t pos = url.find_last_of ( '/' );

  if ( pos == std::string::npos || pos == url.size() - 1 )
  {
    throw std::runtime_error
    (
      url + ": malformed url, no repo name supplied"
    );
  }

  return std::string ("/") + url.substr ( pos + 1 ) + std::string ("/");
}

/////////////////////////////////////////////////////////////////////////
std::string get_magrit_host_impl ( size_t* port = nullptr )
{
  std::string url = magrit::utils::config::get_magrit_url ();

  size_t pos_at = url.find_first_of ( '@' );

  if ( pos_at == std::string::npos )
  {
    throw std::runtime_error ( url + ": malformed url, no user supplied" );
  }

  size_t pos_colon = url.find_first_of ( ':', pos_at );
  size_t pos_slash = url.find_first_of ( '/', pos_at );

  size_t end = 0;

  if ( pos_slash == std::string::npos )
  {
    throw std::runtime_error ( url + ": malformed url, no repo name supplied" );
  }
  else if ( pos_colon != std::string::npos  )
  {
    if ( pos_colon < pos_slash )
    {
      if ( port != nullptr )
      {
        *port = boost::lexical_cast<size_t> 
                 ( url.substr ( pos_colon + 1, pos_slash - pos_colon - 1 ) );
      }

      end = pos_colon;
    }
    else
    {
      throw std::runtime_error
        ( url + ": malformed url, ':' cannot appear in path" );
    }
  }
  else
  {
    end = pos_slash;
  }

  if ( end - pos_at <= 1 )
  {
    throw std::runtime_error ( url + ": malformed url, no host supplied" );
  }

  return url.substr ( pos_at + 1, end - pos_at - 1 );
}

/////////////////////////////////////////////////////////////////////////
std::string magrit::utils::config::get_magrit_host ()
{
  return get_magrit_host_impl ( nullptr );
}

/////////////////////////////////////////////////////////////////////////
size_t magrit::utils::config::get_magrit_port ()
{
  size_t port = 2022;

  get_magrit_host_impl ( &port );

  return port;
}

/////////////////////////////////////////////////////////////////////////
std::string magrit::utils::config::get_magrit_user ()
{
  return "git";
}

/////////////////////////////////////////////////////////////////////////
int magrit::utils::config::get_message_max_width ()
{
  // 60 correspond to the messsage and 9 to the sha1 signature and spaces
  int width = 60 + 9;

  utils::process::git
  (
    std::vector < std::string >
    {
      "config", "--get", "--int", "magrit.log.maxWidth"
    },
    boost::process::inherit_stream(),
    boost::process::capture_stream(),
    boost::process::inherit_stream(),
    [&width]( const std::string& line )
    {
      width = boost::lexical_cast<int> ( line );
    },
    false
  );

  return std::max ( 20, std::min ( 100, width ) );
}

/////////////////////////////////////////////////////////////////////////
std::string magrit::utils::strings::cut_message ( const std::string& msg, size_t width )
{
  if ( msg.size() <= width )
  {
    return msg;
  }
  else
  {
    std::string output ( msg, 0, width - 1 );

    output += "\u2026";

    return output;
  }
}

/////////////////////////////////////////////////////////////////////////
std::vector < std::string >
fix_args ( const std::string& program, const std::vector < std::string >& args )
{
  std::vector < std::string > workaround_args;

  // NOTE: bug in boost::process : doc says arguments to be passed
  //       but execve used by boost::process needs of the whole
  //       command line (executable included).
  workaround_args.push_back ( program );
  workaround_args.insert
    ( workaround_args.end(), args.begin(), args.end() );

  workaround_args.erase
  (
    std::remove_if
    (
      workaround_args.begin(), workaround_args.end(),
      [] ( const std::string& arg ) { return arg.empty(); }
    ),
    workaround_args.end()
  );

  return workaround_args;
}

/////////////////////////////////////////////////////////////////////////
int magrit::utils::process::launch
(
  const std::string& program,
  const std::vector< std::string >& arguments,
  boost::process::stream_behavior _stdin,
  boost::process::stream_behavior _stdout,
  boost::process::stream_behavior _stderr,
  std::function<void (std::string&)> line_processor,
  bool _throw
)
{
  boost::process::context context;

  context.stdin_behavior = _stdin;
  context.stdout_behavior = _stdout;
  context.stderr_behavior = _stderr;

  std::vector < std::string > workaround_args 
    = fix_args ( program, arguments );

  boost::process::child ch
    = boost::process::launch
      (
        boost::process::find_executable_in_path ( program ),
        workaround_args, context
      );

  if ( context.stdout_behavior.get_type()
      != boost::process::silence_stream().get_type() )
  {
    boost::process::pistream& is = ch.get_stdout();

    std::string line; 

    while ( std::getline ( is, line ) )
    {
      line_processor ( line );
    }
    if ( !is.eof() )
    {
      throw std::runtime_error
            ( "An error occurred before reading all the stdout" );
    }
  }

  auto status = ch.wait();

  if ( !status.exited() )
  {
    // Warning: errno is not guaranteed to be wait's one.
    throw std::runtime_error ( strerror ( errno ) );
  }
  else if ( status.exit_status () != 0 && _throw )
  {
    throw std::runtime_error
    ( 
      "[" + program +" "+
      join3 (" ",  arguments.begin(), arguments.end()) + "]"
      " returned " + to_string ( status.exit_status() ) +
      " (" + strerror(errno) + ")"
    );
  }

  return status.exit_status(); 
}

/////////////////////////////////////////////////////////////////////////
int magrit::utils::process::ssh
(
  size_t port,
  const std::string& conn_info,
  const std::vector< std::string >& arguments,
  boost::process::stream_behavior _stdin,
  boost::process::stream_behavior _stdout,
  boost::process::stream_behavior _stderr,
  std::function<void (std::string&)> line_processor,
  bool _throw
)
{
  return
    launch
    (
      "ssh",
      std::vector < std::string >
      {
        "-x", "-p", to_string ( port ), conn_info,
        join3 ( " ", arguments.begin(), arguments.end() )
      },
      _stdin, _stdout, _stderr, line_processor, _throw
    );
}

/////////////////////////////////////////////////////////////////////////
boost::process::children magrit::utils::process::launch_pipeline
(
  const std::vector < boost::process::pipeline_entry >& pipeline
)
  throw ( pipeline_error )
{
  boost::process::children ch 
    = boost::process::launch_pipeline ( pipeline );

  boost::process::status exit_status
    = boost::process::wait_children ( ch ); 

  if ( !exit_status.exited () || exit_status.exit_status() != 0 )
  {
    std::vector < std::string > pipeline_elem;

    for ( size_t i = 0 ; i < pipeline.size() ; ++i )
    {
      auto args = pipeline[i].arguments;
      pipeline_elem.push_back ( join3 (" ", args.begin(), args.end() ) );
    }

    throw pipeline_error ( pipeline_elem );
  }

  return ch; 
}

/////////////////////////////////////////////////////////////////////////
boost::process::pipeline_entry magrit::utils::process::create_pipeline_entry
(
  const std::string& program,
  const std::vector< std::string >& arguments,
  boost::process::stream_behavior _stdin,
  boost::process::stream_behavior _stdout,
  boost::process::stream_behavior _stderr
)
{
  boost::process::context context;

  context.stdin_behavior = _stdin;
  context.stdout_behavior = _stdout;
  context.stderr_behavior = _stderr;

  std::vector < std::string > workaround_args 
    = fix_args ( program, arguments );

  return
    boost::process::pipeline_entry
    (
      boost::process::find_executable_in_path ( program ),
      workaround_args,
      context
    );
}

/////////////////////////////////////////////////////////////////////////
void
magrit::utils::strings::print_status_line
  ( const std::string& desc, const std::string& status, bool color )
{
  auto msg_width
    = color? utils::config::get_message_max_width () + 8
           : utils::config::get_message_max_width();

  std::cout 
    << std::left << std::setw ( msg_width )
    << utils::strings::cut_message ( desc, msg_width ) << " | "
    << colorize_linux ( status , color )
    << std::endl;

}

/////////////////////////////////////////////////////////////////////////
void
magrit::utils::process::send_command
(
  const std::vector < std::string >& git_rev_args,
  const std::vector < std::string >& magrit_command,
  std::function
    <void (const std::string& commit_desc,const std::string& status)> func,
  bool color
)
{
  std::vector < boost::process::pipeline_entry > pipeline;

  pipeline.push_back ( git::get_commits_pipeline_entry ( git_rev_args ) );
  
  pipeline.push_back
  ( 
    utils::process::create_pipeline_entry
    (
      "ssh",
      std::vector < std::string >
      {
        "-x", "-p", to_string ( utils::config::get_magrit_port() ),
        utils::config::get_magrit_connection_info(), "magrit",
        join3 ( " ", magrit_command.begin(), magrit_command.end() )
      },
      bp_close(), bp_capture(), bp_inherit()
    )
  );

  boost::process::children statuses = launch_pipeline ( pipeline );

  // We issue again a git log. For every line, we print the status
  // previously fetched from server. Note: it's theoretically possible
  // that the previous git log had less lines than the following one if
  // a commit was pushed in between, but in practice the odds are very low
  // and the impact is very small.
  git
  (
    std::vector < std::string >
    {
      "log", color?"--color=always":"--color=never", "--oneline", "-z",
      join3 ( " ", git_rev_args.begin(), git_rev_args.end() )
    },
    bp_inherit(), bp_capture(), bp_inherit(),
    [&]( const std::string& line )
    { 
      std::string status;
      std::getline( statuses.back().get_stdout(), status );
      func ( line, status );
    },
    true
  );
}

/////////////////////////////////////////////////////////////////////////
void
magrit::utils::process::send_command_explicit_args
(
  const std::vector < std::string >& git_rev_args,
  const std::vector < std::string >& magrit_command,
  std::function
    <void (const std::string& commit_desc,const std::string& status)> func,
  bool color
)
{
  std::vector < std::string > sha1s
    = utils::git::get_commits ( git_rev_args );

  auto sha1s_it = sha1s.begin();

  ssh
  (
    utils::config::get_magrit_port(),
    utils::config::get_magrit_connection_info(),
    std::vector < std::string >
    {
      join3 ( " ", magrit_command.begin(), magrit_command.end() ),
      " ",
      join3 ( " ", sha1s.begin(), sha1s.end() ),
    },
    bp_close(), bp_inherit(), bp_inherit(),
    [&sha1s_it,&func] ( const std::string& line )
    {
      func ( *(sha1s_it++), line );
    }
  );
}

/////////////////////////////////////////////////////////////////////////
boost::process::pipeline_entry
magrit::utils::git::get_commits_pipeline_entry
  ( const std::vector< std::string >& rev )
{

  std::vector < std::string > git_log_arguments
  {
    "log", "--format=%H", join3 ( " ", rev.begin(), rev.end() )
  };

  return
    process::create_git_pipeline_entry
      ( git_log_arguments, bp_close(), bp_close(), bp_inherit() );
}

/////////////////////////////////////////////////////////////////////////
std::vector<std::string> magrit::utils::git::get_commits
  ( const std::vector<std::string>& git_args )
{
  std::vector<std::string> output;

  std::vector < std::string > git_log_arguments
  {
    "log", "--format=%H", join3 ( " ", git_args.begin(), git_args.end() )
  };

  utils::process::git
  (
    git_log_arguments,
    bp_close(), bp_capture(), bp_inherit(),
    [&output] ( const std::string& line )
    {
      output.push_back ( line );
    },
    true
  );

  return output;
}

/////////////////////////////////////////////////////////////////////////
void magrit::utils::git::check_sanity () 
{
  if ( !git_ok )
  {
    // git commands don't return any useful return code. Only 
    // git rev-parse does, that's why we check if we're in a git
    // folder before executing any git command.
    int status
      = utils::process::launch 
        (
          "git",
          std::vector<std::string>{"rev-parse"},
          bp_close(), bp_capture(), bp_capture(),
          [] ( const std::string& line ) {},
          false
        );
    
    if ( status == 128 )
    {
      throw std::runtime_error ( "not in a git folder" );
    }
    else {

      git_ok = true;

      return;
    }
  }
}

/////////////////////////////////////////////////////////////////////////
std::string magrit::utils::git::rev_parse
  ( const std::vector< std::string >& arguments )
{

  std::vector < std::string > all_arguments
  {
    "rev-parse",
    "--verify",
    join3 ( " ", arguments.begin(), arguments.end() )
  };

  return
    utils::process::git_single_output_line
    ( 
      all_arguments,
      true
    );
}

/////////////////////////////////////////////////////////////////////////
int magrit::utils::process::git
(
  const std::vector< std::string >& arguments,
  boost::process::stream_behavior _stdin,
  boost::process::stream_behavior _stdout,
  boost::process::stream_behavior _stderr,
  std::function<void (std::string&)> line_processor,
  bool _throw
)
{

  utils::git::check_sanity ();
  
  return
    utils::process::launch
      ( "git", arguments, _stdin, _stdout, _stderr, line_processor, _throw );
}

/////////////////////////////////////////////////////////////////////////
boost::process::pipeline_entry magrit::utils::process::create_git_pipeline_entry
(
  const std::vector< std::string >& arguments,
  boost::process::stream_behavior _stdin,
  boost::process::stream_behavior _stdout,
  boost::process::stream_behavior _stderr
)
{

  utils::git::check_sanity ();

  return
    create_pipeline_entry ( "git", arguments, _stdin, _stdout, _stderr );
}

