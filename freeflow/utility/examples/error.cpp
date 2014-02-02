// Copyright (c) 2013-2014 Flowgrammable, LLC.
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at:
// 
// http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an "AS IS"
// BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied. See the License for the specific language governing
// permissions and limitations under the License.

#include <iostream>
#include <fstream>

#include <freeflow/utility/error.hpp>

using namespace std;
using namespace freeflow;

Error expect_success() { return Error::SUCCESS; }
Error expect_failure() { return Error::FAILURE; }

int 
main(int argc, char* argv[]) {
  // An error code will be contextually converted to true if the
  // result indicates suceess. Functions returning error codes
  // can be used in if statements like this:
  if (expect_success())
    cout << "success\n";
  
  // An error code will be contextually converted to false if the
  // result indicates any kind of error. Functions returning error
  // codes can also be used this way:
  if (not expect_failure())
    cout << "failure\n";

  // A Trap object is used to capture a failure as a declaration
  // in an if statement. For example, if we want to know the
  // exact kind of error returned by the expect_failure, we can
  // do the this. The trapped error is contextually converted to
  // true if it represents an error, allowing the program to use
  // actually use the result.
  if (Trap err = expect_failure())
    cout << "trapped failure\n";
}

