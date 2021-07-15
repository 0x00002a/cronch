Metadata 
===========

*Aka bootleg reflection* 


Introduction 
------------- 

cronch is mostly an interface for consuming metadata. The rest of its API is just layers on other libraries that do all the heavy
lifting. 

.. namespace:: cronch

Customisation points
---------------------

.. class:: template<typename T> metadata 

   This is the main customisation point of cronch. It is used to add a type to the introspection system and to thereby allow the 
   serializing and deserializing of it.

   The unspecialised form has no members and is completely empty. To make a type consumable by cronch, you must specialise 
   this struct for your type and define the needed members. See :ref:`metadata concepts` for more information. 

   **Possible fields**

   .. member:: meta::concepts::view members 
       
       Members of the type for use when serializing and deserializing. See :class:`meta::mems` for the provided concrete type 
       for this.

   .. member:: const char* name

       Name of the type used by some serializers and deserializers (e.g. :class:`xml::pugi`)

   **Example**

   .. literalinclude:: /code/metadata_use_example.cpp 
    

Helper macros 
-------------

.. c:macro:: CRONCH_META_TYPE(type, fields)

   Shortcut for generating :class:`metadata\<type\>`. The name is set to ``type`` while the fields are passed to :class:`meta::mems` 
   and set as the value of `metadata\<type\>::members`.

   **Example** 

   .. literalinclude:: /code/metadata_meta_type.cpp

.. c:macro:: CRONCH_META_TYPE(type)

   Same as CRONCH_META_TYPE(type, fields) but without the fields 


**Important Note**

These macros must be used either in the ``cronch`` namespace or outside of any namespaces.

.. namespace-push:: meta


Registration helpers
--------------------

.. class:: template<typename T, typename V> field 

   Holds a field belonging to T with type V. 

   **Constructors**

   .. function:: constexpr field::field(std::string_view name, V T::* n)


.. class:: template<typename T, typename R, typename S> \ 
            property

    Holds a property of T, the value of which is set via setter and obtained
    via getter. serializing a type requires all getters for it to be non-null 
    while deserializing it requires all setters to be non-null


   **Constructors**

    .. function:: constexpr property::property(std::string_view name, R(T::* getter) = nullptr, void(T::* setter) = nullptr) 

    .. function:: constexpr property::property(std::string_view name, R(* getter)(const T* ) = nullptr, void(* setter)(T* , S) = nullptr) 
    
    .. function:: constexpr property::property(std::string_view name, R(* getter)(const T&) = nullptr, void(* setter)(T&, S) = nullptr) 

.. class:: template<typename... Fs> mems

   Holds the members of a type (which may be `property` or :class:`field`'s)

   **Constructors**

   .. function:: constexpr mems::mems(Fs... fs) 
        

.. namespace-push:: concepts

Note
#######

All these types are simply implementations of concepts in :ref:`metadata concepts` that are provided to make cronch easier to use. There 
is nothing to stop you defining your own accessor types and using those instead of/in conjunction with the above.

Querying
--------------

.. function:: template<has_members T> const view auto& accessors() 

   Returns a const reference to a type satisfying `view` that refers to 
   the members of T satisfying :concept:`accessor`

.. function:: template<serializable T> std::string_view nameof()

   Returns the name of the type as registered via `metadata<T>`

.. namespace-pop::
.. namespace-pop::













