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
#ifndef __MAGRIT_SEND__
#define __MAGRIT_SEND__
///////////////////////////////////////////////////////////////////////////
// MAGRIT 
#include "generic_command.hpp"
///////////////////////////////////////////////////////////////////////////

namespace magrit 
{
  class build_send : public generic_command
  {
    public:

      build_send ( generic_command* previous_subcommand );

      /**
       * @see generic_command::get_name
       */
      const char* get_name() const override;

      /**
       * @see generic_command::get_description
       */
      const char* get_description() const override;

      /**
       * Sends the commits describeds by rev_args.
       */ 
      void
      send_build
      (
        bool force,
        const std::string& command,
        const std::vector < std::string >& rev_args 
      )
      const;

    protected:

      void
      process_parsed_options
      (
        const std::vector<std::string>& arguments,
        const boost::program_options::variables_map& vm,
        const std::vector<std::string>& unrecognized_arguments,
        bool allow_zero_arguments
      )
      const;

    protected:

      boost::program_options::options_description _send_options;
  };
}
#endif
