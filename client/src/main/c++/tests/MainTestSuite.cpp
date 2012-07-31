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
///////////////////////////////////////////////////////////////////////////
#define BOOST_TEST_DYN_LINK
// If more tests are added, this must only appear once since it
// creates a main()
#define BOOST_TEST_MODULE main 
///////////////////////////////////////////////////////////////////////////
// BOOST
#include <boost/test/unit_test.hpp>
#include <boost/test/detail/unit_test_parameters.hpp>
/////////////////////////////////////////////////////////////////////////
// MAGRIT 
#include "build_cat.hpp"
#include "build_log.hpp"
/////////////////////////////////////////////////////////////////////////

// This can be set at runtime:
//
// e.g.: ./TestSuite --output_format=XML --report_level=detailed
//                   --show_progress=yes
//
struct my_config
{
    my_config()
    {
        boost::unit_test::unit_test_log.set_threshold_level
        (
            boost::unit_test::log_successful_tests
        ); 

        boost::unit_test::unit_test_log.set_format( boost::unit_test::CLF );
    }

    ~my_config()
    {
    }
};

#include <unistd.h>

/////////////////////////////////////////////////////////////////////////
int save_output ()
{
  return dup ( STDOUT_FILENO );
}

/////////////////////////////////////////////////////////////////////////
std::string get_output ( int handle )
{
  int res = -1;
  std::string output;
  const short buf_size = 256;
  char buf [ buf_size ];

  while ( ( res = read ( handle, buf, buf_size ) ) > 0 )
  {
    output.append ( buf, buf_size );
  }
  std::cerr << "Returning ===============> " << output << std::endl;

  if ( res < 0 )
  {
    throw std::runtime_error 
      (std::string("Error getting the output of a test: ")+strerror(errno));
  }

  res = close ( handle );

  if ( res < 0 )
  {
    throw std::runtime_error 
      (std::string("Error closing test's output copy: ")+strerror(errno));
  }


  return output;
}

/////////////////////////////////////////////////////////////////////////
void check_command
( 
  const magrit::generic_command& cmd,
  const std::vector<std::string>& args, 
  const std::vector<std::string>& expected_output 
)
{
  //auto handle = save_output ();

  cmd.run ( args ); 

  //std::string output = get_output ( handle );

  /*
  check_contains 
  ( 
    output,
    expected_output 
  );
  */
}

/////////////////////////////////////////////////////////////////////////
void check_contains
  ( const std::string& str, const std::vector<std::string>& exprs )
{
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
BOOST_GLOBAL_FIXTURE( my_config )
BOOST_AUTO_TEST_SUITE( main_test_suite )
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE( sanity_check_01 )
{
  BOOST_CHECK( 1 == (2-1) );
}

/////////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE( check_magrit_build_cat )
{
  magrit::build_cat cmd ( nullptr );
  
  std::vector<std::string> args = {"--dryrun"};

  std::vector<std::string> expected =
  {
      "git rev-parse --verify HEAD",
      "ssh -x -p 2022 git@remote.magrit.url magrit cat-build /foo/ d53f7bfafb209e1104ce26c4cf6d4116af96e2df"
  };

  check_command ( cmd, args, expected );
}

/////////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE( check_magrit_build_log )
{
  magrit::log cmd ( nullptr );
  
  std::vector<std::string> args = {"--dryrun"};

  std::vector<std::string> expected =
  {
    "git log --format=%H",
    "ssh -x -p 2022 git@remote.magrit.url magrit status /foo/ -",
    "git log --color=always --oneline -z"
  };

  check_command ( cmd, args, expected );
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_SUITE_END()
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

