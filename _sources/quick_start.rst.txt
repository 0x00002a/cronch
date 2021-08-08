Quick start 
===========

.. namespace:: cronch

Making your type usable with cronch 
------------------------------------

In order to use your type, cronch needs some metadata. The easiest way to provide this is with :c:macro:`CRONCH_META_TYPE` macro. 
It is also possible to add your type via templates (which is what the macro generates). See :class:`metadata` for more information.



Serialization
-------------

Serialization is done via the :func:`serialize` function. 



.. function:: template<concepts::serialization_backend Backend, typename T> \
                std::string serialize(const T& obj) 

             Same as the overload taking document_type, but default constructs the document_type.

             **Additional Requirments**

             - ``(const Backend::document_type& doc) { Backend::to_string(doc) } -> std::string``

.. function:: template<concepts::serialization_backend Backend, typename T> \
              void serialize(const T& obj, Backend::document_type& doc)
    
    Reads a type into the document_type of the specified backend

Deserialzation
--------------

Deserialization is done via the :func:`deserialize` function. 

.. function:: template<typename T, concepts::deserizalation_backend Backend> \
                T deserialize(const Backend& from)

             Parse a type from the data stored by the backend *from*. Default constructs T


.. function:: template<typename T, concepts::deserizalation_backend Backend> \ 
            void deserialize(const Backend& from, T& to)

         Same as the overload that returns T, except it parses it into *to* instead.


An example
-----------

.. literalinclude:: ../../examples/serialization/basic_json.cpp
   :language: cpp


Registering types
------------------

The registration of types is done via :class:`metadata`. See the associated page for detailed information.


Naming your type 
#################

To define a name for your type, add a field: ``constexpr static const char* name = "my name"`` to your specialisation of metadata.


Registering fields 
###################

Fields are added via :class:`meta::field`. For each field you want cronch to be aware of, add a 
``cronch::meta::field("name", &my_type::field_name)`` to the :expr:`metadata::members` field in your specialisation of metadata. 


Registering properties 
#######################

Properties refer to members of your type which are accessed through get/set pairs. cronch supports these via the :class:`meta::property` 
type. Add it to the members field of your metadata specialisation.


Properties example 
###################

.. literalinclude:: ../../examples/props.cpp






