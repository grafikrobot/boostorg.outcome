/* Unit testing for outcomes
(C) 2013-2022 Niall Douglas <http://www.nedproductions.biz/> (1 commit)


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

#include <boost/outcome/result.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_monitor.hpp>

BOOST_OUTCOME_AUTO_TEST_CASE(issues_0259_test, "move assignable is not calculated correctly")
{
  struct DefaultConstructibleMoveAssignable
  {
    int a;
    DefaultConstructibleMoveAssignable() = default;
    DefaultConstructibleMoveAssignable(int) {}
    DefaultConstructibleMoveAssignable(const DefaultConstructibleMoveAssignable &) = delete;
    DefaultConstructibleMoveAssignable(DefaultConstructibleMoveAssignable &&) = delete;
    DefaultConstructibleMoveAssignable &operator=(const DefaultConstructibleMoveAssignable &) = delete;
    DefaultConstructibleMoveAssignable &operator=(DefaultConstructibleMoveAssignable &&) noexcept { return *this; }
    ~DefaultConstructibleMoveAssignable() = default;
  };
  struct DefaultConstructibleCopyAssignable
  {
    int a;
    DefaultConstructibleCopyAssignable() = default;
    DefaultConstructibleCopyAssignable(int) {}
    DefaultConstructibleCopyAssignable(const DefaultConstructibleCopyAssignable &) = delete;
    DefaultConstructibleCopyAssignable(DefaultConstructibleCopyAssignable &&) = delete;
    DefaultConstructibleCopyAssignable &operator=(const DefaultConstructibleCopyAssignable &) { return *this; }
    DefaultConstructibleCopyAssignable &operator=(DefaultConstructibleCopyAssignable &&) = delete;
    ~DefaultConstructibleCopyAssignable() = default;
  };
  struct NonDefaultConstructibleMoveAssignable
  {
    int a;
    NonDefaultConstructibleMoveAssignable() = delete;
    NonDefaultConstructibleMoveAssignable(int) {}
    NonDefaultConstructibleMoveAssignable(const NonDefaultConstructibleMoveAssignable &) = delete;
    NonDefaultConstructibleMoveAssignable(NonDefaultConstructibleMoveAssignable &&) = delete;
    NonDefaultConstructibleMoveAssignable &operator=(const NonDefaultConstructibleMoveAssignable &) = delete;
    NonDefaultConstructibleMoveAssignable &operator=(NonDefaultConstructibleMoveAssignable &&) noexcept { return *this; }
    ~NonDefaultConstructibleMoveAssignable() = default;
  };
  struct NonDefaultConstructibleCopyAssignable
  {
    int a;
    NonDefaultConstructibleCopyAssignable() = delete;
    NonDefaultConstructibleCopyAssignable(int) {}
    NonDefaultConstructibleCopyAssignable(const NonDefaultConstructibleCopyAssignable &) = delete;
    NonDefaultConstructibleCopyAssignable(NonDefaultConstructibleCopyAssignable &&) = delete;
    NonDefaultConstructibleCopyAssignable &operator=(const NonDefaultConstructibleCopyAssignable &) { return *this; }
    NonDefaultConstructibleCopyAssignable &operator=(NonDefaultConstructibleCopyAssignable &&) = delete;
    ~NonDefaultConstructibleCopyAssignable() = default;
  };

  BOOST_OUTCOME_V2_NAMESPACE::result<DefaultConstructibleMoveAssignable> test1(5), test1a(6);
  test1 = std::move(test1a);
  static_assert(!std::is_copy_constructible<BOOST_OUTCOME_V2_NAMESPACE::result<DefaultConstructibleMoveAssignable>>::value, "");
  static_assert(!std::is_move_constructible<BOOST_OUTCOME_V2_NAMESPACE::result<DefaultConstructibleMoveAssignable>>::value, "");
  static_assert(!std::is_copy_assignable<BOOST_OUTCOME_V2_NAMESPACE::result<DefaultConstructibleMoveAssignable>>::value, "");
  static_assert(std::is_move_assignable<BOOST_OUTCOME_V2_NAMESPACE::result<DefaultConstructibleMoveAssignable>>::value, "");
  static_assert(std::is_destructible<BOOST_OUTCOME_V2_NAMESPACE::result<DefaultConstructibleMoveAssignable>>::value, "");

  BOOST_OUTCOME_V2_NAMESPACE::result<DefaultConstructibleCopyAssignable> test2(5), test2a(6);
  test2 = test2a;
  static_assert(!std::is_copy_constructible<BOOST_OUTCOME_V2_NAMESPACE::result<DefaultConstructibleCopyAssignable>>::value, "");
  static_assert(!std::is_move_constructible<BOOST_OUTCOME_V2_NAMESPACE::result<DefaultConstructibleCopyAssignable>>::value, "");
  static_assert(std::is_copy_assignable<BOOST_OUTCOME_V2_NAMESPACE::result<DefaultConstructibleCopyAssignable>>::value, "");
  static_assert(std::is_move_assignable<BOOST_OUTCOME_V2_NAMESPACE::result<DefaultConstructibleCopyAssignable>>::value, "");
  static_assert(std::is_destructible<BOOST_OUTCOME_V2_NAMESPACE::result<DefaultConstructibleCopyAssignable>>::value, "");

  BOOST_OUTCOME_V2_NAMESPACE::result<NonDefaultConstructibleMoveAssignable> test3(5), test3a(6);
  static_assert(!std::is_copy_constructible<BOOST_OUTCOME_V2_NAMESPACE::result<NonDefaultConstructibleMoveAssignable>>::value, "");
  static_assert(!std::is_move_constructible<BOOST_OUTCOME_V2_NAMESPACE::result<NonDefaultConstructibleMoveAssignable>>::value, "");
  static_assert(!std::is_copy_assignable<BOOST_OUTCOME_V2_NAMESPACE::result<NonDefaultConstructibleMoveAssignable>>::value, "");
  static_assert(!std::is_move_assignable<BOOST_OUTCOME_V2_NAMESPACE::result<NonDefaultConstructibleMoveAssignable>>::value, "");
  static_assert(std::is_destructible<BOOST_OUTCOME_V2_NAMESPACE::result<NonDefaultConstructibleMoveAssignable>>::value, "");

  BOOST_OUTCOME_V2_NAMESPACE::result<NonDefaultConstructibleCopyAssignable> test4(5), test4a(6);
  static_assert(!std::is_copy_constructible<BOOST_OUTCOME_V2_NAMESPACE::result<NonDefaultConstructibleCopyAssignable>>::value, "");
  static_assert(!std::is_move_constructible<BOOST_OUTCOME_V2_NAMESPACE::result<NonDefaultConstructibleCopyAssignable>>::value, "");
  static_assert(!std::is_copy_assignable<BOOST_OUTCOME_V2_NAMESPACE::result<NonDefaultConstructibleCopyAssignable>>::value, "");
  static_assert(!std::is_move_assignable<BOOST_OUTCOME_V2_NAMESPACE::result<NonDefaultConstructibleCopyAssignable>>::value, "");
  static_assert(std::is_destructible<BOOST_OUTCOME_V2_NAMESPACE::result<NonDefaultConstructibleCopyAssignable>>::value, "");
}
