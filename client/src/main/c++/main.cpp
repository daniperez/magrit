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
#ifdef MAGRIT_BUILD_LOG
#include "build_log.hpp"
#elif MAGRIT_SEND_BUILD
#include "build_send.hpp"
#elif MAGRIT_CAT_BUILD
#include "build_cat.hpp"
#elif MAGRIT_WAIT_FOR
#include "wait.hpp"
#elif MAGRIT_MONITOR
#include "monitor.hpp"
#elif MAGRIT_STATUS
#include "status.hpp"
#elif MAGRIT_SHARE
#include "share.hpp"
#elif MAGRIT_CONFIG
#include "config.hpp"
#else
#include "magrit.hpp"
#endif
#include "utils.hpp"
/////////////////////////////////////////////////////////////////////////
// STD 
#include <algorithm>
/////////////////////////////////////////////////////////////////////////

int main ( int argc, char** argv )
{

  #ifdef MAGRIT_BUILD_LOG
  magrit::log ma ( nullptr );
  #elif MAGRIT_SEND_BUILD
  magrit::build_send ma ( nullptr );
  #elif MAGRIT_CAT_BUILD
  magrit::build_cat ma ( nullptr );
  #elif MAGRIT_WAIT_FOR
  magrit::wait ma ( nullptr );
  #elif MAGRIT_MONITOR
  magrit::monitor ma ( nullptr );
  #elif MAGRIT_STATUS
  magrit::status ma ( nullptr );
  #elif MAGRIT_SHARE
  magrit::share ma ( nullptr );
  #elif MAGRIT_CONFIG
  magrit::config ma ( nullptr );
  #else
  magrit::magrit ma;
  #endif

  try
  {
    std::vector<std::string> arguments ( argv, argv+argc );

    ma.run ( arguments );

    return 0;
  } 
  catch ( const magrit::option_not_recognized& e )
  {
    std::cerr << magrit::utils::color::error ( "Error:", ma.color ) 
              << " unknown option: "
              << e.what() << std::endl;
  }
  catch ( boost::program_options::error& e )
  {
    std::cerr << magrit::utils::color::error ( "Error: ", ma.color ) 
              << e.what() << std::endl;
  }
  catch ( std::exception& e )
  {
    std::cerr << magrit::utils::color::error ( "Error: ", ma.color )
              << e.what() << std::endl;
  }

  return -1;
}
