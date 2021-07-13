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

   **Example**

   .. literalinclude:: /code/metadata_use_example.cpp 
    

Helper macros 
-------------

.. c:macro:: CRONCH_META_TYPE(type, fields)

   Shortcut for generating :class:`metadata<type>`. The name is set to *type* while the fields are passed to cronch::meta::fields 

   **Example** 

   .. literalinclude:: /code/metadata_meta_type.cpp

.. c:macro:: CRONCH_META_TYPE(type)

   Same as CRONCH_META_TYPE(type, fields) but without the fields 


.. namespace-push:: meta


Registration points
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
    

.. class:: template<typename... Fs> mems

   Holds the members of a type (which may be :class:`property` or :class:`field`'s)

   **Constructors**

   .. function:: constexpr mems::mems(Fs... fs) 
        

Querying
--------------

.. function:: template<has_members T> const concepts::view auto& accessors() 

   Returns a const reference to a type satisfying `view` that refers to 
   the members of T satisfying :concept:`accessor`

.. function:: template<serializable T> std::string_view nameof()

   Returns the name of the type as registered via `metadata<T>`

.. namespace-pop::













