
.. namespace:: cronch

Backends 
=========

All classes in this section satisfy :concept:`concepts::backend` unless otherwise specified



.. namespace-push:: json

JSON 
-----

.. class:: nloh 

   Provides json support via `nlohammn/json <https://github.com/nlohmann/json>`_

   **Supported types**

   Since nlohmann/json already has its own mechanism for parsing user-defined types, cronch leverages that first before 
   falling back to its own metadata. If a type is not known to cronch *and* there is no nlohmann/json support for it 
   then it will cause a compile error, unless it satisfies :concept:`concepts::iterable`, in which case it will be iterated over 
   with the :expr:`nlohmann::json` instance being treated as an array.

   **Supported formats**

   This backend supports any json supported by nlohmann/json, in addition to any binary formats it supports 
   (currently cbor, msgpack, bson, and ubjson). Support for the binary formats is done via the :func:`nloh::nloh` overload which takes a 
   ``nlohmann::json`` (allowing stuff such as :expr:`nlohmann::json::from_bson` to be passed) and the :func:`serialize` overload which 
   takes a ``Backend::document_type&``, allowing the caller full access to the underlying ``nlohamnn::json``.

   **Constructors**

   .. function:: nloh::nloh(const std::string& contents)

       Constructs the backend by parsing the contents string

   .. function:: nloh::nloh(nlohmann::json doc) 
       
       Constructs the backend by copying an existing parsed fragment


.. class:: boost 

   Provides json support via `Boost.JSON <https://www.boost.org/doc/libs/1_76_0/libs/json/doc/html/index.html>`_

   It is similar to the nlohmann backend, but due Boost.JSON's lack of support for user-defined types, it adds its own system 
   for conversion of them.

   **Supported types** 

   All of Boost.JSON's native types (object, array, string, etc). For additional type support, the `converter` struct is provided

   .. class:: template<typename T> converter

        Provides a customisation point for user-defined type support. Specialise it to add support. Specialisations are provided for 
        the following out of the box:

        - std::string 
        - anything satisfying std::intergral

        .. function:: void to_json(boost::json::value& doc, const T& v) 

            Convert the type to a json value 
        
        .. function:: void from_json(const boost::json::value& doc, T& v) 
            
            Convert the type from a json value 

        **Example** 

        .. literalinclude:: /code/boost_json_example.cpp

.. namespace-pop::

.. namespace-push:: xml

XML
----

.. class:: pugi 

   Provides xml support via `pugixml <https://pugixml.org>`_ 

   The default version uses a node-per-value format. With the name of each node being either the name of the type (if from a 
   type without members metadata or the top level node) or the name of the member it references. 

   **Supported types**

   pugixml does not provide any way of parsing user defined types automatically. Instead it uses purely :expr:`const char*` 
   for its values. Therefore, in order to successfully wrap pugi, cronch uses :expr:`boost::lexical_cast<std::string>()` 
   to serialize types and :expr:`boost::lexical_cast\<T\>()` to deserialize them. See the `docs <https://www.boost.org/doc/libs/1_76_0/doc/html/boost_lexical_cast/synopsis.html#boost_lexical_cast.synopsis.lexical_cast>`_ on lexical cast for more information.

   Types that are neither supported by lexical cast or have members known to cronch will cause a compile error, unless the type 
   is :concept:`concepts::iterable`, in which case it must have at least a name known by cronch (either a member name or type name).


   **XML structure**


   The output (and expected input) xml is of the form:
   ``<typename>{inner}</typename>`` where ``typename`` is the name of the type
   passed to `pugi::serialize_to` and ``{inner}`` is the result of serializing
   the inner members of that type. It will be one of the following (uses first matching, top to bottom):

   .. namespace:: ::cronch::concepts

   .. list-table::
      :widths: 7 25 
      :header-rows: 1 

      * 
        - Satisfied concepts 
        - Output 
      * 
        - `has_members` 
        - ``<member name>{inner}</member name>`` for each member 
      * 
        - `ostreamable`
        - Result of :expr:`boost::lexical_cast<std::string>(v)` where ``v`` is the value 
      * 
        - `iterable`
        - ``<0..n>{inner}</0..n>`` for each element 0 to n where n is the size of the iterable - 1

   .. namespace-pop::

   **Example**

   .. code-block:: cpp

       struct mytype { 
          int i;
          std::vector<std::string> elements;
       };
       const auto myvalue = mytype{ .i = 2, .elements = { "Hello", "World" } };

   Here ``myvalue`` would be formatted as:

   .. code-block:: xml 

       <mytype>
           <i>2</i>
           <elements>
               <0>Hello</0>
               <1>World</1>
           </elements>
       </mytype>


   **Constructors**

   .. function:: pugi::pugi(const std::string& contents) 

        Constructs the backend by parsing the contents string 

   .. function:: pugi::pugi(pugi::xml_document doc) 

        Constructs the backend by copying the reference to an existing xml document



Building your own 
------------------

Making your own backend is mostly quite simple. Define a type which satisfies :concept:`concepts::backend` and use it. Done.

A very basic backend, which simply uses :expr:`boost::lexical_cast` to convert types, is shown below:

.. literalinclude:: ../../examples/custom_backend/plain.cpp

   

   





