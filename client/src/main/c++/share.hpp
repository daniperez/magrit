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
#ifndef __MAGRIT_SHARE__
#define __MAGRIT_SHARE__
///////////////////////////////////////////////////////////////////////////
// MAGRIT 
#include "generic_command.hpp"
///////////////////////////////////////////////////////////////////////////

namespace magrit
{
  class share : public generic_command
  {
    public:

      share ( generic_command* previous_subcommand )
        : generic_command ( previous_subcommand ) {}

      /**
       * @see generic_command::get_name
       */
      const char* get_name() const override;

      /**
       * @see generic_command::get_description
       */
      const char* get_description() const override;

      /**
       * Shares the commits specified by rev. 
       */
      static void share_impl ( const std::string& rev );

    protected:

      /**
       * @see generic_command::process_parsed_options
       */
      void
      process_parsed_options
      (
        const std::vector<std::string>& arguments,
        const boost::program_options::variables_map& vm,
        const std::vector<std::string>& unrecognized_arguments,
        bool allow_zero_arguments
      )
      const override;
  };
}
#endif





