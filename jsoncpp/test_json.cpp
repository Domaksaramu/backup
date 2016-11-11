/*#include <cstdio>
#include <cstring>
#include <iostream>
// This is the JSON header
#include "json/json.h"

using namespace std;

int main(int argc, char **argv)
{
string json_example = "{\"array\": \
[\"item1\", \
\"item2\"], \
\"not an array\": \
\"asdf\" \
}";
// Let's parse it  
Json::Value root;
Json::Reader reader;
bool parsedSuccess = reader.parse(json_example, root, false);

if(not parsedSuccess)
{
// Report failures and their locations 
// in the document.
std::cout<<"Failed to parse JSON"<<endl<<reader.getFormatedErrorMessages()<<endl;
return 1;
}
// Let's extract the array contained 
//  // in the root object

const Json::Value array = root["array"];

// Iterate over sequence elements and 
//  // print its values
for(unsigned int index=0; index<array.size(); 
++index)  
{		std::cout<<"Element " 
<<index 
<<" in array: "
<<array[index].asString()
<<endl;
}

// Lets extract the not array element 
//  // contained in the root object and 
//   // print its value
const Json::Value notAnArray = 
root["not an array"];

if(not notAnArray.isNull())
{
std::cout<<"Not an array: "
<<notAnArray.asString()
<<endl;
}

// If we want to print JSON is as easy as doing:
std::cout<<"Json Example pretty print: "
<<endl<<root.toStyledString()<<endl;

return 0;
}*/

#include <iostream>
#include "json/json.h"

std::string str;

int main(){
	Json::Value root;
	root["id"] = "Luna";
	root["name"] = "Silver";
	root["age"] = 19;
	root["hasCar"] = false;

	Json::Value items;
	items.append("nootbook");
	items.append("ipadmini2");
	items.append("iphone5s");
	root["items"] = items;

	Json::Value friends;
	Json::Value tom;
	tom["name"] = "Tom";
	tom["age"] = 21;
	Json::Value jane;
	jane["name"] = "jane";
	jane["age"] = 23;
	friends.append(tom);
	friends.append(jane);
	root["friends"] = friends;

	Json::StyledWriter writer;
	str = writer.write(root);
	std::cout << str << std::endl << std::endl;
}
