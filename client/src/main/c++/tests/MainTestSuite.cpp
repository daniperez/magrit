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
/////////////////////////////////////////////////////////////////////////

// This can be set at runtime:
//
// e.g.: ./TestSuite --output_format=XML --report_level=detailed
//                   --show_progress=yes
//
struct MyConfig
{
    MyConfig()
    {
        boost::unit_test::unit_test_log.set_threshold_level
        (
            boost::unit_test::log_successful_tests
        ); 

        boost::unit_test::unit_test_log.set_format( boost::unit_test::CLF );
    }

    ~MyConfig()
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
/////////////////////////////////////////////////////////////////////////
BOOST_GLOBAL_FIXTURE( MyConfig )
BOOST_AUTO_TEST_SUITE( MainTestSuite )
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE( SanityCheck01 )
{
  BOOST_CHECK( 1 == (2-1) );
}

/////////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE( CheckBuildCat )
{
  //auto handle = save_output ();

  magrit::build_cat cmd ( nullptr );

  std::vector<std::string> args = {"--dryrun"};

  cmd.run ( args ); 

  //std::string output = get_output ( handle );

  BOOST_CHECK( false );
}


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_SUITE_END()
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

