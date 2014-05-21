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
#include <string>

#include <freeflow/sys/print.hpp>
#include <freeflow/sys/json.hpp>

namespace freeflow {

// Print primitive data types.
void Printer::print(json::Null n) {
	os_ << n;
}	

void Printer::print(json::Bool b) {
	os_ << b;
}

void Printer::print(json::Int i) {
	os_ << i;
}

void Printer::print(json::Real r) {
	os_ << r;
}

void Printer::print(const json::String& s){
	os_ << s;
}

// After '{' increment indent_, add spaces to tab and insert newline.
void Printer::start_object() {
	++indent_; 
	tab_  += "   ";
	os_ << "\n" << tab_;
}

void Printer::end_object() {
	--indent_;
	// Remove spaces from tab_
}

// Print object
void Printer::print(const json::Object& o) {

}

} // freeflow

