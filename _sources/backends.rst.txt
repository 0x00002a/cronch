Backends 
=========

All classes in this section satisfy :concept:`backend` unless otherwise specified


.. namespace:: cronch

.. namespace-push:: json

JSON 
-----

.. class:: nloh 

   Provides json support via `nlohammn/json <https://github.com/nlohmann/json>`_

   **Supported types**

   Since nlohmann/json already has its own mechanism for parsing user-defined types, cronch leverages that first before 
   falling back to its own metadata. If a type is not known to cronch *and* there is no nlohmann/json support for it 
   then it will cause a compile error, unless it satisfies :concept:`iterable`, in which case it will be iterated over 
   with the :expr:`nlohmann::json` instance being treated as an array.

   **Constructors**

   .. function:: nloh::nloh(const std::string& contents)

       Constructs the backend by parsing the contents string

   .. function:: nloh::nloh(nlohmann::json doc) 
       
       Constructs the backend by copying an existing parsed fragment



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
   to serialize types and :expr:`boost::lexical_cast<T>` to deserialize them. See the `docs <https://www.boost.org/doc/libs/1_76_0/doc/html/boost_lexical_cast/synopsis.html#boost_lexical_cast.synopsis.lexical_cast>`_ on lexical cast for more information.

   Types that are neither supported by lexical cast or have members known to cronch will cause a compile error, unless the type 
   is :concept:`iterable`, in which case it must have at least a name known by cronch (either a member name or type name).

   **Constructors**

   .. function:: pugi::pugi(const std::string& contents) 

        Constructs the backend by parsing the contents string 

   .. function:: pugi::pugi(pugi::xml_document doc) 

        Constructs the backend by copying the reference to an existing xml document


Building your own 
------------------

Making your own backend is mostly quite simple. Define a type which satisfies :concept:`backend` and use it. Done.

A very basic backend, which simply uses :expr:`boost::lexical_cast` to convert types, is shown below:

.. literalinclude:: ../../examples/custom_backend/plain.cpp

   

   





