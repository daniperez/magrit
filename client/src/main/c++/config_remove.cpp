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
#include "config_remove.hpp"
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
magrit::config_remove::config_remove ( generic_command* previous_subcommand )
  : generic_command ( previous_subcommand )
{
}

/////////////////////////////////////////////////////////////////////////
const char*
magrit::config_remove::get_name() const
{
  return "rm"; 
} 

/////////////////////////////////////////////////////////////////////////
const char* magrit::config_remove::get_description() const
{
  return "Removes an existing repository from magrit's config";
}






