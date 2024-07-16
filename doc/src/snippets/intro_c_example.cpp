/* Example of Outcome
(C) 2024 Niall Douglas <http://www.nedproductions.biz/>


Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include "../../../include/boost/outcome/experimental/result.h"

#include <stdio.h>

void use_string(const char *foo)
{
  (void) foo;
}

//! [signature]
BOOST_OUTCOME_C_DECLARE_RESULT_SYSTEM(result_string, const char *)

BOOST_OUTCOME_C_RESULT_SYSTEM(result_string) data_from_file(const char *path);
//! [signature]

int main()
{
  //! [inspect]
  BOOST_OUTCOME_C_RESULT_SYSTEM(result_string) rslt = data_from_file("config.cfg");
  if(BOOST_OUTCOME_C_RESULT_HAS_VALUE(rslt))
    use_string(rslt.value);  // returns string
  else
    fprintf(stderr, "%s\n", outcome_status_code_message(&rslt.error));
  //! [inspect]
}

//! [implementation]
BOOST_OUTCOME_C_DECLARE_RESULT_SYSTEM(result_int, int)

BOOST_OUTCOME_C_RESULT_SYSTEM(result_int) process(const char *content);

BOOST_OUTCOME_C_RESULT_SYSTEM(result_int) int_from_file(const char *path)
{
  BOOST_OUTCOME_C_RESULT_SYSTEM_TRY(const char *str, result_int, /* cleanup on fail */, data_from_file(path));
  // if control gets here data_from_file() has succeeded
  return process(str);  // decltype(str) == string
}
//! [implementation]

BOOST_OUTCOME_C_RESULT_SYSTEM(result_int) process(const char *foo)
{
  (void) foo;
  return outcome_make_result_system_result_int_success(1);
}

BOOST_OUTCOME_C_RESULT_SYSTEM(result_string) data_from_file(const char *path)
{
  (void) path;
  return outcome_make_result_system_result_string_success("");
}
