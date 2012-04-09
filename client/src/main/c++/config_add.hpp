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
#ifndef __MAGRIT_CONFIG_ADD__
#define __MAGRIT_CONFIG_ADD__
///////////////////////////////////////////////////////////////////////////
// MAGRIT 
#include "generic_command.hpp"
///////////////////////////////////////////////////////////////////////////

namespace magrit
{
  class config_add : public generic_command
  {
    public:

      config_add ( generic_command* previous_subcommand );

      /**
       * @see generic_command::get_name
       */
      const char* get_name() const override;

      /**
       * @see generic_command::get_description
       */
      const char* get_description() const override;

      /**
       * Adds a new config for the given host and 
       * project_name with the given alias.
       */
      static void add_config
      (
        const std::string& alias,
        const std::string& host,
        const std::string& project_name
      );

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

    private:

      boost::program_options::options_description _config_add_options;
      boost::program_options::options_description _config_add_pos_options;

  };
}
#endif

