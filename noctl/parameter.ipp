// Copyright (c) 2013-2014 Flowgrammable.org
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


namespace cli {

/// Returns the name of the parameter. The name contains both the
/// alias and the full name of the parameter
Name Parameter::name() {
  return name_;
}

std::string Parameter::doc() {
  return doc_;
}

json::Value operator()(const std::string& s) const {
  if(s == "true" or s == "yes" or s == "on"){
    return json::Value(json::Bool(true));
  }

  else if(s == "false" or s == "no" or s == "off"){
    return json::Value(json::Bool(false));
  }
  else throw runtime_error(s + " is an invalid value. Expected a value of type Bool");
}

json::Value operator()(const std::string& s) const {
  double d;
  stringstream ss(s);
  if(ss >> d) return json::Value(json::Real(d));
  else throw runtime_error(s + " is an invalid value. Expected a numerical value of type Real");
}

json::Value operator()(const std::string& s) const {
  if(s == "NULL" or s == "null" or s=="") return json::Value(json::Null());
  else return T{}(s);
}

} // namespace cli
