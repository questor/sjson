#ifndef _json_hpp__
#define _json_hpp__

// Copyright (c) 2012, Andre Caron (andre.l.caron@gmail.com)
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/*!
 * @file json.h
 * @brief C++ interface to the sJSON library.
 */

#include "sjson.h"
#include "eastl/string.h"
#include "eastl/vector.h"
#include "eastl/extra/murmurhash.h"

namespace json {

/*!
 * @brief Dynamically typed value.
 *
 * @note Instances of this class must be entirely scoped within the
 *  lifetime of the root @c Document object from which they are extracted.
 */
class Any {
private:
   ::sJSON * myData;

   /* construction. */
public:
   /*!
    * @internal
    * @brief Wraps the underlying implementation.
    * @param data Handle to the JSON data structure.
    */
   explicit Any(::sJSON * data)
      : myData(data)
   {}

   /* methods. */
public:
   /*!
    * @internal
    * @brief Access the underlying implementation.
    * @return Handle to the JSON data structure.
    */
   ::sJSON *data() const {
      return myData;
   }

   /*!
    * @brief Checks if the value is null.
    * @return @c true if the value is null, else @c false.
    */
   bool isNull() const {
      return (myData->type == sJSON_NULL);
   }

   /*!
    * @brief Checks if the value is a boolean.
    * @return @c true if the value is a boolean, else @c false.
    *
    * @see operator bool()
    */
   bool isBool() const {
      return ((myData->type == sJSON_True) || (myData->type == sJSON_False));
   }

   /*!
    * @brief Checks if the value is a number.
    * @return @c true if the value is a number, else @c false.
    *
    * @note There is no way to determine if the value is an integer or a
    *  real number.  The application should determine if it can deal with
    *  real values or if it only accepts integers.
    *
    * @see operator int()
    * @see operator double()
    */
   bool isNumber() const {
      return (myData->type == sJSON_Number);
   }

   /*!
    * @brief Checks if the value is a string.
    * @return @c true if the value is a string, else @c false.
    *
    * @see operator eastl::string()
    */
   bool isString() const {
      return (myData->type == sJSON_String);
   }

   /*!
    * @brief Checks if the value is a list.
    * @return @c true if the value is a list, else @c false.
    *
    * @see Array::Array(const Any&)
    */
   bool isArray() const {
      return (myData->type == sJSON_Array);
   }

   /*!
    * @brief Checks if the value is a map.
    * @return @c true if the value is a map, else @c false.
    *
    * @see Map::Map(const Any&)
    */
   bool isMap() const {
      return (myData->type == sJSON_Object);
   }

   /* operators. */
public:
   /*!
    * @brief Interpret the value as a boolean.
    * @return The underlying boolean value.
    *
    * @pre isBool()
    */
//   operator bool() const {
//      switch (myData->type) {
//      case sJSON_False: {
//         return (false);
//      }
//      case sJSON_True: {
//         return (true);
//      }
//      default: {
//         XASSERT(0, "bool-operator used, but json-object is not bool");
//      }
//      }
//   }
   bool asBool() const {
      switch (myData->type) {
      case sJSON_False: {
         return (false);
      }
      case sJSON_True: {
         return (true);
      }
      default: {
         XASSERT(0, "bool-operator used, but json-object is not bool");
      }
      }
   }

   /*!
    * @brief Interpret the value as an integer.
    * @return The underlying integer value.
    *
    * @pre isNumber()
    *
    * @see operator double()
    */
//   operator int() const {
//      XASSERT(isNumber(), "operator int used on non int json object");
//      return (myData->valueInt);
//   }
   int asInt() const {
      XASSERT(isNumber(), "operator int used on non int json object");
      return (myData->valueInt);
   }

   /*!
    * @brief Interpret the value as a real number.
    * @return The underlying real number.
    *
    * @pre isNumber()
    *
    * @see operator int()
    */
//   operator double() const {
//      XASSERT(isNumber(), "operator double used on non double json object");
//      return (myData->valueDouble);
//   }
   double asDouble() const {
      XASSERT(isNumber(), "operator double used on non double json object");
      return (myData->valueDouble);
   }

   /*!
    * @brief Interpret the value as a string.
    * @return The underlying string.
    *
    * @pre isString()
    */
//   operator eastl::string() const {
//      XASSERT(isString(), "operator string used on non string json object");
//      return (myData->valueString);
//   }

   eastl::string asString() const {
      XASSERT(isString(), "operator string used on non string json object");
      return myData->valueString;
   }
};

/*!
 * @brief Parser, placeholder for document root object.
 *
 * @note Instances of this class must outlive the lifetime of @c Any, @c
 *  Array and @c Map objects extracted from it.
 */
class Document {
   /* class methods. */
private:
   /*!
    * @internal
    * @brief Parse the JSON document in @a text.
    * @param text Serialized JSON document.
    * @return A handle to the JSON data structure.
    */
   static ::sJSON* parse(const eastl::string& text) {
      ::sJSON *const root = ::sJSONparse(text.c_str());
      XASSERT(root != nullptr, "json parse error!");
      // sJSONgetErrorPtr()
      return root;
   }

   /* data. */
private:
   ::sJSON *const myData;

   /* construction. */
public:
   /*!
    * @brief Parse the JSON document in @a text.
    * @param text Serialized JSON document.
    */
   explicit Document(const eastl::string& text)
      : myData(parse(text.c_str()))
   {}

private:
   Document(const Document&);

public:
   /*!
    * @brief Release the memory held by the underlying data structure.
    */
   ~Document() {
      ::sJSONdelete(myData);
   }

   /* methods. */
public:
   /*!
    * @internal
    * @brief Access the underlying implementation.
    * @return Handle to the JSON data structure.
    *
    * @note The data may be a list or a map.
    */
   ::sJSON* data() const {
      return (myData);
   }

   /*!
    * @brief Checks if the root object is an array.
    *
    * @see isMap()
    * @see Array(Document&)
    */
   bool isArray() const {
      return (myData->type == sJSON_Array);
   }

   /*!
    * @brief Checks if the root object is a map.
    *
    * @see isArray()
    * @see Map(Document&)
    */
   bool isMap() const {
      return (myData->type == sJSON_Object);
   }

   /* operators. */
private:
   Document& operator= (const Document&);
};

/*!
 * @brief Ordered group of values.
 *
 * @note Instances of this class must be entirely scoped within the
 *  lifetime of the root @c Document object from which they are extracted.
 */
class Array {
   /* data. */
private:
   ::sJSON * myData;

   /* construction. */
public:
   /*!
    * @internal
    * @brief Wraps the underlying implementation.
    * @param data Handle to the JSON data structure.
    *
    * @pre data->type == sJSON_Array
    */
   explicit Array(::sJSON* data)
      : myData(data)
   {
      XASSERT(myData->type == sJSON_Array, "json object is not an array");
   }

   /*!
    * @brief Extract the root object of @a document as an array.
    * @param document Document who'se root object we're interested in.
    *
    * @pre The document's root object is a list.
    *
    * @see Map(Document&)
    */
   explicit Array(Document& document)
      : myData(document.data())
   {
      XASSERT(myData->type == sJSON_Array, "json object is not an array");
   }

   /*!
    * @brief Converts from a dynamically typed value.
    * @param object Value to interpret as an @c Array.
    *
    * @pre object.isList() == true
    */
   Array(const Any& object)
      : myData(object.data())
   {
      XASSERT(myData->type == sJSON_Array, "json object is not an array");
   }

   /* methods. */
public:
   /*!
    * @internal
    * @brief Access the underlying implementation.
    * @return Handle to the JSON data structure.
    */
   ::sJSON* data() const {
      return myData;
   }

   /*!
    * @brief Obtain the number of items in the array.
    * @return Number of items in the array.
    */
   uint_t size() const {
      return ::sJSONgetArraySize(myData);
   }

   /* operators. */
public:
   /*!
    * @brief Convert to dynamically typed value.
    */
   operator Any() const {
      return (Any(myData));
   }

   /*!
    * @brief Access a field by position.
    * @param key Position of the field to extract.
    * @return The field value.
    */
   Any operator[](int key) const
   {
      ::sJSON *const item = ::sJSONgetArrayItem(myData, key);
      XASSERT(item != 0, "json array item not found");
      return (Any(item));
   }
};

/*!
 * @brief Group of named values.
 *
 * @note Instances of this class must be entirely scoped within the
 *  lifetime of the root @c Document object from which they are extracted.
 */
class Map {
   /* data. */
private:
   ::sJSON* myData;

   /* construction. */
public:
   /*!
    * @internal
    * @brief Wraps the underlying implementation.
    * @param data Handle to the JSON data structure.
    *
    * @pre data->type == sJSON_Object
    */
   explicit Map(::sJSON * data)
      : myData(data)
   {
      XASSERT(myData->type == sJSON_Object, "json object is not map");
   }

   /*!
    * @brief Extract the root object of @a document as a map.
    * @param document Document who'se root object we're interested in.
    *
    * @pre The document's root object is a map.
    *
    * @see Map(Document&)
    */
   explicit Map(Document& document)
      : myData(document.data())
   {
      XASSERT(myData->type == sJSON_Object, "json object is not map");
   }

   /*!
    * @brief Converts from a dynamically typed value.
    * @param object Value to interpret as a @c Map.
    *
    * @pre object.isMap() == true
    */
   Map(const Any& object)
      : myData(object.data())
   {
      XASSERT(myData->type == sJSON_Object, "json object is not map");
   }

   /* operators. */
public:
   /*!
    * @internal
    * @brief Access the underlying implementation.
    * @return Handle to the JSON data structure.
    */
   ::sJSON* data() const {
      return myData;
   }

   /*!
    * @brief Convert to dynamically typed value.
    */
   operator Any() const {
      return (Any(myData));
   }

   /*!
    * @brief Access a field by name.
    * @param key Name of the field to extract.
    * @return The field value.
    */
   Any operator[](const eastl::FixedMurmurHash key) const
   {
      ::sJSON *const item = ::sJSONgetObjectItem(myData, key);
      XASSERT(item != 0, "json map item not found");
      return (Any(item));
   }

   bool hasMapMember(uint32_t nameHash) const {
      if(sJSONgetObjectItem(myData, nameHash) != nullptr)
         return true;
      return false;
   }
   bool hasMapMember(eastl::FixedMurmurHash nameHash) const {
      if(sJSONgetObjectItem(myData, nameHash) != nullptr)
         return true;
      return false;
   }

};

//    // Forward declared.
//    std::ostream& operator<< (std::ostream& stream, const Any& value);

//    /*!
//     * @brief Serialize @a list.
//     * @param stream The output stream.
//     * @param list The value to serialize.
//     * @return @a stream
//     */
//    std::ostream& operator<< (std::ostream& stream, const List& list)
//    {
//        stream << '[';
//        for (int i=0; (i < list.size()-1); ++i) {
//            stream << list[i] << ',';
//        }
//        if (list.size() > 0) {
//            stream << list[list.size()-1];
//        }
//        return (stream << ']');
//    }

//    /*!
//     * @brief Serialize @a map.
//     * @param stream The output stream.
//     * @param map The value to serialize.
//     * @return @a stream
//     */
//    std::ostream& operator<< (std::ostream& stream, const Map& map)
//    {
//        stream << '{';
//        ::sJSON * node = map.data()->child;
//        for (; (node != 0); node = node->next)
//        {
//            stream
//                << "\"" << node->string << "\":" << Any(node);
//            if (node->next != 0) {
//                stream << ",";
//            }
//        }
//        return (stream << '}');
//    }

//    /*!
//     * @brief Serialize @a value.
//     * @param stream The output stream.
//     * @param value The value to serialize.
//     * @return @a stream
//     *
//     * @bug Boolean values output depends on @c std::boolalpha and locale
//     *  settings.  They should always be serialized as @c "true" and @c
//     *  "false".
//     * @bug Double quotes are not escaped in strings.
//     */
//    std::ostream& operator<< (std::ostream& stream, const Any& value)
//    {
//        if (value.is_null()) {
//            return (stream << "null");
//        }
//        if (value.is_bool()) {
//            // TODO: boolalpha?
//            return (stream << bool(value));
//        }
//        if (value.is_number()) {
//            return (stream << double(value));
//        }
//        if (value.is_string()) {
//            // TODO: escape double quotes.
//            return (stream << '"' << std::string(value) << '"');
//        }
//        if (value.is_list()) {
//            return (stream << List(value));
//        }
//        if (value.is_map()) {
//            return (stream << Map(value));
//        }
//        return (stream);
//    }

}

#endif /* _json_hpp__ */
