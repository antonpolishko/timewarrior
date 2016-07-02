////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2015 - 2016, Paul Beckingham, Federico Hernandez.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// http://www.opensource.org/licenses/mit-license.php
//
////////////////////////////////////////////////////////////////////////////////

#include <cmake.h>
#include <commands.h>
#include <timew.h>
#include <iostream>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////
int CmdTag (
  const CLI& cli,
  Rules& rules,
  Database& database)
{
  // Gather IDs and TAGs.
  std::vector <int> ids;
  std::vector <std::string> tags;
  for (auto& arg : cli._args)
  {
    if (arg.hasTag ("ID"))
      ids.push_back (strtol (arg.attribute ("value").c_str (), NULL, 10));

    if (arg.hasTag ("TAG"))
      tags.push_back (arg.attribute ("raw"));
  }

  // Load the data.
  // Note: There is no filter.
  Interval filter;
  auto tracked = getTracked (database, rules, filter);

  // Apply tags to ids.
  for (auto& id : ids)
  {
    if (id <= static_cast <int> (tracked.size ()))
    {
      // Note: It's okay to subtract a one-based number from a zero-based index.
      Interval i = tracked[tracked.size () - id];

      for (auto& tag : tags)
        i.tag (tag);

      validateInterval (database, rules, i);
      database.modifyInterval (tracked[tracked.size () - id], i);

      // Feedback.
      if (rules.getBoolean ("verbose"))
      {
        std::cout << "Added";
        for (auto& tag : tags)
          std::cout << ' ' << quoteIfNeeded (tag);
        std::cout << " to @" << id << '\n';
      }
    }
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
